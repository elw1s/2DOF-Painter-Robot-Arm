#include <iostream>
#include <vector>
#include <cmath>

double multiplyBy5(double x) {
    return x * 5.0;
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

 
int main() {
    std::vector<double> coeffs = {1.0, 2.0, 3.0}; // coefficients for x^2 - 3x + 2
    double (*multiplyPtr)(double);
    double (*polyFuncPtr)(double);
    polyFuncPtr = Polynomial::getFunctionPtr(coeffs);
    multiplyPtr = &multiplyBy5;

    double x = 0.5;
    double result = polyFuncPtr(x);

    double mult = multiplyPtr(6);
    std::cout << "Multiplied:" << mult << std::endl;

    std::cout << "Result of the polynomial for x = " << x << " is: " << result << std::endl;

    return 0;
}
