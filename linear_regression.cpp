#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <iomanip>
#include <ctime>
#include <cmath>
#include <algorithm>

using namespace std;

// Define StockData struct to store stock data
struct StockData {
    std::string date;
    double close;
    double open;
    double vwap;
    double high;
    double low;
    double NOT;
};

std::vector<std::vector<double>> inverseMatrix(const std::vector<std::vector<double>>& matrix) {
    int n = matrix.size();
    ////cout<<"inverse"<<endl;
    std::vector<std::vector<double>> augmentedMatrix(n, std::vector<double>(2 * n, 0));

    // Create an augmented matrix by appending an identity matrix to the right of the original matrix
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            augmentedMatrix[i][j] = matrix[i][j];
        }
        augmentedMatrix[i][i + n] = 1;
    }

    // Perform forward elimination
    for (int i = 0; i < n; ++i) {
        // Divide the current row by the diagonal element to make it 1
        double divisor = augmentedMatrix[i][i];
        for (int j = 0; j < 2 * n; ++j) {
            augmentedMatrix[i][j] /= divisor;
        }

        // Subtract multiples of the current row from other rows to make all other elements in the column zero
        for (int k = 0; k < n; ++k) {
            if (k != i) {
                double factor = augmentedMatrix[k][i];
                for (int j = 0; j < 2 * n; ++j) {
                    augmentedMatrix[k][j] -= factor * augmentedMatrix[i][j];
                }
            }
        }
    }

    // Extract the inverse matrix from the augmented matrix
    std::vector<std::vector<double>> inverse(n, std::vector<double>(n, 0));
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            inverse[i][j] = augmentedMatrix[i][j + n];
        }
    }

    return inverse;
}

std::vector<std::vector<double>> multiplyMatrix(const std::vector<std::vector<double>>& matrix1, const std::vector<std::vector<double>>& matrix2) {
    int rows1 = matrix1.size();
    
    int cols1 = matrix1[0].size();
    int rows2 = matrix2.size();
    int cols2 = matrix2[0].size();
    ////cout<<"multiply"<<endl;
    //cout<<rows1<<" "<<cols1 <<"::"<<rows2<<" "<<cols2<<endl;

    // Check if matrices are compatible for multiplication
    if (cols1 != rows2) {
        std::cerr << "Error: Matrices cannot be multiplied. Number of columns in the first matrix must be equal to the number of rows in the second matrix." << std::endl;
        return {};
    }

    // Initialize the result matrix with appropriate dimensions
    std::vector<std::vector<double>> result(rows1, std::vector<double>(cols2, 0));

    // Perform matrix multiplication
    int j = 0;
    for (int i = 0; i < rows1; i++) {
        for (int j = 0; j < cols2; j++) {
            for (int k = 0; k < cols1; k++) {
                result[i][j] += matrix1[i][k] * matrix2[k][j];
                
            }
            ////cout<<"result["<<i<<"]"<<"["<<j<<"]"<<"="<<result[i][j]<<" ";
        }
        ////cout<<endl;
    }

    return result;
}

std::vector<std::vector<double>> transpose(const std::vector<std::vector<double>>& matrix) {
    int rows = matrix.size();
    int cols = matrix[0].size();
    ////cout<<"transpose"<<endl;

    std::vector<std::vector<double>> result(cols, std::vector<double>(rows));

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            result[j][i] = matrix[i][j];
        }
    }

    return result;
}

// std::vector<std::vector<double>> generateMatrix(const std::vector<StockData>& stockData, const std::string& startDate, const std::string& endDate, int numCols){

// }

