

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

def main():
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

    print("\nBasic statistics for each column:")
    for col in column_names:
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

    if len(column_names) > 1:
        print("\nCorrelation matrix among all numeric columns:")
        try:
            corr_matrix = analyzer.correlationMatrix(column_names)
            print(corr_matrix)
            
            threshold = 0.7
            print(f"\nStrong correlations (|corr| > {threshold}):")
            analyzer.reportStrongCorrelations(column_names, threshold)
        except Exception as e:
            print("Error while computing correlation matrix:", e)
    else:
        print("\nInsufficient columns for correlation analysis.")

if __name__ == "__main__":
    main()
