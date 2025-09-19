#ifndef YIELDCURVE_H
#define YIELDCURVE_H

#include <vector>
#include <map>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <cmath>
#include <iomanip>

struct YieldPoint {
    double maturity;  // in years
    double yield;     // in percentage
    std::string maturity_label;

    YieldPoint(double m, double y, const std::string& label) 
        : maturity(m), yield(y), maturity_label(label) {}
};

class YieldCurve {
private:
    std::vector<YieldPoint> yield_points;
    std::string curve_date;

    // Helper function for interpolation
    double linearInterpolation(double x, double x1, double y1, double x2, double y2) {
        if (std::abs(x2 - x1) < 1e-9) return y1;
        return y1 + (y2 - y1) * (x - x1) / (x2 - x1);
    }

    // Cubic spline interpolation helper
    std::vector<double> calculateSplineCoefficients(const std::vector<double>& x, 
                                                   const std::vector<double>& y) {
        int n = x.size();
        std::vector<double> h(n-1), alpha(n-1), mu(n), z(n), c(n);

        for (int i = 0; i < n-1; i++) {
            h[i] = x[i+1] - x[i];
        }

        for (int i = 1; i < n-1; i++) {
            alpha[i] = (3.0/h[i]) * (y[i+1] - y[i]) - (3.0/h[i-1]) * (y[i] - y[i-1]);
        }

        // Natural spline conditions
        mu[0] = 0;
        z[0] = 0;
        c[0] = 0;

        for (int i = 1; i < n-1; i++) {
            double l = 2 * (x[i+1] - x[i-1]) - h[i-1] * mu[i-1];
            mu[i] = h[i] / l;
            z[i] = (alpha[i] - h[i-1] * z[i-1]) / l;
        }

        c[n-1] = 0;
        for (int i = n-2; i >= 0; i--) {
            c[i] = z[i] - mu[i] * c[i+1];
        }

        return c;
    }

public:
    YieldCurve(const std::string& date = "") : curve_date(date) {}

    // Load yield data from CSV file
    bool loadFromCSV(const std::string& filename, const std::string& date_filter = "") {
        std::ifstream file(filename);
        if (!file.is_open()) {
            std::cerr << "Error: Could not open file " << filename << std::endl;
            return false;
        }

        std::string line;
        std::getline(file, line); // Skip header

        // Maturity mappings
        std::map<std::string, double> maturity_map = {
            {"3MO", 0.25}, {"6MO", 0.5}, {"2Y", 2.0}, 
            {"5Y", 5.0}, {"10Y", 10.0}, {"30Y", 30.0}
        };

        bool found_date = false;
        while (std::getline(file, line)) {
            std::istringstream iss(line);
            std::string token;
            std::vector<std::string> tokens;

            while (std::getline(iss, token, ',')) {
                tokens.push_back(token);
            }

            if (tokens.size() >= 7) {
                std::string date = tokens[0];

                // If no date filter or date matches
                if (date_filter.empty() || date.find(date_filter) != std::string::npos) {
                    curve_date = date;
                    yield_points.clear();

                    // Add yield points for each maturity
                    int col = 1;
                    for (const auto& pair : maturity_map) {
                        if (col < tokens.size()) {
                            double yield_val = std::stod(tokens[col]);
                            yield_points.emplace_back(pair.second, yield_val, pair.first);
                            col++;
                        }
                    }
                    found_date = true;
                    if (!date_filter.empty()) break; // If specific date requested, stop after first match
                }
            }
        }

        // Sort by maturity
        std::sort(yield_points.begin(), yield_points.end(), 
                  [](const YieldPoint& a, const YieldPoint& b) {
                      return a.maturity < b.maturity;
                  });

        file.close();
        return found_date;
    }

    // Get interpolated yield for any maturity
    double getYield(double maturity) {
        if (yield_points.empty()) return 0.0;

        // If exact match
        for (const auto& point : yield_points) {
            if (std::abs(point.maturity - maturity) < 1e-6) {
                return point.yield;
            }
        }

        // Find surrounding points for interpolation
        if (maturity <= yield_points.front().maturity) {
            return yield_points.front().yield;
        }

        if (maturity >= yield_points.back().maturity) {
            return yield_points.back().yield;
        }

        for (size_t i = 0; i < yield_points.size() - 1; i++) {
            if (yield_points[i].maturity <= maturity && 
                yield_points[i+1].maturity >= maturity) {
                return linearInterpolation(maturity, 
                                         yield_points[i].maturity, yield_points[i].yield,
                                         yield_points[i+1].maturity, yield_points[i+1].yield);
            }
        }

        return 0.0;
    }

