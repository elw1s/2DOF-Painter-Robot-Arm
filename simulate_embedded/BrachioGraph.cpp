#include <iostream>
#include <random>
#include <cmath>
#include <nlohmann/json.hpp>
#include <opencv2/opencv.hpp>
#include <ctime>
#include "xtensor/xbuilder.hpp"
#include <fstream>
#include <stdexcept>
#include "utils.cpp"
#include <PillowResize/PillowResize.hpp>

namespace BrachioGraph{
const std::string export_path = "images/out.svg";
const std::string svg_folder = "images/";
const std::string json_folder = "images/";

void linesToFile(const std::vector<std::vector<std::pair<int, int>>>& lines, const std::string& filename) {
    nlohmann::json jsonLines = nlohmann::json::array();

    for (const auto& line : lines) {
        nlohmann::json jsonLine = nlohmann::json::array();
        for (const auto& point : line) {
            jsonLine.push_back({point.first, point.second});
        }
        jsonLines.push_back(jsonLine);
    }

    std::ofstream fileToSave(filename);
    fileToSave << std::setw(4) << jsonLines << std::endl;
}

// Direction vectors as std::pair<int, int>
const std::pair<int, int> E = {1, 0};   // East
const std::pair<int, int> S = {0, 1};   // South
const std::pair<int, int> SE = {1, 1};  // Southeast
const std::pair<int, int> NE = {1, -1}; // Northeast

// Function to get lines in a specific direction
std::vector<std::vector<std::pair<int, int>>> get_lines(cv::Mat& image, const std::string& scan, const std::pair<int, int>& direction, int line_spacing, int level) {
    std::vector<std::vector<std::pair<int, int>>> lines;
    cv::Mat pixels = image;

    int width = image.cols;
    int height = image.rows;
    int i_start = 0;
    int j_start = 0;

    int i_range;

    if (scan == "y") {
        i_range = height;
    }
    else if (scan == "x") {
        i_range = width;
        if (direction == SE) {
            i_start = line_spacing;
        }
        else if (direction == NE) {
            i_start = line_spacing - ((height - 1) % line_spacing);
            j_start = height - 1;
        }
    }

    std::vector<std::pair<int,int>> tempVec;

    for (int i = i_start; i < i_range; i += line_spacing) {
        std::pair<int, int> start_point;

        if (scan == "y") {
            int x = j_start;
            int y = i;
            std::pair<int, int> end_point;

            while (x >= 0 && x < width && y >= 0 && y < height) {
                if (!start_point.first && !start_point.second) {
                    if (pixels.at<uchar>(y, x) < level) {
                        start_point.first = x;
                        start_point.second = y;
                    }
                }
                else {
                    if (pixels.at<uchar>(y, x) >= level) {
                        end_point.first = x;
                        end_point.second = y;
                        tempVec.push_back(start_point);
                        tempVec.push_back(end_point);
                        lines.push_back(tempVec);
                        tempVec.clear();
                        start_point.first = start_point.second = 0;
                    }
                }

                end_point.first = x;
                end_point.second = y;
                x += direction.first;
                y += direction.second;
            }

            if (start_point.first && start_point.second) {
                tempVec.push_back(start_point);
                tempVec.push_back(end_point);
                lines.push_back(tempVec);
                tempVec.clear();
            }
        }
        else if (scan == "x") {
            int x = i;
            int y = j_start;
            std::pair<int, int> end_point;

            while (x >= 0 && x < width && y >= 0 && y < height) {
                if (!start_point.first && !start_point.second) {
                    if (pixels.at<uchar>(y, x) < level) {
                        start_point.first = x;
                        start_point.second = y;
                    }
                }
                else {
                    if (pixels.at<uchar>(y, x) >= level) {
                        end_point.first = x;
                        end_point.second = y;
                        tempVec.push_back(start_point);
                        tempVec.push_back(end_point);
                        lines.push_back(tempVec);
                        tempVec.clear();
                        start_point.first = start_point.second = 0;
                    }
                }

                end_point.first = x;
                end_point.second = y;
                x += direction.first;
                y += direction.second;
            }

            if (start_point.first && start_point.second) {
                tempVec.push_back(start_point);
                tempVec.push_back(end_point);
                lines.push_back(tempVec);
                tempVec.clear();
            }
        }
    }

    // for (const auto& line : lines) {
    //     std::cout << "[(" << line[0].first << ", " << line[0].second << "), ("<< line[1].first << ", " << line[1].second << ")]," << std::endl;
    // }    

    return lines;
}

// Function to hatch an image
std::vector<std::vector<std::pair<int,int>>> hatch(cv::Mat image, int line_spacing = 16) {
    std::vector<std::vector<std::pair<int,int>>> lines;
    std::cout << "inside hatch" << std::endl;
    lines = get_lines(image, "y", E, line_spacing, 160);
    std::cout << "after get_lines" << std::endl;
    std::vector<std::vector<std::pair<int,int>>> returnedLines = get_lines(image, "x", S, line_spacing, 80);
    for(int i = 0; i < returnedLines.size(); i++){
        lines.push_back(returnedLines.at(i));
    }
    returnedLines = get_lines(image, "y", SE, line_spacing, 40);
    for(int i = 0; i < returnedLines.size(); i++){
        lines.push_back(returnedLines.at(i));
    }
    returnedLines = get_lines(image, "x", SE, line_spacing, 40);
    for(int i = 0; i < returnedLines.size(); i++){
        lines.push_back(returnedLines.at(i));
    }
    returnedLines = get_lines(image, "y", NE, line_spacing, 20);
    for(int i = 0; i < returnedLines.size(); i++){
        lines.push_back(returnedLines.at(i));
    }
    returnedLines = get_lines(image, "x", NE, line_spacing, 20);
    for(int i = 0; i < returnedLines.size(); i++){
        lines.push_back(returnedLines.at(i));
    }
    return lines;
}

cv::Mat resizeImage(const cv::Mat& image, int resolution, int divider = 1) {
    if (image.rows > 0) {
     std::cout << "Values of the first row in resizeImage (before): ";
     for (int x = 0; x < image.cols; ++x) {
         int value = image.at<uchar>(0, x);
         std::cout << value << " ";
     }
    std::cout << std::endl;
    }
    cv::Size newSize(int(resolution / divider), int(resolution / divider * image.rows / image.cols));
    //cv::Mat resizedImage = utils::resize(image, newSize);
    cv::Mat resizedImage = PillowResize::resize(image, newSize, 0);
    if (resizedImage.rows > 0) {
     std::cout << "Values of the first row in resizeImage (after): ";
     for (int x = 0; x < resizedImage.cols; ++x) {
         int value = resizedImage.at<uchar>(0, x);
         std::cout << value << " ";
     }
    }
    std::cout << std::endl;
    return resizedImage;
}

std::string makesvg(const std::vector<std::vector<std::pair<int, int>>>& lines) {
    std::cout << "Generating svg file..." << std::endl;

    // Calculate the width and height of the SVG
    int width = 0;
    int height = 0;
    for (const auto& line : lines) {
        for (const auto& point : line) {
            width = std::max(width, static_cast<int>(point.first * 0.5));
            height = std::max(height, static_cast<int>(point.second * 0.5));
        }
    }

    // Create the SVG content
    std::string out = "<svg xmlns=\"http://www.w3.org/2000/svg\" height=\"" + std::to_string(height) + "px\" width=\"" + std::to_string(width) + "px\" version=\"1.1\">\n";

    for (const auto& line : lines) {
        std::string points;
        for (const auto& point : line) {
            points += std::to_string(point.first * 0.5) + "," + std::to_string(point.second * 0.5) + " ";
        }
        out += "<polyline points=\"" + points + "\" stroke=\"black\" stroke-width=\"1\" fill=\"none\" />\n";
    }

    out += "</svg>";
    return out;
}

double squaredDistance(const std::pair<int, int>& point1, const std::pair<int, int>& point2) {
    int xdiff = point1.first - point2.first;
    int ydiff = point1.second - point2.second;
    return xdiff * xdiff + ydiff * ydiff;
}

std::vector<std::vector<std::pair<int, int>>> join_lines(std::vector<std::vector<std::pair<int, int>>>& lines, int closeness = 128) {
    std::vector<std::vector<std::pair<int, int>> > new_lines;
    std::vector<std::pair<int, int>> previous_line;

    for (auto& line : lines) {
        if (previous_line.empty()) {
            new_lines.push_back(line);
            previous_line = line;
        } else {
            int xdiff = std::abs(previous_line.back().first - line.front().first);
            int ydiff = std::abs(previous_line.back().second - line.front().second);
            double squaredDist = squaredDistance(previous_line.back(), line.front());

            if (squaredDist <= closeness) {
                previous_line.insert(previous_line.end(), line.begin(), line.end());
            } else {
                new_lines.push_back(line);
                previous_line = line;
            }
        }
    }

    std::cout << "Reduced " << lines.size() << " lines to " << new_lines.size() << " lines." << std::endl;
    lines = new_lines;

    return lines;
}

std::vector<std::vector<std::pair<int, int>>> connectdots(const std::vector<std::vector<std::pair<int, int>>>& dots) {
    std::cout << "Connecting contour points..." << std::endl;
    std::vector<std::vector<std::pair<int, int>>> contours;
    for (int y = 0; y < dots.size(); y++) {
        for (const std::pair<int, int>& point : dots[y]) {
            int x = point.first;
            int v = point.second;
            if (v > -1) {
                if (y == 0) {
                    contours.push_back({std::make_pair(x, y)});
                } else {
                    int closest = -1;
                    int cdist = 100;
                    for (const std::pair<int, int>& pointPrev : dots[y - 1]) {
                        int x0 = pointPrev.first;
                        int v0 = pointPrev.second;
                        if (std::abs(x0 - x) < cdist) {
                            cdist = std::abs(x0 - x);
                            closest = x0;
                        }
                    }

                    if (cdist > 3) {
                        contours.push_back({std::make_pair(x, y)});
                    } else {
                        int found = 0;
                        for (int i = 0; i < contours.size(); i++) {
                            if (contours[i].back() == std::make_pair(closest, y - 1)) {
                                contours[i].push_back(std::make_pair(x, y));
                                found = 1;
                                break;
                            }
                        }
                        if (found == 0) {
                            contours.push_back({std::make_pair(x, y)});
                        }
                    }
                }
            }
        }
        contours.erase(
            std::remove_if(contours.begin(), contours.end(), [y](const std::vector<std::pair<int, int>>& c) {
                return (c.back().second < y - 1) && (c.size() < 4);
            }),
            contours.end()
        );
    }
    return contours;
}

std::vector<std::vector<std::pair<int, int>>> getdots(const cv::Mat& image) {
    std::cout << "Getting contour points..." << std::endl;
    std::vector<std::vector<std::pair<int, int>>> dots;

    for (int y = 0; y < image.rows - 1; y++) {
        std::vector<std::pair<int, int>> row;
        for (int x = 1; x < image.cols; x++) {
            if (image.at<uchar>(y, x) == 255) {
                if (!row.empty()) {
                    if (x - row.back().first == row.back().second + 1) {
                        row.back().second = row.back().second + 1;
                    } else {
                        row.push_back(std::make_pair(x, 0));
                    }
                } else {
                    row.push_back(std::make_pair(x, 0));
                }
            }
        }
        dots.push_back(row);
    }

    return dots;
}

double distsum(const std::pair<int, int>& point1, const std::pair<int, int>& point2) {
    int x1 = point1.first;
    int y1 = point1.second;
    int x2 = point2.first;
    int y2 = point2.second;
    double distance = std::sqrt(std::pow(x1 - x2, 2) + std::pow(y1 - y2, 2));
    return distance;
}

cv::Mat find_edges(const cv::Mat& image) {
    std::cout << "Finding edges..." << std::endl;
    
    // Apply Gaussian blur
    cv::Mat blurred;
    cv::GaussianBlur(image, blurred, cv::Size(3, 3), 0);
    cv::imwrite("/home/arda/Desktop/CSE396/BrachioGraph/images/blurredCPP.jpg", blurred);


    // Apply Canny edge detection
    cv::Mat edges;
    cv::Canny(blurred, edges, 100, 200); // Adjust the parameters as needed
    cv::imwrite("/home/arda/Desktop/CSE396/BrachioGraph/images/edgesCPP.jpg", edges);


    // Threshold the edges
    cv::Mat thresholded;
    cv::threshold(edges, thresholded, 128, 255, cv::THRESH_BINARY);
    cv::imwrite("/home/arda/Desktop/CSE396/BrachioGraph/images/thresholdCPP.jpg", thresholded);


    return thresholded;
}

std::vector<std::vector<std::pair<int, int>>> sortlines(std::vector<std::vector<std::pair<int, int>>> lines) {
    std::cout << "Optimizing line sequence..." << std::endl;
    std::vector<std::vector<std::pair<int, int>> > clines = lines;
    std::vector<std::vector<std::pair<int, int>> > slines;
    slines.push_back(clines[0]);
    clines.erase(clines.begin());

    while (!clines.empty()) {
        std::vector<std::pair<int, int>> x;
        double s = 1000000;
        bool r = false;

        for (auto& l : clines) {
            double d = distsum(l[0], slines.back().back());
            double dr = distsum(l.back(), slines.back().back());

            if (d < s) {
                x = l;
                s = d;
                r = false;
            }


            if (dr < s) {
                x = l;
                s = s;
                r = true;
            }
        }

        clines.erase(std::remove(clines.begin(), clines.end(), x), clines.end());

        if (r) {
            std::reverse(x.begin(), x.end());
        }

        slines.push_back(x);
    }

    return slines;
}

std::vector<std::vector<std::pair<int, int>>> getcontours(cv::Mat image, int draw_contours = 2) {
    std::cout << "Generating contours..." << std::endl;

    cv::Vec3b pixel = image.at<cv::Vec3b>(0, 0);
    cv::imwrite("/home/arda/Desktop/CSE396/BrachioGraph/images/imageBeforeFindEdgesCPP.jpg", image);
    image = find_edges(image);

    cv::Mat IM1 = image.clone();
    //cv::Mat IM2 = image.clone();
    cv::Mat IM2 = utils::rotate(image, -90, 0, 1);
    //cv::transpose(IM2, IM2);
    //cv::flip(IM2, IM2, 1);
    
    cv::imwrite("/home/arda/Desktop/CSE396/BrachioGraph/images/imageAfterFindEdgesCPP.jpg", image);
    cv::imwrite("/home/arda/Desktop/CSE396/BrachioGraph/images/IM1CPP.jpg", IM1);
    cv::imwrite("/home/arda/Desktop/CSE396/BrachioGraph/images/IM2CPP.jpg", IM2);

    std::vector<std::vector<std::pair<int, int>>> dots1 = getdots(IM1);
    
    std::string svgDOTS1 = makesvg(dots1);
    // Write SVG content to a file
    std::string image_filename = "/home/arda/Desktop/CSE396/BrachioGraph/images/cat.jpeg";
    std::ofstream dots1FILE(image_filename +"svgDOTS1CPP" + ".svg");
    dots1FILE << svgDOTS1;
    dots1FILE.close();

    std::vector<std::vector<std::pair<int, int>>> contours1 = connectdots(dots1);

    std::string svgContours1 = makesvg(contours1);
    // Write SVG content to a file
    std::ofstream contours1File(image_filename +"svgContours1CPP" + ".svg");
    contours1File << svgContours1;
    contours1File.close();

    std::vector<std::vector<std::pair<int, int>>> dots2 = getdots(IM2);
    std::vector<std::vector<std::pair<int, int>>> contours2 = connectdots(dots2);

    for (int i = 0; i < contours2.size(); i++) {
        for (int j = 0; j < contours2[i].size(); j++) {
            contours2[i][j] = std::pair<int, int>(contours2[i][j].second, contours2[i][j].first);
        }
    }

    std::vector<std::vector<std::pair<int, int>>> contours = contours1;
    contours.insert(contours.end(), contours2.begin(), contours2.end());

    for (int i = 0; i < contours.size(); i++) {
        for (int j = 0; j < contours.size(); j++) {
            if (contours[i].size() > 0 && contours[j].size() > 0) {
                if (distsum(contours[j][0], contours[i][contours[i].size() - 1]) < 8) {
                    contours[i].insert(contours[i].end(), contours[j].begin(), contours[j].end());
                    contours[j].clear();
                }
            }
        }
    }

    std::vector<std::pair<int,int>> tempVec;
    for (int i = 0; i < contours.size(); i++) {
        tempVec.clear();
        for (int j = 0; j < contours[i].size(); j += 8) {
            //contours[i].erase(contours[i].begin() + j + 1, contours[i].begin() + j + 8);
            tempVec.push_back(contours[i][j]);
        }
        contours[i] = tempVec;
    }
    
    //contours.erase(std::remove_if(contours.begin(), contours.end(), [](const std::vector<std::pair<int,int>>& c) { return c.size() <= 1; }), contours.end());
    
    std::vector<std::vector<std::pair<int,int>>> contoursTemp;

    for(int i = 0; i < contours.size(); i++){
        if(contours[i].size() > 1){
            contoursTemp.push_back(contours[i]);
        }
    }
    contours = contoursTemp;
    
    for (int i = 0; i < contours.size(); i++) {
        for (int j = 0; j < contours[i].size(); j++) {
            contours[i][j].first *= draw_contours;
            contours[i][j].second *= draw_contours;
        }
    }

    return contours;
}

std::vector<std::vector<std::pair<int, int>>> vectorise(const std::string& image_filename, int resolution = 1024, int draw_contours = 0, int repeat_contours = 1, int draw_hatch = 0, int repeat_hatch = 1) {
    // Load the image using OpenCV
    std::cout << "Image Path:" << image_filename << std::endl;
    cv::Mat image = cv::imread(image_filename);

    if (image.empty()) {
        std::cerr << "Image not loaded properly." << std::endl;
        //throw error
        throw std::invalid_argument("Image Not Loaded Properly");
    }

    //Convert CV Image to Pillow RGB
    cv::cvtColor(image, image, cv::COLOR_BGR2RGB);

    int pix = image.at<uchar>(0, 0);
    std::cout << "After BGR2RGB value at (0, 0): " << pix << std::endl;

    // Convert the image to grayscale
    cv::cvtColor(image, image, cv::COLOR_RGB2GRAY);

    int pixel_value = image.at<uchar>(0, 0);
    std::cout << "After RGB2GRAY value at (0, 0): " << pixel_value << std::endl;

    // Maximize contrast (you may need to adjust parameters)
    //cv::equalizeHist(image, image);

    image = utils::autocontrast(image, 5, -1, cv::Mat(), true);

    int autopix = image.at<uchar>(0, 0);
    std::cout << "After autocontrast value at (0, 0): " << autopix << std::endl;
    
    std::vector<std::vector<std::pair<int, int>>> lines;
    std::vector<std::vector<std::pair<int,int>>> contours;
    std::vector<std::vector<std::pair<int,int>>> hatches;

    if (draw_contours && repeat_contours) {
        std::cout << "image.size before resizeImage:" << image.cols << "," << image.rows << std::endl;
        contours = getcontours(resizeImage(image,resolution,draw_contours), draw_contours);

        // Generate SVG content
        std::string svgContours = makesvg(contours);

        // Write SVG content to a file
        std::ofstream contoursFILE(image_filename +"contoursCPP" + ".svg");
        contoursFILE << svgContours;
        contoursFILE.close();
        std::cout << "Contours CPP Printed: " <<  contours.size()  << std::endl;


        contours = sortlines(contours);
        contours = join_lines(contours);

        for (int r = 0; r < repeat_contours; r++) {
            for(int i = 0; i < contours.size(); i++){
                lines.push_back(contours.at(i));
            }
            //lines.insert(lines.end(), contours.begin(), contours.end());
        }
        std::cout << "A AFTER LOOP" << std::endl;
    }

        // Generate SVG content
    std::string svgContentBefHatch = makesvg(lines);

    // Write SVG content to a file
    std::ofstream befHatch(image_filename +"befHatch" + ".svg");
    befHatch << svgContentBefHatch;
    befHatch.close();

    if (draw_hatch && repeat_hatch) {
        hatches = hatch(resizeImage(image, resolution), draw_hatch);
        hatches = sortlines(hatches);
        hatches = join_lines(hatches);
        for (int r = 0; r < repeat_hatch; r++) {
            for(int i = 0; i < hatches.size(); i++){
                lines.push_back(hatches.at(i));
            }
            //lines.insert(lines.end(), hatches.begin(), hatches.end());
        }
    }

        // Generate SVG content
    std::string hatchStr = makesvg(hatches);

    // Write SVG content to a file
    std::ofstream hatchFile(image_filename +"hatchFile" + ".svg");
    hatchFile << hatchStr;
    hatchFile.close();

    // Calculate the number of segments
    int segments = 0;
    std::cout << "C BEFORE LOOP" << std::endl;
    for (const auto& line : lines) {
        segments += line.size() - 1;
    }
    std::cout << "C AFTER LOOP" << std::endl;

    std::cout << lines.size() << " lines, " << segments << " segments." << std::endl;

    // Generate SVG content
    std::string svgContent = makesvg(lines);

    // Write SVG content to a file
    std::ofstream svgFile(image_filename + ".svg");
    svgFile << svgContent;
    svgFile.close();

    return lines;
}

void imageToJson(const std::string& imageFilename,int resolution = 1024,int drawContours = 0,int repeatContours = 1,int drawHatch = 0,int repeatHatch = 1) 
{
    std::vector<std::vector<std::pair<int, int>>> lines;

    lines = vectorise(imageFilename, resolution, drawContours, repeatContours, drawHatch, repeatHatch);

    //std::string filename = json_folder + imageFilename + ".json";
    std::string filename = "/home/arda/Desktop/CSE396/GUI/scara_gui/tmp/sent.json";
    //std::string filename = "/home/arda/Desktop/CSE396/BrachioGraph/images/cat.json";
    linesToFile(lines, filename);
}

// int main()
// {
//     std::string imagePath = "/home/arda/Downloads/MicrosoftTeams-image.png";
//     //std::string imagePath = "/home/arda/Desktop/CSE396/BrachioGraph/images/test.jpg";
//     imageToJson(imagePath, 1024, 2, 1 , 16, 1);
// }


}