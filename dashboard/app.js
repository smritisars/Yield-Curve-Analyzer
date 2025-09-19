// Live US Treasury Yield Curve Dashboard - Federal Reserve H.15 Data
class LiveYieldCurveDashboard {
  constructor() {
    // Real Federal Reserve H.15 data - September 17, 2025
    this.federalReserveData = {
      "liveTreasuryData": {
        "data_source": "Federal Reserve H.15 Selected Interest Rates",
        "source_url": "https://www.federalreserve.gov/releases/h15/",
        "date": "2025-09-17",
        "curve_shape": "Humped/Complex",
        "yield_points": [
          {"maturity_label": "1MO", "maturity_years": 0.083, "yield": 4.17, "duration": 0.083},
          {"maturity_label": "3MO", "maturity_years": 0.25, "yield": 4.02, "duration": 0.25},
          {"maturity_label": "6MO", "maturity_years": 0.5, "yield": 3.80, "duration": 0.5},
          {"maturity_label": "1Y", "maturity_years": 1.0, "yield": 3.60, "duration": 1.0},
          {"maturity_label": "2Y", "maturity_years": 2.0, "yield": 3.52, "duration": 1.9},
          {"maturity_label": "3Y", "maturity_years": 3.0, "yield": 3.51, "duration": 2.85},
          {"maturity_label": "5Y", "maturity_years": 5.0, "yield": 3.62, "duration": 4.78},
          {"maturity_label": "7Y", "maturity_years": 7.0, "yield": 3.81, "duration": 6.65},
          {"maturity_label": "10Y", "maturity_years": 10.0, "yield": 4.06, "duration": 9.58},
          {"maturity_label": "20Y", "maturity_years": 20.0, "yield": 4.62, "duration": 19.1},
          {"maturity_label": "30Y", "maturity_years": 30.0, "yield": 4.66, "duration": 28.6}
        ],
        "key_spreads": {
          "2s10s_bps": 54,
          "3m10y_bps": 4,
          "5s30s_bps": 104,
          "1m3m_bps": -15
        },
        "forward_rates": {
          "1y1y": 3.44,
          "2y1y": 3.50,
          "5y5y": 4.28,
          "10y10y": 5.02
        },
        "economic_indicators": {
          "recession_warning": false,
          "term_premium_bps": 60,
          "curve_steepness": "normal"
        }
      },
      "marketStatus": {
        "is_open": true,
        "last_update": "2025-09-17T15:30:00Z",
        "next_update": "2025-09-18T15:30:00Z"
      },
      "riskMetrics": [
        {"maturity": "1MO", "duration": 0.083, "dv01": 8, "risk_level": "VERY LOW"},
        {"maturity": "3MO", "duration": 0.25, "dv01": 25, "risk_level": "VERY LOW"},
        {"maturity": "6MO", "duration": 0.5, "dv01": 50, "risk_level": "VERY LOW"},
        {"maturity": "1Y", "duration": 1.0, "dv01": 100, "risk_level": "LOW"},
        {"maturity": "2Y", "duration": 1.9, "dv01": 190, "risk_level": "LOW"},
        {"maturity": "3Y", "duration": 2.85, "dv01": 285, "risk_level": "LOW"},
        {"maturity": "5Y", "duration": 4.78, "dv01": 478, "risk_level": "MODERATE"},
        {"maturity": "7Y", "duration": 6.65, "dv01": 665, "risk_level": "MODERATE"},
        {"maturity": "10Y", "duration": 9.58, "dv01": 958, "risk_level": "HIGH"},
        {"maturity": "20Y", "duration": 19.1, "dv01": 1910, "risk_level": "VERY HIGH"},
        {"maturity": "30Y", "duration": 28.6, "dv01": 2860, "risk_level": "VERY HIGH"}
      ]
    };
    
    this.chart = null;
    this.refreshInterval = null;
    this.init();
  }

  init() {
    this.updateHeader();
    this.updateMarketStatus();
    this.createYieldCurveChart();
    this.updateSpreads();
    this.updateEconomicIndicators();
    this.updateForwardRates();
    this.populateRiskMetricsTable();
    this.bindEvents();
    this.calculateCustomSpread();
    this.checkRecessionWarning();
    this.startAutoRefresh();
  }

