#include <iostream>
#include <opencv2/opencv.hpp>
#include <xtensor/xarray.hpp>
#include <xtensor/xio.hpp>
#include <xtensor/xview.hpp>

namespace utils{

    cv::Rect getSafeBox(const cv::Size& size, int resample, const cv::Rect& box) {
        const double filter_support = cv::INTER_CUBIC - 0.5;
        const double scale_x = (box.width) / static_cast<double>(size.width);
        const double scale_y = (box.height) / static_cast<double>(size.height);
        const double support_x = filter_support * scale_x;
        const double support_y = filter_support * scale_y;

        int x1 = std::max(0, static_cast<int>(box.x - support_x));
        int y1 = std::max(0, static_cast<int>(box.y - support_y));
        int x2 = std::min(box.x + box.width + support_x, static_cast<double>(box.x + box.width));
        int y2 = std::min(box.y + box.height + support_y, static_cast<double>(box.y + box.height));

        return cv::Rect(x1, y1, x2 - x1, y2 - y1);
    }

    cv::Mat resize(const cv::Mat& image, cv::Size size, int resample = cv::INTER_LINEAR, cv::Rect box = cv::Rect(), double reducing_gap = 0.0) {
    cv::Mat result;

    if (resample == -1) {
        // Determine the default resampling filter based on image mode
        bool type_special = (image.type() == CV_16U);
        resample = (type_special) ? cv::INTER_NEAREST : cv::INTER_LINEAR;
    } else if (resample != cv::INTER_NEAREST &&
               resample != cv::INTER_LINEAR &&
               resample != cv::INTER_CUBIC &&
               resample != cv::INTER_LANCZOS4 &&
               resample != cv::INTER_AREA) {
        throw std::invalid_argument("Unknown resampling filter");
    }

    if (reducing_gap != 0.0 && reducing_gap < 1.0) {
        throw std::invalid_argument("reducing_gap must be 1.0 or greater");
    }

    if (box == cv::Rect()) {
        box = cv::Rect(0, 0, image.cols, image.rows);
    }

    if (image.size() == size && box == cv::Rect(0, 0, image.cols, image.rows)) {
        return image.clone();
    }

    if (image.type() == CV_8U || image.type() == CV_8S) {
        resample = cv::INTER_NEAREST;
    }

    if ((image.type() == CV_8UC2 || image.type() == CV_8UC4) && resample != cv::INTER_NEAREST) {
        cv::cvtColor(image, result, (image.type() == CV_8UC2) ? cv::COLOR_Lab2BGR : cv::COLOR_Lab2BGR);
        result = resize(result, size, resample, box, reducing_gap);
        cv::cvtColor(result, result, (image.type() == CV_8UC2) ? cv::COLOR_BGR2Lab : cv::COLOR_BGR2Lab);
        return result;
    } 

    if (reducing_gap != 0.0 && resample != cv::INTER_NEAREST) {
        int factor_x = static_cast<int>((box.width) / size.width / reducing_gap) || 1;
        int factor_y = static_cast<int>((box.height) / size.height / reducing_gap) || 1;

        if (factor_x > 1 || factor_y > 1) {
            cv::Rect reduce_box = getSafeBox(size, resample, box); // Implement getSafeBox function
            cv::resize(image(box), result, size, 0, 0, resample);
            box = cv::Rect((box.x - reduce_box.x) / factor_x, (box.y - reduce_box.y) / factor_y, (box.width - reduce_box.x) / factor_x, (box.height - reduce_box.y) / factor_y);
        }
    }

    cv::resize(image(box), result, size, 0, 0, resample);
    return result;
}

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

class Polynomial {
public:
    Polynomial(const std::vector<double>& coeffs) : coefficients(coeffs) {}

    double operator()(double x) const {
        double result = 0.0;
        int power = coefficients.size() - 1;
        for (const auto& coeff : coefficients) {
            result += coeff * pow(x, power);
            power--;
        }
        return result;
    }

    static double evaluate(double x, const Polynomial* poly) {
        return (*poly)(x);
    }

    static double (*getFunctionPtr(const std::vector<double>& coeffs))(double) {
        static Polynomial poly(coeffs);

        return [](double x) { return poly(x); };
    }

private:
    const std::vector<double>& coefficients;
};

std::vector<double> polyfit(const std::vector<double>& x, const std::vector<double>& y, int degree) {
    int n = x.size();
    int m = degree + 1;

    std::vector<double> X((n * m), 0.0);
    std::vector<double> Y(y.begin(), y.end());

    for (int i = 0; i < n; ++i) {
        double temp = 1.0;
        for (int j = 0; j < m; ++j) {
            X[i * m + j] = temp;
            temp *= x[i];
        }
    }

    std::vector<double> XT(m * n);
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            XT[j * n + i] = X[i * m + j];
        }
    }

    std::vector<double> XT_X(m * m, 0.0);
    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < m; ++j) {
            for (int k = 0; k < n; ++k) {
                XT_X[i * m + j] += XT[i * n + k] * X[k * m + j];
            }
        }
    }

    std::vector<double> XT_Y(m, 0.0);
    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < n; ++j) {
            XT_Y[i] += XT[i * n + j] * Y[j];
        }
    }

    // Solve the system of linear equations
    for (int i = 0; i < m; ++i) {
        double divisor = XT_X[i * m + i];
        for (int j = i; j < m; ++j) {
            XT_X[i * m + j] /= divisor;
        }
        XT_Y[i] /= divisor;

        for (int k = i + 1; k < m; ++k) {
            double factor = XT_X[k * m + i];
            for (int j = i; j < m; ++j) {
                XT_X[k * m + j] -= factor * XT_X[i * m + j];
            }
            XT_Y[k] -= factor * XT_Y[i];
        }
    }

    std::vector<double> coeffs(m);
    for (int i = m - 1; i >= 0; --i) {
        coeffs[i] = XT_Y[i];
        for (int j = i + 1; j < m; ++j) {
            coeffs[i] -= XT_X[i * m + j] * coeffs[j];
        }
    }

    return coeffs;
}


}

