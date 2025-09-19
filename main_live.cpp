#include "YieldCurveLive.h"
#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include <chrono>
#include <ctime>

class LiveTreasuryAnalyzer {
private:
    YieldCurveLive curve;

public:
    LiveTreasuryAnalyzer() = default;

    void displayWelcome() {
        std::cout << "\n" << std::string(60, '=') << std::endl;
        std::cout << "🏦 US TREASURY YIELD CURVE ANALYZER (LIVE DATA)" << std::endl;
        std::cout << "📊 Real-time Federal Reserve H.15 Interest Rate Data" << std::endl;
        std::cout << std::string(60, '=') << std::endl;

        // Show current timestamp
        auto now = std::chrono::system_clock::now();
        auto time_t = std::chrono::system_clock::to_time_t(now);
        std::cout << "🕐 Analysis Time: " << std::put_time(std::localtime(&time_t), "%Y-%m-%d %H:%M:%S") << std::endl;
        std::cout << "🔗 Data Source: https://www.federalreserve.gov/releases/h15/" << std::endl;
        std::cout << std::string(60, '-') << std::endl;
    }

    bool initialize(const std::string& csv_file, const std::string& date = "") {
        std::cout << "\n📂 Loading live Treasury yield data..." << std::endl;

        if (!curve.loadFromCSV(csv_file, date)) {
            std::cerr << "❌ Failed to load yield curve data from " << csv_file << std::endl;
            std::cerr << "💡 Please ensure the file exists and contains valid Treasury data." << std::endl;
            return false;
        }

        std::cout << "✅ Successfully loaded live Federal Reserve data!" << std::endl;
        std::cout << "📈 Curve Date: " << curve.getDate() << std::endl;
        std::cout << "🏛️  Shape: " << curve.getCurveShape() << std::endl;
        return true;
    }

    void runFullAnalysis() {
        std::cout << "\n🔬 PERFORMING COMPREHENSIVE YIELD CURVE ANALYSIS..." << std::endl;

        // Display the complete yield curve
        curve.printCurve();

        // Perform advanced analysis
        performAdvancedAnalysis();

        // Export data for dashboard
        curve.exportToJSON("live_yield_curve_data.json");

        // Create CSV output for further analysis
        exportAdvancedAnalysisCSV();

        std::cout << "\n✅ ANALYSIS COMPLETE!" << std::endl;
        std::cout << "📊 Dashboard data: live_yield_curve_data.json" << std::endl;
        std::cout << "📋 Analysis CSV: live_yield_analysis.csv" << std::endl;
    }

    void performAdvancedAnalysis() {
        std::cout << "\n" << std::string(50, '=') << std::endl;
        std::cout << "🔬 ADVANCED TREASURY MARKET ANALYSIS" << std::endl;
        std::cout << std::string(50, '=') << std::endl;

        // Detailed curve analysis
        analyzeMarketConditions();

        // Volatility and risk analysis
        analyzeInterestRateRisk();

        // Policy implications
        analyzePolicyImplications();
    }

private:
    void analyzeMarketConditions() {
        std::cout << "\n📊 CURRENT MARKET CONDITIONS:" << std::endl;
        std::cout << std::string(35, '-') << std::endl;

        // Get key rates
        double fed_funds_proxy = curve.getYield(1.0/12.0); // 1-month as proxy
        double short_rate = curve.getYield(0.25);           // 3-month
        double benchmark = curve.getYield(10.0);            // 10-year benchmark
        double long_rate = curve.getYield(30.0);            // 30-year

        std::cout << "Policy Rate (1M): " << std::setprecision(2) << fed_funds_proxy << "%" << std::endl;
        std::cout << "Short Rate (3M): " << short_rate << "%" << std::endl;
        std::cout << "Benchmark (10Y): " << benchmark << "%" << std::endl;
        std::cout << "Long Rate (30Y): " << long_rate << "%" << std::endl;

        // Market interpretation
        std::cout << "\n💹 MARKET INTERPRETATION:" << std::endl;
        double curve_slope = curve.getSpread(0.25, 10.0);

        if (curve_slope < -0.5) {
            std::cout << "🚨 DEEPLY INVERTED - Strong recession signal" << std::endl;
            std::cout << "📉 Markets pricing aggressive rate cuts ahead" << std::endl;
        } else if (curve_slope < 0) {
            std::cout << "⚠️  INVERTED - Economic slowdown concerns" << std::endl;
            std::cout << "🔄 Potential policy reversal expected" << std::endl;
        } else if (curve_slope < 0.5) {
            std::cout << "📊 FLAT - Neutral policy stance" << std::endl;
            std::cout << "⚖️  Balanced growth and inflation expectations" << std::endl;
        } else if (curve_slope > 2.0) {
            std::cout << "📈 VERY STEEP - Expansionary conditions" << std::endl;
            std::cout << "🚀 Strong growth and inflation expectations" << std::endl;
        } else {
            std::cout << "✅ NORMAL - Healthy economic expectations" << std::endl;
            std::cout << "📊 Balanced monetary policy stance" << std::endl;
        }

        // Volatility assessment
        std::cout << "\n📊 YIELD VOLATILITY ASSESSMENT:" << std::endl;
        double short_vol = std::abs(curve.getYield(0.25) - curve.getYield(1.0));
        double long_vol = std::abs(curve.getYield(10.0) - curve.getYield(30.0));

        std::cout << "Short-end spread (3M-1Y): " << std::setprecision(0) 
                  << short_vol * 100 << " bps" << std::endl;
        std::cout << "Long-end spread (10Y-30Y): " << std::setprecision(0) 
                  << long_vol * 100 << " bps" << std::endl;
    }

