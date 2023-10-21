#include <iostream>
#include <random>
#include <cmath>
#include <nlohmann/json.hpp>
#include <ctime>
#include "xtensor/xbuilder.hpp"


const std::string export_path = "images/out.svg";
const std::string svg_folder = "images/";
const std::string json_folder = "images/";
const bool NO_CV = false;

//try :
//import numpy as np
//import cv2
//    except :
//print("Cannot import numpy/openCV. Switching to NO_CV mode.")
//no_cv = True

void image_to_json(
    std::string image_filename,
    int resolution = 1024,
    bool draw_contours = false,
    int repeat_contours = 1,
    bool draw_hatch = false,
    int repeat_hatch = 1)
{

}

std::vector<std::vector<std::pair<int, int>>> connectdots(std::vector<std::vector<std::pair<int, int>>> dots) {
    std::cout << "Connecting contour points..." << std::endl;
    std::vector<std::vector<std::pair<int, int>>> contours;
    for (size_t y = 0; y < dots.size(); y++) {
        for (size_t x = 0; x < dots[y].size(); x++) {
            if (dots[y][x].second > -1) {
                if (y == 0) {
                    contours.push_back(std::vector<std::pair<int, int>>{{dots[y][x].first, dots[y][x].second}});
                }
                else {
                    int closest = -1;
                    int cdist = 100;
                    for (size_t i = 0; i < dots[y - 1].size(); i++) {
                        if (abs(dots[y - 1][i].first - dots[y][x].first) < cdist) {
                            cdist = abs(dots[y - 1][i].first - dots[y][x].first);
                            closest = dots[y - 1][i].first;
                        }
                    }

                    if (cdist > 3) {
                        contours.push_back(std::vector<std::pair<int, int>>{{dots[y][x].first, dots[y][x].second}});
                    }
                    else {
                        bool found = false;
                        for (size_t i = 0; i < contours.size(); i++) {
                            if (contours[i].back() == std::pair<int, int>(closest, dots[y - 1][i].second)) {
                                contours[i].push_back(std::pair<int, int>(dots[y][x].first, dots[y][x].second));
                                found = true;
                                break;
                            }
                        }
                        if (!found) {
                            contours.push_back(std::vector<std::pair<int, int>>{{dots[y][x].first, dots[y][x].second}});
                        }
                    }
                }
            }
        }
        for (size_t i = 0; i < contours.size(); i++) {
            if (contours[i].back().second < dots[y - 1][i].second && contours[i].size() < 4) {
                contours.erase(contours.begin() + i);
            }
        }
    }
    return contours;
}

std::vector<std::vector<cv::Point>> getcontours(cv::Mat image, int draw_contours = 2) {
    std::cout << "Generating contours..." << std::endl;
    image = find_edges(image);
    cv::Mat IM1 = image.clone();
    cv::Mat IM2 = image.clone();
    cv::transpose(IM2, IM2);
    cv::flip(IM2, IM2, 1);

    std::vector<std::vector<std::pair<int, int>>> dots1 = getdots(IM1);
    std::vector<std::vector<std::pair<int, int>>> contours1 = connectdots(dots1);
    std::vector<std::vector<std::pair<int, int>>> dots2 = getdots(IM2);
    std::vector<std::vector<std::pair<int, int>>> contours2 = connectdots(dots2);

    for (int i = 0; i < contours2.size(); i++) {
        for (int j = 0; j < contours2[i].size(); j++) {
            contours2[i][j] = std::pair<int, int>(contours2[i][j].second, contours2[i][j].first);
        }
    }

    std::vector<std::vector<cv::Point>> contours = contours1;
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

    for (int i = 0; i < contours.size(); i++) {
        for (int j = 0; j < contours[i].size(); j += 8) {
            contours[i].erase(contours[i].begin() + j + 1, contours[i].begin() + j + 8);
        }
    }

    contours.erase(std::remove_if(contours.begin(), contours.end(), [](const std::vector<cv::Point>& c) { return c.size() <= 1; }), contours.end());

    for (int i = 0; i < contours.size(); i++) {
        for (int j = 0; j < contours[i].size(); j++) {
            contours[i][j].x *= draw_contours;
            contours[i][j].y *= draw_contours;
        }
    }

    return contours;
}







int main()
{
    
}