int main(int argc, char* argv[]) {


    std::string strategy = argv[1];
    std::string symbol = argv[2];
    int x = std::stoi(argv[3]);
    int p = std::stoi(argv[4]);
    string train_start_date = argv[5];
    string train_end_date = argv[6];
    string start_date = argv[7];
    string end_date = argv[8];
    ////cout<<"train start date  "<<train_start_date<<endl;
    ////cout<<"train end date  "<<train_end_date<<endl;

    std::ifstream input_file("archive/" + symbol + ".txt");
    if (!input_file.is_open()) {
        std::cerr << "Error: Unable to open input file" << std::endl;
        return 1;
    }

    std::string line;
    std::vector<StockData> stock_data;
    StockData data;
    bool data_set1,data_set2,data_set3 ,data_set4,data_set5,data_set6,data_set7 = false;

    while (std::getline(input_file, line)) {
    std::istringstream iss(line);
    std::string key, value;
    iss >> key >> value;

        if (key == "DATE:") {
            // Set the date for the current StockData object
            ////cout<<"in date";
            data.date = value;
            ////cout<<value;
            data_set1 = true;
        }
        else if (key == "CLOSE:") {
            ////cout<<"close";
            data.close =  std::stod(value);
            data_set2 = true; // Reset the flag
            ////cout<<value;
            
        }else if(key == "OPEN:"){
            ////cout<<"open";
            data.open = std::stod(value);
            data_set3 = true; // Reset the flag
            ////cout<<value;
            
        }else if(key == "HIGH:"){
            // //cout<<value;
            // //cout<<"high";
            data.high = std::stod(value);
            data_set4 = true; // Reset the flag
            
        }else if(key == "LOW:"){
            // //cout<<value;
            // //cout<<"low";
            data.low = std::stod(value);
            data_set5 = true; // Reset the flag

        }else if(key == "NO_OF_TRADES:"){
            data.NOT = std::stod(value);
            data_set6 = true; // Reset the flag

            if(data_set1 && data_set2 && data_set3 && data_set4 && data_set5 && data_set6 && data_set7 ){
                stock_data.push_back(data);
            }
            data_set1 = false;
            data_set2 = false;
            data_set3 = false;
            data_set4 = false;
            data_set5 = false;
            data_set6 = false;
            data_set7 = false;
            data = StockData();
            //data_set1 = false;
            
        }else if(key == "VWAP:"){
            // //cout<<value;
            // //cout<<"vwap";
            data.vwap = std::stod(value);
            data_set7 = true; // Reset the flag
        }

        
    }
    std::vector<std::string> trading_days;
    for (const auto& data : stock_data) {
        if(data.date >= start_date && data.date<= end_date){
            trading_days.push_back(data.date);
        }
    }
    //cout<<trading_days.size()<<endl;
    // Find the last trading day
    std::string last_trading_day = train_end_date;
    if (std::find(trading_days.begin(), trading_days.end(), last_trading_day) == trading_days.end()) {
        last_trading_day = trading_days.front();
    }



    std::vector<std::vector<double>> X(8);//,std::vector<double>(sz, 0));
    int row = 0;
    int sz = 0;
    for(auto & st_data: stock_data){
        if(st_data.date >= train_start_date && st_data.date < train_end_date){
           sz++;
           //cout<<st_data.date<<endl;
        }
    }
    //cout<<"sz = "<<sz<<endl;;
    // std::tm start_date_tm = {};
    // std::istringstream start_date_stream(start_date);
    // start_date_stream >> std::get_time(&start_date_tm, "%Y-%m-%d");

    // // Add one day
    // start_date_tm.tm_mday -= 1;
    // std::mktime(&start_date_tm);

    // // Convert the updated date back to a string
    // std::ostringstream start_date_updated_stream;
    // start_date_updated_stream << std::put_time(&start_date_tm, "%Y-%m-%d");
    // train_start_date = start_date_updated_stream.str();

    //std::vector<std::string> trading_days;
    for (const auto& data : stock_data) {
        trading_days.push_back(data.date);
    }

    // Find the last trading day

    //std::string last_trading_day = end_date;
    if (std::find(trading_days.begin(), trading_days.end(), last_trading_day) == trading_days.end()) {
        last_trading_day = trading_days.back();
    }




    for(auto & st_data: stock_data){
        if(st_data.date >= train_start_date && st_data.date < train_end_date){
            X[0].push_back(1) ;
        }
    }
    int count1 =0;
    ////cout<<stock_data.size()<<endl;
    for(auto& st_data: stock_data){
        //int count = 0;
        // //cout << "Data date: " << st_data.date << endl;
        // //cout << "Start date: " << train_start_date << ", End date: " << train_end_date << endl;
        if(st_data.date >= train_start_date && st_data.date < train_end_date){
            count1++;
            ////cout<<"in"<<endl;
            X[1].push_back(st_data.close);
            X[2].push_back(st_data.open);
            X[3].push_back(st_data.vwap);
            X[4].push_back(st_data.low);
            X[5].push_back(st_data.high);
            X[6].push_back(st_data.NOT);
            ////cout<<st_data.high<<st_data.close;
            //X[7].push_back(st_data.high);
        }
    }
    //cout<<"count1 "<<count1<<endl;;
    // std::tm start_date_tm = {};
    // std::istringstream start_date_stream(start_date);
    // start_date_stream >> std::get_time(&start_date_tm, "%Y-%m-%d");

    // Add one day
    // start_date_tm.tm_mday += 1;
    // std::mktime(&start_date_tm);

    // Convert the updated date back to a string
    // std::ostringstream start_date_updated_stream;
    // start_date_updated_stream << std::put_time(&start_date_tm, "%Y-%m-%d");
    // train_start_date = start_date_updated_stream.str();
    std::tm train_start_date_tm = {};
    std::istringstream train_start_date_stream(train_start_date);
    train_start_date_stream >> std::get_time(&train_start_date_tm, "%Y-%m-%d");

    // Add one day
    train_start_date_tm.tm_mday -= 1;
    std::mktime(&train_start_date_tm);

    // Convert the updated date back to a string
    std::ostringstream train_start_date_updated_stream;
    train_start_date_updated_stream << std::put_time(&train_start_date_tm, "%Y-%m-%d");
    train_start_date = train_start_date_updated_stream.str();

    // std::tm end_date_tm = {};
    // std::istringstream end_date_stream(train_end_date);
    // start_date_stream >> std::get_time(&start_date_tm, "%Y-%m-%d");

    // // Add one day
    // start_date_tm.tm_mday -= 1;
    // std::mktime(&end_date_tm);

    // // Convert the updated date back to a string
    // std::ostringstream end_date_updated_stream;
    // end_date_updated_stream << std::put_time(&end_date_tm, "%Y-%m-%d");
    // train_end_date = end_date_updated_stream.str();

    int don = 0;
    for(auto& st_data: stock_data){
        
        if(st_data.date >= train_start_date && st_data.date<=train_end_date){
        if(don<sz){
            X[7].push_back(st_data.open);
            don++;
            ////cout<<st_data.open<<endl;
        }
        }
    }
    // cout<<"dimentions of X "<<X.size()<<" "<<X[0].size()<<endl;
    for(auto & row: X){
        for(auto & e:row){
            cout<<e<<" ";
        }
        cout<<endl;
    }
    cout<<"X_new"<<endl;
   // Initialize Y with one row and 'sz' columns, filled with zeros
    std::vector<std::vector<double>> Y(1, std::vector<double>(sz, 0));
    //cout<<"dimentions of Y : "<<Y.size()<<" "<<Y[0].size()<<endl;
    int i = 0;
    for(auto & st_data: stock_data){
        if(st_data.date >= train_start_date && st_data.date < train_end_date){
            Y[0][i] = st_data.close ;
            i++;
        }
    }

    // for(auto & e:Y){
    //         //cout<<e<<endl;
    //      }
    
    std::vector<std::vector<double>> X_Tras = transpose(X);
    //  for(auto & row: X_Tras){
    //     for(auto & e:row){
    //         //cout<<e<<endl;
    //     }
    // }
    
    std::vector<std::vector<double>> X_mul_X_Trans = multiplyMatrix(X, X_Tras);
    ////cout<<"here2"<<endl;
    // //cout<<"multiplication of X and X_Transpose"<<endl;
    //  for(auto & row: X){
    //     for(auto & e:row){
    //         //cout<<e<<endl;
    //     }
    //     //cout<<endl;
    // }

    //correct till here


    //cout<<"dimention of X :  " <<X.size()<<" "<<X[0].size()<<endl;
    //cout<<"dimention of X trans:  " <<X_Tras.size()<<" "<<X_Tras[0].size()<<endl;
    //cout<<"dimention of Y:  "<<Y.size()<<" "<<Y[0].size()<<endl;
    std::vector<std::vector<double>> Y_XT = multiplyMatrix(Y, X_Tras);
    //cout<<"multiplication of Y and X Transpose"<<endl;
      for(auto & row: Y_XT){
        for(auto & e:row){
            //cout<<e<<endl;
        }
        //cout<<endl;
    }
    std::vector<std::vector<double>> X_XT_1 = inverseMatrix(X_mul_X_Trans);
    //cout<<"inverse of X mul X transpose"<<endl;
    for(auto & row: X_XT_1){
        for(auto & e:row){
            //cout<<e<<endl;
        }
        //cout<<endl;
    }
    //cout<<"dimention of X_XT_1:  "<<X_XT_1.size()<<" "<<X_XT_1[0].size()<<endl;
    std::vector<std::vector<double>> BETA = multiplyMatrix(Y_XT, X_XT_1);
    //cout<<"multiplication of Y and X Transpose and X inverse"<<endl;
    for(auto & row: BETA){
        for(auto & e:row){
            //cout<<e<<endl;
        }
        //cout<<endl;
    }
    //cout<<"dimention of BETA:  "<<BETA.size()<<" "<<BETA[0].size()<<endl;
    
    // for(int i = 0; i < BETA[0].size();i++){
    //     cout<<BETA[0][i]<<" ";
    // }
    // for(auto & row: X_1){
    //     for(auto & e:row){
    //         //cout<<e<<endl;
    //     }
    // }
    // std::vector<std::vector<double>> YX_1 = multiplyMatrix(Y, X);
    // for(int i = 0; i< YX_1[0].size();i++){
    //     //cout<<YX_1[i][0]<<endl;
    // }  

    double beta_0 = BETA[0][0];
    double beta_1 = BETA[0][1];
    double beta_2 = BETA[0][2];
    double beta_3 = BETA[0][3];
    double beta_4 = BETA[0][4];
    double beta_5 = BETA[0][5];
    double beta_6 = BETA[0][6];
    double beta_7 = BETA[0][7];

    std::vector<std::vector<double>> X_new(8);
    for(auto & st_data: stock_data){
        if(st_data.date >= start_date && st_data.date < end_date){
            X_new[0].push_back(1) ;
        }
    }
    
    // cout << "Train Start date: " << train_start_date << ",Train End date: " << train_end_date << endl;
    // cout << "Start date: " << start_date << ", End date: " << end_date << endl;
    
    for(auto& st_data: stock_data){
        
        // //cout << "Data date: " << st_data.date << endl;
        
        if(st_data.date >= start_date && st_data.date < end_date){
            //count++;
            ////cout<<"in"<<endl;
            X_new[1].push_back(st_data.close);
            X_new[2].push_back(st_data.open);
            X_new[3].push_back(st_data.vwap);
            X_new[4].push_back(st_data.low);
            X_new[5].push_back(st_data.high);
            X_new[6].push_back(st_data.NOT);
        }
    }
    int count = X_new[0].size();
    cout<<count<<endl;
    //cout<<"count = "<<count<<endl;
    cout<<start_date<<endl;

    std::tm start_date_tm = {};
    std::istringstream start_date_stream(start_date);
    start_date_stream >> std::get_time(&start_date_tm, "%Y-%m-%d");

// Add one day
    start_date_tm.tm_mday += 1;
    std::mktime(&start_date_tm);

    // Convert the updated date back to a string
    std::ostringstream start_date_updated_stream;
    start_date_updated_stream << std::put_time(&start_date_tm, "%Y-%m-%d");
    start_date = start_date_updated_stream.str();

    
    cout<<start_date<<endl;
    

    int itr = 0;
    for(auto& st_data: stock_data){
        
        if(st_data.date >= start_date && st_data.date < end_date){
            if(itr!=count + 1){
                //cout<<st_data.date<<endl;
                X_new[7].push_back(st_data.open);
                ////cout<<st_data.open<<endl;
                itr++;
                //cout
            }
        }
    }
    cout<<count<<endl;


    for(auto&a : X_new){
        for(auto& b: a){
            cout<<b<<" ";
        }
        cout<<endl;
    }
    
    // cout<<"X_new[0]  size  === "<<X_new[0].size()<<endl;;
    // cout<<"X_new[6]  size  === "<<X_new[6].size()<<endl;;
    // cout<<"X_new[7]  size  === "<<X_new[7].size()<<endl;;
    // cout<<"count2 = "<<count2<<endl;
    // cout<<"dimentions of X_new "<<X_new.size()<<" "<<X_new[0].size()<<endl;
    std::vector<std::vector<double>> Y_predicted;
    for(auto& a:X){
        for(auto b: a){
            cout<<b<<" ";
        }
    }
    Y_predicted = multiplyMatrix(BETA, X_new);
    
    
    std::vector<std::vector<double>> Y_actual(1, std::vector<double>(count, 0));
    int j = 0;
    for(auto & st_data: stock_data){
        if(st_data.date >= start_date && st_data.date < end_date){
            Y_actual[0][j] = st_data.close ;
            j++;
        }
    }
    
    //cout<<"dimentions of Y_predicted : "<<Y_predicted.size() << " "<<Y_predicted[0].size()<<endl;
    // for(auto& row: Y_predicted){
        
    // }
    // for(auto& a: Y_predicted){
    //     for(auto& b: a){
    //         cout<<b<<" ";
    //     }
    // }

    std::vector<int> signals;
    for(int i = 0; i<count; i++){
        //cout<<Y_actual[0][i] <<"  "<<Y_predicted[0][i]<<endl;
        if(Y_actual[0][i] > (Y_predicted[0][i])*p){
            signals.push_back(1);
        }else if(Y_actual[0][i] < (Y_predicted[0][i])*p){
            signals.push_back(-1);
        }
    }  
    // cout<<signals.size()<<endl;  
    // for(auto& a:signals){
    //     cout<<a<<" ";
    // } 
    std::vector<StockData> result_data;
    for (const auto& data : stock_data) {
        if (data.date >= start_date && data.date <= last_trading_day) {
            result_data.push_back(data);
        }
    }
    std::ofstream cashflow_file("daily_cashflow.csv");
    cashflow_file << "Date,Cashflow\n";
    double cashflow = 0.0;
    int a = signals.size();
    int k = a-1;
    while(k >= 0) {
        cashflow += signals[k] * (result_data[k].close);
        cashflow_file << result_data[k].date << "," << -1 * cashflow << "\n";
        k--;
    }
    cashflow_file.close();

    // Write to order statistics.csv
    std::ofstream order_file("order_statistics.csv");
    order_file << "Date,Direction,Quantity,Price\n";

    for (size_t i = 0; i < signals.size(); ++i) {
        if (signals[i] != 0) {
            order_file << result_data[i].date << "," << (signals[i] == 1 ? "BUY" : "SELL") << ",1," << result_data[i+1].close << "\n";
        }
    }
    order_file.close();

    return 0;


    
}



