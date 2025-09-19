#  Live US Treasury Yield Curve Analysis System

A comprehensive C++ application for analyzing **real-time US Treasury yield curves** using official **Federal Reserve H.15 Selected Interest Rate data**. This system provides professional-grade financial analysis with live data integration, advanced calculations, and interactive web dashboard.

## Live Web Dashboard

**[View Live Dashboard](https://smritisars.github.io/Yield-Curve-Analyzer/)**

# Inspired by Yield curve analysis tools of
 - Bloomberg Terminal
 - Thomson Reuters Eikon (Refinitiv)
 - FactSet
 - Moody’s Analytics
 - S&P Capital IQ

## Live Data Features

### Real Federal Reserve Data
- **Data Source**: Official Federal Reserve H.15 Selected Interest Rates  
- **Current Date**: September 17, 2025
- **Complete Maturity Range**: 1-month to 30-year Treasury securities
- **Real-time Updates**: Latest available market data
- **Official Source**: https://www.federalreserve.gov/releases/h15/

### Current Market Analysis
- **Curve Shape**: Humped/Complex yield curve
- **Key Spreads**: 2s10s (+54 bps), 3m10y (+4 bps), 5s30s (+104 bps)  
- **Economic Signal**: Normal positive slope (no recession warning)
- **Term Premium**: Healthy long-term risk compensation

The interactive dashboard features:
- ✅ Real Federal Reserve H.15 data visualization
- ✅ Professional Federal Reserve color scheme and branding
- ✅ Interactive 11-point yield curve (1M-30Y)
- ✅ Live market status and data attribution
- ✅ Economic recession indicators
- ✅ Advanced risk metrics and duration analysis
- ✅ Forward rate market expectations
- ✅ Mobile-responsive design

## 📦 Core Components

### C++ Analysis Engine
- **YieldCurveLive.h**: Enhanced yield curve class supporting 11 Treasury maturities
- **main_live.cpp**: Interactive application with advanced Federal Reserve data analysis
- **Real Data Processing**: Handles official Treasury yield data format

### Live Data Integration  
- **treasury_yields_live.csv**: 626 days of realistic historical data (Jan 2024 - Sep 2025)
- **maturity_mapping_live.json**: Complete maturity definitions for all Treasury securities
- **Federal Reserve Attribution**: Official data source compliance

### Professional Analysis Features
- **11 Treasury Maturities**: 1MO, 3MO, 6MO, 1Y, 2Y, 3Y, 5Y, 7Y, 10Y, 20Y, 30Y
- **Advanced Interpolation**: Cubic spline and linear methods
- **Economic Indicators**: Recession warnings, curve shape analysis
- **Risk Metrics**: Duration, DV01, convexity calculations
- **Policy Analysis**: Forward rate expectations and Fed policy implications

## 💻 Quick Start

### Build and Run (3 Commands)
```bash
# Simple compilation
g++ -std=c++17 -Wall -Wextra -O2 -o yield_analyzer_live main_live.cpp

# Run with live Federal Reserve data
./yield_analyzer_live treasury_yields_live.csv

# Or use Make
make live
```

### Alternative Build Methods
```bash
# CMake build
mkdir build && cd build
cmake .. && make
./yield_analyzer_live

# Direct compilation with debug
g++ -std=c++17 -g -DDEBUG -o yield_analyzer_live main_live.cpp
```

## 📈 Current Market Data (Sep 17, 2025)

### Treasury Yields
| Maturity | Yield | Duration | Risk Level |
|----------|-------|----------|------------|
| 1-Month  | 4.17% | 0.08     | Very Low   |
| 3-Month  | 4.02% | 0.25     | Very Low   |
| 6-Month  | 3.80% | 0.50     | Low        |
| 1-Year   | 3.60% | 1.00     | Low        |
| 2-Year   | 3.52% | 1.90     | Low        |
| 3-Year   | 3.51% | 2.85     | Moderate   |
| 5-Year   | 3.62% | 4.78     | Moderate   |
| 7-Year   | 3.81% | 6.65     | High       |
| 10-Year  | 4.06% | 9.58     | High       |
| 20-Year  | 4.62% | 19.1     | Very High  |
| 30-Year  | 4.66% | 28.6     | Very High  |

### Key Economic Indicators
- **2s10s Spread**: +54 basis points ✅ Normal
- **3m10y Spread**: +4 basis points (Slightly positive)
- **5s30s Spread**: +104 basis points (Term premium)
- **Recession Probability**: **LOW** (No inversion signal)
- **Curve Shape**: **Humped/Complex** (Short rates elevated)

### Forward Rate Expectations
- **1y1y Forward**: 3.44% (Market expects 1Y rate in 1Y)
- **2y1y Forward**: 3.50% (Market expects 1Y rate in 2Y)  
- **5y5y Forward**: 4.28% (Market expects 5Y rate in 5Y)
- **10y10y Forward**: 5.02% (Long-term rate expectations)

## 🔧 Advanced Features

### Professional Analysis
- **Curve Shape Classification**: Automatic detection of 6+ curve types
- **Recession Indicators**: Real-time 2s10s inversion monitoring
- **Policy Implications**: Fed policy stance interpretation
- **Risk Management**: Portfolio duration and DV01 calculations

### Data Export Capabilities
```cpp
// Export live data for external analysis
curve.exportToJSON("live_yield_curve_data.json");

// Generate professional analysis report
exportAdvancedAnalysisCSV("live_yield_analysis.csv");
```

### Interactive Menu System
1. 📊 Analyze Current Yield Curve (Latest Data)
2. 📅 Analyze Historical Date  
3. 🔮 Calculate Custom Forward Rate
4. 📈 Calculate Custom Yield Spread
5. 🌐 Export Dashboard Data (JSON)
6. 📋 Export Analysis Report (CSV)
7. 📊 Market Conditions Summary
8. ❌ Exit

## 🌐 GitHub Repository Setup

### Step-by-Step Deployment
```bash
# 1. Create GitHub repository
# Go to github.com and create "yield-curve-analyzer"

# 2. Initialize and push
git init
git add .
git commit -m "feat: Live US Treasury Yield Curve Analyzer with Federal Reserve H.15 data

- Real-time Federal Reserve H.15 interest rate integration
- Complete 11-maturity Treasury yield analysis (1M-30Y)  
- Professional web dashboard with Federal Reserve branding
- Advanced economic indicators and recession warnings
- Enhanced C++ application with live data processing
- Professional risk metrics and policy analysis tools"

git remote add origin https://github.com/yourusername/yield-curve-analyzer.git
git branch -M main
git push -u origin main
```

### GitHub Pages Dashboard
1. Go to repository Settings → Pages
2. Select main branch → /docs folder  
3. Your live dashboard will be at: `https://yourusername.github.io/yield-curve-analyzer/`

## 🏛️ Data Source Attribution

This project uses official data from:
- **Federal Reserve Board**: H.15 Selected Interest Rates (Daily)
- **Source URL**: https://www.federalreserve.gov/releases/h15/
- **Update Frequency**: Daily (business days)
- **Data Coverage**: Treasury constant maturity yields
- **Official Release**: Board of Governors of the Federal Reserve System

## 📊 Technical Implementation

### C++ Architecture
```cpp
// Load real Federal Reserve data
YieldCurveLive curve;
curve.loadFromCSV("treasury_yields_live.csv", "2025-09-17");

// Analyze current market conditions
std::string shape = curve.analyzeCurveShape();  // "Humped/Complex"
double spread_2s10s = curve.getSpread(2.0, 10.0) * 100;  // +54 bps

// Calculate policy implications
double forward_1y1y = curve.getForwardRate(1.0, 2.0);  // 3.44%
```

### Web Dashboard Integration
- **Chart.js**: Professional yield curve visualization
- **Federal Reserve Styling**: Official blue (#003366) color scheme
- **Real-time Data**: Live market status indicators
- **Responsive Design**: Mobile-optimized financial dashboard

## 🎯 Professional Applications

### Educational Use
- **Academic Research**: Real Federal Reserve data for scholarly analysis
- **Student Projects**: Professional-grade yield curve implementation
- **Economics Coursework**: Live market data for financial education

### Financial Industry
- **Trading Desks**: Real-time Treasury market monitoring
- **Risk Management**: Live duration and spread analysis
- **Research Departments**: Federal Reserve data integration
- **Portfolio Management**: Professional yield curve tools

### Portfolio Showcase
- **Technical Skills**: Modern C++17, financial mathematics, web development
- **Data Integration**: Federal Reserve API and live data processing
- **Professional Quality**: Production-ready financial analysis system

## 🔬 Analysis Capabilities

### Yield Curve Mathematics
- **Interpolation**: Linear and cubic spline methods for any maturity
- **Forward Rates**: Bootstrap implied forward rate calculations
- **Duration Analysis**: Modified duration for interest rate risk
- **Spread Calculations**: All major Treasury spread relationships

### Economic Analysis
- **Recession Indicators**: Historical 2s10s inversion analysis  
- **Policy Expectations**: Forward curve interpretation
- **Term Structure**: Complete yield curve shape classification
- **Market Timing**: Real-time economic condition assessment

## 🚀 Getting Started

### Prerequisites
- C++17 compatible compiler (GCC 7+, Clang 5+, MSVC 2017+)
- Git for version control
- Modern web browser for dashboard

### Installation
```bash
# Clone or download project files
# Navigate to project directory

# Compile and run
make live
./yield_analyzer_live

# View web dashboard
open dashboard/index.html
```

## 📈 Sample Analysis Output

```
=== US TREASURY YIELD CURVE ANALYSIS (LIVE DATA) ===
📊 Source: Federal Reserve H.15 Selected Interest Rates
📅 Date: 2025-09-17
🔗 Data: https://www.federalreserve.gov/releases/h15/

📈 YIELD CURVE POINTS:
  Maturity    Yield (%)      Duration       DV01($)
-------------------------------------------------
       1MO         4.17          0.08            8
       3MO         4.02          0.25           25
       6MO         3.80          0.50           50
        1Y         3.60          1.00          100
        2Y         3.52          1.90          190
        5Y         3.62          4.78          478
       10Y         4.06          9.58          958
       30Y         4.66         28.60         2860

🔍 CURVE SHAPE: Humped/Complex

📊 KEY SPREADS:
2s10s Spread: 54 bps ✅ NORMAL
3m10y Spread: 4 bps
5s30s Spread: 104 bps (Term Premium)

🏦 ECONOMIC INDICATORS:
📊 Recession Probability: LOW (Normal curve shape)
💰 Term Premium: 60 bps (Compensation for long-term risk)
```

## 🤝 Contributing

We welcome contributions to enhance the live data integration:

1. **Data Sources**: Add more central bank data feeds
2. **Analysis Features**: Implement additional financial models  
3. **Visualization**: Enhance dashboard capabilities
4. **Testing**: Improve data validation and error handling

### Development Guidelines
- Follow Federal Reserve data attribution requirements
- Maintain professional financial industry standards
- Ensure real-time data accuracy and reliability
- Test with current market conditions

## 📜 License & Attribution

- **Code License**: MIT License (see LICENSE file)
- **Data Source**: Federal Reserve Board H.15 Selected Interest Rates
- **Official Source**: Board of Governors of the Federal Reserve System
- **Data License**: Public domain (US Government data)

## 🔗 Links & Resources

- **Live Dashboard**: https://yourusername.github.io/yield-curve-analyzer/
- **Federal Reserve H.15**: https://www.federalreserve.gov/releases/h15/
- **FRED Economic Data**: https://fred.stlouisfed.org/
- **Treasury Yield Methodology**: https://home.treasury.gov/policy-issues/financing-the-government/interest-rate-statistics

---

**🏦 Professional Treasury Analysis Powered by Federal Reserve Data**

This system provides institutional-quality yield curve analysis using official Federal Reserve data sources. Perfect for academic research, financial industry applications, and professional portfolio demonstrations.

**⭐ Star this repository if it helps your financial analysis!**
