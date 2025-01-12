

import os
import csv

import sys
ROOT_DIR = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
sys.path.append(os.path.join(ROOT_DIR, 'lib', 'python'))

import stats_cpp

def read_csv_to_dataset(file_path: str) -> stats_cpp.Dataset:
    """
    Reads a CSV file and returns a stats_cpp.Dataset object
    populated with rows of data. Numerical values are cast to float if possible.
    """
    dataset = stats_cpp.Dataset()

    with open(file_path, mode='r', newline='', encoding='utf-8') as f:
        reader = csv.DictReader(f)
        
        for row in reader:
            parsed_row = {}
            for key, value in row.items():
                try:
                    parsed_value = float(value)
                except ValueError:
                    parsed_value = value
                parsed_row[key] = parsed_value

            dataset.addRow(parsed_row)

    return dataset

def is_numeric_column(dataset, column_name):
    """Check if a column contains only numeric values."""
    try:
        # Try to get mean - this will fail if column has non-numeric values
        analyzer = stats_cpp.StatisticalAnalyzer(dataset)
        analyzer.mean(column_name)
        return True
    except:
        return False

def main():
    print(sys.argv)
    if len(sys.argv) != 2:
        print("Usage: python3 Statistics_Module_main.py <path_to_csv_file>")
        sys.exit(1)

    csv_file = sys.argv[1]
    
    if not os.path.exists(csv_file):
        print(f"Error: File '{csv_file}' not found.")
        sys.exit(1)

    print(f"\nLoading dataset from: {csv_file}")

    dataset = read_csv_to_dataset(csv_file)
    analyzer = stats_cpp.StatisticalAnalyzer(dataset)

    column_names = dataset.getColumnNames()
    print(f"\nColumn names in the dataset: {column_names}")

    if not column_names:
        print("No columns found. Exiting.")
        return

    print("\nBasic statistics for each numeric column:")
    numeric_columns = [col for col in column_names if is_numeric_column(dataset, col)]
    
    for col in numeric_columns:
        try:
            col_mean = analyzer.mean(col)
            col_median = analyzer.median(col)
            col_variance = analyzer.variance(col)
            col_std = analyzer.standardDeviation(col)
            col_freq = analyzer.frequencyCount(col)

            print(f"\n  Column: {col}")
            print(f"    Mean:          {col_mean}")
            print(f"    Median:        {col_median}")
            print(f"    Variance:      {col_variance}")
            print(f"    Std Dev:       {col_std}")
            print(f"    Freq. Count:   {col_freq}")

        except Exception as e:
            print(f"\n  Column: {col} (skipped) -> {e}")

    if len(numeric_columns) > 1:
        print("\nCorrelation matrix among all numeric columns:")
        try:
            corr_matrix = analyzer.correlationMatrix(numeric_columns)
            print(corr_matrix)
            
            threshold = 0.7
            print(f"\nStrong correlations (|corr| > {threshold}):")
            analyzer.reportStrongCorrelations(numeric_columns, threshold)
        except Exception as e:
            print("Error while computing correlation matrix:", e)
    else:
        print("\nInsufficient numeric columns for correlation analysis.")

if __name__ == "__main__":
    main()
