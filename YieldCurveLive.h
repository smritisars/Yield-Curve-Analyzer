#ifndef YIELDCURVE_LIVE_H
#define YIELDCURVE_LIVE_H

#include <vector>
#include <map>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <stdexcept>

struct YieldPoint {
    double maturity;  // in years
    double yield;     // in percentage
    std::string maturity_label;
    
    YieldPoint(double m, double y, const std::string& label) 
        : maturity(m), yield(y), maturity_label(label) {}
};

class YieldCurveLive {
private:
    std::vector<YieldPoint> yield_points;
    std::string curve_date;
    std::map<std::string, double> maturity_map;
    
    // Initialize maturity mappings for real Treasury data
    void initializeMaturityMap() {
        maturity_map = {
            {"1MO", 1.0/12.0},    // 0.083 years
            {"3MO", 0.25},        // 0.25 years  
            {"6MO", 0.5},         // 0.5 years
            {"1Y", 1.0},          // 1 year
            {"2Y", 2.0},          // 2 years
            {"3Y", 3.0},          // 3 years
            {"5Y", 5.0},          // 5 years
            {"7Y", 7.0},          // 7 years
            {"10Y", 10.0},        // 10 years
            {"20Y", 20.0},        // 20 years
            {"30Y", 30.0}         // 30 years
        };
    }
    
    // Helper function for linear interpolation
    double linearInterpolation(double x, double x1, double y1, double x2, double y2) const {
        if (std::abs(x2 - x1) < 1e-9) return y1;
        return y1 + (y2 - y1) * (x - x1) / (x2 - x1);
    }
    