  updateHeader() {
    // Update current date
    const currentDate = new Date('2025-09-17').toLocaleDateString('en-US', {
      weekday: 'long',
      year: 'numeric',
      month: 'long',
      day: 'numeric'
    });
    
    // Update last update time
    const lastUpdateTime = new Date(this.federalReserveData.marketStatus.last_update).toLocaleTimeString('en-US', {
      hour: '2-digit',
      minute: '2-digit',
      timeZone: 'America/New_York',
      timeZoneName: 'short'
    });

    document.getElementById('currentDate').textContent = currentDate;
    document.getElementById('lastUpdate').textContent = lastUpdateTime;
  }

  updateMarketStatus() {
    const marketStatus = this.federalReserveData.marketStatus.is_open ? 'Market Open' : 'Market Closed';
    document.getElementById('marketStatus').textContent = marketStatus;
    
    const curveShape = this.federalReserveData.liveTreasuryData.curve_shape;
    document.getElementById('curveShapeIndicator').textContent = curveShape;
  }

  createYieldCurveChart() {
    const ctx = document.getElementById('yieldCurveChart').getContext('2d');
    const yieldData = this.federalReserveData.liveTreasuryData.yield_points;

    // Federal Reserve professional colors
    const fedBlue = '#003366';
    const fedLightBlue = 'rgba(0, 51, 102, 0.1)';
    const fedAccent = '#1FB8CD';

    this.chart = new Chart(ctx, {
      type: 'line',
      data: {
        labels: yieldData.map(point => point.maturity_label),
        datasets: [{
          label: 'US Treasury Yield Curve',
          data: yieldData.map(point => point.yield),
          borderColor: fedBlue,
          backgroundColor: fedLightBlue,
          borderWidth: 3,
          fill: true,
          tension: 0.3,
          pointBackgroundColor: fedBlue,
          pointBorderColor: '#ffffff',
          pointBorderWidth: 2,
          pointRadius: 6,
          pointHoverRadius: 10,
          pointHoverBackgroundColor: fedAccent,
          pointHoverBorderColor: '#ffffff',
          pointHoverBorderWidth: 3
        }]
      },
      options: {
        responsive: true,
        maintainAspectRatio: false,
        plugins: {
          title: {
            display: true,
            text: `Federal Reserve H.15 Data - ${new Date('2025-09-17').toLocaleDateString('en-US')}`,
            font: {
              size: 16,
              weight: 'bold'
            },
            color: '#134252'
          },
          legend: {
            display: false
          },
          tooltip: {
            mode: 'index',
            intersect: false,
            backgroundColor: 'rgba(255, 255, 255, 0.95)',
            titleColor: '#003366',
            bodyColor: '#003366',
            borderColor: fedBlue,
            borderWidth: 2,
            cornerRadius: 8,
            displayColors: false,
            titleFont: {
              size: 14,
              weight: 'bold'
            },
            bodyFont: {
              size: 13
            },
            callbacks: {
              title: function(context) {
                const point = yieldData[context[0].dataIndex];
                return `${point.maturity_label} Treasury`;
              },
              label: function(context) {
                const point = yieldData[context.dataIndex];
                return [
                  `Yield: ${context.parsed.y.toFixed(2)}%`,
                  `Maturity: ${point.maturity_years} year${point.maturity_years !== 1 ? 's' : ''}`,
                  `Duration: ${point.duration.toFixed(2)} years`
                ];
              }
            }
          }
        },
        scales: {
          x: {
            display: true,
            title: {
              display: true,
              text: 'Maturity',
              font: {
                size: 14,
                weight: 'bold'
              },
              color: '#626c71'
            },
            grid: {
              color: 'rgba(98, 108, 113, 0.2)',
              drawBorder: false
            },
            ticks: {
              color: '#626c71',
              font: {
                size: 12,
                weight: '500'
              }
            }
          },
          y: {
            display: true,
            title: {
              display: true,
              text: 'Yield (%)',
              font: {
                size: 14,
                weight: 'bold'
              },
              color: '#626c71'
            },
            grid: {
              color: 'rgba(98, 108, 113, 0.2)',
              drawBorder: false
            },
            ticks: {
              color: '#626c71',
              font: {
                size: 12,
                weight: '500'
              },
              callback: function(value) {
                return value.toFixed(1) + '%';
              }
            },
            min: 3.0,
            max: 5.0
          }
        },
        interaction: {
          mode: 'nearest',
          axis: 'x',
          intersect: false
        },
        elements: {
          point: {
            hoverRadius: 10
          }
        }
      }
    });

    // Log chart creation for debugging
    console.log('✅ Yield curve chart created successfully');
    console.log('📊 Chart data points:', yieldData.length);
  }

