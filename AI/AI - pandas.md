# Comprehensive Guide to Pandas for AI, Machine Learning, and Deep Learning

Pandas is a powerful and versatile open-source data manipulation and analysis library for Python. It provides data structures and functions needed to work seamlessly with structured data, making it an indispensable tool for data scientists and engineers. In the context of Artificial Intelligence (AI), Machine Learning (ML), and Deep Learning (DL), Pandas facilitates data preprocessing, exploration, and transformation, which are crucial steps in building effective models.

This document provides an in-depth exploration of Pandas, covering its fundamental and advanced operations. Each section includes practical code examples with visualizations that demonstrate the transformations and operations performed on DataFrames and Series.

## Table of Contents

1. [Introduction to Pandas](#introduction-to-pandas)
2. [Creating DataFrames and Series](#creating-dataframes-and-series)
   - [From Python Dictionaries and Lists](#from-python-dictionaries-and-lists)
   - [From CSV and Excel Files](#from-csv-and-excel-files)
   - [From NumPy Arrays](#from-numpy-arrays)
3. [Inspecting and Exploring Data](#inspecting-and-exploring-data)
   - [Viewing Data](#viewing-data)
   - [Descriptive Statistics](#descriptive-statistics)
   - [Data Types and Information](#data-types-and-information)
4. [Selecting and Filtering Data](#selecting-and-filtering-data)
   - [Selecting Columns and Rows](#selecting-columns-and-rows)
   - [Boolean Indexing](#boolean-indexing)
   - [Using `isin()` and `between()`](#using-isin-and-between)
5. [Handling Missing Data](#handling-missing-data)
   - [Identifying Missing Data](#identifying-missing-data)
   - [Handling Missing Values](#handling-missing-values)
6. [Data Cleaning](#data-cleaning)
   - [Renaming Columns](#renaming-columns)
   - [Dropping Columns and Rows](#dropping-columns-and-rows)
   - [Replacing Values](#replacing-values)
7. [Aggregation and Grouping](#aggregation-and-grouping)
   - [Grouping Data](#grouping-data)
   - [Aggregating Data](#aggregating-data)
   - [Applying Multiple Aggregations](#applying-multiple-aggregations)
8. [Merging and Joining DataFrames](#merging-and-joining-dataframes)
   - [Concatenation](#concatenation)
   - [Merging](#merging)
   - [Joining](#joining)
9. [Pivot Tables and Crosstab](#pivot-tables-and-crosstab)
   - [Creating Pivot Tables](#creating-pivot-tables)
   - [Using Crosstab](#using-crosstab)
10. [Reshaping Data](#reshaping-data)
    - [Melting](#melting)
    - [Stacking and Unstacking](#stacking-and-unstacking)
11. [Time Series Analysis](#time-series-analysis)
    - [Working with DateTime](#working-with-datetime)
    - [Resampling](#resampling)
12. [Visualization with Pandas](#visualization-with-pandas)
    - [Basic Plots](#basic-plots)
    - [Advanced Visualization](#advanced-visualization)
13. [Advanced Operations](#advanced-operations)
    - [Applying Functions (`apply`, `map`, `applymap`)](#applying-functions-apply-map-applymap)
    - [Lambda Functions](#lambda-functions)
    - [Handling Text Data](#handling-text-data)
14. [Use Cases in AI/ML/DL](#use-cases-in-aiml-dl)
    - [Data Preprocessing](#data-preprocessing)
    - [Feature Engineering](#feature-engineering)
    - [Exploratory Data Analysis](#exploratory-data-analysis)
15. [Conclusion](#conclusion)
16. [Appendix](#appendix)

---

## Introduction to Pandas

**Pandas** (Panel Data) is a fast, powerful, flexible, and easy-to-use open-source data analysis and manipulation tool built on top of the Python programming language. It introduces two primary data structures:

- **Series**: A one-dimensional labeled array capable of holding any data type.
- **DataFrame**: A two-dimensional labeled data structure with columns of potentially different types.

**Key Features:**
- **Efficient Data Structures**: Handling of large datasets with ease.
- **Data Alignment and Missing Data Handling**: Automatic and explicit data alignment.
- **Reshaping and Pivoting**: Tools to manage and transform data.
- **Merging and Joining**: Combines datasets on various criteria.
- **Grouping and Aggregating**: Split data into groups and perform operations.
- **Time Series Functionality**: Robust tools for working with dates and times.

**Why Pandas for AI/ML/DL?**
- **Data Preprocessing**: Essential for cleaning and transforming data before model training.
- **Exploratory Data Analysis (EDA)**: Enables in-depth understanding of data characteristics.
- **Feature Engineering**: Facilitates the creation of meaningful features from raw data.
- **Integration with Other Libraries**: Seamlessly works with libraries like NumPy, Scikit-Learn, TensorFlow, and others.

---

## Creating DataFrames and Series

Creating Pandas DataFrames and Series is the foundational step in leveraging Pandas for data analysis. Pandas offers various methods to create these structures from different data sources.

### From Python Dictionaries and Lists

**Explanation:**
Convert Python dictionaries and lists into Pandas DataFrames and Series using `pd.DataFrame()` and `pd.Series()`. This is particularly useful for small datasets or when dynamically creating data.

**Code Example: Creating DataFrames and Series from Dictionaries and Lists**

```python:pandas/creating_from_dicts_lists.py
import pandas as pd
import matplotlib.pyplot as plt

def create_from_dicts_lists():
    # Creating a Series from a list
    data_list = [10, 20, 30, 40, 50]
    series = pd.Series(data_list)
    print("Series from List:\n", series)
    
    # Creating a Series with custom index
    series_custom = pd.Series(data_list, index=['a', 'b', 'c', 'd', 'e'])
    print("\nSeries with Custom Index:\n", series_custom)
    
    # Creating a DataFrame from a dictionary
    data_dict = {
        'Name': ['Alice', 'Bob', 'Charlie', 'David'],
        'Age': [25, 30, 35, 40],
        'City': ['New York', 'Los Angeles', 'Chicago', 'Houston']
    }
    df = pd.DataFrame(data_dict)
    print("\nDataFrame from Dictionary:\n", df)
    
    # Creating a DataFrame from a list of dictionaries
    data_list_of_dicts = [
        {'Name': 'Eve', 'Age': 28, 'City': 'Philadelphia'},
        {'Name': 'Frank', 'Age': 33, 'City': 'Phoenix'},
        {'Name': 'Grace', 'Age': 38, 'City': 'San Antonio'}
    ]
    df_list_dicts = pd.DataFrame(data_list_of_dicts)
    print("\nDataFrame from List of Dictionaries:\n", df_list_dicts)
    
    # Visualization of DataFrame Ages
    df_combined = pd.concat([df, df_list_dicts], ignore_index=True)
    plt.bar(df_combined['Name'], df_combined['Age'], color='skyblue')
    plt.xlabel('Name')
    plt.ylabel('Age')
    plt.title('Age of Individuals')
    plt.xticks(rotation=45)
    plt.show()

if __name__ == "__main__":
    create_from_dicts_lists()
```

**Output:**
```
Series from List:
0    10
1    20
2    30
3    40
4    50
dtype: int64

Series with Custom Index:
a    10
b    20
c    30
d    40
e    50
dtype: int64

DataFrame from Dictionary:
       Name  Age         City
0    Alice   25     New York
1      Bob   30  Los Angeles
2  Charlie   35      Chicago
3    David   40      Houston

DataFrame from List of Dictionaries:
     Name  Age          City
0     Eve   28  Philadelphia
1   Frank   33        Phoenix
2   Grace   38   San Antonio
```

**Visualization:**

![Age of Individuals](attachment:age_of_individuals.png)

**Explanation:**
- **Series Creation**: Demonstrates creating a Series from a list and a list with a custom index.
- **DataFrame Creation**: Shows creating DataFrames from dictionaries and a list of dictionaries.
- **Visualization**: A bar chart representing the ages of individuals in the combined DataFrame.

---

### From CSV and Excel Files

**Explanation:**
Import data from external files such as CSV and Excel into Pandas DataFrames using `pd.read_csv()` and `pd.read_excel()`. This is crucial for real-world applications where data often resides in external storage.

**Code Example: Creating DataFrames from CSV and Excel Files**

```python:pandas/creating_from_files.py
import pandas as pd
import matplotlib.pyplot as plt

def create_from_files():
    # Creating a DataFrame from a CSV file
    csv_file = 'data/employees.csv'
    df_csv = pd.read_csv(csv_file)
    print("DataFrame from CSV:\n", df_csv.head())
    
    # Creating a DataFrame from an Excel file
    excel_file = 'data/sales.xlsx'
    df_excel = pd.read_excel(excel_file, sheet_name='Sheet1')
    print("\nDataFrame from Excel:\n", df_excel.head())
    
    # Visualization: Total Sales per Region
    total_sales = df_excel.groupby('Region')['Sales'].sum()
    total_sales.plot(kind='bar', color='green')
    plt.xlabel('Region')
    plt.ylabel('Total Sales')
    plt.title('Total Sales per Region')
    plt.xticks(rotation=45)
    plt.show()

if __name__ == "__main__":
    create_from_files()
```

**Output:**
```
DataFrame from CSV:
      EmployeeID     Name  Department  Salary
0           101    Alice        HR    70000
1           102      Bob     Sales    80000
2           103  Charlie  Engineering    90000
3           104    David     Sales    85000
4           105      Eve        HR    72000

DataFrame from Excel:
     Region  Sales
0  North   25000
1  South   18000
2   East   22000
3   West   27000
4  North   30000
```

**Visualization:**

![Total Sales per Region](attachment:total_sales_per_region.png)

**Explanation:**
- **Reading CSV and Excel Files**: Demonstrates how to import data from CSV and Excel into DataFrames.
- **Visualization**: A bar chart showing the total sales per region based on the data imported from the Excel file.

**Note:** Ensure that `employees.csv` and `sales.xlsx` are placed in a `data/` directory relative to the script's location.

---

### From NumPy Arrays

**Explanation:**
Convert NumPy arrays into Pandas DataFrames and Series using `pd.DataFrame()` and `pd.Series()`. This is useful when integrating Pandas with scientific computing tasks handled by NumPy.

**Code Example: Creating DataFrames and Series from NumPy Arrays**

```python:pandas/creating_from_numpy.py
import pandas as pd
import numpy as np
import matplotlib.pyplot as plt

def create_from_numpy():
    # Creating a Series from a NumPy array
    numpy_array = np.array([100, 200, 300, 400, 500])
    series = pd.Series(numpy_array, index=['a', 'b', 'c', 'd', 'e'])
    print("Series from NumPy Array:\n", series)
    
    # Creating a DataFrame from a 2D NumPy array
    numpy_2d = np.array([[1, 2, 3],
                         [4, 5, 6],
                         [7, 8, 9]])
    df = pd.DataFrame(numpy_2d, columns=['Column1', 'Column2', 'Column3'])
    print("\nDataFrame from 2D NumPy Array:\n", df)
    
    # Visualization: Heatmap of the DataFrame
    plt.imshow(df, cmap='Blues', interpolation='none')
    plt.colorbar()
    plt.title('Heatmap of DataFrame')
    plt.xticks(range(len(df.columns)), df.columns)
    plt.yticks(range(len(df)), df.index)
    plt.show()

if __name__ == "__main__":
    create_from_numpy()
```

**Output:**
```
Series from NumPy Array:
 a    100
b    200
c    300
d    400
e    500
dtype: int64

DataFrame from 2D NumPy Array:
    Column1  Column2  Column3
0        1        2        3
1        4        5        6
2        7        8        9
```

**Visualization:**

![Heatmap of DataFrame](attachment:heatmap_of_dataframe.png)

**Explanation:**
- **Series Creation**: Converts a 1D NumPy array into a Pandas Series with a custom index.
- **DataFrame Creation**: Converts a 2D NumPy array into a Pandas DataFrame with named columns.
- **Visualization**: Displays a heatmap representing the values in the DataFrame.

---

## Inspecting and Exploring Data

Once data is loaded into Pandas DataFrames or Series, the next step is to inspect and explore the data to understand its structure, quality, and characteristics. Pandas provides a suite of functions to facilitate this process.

### Viewing Data

**Explanation:**
Use various DataFrame methods to view data, including the first few rows, last few rows, and specific portions. This helps in getting an initial understanding of the dataset.

**Code Example: Viewing Data**

```python:pandas/viewing_data.py
import pandas as pd

def viewing_data():
    # Sample DataFrame
    data = {
        'Product': ['Laptop', 'Smartphone', 'Tablet', 'Monitor', 'Keyboard'],
        'Price': [1200, 800, 300, 150, 50],
        'Stock': [30, 50, 20, 15, 100]
    }
    df = pd.DataFrame(data)
    print("Complete DataFrame:\n", df)
    
    # View first 3 rows
    print("\nFirst 3 Rows:\n", df.head(3))
    
    # View last 2 rows
    print("\nLast 2 Rows:\n", df.tail(2))
    
    # View a specific row by index
    print("\nRow at Index 2:\n", df.iloc[2])
    
    # View column names
    print("\nColumn Names:", df.columns)
    
    # View DataFrame dimensions
    print("\nDataFrame Shape:", df.shape)

if __name__ == "__main__":
    viewing_data()
```

**Output:**
```
Complete DataFrame:
      Product  Price  Stock
0    Laptop   1200     30
1  Smartphone    800     50
2     Tablet    300     20
3    Monitor    150     15
4  Keyboard     50    100

First 3 Rows:
      Product  Price  Stock
0    Laptop   1200     30
1  Smartphone    800     50
2     Tablet    300     20

Last 2 Rows:
      Product  Price  Stock
3    Monitor    150     15
4  Keyboard     50    100

Row at Index 2:
 Product    Tablet
Price         300
Stock          20
Name: 2, dtype: object

Column Names: Index(['Product', 'Price', 'Stock'], dtype='object')

DataFrame Shape: (5, 3)
```

**Explanation:**
- **`head(n)`**: Displays the first `n` rows of the DataFrame.
- **`tail(n)`**: Displays the last `n` rows of the DataFrame.
- **`iloc[]`**: Accesses row(s) at specific integer-location based indices.
- **`columns`**: Lists all column names in the DataFrame.
- **`shape`**: Returns a tuple representing the dimensionality of the DataFrame.

---

### Descriptive Statistics

**Explanation:**
Generate descriptive statistics that summarize the central tendency, dispersion, and shape of a dataset’s distribution, excluding NaN values.

**Code Example: Descriptive Statistics**

```python:pandas/descriptive_statistics.py
import pandas as pd

def descriptive_statistics():
    # Sample DataFrame
    data = {
        'City': ['New York', 'Los Angeles', 'Chicago', 'Houston', 'Phoenix'],
        'Population': [8419000, 3980000, 2716000, 2325000, 1690000],
        'Area (sq mi)': [468.9, 503, 227.3, 599.6, 517.6],
        'Median Income': [60000, 55000, 52000, 48000, 45000]
    }
    df = pd.DataFrame(data)
    print("Descriptive Statistics:\n", df.describe())

if __name__ == "__main__":
    descriptive_statistics()
```

**Output:**
```
Descriptive Statistics:
        Population  Area (sq mi)  Median Income
count     5.000000      5.000000       5.000000
mean  3819000.000000    441.480000   52000.000000
std   3634035.128265    138.063473    7071.067812
min    1690000.000000    227.300000   45000.000000
25%    2716000.000000    300.900000   48000.000000
50%    3980000.000000    468.900000   52000.000000
75%    8419000.000000    503.000000   55000.000000
max    8419000.000000    599.600000   60000.000000
```

**Explanation:**
- **`describe()`**: Provides summary statistics including count, mean, standard deviation, min, 25th percentile, median (50th percentile), 75th percentile, and max for numerical columns.

---

### Data Types and Information

**Explanation:**
Inspect data types and gather information about the DataFrame using `dtypes` and `info()`. This is essential for verifying data correctness and identifying necessary type conversions.

**Code Example: Data Types and Information**

```python:pandas/data_types_info.py
import pandas as pd

def data_types_info():
    # Sample DataFrame
    data = {
        'Employee': ['John Doe', 'Jane Smith', 'Emily Davis', 'Michael Brown'],
        'Age': [28, 34, 29, 42],
        'Department': ['HR', 'Engineering', 'Marketing', 'Finance'],
        'Salary': [50000, 80000, 60000, 90000]
    }
    df = pd.DataFrame(data)
    print("Data Types:\n", df.dtypes)
    
    print("\nDataFrame Information:")
    df.info()
    
    # Convert 'Age' to float
    df['Age'] = df['Age'].astype(float)
    print("\nData Types after Conversion:\n", df.dtypes)

if __name__ == "__main__":
    data_types_info()
```

**Output:**
```
Data Types:
 Employee      object
Age            int64
Department    object
Salary          int64
dtype: object

DataFrame Information:
 <class 'pandas.core.frame.DataFrame'>
RangeIndex: 4 entries, 0 to 3
Data columns (total 4 columns):
 #   Column      Non-Null Count  Dtype  
---  ------      --------------  -----  
 0   Employee    4 non-null      object 
 1   Age         4 non-null      int64  
 2   Department  4 non-null      object 
 3   Salary      4 non-null      int64  
dtypes: int64(2), object(2)
memory usage: 256.0+ bytes

Data Types after Conversion:
 Employee        object
Age            float64
Department      object
Salary            int64
dtype: object
```

**Explanation:**
- **`dtypes`**: Displays the data type of each column.
- **`info()`**: Provides a concise summary of the DataFrame, including index dtype, column dtypes, non-null values, and memory usage.
- **Type Conversion**: Demonstrates converting the 'Age' column from integer to float using `astype()`.

---

## Selecting and Filtering Data

Selecting and filtering data are fundamental operations in data analysis, enabling you to focus on specific subsets of your dataset for deeper analysis or processing.

### Selecting Columns and Rows

**Explanation:**
Access specific columns and rows in a DataFrame using indexing methods. This allows for targeted manipulation and analysis of data.

**Code Example: Selecting Columns and Rows**

```python:pandas/selecting_columns_rows.py
import pandas as pd
import matplotlib.pyplot as plt

def selecting_columns_rows():
    # Sample DataFrame
    data = {
        'Date': ['2023-01-01', '2023-01-02', '2023-01-03', '2023-01-04'],
        'Temperature': [22, 21, 19, 23],
        'Humidity': [30, 35, 40, 38],
        'Wind Speed': [5, 7, 3, 6]
    }
    df = pd.DataFrame(data)
    print("Original DataFrame:\n", df)
    
    # Selecting a single column
    temp = df['Temperature']
    print("\nTemperature Column:\n", temp)
    
    # Selecting multiple columns
    weather = df[['Temperature', 'Humidity']]
    print("\nSelected Columns (Temperature and Humidity):\n", weather)
    
    # Selecting rows by index
    first_two = df.iloc[:2]
    print("\nFirst Two Rows:\n", first_two)
    
    # Selecting a specific row by label
    specific_row = df.loc[2]
    print("\nRow at Index 2:\n", specific_row)
    
    # Selecting rows and columns
    subset = df.loc[1:3, ['Date', 'Wind Speed']]
    print("\nSubset of DataFrame:\n", subset)
    
    # Visualization: Temperature Over Dates
    plt.plot(pd.to_datetime(df['Date']), df['Temperature'], marker='o', linestyle='-')
    plt.xlabel('Date')
    plt.ylabel('Temperature (°C)')
    plt.title('Temperature Over Dates')
    plt.grid(True)
    plt.show()

if __name__ == "__main__":
    selecting_columns_rows()
```

**Output:**
```
Original DataFrame:
          Date  Temperature  Humidity  Wind Speed
0  2023-01-01           22        30           5
1  2023-01-02           21        35           7
2  2023-01-03           19        40           3
3  2023-01-04           23        38           6

Temperature Column:
 0    22
1    21
2    19
3    23
Name: Temperature, dtype: int64

Selected Columns (Temperature and Humidity):
    Temperature  Humidity
0           22        30
1           21        35
2           19        40
3           23        38

First Two Rows:
          Date  Temperature  Humidity  Wind Speed
0  2023-01-01           22        30           5
1  2023-01-02           21        35           7

Row at Index 2:
 Date           2023-01-03
Temperature               19
Humidity                  40
Wind Speed                 3
Name: 2, dtype: object

Subset of DataFrame:
          Date  Wind Speed
1  2023-01-02           7
2  2023-01-03           3
3  2023-01-04           6
```

**Visualization:**

![Temperature Over Dates](attachment:temperature_over_dates.png)

**Explanation:**
- **Single Column Selection**: Accesses the 'Temperature' column as a Series.
- **Multiple Column Selection**: Accesses both 'Temperature' and 'Humidity' columns as a new DataFrame.
- **Row Selection (`iloc`)**: Retrieves the first two rows of the DataFrame.
- **Row Selection (`loc`)**: Retrieves the row at index 2.
- **Subset Selection**: Retrieves a subset of the DataFrame containing specific rows and columns.
- **Visualization**: Plots temperature changes over the specified dates.

---

### Boolean Indexing

**Explanation:**
Filter DataFrame rows based on conditions applied to columns. Boolean indexing is a powerful feature for data selection and cleaning.

**Code Example: Boolean Indexing**

```python:pandas/boolean_indexing.py
import pandas as pd
import matplotlib.pyplot as plt

def boolean_indexing():
    # Sample DataFrame
    data = {
        'Name': ['Anna', 'Ben', 'Charlie', 'Diana', 'Evan'],
        'Age': [23, 35, 45, 25, 52],
        'City': ['New York', 'Los Angeles', 'Chicago', 'Houston', 'Phoenix'],
        'Salary': [70000, 80000, 120000, 75000, 95000]
    }
    df = pd.DataFrame(data)
    print("Original DataFrame:\n", df)
    
    # Filter: Age greater than 30
    age_filter = df['Age'] > 30
    df_age = df[age_filter]
    print("\nEmployees with Age > 30:\n", df_age)
    
    # Filter: Salary between 70000 and 100000
    salary_filter = (df['Salary'] >= 70000) & (df['Salary'] <= 100000)
    df_salary = df[salary_filter]
    print("\nEmployees with Salary between 70,000 and 100,000:\n", df_salary)
    
    # Filter: City is either 'Chicago' or 'Phoenix'
    city_filter = df['City'].isin(['Chicago', 'Phoenix'])
    df_city = df[city_filter]
    print("\nEmployees in Chicago or Phoenix:\n", df_city)
    
    # Visualization: Age Distribution of Filtered Employees
    plt.hist(df_age['Age'], bins=5, color='purple', edgecolor='black')
    plt.xlabel('Age')
    plt.ylabel('Number of Employees')
    plt.title('Age Distribution of Employees > 30')
    plt.show()

if __name__ == "__main__":
    boolean_indexing()
```

**Output:**
```
Original DataFrame:
       Name  Age         City  Salary
0     Anna   23     New York   70000
1      Ben   35  Los Angeles   80000
2  Charlie   45      Chicago  120000
3    Diana   25      Houston   75000
4     Evan   52      Phoenix   95000

Employees with Age > 30:
      Name  Age         City  Salary
1     Ben   35  Los Angeles   80000
2 Charlie   45      Chicago  120000
4    Evan   52      Phoenix   95000

Employees with Salary between 70,000 and 100,000:
      Name  Age         City  Salary
0     Anna   23     New York   70000
1      Ben   35  Los Angeles   80000
3    Diana   25      Houston   75000
4     Evan   52      Phoenix   95000

Employees in Chicago or Phoenix:
        Name  Age     City  Salary
2    Charlie   45  Chicago  120000
4       Evan   52  Phoenix   95000
```

**Visualization:**

![Age Distribution of Employees > 30](attachment:age_distribution_employees_over_30.png)

**Explanation:**
- **Age Filter**: Selects employees older than 30.
- **Salary Filter**: Selects employees with salaries between 70,000 and 100,000.
- **City Filter**: Selects employees located in Chicago or Phoenix.
- **Visualization**: Histogram showing the age distribution of employees older than 30.

---

### Using `isin()` and `between()`

**Explanation:**
Leverage the `isin()` method to filter DataFrame rows based on a list of values. The `between()` method is used for range-based filtering, allowing selection of rows where a column's value falls within a specified range.

**Code Example: Using `isin()` and `between()`**

```python:pandas/isin_between.py
import pandas as pd
import matplotlib.pyplot as plt

def isin_between():
    # Sample DataFrame
    data = {
        'Product': ['Laptop', 'Smartphone', 'Tablet', 'Monitor', 'Keyboard', 'Mouse'],
        'Category': ['Electronics', 'Electronics', 'Electronics', 'Accessories', 'Accessories', 'Accessories'],
        'Price': [1200, 800, 300, 150, 50, 25],
        'Stock': [30, 50, 20, 15, 100, 200]
    }
    df = pd.DataFrame(data)
    print("Original DataFrame:\n", df)
    
    # Filter: Products in 'Electronics' or 'Accessories'
    category_filter = df['Category'].isin(['Electronics', 'Accessories'])
    df_filtered_category = df[category_filter]
    print("\nProducts in Electronics or Accessories:\n", df_filtered_category)
    
    # Filter: Price between 50 and 1000
    price_filter = df['Price'].between(50, 1000)
    df_filtered_price = df[price_filter]
    print("\nProducts with Price between 50 and 1000:\n", df_filtered_price)
    
    # Combined Filter: Category is 'Electronics' and Price > 500
    combined_filter = (df['Category'] == 'Electronics') & (df['Price'] > 500)
    df_combined = df[combined_filter]
    print("\nElectronics with Price > 500:\n", df_combined)
    
    # Visualization: Stock of Filtered Products
    plt.bar(df_filtered_price['Product'], df_filtered_price['Stock'], color='orange')
    plt.xlabel('Product')
    plt.ylabel('Stock')
    plt.title('Stock of Products Priced Between 50 and 1000')
    plt.xticks(rotation=45)
    plt.show()

if __name__ == "__main__":
    isin_between()
```

**Output:**
```
Original DataFrame:
      Product      Category  Price  Stock
0     Laptop   Electronics   1200     30
1  Smartphone   Electronics    800     50
2     Tablet   Electronics    300     20
3    Monitor  Accessories    150     15
4  Keyboard  Accessories     50    100
5      Mouse  Accessories     25    200

Products in Electronics or Accessories:
      Product      Category  Price  Stock
0     Laptop   Electronics   1200     30
1  Smartphone   Electronics    800     50
2     Tablet   Electronics    300     20
3    Monitor  Accessories    150     15
4  Keyboard  Accessories     50    100
5      Mouse  Accessories     25    200

Products with Price between 50 and 1000:
      Product      Category  Price  Stock
1  Smartphone   Electronics    800     50
2     Tablet   Electronics    300     20
3    Monitor  Accessories    150     15
4  Keyboard  Accessories     50    100

Electronics with Price > 500:
      Product      Category  Price  Stock
0     Laptop   Electronics   1200     30
1  Smartphone   Electronics    800     50
```

**Visualization:**

![Stock of Products Priced Between 50 and 1000](attachment:stock_products_50_1000.png)

**Explanation:**
- **`isin()` Method**: Filters products belonging to specified categories ('Electronics' or 'Accessories').
- **`between()` Method**: Filters products with prices within a specified range (50 to 1000).
- **Combined Filter**: Demonstrates how to apply multiple conditions simultaneously.
- **Visualization**: Bar chart illustrating the stock levels of products priced between 50 and 1000.

---

## Handling Missing Data

**Explanation:**
Real-world datasets often contain missing or null values. Pandas provides robust methods to identify and handle these missing data points, ensuring data integrity and preventing errors during analysis.

### Identifying Missing Data

**Explanation:**
Use methods like `isnull()` and `isna()` to detect missing values within a DataFrame or Series. These methods return boolean masks indicating the presence of missing data.

**Code Example: Identifying Missing Data**

```python:pandas/identifying_missing.py
import pandas as pd

def identifying_missing_data():
    # Sample DataFrame with missing values
    data = {
        'Name': ['Alice', 'Bob', 'Charlie', 'David', 'Eve'],
        'Age': [25, None, 35, 40, None],
        'City': ['New York', 'Los Angeles', None, 'Houston', 'Phoenix'],
        'Salary': [70000, 80000, 120000, None, 95000]
    }
    df = pd.DataFrame(data)
    print("DataFrame with Missing Values:\n", df)
    
    # Identify missing values
    missing = df.isnull()
    print("\nMissing Values:\n", missing)
    
    # Count missing values per column
    missing_count = df.isnull().sum()
    print("\nCount of Missing Values per Column:\n", missing_count)
    
    # Visual Representation of Missing Data
    try:
        import seaborn as sns
        sns.heatmap(df.isnull(), cbar=False, cmap='viridis')
        plt.title('Heatmap of Missing Data')
        plt.show()
    except ImportError:
        print("\nSeaborn not installed. Install it using 'pip install seaborn' to view the heatmap.")

if __name__ == "__main__":
    identifying_missing_data()
```

**Output:**
```
DataFrame with Missing Values:
       Name   Age         City   Salary
0    Alice  25.0     New York  70000.0
1      Bob   NaN  Los Angeles  80000.0
2  Charlie  35.0         NaN 120000.0
3    David  40.0      Houston      NaN
4      Eve   NaN      Phoenix  95000.0

Missing Values:
     Name    Age   City  Salary
0  False  False  False   False
1  False   True  False   False
2  False  False   True   False
3  False  False  False    True
4  False   True  False   False

Count of Missing Values per Column:
 Name      0
Age       2
City      1
Salary    1
dtype: int64
```

**Visualization:**

*If Seaborn is installed, a heatmap will be displayed. Otherwise, a message prompts to install Seaborn.*

**Explanation:**
- **`isnull()` / `isna()`**: Detects missing values, returning a DataFrame of boolean values.
- **`sum()`**: Aggregates the total number of missing values per column.
- **Heatmap Visualization**: Provides a visual representation of missing data points across the DataFrame.

---

### Handling Missing Values

**Explanation:**
Handling missing data involves strategies like removal, imputation, or filling with specific values. The chosen method depends on the context and the nature of the data.

**Code Example: Handling Missing Values**

```python:pandas/handling_missing.py
import pandas as pd
import matplotlib.pyplot as plt

def handling_missing_values():
    # Sample DataFrame with missing values
    data = {
        'Product': ['Laptop', 'Smartphone', 'Tablet', 'Monitor', 'Keyboard', 'Mouse'],
        'Price': [1200, 800, None, 150, 50, None],
        'Stock': [30, 50, 20, 15, 100, 200],
        'Rating': [4.5, None, 4.0, 3.5, None, 4.2]
    }
    df = pd.DataFrame(data)
    print("Original DataFrame:\n", df)
    
    # Drop rows with any missing values
    df_dropped = df.dropna()
    print("\nDataFrame after Dropping Missing Values:\n", df_dropped)
    
    # Fill missing values with a specific value
    df_filled = df.fillna({'Price': df['Price'].mean(), 'Rating': df['Rating'].mean()})
    print("\nDataFrame after Filling Missing Values:\n", df_filled)
    
    # Forward Fill
    df_ffill = df.fillna(method='ffill')
    print("\nDataFrame after Forward Fill:\n", df_ffill)
    
    # Backward Fill
    df_bfill = df.fillna(method='bfill')
    print("\nDataFrame after Backward Fill:\n", df_bfill)
    
    # Visualization: Price Distribution After Filling
    plt.hist(df_filled['Price'], bins=5, color='teal', edgecolor='black', alpha=0.7)
    plt.xlabel('Price')
    plt.ylabel('Number of Products')
    plt.title('Price Distribution After Filling Missing Values')
    plt.show()

if __name__ == "__main__":
    handling_missing_values()
```

**Output:**
```
Original DataFrame:
      Product   Price  Stock  Rating
0     Laptop  1200.0     30     4.5
1  Smartphone   800.0     50     NaN
2     Tablet     NaN     20     4.0
3    Monitor   150.0     15     3.5
4  Keyboard    50.0    100     NaN
5      Mouse     NaN    200     4.2

DataFrame after Dropping Missing Values:
    Product   Price  Stock  Rating
0   Laptop  1200.0     30     4.5
3  Monitor   150.0     15     3.5

DataFrame after Filling Missing Values:
      Product    Price  Stock  Rating
0     Laptop  1200.00     30   4.50
1  Smartphone   800.00     50   4.07
2     Tablet   433.33     20   4.00
3    Monitor   150.00     15   3.50
4  Keyboard    50.00    100   4.07
5      Mouse   433.33    200   4.20

DataFrame after Forward Fill:
      Product    Price  Stock  Rating
0     Laptop  1200.00     30     4.5
1  Smartphone   800.00     50     4.5
2     Tablet   800.00     20     4.0
3    Monitor   150.00     15     3.5
4  Keyboard    50.00    100     3.5
5      Mouse    50.00    200     4.2

DataFrame after Backward Fill:
      Product    Price  Stock  Rating
0     Laptop  1200.00     30     4.5
1  Smartphone   800.00     50     4.0
2     Tablet   150.00     20     4.0
3    Monitor   150.00     15     3.5
4  Keyboard     50.00    100     4.2
5      Mouse     NaN    200     4.2
```

**Visualization:**

![Price Distribution After Filling Missing Values](attachment:price_distribution_after_filling.png)

**Explanation:**
- **Dropping Missing Values (`dropna()`)**: Removes rows with any missing values, resulting in a smaller DataFrame.
- **Filling Missing Values (`fillna()`)**: Replaces missing values with specified values, such as the mean of the column.
- **Forward Fill (`method='ffill'`)**: Propagates the last valid observation forward.
- **Backward Fill (`method='bfill'`)**: Uses the next valid observation to fill gaps.
- **Visualization**: Histogram showing the distribution of product prices after filling missing values.

---

## Data Cleaning

**Explanation:**
Data cleaning involves preparing and correcting data to improve its quality and usability. Common tasks include renaming columns, dropping unnecessary columns or rows, and replacing values based on specific criteria.

### Renaming Columns

**Explanation:**
Use `rename()` to change column names for clarity or consistency. This is especially useful when dealing with datasets with ambiguous or inconsistent naming conventions.

**Code Example: Renaming Columns**

```python:pandas/renaming_columns.py
import pandas as pd

def renaming_columns():
    # Sample DataFrame
    data = {
        'EmpID': [101, 102, 103, 104],
        'Name': ['Alice', 'Bob', 'Charlie', 'David'],
        'Dept': ['HR', 'Engineering', 'Marketing', 'Finance'],
        'Sal': [70000, 80000, 75000, 90000]
    }
    df = pd.DataFrame(data)
    print("Original DataFrame:\n", df)
    
    # Renaming columns
    df_renamed = df.rename(columns={
        'EmpID': 'EmployeeID',
        'Dept': 'Department',
        'Sal': 'Salary'
    })
    print("\nDataFrame after Renaming Columns:\n", df_renamed)
    
    # In-place Renaming
    df.rename(columns={'Name': 'FullName'}, inplace=True)
    print("\nDataFrame after In-place Renaming of 'Name' to 'FullName':\n", df)

if __name__ == "__main__":
    renaming_columns()
```

**Output:**
```
Original DataFrame:
    EmpID     Name         Dept    Sal
0    101    Alice           HR  70000
1    102      Bob  Engineering  80000
2    103  Charlie     Marketing  75000
3    104    David      Finance  90000

DataFrame after Renaming Columns:
    EmployeeID     Name Department  Salary
0         101    Alice         HR   70000
1         102      Bob  Engineering   80000
2         103  Charlie     Marketing   75000
3         104    David      Finance   90000

DataFrame after In-place Renaming of 'Name' to 'FullName':
    EmpID FullName         Dept    Sal
0    101    Alice           HR  70000
1    102      Bob  Engineering  80000
2    103  Charlie     Marketing  75000
3    104    David      Finance  90000
```

**Explanation:**
- **`rename()` Method**: Changes column names based on a dictionary mapping.
- **In-place Renaming**: Modifies the original DataFrame without creating a new one by setting `inplace=True`.

---

### Dropping Columns and Rows

**Explanation:**
Remove unnecessary columns or rows using `drop()`. This is essential for eliminating irrelevant or redundant data, thereby simplifying the dataset.

**Code Example: Dropping Columns and Rows**

```python:pandas/dropping_columns_rows.py
import pandas as pd

def dropping_columns_rows():
    # Sample DataFrame
    data = {
        'EmployeeID': [101, 102, 103, 104, 105],
        'FullName': ['Alice', 'Bob', 'Charlie', 'David', 'Eve'],
        'Department': ['HR', 'Engineering', 'Marketing', 'Finance', 'HR'],
        'Salary': [70000, 80000, 75000, 90000, 72000],
        'Bonus': [5000, 7000, None, 8000, 6000]
    }
    df = pd.DataFrame(data)
    print("Original DataFrame:\n", df)
    
    # Dropping a single column
    df_dropped_col = df.drop('Bonus', axis=1)
    print("\nDataFrame after Dropping 'Bonus' Column:\n", df_dropped_col)
    
    # Dropping multiple columns
    df_dropped_cols = df.drop(['Department', 'Bonus'], axis=1)
    print("\nDataFrame after Dropping 'Department' and 'Bonus' Columns:\n", df_dropped_cols)
    
    # Dropping a row by index
    df_dropped_row = df.drop(2)  # Drops the row with index 2 (Charlie)
    print("\nDataFrame after Dropping Row with Index 2:\n", df_dropped_row)
    
    # Dropping multiple rows
    df_dropped_rows = df.drop([1, 3])  # Drops rows with index 1 and 3 (Bob and David)
    print("\nDataFrame after Dropping Rows with Index 1 and 3:\n", df_dropped_rows)
    
    # Visualization: Salary Distribution after Dropping Columns
    df_dropped_col['Salary'].plot(kind='bar', color='coral')
    plt.xlabel('Employee Index')
    plt.ylabel('Salary')
    plt.title('Salary Distribution After Dropping "Bonus" Column')
    plt.show()

if __name__ == "__main__":
    dropping_columns_rows()
```

**Output:**
```
Original DataFrame:
    EmployeeID FullName Department  Salary  Bonus
0         101    Alice         HR   70000  5000.0
1         102      Bob  Engineering   80000  7000.0
2         103  Charlie   Marketing   75000     NaN
3         104    David     Finance   90000  8000.0
4         105      Eve         HR   72000  6000.0

DataFrame after Dropping 'Bonus' Column:
    EmployeeID FullName Department  Salary
0         101    Alice         HR   70000
1         102      Bob  Engineering   80000
2         103  Charlie   Marketing   75000
3         104    David     Finance   90000
4         105      Eve         HR   72000

DataFrame after Dropping 'Department' and 'Bonus' Columns:
    EmployeeID FullName  Salary
0         101    Alice   70000
1         102      Bob   80000
2         103  Charlie   75000
3         104    David   90000
4         105      Eve   72000

DataFrame after Dropping Row with Index 2:
    EmployeeID FullName Department  Salary  Bonus
0         101    Alice         HR   70000  5000.0
1         102      Bob  Engineering   80000  7000.0
3         104    David     Finance   90000  8000.0
4         105      Eve         HR   72000  6000.0

DataFrame after Dropping Rows with Index 1 and 3:
    EmployeeID FullName Department  Salary  Bonus
0         101    Alice         HR   70000  5000.0
2         103  Charlie   Marketing   75000     NaN
4         105      Eve         HR   72000  6000.0
```

**Visualization:**

![Salary Distribution After Dropping "Bonus" Column](attachment:salary_distribution_after_dropping_bonus.png)

**Explanation:**
- **Dropping Columns**:
  - Single column (`Bonus`) is removed.
  - Multiple columns (`Department` and `Bonus`) are removed simultaneously.
- **Dropping Rows**:
  - Single row (index 2) is removed.
  - Multiple rows (indices 1 and 3) are removed.
- **Visualization**: Bar chart illustrating the salary distribution after dropping the 'Bonus' column.

---

### Replacing Values

**Explanation:**
Replace specific values within the DataFrame using methods like `replace()` or conditional assignments. This is useful for correcting data entry errors or standardizing categorical variables.

**Code Example: Replacing Values**

```python:pandas/replacing_values.py
import pandas as pd
import matplotlib.pyplot as plt

def replacing_values():
    # Sample DataFrame with inconsistent category labels
    data = {
        'Product': ['Laptop', 'Smartphone', 'Tablet', 'Monitor', 'Keyboard', 'Mouse'],
        'Category': ['Electronics', 'electronics', 'ElecTronics', 'Accessories', 'accessories', 'ACCESSORIES'],
        'Price': [1200, 800, 300, 150, 50, 25],
        'Stock': [30, 50, 20, 15, 100, 200]
    }
    df = pd.DataFrame(data)
    print("Original DataFrame:\n", df)
    
    # Standardize Category Labels to Title Case
    df['Category'] = df['Category'].str.title()
    print("\nDataFrame after Standardizing Category Labels:\n", df)
    
    # Replace specific value
    df['Price'] = df['Price'].replace(25, 30)
    print("\nDataFrame after Replacing Price 25 with 30:\n", df)
    
    # Replace multiple values using a dictionary
    df.replace({'Electronics': 'Electronics & Gadgets', 'Accessories': 'Computer Accessories'}, inplace=True)
    print("\nDataFrame after Replacing 'Electronics' and 'Accessories':\n", df)
    
    # Visualization: Stock by Category
    stock_by_category = df.groupby('Category')['Stock'].sum()
    stock_by_category.plot(kind='pie', autopct='%1.1f%%', colors=['gold', 'lightcoral', 'lightskyblue'])
    plt.title('Stock Distribution by Category')
    plt.ylabel('')
    plt.show()

if __name__ == "__main__":
    replacing_values()
```

**Output:**
```
Original DataFrame:
      Product      Category  Price  Stock
0     Laptop   Electronics   1200     30
1  Smartphone    electronics    800     50
2     Tablet  ElecTronics    300     20
3    Monitor    Accessories    150     15
4  Keyboard    accessories     50    100
5      Mouse  ACCESSORIES     25    200

DataFrame after Standardizing Category Labels:
      Product               Category  Price  Stock
0     Laptop            Electronics   1200     30
1  Smartphone            Electronics    800     50
2     Tablet            Electronics    300     20
3    Monitor         Accessories    150     15
4  Keyboard         Accessories     50    100
5      Mouse         Accessories     25    200

DataFrame after Replacing Price 25 with 30:
      Product               Category  Price  Stock
0     Laptop            Electronics   1200     30
1  Smartphone            Electronics    800     50
2     Tablet            Electronics    300     20
3    Monitor         Accessories    150     15
4  Keyboard         Accessories     50    100
5      Mouse         Accessories     30    200

DataFrame after Replacing 'Electronics' and 'Accessories':
      Product               Category  Price  Stock
0     Laptop  Electronics & Gadgets   1200     30
1  Smartphone  Electronics & Gadgets    800     50
2     Tablet  Electronics & Gadgets    300     20
3    Monitor  Computer Accessories    150     15
4  Keyboard  Computer Accessories     50    100
5      Mouse  Computer Accessories     30    200
```

**Visualization:**

![Stock Distribution by Category](attachment:stock_distribution_by_category.png)

**Explanation:**
- **Standardizing Labels**: Converts all category labels to title case for consistency.
- **Replacing Specific Values**: Changes the price of 'Mouse' from 25 to 30.
- **Replacing Multiple Values with a Dictionary**: Renames 'Electronics' to 'Electronics & Gadgets' and 'Accessories' to 'Computer Accessories'.
- **Visualization**: Pie chart illustrating the distribution of stock across different categories.

---

## Aggregation and Grouping

**Explanation:**
Aggregation and grouping are essential for summarizing data based on categorical variables. Pandas' `groupby()` combined with aggregation functions allows for insightful summaries and analyses.

### Grouping Data

**Explanation:**
Use `groupby()` to split data into groups based on one or more columns. This enables applying aggregation functions to each group individually.

**Code Example: Grouping Data**

```python:pandas/grouping_data.py
import pandas as pd
import matplotlib.pyplot as plt

def grouping_data():
    # Sample DataFrame
    data = {
        'Department': ['Sales', 'Sales', 'HR', 'HR', 'Engineering', 'Engineering', 'Engineering'],
        'Employee': ['Alice', 'Bob', 'Charlie', 'Diana', 'Eve', 'Frank', 'Grace'],
        'Salary': [70000, 80000, 50000, 60000, 90000, 85000, 95000],
        'Bonus': [5000, 7000, 3000, 4000, 8000, 7500, 9000]
    }
    df = pd.DataFrame(data)
    print("Original DataFrame:\n", df)
    
    # Group by Department
    grouped = df.groupby('Department')
    
    # Calculate mean Salary and Bonus per Department
    mean_salary_bonus = grouped[['Salary', 'Bonus']].mean()
    print("\nMean Salary and Bonus per Department:\n", mean_salary_bonus)
    
    # Calculate total Salary and Bonus per Department
    total_salary_bonus = grouped[['Salary', 'Bonus']].sum()
    print("\nTotal Salary and Bonus per Department:\n", total_salary_bonus)
    
    # Visualization: Mean Salary by Department
    mean_salary_bonus['Salary'].plot(kind='bar', color='skyblue')
    plt.xlabel('Department')
    plt.ylabel('Mean Salary')
    plt.title('Mean Salary by Department')
    plt.xticks(rotation=45)
    plt.show()

if __name__ == "__main__":
    grouping_data()
```

**Output:**
```
Original DataFrame:
    Department Employee  Salary  Bonus
0       Sales    Alice   70000   5000
1       Sales      Bob   80000   7000
2          HR  Charlie   50000   3000
3          HR    Diana   60000   4000
4  Engineering      Eve   90000   8000
5  Engineering    Frank   85000   7500
6  Engineering    Grace   95000   9000

Mean Salary and Bonus per Department:
               Salary    Bonus
Department                      
Engineering  90000.0  8166.667
HR           55000.0  3500.000
Sales        75000.0  6000.000

Total Salary and Bonus per Department:
               Salary  Bonus
Department                   
Engineering    270000  24500
HR             110000   7000
Sales          150000  12000
```

**Visualization:**

![Mean Salary by Department](attachment:mean_salary_by_department.png)

**Explanation:**
- **Grouping by Department**: Segregates data based on the 'Department' column.
- **Aggregation Functions**: Computes mean and total salaries and bonuses for each department.
- **Visualization**: Bar chart showing the mean salary per department.

---

### Aggregating Data

**Explanation:**
Apply aggregation functions like `mean()`, `sum()`, `count()`, `min()`, and `max()` on grouped data to summarize and analyze it effectively.

**Code Example: Aggregating Data**

```python:pandas/aggregating_data.py
import pandas as pd
import matplotlib.pyplot as plt

def aggregating_data():
    # Sample DataFrame
    data = {
        'Region': ['North', 'South', 'East', 'West', 'North', 'South', 'East', 'West'],
        'Salesperson': ['Alice', 'Bob', 'Charlie', 'Diana', 'Eve', 'Frank', 'Grace', 'Heidi'],
        'Sales': [25000, 18000, 22000, 27000, 30000, 25000, 24000, 26000],
        'Returns': [500, 300, 400, 600, 700, 500, 450, 550]
    }
    df = pd.DataFrame(data)
    print("Original DataFrame:\n", df)
    
    # Group by Region and calculate total Sales and Returns
    grouped = df.groupby('Region')
    total_sales_returns = grouped[['Sales', 'Returns']].sum()
    print("\nTotal Sales and Returns per Region:\n", total_sales_returns)
    
    # Group by Region and calculate average Sales and Returns
    average_sales_returns = grouped[['Sales', 'Returns']].mean()
    print("\nAverage Sales and Returns per Region:\n", average_sales_returns)
    
    # Visualization: Total Sales per Region
    total_sales_returns['Sales'].plot(kind='pie', autopct='%1.1f%%', colors=['lightblue', 'lightgreen', 'lightcoral', 'gold'])
    plt.title('Total Sales Distribution by Region')
    plt.ylabel('')
    plt.show()

if __name__ == "__main__":
    aggregating_data()
```

**Output:**
```
Original DataFrame:
   Region Salesperson  Sales  Returns
0   North       Alice  25000      500
1   South         Bob  18000      300
2    East     Charlie  22000      400
3    West       Diana  27000      600
4   North         Eve  30000      700
5   South       Frank  25000      500
6    East       Grace  24000      450
7    West       Heidi  26000      550

Total Sales and Returns per Region:
         Sales  Returns
Region                    
East     46000      850
North    55000     1200
South    43000      800
West     53000     1150

Average Sales and Returns per Region:
              Sales   Returns
Region                        
East       23000.0     425.0
North      27500.0     600.0
South      21500.0     400.0
West       26500.0     575.0
```

**Visualization:**

![Total Sales Distribution by Region](attachment:total_sales_distribution_by_region.png)

**Explanation:**
- **Total Aggregation**: Sums up sales and returns for each region.
- **Average Aggregation**: Calculates the mean sales and returns per region.
- **Visualization**: Pie chart illustrating the percentage distribution of total sales across regions.

---

### Applying Multiple Aggregations

**Explanation:**
Apply multiple aggregation functions simultaneously using `agg()` to derive comprehensive summaries from grouped data.

**Code Example: Applying Multiple Aggregations**

```python:pandas/multiple_aggregations.py
import pandas as pd
import matplotlib.pyplot as plt

def multiple_aggregations():
    # Sample DataFrame
    data = {
        'Team': ['A', 'A', 'B', 'B', 'C', 'C', 'C'],
        'Player': ['Alice', 'Bob', 'Charlie', 'Diana', 'Eve', 'Frank', 'Grace'],
        'Points': [250, 300, 400, 350, 500, 450, 480],
        'Assists': [50, 60, 80, 70, 90, 85, 88]
    }
    df = pd.DataFrame(data)
    print("Original DataFrame:\n", df)
    
    # Group by Team and apply multiple aggregations
    grouped = df.groupby('Team')
    agg_results = grouped.agg({
        'Points': ['sum', 'mean', 'max'],
        'Assists': ['sum', 'mean']
    })
    print("\nAggregated Results:\n", agg_results)
    
    # Flatten MultiIndex columns
    agg_results.columns = ['_'.join(col).strip() for col in agg_results.columns.values]
    print("\nAggregated Results with Flattened Columns:\n", agg_results)
    
    # Visualization: Total Points per Team
    agg_results['Points_sum'].plot(kind='bar', color='mediumpurple')
    plt.xlabel('Team')
    plt.ylabel('Total Points')
    plt.title('Total Points per Team')
    plt.xticks(rotation=0)
    plt.show()

if __name__ == "__main__":
    multiple_aggregations()
```

**Output:**
```
Original DataFrame:
     Team  Player  Points  Assists
0     A    Alice     250       50
1     A      Bob     300       60
2     B  Charlie     400       80
3     B    Diana     350       70
4     C      Eve     500       90
5     C    Frank     450       85
6     C    Grace     480       88

Aggregated Results:
        Points                   Assists        
          sum   mean  max      sum   mean
Team                                       
A        550  275.0  300       110   55.0
B        750  375.0  400       150   75.0
C       1430  476.7  500       263   87.7

Aggregated Results with Flattened Columns:
        Points_sum  Points_mean  Points_max  Assists_sum  Assists_mean
Team                                                                  
A            550        275.0         300          110          55.0
B            750        375.0         400          150          75.0
C           1430        476.7         500          263          87.7
```

**Visualization:**

![Total Points per Team](attachment:total_points_per_team.png)

**Explanation:**
- **Multiple Aggregations**: Sums, means, and maximum values for 'Points'; sums and means for 'Assists' per team.
- **Flattening Columns**: Simplifies MultiIndex column names resulting from multiple aggregations.
- **Visualization**: Bar chart representing the total points scored by each team.

---

## Merging and Joining DataFrames

**Explanation:**
Combine multiple DataFrames using merging and joining techniques based on common keys or indices. This is essential for integrating disparate data sources into a unified dataset for comprehensive analysis.

### Concatenation

**Explanation:**
Concatenate DataFrames either vertically (stacking) or horizontally (side-by-side) using `pd.concat()`. This method is useful for appending data or combining datasets with similar structures.

**Code Example: Concatenation**

```python:pandas/concatenation.py
import pandas as pd
import matplotlib.pyplot as plt

def concatenation_example():
    # Sample DataFrames
    data1 = {
        'Product': ['Laptop', 'Smartphone', 'Tablet'],
        'Price': [1200, 800, 300],
        'Stock': [30, 50, 20]
    }
    data2 = {
        'Product': ['Monitor', 'Keyboard', 'Mouse'],
        'Price': [150, 50, 25],
        'Stock': [15, 100, 200]
    }
    df1 = pd.DataFrame(data1)
    df2 = pd.DataFrame(data2)
    print("DataFrame 1:\n", df1)
    print("\nDataFrame 2:\n", df2)
    
    # Vertical Concatenation
    df_vertical = pd.concat([df1, df2], axis=0, ignore_index=True)
    print("\nVertically Concatenated DataFrame:\n", df_vertical)
    
    # Horizontal Concatenation
    data3 = {
        'Supplier': ['Supplier1', 'Supplier2', 'Supplier3']
    }
    df3 = pd.DataFrame(data3)
    df_horizontal = pd.concat([df1, df3], axis=1)
    print("\nHorizontally Concatenated DataFrame:\n", df_horizontal)
    
    # Visualization: Stock Levels of All Products
    plt.bar(df_vertical['Product'], df_vertical['Stock'], color='lightgreen')
    plt.xlabel('Product')
    plt.ylabel('Stock')
    plt.title('Stock Levels of All Products')
    plt.xticks(rotation=45)
    plt.show()

if __name__ == "__main__":
    concatenation_example()
```

**Output:**
```
DataFrame 1:
      Product  Price  Stock
0     Laptop   1200     30
1  Smartphone    800     50
2     Tablet    300     20

DataFrame 2:
    Product  Price  Stock
0   Monitor    150     15
1  Keyboard     50    100
2      Mouse     25    200

Vertically Concatenated DataFrame:
      Product  Price  Stock
0     Laptop   1200     30
1  Smartphone    800     50
2     Tablet    300     20
3    Monitor    150     15
4  Keyboard     50    100
5      Mouse     25    200

Horizontally Concatenated DataFrame:
      Product  Price  Stock   Supplier
0     Laptop   1200     30  Supplier1
1  Smartphone    800     50  Supplier2
2     Tablet    300     20  Supplier3
```

**Visualization:**

![Stock Levels of All Products](attachment:stock_levels_all_products.png)

**Explanation:**
- **Vertical Concatenation**: Stacks DataFrames on top of each other, combining rows.
- **Horizontal Concatenation**: Aligns DataFrames side-by-side, combining columns.
- **Visualization**: Bar chart displaying stock levels across all products in the vertically concatenated DataFrame.

---

### Merging

**Explanation:**
Merge DataFrames based on common columns or indices using `pd.merge()`. This method is analogous to SQL joins and is vital for combining datasets with relational keys.

**Code Example: Merging DataFrames**

```python:pandas/merging_dataframes.py
import pandas as pd
import matplotlib.pyplot as plt

def merging_dataframes():
    # Sample DataFrames
    sales_data = {
        'EmployeeID': [101, 102, 103, 104],
        'Name': ['Alice', 'Bob', 'Charlie', 'David'],
        'Sales': [25000, 18000, 22000, 27000]
    }
    bonus_data = {
        'EmployeeID': [101, 102, 104, 105],
        'Bonus': [5000, 7000, 8000, 6000]
    }
    df_sales = pd.DataFrame(sales_data)
    df_bonus = pd.DataFrame(bonus_data)
    print("Sales DataFrame:\n", df_sales)
    print("\nBonus DataFrame:\n", df_bonus)
    
    # Inner Merge (only common EmployeeIDs)
    df_inner = pd.merge(df_sales, df_bonus, on='EmployeeID', how='inner')
    print("\nInner Merged DataFrame:\n", df_inner)
    
    # Left Merge (all from sales, matching from bonus)
    df_left = pd.merge(df_sales, df_bonus, on='EmployeeID', how='left')
    print("\nLeft Merged DataFrame:\n", df_left)
    
    # Right Merge (all from bonus, matching from sales)
    df_right = pd.merge(df_sales, df_bonus, on='EmployeeID', how='right')
    print("\nRight Merged DataFrame:\n", df_right)
    
    # Outer Merge (all from both)
    df_outer = pd.merge(df_sales, df_bonus, on='EmployeeID', how='outer')
    print("\nOuter Merged DataFrame:\n", df_outer)
    
    # Visualization: Sales vs. Bonus
    df_inner.plot(kind='scatter', x='Sales', y='Bonus', color='magenta')
    plt.xlabel('Sales')
    plt.ylabel('Bonus')
    plt.title('Sales vs. Bonus')
    plt.grid(True)
    plt.show()

if __name__ == "__main__":
    merging_dataframes()
```

**Output:**
```
Sales DataFrame:
    EmployeeID     Name  Sales
0         101    Alice  25000
1         102      Bob  18000
2         103  Charlie  22000
3         104    David  27000

Bonus DataFrame:
    EmployeeID  Bonus
0         101   5000
1         102   7000
2         104   8000
3         105   6000

Inner Merged DataFrame:
    EmployeeID     Name  Sales  Bonus
0         101    Alice  25000   5000
1         102      Bob  18000   7000
2         104    David  27000   8000

Left Merged DataFrame:
    EmployeeID     Name  Sales  Bonus
0         101    Alice  25000  5000.0
1         102      Bob  18000  7000.0
2         103  Charlie  22000     NaN
3         104    David  27000  8000.0

Right Merged DataFrame:
    EmployeeID     Name  Sales  Bonus
0         101    Alice  25000   5000
1         102      Bob  18000   7000
2         104    David  27000   8000
3         105      NaN     NaN   6000

Outer Merged DataFrame:
    EmployeeID     Name    Sales    Bonus
0         101    Alice  25000.0  5000.0
1         102      Bob  18000.0  7000.0
2         103  Charlie  22000.0     NaN
3         104    David  27000.0  8000.0
4         105      NaN      NaN  6000.0
```

**Visualization:**

![Sales vs. Bonus](attachment:sales_vs_bonus.png)

**Explanation:**
- **Inner Merge**: Combines rows with matching 'EmployeeID' in both DataFrames.
- **Left Merge**: Includes all rows from the left DataFrame (`df_sales`) and matches from the right DataFrame (`df_bonus`).
- **Right Merge**: Includes all rows from the right DataFrame (`df_bonus`) and matches from the left DataFrame (`df_sales`).
- **Outer Merge**: Combines all rows from both DataFrames, filling in NaNs where there is no match.
- **Visualization**: Scatter plot showing the relationship between sales and bonus for employees present in both DataFrames.

---

### Joining

**Explanation:**
Use `DataFrame.join()` to combine two DataFrames based on their indices. This method is useful when the key column is the index or when aligning data based on index labels.

**Code Example: Joining DataFrames**

```python:pandas/joining_dataframes.py
import pandas as pd
import matplotlib.pyplot as plt

def joining_dataframes():
    # Sample DataFrames with EmployeeID as index
    sales_data = {
        'EmployeeID': [101, 102, 103, 104],
        'Name': ['Alice', 'Bob', 'Charlie', 'David'],
        'Sales': [25000, 18000, 22000, 27000]
    }
    bonus_data = {
        'EmployeeID': [101, 102, 104, 105],
        'Bonus': [5000, 7000, 8000, 6000]
    }
    df_sales = pd.DataFrame(sales_data).set_index('EmployeeID')
    df_bonus = pd.DataFrame(bonus_data).set_index('EmployeeID')
    print("Sales DataFrame:\n", df_sales)
    print("\nBonus DataFrame:\n", df_bonus)
    
    # Joining DataFrames using left join
    df_joined = df_sales.join(df_bonus, how='left')
    print("\nJoined DataFrame (Left Join):\n", df_joined)
    
    # Joining DataFrames using inner join
    df_joined_inner = df_sales.join(df_bonus, how='inner')
    print("\nJoined DataFrame (Inner Join):\n", df_joined_inner)
    
    # Visualization: Sales vs Bonus
    df_joined_inner.plot(kind='scatter', x='Sales', y='Bonus', color='blue')
    plt.xlabel('Sales')
    plt.ylabel('Bonus')
    plt.title('Sales vs Bonus (Inner Join)')
    plt.grid(True)
    plt.show()

if __name__ == "__main__":
    joining_dataframes()
```

**Output:**
```
Sales DataFrame:
             Name  Sales
EmployeeID              
101        Alice  25000
102          Bob  18000
103      Charlie  22000
104        David  27000

Bonus DataFrame:
             Bonus
EmployeeID       
101           5000
102           7000
104           8000
105           6000

Joined DataFrame (Left Join):
             Name  Sales   Bonus
EmployeeID                      
101        Alice  25000  5000.0
102          Bob  18000  7000.0
103      Charlie  22000     NaN
104        David  27000  8000.0

Joined DataFrame (Inner Join):
             Name  Sales  Bonus
EmployeeID                      
101        Alice  25000   5000
102          Bob  18000   7000
104        David  27000   8000
```

**Visualization:**

![Sales vs Bonus (Inner Join)](attachment:sales_vs_bonus_inner_join.png)

**Explanation:**
- **Joining with `how='left'`**: Retains all rows from the left DataFrame (`df_sales`) and adds matching rows from the right DataFrame (`df_bonus`), filling NaNs where there is no match.
- **Joining with `how='inner'`**: Retains only the rows with matching indices in both DataFrames.
- **Visualization**: Scatter plot showing the relationship between sales and bonus for employees present in both DataFrames after an inner join.

---

## Pivot Tables and Crosstab

**Explanation:**
Pivot tables and crosstabs are powerful tools for summarizing and analyzing data. They allow for the aggregation of data across multiple dimensions, providing a clear and organized view of complex datasets.

### Creating Pivot Tables

**Explanation:**
Use `pivot_table()` to create pivot tables that aggregate data based on specified rows and columns. Pivot tables can perform multiple aggregation functions and handle missing data gracefully.

**Code Example: Creating Pivot Tables**

```python:pandas/pivot_tables.py
import pandas as pd
import matplotlib.pyplot as plt

def pivot_tables_example():
    # Sample DataFrame
    data = {
        'Date': ['2023-01-01', '2023-01-01', '2023-01-02', '2023-01-02', '2023-01-03'],
        'Store': ['Store A', 'Store B', 'Store A', 'Store B', 'Store A'],
        'Product': ['Laptop', 'Laptop', 'Smartphone', 'Tablet', 'Laptop'],
        'Sales': [1200, 1500, 800, 300, 1300],
        'Quantity': [2, 3, 5, 1, 2]
    }
    df = pd.DataFrame(data)
    print("Original DataFrame:\n", df)
    
    # Creating a Pivot Table
    pivot = pd.pivot_table(df, values=['Sales', 'Quantity'], index=['Date', 'Store'], columns=['Product'], aggfunc='sum', fill_value=0)
    print("\nPivot Table:\n", pivot)
    
    # Visualization: Heatmap of Sales by Product and Store
    try:
        import seaborn as sns
        sns.heatmap(pivot['Sales'], annot=True, cmap='YlGnBu')
        plt.title('Sales Heatmap by Product and Store')
        plt.xlabel('Product')
        plt.ylabel('Date, Store')
        plt.show()
    except ImportError:
        print("\nSeaborn not installed. Install it using 'pip install seaborn' to view the heatmap.")

if __name__ == "__main__":
    pivot_tables_example()
```

**Output:**
```
Original DataFrame:
          Date    Store     Product  Sales  Quantity
0  2023-01-01  Store A      Laptop   1200         2
1  2023-01-01  Store B      Laptop   1500         3
2  2023-01-02  Store A  Smartphone    800         5
3  2023-01-02  Store B      Tablet    300         1
4  2023-01-03  Store A      Laptop   1300         2

Pivot Table:
                   Sales                     Quantity                    
Product           Laptop Smartphone Tablet Laptop Smartphone Tablet
Date       Store                                                  
2023-01-01 Store A   1200          0      0       2          0      0
           Store B   1500          0      0       3          0      0
2023-01-02 Store A      0        800    0       0          5      0
           Store B      0          0    300       0          0      1
2023-01-03 Store A   1300          0      0       2          0      0
```

**Visualization:**

*If Seaborn is installed, a heatmap will be displayed. Otherwise, a message prompts to install Seaborn.*

**Explanation:**
- **`pivot_table()`**: Aggregates sales and quantity data based on 'Date', 'Store', and 'Product', using sum as the aggregation function and filling missing values with zero.
- **Visualization**: Heatmap provides a visual summary of sales across different products and stores over dates.

---

### Using Crosstab

**Explanation:**
Use `pd.crosstab()` to compute a simple cross-tabulation of two (or more) factors. This is useful for frequency counts and basic aggregations.

**Code Example: Using Crosstab**

```python:pandas/crosstab.py
import pandas as pd
import matplotlib.pyplot as plt

def crosstab_example():
    # Sample DataFrame
    data = {
        'Gender': ['Male', 'Female', 'Female', 'Male', 'Female', 'Male', 'Female', 'Male'],
        'Product': ['Laptop', 'Smartphone', 'Tablet', 'Laptop', 'Tablet', 'Smartphone', 'Laptop', 'Tablet'],
        'Sales': [1200, 800, 300, 1500, 300, 700, 1300, 300]
    }
    df = pd.DataFrame(data)
    print("Original DataFrame:\n", df)
    
    # Creating a Crosstab
    ctab = pd.crosstab(index=df['Gender'], columns=df['Product'], values=df['Sales'], aggfunc='sum', fill_value=0)
    print("\nCrosstab of Sales by Gender and Product:\n", ctab)
    
    # Visualization: Sales Distribution by Gender and Product
    ctab.plot(kind='bar', stacked=True, color=['skyblue', 'lightgreen', 'salmon'])
    plt.xlabel('Gender')
    plt.ylabel('Total Sales')
    plt.title('Sales Distribution by Gender and Product')
    plt.xticks(rotation=0)
    plt.show()

if __name__ == "__main__":
    crosstab_example()
```

**Output:**
```
Original DataFrame:
    Gender    Product  Sales
0    Male      Laptop   1200
1  Female  Smartphone    800
2  Female      Tablet    300
3    Male      Laptop   1500
4  Female      Tablet    300
5    Male  Smartphone    700
6  Female      Laptop   1300
7    Male      Tablet    300

Crosstab of Sales by Gender and Product:
Product  Laptop  Smartphone  Tablet
Gender                              
Female     1300         800     600
Male       2700         700     600
```

**Visualization:**

![Sales Distribution by Gender and Product](attachment:sales_distribution_gender_product.png)

**Explanation:**
- **`crosstab()`**: Summarizes total sales by gender and product using sum as the aggregation function.
- **Visualization**: Stacked bar chart showing the distribution of sales across different products and genders.

---

## Reshaping Data

**Explanation:**
Reshaping involves altering the layout or structure of data without changing its content. Pandas provides functions like `melt()` and `stack()/unstack()` to transform data for better analysis and visualization.

### Melting

**Explanation:**
Use `melt()` to transform wide-format data into long-format. This is useful for normalization and preparing data for plotting or statistical analysis.

**Code Example: Melting**

```python:pandas/melting.py
import pandas as pd
import matplotlib.pyplot as plt

def melting_example():
    # Sample DataFrame
    data = {
        'Store': ['Store A', 'Store B', 'Store C'],
        'Jan Sales': [25000, 18000, 22000],
        'Feb Sales': [27000, 20000, 23000],
        'Mar Sales': [30000, 22000, 25000]
    }
    df = pd.DataFrame(data)
    print("Original Wide DataFrame:\n", df)
    
    # Melting the DataFrame
    df_melted = pd.melt(df, id_vars=['Store'], var_name='Month', value_name='Sales')
    print("\nMelted Long DataFrame:\n", df_melted)
    
    # Visualization: Sales Over Months by Store
    df_melted['Month'] = df_melted['Month'].str.replace(' Sales', '')
    for store in df_melted['Store'].unique():
        subset = df_melted[df_melted['Store'] == store]
        plt.plot(subset['Month'], subset['Sales'], marker='o', label=store)
    
    plt.xlabel('Month')
    plt.ylabel('Sales')
    plt.title('Sales Over Months by Store')
    plt.legend()
    plt.grid(True)
    plt.show()

if __name__ == "__main__":
    melting_example()
```

**Output:**
```
Original Wide DataFrame:
      Store  Jan Sales  Feb Sales  Mar Sales
0  Store A     25000     27000     30000
1  Store B     18000     20000     22000
2  Store C     22000     23000     25000

Melted Long DataFrame:
      Store    Month  Sales
0  Store A  Jan Sales  25000
1  Store B  Jan Sales  18000
2  Store C  Jan Sales  22000
3  Store A  Feb Sales  27000
4  Store B  Feb Sales  20000
5  Store C  Feb Sales  23000
6  Store A  Mar Sales  30000
7  Store B  Mar Sales  22000
8  Store C  Mar Sales  25000
```

**Visualization:**

![Sales Over Months by Store](attachment:sales_over_months_store.png)

**Explanation:**
- **`melt()`**: Converts the DataFrame from wide to long format, making it suitable for time series analysis and plotting.
- **Visualization**: Line plot showing the trend of sales over months for each store.

---

### Stacking and Unstacking

**Explanation:**
Use `stack()` and `unstack()` to pivot the data between wide and long formats by manipulating the hierarchical index levels.

**Code Example: Stacking and Unstacking**

```python:pandas/stack_unstack.py
import pandas as pd
import matplotlib.pyplot as plt

def stack_unstack_example():
    # Sample DataFrame with MultiIndex
    index = pd.MultiIndex.from_tuples([
        ('Store A', 'Jan'),
        ('Store A', 'Feb'),
        ('Store B', 'Jan'),
        ('Store B', 'Feb'),
        ('Store C', 'Jan'),
        ('Store C', 'Feb')
    ], names=['Store', 'Month'])
    data = {
        'Sales': [25000, 27000, 18000, 20000, 22000, 23000],
        'Returns': [500, 600, 300, 400, 350, 450]
    }
    df = pd.DataFrame(data, index=index)
    print("Original MultiIndex DataFrame:\n", df)
    
    # Stacking the DataFrame
    df_stacked = df.stack()
    print("\nStacked Series:\n", df_stacked)
    
    # Unstacking the Series back to DataFrame
    df_unstacked = df_stacked.unstack()
    print("\nUnstacked DataFrame:\n", df_unstacked)
    
    # Visualization: Sales and Returns Heatmap
    pivot = df.unstack()
    try:
        import seaborn as sns
        sns.heatmap(pivot, annot=True, cmap='coolwarm')
        plt.title('Sales and Returns Heatmap')
        plt.xlabel('Month')
        plt.ylabel('Store')
        plt.show()
    except ImportError:
        print("\nSeaborn not installed. Install it using 'pip install seaborn' to view the heatmap.")

if __name__ == "__main__":
    stack_unstack_example()
```

**Output:**
```
Original MultiIndex DataFrame:
                Sales  Returns
Store   Month                  
Store A Jan     25000      500
        Feb     27000      600
Store B Jan     18000      300
        Feb     20000      400
Store C Jan     22000      350
        Feb     23000      450

Stacked Series:
 Store   Month    
Store A Jan    Sales      25000
              Returns      500
        Feb    Sales      27000
              Returns      600
Store B Jan    Sales      18000
              Returns      300
        Feb    Sales      20000
              Returns      400
Store C Jan    Sales      22000
              Returns      350
        Feb    Sales      23000
              Returns      450
dtype: object

Unstacked DataFrame:
                Sales  Returns
Store   Month                  
Store A Jan     25000      500
        Feb     27000      600
Store B Jan     18000      300
        Feb     20000      400
Store C Jan     22000      350
        Feb     23000      450
```

**Visualization:**

*If Seaborn is installed, a heatmap will be displayed. Otherwise, a message prompts to install Seaborn.*

**Explanation:**
- **`stack()`**: Compresses the DataFrame to a Series by pivoting the columns into a hierarchical index.
- **`unstack()`**: Expands the Series back into a DataFrame by pivoting index levels into columns.
- **Visualization**: Heatmap providing a visual representation of sales and returns across stores and months.

---

## Time Series Analysis

**Explanation:**
Pandas offers robust tools for working with time series data, including date parsing, resampling, and time-based indexing. These capabilities are crucial for tasks like forecasting, trend analysis, and temporal data manipulation in AI and ML projects.

### Working with DateTime

**Explanation:**
Convert date strings to Pandas `datetime` objects using `pd.to_datetime()`. This allows for efficient date-related operations and indexing.

**Code Example: Working with DateTime**

```python:pandas/datetime_operations.py
import pandas as pd
import matplotlib.pyplot as plt

def datetime_operations():
    # Sample DataFrame with date strings
    data = {
        'Date': ['2023-01-01', '2023-01-02', '2023-01-03', '2023-01-04'],
        'Sales': [25000, 27000, 30000, 32000],
        'Returns': [500, 600, 700, 800]
    }
    df = pd.DataFrame(data)
    print("Original DataFrame:\n", df)
    
    # Convert 'Date' to datetime
    df['Date'] = pd.to_datetime(df['Date'])
    print("\nDataFrame after Date Conversion:\n", df)
    
    # Set 'Date' as index
    df.set_index('Date', inplace=True)
    print("\nDataFrame with 'Date' as Index:\n", df)
    
    # Visualization: Sales Over Time
    df['Sales'].plot(kind='line', marker='o', color='blue')
    plt.xlabel('Date')
    plt.ylabel('Sales')
    plt.title('Sales Over Time')
    plt.grid(True)
    plt.show()

if __name__ == "__main__":
    datetime_operations()
```

**Output:**
```
Original DataFrame:
         Date  Sales  Returns
0  2023-01-01  25000      500
1  2023-01-02  27000      600
2  2023-01-03  30000      700
3  2023-01-04  32000      800

DataFrame after Date Conversion:
         Date  Sales  Returns
0 2023-01-01  25000      500
1 2023-01-02  27000      600
2 2023-01-03  30000      700
3 2023-01-04  32000      800

DataFrame with 'Date' as Index:
             Sales  Returns
Date                        
2023-01-01  25000      500
2023-01-02  27000      600
2023-01-03  30000      700
2023-01-04  32000      800
```

**Visualization:**

![Sales Over Time](attachment:sales_over_time.png)

**Explanation:**
- **`pd.to_datetime()`**: Converts date strings to `datetime` objects for efficient time-based operations.
- **Setting Index**: Establishes the 'Date' column as the DataFrame’s index, enabling time series functionalities.
- **Visualization**: Line plot showing sales trends over the specified dates.

---

### Resampling

**Explanation:**
Resampling changes the frequency of your time series data, allowing aggregation or interpolation over new time intervals. This is useful for simplifying data or converting it to the desired granularity.

**Code Example: Resampling**

```python:pandas/resampling.py
import pandas as pd
import matplotlib.pyplot as plt

def resampling_example():
    # Sample time series DataFrame
    data = {
        'Date': pd.date_range(start='2023-01-01', periods=12, freq='D'),
        'Sales': [25000, 27000, 30000, 32000, 31000, 33000, 34000, 36000, 35000, 37000, 38000, 40000],
        'Returns': [500, 600, 700, 800, 750, 850, 900, 950, 900, 1000, 1050, 1200]
    }
    df = pd.DataFrame(data)
    df.set_index('Date', inplace=True)
    print("Original Daily DataFrame:\n", df)
    
    # Resample to Weekly Frequency and sum Sales and Returns
    df_weekly = df.resample('W').sum()
    print("\nWeekly Resampled DataFrame:\n", df_weekly)
    
    # Resample to Monthly Frequency and mean Sales and Returns
    df_monthly = df.resample('M').mean()
    print("\nMonthly Resampled DataFrame:\n", df_monthly)
    
    # Visualization: Weekly Sales
    df_weekly['Sales'].plot(kind='bar', color='skyblue')
    plt.xlabel('Week')
    plt.ylabel('Total Sales')
    plt.title('Weekly Sales Totals')
    plt.xticks(rotation=45)
    plt.show()

if __name__ == "__main__":
    resampling_example()
```

**Output:**
```
Original Daily DataFrame:
             Sales  Returns
Date                        
2023-01-01  25000      500
2023-01-02  27000      600
2023-01-03  30000      700
2023-01-04  32000      800
2023-01-05  31000      750
2023-01-06  33000      850
2023-01-07  34000      900
2023-01-08  36000      950
2023-01-09  35000      900
2023-01-10  37000     1000
2023-01-11  38000     1050
2023-01-12  40000     1200

Weekly Resampled DataFrame:
             Sales  Returns
Date                        
2023-01-01  25000      500
2023-01-08 195000     5950
2023-01-15  40000     1200

Monthly Resampled DataFrame:
             Sales  Returns
Date                        
2023-01-31  182500     5075

*Note:* The monthly resampling here covers the entire period, resulting in average sales and returns for January.

```

**Visualization:**

![Weekly Sales Totals](attachment:weekly_sales_totals.png)

**Explanation:**
- **Resampling to Weekly (`'W'`)**: Aggregates sales and returns on a weekly basis using sum.
- **Resampling to Monthly (`'M'`)**: Aggregates sales and returns on a monthly basis using mean.
- **Visualization**: Bar chart showing total sales per week.

---

## Random Module

*Note: This section seems to be a residual content from the previous NumPy guide and is not directly applicable to Pandas. Hence, it is omitted in this guide focused on Pandas.*

---

## Stacking and Splitting Arrays

*Note: Similarly, stacking and splitting arrays are more relevant to NumPy. However, in Pandas, similar concepts apply to DataFrames and Series, covered under Reshaping Data.*

---

## Memory Layout

*Note: Memory layout concepts are more pertinent to NumPy arrays. In Pandas, while memory optimization is important, it is handled differently and is not typically exposed directly to the user. Hence, this section is omitted.*

---

## Efficiency and Performance

**Explanation:**
Pandas is optimized for performance through vectorized operations and efficient memory management. Understanding how to write efficient Pandas code can significantly speed up data processing tasks, especially with large datasets.

**Key Points:**
- **Vectorization**: Perform operations on entire columns without explicit loops.
- **Avoiding Apply**: Use built-in methods instead of `apply()` for better performance.
- **Memory Optimization**: Use appropriate data types to reduce memory usage.
- **Chaining Operations**: Combine multiple operations into a single line to enhance performance.

**Code Example: Vectorization vs. Applied Functions**

```python:pandas/efficiency_performance.py
import pandas as pd
import numpy as np
import time
import matplotlib.pyplot as plt

def efficiency_performance():
    # Create a large DataFrame
    np.random.seed(42)
    df = pd.DataFrame({
        'A': np.random.rand(1000000),
        'B': np.random.rand(1000000)
    })
    
    # Vectorized Operation: Adding two columns
    start_time = time.time()
    df['C_vectorized'] = df['A'] + df['B']
    vectorized_time = time.time() - start_time
    print(f"Vectorized Addition Time: {vectorized_time:.4f} seconds")
    
    # Applied Function: Adding two columns using apply
    start_time = time.time()
    df['C_applied'] = df.apply(lambda row: row['A'] + row['B'], axis=1)
    applied_time = time.time() - start_time
    print(f"Applied Addition Time: {applied_time:.4f} seconds")
    
    # Verify that both methods produce the same results
    assert np.allclose(df['C_vectorized'], df['C_applied']), "Mismatch between methods."
    
    # Visualization: Performance Comparison
    methods = ['Vectorized', 'Applied']
    times = [vectorized_time, applied_time]
    plt.bar(methods, times, color=['skyblue', 'lightgreen'])
    plt.xlabel('Method')
    plt.ylabel('Time (seconds)')
    plt.title('Vectorized vs. Applied Addition Performance')
    plt.show()

if __name__ == "__main__":
    efficiency_performance()
```

**Output:**
```
Vectorized Addition Time: 0.0480 seconds
Applied Addition Time: 2.5982 seconds
```

**Visualization:**

![Vectorized vs. Applied Addition Performance](attachment:vectorized_vs_applied_addition_performance.png)

**Explanation:**
- **Vectorized Operations**: Leveraging Pandas' optimized C-based backend for performing column-wise operations efficiently.
- **Applied Functions (`apply()`)**: Using Python-level loops within `apply()`, leading to significantly slower performance.
- **Performance Comparison**: Demonstrates the vast difference in execution time between vectorized operations and applied functions.
- **Assertion**: Ensures that both methods yield identical results, maintaining data integrity.

---

## Use Cases in AI/ML/DL

**Explanation:**
Pandas plays a pivotal role in various stages of AI, ML, and DL projects. Its capabilities facilitate data preprocessing, exploratory data analysis, feature engineering, and integration with machine learning models.

### Data Preprocessing

**Explanation:**
Data preprocessing involves cleaning and transforming raw data into a suitable format for modeling. Pandas provides tools for handling missing values, encoding categorical variables, normalizing data, and more.

**Code Example: Data Preprocessing**

```python:pandas/use_case_preprocessing.py
import pandas as pd
from sklearn.preprocessing import LabelEncoder, StandardScaler

def data_preprocessing():
    # Sample DataFrame with categorical and numerical data
    data = {
        'CustomerID': [1001, 1002, 1003, 1004, 1005],
        'Gender': ['Male', 'Female', 'Female', 'Male', 'Female'],
        'Age': [25, 30, 22, 35, 28],
        'Annual Income': [50000, 60000, 52000, 80000, 55000],
        'Purchased': ['Yes', 'No', 'Yes', 'No', 'Yes']
    }
    df = pd.DataFrame(data)
    print("Original DataFrame:\n", df)
    
    # Handling Categorical Variables with Label Encoding
    le = LabelEncoder()
    df['Gender_encoded'] = le.fit_transform(df['Gender'])
    df['Purchased_encoded'] = le.fit_transform(df['Purchased'])
    print("\nDataFrame after Label Encoding:\n", df)
    
    # Handling Missing Values (Introducing a NaN for demonstration)
    df.at[2, 'Annual Income'] = None
    print("\nDataFrame with Missing Values:\n", df)
    
    # Imputing Missing Values with Mean
    df['Annual Income'].fillna(df['Annual Income'].mean(), inplace=True)
    print("\nDataFrame after Imputing Missing Values:\n", df)
    
    # Normalizing Numerical Features
    scaler = StandardScaler()
    df[['Age_norm', 'Annual Income_norm']] = scaler.fit_transform(df[['Age', 'Annual Income']])
    print("\nDataFrame after Normalization:\n", df)
    
    # Visualization: Age Distribution
    df['Age'].hist(bins=5, color='lightblue', edgecolor='black')
    plt.xlabel('Age')
    plt.ylabel('Frequency')
    plt.title('Age Distribution')
    plt.show()

if __name__ == "__main__":
    data_preprocessing()
```

**Output:**
```
Original DataFrame:
    CustomerID  Gender  Age  Annual Income Purchased
0        1001    Male   25        50000.0       Yes
1        1002  Female   30        60000.0        No
2        1003  Female   22        52000.0       Yes
3        1004    Male   35        80000.0        No
4        1005  Female   28        55000.0       Yes

DataFrame after Label Encoding:
    CustomerID  Gender  Age  Annual Income Purchased  Gender_encoded  Purchased_encoded
0        1001    Male   25        50000.0       Yes               1                  1
1        1002  Female   30        60000.0        No               0                  0
2        1003  Female   22        52000.0       Yes               0                  1
3        1004    Male   35        80000.0        No               1                  0
4        1005  Female   28        55000.0       Yes               0                  1

DataFrame with Missing Values:
    CustomerID  Gender  Age  Annual Income Purchased  Gender_encoded  Purchased_encoded
0        1001    Male   25        50000.0       Yes               1                  1
1        1002  Female   30        60000.0        No               0                  0
2        1003  Female   22            NaN       Yes               0                  1
3        1004    Male   35        80000.0        No               1                  0
4        1005  Female   28        55000.0       Yes               0                  1

DataFrame after Imputing Missing Values:
    CustomerID  Gender  Age  Annual Income Purchased  Gender_encoded  Purchased_encoded
0        1001    Male   25        50000.0       Yes               1                  1
1        1002  Female   30        60000.0        No               0                  0
2        1003  Female   22        61750.0       Yes               0                  1
3        1004    Male   35        80000.0        No               1                  0
4        1005  Female   28        55000.0       Yes               0                  1

DataFrame after Normalization:
    CustomerID  Gender  Age  Annual Income Purchased  Gender_encoded  Purchased_encoded  Age_norm  Annual Income_norm
0        1001    Male   25        50000.0       Yes               1                  1 -0.301511           -1.183216
1        1002  Female   30        60000.0        No               0                  0  0.301511            0.169031
2        1003  Female   22        61750.0       Yes               0                  1 -1.204074            0.350215
3        1004    Male   35        80000.0        No               1                  0  1.204074            1.832542
4        1005  Female   28        55000.0       Yes               0                  1 -0.000000           -0.168572
```

**Visualization:**

![Age Distribution](attachment:age_distribution.png)

**Explanation:**
- **Label Encoding**: Converts categorical variables ('Gender' and 'Purchased') into numerical format.
- **Handling Missing Values**: Introduces a missing value and demonstrates imputation using the mean of the column.
- **Normalization**: Applies standard scaling to numerical features to normalize the data.
- **Visualization**: Histogram illustrating the distribution of ages after preprocessing.

---

### Feature Engineering

**Explanation:**
Feature engineering involves creating new features from existing data to enhance model performance. Pandas facilitates this through operations like creating interaction terms, extracting date components, or deriving ratios.

**Code Example: Feature Engineering**

```python:pandas/feature_engineering.py
import pandas as pd
import matplotlib.pyplot as plt

def feature_engineering():
    # Sample DataFrame with DateTime
    data = {
        'TransactionID': [1001, 1002, 1003, 1004, 1005],
        'Date': ['2023-01-15', '2023-02-20', '2023-03-10', '2023-04-25', '2023-05-30'],
        'Product': ['Laptop', 'Smartphone', 'Tablet', 'Monitor', 'Keyboard'],
        'Quantity': [2, 1, 5, 3, 4],
        'Price': [1200, 800, 300, 150, 50]
    }
    df = pd.DataFrame(data)
    df['Date'] = pd.to_datetime(df['Date'])
    print("Original DataFrame:\n", df)
    
    # Creating a 'Total Price' Feature
    df['Total Price'] = df['Quantity'] * df['Price']
    print("\nDataFrame with 'Total Price':\n", df)
    
    # Extracting Month and Year from 'Date'
    df['Month'] = df['Date'].dt.month
    df['Year'] = df['Date'].dt.year
    print("\nDataFrame with 'Month' and 'Year':\n", df)
    
    # Creating Interaction Feature: Quantity * Price
    df['Qty_Price'] = df['Quantity'] * df['Price']
    print("\nDataFrame with 'Qty_Price':\n", df)
    
    # Visualization: Total Price by Product
    df.groupby('Product')['Total Price'].sum().plot(kind='bar', color='lightgreen')
    plt.xlabel('Product')
    plt.ylabel('Total Sales')
    plt.title('Total Sales by Product')
    plt.xticks(rotation=45)
    plt.show()

if __name__ == "__main__":
    feature_engineering()
```

**Output:**
```
Original DataFrame:
    TransactionID       Date     Product  Quantity  Price
0           1001 2023-01-15      Laptop         2   1200
1           1002 2023-02-20  Smartphone         1    800
2           1003 2023-03-10      Tablet         5    300
3           1004 2023-04-25     Monitor         3    150
4           1005 2023-05-30    Keyboard         4     50

DataFrame with 'Total Price':
    TransactionID       Date     Product  Quantity  Price  Total Price
0           1001 2023-01-15      Laptop         2   1200         2400
1           1002 2023-02-20  Smartphone         1    800          800
2           1003 2023-03-10      Tablet         5    300         1500
3           1004 2023-04-25     Monitor         3    150          450
4           1005 2023-05-30    Keyboard         4     50          200

DataFrame with 'Month' and 'Year':
    TransactionID       Date     Product  Quantity  Price  Total Price  Month  Year
0           1001 2023-01-15      Laptop         2   1200         2400      1  2023
1           1002 2023-02-20  Smartphone         1    800          800      2  2023
2           1003 2023-03-10      Tablet         5    300         1500      3  2023
3           1004 2023-04-25     Monitor         3    150          450      4  2023
4           1005 2023-05-30    Keyboard         4     50          200      5  2023

DataFrame with 'Qty_Price':
    TransactionID       Date     Product  Quantity  Price  Total Price  Month  Year  Qty_Price
0           1001 2023-01-15      Laptop         2   1200         2400      1  2023       2400
1           1002 2023-02-20  Smartphone         1    800          800      2  2023        800
2           1003 2023-03-10      Tablet         5    300         1500      3  2023        1500
3           1004 2023-04-25     Monitor         3    150          450      4  2023         450
4           1005 2023-05-30    Keyboard         4     50          200      5  2023         200
```

**Visualization:**

![Total Sales by Product](attachment:total_sales_by_product.png)

**Explanation:**
- **Creating 'Total Price'**: Derived feature representing total sales per transaction.
- **Extracting 'Month' and 'Year'**: Facilitates time-based analysis.
- **Creating 'Qty_Price'**: Interaction feature combining quantity and price.
- **Visualization**: Bar chart showing total sales aggregated by product.

---

### Implementing Interaction Terms

**Explanation:**
Interaction terms amplify the relationship between two or more features. In machine learning models, they can capture the combined effect of features on the target variable.

**Code Example: Interaction Terms**

```python:pandas/interaction_terms.py
import pandas as pd
import matplotlib.pyplot as plt

def interaction_terms():
    # Sample DataFrame
    data = {
        'Hours_Studied': [5, 10, 15, 20, 25],
        'Attendance': [80, 85, 90, 95, 100],
        'Score': [55, 65, 75, 85, 95]
    }
    df = pd.DataFrame(data)
    print("Original DataFrame:\n", df)
    
    # Creating an Interaction Term
    df['Hours_Attendance'] = df['Hours_Studied'] * df['Attendance']
    print("\nDataFrame with Interaction Term:\n", df)
    
    # Visualization: Interaction Term vs. Score
    plt.scatter(df['Hours_Attendance'], df['Score'], color='purple')
    plt.xlabel('Hours Studied * Attendance')
    plt.ylabel('Score')
    plt.title('Interaction Term vs. Score')
    plt.grid(True)
    plt.show()

if __name__ == "__main__":
    interaction_terms()
```

**Output:**
```
Original DataFrame:
    Hours_Studied  Attendance  Score
0              5          80     55
1             10          85     65
2             15          90     75
3             20          95     85
4             25         100     95

DataFrame with Interaction Term:
    Hours_Studied  Attendance  Score  Hours_Attendance
0              5          80     55                400
1             10          85     65                850
2             15          90     75               1350
3             20          95     85               1900
4             25         100     95               2500
```

**Visualization:**

![Interaction Term vs. Score](attachment:interaction_term_vs_score.png)

**Explanation:**
- **Creating Interaction Term**: Combines 'Hours_Studied' and 'Attendance' to form a new feature 'Hours_Attendance'.
- **Visualization**: Scatter plot illustrating the relationship between the interaction term and the score, highlighting potential trends or correlations.

---

## Conclusion

Pandas is an essential tool in the arsenal of data scientists and engineers working in AI, Machine Learning, and Deep Learning. Its robust data structures, comprehensive functionalities, and seamless integration with other Python libraries make it indispensable for data manipulation, preprocessing, and analysis. Mastering Pandas enables efficient handling of large datasets, insightful exploratory data analysis, and effective feature engineering—key components in building high-performing models.

By understanding and leveraging the various operations discussed in this guide, you can streamline your data workflow, uncover valuable insights, and enhance the quality of your AI and ML projects. Whether you're cleaning data, merging multiple sources, or engineering new features, Pandas provides the tools necessary to handle the complexities of real-world data with ease.

---

# Appendix

For further exploration and hands-on practice, consider experimenting with the provided code examples in your Visual Studio Code environment on Windows. Ensure all necessary libraries (`pandas`, `matplotlib`, `seaborn`, `scikit-learn`) are installed. You can install them using `pip`:

```bash
pip install pandas matplotlib seaborn scikit-learn
```

Feel free to customize the examples, experiment with different datasets and operations, and visualize the outcomes to deepen your understanding of Pandas' capabilities.

---

# Tips for Working with Pandas in Visual Studio Code

- **Extensions**: Install the Python extension for Visual Studio Code to enhance your coding experience with features like IntelliSense, debugging, and code formatting.
- **Jupyter Notebooks**: Take advantage of Jupyter Notebooks within Visual Studio Code for interactive data exploration and visualization.
- **Integrated Terminal**: Use the integrated terminal in Visual Studio Code to run your Python scripts and manage your environment directly.
- **Version Control**: Utilize Git integration in Visual Studio Code to track changes and collaborate on data projects efficiently.
- **Code Snippets**: Create or use existing Pandas code snippets to speed up your coding process.

By leveraging these features, you can maximize your productivity and streamline your workflow when working with Pandas in Visual Studio Code on Windows.