    // Cubic spline interpolation for smoother curves
    std::vector<double> calculateSplineCoefficients(const std::vector<double>& x, 
                                                   const std::vector<double>& y) const {
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
    YieldCurveLive(const std::string& date = "") : curve_date(date) {
        initializeMaturityMap();
    }
    
    // Load yield data from CSV file with expanded Treasury maturities
    bool loadFromCSV(const std::string& filename, const std::string& date_filter = "") {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file " << filename << std::endl;
        return false; // Stop processing if file can't be opened
    }

    std::string line;
    if (!std::getline(file, line)) {
        std::cerr << "Error: CSV file " << filename << " is empty or unreadable." << std::endl;
        return false; // No header or empty file
    }

    bool found_date = false;
    while (std::getline(file, line)) {
        if (line.empty()) {
            std::cerr << "Warning: Skipping empty line in CSV file." << std::endl;
            continue; // Skip empty lines gracefully
        }
        std::istringstream iss(line);
        std::string token;
        std::vector<std::string> tokens;

        while (std::getline(iss, token, ',')) {
            tokens.push_back(token);
        }

        if (tokens.size() < 12) {
            std::cerr << "Warning: Skipping line with insufficient columns (expected 12): " << line << std::endl;
            continue; // Skip malformed lines without enough columns
        }

        std::string date = tokens[0];

        if (!date_filter.empty() && date.find(date_filter) == std::string::npos) {
            continue; // Skip lines not matching date filter early
        }

        // Clear previous data before new date load
        yield_points.clear();
        curve_date = date;

        std::vector<std::string> maturity_order = {
            "1MO", "3MO", "6MO", "1Y", "2Y", "3Y", "5Y", "7Y", "10Y", "20Y", "30Y"
        };

        bool valid_data_found = false;
        for (size_t i = 0; i < maturity_order.size() && i + 1 < tokens.size(); i++) {
            std::string mat_label = maturity_order[i];

            try {
                if (tokens[i + 1].empty()) {
                    std::cerr << "Warning: Missing yield value for " << mat_label << " on date " << date << std::endl;
                    continue; // Skip missing yield values
                }
                double yield_val = std::stod(tokens[i + 1]);
                double maturity_years = maturity_map.at(mat_label);
                yield_points.emplace_back(maturity_years, yield_val, mat_label);
                valid_data_found = true;
            } catch (const std::invalid_argument&) {
                std::cerr << "Warning: Invalid yield data '" << tokens[i + 1] << "' for " << mat_label 
                          << " on date " << date << std::endl;
                continue;
            } catch (const std::out_of_range&) {
                std::cerr << "Warning: Yield data out of range for " << mat_label << std::endl;
                continue;
            }
        }

        if (valid_data_found) {
            found_date = true;
            if (!date_filter.empty()) break; // Stop if filtered date found and loaded
        }
    }

    if (!found_date) {
        std::cerr << "Warning: No valid data found in CSV for date filter '" << date_filter << "'." << std::endl;
    }

    if (!yield_points.empty()) {
        std::sort(yield_points.begin(), yield_points.end(), 
                  [](const YieldPoint& a, const YieldPoint& b) {
                      return a.maturity < b.maturity;
                  });
    } else {
        std::cerr << "Error: No yield points loaded. Please check the CSV file content." << std::endl;
    }

    file.close();
    return found_date;
}

    
    // Get interpolated yield for any maturity using improved method
    double getYield(double maturity) const {
        if (yield_points.empty()) return 0.0;
        
        // If exact match
        for (const auto& point : yield_points) {
            if (std::abs(point.maturity - maturity) < 1e-6) {
                return point.yield;
            }
        }
        
        // Handle boundary cases
        if (maturity <= yield_points.front().maturity) {
            return yield_points.front().yield;
        }
        
        if (maturity >= yield_points.back().maturity) {
            return yield_points.back().yield;
        }
        
        // Linear interpolation between surrounding points
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
    
    // Calculate forward rates with improved precision
    double getForwardRate(double start_maturity, double end_maturity) const {
        if (end_maturity <= start_maturity) return 0.0;
        
        double y1 = getYield(start_maturity) / 100.0; // Convert to decimal
        double y2 = getYield(end_maturity) / 100.0;
        
        try {
            double forward_rate = std::pow((std::pow(1 + y2, end_maturity) / 
                                           std::pow(1 + y1, start_maturity)), 
                                          1.0 / (end_maturity - start_maturity)) - 1.0;
            
            return forward_rate * 100.0; // Convert back to percentage
        } catch (const std::exception& e) {
            return 0.0; // Return 0 on calculation error
        }
    }
    
    // Calculate modified duration
    double getDuration(double maturity, double coupon_rate = 0.0) const {
        double yield = getYield(maturity) / 100.0;
        
        if (coupon_rate == 0.0) {
            return maturity;
        }
        
        double modified_duration = maturity / (1 + yield);
        return modified_duration;
    }
    
    // Get yield spread between two maturities
    double getSpread(double maturity1, double maturity2) const {
        return getYield(maturity2) - getYield(maturity1);
    }
    
    // Analyze curve shape with detailed classification
    std::string analyzeCurveShape() const {
        if (yield_points.size() < 3) return "Insufficient Data";
        
        double short_rate = getYield(0.25);
        double medium_rate = getYield(5.0);
        double long_rate = getYield(30.0);
        
        if (short_rate > medium_rate + 0.2 && long_rate > medium_rate + 0.2) {
            return "Humped";
        } else if (short_rate > long_rate + 0.1) {
            return "Inverted";
        } else if (long_rate > short_rate + 0.5) {
            return "Steep Normal";
        } else if (long_rate > short_rate + 0.1) {
            return "Normal";
        } else {
            return "Flat";
        }
    }
    
    // Print comprehensive yield curve analysis
    void printCurve() const {
        std::cout << "\n=== US TREASURY YIELD CURVE ANALYSIS (LIVE DATA) ===" << std::endl;
        std::cout << "📊 Source: Federal Reserve H.15 Selected Interest Rates" << std::endl;
        std::cout << "📅 Date: " << curve_date << std::endl;
        std::cout << "🔗 Data: https://www.federalreserve.gov/releases/h15/" << std::endl;
        
        std::cout << "\n📈 YIELD CURVE POINTS:" << std::endl;
        std::cout << std::setw(10) << "Maturity" << std::setw(12) << "Yield (%)" 
                  << std::setw(15) << "Duration" << std::setw(12) << "DV01($)" << std::endl;
        std::cout << std::string(49, '-') << std::endl;
        
        for (const auto& point : yield_points) {
            double duration = getDuration(point.maturity);
            double dv01 = duration * 100; // Approximate DV01 for $10,000 face value
            
            std::cout << std::setw(10) << point.maturity_label 
                      << std::setw(12) << std::fixed << std::setprecision(2) << point.yield
                      << std::setw(15) << std::setprecision(2) << duration
                      << std::setw(12) << std::setprecision(0) << dv01
                      << std::endl;
        }
        
        std::string shape = analyzeCurveShape();
        std::cout << "\n🔍 CURVE SHAPE: " << shape << std::endl;
        
        std::cout << "\n📊 KEY SPREADS:" << std::endl;
        std::cout << std::string(30, '-') << std::endl;
        
        double spread_2s10s = getSpread(2.0, 10.0);
        double spread_3m10y = getSpread(0.25, 10.0);
        double spread_5s30s = getSpread(5.0, 30.0);
        
        std::cout << "2s10s Spread: " << std::setprecision(0) << spread_2s10s * 100 
                  << " bps";
        if (spread_2s10s < -0.2) {
            std::cout << " 🚨 RECESSION WARNING";
        } else if (spread_2s10s < 0) {
            std::cout << " ⚠️  INVERTED";
        } else if (spread_2s10s < 0.5) {
            std::cout << " ⚡ FLATTENING";
        } else {
            std::cout << " ✅ NORMAL";
        }
        std::cout << std::endl;
        
        std::cout << "3m10y Spread: " << std::setprecision(0) << spread_3m10y * 100 
                  << " bps" << std::endl;
        std::cout << "5s30s Spread: " << std::setprecision(0) << spread_5s30s * 100 
                  << " bps (Term Premium)" << std::endl;
    }
    
    // Export curve data for dashboard with enhanced metrics
    void exportToJSON(const std::string& filename) const {
        std::ofstream file(filename);
        if (!file.is_open()) {
            std::cerr << "Error: Could not create file " << filename << std::endl;
            return;
        }
        
        file << "{\\n";
        file << "  \"data_source\": \"Federal Reserve H.15 Selected Interest Rates\",\\n";
        file << "  \"source_url\": \"https://www.federalreserve.gov/releases/h15/\",\\n";
        file << "  \"date\": \"" << curve_date << "\",\\n";
        file << "  \"curve_shape\": \"" << analyzeCurveShape() << "\",\\n";
        file << "  \"yield_points\": [\\n";
        
        for (size_t i = 0; i < yield_points.size(); i++) {
            const auto& point = yield_points[i];
            file << "    {\\n";
            file << "      \"maturity_label\": \"" << point.maturity_label << "\",\\n";
            file << "      \"maturity_years\": " << point.maturity << ",\\n";
            file << "      \"yield\": " << point.yield << ",\\n";
            file << "      \"duration\": " << getDuration(point.maturity) << "\\n";
            file << "    }";
            if (i < yield_points.size() - 1) file << ",";
            file << "\\n";
        }
        
        file << "  ],\\n";
        file << "  \"key_spreads\": {\\n";
        file << "    \"2s10s_bps\": " << getSpread(2.0, 10.0) * 100 << ",\\n";
        file << "    \"3m10y_bps\": " << getSpread(0.25, 10.0) * 100 << ",\\n";
        file << "    \"5s30s_bps\": " << getSpread(5.0, 30.0) * 100 << ",\\n";
        file << "    \"1m3m_bps\": " << getSpread(1.0/12.0, 0.25) * 100 << "\\n";
        file << "  },\\n";
        file << "  \"forward_rates\": {\\n";
        file << "    \"1y1y\": " << getForwardRate(1.0, 2.0) << ",\\n";
        file << "    \"2y1y\": " << getForwardRate(2.0, 3.0) << ",\\n";
        file << "    \"5y5y\": " << getForwardRate(5.0, 10.0) << ",\\n";
        file << "    \"10y10y\": " << getForwardRate(10.0, 20.0) << "\\n";
        file << "  },\\n";
        file << "  \"economic_indicators\": {\\n";
        double recession_spread = getSpread(2.0, 10.0);
        file << "    \"recession_warning\": " << (recession_spread < -0.2 ? "true" : "false") << ",\\n";
        file << "    \"term_premium_bps\": " << getSpread(10.0, 30.0) * 100 << ",\\n";
        file << "    \"curve_steepness\": \"" << (recession_spread > 1.0 ? "steep" : 
                                                    recession_spread < -0.1 ? "inverted" : "flat") << "\"\\n";
        file << "  }\\n";
        file << "}\\n";
        
        file.close();
        std::cout << "\\n💾 Live yield curve data exported to " << filename << std::endl;
        std::cout << "🌐 Ready for web dashboard integration!" << std::endl;
    }
    
    const std::vector<YieldPoint>& getYieldPoints() const { return yield_points; }
    const std::string& getDate() const { return curve_date; }
    std::string getCurveShape() const { return analyzeCurveShape(); }
};

#endif // YIELDCURVE_LIVE_H