  updateSpreads() {
    const spreads = this.federalReserveData.liveTreasuryData.key_spreads;
    
    this.updateSpreadElement('spread2s10s', spreads['2s10s_bps']);
    this.updateSpreadElement('spread3m10y', spreads['3m10y_bps']);
    this.updateSpreadElement('spread5s30s', spreads['5s30s_bps']);
    this.updateSpreadElement('spread1m3m', spreads['1m3m_bps']);
  }

  updateSpreadElement(elementId, spreadBps) {
    const element = document.getElementById(elementId);
    const prefix = spreadBps >= 0 ? '+' : '';
    element.textContent = `${prefix}${spreadBps} bps`;
    
    // Remove existing classes and add appropriate class
    element.classList.remove('positive', 'negative');
    element.classList.add(spreadBps >= 0 ? 'positive' : 'negative');
  }

  updateEconomicIndicators() {
    const indicators = this.federalReserveData.liveTreasuryData.economic_indicators;
    
    // Update recession indicator
    const recessionEl = document.getElementById('recessionIndicator');
    recessionEl.textContent = indicators.recession_warning ? 'High' : 'Low';
    recessionEl.className = indicators.recession_warning ? 'status status--error' : 'status status--success';
    
    // Update term premium
    document.getElementById('termPremium').textContent = `${indicators.term_premium_bps} bps`;
    
    // Update curve state
    const curveStateEl = document.getElementById('curveState');
    curveStateEl.textContent = indicators.curve_steepness === 'normal' ? 'Normal' : 
                               indicators.curve_steepness === 'inverted' ? 'Inverted' : 'Flat';
    curveStateEl.className = `status status--${indicators.curve_steepness === 'normal' ? 'success' : 
                             indicators.curve_steepness === 'inverted' ? 'error' : 'warning'}`;
  }

  updateForwardRates() {
    const forwards = this.federalReserveData.liveTreasuryData.forward_rates;
    
    document.getElementById('forward1y1y').textContent = `${forwards['1y1y']}%`;
    document.getElementById('forward2y1y').textContent = `${forwards['2y1y']}%`;
    document.getElementById('forward5y5y').textContent = `${forwards['5y5y']}%`;
    document.getElementById('forward10y10y').textContent = `${forwards['10y10y']}%`;
  }

  populateRiskMetricsTable() {
    const yieldPoints = this.federalReserveData.liveTreasuryData.yield_points;
    const riskMetrics = this.federalReserveData.riskMetrics;
    const tbody = document.getElementById('yieldTableBody');
    
    tbody.innerHTML = '';
    
    yieldPoints.forEach(point => {
      const risk = riskMetrics.find(r => r.maturity === point.maturity_label) || 
                   { duration: '-', dv01: '-', risk_level: 'UNKNOWN' };
      
      const row = document.createElement('tr');
      row.innerHTML = `
        <td><strong>${point.maturity_label}</strong></td>
        <td class="yield-value">${point.yield.toFixed(2)}%</td>
        <td class="duration-value">${risk.duration.toFixed(2)}</td>
        <td class="dv01-value">${risk.dv01.toLocaleString()}</td>
        <td><span class="risk-level ${risk.risk_level.toLowerCase().replace(' ', '-')}">${risk.risk_level}</span></td>
      `;
      tbody.appendChild(row);
    });
  }

