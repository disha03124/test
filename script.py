import os
import sys
from datetime import datetime, timedelta
import pandas as pd
from jugaad_data.nse import stock_df

# Check if the correct number of command-line arguments are provided
if len(sys.argv) < 3:
    print("Usage: python script.py <stock_symbol> <x>")
    sys.exit(1)

# Get the stock symbol and x from the command-line arguments
stock_symbol = sys.argv[1]
x = int(sys.argv[2])  # Convert x to an integer
archive_folder = os.path.join(os.path.dirname(__file__), 'archive')  # Use the existing archive folder

# Write data for the specified stock into a text file in the archive folder
start_date = datetime.today().date() - timedelta(days=365 * x)
end_date = datetime.today().date()

df = stock_df(symbol=stock_symbol, from_date=start_date, to_date=end_date)

# Replace spaces in column names with underscores
df.columns = df.columns.str.replace(' ', '_')

# Change the file extension to .txt and write the data in the specified format
filename = os.path.join(archive_folder, f'{stock_symbol}.txt')
with open(filename, 'w') as txt_file:
    for index, row in df.iterrows():
        # txt_file.write(f"Date: {index}\n")
        for key, value in row.items():
            txt_file.write(f"{key}: {value}\n")
        txt_file.write("\n")

print(f"Jugaad data for {stock_symbol} with x={x} has been written to a text file in the archive folder.")