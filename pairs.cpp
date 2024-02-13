#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <iomanip>
#include <ctime>
#include <algorithm>
#include <cmath>

using namespace std;

struct StockData {
    std::string date;
    double price1; // Price of stock 1
    double price2; // Price of stock 2
};

int main(int argc, char* argv[]) {

    std::string strategy = argv[1];
    std::string symbol1 = argv[2];
    std::string symbol2 = argv[3];
    int x = std::stoi(argv[4]);
    int n = std::stoi(argv[5]);
    float threshold = std::stof(argv[6]);
    std::string start_date = argv[7];
    std::string end_date = argv[8];

    std::ifstream input_file1("archive/" + symbol1 + ".txt");
    std::ifstream input_file2("archive/" + symbol2 + ".txt");

    if (!input_file1.is_open() || !input_file2.is_open()) {
        std::cerr << "Error: Unable to open input file(s)" << std::endl;
        return 1;
    }

    std::string line;
    std::vector<StockData> stock_data1;
    std::vector<StockData> stock_data2;
    StockData data1;
    StockData data2;
    bool date_set=false;

    while (std::getline(input_file1, line)) {
        std::istringstream iss(line);
        std::string key, value;
        iss >> key >> value;

        if (key == "DATE:") {
            // Set the date for the current StockData object
            data1.date = value;
            date_set = true;
        }
        else if (key == "CLOSE:") {
            // Set the price for the current StockData object
            data1.price1 = std::stod(value);

            // If both date and price are set, push the StockData object into the vector
            if (date_set) {
                stock_data1.push_back(data1);
                data1 = StockData(); // Create a new StockData object for the next pair of DATE and CLOSE lines
                date_set = false; // Reset the flag
            }
        }
    }

    while (std::getline(input_file2, line)) {
      std::istringstream iss(line);
      std::string key, value;
      iss >> key >> value;

      if (key == "DATE:") {
          // Set the date for the current StockData object
          data2.date = value;
          date_set = true;
      }
      else if (key == "CLOSE:") {
          // Set the price for the current StockData object
          data2.price2 = std::stod(value);

          // If both date and price are set, push the StockData object into the vector
          if (date_set) {
              stock_data2.push_back(data2);
              data2 = StockData(); // Create a new StockData object for the next pair of DATE and CLOSE lines
              date_set = false; // Reset the flag
          }
      }
    }
    // std:: cout << stock_data1.size() << stock_data2.size();
    // Check if the number of data points for both stocks match
    if (stock_data1.size() != stock_data2.size()) {
        std::cerr << "Error: Unequal number of data points for the given stock pair." << std::endl;
        return 1;
    }

    // Implement Pairs Trading Strategy
    std::vector<int> signals;
    for (size_t i = 0; i < stock_data1.size(); ++i) {
        if (stock_data1[i].date >= start_date && stock_data1[i].date <= end_date) {
            // Calculate spread
            double spread = stock_data1[i].price1 - stock_data2[i].price2;

            // Calculate rolling mean and std dev of the spread
            double sum = 0.0;
            double sum_sq = 0.0;

            for (int j = 0; j < n ; ++j) {
                sum += stock_data1[i + j+1].price1 - stock_data2[i + j+1].price2;
                sum_sq += pow(stock_data1[i +j+1].price1 - stock_data2[i+j+1].price2, 2);
            }

            double mean = sum / n;
            double std_dev = sqrt((sum_sq - n * pow(mean, 2)) / n);

            // Calculate z-score
            double z_score = (spread - mean) / std_dev;

            // Generate signals
            if (z_score > threshold) {
              //std:: cout<< "sell";
                signals.push_back(-1);  // Sell spread (short S1, long S2)
            } else if (z_score < -threshold) {
              //std:: cout<< "buy";
                signals.push_back(1);  // Buy spread (long S1, short S2)
            } else {
              //std:: cout<< "hold";
                signals.push_back(0);  // Hold
            }
        }
    }

    /// Filter data for start_date to end_date
    std::vector<StockData> result_data1;
    std::vector<StockData> result_data2;

    for (size_t i = 0; i < stock_data1.size(); ++i) {
        if (stock_data1[i].date >= start_date && stock_data1[i].date <= end_date) {
            result_data1.push_back(stock_data1[i]);
            result_data2.push_back(stock_data2[i]);
        }
    }

    // Calculate daily cashflow
    std::ofstream cashflow_file("daily_cashflow.csv");
    cashflow_file << "Date,Cashflow\n";
    double cashflow = 0.0;
    int a = signals.size();
    int i = a - 1;

    while (i >= 0) {
        double spread = result_data1[i].price1 - result_data2[i].price2;
        cashflow += signals[i] * spread;
        cashflow_file << result_data1[i].date << "," << -1 * cashflow << "\n";
        i--;
    }
    cashflow_file.close();

    // Write to order statistics files
    std::ofstream order_file1("order_statistics_1.csv");
    std::ofstream order_file2("order_statistics_2.csv");

    order_file1 << "Date,Direction,Quantity,Price\n";
    order_file2 << "Date,Direction,Quantity,Price\n";

    for (size_t i = 0; i < signals.size(); ++i) {
        if (signals[i] != 0) {
            order_file1 << result_data1[i].date << "," << (signals[i] == 1 ? "BUY" : "SELL") << "," << x << "," << result_data1[i].price1 << "\n";
            order_file2 << result_data2[i].date << "," << (signals[i] == 1 ? "BUY" : "SELL") << "," << x << "," << result_data2[i].price2 << "\n";
        }
    }

    order_file1.close();
    order_file2.close();

    return 0;
}