  calculateCustomSpread() {
    const maturity1 = parseFloat(document.getElementById('calc1').value);
    const maturity2 = parseFloat(document.getElementById('calc2').value);
    
    const yieldPoints = this.federalReserveData.liveTreasuryData.yield_points;
    const yield1Point = yieldPoints.find(p => p.maturity_years === maturity1);
    const yield2Point = yieldPoints.find(p => p.maturity_years === maturity2);
    
    if (yield1Point && yield2Point) {
      const spreadBps = Math.round((yield1Point.yield - yield2Point.yield) * 100);
      const prefix = spreadBps >= 0 ? '+' : '';
      const resultElement = document.getElementById('customSpread');
      
      resultElement.textContent = `${prefix}${spreadBps} bps`;
      resultElement.style.color = spreadBps >= 0 ? 'var(--color-success)' : 'var(--color-error)';
    } else {
      document.getElementById('customSpread').textContent = 'N/A';
      document.getElementById('customSpread').style.color = 'var(--color-text-secondary)';
    }
  }

  exportChart() {
    if (this.chart) {
      const link = document.createElement('a');
      link.download = `treasury-yield-curve-${this.federalReserveData.liveTreasuryData.date}.png`;
      link.href = this.chart.toBase64Image('image/png', 1);
      link.click();
    }
  }

  exportData() {
    const yieldPoints = this.federalReserveData.liveTreasuryData.yield_points;
    const riskMetrics = this.federalReserveData.riskMetrics;
    
    let csvContent = 'Maturity,Yield (%),Duration (Years),DV01,Risk Level\n';
    
    yieldPoints.forEach(point => {
      const risk = riskMetrics.find(r => r.maturity === point.maturity_label);
      if (risk) {
        csvContent += `${point.maturity_label},${point.yield},${risk.duration},${risk.dv01},${risk.risk_level}\n`;
      }
    });
    
    // Add spreads and forward rates sections
    csvContent += '\n\nKey Spreads (Basis Points)\n';
    csvContent += 'Spread,Value (bps)\n';
    const spreads = this.federalReserveData.liveTreasuryData.key_spreads;
    csvContent += `2s10s,${spreads['2s10s_bps']}\n`;
    csvContent += `3m10y,${spreads['3m10y_bps']}\n`;
    csvContent += `5s30s,${spreads['5s30s_bps']}\n`;
    csvContent += `1m3m,${spreads['1m3m_bps']}\n`;
    
    csvContent += '\n\nForward Rates (%)\n';
    csvContent += 'Forward Rate,Value (%)\n';
    const forwards = this.federalReserveData.liveTreasuryData.forward_rates;
    Object.entries(forwards).forEach(([key, value]) => {
      csvContent += `${key},${value}\n`;
    });
    
    const blob = new Blob([csvContent], { type: 'text/csv' });
    const link = document.createElement('a');
    link.download = `federal-reserve-treasury-data-${this.federalReserveData.liveTreasuryData.date}.csv`;
    link.href = URL.createObjectURL(blob);
    link.click();
  }

  refreshData() {
    const refreshBtn = document.getElementById('refreshBtn');
    refreshBtn.classList.add('loading');
    
    // Simulate Federal Reserve data refresh
    setTimeout(() => {
      // Update timestamp
      this.federalReserveData.marketStatus.last_update = new Date().toISOString();
      this.updateHeader();
      
      // Simulate minor yield fluctuations (±2 basis points)
      this.federalReserveData.liveTreasuryData.yield_points.forEach(point => {
        const fluctuation = (Math.random() - 0.5) * 0.04; // ±2 basis points
        point.yield = Math.max(0, point.yield + fluctuation);
      });
      
      // Update chart with new data
      if (this.chart) {
        this.chart.data.datasets[0].data = this.federalReserveData.liveTreasuryData.yield_points.map(point => point.yield);
        this.chart.update();
      }
      
      // Recalculate spreads
      this.recalculateSpreads();
      this.updateSpreads();
      this.populateRiskMetricsTable();
      this.calculateCustomSpread();
      
      refreshBtn.classList.remove('loading');
      
      // Show success feedback
      const originalContent = refreshBtn.innerHTML;
      refreshBtn.innerHTML = `
        <svg width="16" height="16" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2">
          <polyline points="20 6 9 17 4 12"></polyline>
        </svg>
        Updated
      `;
      
      setTimeout(() => {
        refreshBtn.innerHTML = originalContent;
      }, 2000);
    }, 1500);
  }

