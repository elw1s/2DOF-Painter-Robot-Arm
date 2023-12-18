#include <iostream>
//#include <opencv2/opencv.hpp>
#include <vector>
#include <numeric>
#include <cmath>
//#include <xtensor/xarray.hpp>
//#include <xtensor/xio.hpp>
//#include <xtensor/xview.hpp>

namespace utils{

    template <typename T>
    double mean(const std::vector<T>& v) {
        if (v.empty()) {
            throw std::invalid_argument("Cannot compute mean of an empty vector");
        }

        T sum = std::accumulate(v.begin(), v.end(), T{});
        return static_cast<double>(sum) / v.size();
    }

class Polynomial {
public:
    Polynomial(const std::vector<double>& coeffs) : coefficients(coeffs) {}

    double operator()(double x) const {
        double result = 0.0;
        int power = coefficients.size() - 1;
        for (const auto& coeff : coefficients) {
            result += coeff * std::pow(x, power);
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

