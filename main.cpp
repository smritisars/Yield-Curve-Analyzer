#include "YieldCurve.h"
#include <iostream>
#include <string>
#include <vector>
#include <iomanip>

class YieldCurveAnalyzer {
private:
    YieldCurve curve;

public:
    YieldCurveAnalyzer() = default;

    bool initialize(const std::string& csv_file, const std::string& date = "") {
        std::cout << "=== US Treasury Yield Curve Analyzer ===" << std::endl;
        std::cout << "Loading Treasury yield data..." << std::endl;

        if (!curve.loadFromCSV(csv_file, date)) {
            std::cerr << "Failed to load yield curve data from " << csv_file << std::endl;
            return false;
        }

        std::cout << "Successfully loaded yield curve data." << std::endl;
        return true;
    }

    void runFullAnalysis() {
        // Display the yield curve
        curve.printCurve();

        // Perform additional analysis
        performCurveAnalysis();

        // Export data for dashboard
        curve.exportToJSON("yield_curve_data.json");

        // Create CSV output for further analysis
        exportAnalysisCSV();
    }

    void performCurveAnalysis() {
        std::cout << "\n=== Advanced Yield Curve Analysis ===" << std::endl;

        // Curve shape analysis
        analyzeCurveShape();

        // Interest rate risk metrics
        calculateRiskMetrics();

        // Economic indicators
        analyzeEconomicIndicators();
    }

private:
    void analyzeCurveShape() {
        std::cout << "\n--- Curve Shape Analysis ---" << std::endl;

        double short_rate = curve.getYield(0.25);
        double medium_rate = curve.getYield(5.0);
        double long_rate = curve.getYield(30.0);

        // Determine curve shape
        std::string shape;
        if (long_rate > medium_rate && medium_rate > short_rate) {
            shape = "Normal (Upward Sloping)";
        } else if (short_rate > medium_rate && medium_rate > long_rate) {
            shape = "Inverted (Downward Sloping)";
        } else {
            shape = "Flat/Humped";
        }

        std::cout << "Curve Shape: " << shape << std::endl;
        std::cout << "Short-term (3M): " << std::setprecision(2) << short_rate << "%" << std::endl;
        std::cout << "Medium-term (5Y): " << std::setprecision(2) << medium_rate << "%" << std::endl;
        std::cout << "Long-term (30Y): " << std::setprecision(2) << long_rate << "%" << std::endl;

        // Slope calculation
        double slope_2s10s = curve.getSpread(2.0, 10.0);
        std::cout << "Yield Curve Slope (2s10s): " << std::setprecision(0) 
                  << slope_2s10s * 100 << " bps";

        if (slope_2s10s > 0.5) {
            std::cout << " (Steep)" << std::endl;
        } else if (slope_2s10s < -0.2) {
            std::cout << " (Inverted)" << std::endl;
        } else {
            std::cout << " (Flat)" << std::endl;
        }
    }

    void calculateRiskMetrics() {
        std::cout << "\n--- Interest Rate Risk Metrics ---" << std::endl;

        // Calculate duration for key maturities
        std::vector<double> maturities = {2.0, 5.0, 10.0, 30.0};
        std::cout << std::setw(10) << "Maturity" << std::setw(12) << "Duration" 
                  << std::setw(15) << "DV01 ($)" << std::endl;
        std::cout << std::string(37, '-') << std::endl;

        for (double mat : maturities) {
            double duration = curve.getDuration(mat);
            double dv01 = duration * 100; // Approximate DV01 for $10,000 face value

            std::cout << std::setw(8) << std::setprecision(0) << mat << "Y" 
                      << std::setw(12) << std::setprecision(2) << duration
                      << std::setw(15) << std::setprecision(0) << dv01 << std::endl;
        }
    }

    void analyzeEconomicIndicators() {
        std::cout << "\n--- Economic Indicators ---" << std::endl;

        // Recession indicator (2s10s spread)
        double recession_spread = curve.getSpread(2.0, 10.0);
        std::cout << "Recession Indicator (2s10s): ";
        if (recession_spread < -0.2) {
            std::cout << "WARNING - Yield curve inversion detected!" << std::endl;
        } else if (recession_spread < 0.5) {
            std::cout << "CAUTION - Yield curve flattening" << std::endl;
        } else {
            std::cout << "Normal - Positive slope" << std::endl;
        }

        // Term premium analysis
        double term_premium = curve.getYield(30.0) - curve.getYield(10.0);
        std::cout << "Term Premium (30Y-10Y): " << std::setprecision(0) 
                  << term_premium * 100 << " bps" << std::endl;

        // Market expectations (forward rates)
        std::cout << "Market Expectations:" << std::endl;
        std::cout << "  Expected 1Y rate in 1Y: " << std::setprecision(2) 
                  << curve.getForwardRate(1.0, 2.0) << "%" << std::endl;
        std::cout << "  Expected 1Y rate in 5Y: " << std::setprecision(2) 
                  << curve.getForwardRate(5.0, 6.0) << "%" << std::endl;
    }