    // Calculate forward rates
    double getForwardRate(double start_maturity, double end_maturity) {
        if (end_maturity <= start_maturity) return 0.0;

        double y1 = getYield(start_maturity) / 100.0; // Convert to decimal
        double y2 = getYield(end_maturity) / 100.0;

        // Forward rate calculation: ((1 + y2)^T2 / (1 + y1)^T1)^(1/(T2-T1)) - 1
        double forward_rate = std::pow((std::pow(1 + y2, end_maturity) / 
                                       std::pow(1 + y1, start_maturity)), 
                                      1.0 / (end_maturity - start_maturity)) - 1.0;

        return forward_rate * 100.0; // Convert back to percentage
    }

    // Calculate duration for a bond at given maturity
    double getDuration(double maturity, double coupon_rate = 0.0) {
        double yield = getYield(maturity) / 100.0;

        if (coupon_rate == 0.0) {
            // Zero-coupon bond duration equals maturity
            return maturity;
        }

        // Approximate modified duration for coupon bond
        double modified_duration = maturity / (1 + yield);
        return modified_duration;
    }

    // Get yield spread between two maturities
    double getSpread(double maturity1, double maturity2) {
        return getYield(maturity2) - getYield(maturity1);
    }

    // Print yield curve information
    void printCurve() {
        std::cout << "\n=== US Treasury Yield Curve Analysis ===" << std::endl;
        std::cout << "Date: " << curve_date << std::endl;
        std::cout << "\nYield Points:" << std::endl;
        std::cout << std::setw(10) << "Maturity" << std::setw(12) << "Yield (%)" 
                  << std::setw(15) << "Duration" << std::endl;
        std::cout << std::string(37, '-') << std::endl;

        for (const auto& point : yield_points) {
            std::cout << std::setw(10) << point.maturity_label 
                      << std::setw(12) << std::fixed << std::setprecision(2) << point.yield
                      << std::setw(15) << std::setprecision(2) << getDuration(point.maturity)
                      << std::endl;
        }

        // Key spreads
        std::cout << "\n=== Key Spreads ===" << std::endl;
        std::cout << "2s10s Spread: " << std::setprecision(0) << getSpread(2.0, 10.0) * 100 
                  << " basis points" << std::endl;
        std::cout << "3m10y Spread: " << std::setprecision(0) << getSpread(0.25, 10.0) * 100 
                  << " basis points" << std::endl;
        std::cout << "5s30s Spread: " << std::setprecision(0) << getSpread(5.0, 30.0) * 100 
                  << " basis points" << std::endl;

        // Forward rates
        std::cout << "\n=== Forward Rates ===" << std::endl;
        std::cout << "1y1y Forward: " << std::setprecision(2) << getForwardRate(1.0, 2.0) 
                  << "%" << std::endl;
        std::cout << "2y1y Forward: " << std::setprecision(2) << getForwardRate(2.0, 3.0) 
                  << "%" << std::endl;
        std::cout << "5y5y Forward: " << std::setprecision(2) << getForwardRate(5.0, 10.0) 
                  << "%" << std::endl;
    }

    // Export curve data for dashboard
    void exportToJSON(const std::string& filename) {
        std::ofstream file(filename);
        if (!file.is_open()) {
            std::cerr << "Error: Could not create file " << filename << std::endl;
            return;
        }

        file << "{\n";
        file << "  \"date\": \"" << curve_date << "\",\n";
        file << "  \"yield_points\": [\n";

        for (size_t i = 0; i < yield_points.size(); i++) {
            const auto& point = yield_points[i];
            file << "    {\n";
            file << "      \"maturity_label\": \"" << point.maturity_label << "\",\n";
            file << "      \"maturity_years\": " << point.maturity << ",\n";
            file << "      \"yield\": " << point.yield << "\n";
            file << "    }";
            if (i < yield_points.size() - 1) file << ",";
            file << "\n";
        }

        file << "  ],\n";
        file << "  \"spreads\": {\n";
        file << "    \"2s10s\": " << getSpread(2.0, 10.0) * 100 << ",\n";
        file << "    \"3m10y\": " << getSpread(0.25, 10.0) * 100 << ",\n";
        file << "    \"5s30s\": " << getSpread(5.0, 30.0) * 100 << "\n";
        file << "  },\n";
        file << "  \"forward_rates\": {\n";
        file << "    \"1y1y\": " << getForwardRate(1.0, 2.0) << ",\n";
        file << "    \"2y1y\": " << getForwardRate(2.0, 3.0) << ",\n";
        file << "    \"5y5y\": " << getForwardRate(5.0, 10.0) << "\n";
        file << "  }\n";
        file << "}\n";

        file.close();
        std::cout << "\nYield curve data exported to " << filename << std::endl;
    }

    const std::vector<YieldPoint>& getYieldPoints() const {
        return yield_points;
    }

    const std::string& getDate() const {
        return curve_date;
    }
};

#endif // YIELDCURVE_H