    void analyzeInterestRateRisk() {
        std::cout << "\n⚡ INTEREST RATE RISK ANALYSIS:" << std::endl;
        std::cout << std::string(35, '-') << std::endl;

        std::vector<double> key_maturities = {2.0, 5.0, 10.0, 30.0};
        std::vector<std::string> labels = {"2Y", "5Y", "10Y", "30Y"};

        std::cout << std::setw(8) << "Tenor" << std::setw(10) << "Yield%" 
                  << std::setw(12) << "Duration" << std::setw(10) << "DV01$" 
                  << std::setw(12) << "Risk Level" << std::endl;
        std::cout << std::string(52, '-') << std::endl;

        for (size_t i = 0; i < key_maturities.size(); i++) {
            double maturity = key_maturities[i];
            double yield = curve.getYield(maturity);
            double duration = curve.getDuration(maturity);
            double dv01 = duration * 100; // Per $10,000 face value

            std::string risk_level;
            if (duration < 2) risk_level = "LOW";
            else if (duration < 7) risk_level = "MODERATE";
            else if (duration < 15) risk_level = "HIGH";
            else risk_level = "VERY HIGH";

            std::cout << std::setw(8) << labels[i] 
                      << std::setw(10) << std::setprecision(2) << yield
                      << std::setw(12) << std::setprecision(1) << duration
                      << std::setw(10) << std::setprecision(0) << dv01
                      << std::setw(12) << risk_level << std::endl;
        }

        std::cout << "\n💡 Portfolio Implications:" << std::endl;
        std::cout << "• Short-term bonds: Lower risk, rate-sensitive positioning" << std::endl;
        std::cout << "• Long-term bonds: Higher risk, duration exposure" << std::endl;
        std::cout << "• Barbell strategy: Combine short and long maturities" << std::endl;
    }

    void analyzePolicyImplications() {
        std::cout << "\n🏛️  MONETARY POLICY IMPLICATIONS:" << std::endl;
        std::cout << std::string(35, '-') << std::endl;

        // Analyze forward rate expectations
        double near_forward = curve.getForwardRate(0.25, 1.25); // 3M-15M forward
        double medium_forward = curve.getForwardRate(1.0, 3.0);  // 1Y-3Y forward
        double long_forward = curve.getForwardRate(5.0, 10.0);   // 5Y-10Y forward

        std::cout << "📈 MARKET EXPECTATIONS (Forward Rates):" << std::endl;
        std::cout << "Near-term (3M-15M): " << std::setprecision(2) << near_forward << "%" << std::endl;
        std::cout << "Medium-term (1Y-3Y): " << medium_forward << "%" << std::endl;
        std::cout << "Long-term (5Y-10Y): " << long_forward << "%" << std::endl;

        // Policy interpretation
        double current_short = curve.getYield(0.25);
        std::cout << "\n🔮 POLICY OUTLOOK:" << std::endl;

        if (near_forward < current_short - 0.5) {
            std::cout << "📉 Markets expect AGGRESSIVE rate cuts" << std::endl;
            std::cout << "⚠️  Economic stress or recession fears" << std::endl;
        } else if (near_forward < current_short - 0.1) {
            std::cout << "📊 Markets expect MODEST rate cuts" << std::endl;
            std::cout << "🔄 Policy easing cycle anticipated" << std::endl;
        } else if (near_forward > current_short + 0.1) {
            std::cout << "📈 Markets expect rate INCREASES" << std::endl;
            std::cout << "🔥 Inflation concerns driving policy" << std::endl;
        } else {
            std::cout << "⚖️  Markets expect STABLE rates" << std::endl;
            std::cout << "✅ Current policy stance appropriate" << std::endl;
        }

        // Term premium analysis
        double term_premium = curve.getYield(30.0) - curve.getYield(10.0);
        std::cout << "\n💰 TERM PREMIUM: " << std::setprecision(0) << term_premium * 100 << " bps" << std::endl;

        if (term_premium < 0.2) {
            std::cout << "📊 LOW - Minimal long-term risk compensation" << std::endl;
        } else if (term_premium > 0.8) {
            std::cout << "📈 HIGH - Significant long-term uncertainty" << std::endl;
        } else {
            std::cout << "✅ NORMAL - Balanced long-term expectations" << std::endl;
        }
    }