    void exportAnalysisCSV() {
        std::ofstream file("yield_analysis.csv");
        if (!file.is_open()) {
            std::cerr << "Error: Could not create yield_analysis.csv" << std::endl;
            return;
        }

        // Header
        file << "Analysis_Date,Maturity_Label,Maturity_Years,Yield_Pct,Duration,Forward_1Y\n";

        // Data rows
        std::string date = curve.getDate();
        for (const auto& point : curve.getYieldPoints()) {
            double forward_1y = 0.0;
            if (point.maturity >= 1.0) {
                forward_1y = curve.getForwardRate(point.maturity, point.maturity + 1.0);
            }

            file << date << "," << point.maturity_label << "," 
                 << point.maturity << "," << point.yield << ","
                 << curve.getDuration(point.maturity) << ","
                 << forward_1y << "\n";
        }

        file.close();
        std::cout << "\nDetailed analysis exported to yield_analysis.csv" << std::endl;
    }
};

// Interactive menu system
void displayMenu() {
    std::cout << "\n=== Yield Curve Analysis Menu ===" << std::endl;
    std::cout << "1. Load and analyze current yield curve" << std::endl;
    std::cout << "2. Load yield curve for specific date" << std::endl;
    std::cout << "3. Calculate custom forward rate" << std::endl;
    std::cout << "4. Calculate custom yield spread" << std::endl;
    std::cout << "5. Export dashboard data" << std::endl;
    std::cout << "6. Exit" << std::endl;
    std::cout << "\nEnter your choice (1-6): ";
}

int main(int argc, char* argv[]) {
    YieldCurveAnalyzer analyzer;
    std::string csv_filename = "treasury_yields.csv";

    // Check if CSV file is provided as command line argument
    if (argc > 1) {
        csv_filename = argv[1];
    }

    std::cout << "US Treasury Yield Curve Analysis System" << std::endl;
    std::cout << "=======================================" << std::endl;

    int choice;
    bool running = true;

    while (running) {
        displayMenu();
        std::cin >> choice;

        switch (choice) {
            case 1: {
                if (analyzer.initialize(csv_filename)) {
                    analyzer.runFullAnalysis();
                }
                break;
            }

            case 2: {
                std::cout << "Enter date (YYYY-MM-DD format): ";
                std::string date;
                std::cin >> date;

                if (analyzer.initialize(csv_filename, date)) {
                    analyzer.runFullAnalysis();
                }
                break;
            }

            case 3: {
                if (analyzer.initialize(csv_filename)) {
                    double start_mat, end_mat;
                    std::cout << "Enter start maturity (years): ";
                    std::cin >> start_mat;
                    std::cout << "Enter end maturity (years): ";
                    std::cin >> end_mat;

                    YieldCurve temp_curve;
                    temp_curve.loadFromCSV(csv_filename);
                    double forward = temp_curve.getForwardRate(start_mat, end_mat);

                    std::cout << "Forward rate from " << start_mat << "Y to " 
                              << end_mat << "Y: " << std::setprecision(2) 
                              << forward << "%" << std::endl;
                }
                break;
            }

            case 4: {
                if (analyzer.initialize(csv_filename)) {
                    double mat1, mat2;
                    std::cout << "Enter first maturity (years): ";
                    std::cin >> mat1;
                    std::cout << "Enter second maturity (years): ";
                    std::cin >> mat2;

                    YieldCurve temp_curve;
                    temp_curve.loadFromCSV(csv_filename);
                    double spread = temp_curve.getSpread(mat1, mat2);

                    std::cout << "Yield spread (" << mat1 << "Y - " << mat2 
                              << "Y): " << std::setprecision(0) << spread * 100 
                              << " basis points" << std::endl;
                }
                break;
            }

            case 5: {
                if (analyzer.initialize(csv_filename)) {
                    YieldCurve temp_curve;
                    temp_curve.loadFromCSV(csv_filename);
                    temp_curve.exportToJSON("yield_curve_data.json");
                    std::cout << "Dashboard data exported successfully!" << std::endl;
                }
                break;
            }

            case 6: {
                running = false;
                std::cout << "Thank you for using the Yield Curve Analyzer!" << std::endl;
                break;
            }

            default: {
                std::cout << "Invalid choice. Please enter 1-6." << std::endl;
                break;
            }
        }

        if (choice != 6) {
            std::cout << "\nPress Enter to continue...";
            std::cin.ignore();
            std::cin.get();
        }
    }

    return 0;
}
