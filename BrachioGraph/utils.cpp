#include <iostream>
#include <opencv2/opencv.hpp>
#include <xtensor/xarray.hpp>
#include <xtensor/xio.hpp>
#include <xtensor/xview.hpp>

namespace utils{

    // Define the _lut function
    cv::Mat _lut(const cv::Mat& image, const std::vector<uchar>& lut) {
        cv::Mat result;
        cv::LUT(image, cv::Mat(lut), result);
        return result;
    }

    // Define the autocontrast function
    cv::Mat autocontrast(const cv::Mat& image, double cutoff = 0, int ignore = -1, const cv::Mat& mask = cv::Mat(), bool preserve_tone = false) {
        cv::Mat histogram;
        if (preserve_tone) {
            // Convert the image to grayscale if it's not already
            cv::Mat grayImage;
            if (image.channels() == 3) {
                cv::cvtColor(image, grayImage, cv::COLOR_BGR2GRAY);
            } else {
                grayImage = image.clone();  // Image is already grayscale
            }
            
            if (!mask.empty()) {
                // Apply the mask
                cv::Mat maskedGrayImage;
                cv::bitwise_and(grayImage, mask, maskedGrayImage);
                histogram = cv::Mat::zeros(1, 256, CV_64F);
                for (int i = 0; i < maskedGrayImage.rows; ++i) {
                    for (int j = 0; j < maskedGrayImage.cols; ++j) {
                        int pixel = maskedGrayImage.at<uchar>(i, j);
                        histogram.at<double>(0, pixel) += 1.0;
                    }
                }
            } else {
                histogram = cv::Mat::zeros(1, 256, CV_64F);
                for (int i = 0; i < grayImage.rows; ++i) {
                    for (int j = 0; j < grayImage.cols; ++j) {
                        int pixel = grayImage.at<uchar>(i, j);
                        histogram.at<double>(0, pixel) += 1.0;
                    }
                }
            }
        } else {
            if (!mask.empty()) {
                histogram = cv::Mat::zeros(1, 256, CV_64F);
                for (int i = 0; i < image.rows; ++i) {
                    for (int j = 0; j < image.cols; ++j) {
                        int pixel = image.at<uchar>(i, j);
                        histogram.at<double>(0, pixel) += 1.0;
                    }
                }
            } else {
                int histSize[] = {256};  // Specify the number of bins for each channel
                cv::calcHist(&image, 1, 0, mask, histogram, 1, histSize, nullptr, true, false);
            }
        }

        std::vector<uchar> lut;
        for (int layer = 0; layer < histogram.cols; layer += 256) {
            cv::Mat h = histogram.colRange(layer, layer + 256);
            if (ignore != -1) {
                h.at<double>(0, ignore) = 0.0;
            }

            if (cutoff > 0) {
                double n = cv::sum(h)[0];
                double cut = cutoff * n / 100.0;

                int lo = 0;
                double hist_sum = 0.0;
                for (int i = 0; i < 256; ++i) {
                    hist_sum += h.at<double>(0, i);
                    if (hist_sum > cut) {
                        lo = i;
                        break;
                    }
                }

                int hi = 255;
                hist_sum = 0.0;
                for (int i = 255; i >= 0; --i) {
                    hist_sum += h.at<double>(0, i);
                    if (hist_sum > cut) {
                        hi = i;
                        break;
                    }
                }

                if (hi <= lo) {
                    for (int i = 0; i < 256; ++i) {
                        lut.push_back(i);
                    }
                } else {
                    double scale = 255.0 / (hi - lo);
                    double offset = -lo * scale;
                    for (int i = 0; i < 256; ++i) {
                        int ix = static_cast<int>(i * scale + offset);
                        if (ix < 0) {
                            ix = 0;
                        } else if (ix > 255) {
                            ix = 255;
                        }
                        lut.push_back(static_cast<uchar>(ix));
                    }
                }
            } else {
                for (int i = 0; i < 256; ++i) {
                    lut.push_back(i);
                }
            }
        }

        return _lut(image, lut);
    }

    cv::Mat rotate(
    const cv::Mat& image,
    double angle,
    int resample = cv::INTER_NEAREST,
    int expand = 0,
    const cv::Point2f& center = cv::Point2f(-1, -1),
    const cv::Scalar& fillcolor = cv::Scalar(0, 0, 0)
) {
    angle = fmod(angle, 360.0);

    if (angle == 0.0) {
        return image.clone();
    }

    cv::Mat rotated;
    cv::Point2f actualCenter = center;

    if (center == cv::Point2f(-1, -1)) {
        actualCenter.x = image.cols / 2.0;
        actualCenter.y = image.rows / 2.0;
    }

    cv::Mat rotationMatrix = cv::getRotationMatrix2D(actualCenter, -angle, 1.0);

    if (expand) {
        double angleRadians = angle * (CV_PI / 180.0);
        double sinAngle = std::abs(std::sin(angleRadians));
        double cosAngle = std::abs(std::cos(angleRadians));
        int newWidth = static_cast<int>(image.cols * cosAngle + image.rows * sinAngle);
        int newHeight = static_cast<int>(image.cols * sinAngle + image.rows * cosAngle);

        // Adjust the translation to keep the image centered
        rotationMatrix.at<double>(0, 2) += (newWidth - image.cols) / 2;
        rotationMatrix.at<double>(1, 2) += (newHeight - image.rows) / 2;

        cv::warpAffine(image, rotated, rotationMatrix, cv::Size(newWidth, newHeight), resample, cv::BORDER_CONSTANT, fillcolor);
    } else {
        cv::warpAffine(image, rotated, rotationMatrix, image.size(), resample);
    }

    return rotated;
}

}