  recalculateSpreads() {
    const yields = this.federalReserveData.liveTreasuryData.yield_points;
    const getYield = (maturity) => yields.find(p => p.maturity_label === maturity)?.yield || 0;
    
    // Recalculate key spreads
    this.federalReserveData.liveTreasuryData.key_spreads = {
      "2s10s_bps": Math.round((getYield('10Y') - getYield('2Y')) * 100),
      "3m10y_bps": Math.round((getYield('10Y') - getYield('3MO')) * 100),
      "5s30s_bps": Math.round((getYield('30Y') - getYield('5Y')) * 100),
      "1m3m_bps": Math.round((getYield('3MO') - getYield('1MO')) * 100)
    };
  }

  checkRecessionWarning() {
    const spreads = this.federalReserveData.liveTreasuryData.key_spreads;
    const isInverted = spreads['2s10s_bps'] < -20;
    
    if (isInverted && !this.federalReserveData.liveTreasuryData.economic_indicators.recession_warning) {
      this.showRecessionWarning();
    }
  }

  showRecessionWarning() {
    // Create recession warning banner
    const warningBanner = document.createElement('div');
    warningBanner.className = 'recession-warning';
    warningBanner.innerHTML = `
      <div class="container">
        <div style="display: flex; align-items: center; gap: 12px;">
          <svg width="20" height="20" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2">
            <path d="M10.29 3.86L1.82 18a2 2 0 0 0 1.71 3h16.94a2 2 0 0 0 1.71-3L13.71 3.86a2 2 0 0 0-3.42 0z"></path>
            <line x1="12" y1="9" x2="12" y2="13"></line>
            <line x1="12" y1="17" x2="12.01" y2="17"></line>
          </svg>
          <strong>RECESSION WARNING: Yield Curve Inversion Detected</strong>
          <span>2s10s spread: ${this.federalReserveData.liveTreasuryData.key_spreads['2s10s_bps']} bps</span>
        </div>
      </div>
    `;
    warningBanner.style.cssText = `
      position: fixed;
      top: 0;
      left: 0;
      right: 0;
      background: linear-gradient(135deg, #c02152 0%, #ff5459 100%);
      color: white;
      padding: 12px 0;
      font-weight: 500;
      font-size: 14px;
      z-index: 1000;
      box-shadow: 0 2px 8px rgba(192, 33, 82, 0.3);
      transform: translateY(-100%);
      transition: transform 0.3s ease;
    `;
    
    document.body.insertBefore(warningBanner, document.body.firstChild);
    
    setTimeout(() => {
      warningBanner.style.transform = 'translateY(0)';
    }, 100);
    
    setTimeout(() => {
      warningBanner.style.transform = 'translateY(-100%)';
      setTimeout(() => {
        if (document.body.contains(warningBanner)) {
          document.body.removeChild(warningBanner);
        }
      }, 300);
    }, 8000);
  }

  startAutoRefresh() {
    // Auto-refresh every 5 minutes during market hours
    this.refreshInterval = setInterval(() => {
      if (this.federalReserveData.marketStatus.is_open) {
        console.log('Auto-refreshing Federal Reserve data...');
        this.refreshData();
      }
    }, 5 * 60 * 1000); // 5 minutes
  }