    void exportAdvancedAnalysisCSV() {
        std::ofstream file("live_yield_analysis.csv");
        if (!file.is_open()) {
            std::cerr << "Error: Could not create live_yield_analysis.csv" << std::endl;
            return;
        }

        // Enhanced CSV header
        file << "Analysis_Date,Data_Source,Maturity_Label,Maturity_Years,Yield_Pct,"
             << "Duration,DV01,Forward_1Y,Risk_Level,Notes\n";

        // Data rows with enhanced analysis
        std::string date = curve.getDate();
        for (const auto& point : curve.getYieldPoints()) {
            double forward_1y = 0.0;
            if (point.maturity >= 1.0) {
                forward_1y = curve.getForwardRate(point.maturity, point.maturity + 1.0);
            }

            double duration = curve.getDuration(point.maturity);
            double dv01 = duration * 100;

            std::string risk_level = (duration < 2) ? "LOW" : 
                                   (duration < 7) ? "MODERATE" : 
                                   (duration < 15) ? "HIGH" : "VERY_HIGH";

            std::string notes = "Federal_Reserve_H15_Official_Data";

            file << date << ",Federal_Reserve_H15," << point.maturity_label << "," 
                 << point.maturity << "," << point.yield << ","
                 << duration << "," << dv01 << ","
                 << forward_1y << "," << risk_level << "," << notes << "\n";
        }

        file.close();
        std::cout << "\n💾 Advanced analysis exported to live_yield_analysis.csv" << std::endl;
    }
};

// Enhanced interactive menu system
void displayMainMenu() {
    std::cout << "\n" << std::string(50, '=') << std::endl;
    std::cout << "🏦 LIVE TREASURY YIELD CURVE ANALYSIS MENU" << std::endl;
    std::cout << std::string(50, '=') << std::endl;
    std::cout << "1. 📊 Analyze Current Yield Curve (Latest Data)" << std::endl;
    std::cout << "2. 📅 Analyze Historical Date" << std::endl;
    std::cout << "3. 🔮 Calculate Custom Forward Rate" << std::endl;
    std::cout << "4. 📈 Calculate Custom Yield Spread" << std::endl;
    std::cout << "5. 🌐 Export Dashboard Data (JSON)" << std::endl;
    std::cout << "6. 📋 Export Analysis Report (CSV)" << std::endl;
    std::cout << "7. 📊 Market Conditions Summary" << std::endl;
    std::cout << "8. ❌ Exit" << std::endl;
    std::cout << std::string(50, '-') << std::endl;
    std::cout << "👉 Enter your choice (1-8): ";
}

void displayMarketSummary(const YieldCurveLive& curve) {
    std::cout << "\n📊 QUICK MARKET SUMMARY:" << std::endl;
    std::cout << std::string(30, '-') << std::endl;

    // Key rates
    std::cout << "🔑 Key Rates:" << std::endl;
    std::cout << "  3M: " << std::setprecision(2) << curve.getYield(0.25) << "%" << std::endl;
    std::cout << "  2Y: " << curve.getYield(2.0) << "%" << std::endl;
    std::cout << "  10Y: " << curve.getYield(10.0) << "%" << std::endl;
    std::cout << "  30Y: " << curve.getYield(30.0) << "%" << std::endl;

    // Key spread
    double spread_2s10s = (curve.getYield(10.0) - curve.getYield(2.0)) * 100;
    std::cout << "\n📏 2s10s Spread: " << std::setprecision(0) << spread_2s10s << " bps";

    if (spread_2s10s < -20) {
        std::cout << " 🚨 RECESSION ALERT!" << std::endl;
    } else if (spread_2s10s < 0) {
        std::cout << " ⚠️ INVERTED" << std::endl;
    } else {
        std::cout << " ✅ NORMAL" << std::endl;
    }

    std::cout << "🏛️ Shape: " << curve.getCurveShape() << std::endl;
}