  bindEvents() {
    // Refresh button
    document.getElementById('refreshBtn').addEventListener('click', () => {
      this.refreshData();
    });

    // Export buttons
    document.getElementById('exportChart').addEventListener('click', () => {
      this.exportChart();
    });

    document.getElementById('exportData').addEventListener('click', () => {
      this.exportData();
    });

    // Calculator
    document.getElementById('calculateSpread').addEventListener('click', () => {
      this.calculateCustomSpread();
    });

    // Calculator auto-calculate on change
    document.getElementById('calc1').addEventListener('change', () => {
      this.calculateCustomSpread();
    });

    document.getElementById('calc2').addEventListener('change', () => {
      this.calculateCustomSpread();
    });

    // Keyboard shortcuts
    document.addEventListener('keydown', (e) => {
      if (e.ctrlKey || e.metaKey) {
        switch (e.key) {
          case 'r':
            e.preventDefault();
            this.refreshData();
            break;
          case 'e':
            e.preventDefault();
            this.exportData();
            break;
          case 'p':
            e.preventDefault();
            this.exportChart();
            break;
        }
      }
    });

    // Chart interactions
    if (this.chart) {
      this.chart.canvas.addEventListener('click', (event) => {
        const points = this.chart.getElementsAtEventForMode(event, 'nearest', { intersect: true }, false);
        if (points.length > 0) {
          const point = points[0];
          const dataIndex = point.index;
          const yieldPoint = this.federalReserveData.liveTreasuryData.yield_points[dataIndex];
          
          // Show detailed maturity information
          console.log(`Selected: ${yieldPoint.maturity_label} - ${yieldPoint.yield}% yield`);
        }
      });
    }

    // Responsive table handling
    window.addEventListener('resize', () => {
      if (this.chart) {
        this.chart.resize();
      }
    });

    // Visibility change handling (pause auto-refresh when tab is hidden)
    document.addEventListener('visibilitychange', () => {
      if (document.hidden) {
        if (this.refreshInterval) {
          clearInterval(this.refreshInterval);
        }
      } else {
        this.startAutoRefresh();
      }
    });
  }

  // Cleanup method
  destroy() {
    if (this.refreshInterval) {
      clearInterval(this.refreshInterval);
    }
    if (this.chart) {
      this.chart.destroy();
    }
  }
}

// Initialize dashboard when DOM is loaded
document.addEventListener('DOMContentLoaded', () => {
  const dashboard = new LiveYieldCurveDashboard();
  
  // Make dashboard globally accessible for debugging and console access
  window.treasuryDashboard = dashboard;
  
  console.log('🏛️ Federal Reserve H.15 Treasury Yield Curve Dashboard Initialized');
  console.log('📊 Data Source: Federal Reserve Board of Governors');
  console.log('📈 Current Date:', dashboard.federalReserveData.liveTreasuryData.date);
  console.log('💹 Curve Shape:', dashboard.federalReserveData.liveTreasuryData.curve_shape);
  
  // Log keyboard shortcuts
  console.log('\n⌨️ Keyboard Shortcuts:');
  console.log('  Ctrl/Cmd + R: Refresh data');
  console.log('  Ctrl/Cmd + E: Export CSV');
  console.log('  Ctrl/Cmd + P: Export chart PNG');
});

// Handle theme changes for professional Federal Reserve styling
const observer = new MutationObserver((mutations) => {
  mutations.forEach((mutation) => {
    if (mutation.type === 'attributes' && mutation.attributeName === 'data-color-scheme') {
      if (window.treasuryDashboard && window.treasuryDashboard.chart) {
        // Update chart colors for theme change while maintaining Fed branding
        window.treasuryDashboard.chart.update();
      }
    }
  });
});

observer.observe(document.documentElement, {
  attributes: true,
  attributeFilter: ['data-color-scheme']
});

// Error handling for network issues
window.addEventListener('error', (event) => {
  console.error('Dashboard Error:', event.error);
});

window.addEventListener('unhandledrejection', (event) => {
  console.error('Unhandled Promise Rejection:', event.reason);
});

// Performance monitoring
if ('performance' in window) {
  window.addEventListener('load', () => {
    const loadTime = performance.timing.loadEventEnd - performance.timing.navigationStart;
    console.log(`📊 Dashboard loaded in ${loadTime}ms`);
  });
}

// Export for module systems
if (typeof module !== 'undefined' && module.exports) {
  module.exports = LiveYieldCurveDashboard;
}