int main(int argc, char* argv[]) {
    LiveTreasuryAnalyzer analyzer;
    std::string csv_filename = "treasury_yields_live.csv";

    // Check if CSV file is provided as command line argument
    if (argc > 1) {
        csv_filename = argv[1];
    }

    analyzer.displayWelcome();

    int choice;
    bool running = true;

    while (running) {
        displayMainMenu();
        std::cin >> choice;

        switch (choice) {
            case 1: {
                if (analyzer.initialize(csv_filename)) {
                    analyzer.runFullAnalysis();
                }
                break;
            }

            case 2: {
                std::cout << "📅 Enter date (YYYY-MM-DD format): ";
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
                    std::cout << "📊 Enter start maturity (years): ";
                    std::cin >> start_mat;
                    std::cout << "📊 Enter end maturity (years): ";
                    std::cin >> end_mat;

                    YieldCurveLive temp_curve;
                    temp_curve.loadFromCSV(csv_filename);
                    double forward = temp_curve.getForwardRate(start_mat, end_mat);

                    std::cout << "🔮 Forward rate from " << start_mat << "Y to " 
                              << end_mat << "Y: " << std::setprecision(2) 
                              << forward << "%" << std::endl;
                    std::cout << "💡 Market expects " << forward << "% " << (end_mat - start_mat) 
                              << "-year rate in " << start_mat << " years" << std::endl;
                }
                break;
            }

            case 4: {
                if (analyzer.initialize(csv_filename)) {
                    double mat1, mat2;
                    std::cout << "📊 Enter first maturity (years): ";
                    std::cin >> mat1;
                    std::cout << "📊 Enter second maturity (years): ";
                    std::cin >> mat2;

                    YieldCurveLive temp_curve;
                    temp_curve.loadFromCSV(csv_filename);
                    double spread = temp_curve.getSpread(mat1, mat2);

                    std::cout << "📈 Yield spread (" << mat2 << "Y - " << mat1 
                              << "Y): " << std::setprecision(0) << spread * 100 
                              << " basis points" << std::endl;

                    if (std::abs(mat1 - 2.0) < 0.1 && std::abs(mat2 - 10.0) < 0.1 && spread < -0.2) {
                        std::cout << "🚨 WARNING: This is the key recession indicator!" << std::endl;
                    }
                }
                break;
            }

            case 5: {
                if (analyzer.initialize(csv_filename)) {
                    YieldCurveLive temp_curve;
                    temp_curve.loadFromCSV(csv_filename);
                    temp_curve.exportToJSON("live_yield_curve_data.json");
                    std::cout << "🌐 Dashboard data exported successfully!" << std::endl;
                    std::cout << "📊 File: live_yield_curve_data.json" << std::endl;
                }
                break;
            }

            case 6: {
                if (analyzer.initialize(csv_filename)) {
                    analyzer.runFullAnalysis(); // This includes CSV export
                }
                break;
            }

            case 7: {
                if (analyzer.initialize(csv_filename)) {
                    YieldCurveLive temp_curve;
                    temp_curve.loadFromCSV(csv_filename);
                    displayMarketSummary(temp_curve);
                }
                break;
            }

            case 8: {
                running = false;
                std::cout << "\n🏦 Thank you for using the Live Treasury Yield Curve Analyzer!" << std::endl;
                std::cout << "📊 Data source: Federal Reserve H.15 Selected Interest Rates" << std::endl;
                std::cout << "🔗 https://www.federalreserve.gov/releases/h15/" << std::endl;
                break;
            }

            default: {
                std::cout << "❌ Invalid choice. Please enter 1-8." << std::endl;
                break;
            }
        }

        if (choice != 8) {
            std::cout << "\n⏸️  Press Enter to continue...";
            std::cin.ignore();
            std::cin.get();
        }
    }

    return 0;
}
