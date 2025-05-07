
Below is an extensive 30‑day roadmap for learning statistics with a strong focus on machine learning and AI. This plan is designed so that every day you have about 10 hours of study combining theory, interactive visualizations, code examples with real sample data, and practical exercises. Each day’s focus is broken down into clearly defined topics, with explanations on the purpose of each technique, when to use it (and when not to), and suggestions for alternative approaches.

> **Note:** Before getting started, make sure you have a working Python environment on your Windows machine with VSCode and Jupyter Notebook installed. Install the following libraries if they’re not already installed:
> - **Pandas**
> - **NumPy**
> - **Matplotlib & Seaborn**
> - **SciPy**
> - **scikit-learn**
> - **statsmodels**
> - (Optionally) **PyMC3** or another Bayesian inference library

---

## 30‑Day Roadmap for Statistics in Machine Learning & AI

### **Pre-Roadmap Setup (Day 0)**
- **Goals:**
  - Install Anaconda or set up a Python virtual environment.
  - Install VSCode and configure the Python extension.
  - Confirm installation of the key libraries listed above.
- **Outcome:**  
  You’ll be ready to code and visualize on your Windows machine.

---

### **Day 1: Introduction & Descriptive Statistics**

- **Topics:**
  - Overview of statistics in ML & AI.
  - Types of data (categorical, numerical, continuous, discrete).
  - Descriptive statistics: mean, median, mode, variance, standard deviation.
- **Learning Activities:**
  - Read introductory material on data types and summary statistics.
  - Use a Jupyter Notebook to compute and visualize basic descriptive stats on a sample dataset (for example, a simple CSV file containing demographic data).
- **Example:**
  - Create a Pandas DataFrame.
  - Plot histograms and box plots to understand data distribution.
- **Sample Code:**

  ```python:src/descriptive_stats.py
  import pandas as pd
  import numpy as np
  import matplotlib.pyplot as plt

  # Create a sample dataset using NumPy and Pandas
  np.random.seed(42)
  data = {
      'Age': np.random.randint(18, 70, size=100),
      'Salary': np.random.normal(50000, 15000, size=100)
  }
  df = pd.DataFrame(data)

  # Calculate descriptive statistics
  print("Descriptive Statistics:\n", df.describe())

  # Visualization: Histogram for Age and Salary
  plt.figure(figsize=(12, 5))

  plt.subplot(1, 2, 1)
  plt.hist(df['Age'], bins=10, color='skyblue', edgecolor='black')
  plt.title('Distribution of Age')
  plt.xlabel('Age')
  plt.ylabel('Frequency')

  plt.subplot(1, 2, 2)
  plt.hist(df['Salary'], bins=10, color='lightgreen', edgecolor='black')
  plt.title('Distribution of Salary')
  plt.xlabel('Salary')
  plt.ylabel('Frequency')

  plt.tight_layout()
  plt.show()
  ```

  *This code demonstrates how to generate summary statistics and visualize the distribution of data, which is fundamental for any further analysis.*

---

### **Day 2: Exploratory Data Analysis (EDA) and Visualization**

- **Topics:**
  - Introduction to EDA.
  - Data cleaning, dealing with missing data, and outlier detection.
  - Using visualization libraries (Matplotlib, Seaborn) to explore relationships (scatter plots, pair plots, correlation matrices).
- **Learning Activities:**
  - Work on a real-world dataset (e.g., Titanic dataset or your own CSV file).
  - Experiment with Seaborn’s pairplot and heatmap functions.
- **Key Concepts:**
  - How to decide which plots best represent your data.
  - Understanding the limitations of visualizations (misleading representations, overplotting).
- **Example Discussion:**
  - When is a box plot not appropriate (e.g., small datasets may not show enough distribution detail)?  
  - Better alternatives might be a violin plot for revealing more detail in the data distribution.

---

### **Day 3: Basics of Probability**

- **Topics:**
  - Fundamental definitions: probability, sample space, events.
  - Independent and dependent events.
  - Conditional probability and Bayesian basics.
- **Learning Activities:**
  - Solve probability puzzles with code simulations.
  - Visualize probability distributions for simple events using Python.
- **Example:**
  - Simulate coin tosses or dice rolls and compare theoretical vs. simulated probabilities.

---

### **Day 4: Discrete Probability Distributions**

- **Topics:**
  - Bernoulli, Binomial, Geometric, and Poisson distributions.
  - Properties and expectation/variance calculations.
- **Learning Activities:**
  - Use SciPy to plot the probability mass functions (PMFs) of these distributions.
  - Work through real-life examples (e.g., modeling the number of successes in trials).
- **Discussion:**
  - When to use a Poisson distribution (e.g., count of events over time) versus a binomial (fixed number of trials).

---

### **Day 5: Continuous Probability Distributions**

- **Topics:**
  - Uniform, Normal (Gaussian), Exponential, and Gamma distributions.
  - Density functions and cumulative distribution functions.
- **Learning Activities:**
  - Plot and compare different continuous distributions.
  - Simulate real-life processes such as measuring physical quantities (e.g., heights following a normal distribution).
- **Discussion:**
  - Discuss cases where the normal distribution assumption might fail and alternatives (e.g., heavy-tailed distributions).

---

### **Day 6: Sampling Methods & the Central Limit Theorem (CLT)**

- **Topics:**
  - Types of sampling: simple random sampling, stratified sampling.
  - The importance and implications of the CLT.
- **Learning Activities:**
  - Simulate sampling from various distributions.
  - Demonstrate the convergence of the sample mean distribution to a normal distribution using code.
- **Example:**
  - Show how increasing sample sizes reduce the estimation error.

---

### **Day 7: Estimation and Confidence Intervals**

- **Topics:**
  - Point estimation vs. interval estimation.
  - Constructing confidence intervals for means and proportions.
- **Learning Activities:**
  - Calculate confidence intervals using both analytical methods and bootstrapping techniques.
  - Visualize the confidence intervals on plots.
- **Discussion:**
  - When a confidence interval is more informative than a single point estimate.

---

### **Day 8: Fundamentals of Hypothesis Testing**

- **Topics:**
  - Null and alternative hypotheses.
  - Type I and Type II errors, p-values, significance levels.
- **Learning Activities:**
  - Conduct a one-sample t-test using SciPy.
  - Visualize the distribution of test statistics and critical regions.
- **Example Discussion:**
  - Why using a p-value alone can be misleading and the importance of effect size.

---

### **Day 9: Advanced Hypothesis Testing and ANOVA**

- **Topics:**
  - Two-sample tests (t-tests, Mann–Whitney U test).
  - Analysis of Variance (ANOVA) for comparing multiple means.
- **Learning Activities:**
  - Work on examples comparing groups (e.g., comparing the means of different populations).
  - Visualize the group differences using box plots.
- **Discussion:**
  - When to use non-parametric tests vs. parametric tests.

---

### **Day 10: Correlation & Covariance**

- **Topics:**
  - Understanding covariance and correlation coefficients.
  - Scatter plots and linear relationships.
- **Learning Activities:**
  - Calculate and visualize the Pearson and Spearman correlation coefficients.
  - Discuss spurious correlations and causation warnings.
- **Example:**
  - Use a simulated dataset to illustrate how outliers can affect correlation.

---

### **Day 11: Simple Linear Regression**

- **Topics:**
  - Linear regression theory and model assumptions.
  - Least Squares estimation.
- **Learning Activities:**
  - Fit a linear regression model using scikit-learn and statsmodels.
  - Visualize the regression line and residuals.
- **Discussion:**
  - When linear regression is appropriate and its limitations (e.g., assumption of linearity).

---

### **Day 12: Multiple Linear Regression**

- **Topics:**
  - Extending simple regression to multiple features.
  - Understanding multicollinearity and model diagnostics.
- **Learning Activities:**
  - Build and evaluate multiple linear regression models on multidimensional datasets.
  - Perform residual analysis to check assumptions.
- **Example:**
  - Use sample marketing data to predict sales based on multiple factors.

---

### **Day 13: Logistic Regression**

- **Topics:**
  - Binary classification using logistic regression.
  - The concept of odds and log odds.
- **Learning Activities:**
  - Implement logistic regression for a binary classification problem.
  - Evaluate using confusion matrices and ROC curves.
- **Discussion:**
  - Differences between logistic and linear regression in terms of output interpretation.

---

### **Day 14: Non-Parametric Tests**

- **Topics:**
  - When parametric test assumptions are violated.
  - Tests such as the Wilcoxon signed-rank test and Kruskal-Wallis test.
- **Learning Activities:**
  - Compare parametric vs. non-parametric test outcomes on sample data.
  - Discuss scenarios where non-parametric methods are preferred.
  
---

### **Day 15: In-Depth ANOVA and Post Hoc Tests**

- **Topics:**
  - One-way and two-way ANOVA.
  - Post hoc tests such as Tukey’s range test.
- **Learning Activities:**
  - Analyze datasets with multiple groups.
  - Visualize mean differences and perform post hoc analysis.
- **Discussion:**
  - Limitations of ANOVA when group variances differ dramatically; consider robust methods.

---

### **Day 16: Model Evaluation Metrics**

- **Topics:**
  - Regression: RMSE, MAE, R-squared, adjusted R-squared.
  - Classification: Accuracy, precision, recall, F1-score, ROC-AUC.
  - The role of cross-validation in model evaluation.
- **Learning Activities:**
  - Calculate various metrics in code using scikit-learn.
  - Visualize ROC curves and error distributions.
- **Discussion:**
  - When a single metric is misleading and the benefit of combined indicators.

---

### **Day 17: Introduction to Bayesian Statistics**

- **Topics:**
  - Differences between frequentist and Bayesian approaches.
  - Key concepts: priors, likelihood, posterior.
- **Learning Activities:**
  - Introduce simple Bayesian updating with examples.
  - Compare Bayesian credible intervals with frequentist confidence intervals.
- **Discussion:**
  - Use cases where Bayesian methods provide a clearer picture of uncertainty.

---

### **Day 18: Bayesian Inference & MCMC**

- **Topics:**
  - Markov Chain Monte Carlo (MCMC) methods.
  - Hands-on with Bayesian tools (e.g., PyMC3 or simpler samplers).
- **Learning Activities:**
  - Implement a basic Bayesian model and run MCMC sampling.
  - Visualize the convergence of chains and posterior distributions.
- **Discussion:**
  - Pros and cons of Bayesian inference in dynamic decision systems.

---

### **Day 19: Time Series Fundamentals**

- **Topics:**
  - Components of time series: trend, seasonality, noise.
  - Stationarity and autocorrelation.
- **Learning Activities:**
  - Analyze a real-world time series dataset (e.g., stock prices or weather data).
  - Use Pandas for time series manipulation and Matplotlib/Seaborn for visualization.
- **Discussion:**
  - When to use simple models vs. time series forecasting models.

---

### **Day 20: Advanced Time Series Modeling**

- **Topics:**
  - ARIMA and Seasonal ARIMA models.
  - Model diagnostics and residual analysis.
- **Learning Activities:**
  - Build ARIMA models using the statsmodels library.
  - Visualize the forecasts and compare them to actual data.
- **Discussion:**
  - Limitations of ARIMA models and introduction to another method when dealing with non-stationary data (like exponential smoothing).

---

### **Day 21: Multivariate Statistics & PCA**

- **Topics:**
  - Introduction to multivariate analysis.
  - Principal Component Analysis (PCA) for dimensionality reduction.
- **Learning Activities:**
  - Apply PCA using scikit-learn on a dataset with many features.
  - Visualize the explained variance and the transformed features.
- **Discussion:**
  - When PCA is beneficial and when other dimensionality techniques—like t-SNE or UMAP—might be more appropriate.

---

### **Day 22: Clustering and Unsupervised Methods**

- **Topics:**
  - Overview of clustering: K-Means, hierarchical clustering.
  - Evaluating cluster quality with silhouette scores.
- **Learning Activities:**
  - Cluster a dataset and visualize clusters using scatter plots.
  - Compare different clustering results and discuss strengths/weaknesses.
- **Discussion:**
  - Limitations of clustering methods in high-dimensional spaces.

---

### **Day 23: Statistical Learning Theory Basics**

- **Topics:**
  - Bias-variance tradeoff.
  - Overfitting vs. underfitting and model complexity.
- **Learning Activities:**
  - Use simulation examples to see how increasing model complexity affects bias and variance.
  - Discuss cross-validation and other techniques to mitigate overfitting.
- **Discussion:**
  - How statistical theory underpins many ML regularization techniques.

---

### **Day 24: Regularization Techniques**

- **Topics:**
  - Ridge, Lasso, and ElasticNet regression.
  - How regularization helps manage multicollinearity and prevent overfitting.
- **Learning Activities:**
  - Implement and compare these techniques using scikit-learn.
  - Visualize coefficient shrinkage and model performance.
- **Discussion:**
  - When to choose each method and how to interpret penalized coefficients.

---

### **Day 25: Model Selection & Hyperparameter Tuning**

- **Topics:**
  - Using cross-validation, grid search, and randomized search.
  - Evaluation strategies for both regression and classification.
- **Learning Activities:**
  - Build a complete ML pipeline and optimize hyperparameters with scikit-learn’s utilities.
  - Visualize performance across varying hyperparameter values.
- **Discussion:**
  - The importance of avoiding data leakage and overfitting during model selection.

---

### **Day 26: Special Topics in Advanced Statistics**

- **Topics:**
  - Survival analysis (time-to-event data) or feature importance assessment.
  - Advanced model interpretation techniques (e.g., permutation importance).
- **Learning Activities:**
  - Explore a dataset where survival analysis is applicable (e.g., patient data).
  - Experiment with feature selection using statistical tests.
- **Discussion:**
  - Limitations of standard regression in time-to-event data.

---

### **Day 27: Interpreting and Communicating Statistical Results**

- **Topics:**
  - Common pitfalls: p-hacking, multiple comparisons.
  - Effective visualization and interpretation for non-technical audiences.
- **Learning Activities:**
  - Review case studies where statistical results were misinterpreted.
  - Create clear reports with data visualizations demonstrating your findings.
- **Discussion:**
  - Best practices for transparent and reproducible statistical analysis.

---

### **Day 28: Statistics for Fairness, Bias, & Interpretability in AI**

- **Topics:**
  - Statistical methods to detect bias in AI models.
  - Techniques for model interpretability (like SHAP values).
- **Learning Activities:**
  - Analyze a dataset for fairness issues.
  - Visualize the contributions of different features to model predictions.
- **Discussion:**
  - Why rigorous statistical analysis is critical for ethical AI.

---

### **Day 29: Integrating Statistics into End-to-End Machine Learning Pipelines**

- **Topics:**
  - Combining EDA, statistical testing, model building, and evaluation.
  - Workflow automation using Python (e.g., scikit-learn pipelines).
- **Learning Activities:**
  - Build an end-to-end project that starts with data cleaning, moves through statistical analysis, and ends with an ML model evaluation.
  - Document the process and highlight the reasons behind each statistical test.
- **Discussion:**
  - When and why each statistical component is critical within the pipeline.

---

### **Day 30: Capstone Project & Review**

- **Topics:**
  - Apply all the concepts learned over the past month.
- **Project Outline:**
  - Choose a real-world dataset (for example, a public dataset from the UCI repository).
  - **Steps:**
    1. **Data Exploration & Cleaning:** Use descriptive statistics and EDA.
    2. **Statistical Inference:** Conduct hypothesis tests or confidence interval analysis to validate initial assumptions.
    3. **Feature Engineering & Model Building:** Utilize regression or classification (with proper statistical evaluation).
    4. **Model Evaluation:** Use cross-validation and statistical tests to confirm model significance.
    5. **Interpret & Communicate:** Prepare a final report with visual explanations (charts, confidence intervals, etc.).
- **Outcome:**
  - A comprehensive project demonstration that reinforces when and why to use specific statistical methods, outlines their limitations, and presents alternatives where necessary.

---
---

Below is the detailed plan and examples for Days 1 through 7 from the 30‑day statistics roadmap with a focus on machine learning and AI. Each day’s section includes theory, visualization ideas, discussion points, and sample code examples (with detailed comments) using real sample data. You can run these examples in your Python environment (e.g., in VSCode with Jupyter notebooks or plain Python scripts). Feel free to adjust the datasets or visualization parameters to suit your learning pace.

---

## **Day 1: Introduction & Descriptive Statistics**

### **Topics Covered**
- **Overview of Statistics in ML & AI:** Importance of statistics in understanding data and building models.
- **Types of Data:** Categorical vs. numerical; continuous vs. discrete.
- **Descriptive Statistics:** Computation of mean, median, mode, variance, and standard deviation.

### **Learning Activities**
- **Hands-on Coding:** Create a sample dataset using NumPy and Pandas.
- **Visualization:** Plot histograms and box plots to understand data distribution.

### **Example Code**

```python:src/day1/descriptive_stats.py
import pandas as pd
import numpy as np
import matplotlib.pyplot as plt

# Set a random seed for reproducibility.
np.random.seed(42)

# Create a simple dataset with two columns: 'Age' and 'Salary'.
data = {
    'Age': np.random.randint(18, 70, size=100),  # Random ages between 18 and 70.
    'Salary': np.random.normal(50000, 15000, size=100)  # Salaries with a mean of 50k and std dev of 15k.
}
df = pd.DataFrame(data)

# Display descriptive statistics for the dataset.
print("Descriptive Statistics:\n", df.describe())

# Visualization:
# Create a figure with 2 subplots for Age and Salary distributions.
plt.figure(figsize=(12, 5))

# Histogram for Age.
plt.subplot(1, 2, 1)
plt.hist(df['Age'], bins=10, color='skyblue', edgecolor='black')
plt.title('Distribution of Age')
plt.xlabel('Age')
plt.ylabel('Frequency')

# Histogram for Salary.
plt.subplot(1, 2, 2)
plt.hist(df['Salary'], bins=10, color='lightgreen', edgecolor='black')
plt.title('Distribution of Salary')
plt.xlabel('Salary')
plt.ylabel('Frequency')

plt.tight_layout()  # Adjust subplots to not overlap.
plt.show()
```

*This script covers the basics: dataset creation, computing statistics, and visualizations. Understanding your data early is vital to know what kind of models or additional processing might be needed later.*

---

## **Day 2: Exploratory Data Analysis (EDA) and Visualization**

### **Topics Covered**
- **EDA Importance:** Data cleaning, identifying missing data, and detecting outliers.
- **Visualization Techniques:** Scatter plots, pair plots, and correlation heatmaps.
- **Library Tools:** Pandas, Matplotlib, and Seaborn for interactive EDA.

### **Learning Activities**
- **Dataset Choice:** Use a dataset such as the Titanic dataset or any CSV file with real-world data.
- **Visualizations:** Create pairplots and heatmaps to observe relationships.

### **Example Code**

```python:src/day2/eda_visualization.py
import pandas as pd
import seaborn as sns
import matplotlib.pyplot as plt

# Load a sample dataset. For example, here we use the famous Titanic dataset.
# You can download it from: https://www.kaggle.com/c/titanic/data
titanic_df = pd.read_csv('data/titanic.csv')

# Display the first few records to understand the data structure.
print("First 5 records:\n", titanic_df.head())

# Checking for missing values.
missing = titanic_df.isnull().sum()
print("Missing Values:\n", missing)

# Visualize the relationships with Seaborn's pairplot.
# For simplicity, use a small number of columns (e.g., Age, Fare, and a binary indicator for Survived).
cols_to_plot = ['Age', 'Fare', 'Survived']
sns.pairplot(titanic_df[cols_to_plot].dropna(), hue='Survived', palette='Set1')
plt.suptitle("Pairplot of Selected Titanic Data Features", y=1.02)
plt.show()

# Create a heatmap of the correlation matrix for numerical features.
plt.figure(figsize=(8, 6))
corr_matrix = titanic_df.corr()
sns.heatmap(corr_matrix, annot=True, cmap='coolwarm', fmt=".2f")
plt.title("Correlation Matrix for Titanic Dataset")
plt.show()
```

*This script demonstrates how to perform basic EDA. You learn to identify missing data, visualize pairwise relationships, and understand correlations—all key for proper model building later.*

---

## **Day 3: Basics of Probability**

### **Topics Covered**
- **Probability Fundamentals:** Definitions, sample space, events.
- **Independent vs. Dependent Events:** How they differ and how they can be simulated.
- **Conditional Probability & Bayes’ Theorem:** Basic concepts without going too deep.

### **Learning Activities**
- **Simulations:** Code to simulate coin tosses and dice rolls.
- **Visualization:** Plot the frequencies of outcomes versus theoretical probabilities.

### **Example Code**

```python:src/day3/probability_basics.py
import numpy as np
import matplotlib.pyplot as plt

# Simulate coin tosses.
def simulate_coin_tosses(num_tosses=1000):
    # 0 represents "Heads", 1 represents "Tails"
    tosses = np.random.randint(0, 2, size=num_tosses)
    return tosses

# Compute frequencies of heads and tails.
tosses = simulate_coin_tosses(1000)
heads = (tosses == 0).sum()
tails = (tosses == 1).sum()

print(f"Total Heads: {heads}, Total Tails: {tails}")

# Bar plot to visualize these results.
labels = ['Heads', 'Tails']
counts = [heads, tails]

plt.bar(labels, counts, color=['dodgerblue', 'salmon'], edgecolor='black')
plt.title("Coin Toss Simulation Results")
plt.ylabel("Frequency")
plt.show()

# Example: Theoretical vs. Empirical probabilities.
# The theoretical probability for a fair coin is 0.5 for heads and 0.5 for tails.
theoretical_prob = 0.5
print("Theoretical probability (each):", theoretical_prob)
# Compare with the simulated empirical probabilities.
print("Empirical probabilities:",
      f"Heads: {heads / 1000:.3f}, Tails: {tails / 1000:.3f}")
```

*In this script, we simulate coin tosses to see how experimental frequencies compare with theoretical probabilities. This helps to visualize basic probability laws.*

---

## **Day 4: Discrete Probability Distributions**

### **Topics Covered**
- **Common Discrete Distributions:** Bernoulli, Binomial, Geometric, and Poisson.
- **Properties:** Expectation, variance, and probability mass functions (PMFs).

### **Learning Activities**
- **Using SciPy:** Plotting the PMFs with available functions.
- **Real-World Modeling:** Understanding when to apply these distributions.

### **Example Code**

```python:src/day4/discrete_distributions.py
import numpy as np
import matplotlib.pyplot as plt
from scipy.stats import binom, poisson, geom

# Example: Binomial Distribution.
# Parameters: n (number of trials), p (success probability).
n, p = 10, 0.5
x = np.arange(0, n+1)
binom_pmf = binom.pmf(x, n, p)

plt.figure(figsize=(12, 4))

plt.subplot(1, 3, 1)
plt.stem(x, binom_pmf, basefmt=" ", use_line_collection=True)
plt.title("Binomial Distribution PMF")
plt.xlabel("Number of successes")
plt.ylabel("Probability")

# Example: Poisson Distribution.
# Parameter: lambda (average rate).
lambda_poisson = 3
x_poisson = np.arange(0, 15)
poisson_pmf = poisson.pmf(x_poisson, lambda_poisson)

plt.subplot(1, 3, 2)
plt.stem(x_poisson, poisson_pmf, basefmt=" ", use_line_collection=True)
plt.title("Poisson Distribution PMF")
plt.xlabel("Events per interval")
plt.ylabel("Probability")

# Example: Geometric Distribution.
# Parameter: p (success probability).
p_geom = 0.3
x_geom = np.arange(1, 15)  # Geometric distribution starts at 1.
geom_pmf = geom.pmf(x_geom, p_geom)

plt.subplot(1, 3, 3)
plt.stem(x_geom, geom_pmf, basefmt=" ", use_line_collection=True)
plt.title("Geometric Distribution PMF")
plt.xlabel("Trial number")
plt.ylabel("Probability")

plt.tight_layout()
plt.show()
```

*This script demonstrates how to use the SciPy library to visualize discrete probability distributions. Understanding the shape and spread of these distributions will help decide which is best suited for modeling your data.*

---

## **Day 5: Continuous Probability Distributions**

### **Topics Covered**
- **Common Continuous Distributions:** Uniform, Normal (Gaussian), Exponential, etc.
- **Functions:** Probability density functions (PDFs) and cumulative distribution functions (CDFs).

### **Learning Activities**
- **Visualization:** Plotting PDFs and CDFs for comparison.
- **Simulation:** Generating random samples from these distributions.

### **Example Code**

```python:src/day5/continuous_distributions.py
import numpy as np
import matplotlib.pyplot as plt
from scipy.stats import norm, uniform, expon

# Prepare a range of values.
x = np.linspace(-5, 5, 1000)

plt.figure(figsize=(12, 8))

# Normal (Gaussian) Distribution.
mu, sigma = 0, 1
normal_pdf = norm.pdf(x, mu, sigma)
plt.subplot(2, 2, 1)
plt.plot(x, normal_pdf, label='Normal PDF', color='blue')
plt.title("Normal (Gaussian) Distribution")
plt.xlabel("x")
plt.ylabel("Probability density")
plt.legend()

# Uniform Distribution.
# Define Uniform distribution between -2 and 2.
a, b = -2, 2
uniform_pdf = uniform.pdf(x, loc=a, scale=b - a)
plt.subplot(2, 2, 2)
plt.plot(x, uniform_pdf, label='Uniform PDF', color='green')
plt.title("Uniform Distribution")
plt.xlabel("x")
plt.ylabel("Probability density")
plt.legend()

# Exponential Distribution.
# Parameter: lambda. Here, scale = 1/lambda.
lambda_exp = 1.0
x_exp = np.linspace(0, 10, 1000)
exponential_pdf = expon.pdf(x_exp, scale=1/lambda_exp)
plt.subplot(2, 2, 3)
plt.plot(x_exp, exponential_pdf, label='Exponential PDF', color='red')
plt.title("Exponential Distribution")
plt.xlabel("x")
plt.ylabel("Probability density")
plt.legend()

plt.tight_layout()
plt.show()
```

*This code compares three common continuous distributions. You can see the bell-curve of the normal distribution versus the flat uniform distribution and the exponential decay. Such visualizations are key when making assumptions in real-world modeling.*

---

## **Day 6: Sampling Methods & the Central Limit Theorem (CLT)**

### **Topics Covered**
- **Sampling Methods:** Simple random sampling, stratified sampling.
- **Central Limit Theorem (CLT):** How the sampling distribution of the mean approaches a normal distribution with larger sample sizes.

### **Learning Activities**
- **Simulation:** Draw samples from a non-normal distribution (e.g., exponential) and observe the distribution of sample means.
- **Visualization:** Show how the histogram of the means narrows and approximates a normal distribution.

### **Example Code**

```python:src/day6/central_limit_theorem.py
import numpy as np
import matplotlib.pyplot as plt

# Function to generate sample means.
def generate_sample_means(population, sample_size, num_samples):
    means = [np.mean(np.random.choice(population, size=sample_size)) for _ in range(num_samples)]
    return means

# Create a non-normal population: Exponential distribution.
lambda_value = 1.0
population = np.random.exponential(scale=1/lambda_value, size=10000)

# Parameters for sampling.
sample_size = 50
num_samples = 500

# Generate sample means.
sample_means = generate_sample_means(population, sample_size, num_samples)

# Plot the histogram of sample means.
plt.figure(figsize=(10, 6))
plt.hist(sample_means, bins=30, color='purple', edgecolor='black', density=True)
plt.title("Distribution of Sample Means (CLT Demonstration)")
plt.xlabel("Sample Mean")
plt.ylabel("Density")

# Overlay a normal distribution curve for reference.
from scipy.stats import norm
mean_of_means = np.mean(sample_means)
std_of_means = np.std(sample_means)
x_norm = np.linspace(mean_of_means - 3*std_of_means, mean_of_means + 3*std_of_means, 1000)
plt.plot(x_norm, norm.pdf(x_norm, mean_of_means, std_of_means), color='orange', lw=2, label='Normal Approximation')
plt.legend()
plt.show()
```

*This demonstration shows that even if the original data is skewed (exponential in this case), the means of sufficiently large random samples form a distribution that is approximately normal—an essential idea behind many statistical methods.*

---

## **Day 7: Estimation and Confidence Intervals**

### **Topics Covered**
- **Estimation Methods:** Point estimates vs. interval estimates.
- **Confidence Intervals:** Constructing intervals for means and proportions.
- **Techniques:** Analytical methods and bootstrapping.

### **Learning Activities**
- **Calculate Confidence Intervals:** Using sample data to compute the interval for an estimate.
- **Visualization:** Plot the sample mean along with its confidence interval on a graph.

### **Example Code**

```python:src/day7/confidence_intervals.py
import numpy as np
import matplotlib.pyplot as plt
from scipy import stats

# Generate a sample dataset.
np.random.seed(42)
sample_data = np.random.normal(loc=100, scale=15, size=50)

# Compute the sample mean and standard error.
sample_mean = np.mean(sample_data)
sample_std = np.std(sample_data, ddof=1)  # Use ddof=1 for sample standard deviation.
n = len(sample_data)
standard_error = sample_std / np.sqrt(n)

# Set the confidence level, e.g., 95%.
confidence_level = 0.95
alpha = 1 - confidence_level

# Calculate the margin of error using the t-distribution.
margin_error = stats.t.ppf(1 - alpha/2, df=n-1) * standard_error

# Define the confidence interval.
conf_interval = (sample_mean - margin_error, sample_mean + margin_error)
print(f"Sample Mean: {sample_mean:.2f}")
print(f"95% Confidence Interval: {conf_interval[0]:.2f} to {conf_interval[1]:.2f}")

# Visualization: Plot the sample points and the confidence interval.
plt.figure(figsize=(8, 4))
plt.errorbar(1, sample_mean, yerr=margin_error, fmt='o', color='blue', capsize=5)
plt.xlim(0.5, 1.5)
plt.title("Confidence Interval for the Sample Mean")
plt.xticks([1], ['Sample Mean'])
plt.ylabel("Value")
plt.grid(True)
plt.show()
```

*In this example, we generate a dataset, compute its mean and standard error, and then calculate and visualize a 95% confidence interval using the t-distribution. Understanding confidence intervals helps in assessing the reliability of estimates—vital for both hypothesis testing and model predictions.*

---

## **Final Notes for Days 1-7**

- **Visualization & Real-World Data:** Whenever possible, replace simulated data with real-world cases to see how these methods apply in practice.
- **Discussion Points:**  
  - **When Not to Use a Method:** For example, a box plot may not fully capture the nuances of very small datasets, or a normal assumption might fail for heavy-tailed data.
  - **Alternative Methods:** In cases where assumptions are violated, non-parametric or robust statistical methods might be more appropriate.
- **Practice:** The hands-on exercises included in each day reinforce your theoretical learning through practical implementation.

---
---
Below is the detailed plan with examples for Days 8, 9, and 10 from the roadmap. Each day’s section includes theory, discussion points, and fully commented code examples using Python (with visualization) on your Windows machine in VSCode. You can run these examples in a Jupyter Notebook or directly as Python scripts. The file paths are provided after the initial backticks for clarity.

---

## **Day 8: Fundamentals of Hypothesis Testing**

### **Topics Covered**
- **Key Concepts:**  
  - **Null Hypothesis (H₀):** The statement being tested (e.g., "the population mean equals a specific value").  
  - **Alternative Hypothesis (H₁):** The statement we support if evidence rejects H₀.  
  - **Type I & II Errors:** False positive and false negative errors.  
  - **p-value & Significance Level:** The p-value helps determine the statistical significance.  
- **Learning Goals:**  
  - Conduct a one‑sample t‑test using SciPy.  
  - Visualize the test statistic distribution, highlighting the critical region.

### **Example: One-Sample t-Test**

The following example generates a sample dataset from a normal distribution and tests whether the sample mean significantly differs from an assumed population mean (say 100). It also shows a plot of the t-distribution with the computed t‑statistic and critical regions.

```python:src/day8/hypothesis_testing.py
import numpy as np
import matplotlib.pyplot as plt
from scipy import stats

# -------------------------------
# Generate sample data
# -------------------------------
np.random.seed(42)  # Reproducible output
n = 30  # Sample size
true_mean = 105  # True mean used to generate the data
population_std = 15  # Standard deviation of the population

# Generate a random sample from a normal distribution
sample_data = np.random.normal(loc=true_mean, scale=population_std, size=n)

# -------------------------------
# One-sample t-test
# -------------------------------
# Null hypothesis: population mean (mu0) is equal to 100.
mu0 = 100

# Calculate the t-statistic and corresponding two-tailed p-value.
t_stat, p_value = stats.ttest_1samp(sample_data, popmean=mu0)
print(f"One-Sample t-test results:\n t-statistic = {t_stat:.3f}, p-value = {p_value:.3f}")

# -------------------------------
# Visualization of the t-distribution with critical regions
# -------------------------------
# Calculate degrees of freedom and the critical t-value for alpha = 0.05 (two-tailed).
df = n - 1
confidence_level = 0.95
alpha = 1 - confidence_level
t_critical = stats.t.ppf(1 - alpha/2, df)

# Create a range of t values for plotting the t-distribution.
t_range = np.linspace(-4, 4, 1000)
pdf_t = stats.t.pdf(t_range, df)

plt.figure(figsize=(10, 6))
plt.plot(t_range, pdf_t, label=f't-distribution (df={df})', color='black')
plt.fill_between(t_range, pdf_t, where=(t_range <= -t_critical) | (t_range >= t_critical),
                 color='red', alpha=0.3, label='Critical region (α/2)')
plt.axvline(x=t_stat, color='blue', linestyle='--', label=f'Observed t-statistic = {t_stat:.2f}')
plt.axvline(x=-t_critical, color='red', linestyle='--', label=f'Critical value = -{t_critical:.2f}')
plt.axvline(x=t_critical, color='red', linestyle='--', label=f'Critical value = {t_critical:.2f}')
plt.title("One-Sample t-Test Visualization")
plt.xlabel("t value")
plt.ylabel("Probability Density")
plt.legend()
plt.grid(True)
plt.show()

# -------------------------------
# Discussion:
# -------------------------------
# If p_value < alpha, we reject H₀, meaning the sample provides statistically significant evidence that the
# mean is different from mu0. It is important to note the limitations of the p-value and consider effect size
# and practical significance as well.
```

*In this example, the script calculates a one‑sample t‑test, prints the test statistic and p‑value, and then visualizes the relevant t‑distribution, critical regions, and the observed t‑statistic.*

---

## **Day 9: Advanced Hypothesis Testing and ANOVA**

### **Topics Covered**
- **Two-Sample Tests:** Compare means (e.g., independent t-test) or distributions (e.g., Mann–Whitney U test for non-parametric data).  
- **ANOVA (Analysis of Variance):** Used to compare means across three or more groups.  
- **Learning Goals:**  
  - Conduct two‑sample t‑tests and one‑way ANOVA using SciPy.  
  - Visualize group differences with box plots.

### **Example: Two-Sample t-Test and One-Way ANOVA**

The following example creates a simulated dataset with three groups representing, for instance, different treatments and compares their means using both independent t‑tests (for pairwise comparisons) and one‑way ANOVA (to test if any group mean differs).

```python:src/day9/anova_tests.py
import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
from scipy import stats

# -------------------------------
# Create a sample dataset with three groups
# -------------------------------
np.random.seed(42)
group_size = 40

# Generate data for three groups with different means
group_A = np.random.normal(loc=50, scale=10, size=group_size)
group_B = np.random.normal(loc=55, scale=10, size=group_size)
group_C = np.random.normal(loc=60, scale=10, size=group_size)

# Combine into a Pandas DataFrame for convenience
data = pd.DataFrame({
    'Value': np.concatenate([group_A, group_B, group_C]),
    'Group': ['A'] * group_size + ['B'] * group_size + ['C'] * group_size
})
print("First 5 records of the dataset:\n", data.head())

# -------------------------------
# Visualization: Boxplot for group differences
# -------------------------------
plt.figure(figsize=(8, 6))
data.boxplot(column='Value', by='Group', grid=False, patch_artist=True,
             boxprops=dict(facecolor='skyblue', color='blue'),
             medianprops=dict(color='red'))
plt.title("Boxplot for Groups A, B, and C")
plt.suptitle("")  # Remove the automatic title to avoid duplication.
plt.xlabel("Group")
plt.ylabel("Value")
plt.show()

# -------------------------------
# Pairwise t-tests: Compare groups A and B (as an example of two-sample t-test)
# -------------------------------
t_stat_AB, p_value_AB = stats.ttest_ind(group_A, group_B)
print(f"Two-Sample t-test (Group A vs. Group B): t-statistic = {t_stat_AB:.3f}, p-value = {p_value_AB:.3f}")

# -------------------------------
# One-way ANOVA: Test if the means of groups A, B, and C are all equal
# -------------------------------
f_stat, p_value_anova = stats.f_oneway(group_A, group_B, group_C)
print(f"One-way ANOVA: F-statistic = {f_stat:.3f}, p-value = {p_value_anova:.3f}")

# -------------------------------
# Discussion:
# -------------------------------
# - Pairwise t-tests (like between Group A and B) help explore differences in specific cases.
# - One-way ANOVA tests the overall null hypothesis that all group means are equal.
# - When assumptions (e.g., normality and equal variances) are violated, non-parametric tests can be used.
```

*In this example, we simulate data for three groups, use a boxplot to visually inspect group differences, perform pairwise independent t‑tests, and then execute a one‑way ANOVA to test whether the differences among group means are statistically significant.*

---

## **Day 10: Correlation & Covariance**

### **Topics Covered**
- **Correlation:** Measures linear association between two variables; examples include Pearson and Spearman correlation.  
- **Covariance:** Indicates the directional relationship between two variables.  
- **Learning Goals:**  
  - Calculate Pearson and Spearman correlation coefficients.  
  - Visualize correlations using scatter plots.  
  - Discuss the effect of outliers and potential pitfalls (spurious correlations).

### **Example: Correlation Analysis**

The following example uses a simulated dataset to illustrate the correlation between two variables. It computes both Pearson and Spearman correlations and visualizes the result using a scatter plot. In addition, a covariance computation is provided.

```python:src/day10/correlation_covariance.py
import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
from scipy import stats

# -------------------------------
# Generate sample data
# -------------------------------
np.random.seed(42)

# Create a sample dataset with two correlated variables.
# For example, X is uniformly spaced random values and Y is linearly dependent on X with some noise.
n_points = 100
X = np.random.uniform(0, 100, n_points)
noise = np.random.normal(0, 10, n_points)
Y = 2 * X + noise  # A linear relationship with some noise added.

# Create a DataFrame for easier handling.
df = pd.DataFrame({'X': X, 'Y': Y})
print("First 5 records of the correlation dataset:\n", df.head())

# -------------------------------
# Calculate Pearson correlation coefficient
# -------------------------------
pearson_corr, pearson_pvalue = stats.pearsonr(df['X'], df['Y'])
print(f"Pearson correlation: {pearson_corr:.3f} (p-value = {pearson_pvalue:.3f})")

# -------------------------------
# Calculate Spearman correlation coefficient
# -------------------------------
spearman_corr, spearman_pvalue = stats.spearmanr(df['X'], df['Y'])
print(f"Spearman correlation: {spearman_corr:.3f} (p-value = {spearman_pvalue:.3f})")

# -------------------------------
# Calculate Covariance
# -------------------------------
covariance = np.cov(df['X'], df['Y'])[0, 1]
print(f"Covariance between X and Y: {covariance:.3f}")

# -------------------------------
# Visualization: Scatter plot with regression line
# -------------------------------
plt.figure(figsize=(8, 6))
plt.scatter(df['X'], df['Y'], color='dodgerblue', edgecolor='black', alpha=0.7, label='Data points')
plt.title("Scatter Plot of X vs Y with Regression Line")
plt.xlabel("X")
plt.ylabel("Y")

# Fit a linear regression line (using numpy.polyfit for convenience)
slope, intercept = np.polyfit(df['X'], df['Y'], 1)
x_line = np.linspace(df['X'].min(), df['X'].max(), 100)
y_line = slope * x_line + intercept
plt.plot(x_line, y_line, color='red', linestyle='--', label=f'Regression line: Y = {slope:.2f}X + {intercept:.2f}')
plt.legend()
plt.grid(True)
plt.show()

# -------------------------------
# Discussion:
# -------------------------------
# - Pearson correlation measures the linear relationship but is sensitive to outliers.
# - Spearman correlation uses rank-based correlation and is more robust to outlier effects.
# - Covariance shows the direction (positive or negative) of the linear dependency, but its magnitude is not standardized.
# - Remember: Correlation does not imply causation; always consider the context of the data.
```

*In this example, we generate a dataset of two variables with a known linear dependency, compute both Pearson and Spearman correlation coefficients, calculate the covariance, and then visualize the data with a fitted regression line. This analysis is crucial for understanding relationships between variables before using them in predictive models.*

---

## **Final Thoughts for Days 8-10**

- **Hypothesis Testing:** Know when to reject or accept the null hypothesis by understanding p‑values and the test statistic under its distribution.  
- **ANOVA & Pairwise Testing:** Beware of multiple comparisons and validate assumptions like homogeneity of variances.  
- **Correlation & Covariance:** Use multiple metrics to assess relationships and be cautious of outliers and spurious correlations.

---
---
Below is an in‑depth explanation and detailed examples (including code with extensive inline comments) for Days 10 through 15. These examples not only cover the theory and purpose behind each statistical method but also include code that you can run in your Windows environment using VSCode with Python (or via Jupyter notebooks). The file paths are specified in each code block for clarity.

---

## **Day 10: Correlation & Covariance (Detailed)**

### **Overview**

- **Correlation** measures the strength and direction of a linear relationship between two variables. For example, the Pearson correlation coefficient is defined as

\[
\rho = \frac{\mathrm{Cov}(X,Y)}{\sigma_X \, \sigma_Y}
\]

  where \(\mathrm{Cov}(X,Y)\) is the covariance and \(\sigma_X\) and \(\sigma_Y\) are the standard deviations of \(X\) and \(Y\), respectively.

- **Spearman correlation** is a rank-based (non-parametric) method that is more robust with outliers.
  
- **Covariance** indicates the direction of the linear relationship between two variables. Its magnitude, however, is not standardized.

### **Purpose & Use**

- **When to Use:**  
  - Use Pearson correlation if both variables are continuous and the relationship is linear.
  - Use Spearman correlation when the relationship is monotonic but not necessarily linear, or when outliers are present.
  
- **Limitations:**  
  - Correlation “does not imply causation.”
  - Outliers can distort the Pearson correlation significantly.

### **Example Code**

```python:src/day10/correlation_covariance_detailed.py
import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
from scipy import stats

# -------------------------------
# Generate a simulated dataset:
# -------------------------------
np.random.seed(42)
n_points = 100

# Create a variable X with uniform random values.
X = np.random.uniform(0, 100, n_points)

# Create Y as a linear transformation of X with added noise (to simulate a real-world scenario).
noise = np.random.normal(0, 10, n_points)  # normally distributed noise
Y = 2 * X + noise  # Y is approximately 2 times X plus noise

# Combine into a DataFrame for convenience.
df = pd.DataFrame({'X': X, 'Y': Y})
print("First 5 records of the dataset:\n", df.head())

# -------------------------------
# Calculate Pearson correlation:
# -------------------------------
# Pearson correlation is sensitive to outliers.
pearson_corr, pearson_pvalue = stats.pearsonr(df['X'], df['Y'])
print(f"Pearson correlation coefficient: {pearson_corr:.3f} (p-value = {pearson_pvalue:.3f})")

# -------------------------------
# Calculate Spearman correlation:
# -------------------------------
# Spearman's rank correlation is robust for non-linear but monotonic relationships.
spearman_corr, spearman_pvalue = stats.spearmanr(df['X'], df['Y'])
print(f"Spearman correlation coefficient: {spearman_corr:.3f} (p-value = {spearman_pvalue:.3f})")

# -------------------------------
# Calculate Covariance:
# -------------------------------
# numpy.cov returns a covariance matrix.
covariance_matrix = np.cov(df['X'], df['Y'])
covariance = covariance_matrix[0, 1]
print(f"Covariance between X and Y: {covariance:.3f}")

# -------------------------------
# Visualization: Scatter Plot & Regression Line
# -------------------------------
plt.figure(figsize=(8, 6))
plt.scatter(df['X'], df['Y'], color='dodgerblue', edgecolor='black', alpha=0.7, label='Data points')
plt.title("Scatter Plot of X vs Y with Regression Line")
plt.xlabel("X")
plt.ylabel("Y")

# Fit a regression line using numpy.polyfit:
slope, intercept = np.polyfit(df['X'], df['Y'], 1)
x_line = np.linspace(df['X'].min(), df['X'].max(), 100)
y_line = slope * x_line + intercept
plt.plot(x_line, y_line, color='red', linestyle='--', label=f'Regression line:\nY = {slope:.2f}X + {intercept:.2f}')
plt.legend()
plt.grid(True)
plt.show()

# -------------------------------
# Discussion:
# -------------------------------
# - Pearson's correlation quantifies the linear relationship whereas Spearman's uses ranking.
# - Covariance tells us the direction of the association, but its scale depends on the units.
# - Visual inspection through scatter plots can provide context beyond the computed correlation.
```

---

## **Day 11: Simple Linear Regression (Detailed)**

### **Overview**

- **Simple Linear Regression** models the relationship between a single predictor \(X\) and a response \(Y\) using the equation:

\[
Y = \beta_0 + \beta_1 X + \epsilon
\]

  where \(\beta_0\) is the intercept, \(\beta_1\) is the slope, and \(\epsilon\) is the error term.

- **Purpose & Use:**  
  - Estimate and infer the relationship between \(X\) and \(Y\).
  - When the relationship is approximately linear.

### **Key Concepts**

- **Assumptions:**  
  - Linearity, independence, homoscedasticity (constant variance), and normality of residuals.
- **Diagnostics:**  
  - Residual plots to assess model assumptions.
  
### **Example Code**

Below is an example that generates synthetic data, fits a simple linear regression model using both scikit‑learn and statsmodels (for a detailed summary), and visualizes the regression along with residual analysis.

```python:src/day11/simple_linear_regression.py
import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
from sklearn.linear_model import LinearRegression
import statsmodels.api as sm

# -------------------------------
# Generate synthetic data:
# -------------------------------
np.random.seed(42)
n = 100

# Generate X uniformly and create Y with a linear relationship plus noise.
X = np.random.uniform(0, 10, n)
true_slope = 3.5
true_intercept = 2.0
noise = np.random.normal(0, 2, n)
Y = true_intercept + true_slope * X + noise

# Reshape X for scikit-learn
X_reshaped = X.reshape(-1, 1)

# -------------------------------
# Method 1: Using scikit-learn:
# -------------------------------
model = LinearRegression()
model.fit(X_reshaped, Y)
pred_Y = model.predict(X_reshaped)

print("Scikit-learn Linear Regression:")
print(f"Estimated Intercept: {model.intercept_:.3f}")
print(f"Estimated Slope: {model.coef_[0]:.3f}")

# -------------------------------
# Visualization: Scatter Plot with the Regression Line
# -------------------------------
plt.figure(figsize=(12, 5))
plt.subplot(1, 2, 1)
plt.scatter(X, Y, color='lightblue', edgecolor='black', label='Data points')
plt.plot(X, pred_Y, color='red', linestyle='--', label='Regression Line')
plt.title("Simple Linear Regression")
plt.xlabel("X")
plt.ylabel("Y")
plt.legend()
plt.grid(True)

# -------------------------------
# Residual Analysis:
# -------------------------------
residuals = Y - pred_Y
plt.subplot(1, 2, 2)
plt.scatter(X, residuals, color='purple', edgecolor='black')
plt.axhline(y=0, color='red', linestyle='--')
plt.title("Residual Plot")
plt.xlabel("X")
plt.ylabel("Residuals")
plt.grid(True)
plt.tight_layout()
plt.show()

# -------------------------------
# Method 2: Using statsmodels for a detailed summary:
# -------------------------------
# Add a constant term to X for intercept estimation.
X_with_const = sm.add_constant(X)
model_sm = sm.OLS(Y, X_with_const)
results = model_sm.fit()
print(results.summary())

# -------------------------------
# Discussion:
# -------------------------------
# - scikit-learn provides a simple implementation, while statsmodels gives a detailed statistical summary.
# - Residual plots help verify assumptions such as homoscedasticity and linearity.
# - The regression output includes estimates, p-values, confidence intervals, and model diagnostics.
```

---

## **Day 12: Multiple Linear Regression (Detailed)**

### **Overview**

- **Multiple Linear Regression** extends simple regression to include multiple predictors:

\[
Y = \beta_0 + \beta_1X_1 + \beta_2X_2 + \cdots + \beta_pX_p + \epsilon
\]

- **Purpose & Use:**  
  - Models relationships where the outcome is influenced by more than one factor.
  - Helpful in understanding the relative importance of various predictors.

### **Key Concepts**

- **Multicollinearity:** When predictor variables are highly correlated, it can distort coefficient estimates.
- **Diagnostics:** Residual analysis, Variance Inflation Factor (VIF).

### **Example Code**

In the following example, we create a synthetic dataset with multiple features, fit a multiple linear regression model, and visualize the model performance and residuals.

```python:src/day12/multiple_linear_regression.py
import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
from sklearn.linear_model import LinearRegression
import statsmodels.api as sm

# -------------------------------
# Generate synthetic multivariate data:
# -------------------------------
np.random.seed(42)
n = 150
# Two predictor variables X1 and X2 drawn from uniform distributions.
X1 = np.random.uniform(0, 10, n)
X2 = np.random.uniform(5, 15, n)

# Define true coefficients.
beta0 = 1.5
beta1 = 4.0
beta2 = -2.5

# Generate response variable Y with added noise.
noise = np.random.normal(0, 3, n)
Y = beta0 + beta1 * X1 + beta2 * X2 + noise

# Create a DataFrame for easy handling.
data = pd.DataFrame({'X1': X1, 'X2': X2, 'Y': Y})
print("First 5 rows of the data:\n", data.head())

# -------------------------------
# Method 1: Using scikit-learn:
# -------------------------------
# Prepare predictor matrix.
X_sklearn = data[['X1', 'X2']]
model = LinearRegression()
model.fit(X_sklearn, Y)
pred_Y = model.predict(X_sklearn)

print("\nScikit-learn Multiple Linear Regression:")
print(f"Estimated Intercept: {model.intercept_:.3f}")
print(f"Estimated Coefficients: {model.coef_}")

# -------------------------------
# Visualization: Actual vs Predicted & Residuals
# -------------------------------
plt.figure(figsize=(12, 5))
plt.subplot(1, 2, 1)
plt.scatter(Y, pred_Y, color='mediumseagreen', edgecolor='black', alpha=0.7)
plt.xlabel("Actual Y")
plt.ylabel("Predicted Y")
plt.title("Actual vs Predicted")
plt.plot([Y.min(), Y.max()], [Y.min(), Y.max()], color='red', linestyle='--')
plt.grid(True)

# Residual Plot:
residuals = Y - pred_Y
plt.subplot(1, 2, 2)
plt.scatter(pred_Y, residuals, color='slateblue', edgecolor='black', alpha=0.7)
plt.axhline(y=0, color='red', linestyle='--')
plt.xlabel("Predicted Y")
plt.ylabel("Residuals")
plt.title("Residual Plot")
plt.grid(True)
plt.tight_layout()
plt.show()

# -------------------------------
# Method 2: Using statsmodels:
# -------------------------------
X_sm = sm.add_constant(X_sklearn)
model_sm = sm.OLS(Y, X_sm).fit()
print(model_sm.summary())

# -------------------------------
# Discussion:
# -------------------------------
# - Multiple linear regression helps us understand the contribution of each predictor.
# - Comparing actual versus predicted values and examining residuals are critical for diagnostics.
# - Statsmodels provides detailed inference (p-values, R-squared, etc.) which is essential for interpretation.
```

---

## **Day 13: Logistic Regression (Detailed)**

### **Overview**

- **Logistic Regression** is used for binary classification. The model predicts the probability of a binary outcome using the logistic function:

\[
P(Y=1|X) = \frac{1}{1 + e^{-(\beta_0 + \beta_1X)}}
\]

- **Purpose & Use:**  
  - Estimate the odds of a binary event occurring.
  - Commonly used in medical diagnosis, fraud detection, and other classification tasks.

### **Key Concepts**

- **Interpretation:**  
  - Coefficients represent the log odds.
  - Odds ratios can be derived from coefficients.
- **Diagnostics:**  
  - Confusion matrix, ROC curve, accuracy, precision, recall, and AUC.

### **Example Code**

Below is a complete example using synthetic data for a binary classification problem. We fit a logistic regression model, visualize the decision boundary, and evaluate the model.

```python:src/day13/logistic_regression.py
import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
from sklearn.linear_model import LogisticRegression
from sklearn.metrics import confusion_matrix, roc_curve, auc
from sklearn.model_selection import train_test_split

# -------------------------------
# Generate synthetic binary classification data:
# -------------------------------
np.random.seed(42)
n = 300

# Generate one predictor X from a uniform distribution.
X = np.random.uniform(0, 10, n)

# True relationship: probability of class 1 increases with X.
# We define a logistic function.
def true_probability(x):
    return 1 / (1 + np.exp(-(x - 5)))

# Generate probabilities and then binary outcomes.
probabilities = true_probability(X)
Y = (np.random.rand(n) < probabilities).astype(int)

# Create a DataFrame.
data = pd.DataFrame({'X': X, 'Y': Y})
print("Distribution of classes:\n", data['Y'].value_counts())

# -------------------------------
# Split data into training and test sets:
# -------------------------------
X_reshaped = X.reshape(-1, 1)
X_train, X_test, y_train, y_test = train_test_split(X_reshaped, Y, test_size=0.3, random_state=42)

# -------------------------------
# Fit Logistic Regression model:
# -------------------------------
log_model = LogisticRegression()
log_model.fit(X_train, y_train)

# Get predicted probabilities for the test set.
y_prob = log_model.predict_proba(X_test)[:, 1]
y_pred = log_model.predict(X_test)

# -------------------------------
# Evaluation: Confusion Matrix and ROC Curve:
# -------------------------------
conf_matrix = confusion_matrix(y_test, y_pred)
print("Confusion Matrix:\n", conf_matrix)

# Compute ROC curve and AUC:
fpr, tpr, thresholds = roc_curve(y_test, y_prob)
roc_auc = auc(fpr, tpr)

plt.figure(figsize=(12, 5))
plt.subplot(1, 2, 1)
plt.scatter(X_test, y_test, c=y_pred, cmap='coolwarm', edgecolor='k', label='Test data')
plt.title("Logistic Regression: Test Data Classification")
plt.xlabel("X")
plt.ylabel("Predicted Class")
plt.grid(True)
plt.legend()

plt.subplot(1, 2, 2)
plt.plot(fpr, tpr, color='blue', label=f'ROC curve (AUC = {roc_auc:.2f})')
plt.plot([0, 1], [0, 1], color='red', linestyle='--')
plt.title("Receiver Operating Characteristic (ROC) Curve")
plt.xlabel("False Positive Rate")
plt.ylabel("True Positive Rate")
plt.legend()
plt.grid(True)
plt.tight_layout()
plt.show()

# -------------------------------
# Discussion:
# -------------------------------
# - Logistic regression maps the linear combination of predictors to \([0, 1]\) using the sigmoid function.
# - Evaluation metrics such as the confusion matrix and ROC-AUC help assess model performance.
# - Visualizing the decision boundary (or class allocation) is useful to understand model behavior.
```

---

## **Day 14: Non-Parametric Tests (Detailed)**

### **Overview**

- **Non-Parametric Tests** are used when the assumptions of parametric tests (such as normality) are not met.
- **Common Non-Parametric Tests:**  
  - **Wilcoxon Signed-Rank Test:** For paired differences.
  - **Mann‑Whitney U Test:** For comparing two independent samples.
  - **Kruskal‑Wallis Test:** For comparing more than two groups (non‑parametrically).

### **Purpose & Use**

- **When to Use:**  
  - Data do not meet the assumptions (e.g., normal distribution) required by parametric tests.
  - When sample sizes are small or when dealing with ordinal data.

### **Example Code**

Below are examples for a paired test using the Wilcoxon signed-rank test and an independent test using the Kruskal‑Wallis test.

```python:src/day14/non_parametric_tests.py
import numpy as np
import matplotlib.pyplot as plt
from scipy import stats

# -------------------------------
# Example 1: Wilcoxon Signed-Rank Test (Paired Data)
# -------------------------------
# Simulate paired data (e.g., measurements before and after a treatment).
np.random.seed(42)
n = 30
before = np.random.normal(50, 5, n)  # baseline measurements
after = before + np.random.normal(2, 3, n)  # treatment effect with noise

# Conduct Wilcoxon signed-rank test.
w_stat, p_value_wilcoxon = stats.wilcoxon(before, after)
print(f"Wilcoxon Signed-Rank Test: statistic = {w_stat:.3f}, p-value = {p_value_wilcoxon:.3f}")

# -------------------------------
# Example 2: Kruskal-Wallis Test (Independent Samples)
# -------------------------------
# Generate data for three independent groups (ordinal or non-normally distributed).
group_A = np.random.normal(30, 4, 25)
group_B = np.random.normal(35, 5, 25)
group_C = np.random.normal(40, 6, 25)

# Conduct the non-parametric Kruskal-Wallis test.
h_stat, p_value_kruskal = stats.kruskal(group_A, group_B, group_C)
print(f"Kruskal-Wallis Test: H-statistic = {h_stat:.3f}, p-value = {p_value_kruskal:.3f}")

# -------------------------------
# Visualization: Boxplots for Group Comparison
# -------------------------------
import matplotlib.pyplot as plt
plt.figure(figsize=(8, 6))
plt.boxplot([group_A, group_B, group_C], labels=['Group A', 'Group B', 'Group C'],
            patch_artist=True, boxprops=dict(facecolor='lightblue'))
plt.title("Boxplots for Three Groups")
plt.ylabel("Values")
plt.grid(True)
plt.show()

# -------------------------------
# Discussion:
# -------------------------------
# - The Wilcoxon signed-rank test is useful when dealing with paired samples without assuming normality.
# - The Kruskal-Wallis test extends the idea to more than two independent groups.
# - Visual tools like boxplots aid in understanding the distribution differences that motivated the use of non-parametric tests.
```

---

## **Day 15: In-Depth ANOVA and Post Hoc Tests (Detailed)**

### **Overview**

- **ANOVA (Analysis of Variance)** tests the null hypothesis that all group means are equal among three or more groups.
- **Post Hoc Tests** (e.g., Tukey’s HSD) are used after a significant ANOVA result to determine which specific groups differ.

### **Purpose & Use**

- **When to Use:**  
  - When comparing means across multiple independent groups.
  - When assumptions of ANOVA (normality, homogeneity of variances) are reasonably met.
- **Post Hoc Analysis:**  
  - Helps pinpoint where significant differences lie, avoiding issues with multiple comparisons.

### **Example Code**

Below is an example that first conducts a one‑way ANOVA on simulated data for three groups and then applies Tukey’s HSD for a post hoc comparison using tools from the statsmodels library.

```python:src/day15/anova_posthoc.py
import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
from scipy import stats
import statsmodels.api as sm
from statsmodels.stats.multicomp import pairwise_tukeyhsd

# -------------------------------
# Generate synthetic data for three groups:
# -------------------------------
np.random.seed(42)
group_size = 40

group_A = np.random.normal(50, 8, group_size)
group_B = np.random.normal(55, 8, group_size)
group_C = np.random.normal(60, 8, group_size)

# Combine into a DataFrame.
data = pd.DataFrame({
    'Value': np.concatenate([group_A, group_B, group_C]),
    'Group': ['A'] * group_size + ['B'] * group_size + ['C'] * group_size
})
print("First 5 records:\n", data.head())

# -------------------------------
# Visualization: Boxplot to visualize group differences:
# -------------------------------
plt.figure(figsize=(8, 6))
data.boxplot(column='Value', by='Group', grid=False, patch_artist=True,
             boxprops=dict(facecolor='skyblue', color='blue'),
             medianprops=dict(color='red'))
plt.title("Boxplot for Groups A, B, and C")
plt.suptitle("")  # Clean up the title.
plt.xlabel("Group")
plt.ylabel("Value")
plt.show()

# -------------------------------
# One-Way ANOVA:
# -------------------------------
# The null hypothesis is that all group means are equal.
f_stat, p_value_anova = stats.f_oneway(group_A, group_B, group_C)
print(f"One-way ANOVA: F-statistic = {f_stat:.3f}, p-value = {p_value_anova:.3f}")

# -------------------------------
# Post Hoc Analysis using Tukey's HSD:
# -------------------------------
# Tukey's test compares means pairwise, controlling for Type I error.
tukey = pairwise_tukeyhsd(endog=data['Value'], groups=data['Group'], alpha=0.05)
print(tukey)

# Plot the results of the Tukey post hoc comparison.
tukey.plot_simultaneous()
plt.title("Tukey HSD Post Hoc Test")
plt.xlabel("Mean Difference")
plt.show()

# -------------------------------
# Discussion:
# -------------------------------
# - The ANOVA result tells us if there is a statistically significant difference among the groups.
# - After a significant ANOVA, Tukey's HSD pinpoints exactly which groups differ.
# - Visualizing through boxplots and simultaneous confidence intervals reinforces the numeric findings.
```

---

## **Final Summary for Days 10-15**

- **Day 10 (Correlation & Covariance):**  
  You learned to compute and visualize relationships between variables using Pearson and Spearman coefficients. Understanding these measures is essential before applying predictive models.

- **Day 11 (Simple Linear Regression):**  
  You practiced fitting a linear model, examining its parameters, and diagnosing model assumptions with residual analysis.

- **Day 12 (Multiple Linear Regression):**  
  Extending to multiple predictors, you saw how to build and interpret models that incorporate more than one independent variable and the importance of diagnostics.

- **Day 13 (Logistic Regression):**  
  You learned how to handle binary classification with logistic regression and to evaluate it using ROC and confusion matrices.

- **Day 14 (Non-Parametric Tests):**  
  When parametric assumptions fail, non‑parametric tests such as the Wilcoxon signed‐rank and Kruskal‑Wallis come in handy.

- **Day 15 (In‑Depth ANOVA and Post Hoc Tests):**  
  You leveraged ANOVA to test for differences across multiple groups and used Tukey’s HSD for detailed pairwise comparisons.

---
---

Below is a detailed explanation and working examples for Days 16 through 22. Each day’s section includes theory, discussion points, and fully commented code examples (with file paths indicated) that you can run in your Windows VSCode environment (using Jupyter Notebook or Python scripts). The code examples include relevant visualizations and explanations to help you understand each concept step by step.

---

## **Day 16: Model Evaluation Metrics**

### **Overview**

In this session, you will learn key evaluation metrics for:

- **Regression Models:**  
  - **RMSE (Root Mean Squared Error):** Measures the standard deviation of residuals (prediction errors).  
  - **MAE (Mean Absolute Error):** The average absolute difference between predictions and actuals.  
  - **R-squared & Adjusted R-squared:** Indicate the proportion of variance explained by the model.

- **Classification Models:**  
  - **Accuracy, Precision, Recall, F1-score:** Standard metrics to evaluate performance.  
  - **ROC Curve & AUC:** Visual and numerical measures of classifier performance.

### **Example Code**

Below is an example that demonstrates metrics for both regression and classification using scikit‑learn.

```python:src/day16/model_evaluation_metrics.py
import numpy as np
import matplotlib.pyplot as plt
from sklearn.metrics import mean_squared_error, mean_absolute_error, r2_score
from sklearn.metrics import accuracy_score, precision_score, recall_score, f1_score, roc_curve, auc
from sklearn.model_selection import train_test_split
from sklearn.linear_model import LinearRegression, LogisticRegression

# -------------------------------
# Regression Example:
# -------------------------------
# Generate synthetic regression data.
np.random.seed(42)
n = 200
X_reg = 2 * np.random.rand(n, 1)  # Predictor in the range [0, 2]
true_slope = 3.5
true_intercept = 1.2
noise_reg = np.random.randn(n, 1)
Y_reg = true_intercept + true_slope * X_reg + noise_reg

# Split the data into training and test sets.
X_train_reg, X_test_reg, y_train_reg, y_test_reg = train_test_split(X_reg, Y_reg, test_size=0.3, random_state=42)

# Create and train the linear regression model.
reg_model = LinearRegression()
reg_model.fit(X_train_reg, y_train_reg)
y_pred_reg = reg_model.predict(X_test_reg)

# Compute evaluation metrics.
rmse = np.sqrt(mean_squared_error(y_test_reg, y_pred_reg))
mae = mean_absolute_error(y_test_reg, y_pred_reg)
r2 = r2_score(y_test_reg, y_pred_reg)

print("Regression Metrics:")
print(f"RMSE: {rmse:.3f}")
print(f"MAE: {mae:.3f}")
print(f"R-squared: {r2:.3f}")

# Visualization: Plot actual vs. predicted values.
plt.figure(figsize=(10, 4))
plt.subplot(1, 2, 1)
plt.scatter(y_test_reg, y_pred_reg, color='mediumseagreen', edgecolor='black', alpha=0.7)
plt.plot([y_test_reg.min(), y_test_reg.max()], [y_test_reg.min(), y_test_reg.max()], color='red', linestyle='--')
plt.xlabel("Actual Y")
plt.ylabel("Predicted Y")
plt.title("Actual vs. Predicted (Regression)")
plt.grid(True)

# -------------------------------
# Classification Example:
# -------------------------------
# Generate synthetic classification data.
np.random.seed(42)
n_class = 300
X_class = np.random.rand(n_class, 1) * 10  # Feature in [0,10]
# Define a binary outcome: probability increases with X.
def true_probability(x): 
    return 1 / (1 + np.exp(-(x - 5)))
probabilities = true_probability(X_class)
Y_class = (np.random.rand(n_class, 1) < probabilities).astype(int).ravel()  # Binary labels

# Split the data.
X_train_class, X_test_class, y_train_class, y_test_class = train_test_split(X_class, Y_class, test_size=0.3, random_state=42)

# Train a logistic regression classifier.
clf = LogisticRegression()
clf.fit(X_train_class, y_train_class)
y_pred_class = clf.predict(X_test_class)
y_prob_class = clf.predict_proba(X_test_class)[:, 1]

# Compute evaluation metrics.
accuracy = accuracy_score(y_test_class, y_pred_class)
precision = precision_score(y_test_class, y_pred_class)
recall = recall_score(y_test_class, y_pred_class)
f1 = f1_score(y_test_class, y_pred_class)

print("\nClassification Metrics:")
print(f"Accuracy: {accuracy:.3f}")
print(f"Precision: {precision:.3f}")
print(f"Recall: {recall:.3f}")
print(f"F1 Score: {f1:.3f}")

# ROC Curve and AUC.
fpr, tpr, thresholds = roc_curve(y_test_class, y_prob_class)
roc_auc = auc(fpr, tpr)

plt.subplot(1, 2, 2)
plt.plot(fpr, tpr, color='blue', lw=2, label=f'ROC curve (AUC = {roc_auc:.2f})')
plt.plot([0, 1], [0, 1], color='red', linestyle='--')
plt.xlabel("False Positive Rate")
plt.ylabel("True Positive Rate")
plt.title("ROC Curve (Classification)")
plt.legend()
plt.grid(True)
plt.tight_layout()
plt.show()

# -------------------------------
# Discussion:
# -------------------------------
# - For regression tasks, always consider multiple metrics to get a comprehensive view of errors.
# - For classification, understanding the trade-offs between precision and recall is vital.
# - ROC-AUC is particularly useful when class distributions are imbalanced.
```

---

## **Day 17: Introduction to Bayesian Statistics**

### **Overview**

- **Bayesian Statistics** provides a framework where prior beliefs about parameters are updated with data to produce a posterior distribution.
- **Key Terminology:**
  - **Prior \(P(\theta)\):** Your belief about a parameter \(\theta\) before data.
  - **Likelihood \(P(D|\theta)\):** Probability of observing the data \(D\) for a given \(\theta\).
  - **Posterior \(P(\theta|D)\):** Updated belief, computed via Bayes’ Theorem:
    
    \[
    P(\theta|D) = \frac{P(D|\theta) \, P(\theta)}{P(D)}
    \]
    
- **Purpose & Use:**
  - Ideal for situations with small data or where uncertainty quantification is crucial.
  - Helps visually represent uncertainty with credible intervals.

### **Example: Bayesian Updating for a Coin Toss**

Below is a simple example using a Beta prior to update the belief about the probability of heads.

```python:src/day17/bayesian_statistics.py
import numpy as np
import matplotlib.pyplot as plt
from scipy.stats import beta

# -------------------------------
# Scenario: Estimating the Probability of Heads
# -------------------------------
# Prior: We start with a non-informative prior Beta(1, 1) (uniform distribution)
alpha_prior, beta_prior = 1, 1  

# Simulated coin toss outcomes.
# Let's assume we toss a coin 10 times with 7 heads and 3 tails.
heads, tails = 7, 3

# -------------------------------
# Bayesian Update:
# -------------------------------
# The posterior for the Beta distribution becomes:
#   Beta(alpha_prior + heads, beta_prior + tails)
alpha_post = alpha_prior + heads
beta_post = beta_prior + tails

# Generate x values
x = np.linspace(0, 1, 100)
prior_pdf = beta.pdf(x, alpha_prior, beta_prior)
posterior_pdf = beta.pdf(x, alpha_post, beta_post)

# Plot Prior vs. Posterior
plt.figure(figsize=(10, 5))
plt.plot(x, prior_pdf, label=f'Prior Beta({alpha_prior}, {beta_prior})', lw=2)
plt.plot(x, posterior_pdf, label=f'Posterior Beta({alpha_post}, {beta_post})', lw=2, color='red')
plt.title("Bayesian Updating of Coin Toss Probability")
plt.xlabel("Probability of Heads")
plt.ylabel("Density")
plt.legend()
plt.grid(True)
plt.show()

# -------------------------------
# Discussion:
# -------------------------------
# - The prior represents our initial belief (uniform, in this case).
# - After observing 7 heads and 3 tails, our updated belief (posterior) shifts,
#   reflecting a higher probability for heads.
# - This method can be generalized to many scenarios where uncertainty quantification is needed.
```

---

## **Day 18: Bayesian Inference & MCMC**

### **Overview**

- **Bayesian Inference** often involves obtaining samples from the posterior distribution when its analytical form is hard to compute.
- **MCMC (Markov Chain Monte Carlo)** methods provide a mechanism to sample from this distribution.
- **Purpose & Use:**
  - Useful for complex models such as Bayesian linear regression.
  - The samples can be used to estimate credible intervals, means, and other posterior properties.

### **Example: Bayesian Linear Regression Using PyMC3**

Below is an example using PyMC3 to perform Bayesian linear regression on synthetic data. *(Ensure PyMC3 is installed in your environment.)*

```python:src/day18/bayesian_inference_mcmc.py
import numpy as np
import matplotlib.pyplot as plt
import pymc3 as pm
import arviz as az

# -------------------------------
# Generate synthetic data for linear regression:
# -------------------------------
np.random.seed(42)
n = 100
X = np.linspace(0, 10, n)
true_intercept = 1.5
true_slope = 2.5
noise = np.random.normal(0, 1.0, n)
Y = true_intercept + true_slope * X + noise

# -------------------------------
# Bayesian Linear Regression using PyMC3:
# -------------------------------
with pm.Model() as model:
    # Priors for intercept and slope:
    intercept = pm.Normal('intercept', mu=0, sigma=10)
    slope = pm.Normal('slope', mu=0, sigma=10)
    
    # Prior for noise (standard deviation)
    sigma = pm.HalfNormal('sigma', sigma=1)
    
    # Expected value of outcome:
    mu = intercept + slope * X
    
    # Likelihood with observed data:
    Y_obs = pm.Normal('Y_obs', mu=mu, sigma=sigma, observed=Y)
    
    # Inference using MCMC:
    trace = pm.sample(2000, tune=1000, target_accept=0.95, return_inferencedata=True)

# -------------------------------
# Visualization: Posterior distributions and regression line:
# -------------------------------
az.plot_trace(trace)
plt.tight_layout()
plt.show()

# Posterior predictive check and regression line plot.
with model:
    posterior_predictive = pm.sample_posterior_predictive(trace, var_names=['intercept', 'slope'])

# Use the posterior samples to compute the regression line.
intercepts_samples = posterior_predictive['intercept']
slopes_samples = posterior_predictive['slope']

# Take the mean estimates.
intercept_est = intercepts_samples.mean()
slope_est = slopes_samples.mean()

plt.figure(figsize=(10, 5))
plt.scatter(X, Y, color='skyblue', edgecolor='black', label='Data')
plt.plot(X, intercept_est + slope_est * X, color='red', lw=2, label='Posterior Mean Regression Line')
plt.xlabel("X")
plt.ylabel("Y")
plt.title("Bayesian Linear Regression Posterior Estimate")
plt.legend()
plt.grid(True)
plt.show()

# -------------------------------
# Discussion:
# -------------------------------
# - This example demonstrates Bayesian inference for linear regression using MCMC sampling.
# - Posterior distributions for the parameters provide a complete description of uncertainty.
# - MCMC can be computationally expensive, but it is powerful for complex Bayesian models.
```

---

## **Day 19: Time Series Fundamentals**

### **Overview**

- **Time Series Analysis** involves studying data points indexed in time order.
- **Key Components:**
  - **Trend:** Long-term movement in data.
  - **Seasonality:** Regular cyclic patterns.
  - **Noise (Residual):** Unexplained variability.
- **Stationarity & Autocorrelation:**  
  - A stationary time series has constant statistical properties over time.
  - Autocorrelation measures the correlation of a signal with a delayed copy of itself.

### **Example: Time Series Decomposition**

Below is an example that simulates a time series with trend and seasonality and uses the `seasonal_decompose` function from statsmodels.

```python:src/day19/time_series_fundamentals.py
import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
from statsmodels.tsa.seasonal import seasonal_decompose

# -------------------------------
# Generate synthetic time series data:
# -------------------------------
np.random.seed(42)
# Create a date range.
dates = pd.date_range(start='2020-01-01', periods=200, freq='D')

# Simulate data: increasing trend + sinusoidal seasonality + noise.
trend = np.linspace(10, 20, 200)
seasonality = 5 * np.sin(np.linspace(0, 3 * np.pi, 200))
noise = np.random.normal(0, 1, 200)
time_series = trend + seasonality + noise

# Create a DataFrame.
ts_data = pd.DataFrame({'Value': time_series}, index=dates)

# -------------------------------
# Decompose the time series:
# -------------------------------
decomposition = seasonal_decompose(ts_data, model='additive', period=30)
trend_component = decomposition.trend
seasonal_component = decomposition.seasonal
residual_component = decomposition.resid

# Plot the decomposition.
plt.figure(figsize=(12, 9))
plt.subplot(411)
plt.plot(ts_data, label='Original', color='black')
plt.legend(loc='upper left')
plt.subplot(412)
plt.plot(trend_component, label='Trend', color='blue')
plt.legend(loc='upper left')
plt.subplot(413)
plt.plot(seasonal_component, label='Seasonality', color='green')
plt.legend(loc='upper left')
plt.subplot(414)
plt.plot(residual_component, label='Residuals', color='red')
plt.legend(loc='upper left')
plt.tight_layout()
plt.show()

# -------------------------------
# Discussion:
# -------------------------------
# - Decomposition helps identify and visualize the underlying structure in a time series.
# - Understanding trend, seasonality, and noise is crucial for selecting forecasting models.
# - Real-world time series might require further stationarity checks and transformation.
```

---

## **Day 20: Advanced Time Series Modeling (ARIMA & SARIMA)**

### **Overview**

- **ARIMA (AutoRegressive Integrated Moving Average)** models are used for forecasting time series data.
- **Seasonal ARIMA (SARIMA):** Extends ARIMA by adding seasonal components.
- **Purpose & Use:**
  - Model non-stationary data with trends and seasonality.
  - Diagnose model performance using residual analysis.

### **Example: Fitting an ARIMA Model**

Below is an example using statsmodels to fit an ARIMA model to synthetic time series data.

```python:src/day20/arima_modeling.py
import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
import statsmodels.api as sm

# -------------------------------
# Generate synthetic time series data:
# -------------------------------
np.random.seed(42)
dates = pd.date_range(start='2020-01-01', periods=300, freq='D')

# Create data with trend and seasonality.
trend = np.linspace(20, 40, 300)
seasonality = 8 * np.sin(np.linspace(0, 4 * np.pi, 300))
noise = np.random.normal(0, 2, 300)
ts_values = trend + seasonality + noise
ts_data = pd.Series(ts_values, index=dates)

# -------------------------------
# Fit an ARIMA model:
# -------------------------------
# Here we use an ARIMA(p, d, q) model; for example, ARIMA(1,1,1)
model_arima = sm.tsa.arima.model.ARIMA(ts_data, order=(1, 1, 1))
results_arima = model_arima.fit()
print(results_arima.summary())

# -------------------------------
# Forecasting:
# -------------------------------
forecast_steps = 30
forecast_result = results_arima.get_forecast(steps=forecast_steps)
forecast_mean = forecast_result.predicted_mean
forecast_conf_int = forecast_result.conf_int()

# Plot the time series and forecast.
plt.figure(figsize=(10, 6))
plt.plot(ts_data, label='Observed', color='blue')
plt.plot(forecast_mean, label='Forecast', color='red', linestyle='--')
plt.fill_between(forecast_conf_int.index,
                 forecast_conf_int.iloc[:, 0],
                 forecast_conf_int.iloc[:, 1],
                 color='pink', alpha=0.3, label='Confidence Interval')
plt.xlabel("Date")
plt.ylabel("Value")
plt.title("ARIMA Forecast")
plt.legend()
plt.grid(True)
plt.show()

# -------------------------------
# Discussion:
# -------------------------------
# - The ARIMA model can capture trends and residual patterns.
# - Always perform residual analysis to validate model assumptions.
# - For seasonal data, consider using a SARIMA model which explicitly models seasonality.
```

---

## **Day 21: Multivariate Statistics & PCA**

### **Overview**

- **Principal Component Analysis (PCA)** is a dimensionality reduction technique.
- **Purpose & Use:**
  - Reduce the number of features while retaining most variance.
  - Visualize high-dimensional data in 2D or 3D.
- **Key Concept:**  
  - **Explained Variance:** Indicates how much information (variance) is captured by each principal component.

### **Example: PCA on the Iris Dataset**

Below is an example using scikit‑learn’s PCA applied to the Iris dataset (a well-known multivariate dataset).

```python:src/day21/pca_analysis.py
import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
from sklearn.decomposition import PCA
from sklearn.datasets import load_iris

# -------------------------------
# Load the Iris dataset:
# -------------------------------
iris = load_iris()
X = iris.data    # Features (sepal length, sepal width, petal length, petal width)
y = iris.target  # Class labels

# Standardize features (optional but recommended for PCA)
from sklearn.preprocessing import StandardScaler
scaler = StandardScaler()
X_scaled = scaler.fit_transform(X)

# -------------------------------
# Apply PCA:
# -------------------------------
pca = PCA(n_components=2)  # Reduce to 2 principal components for visualization.
principal_components = pca.fit_transform(X_scaled)
explained_variance = pca.explained_variance_ratio_
print(f"Explained variance by the 2 components: {explained_variance}")

# Create a DataFrame for the principal components.
pca_df = pd.DataFrame(data=principal_components, columns=['PC1', 'PC2'])
pca_df['Target'] = y

# -------------------------------
# Visualization: Scatter plot of the principal components:
# -------------------------------
plt.figure(figsize=(8, 6))
colors = ['red', 'green', 'blue']
for target, color in zip(np.unique(y), colors):
    indices = pca_df['Target'] == target
    plt.scatter(pca_df.loc[indices, 'PC1'], pca_df.loc[indices, 'PC2'],
                c=color, s=50, label=iris.target_names[target], edgecolor='k')
plt.xlabel("Principal Component 1")
plt.ylabel("Principal Component 2")
plt.title("PCA on Iris Dataset")
plt.legend()
plt.grid(True)
plt.show()

# -------------------------------
# Discussion:
# -------------------------------
# - PCA effectively reduces data dimensions while keeping most of the variance.
# - Visualizing the principal components helps understand clustering and groupings.
# - Examine the explained variance ratio to decide on the number of components to retain.
```

---

## **Day 22: Clustering and Unsupervised Methods**

### **Overview**

- **Clustering** aims to group similar data points together without prior labels.
- **Common Techniques:**
  - **K-Means Clustering:** Partitions data into k clusters.
  - **Hierarchical Clustering:** Builds nested clusters.
- **Evaluating Clusters:**
  - **Silhouette Score:** Measures how similar an object is to its own cluster versus other clusters.

### **Example: K-Means Clustering on the Iris Dataset**

Below is an example demonstrating K‑Means clustering on the Iris dataset and evaluating clusters with the silhouette score.

```python:src/day22/clustering_unsupervised.py
import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
from sklearn.cluster import KMeans
from sklearn.metrics import silhouette_score
from sklearn.datasets import load_iris
from sklearn.decomposition import PCA

# -------------------------------
# Load the Iris dataset:
# -------------------------------
iris = load_iris()
X = iris.data  # 4 features
y_true = iris.target  # Actual labels (for evaluation purposes)

# -------------------------------
# Apply K-Means Clustering:
# -------------------------------
k = 3  # We expect three clusters for Iris.
kmeans = KMeans(n_clusters=k, random_state=42)
clusters = kmeans.fit_predict(X)

# Evaluate using silhouette score.
silhouette_avg = silhouette_score(X, clusters)
print(f"Silhouette Score for K-Means clustering on Iris: {silhouette_avg:.3f}")

# -------------------------------
# Reduce dimensions for visualization using PCA:
# -------------------------------
pca = PCA(n_components=2)
X_pca = pca.fit_transform(X)

# Create a DataFrame for ease of plotting.
df_cluster = pd.DataFrame(X_pca, columns=['PC1', 'PC2'])
df_cluster['Cluster'] = clusters

# Visualization: Scatter plot with cluster coloring.
plt.figure(figsize=(8, 6))
colors = ['red', 'green', 'blue']
for cluster in range(k):
    indices = df_cluster['Cluster'] == cluster
    plt.scatter(df_cluster.loc[indices, 'PC1'], df_cluster.loc[indices, 'PC2'],
                c=colors[cluster], s=50, label=f'Cluster {cluster}', edgecolor='k')
plt.title("K-Means Clustering on Iris (PCA-reduced)")
plt.xlabel("Principal Component 1")
plt.ylabel("Principal Component 2")
plt.legend()
plt.grid(True)
plt.show()

# -------------------------------
# Discussion:
# -------------------------------
# - Clustering algorithms help group similar items when labels are not known.
# - K-Means is effective for spherical clusters but may struggle with irregular shapes.
# - Silhouette scores provide insight into the quality of clusters.
# - Visualizing clusters using PCA can help interpret the results.
```

---

## **Final Summary for Days 16-22**

- **Day 16:**  
  You explored evaluation metrics for both regression and classification with ample visualization, which are vital for selecting and validating models.

- **Day 17:**  
  Introduced Bayesian statistics using a coin toss example with a Beta prior to illustrate Bayesian updating.

- **Day 18:**  
  Delved into Bayesian inference with MCMC sampling using PyMC3 for linear regression, providing full posterior visualization and diagnostics.

- **Day 19:**  
  Covered time series fundamentals through decomposition, highlighting trend, seasonality, and residual analysis.

- **Day 20:**  
  Advanced time series modeling with ARIMA demonstrated forecasting power and the importance of residual checks.

- **Day 21:**  
  PCA was used to reduce dimensionality on the Iris dataset, allowing you to examine variance retention and visualize high-dimensional data.

- **Day 22:**  
  Concluded with unsupervised clustering (K-Means) and silhouette scoring with visualization via PCA, emphasizing how to interpret clustering results.

---
---
Below is a comprehensive, detailed guide covering Days 23 through 30. Each day’s section explains the key ideas, discusses when and why to use each method, and provides fully commented code examples with file path annotations. You can run these examples on your Windows machine using VSCode (or in Jupyter Notebook). The code blocks are labeled with file paths for clarity.

---

## **Day 23: Statistical Learning Theory Basics**

### **Overview**

- **Key Ideas:**  
  - **Bias–Variance Trade-Off:**  
    - *Bias* is the error introduced by approximating a real-life problem (possibly complex) with a simplified model.  
    - *Variance* measures how much the model’s predictions change for different training data.
  - **Overfitting vs. Underfitting:**  
    - An overfit model has low bias but high variance.
    - An underfit model has high bias and low variance.

### **Purpose & Use**

- Understand how model complexity affects prediction errors.
- Use simulation to illustrate that as model complexity increases, bias tends to decrease while variance increases.

### **Example: Bias–Variance Trade-Off Simulation**

```python:src/day23/bias_variance_tradeoff.py
import numpy as np
import matplotlib.pyplot as plt
from sklearn.linear_model import LinearRegression
from sklearn.preprocessing import PolynomialFeatures
from sklearn.metrics import mean_squared_error
from sklearn.model_selection import train_test_split, cross_val_score

# -------------------------------
# Simulated dataset: Sine function with noise.
# -------------------------------
np.random.seed(42)
n_samples = 100
X = np.linspace(0, 2 * np.pi, n_samples).reshape(-1, 1)
true_y = np.sin(X).ravel()
noise = np.random.normal(scale=0.2, size=n_samples)
y = true_y + noise

# Split data into training and test sets.
X_train, X_test, y_train, y_test = train_test_split(X, y, test_size=0.3, random_state=42)

# -------------------------------
# Evaluate models with different polynomial degrees.
# -------------------------------
degrees = [1, 3, 5, 7, 9]
train_errors = []
test_errors = []

for degree in degrees:
    # Transform features into polynomial features.
    poly = PolynomialFeatures(degree)
    X_train_poly = poly.fit_transform(X_train)
    X_test_poly = poly.transform(X_test)
    
    # Fit linear regression on polynomial features.
    model = LinearRegression()
    model.fit(X_train_poly, y_train)
    
    # Calculate training and test errors.
    y_train_pred = model.predict(X_train_poly)
    y_test_pred = model.predict(X_test_poly)
    train_errors.append(mean_squared_error(y_train, y_train_pred))
    test_errors.append(mean_squared_error(y_test, y_test_pred))

# -------------------------------
# Visualization: Plot errors vs. polynomial degree.
# -------------------------------
plt.figure(figsize=(10, 6))
plt.plot(degrees, train_errors, marker='o', label='Training Error', color='blue')
plt.plot(degrees, test_errors, marker='o', label='Test Error', color='red')
plt.xlabel("Polynomial Degree (Model Complexity)")
plt.ylabel("Mean Squared Error")
plt.title("Bias-Variance Trade-Off Simulation")
plt.legend()
plt.grid(True)
plt.show()

# -------------------------------
# Discussion:
# -------------------------------
# - Lower-degree models tend to underfit (high bias, low variance), leading to higher training error.
# - As the degree increases, training error decreases (low bias) but test error may increase due to overfitting (high variance).
```

*The simulation helps you visualize how bias and variance affect performance depending on model complexity.*

---

## **Day 24: Regularization Techniques**

### **Overview**

- **Regularization methods** help prevent overfitting by adding a penalty to the loss function.
- **Ridge Regression:** Penalizes the sum of squared coefficients.  
- **Lasso Regression:** Penalizes the sum of absolute coefficients (can drive some coefficients to zero).  
- **Elastic Net:** A combination of both.

### **Purpose & Use**

- Employ regularization to balance the trade-off between bias and variance.
- Useful when predictors are highly correlated or when feature selection is desired.

### **Example: Comparing Ridge, Lasso, and ElasticNet**

```python:src/day24/regularization_techniques.py
import numpy as np
import matplotlib.pyplot as plt
import pandas as pd
from sklearn.linear_model import Ridge, Lasso, ElasticNet
from sklearn.model_selection import train_test_split
from sklearn.metrics import mean_squared_error

# -------------------------------
# Generate synthetic data:
# -------------------------------
np.random.seed(42)
n_samples = 200
n_features = 10
X = np.random.randn(n_samples, n_features)
# True coefficients, with only a few nonzero values.
true_coef = np.array([5, 0, -3, 0, 2, 0, 0, 0, 0, 0])
noise = np.random.normal(0, 1, n_samples)
y = X @ true_coef + noise

# Split data into training and test sets.
X_train, X_test, y_train, y_test = train_test_split(X, y, test_size=0.3, random_state=42)

# -------------------------------
# Regularization models and hyperparameters.
# -------------------------------
alpha = 1.0  # Regularization strength
models = {
    'Ridge': Ridge(alpha=alpha),
    'Lasso': Lasso(alpha=alpha),
    'ElasticNet': ElasticNet(alpha=alpha, l1_ratio=0.5)
}

results = {}
for name, model in models.items():
    model.fit(X_train, y_train)
    y_pred = model.predict(X_test)
    mse = mean_squared_error(y_test, y_pred)
    results[name] = {
        'MSE': mse,
        'Coefficients': model.coef_
    }
    print(f"{name} Regression:\n MSE = {mse:.3f}\n Coefficients = {model.coef_}\n")

# -------------------------------
# Visualization: Coefficient comparison.
# -------------------------------
coef_df = pd.DataFrame({name: result['Coefficients'] for name, result in results.items()},
                       index=[f'Feature {i}' for i in range(1, n_features+1)])
coef_df.plot(kind='bar', figsize=(10, 6))
plt.title("Comparison of Coefficients across Regularization Methods")
plt.ylabel("Coefficient Value")
plt.xlabel("Features")
plt.grid(True)
plt.legend(title="Model")
plt.show()

# -------------------------------
# Discussion:
# -------------------------------
# - Ridge shrinks coefficients evenly.  
# - Lasso may set some coefficients to zero, lending itself to feature selection.  
# - ElasticNet combines benefits of both.
```

*This example demonstrates how different regularization methods affect the coefficient estimates and overall model performance.*

---

## **Day 25: Model Selection & Hyperparameter Tuning**

### **Overview**

- **Goal:** Identify the best model configuration and avoid overfitting.
- **Techniques:**  
  - **Cross-Validation:** Assess model performance on unseen data.
  - **Grid Search/Randomized Search:** Explore a range of hyperparameters.
- **Purpose:**  
  - Ensure that the chosen model generalizes well.

### **Example: Hyperparameter Tuning with GridSearchCV**

```python:src/day25/hyperparameter_tuning.py
import numpy as np
import matplotlib.pyplot as plt
from sklearn.datasets import make_regression
from sklearn.model_selection import train_test_split, GridSearchCV
from sklearn.linear_model import Ridge
from sklearn.metrics import mean_squared_error

# -------------------------------
# Generate synthetic regression dataset:
# -------------------------------
np.random.seed(42)
X, y = make_regression(n_samples=300, n_features=20, noise=10.0)

# Split data into training and test sets.
X_train, X_test, y_train, y_test = train_test_split(X, y, test_size=0.3, random_state=42)

# -------------------------------
# Set up the hyperparameter grid for Ridge Regression:
# -------------------------------
param_grid = {
    'alpha': np.logspace(-3, 3, 7)  # Testing values from 0.001 to 1000.
}

ridge = Ridge()
# Use GridSearchCV for exhaustive search.
grid_search = GridSearchCV(ridge, param_grid, cv=5, scoring='neg_mean_squared_error')
grid_search.fit(X_train, y_train)

# Display best hyperparameters.
best_alpha = grid_search.best_params_['alpha']
print(f"Best alpha value: {best_alpha}")

# Evaluate the best model on the test set.
best_model = grid_search.best_estimator_
y_pred = best_model.predict(X_test)
mse = mean_squared_error(y_test, y_pred)
print(f"Test MSE of best Ridge model: {mse:.3f}")

# -------------------------------
# Visualization: Plot MSE vs alpha values.
# -------------------------------
results = grid_search.cv_results_
alphas = param_grid['alpha']
mse_scores = -results['mean_test_score']  # Convert scores back to MSE

plt.figure(figsize=(8, 6))
plt.semilogx(alphas, mse_scores, marker='o', linestyle='-', color='purple')
plt.xlabel("Alpha")
plt.ylabel("Mean Squared Error")
plt.title("Grid Search Results for Ridge Regression")
plt.grid(True)
plt.show()

# -------------------------------
# Discussion:
# -------------------------------
# - Grid search helps identify the hyperparameter that minimizes validation error.
# - Cross-validation provides a better measure of model performance.
```

*This example sets up an end-to-end tuning pipeline for Ridge Regression and shows how to visualize the effects of the hyperparameter.*

---

## **Day 26: Special Topics in Advanced Statistics**

### **Overview**

- **Focus Areas:**  
  - **Feature Importance Assessment:** Understand which features contribute most to model predictions.
  - **Statistical Tests for Features:** Permutation tests and others can reveal the significance of predictors.
- **Purpose & Use:**  
  - Use techniques like permutation feature importance to verify the influence of variables in a predictive model.

### **Example: Permutation Feature Importance with scikit‑learn**

```python:src/day26/feature_importance.py
import numpy as np
import matplotlib.pyplot as plt
from sklearn.datasets import load_boston
from sklearn.linear_model import LinearRegression
from sklearn.model_selection import train_test_split
from sklearn.inspection import permutation_importance

# -------------------------------
# Load a regression dataset
# (Note: For a non-deprecated dataset, consider using California Housing instead)
# -------------------------------
from sklearn.datasets import fetch_california_housing
data = fetch_california_housing()
X = data.data
y = data.target
feature_names = data.feature_names

# Split the data.
X_train, X_test, y_train, y_test = train_test_split(X, y, test_size=0.3, random_state=42)

# Fit a linear regression model.
model = LinearRegression()
model.fit(X_train, y_train)

# Calculate permutation feature importance.
result = permutation_importance(model, X_test, y_test, n_repeats=10, random_state=42, scoring='neg_mean_squared_error')
importance_means = result.importances_mean

# -------------------------------
# Visualization: Bar plot of feature importances.
# -------------------------------
plt.figure(figsize=(10, 6))
indices = np.argsort(importance_means)
plt.barh(range(len(indices)), importance_means[indices], align='center', color='teal')
plt.yticks(range(len(indices)), [feature_names[i] for i in indices])
plt.xlabel("Decrease in Model Score (MSE)")
plt.title("Permutation Feature Importance")
plt.grid(True)
plt.show()

# -------------------------------
# Discussion:
# -------------------------------
# - Permutation feature importance helps identify how shuffling each feature affects performance.
# - It provides an alternative to model coefficients, especially when predictors are on different scales.
```

*Analyzing feature importance with permutation tests confirms which predictors are most influential.*

---

## **Day 27: Interpreting and Communicating Statistical Results**

### **Overview**

- **Key Elements:**  
  - **Reporting Results:** Summarize statistical findings (e.g., p-values, confidence intervals).
  - **Visualization:** Use clear plots (error bars, coefficient plots, boxplots) to convey findings.
  - **Communication:** Present results in a reproducible, understandable manner.
- **Purpose:**  
  - Ensure that stakeholders (technical or non-technical) can understand the analytical outcomes.

### **Example: Reporting Regression Results with Confidence Intervals**

```python:src/day27/interpreting_results.py
import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
import statsmodels.api as sm

# -------------------------------
# Generate synthetic data for regression:
# -------------------------------
np.random.seed(42)
n = 100
X = np.random.rand(n) * 10
true_intercept = 3.0
true_slope = 2.0
noise = np.random.normal(0, 2, n)
y = true_intercept + true_slope * X + noise

# Prepare data for statsmodels.
X_with_const = sm.add_constant(X)
model = sm.OLS(y, X_with_const)
results = model.fit()
print(results.summary())

# -------------------------------
# Visualization: Plot regression coefficients with confidence intervals.
# -------------------------------
coef_df = pd.DataFrame({
    'Coefficient': results.params,
    'CI Lower': results.conf_int()[0],
    'CI Upper': results.conf_int()[1]
})
coef_df = coef_df.reset_index().rename(columns={'index': 'Variable'})

plt.figure(figsize=(8, 6))
plt.errorbar(coef_df['Variable'], coef_df['Coefficient'],
             yerr=(coef_df['Coefficient'] - coef_df['CI Lower']), fmt='o', 
             capsize=5, color='navy')
plt.title("Regression Coefficients with 95% Confidence Intervals")
plt.xlabel("Variables")
plt.ylabel("Coefficient Value")
plt.grid(True)
plt.show()

# -------------------------------
# Discussion:
# -------------------------------
# - The statsmodels summary provides detailed inference (p-values, R-squared, etc.).
# - Visualizing confidence intervals helps stakeholders assess the reliability of estimates.
# - Clear reports and visualizations are essential for reproducible and transparent analysis.
```

*Here you combine statistical output with visualization to effectively communicate model insights.*

---

## **Day 28: Statistics for Fairness, Bias, & Interpretability in AI**

### **Overview**

- **Focus:**  
  - **Fairness and Bias Detection:** Statistical tests can identify whether models behave differently across groups.
  - **Interpretability Tools:** SHAP (SHapley Additive exPlanations) or LIME help explain model predictions.
- **Purpose & Use:**  
  - Ensure that AI models are ethically sound and their decisions are transparent.

### **Example: Using SHAP for Model Interpretability**

```python:src/day28/fairness_interpretability.py
import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
from sklearn.datasets import load_breast_cancer
from sklearn.linear_model import LogisticRegression
from sklearn.model_selection import train_test_split
import shap

# -------------------------------
# Load a binary classification dataset (Breast Cancer dataset):
# -------------------------------
data = load_breast_cancer()
X = pd.DataFrame(data.data, columns=data.feature_names)
y = data.target

# Split the data.
X_train, X_test, y_train, y_test = train_test_split(X, y, test_size=0.3, random_state=42)

# Train a logistic regression classifier.
clf = LogisticRegression(max_iter=10000)
clf.fit(X_train, y_train)

# -------------------------------
# Compute SHAP values:
# -------------------------------
# Create a TreeExplainer or KernelExplainer (here we use KernelExplainer for a linear model).
explainer = shap.KernelExplainer(clf.predict_proba, X_train.iloc[:50], link="logit")
shap_values = explainer.shap_values(X_test.iloc[:50])

# -------------------------------
# Visualization: SHAP summary plot for class 1.
# -------------------------------
shap.summary_plot(shap_values[1], X_test.iloc[:50], feature_names=data.feature_names, show=True)

# -------------------------------
# Discussion:
# -------------------------------
# - SHAP values quantify each feature’s contribution to the model prediction.
# - Such tools enable you to scrutinize model fairness and understand biases.
# - Transparent model interpretability is key when deploying AI in sensitive applications.
```

*This example shows how to apply SHAP to interpret a classification model’s predictions, fostering accountability in AI.*

---

## **Day 29: Integrating Statistics into End-to-End Machine Learning Pipelines**

### **Overview**

- **Objective:**  
  - Combine EDA, statistical testing, feature engineering, model building, and evaluation.
- **Workflow:**  
  1. **Data Exploration:** Summary statistics, visualization, and data cleaning.
  2. **Statistical Testing:** Hypothesis testing and confidence intervals.
  3. **Model Building:** Regression, classification, or other predictive modeling.
  4. **Evaluation:** Leverage cross-validation, error metrics, and diagnostic plots.
- **Purpose & Use:**  
  - Create reproducible, automated pipelines that integrate statistical rigor with predictive modeling.

### **Example: An End-to-End Pipeline**

```python:src/day29/end_to_end_pipeline.py
import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns
from sklearn.datasets import fetch_california_housing
from sklearn.model_selection import train_test_split, GridSearchCV
from sklearn.linear_model import LinearRegression
from sklearn.metrics import mean_squared_error, r2_score
import statsmodels.api as sm

# -------------------------------
# 1. Data Exploration (EDA):
# -------------------------------
# Load the dataset.
data = fetch_california_housing(as_frame=True)
df = data.frame
print("Dataset Information:")
print(df.info())
print("\nSummary Statistics:")
print(df.describe())

# Visualize correlation matrix.
plt.figure(figsize=(10, 8))
sns.heatmap(df.corr(), annot=True, cmap='coolwarm', fmt=".2f")
plt.title("Correlation Matrix")
plt.show()

# -------------------------------
# 2. Statistical Testing:
# -------------------------------
# Example: Test if the mean of the target variable differs significantly from a benchmark.
target_mean = df['MedHouseVal'].mean()
print(f"Mean of target (MedHouseVal): {target_mean:.3f}")
# (Further tests might be applied based on domain knowledge.)

# -------------------------------
# 3. Model Building:
# -------------------------------
# Split the dataset.
X = df.drop(columns=['MedHouseVal'])
y = df['MedHouseVal']
X_train, X_test, y_train, y_test = train_test_split(X, y, test_size=0.3, random_state=42)

# Fit a linear regression model.
model = LinearRegression()
model.fit(X_train, y_train)
y_pred = model.predict(X_test)

# -------------------------------
# 4. Evaluation:
# -------------------------------
mse = mean_squared_error(y_test, y_pred)
r2 = r2_score(y_test, y_pred)
print(f"Test MSE: {mse:.3f}")
print(f"Test R-squared: {r2:.3f}")

# Residual Plot.
plt.figure(figsize=(8, 6))
plt.scatter(y_pred, y_test - y_pred, alpha=0.5, color='navy')
plt.axhline(0, color='red', linestyle='--')
plt.xlabel("Predicted Values")
plt.ylabel("Residuals")
plt.title("Residual Plot")
plt.grid(True)
plt.show()

# -------------------------------
# Optional: Hyperparameter Tuning through Grid Search (if using a regularized model, for instance)
# -------------------------------
# (In a full pipeline, you could add a GridSearchCV over a model like Ridge or Lasso.)

# -------------------------------
# Discussion:
# -------------------------------
# - This pipeline demonstrates the integration of exploratory analysis, statistical validation, and predictive modeling.
# - Each module of the pipeline should have proper logging/reporting for reproducibility.
```

*This end-to-end example ties together statistical inference and machine learning into a cohesive, automated pipeline.*

---

## **Day 30: Capstone Project & Review**

### **Overview**

- **Capstone Project:**  
  - Choose a real-world dataset (public repositories, Kaggle, or UCI) and apply all concepts learned.
- **Project Phases:**  
  1. **Data Exploration & Cleaning:** Understand the data with EDA.
  2. **Statistical Inference:** Validate hypotheses with statistical tests.
  3. **Feature Engineering:** Transform and select features.
  4. **Model Building:** Apply one or more predictive models.
  5. **Model Evaluation:** Use cross-validation, error metrics, and diagnostic plots.
  6. **Interpretation & Communication:** Generate a report with visualizations and discuss findings.
- **Purpose & Use:**  
  - Demonstrate mastery of integrating theory, statistics, and machine learning into a reproducible project.

### **Example: Capstone Project Skeleton**

```python:src/day30/capstone_project.py
import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns
from sklearn.model_selection import train_test_split, GridSearchCV
from sklearn.linear_model import Ridge
from sklearn.metrics import mean_squared_error, r2_score
import statsmodels.api as sm

# -------------------------------
# 1. Data Loading & Cleaning:
# -------------------------------
# Replace with your chosen dataset path or URL.
df = pd.read_csv('data/your_dataset.csv')
print("Data Overview:")
print(df.head())
print(df.info())
print(df.describe())

# Data cleaning: handle missing values, outliers, and encoding.
df = df.dropna()  # Simplest method; apply more advanced techniques as needed.

# -------------------------------
# 2. Exploratory Data Analysis (EDA):
# -------------------------------
# Visualize distributions and relationships.
plt.figure(figsize=(10, 6))
sns.histplot(df['target_variable'], kde=True)
plt.title("Distribution of Target Variable")
plt.show()

plt.figure(figsize=(10, 8))
sns.heatmap(df.corr(), annot=True, cmap='viridis', fmt=".2f")
plt.title("Correlation Heatmap")
plt.show()

# -------------------------------
# 3. Statistical Inference:
# -------------------------------
# Example: Hypothesis test on whether the mean of a key feature is equal to a specific value.
from scipy import stats
feature_mean = df['important_feature'].mean()
t_stat, p_value = stats.ttest_1samp(df['important_feature'], popmean=50)
print(f"T-test result: t-statistic = {t_stat:.3f}, p-value = {p_value:.3f}")

# -------------------------------
# 4. Feature Engineering & Model Building:
# -------------------------------
# Define predictors and target.
X = df.drop(columns=['target_variable'])
y = df['target_variable']

# Split data.
X_train, X_test, y_train, y_test = train_test_split(X, y, test_size=0.3, random_state=42)

# Fit a regularized regression model (Ridge).
ridge = Ridge()
param_grid = {'alpha': np.logspace(-3, 3, 7)}
grid_search = GridSearchCV(ridge, param_grid, cv=5, scoring='neg_mean_squared_error')
grid_search.fit(X_train, y_train)
best_model = grid_search.best_estimator_

# -------------------------------
# 5. Evaluation:
# -------------------------------
y_pred = best_model.predict(X_test)
mse = mean_squared_error(y_test, y_pred)
r2 = r2_score(y_test, y_pred)
print(f"Best model Test MSE: {mse:.3f}")
print(f"Best model Test R-squared: {r2:.3f}")

# Plot residuals.
plt.figure(figsize=(8, 6))
plt.scatter(y_pred, y_test - y_pred, color='darkgreen', alpha=0.6)
plt.axhline(0, color='red', linestyle='--')
plt.xlabel("Predicted values")
plt.ylabel("Residuals")
plt.title("Residual Plot")
plt.show()

# -------------------------------
# 6. Report & Communication:
# -------------------------------
# Summarize the findings in a report.
results_summary = {
    'Test MSE': mse,
    'Test R-squared': r2,
    'Best Alpha': grid_search.best_params_['alpha']
}
print("Final Model Performance:")
print(results_summary)

# (Optionally, create a PDF/HTML report combining text and visualizations.)
# -------------------------------
# Discussion:
# -------------------------------
# - This capstone project consolidates the entire workflow from data ingestion to model evaluation.
# - The report should clearly explain what steps were taken, the rationale behind decisions, and the implications of the findings.
```

*In your capstone project, you are encouraged to document every step and discuss both successes and limitations. This project will serve as a practical demonstration of your integrated understanding of statistics and machine learning.*

---

## **Final Summary for Days 23-30**

- **Day 23:** Emphasized the bias–variance trade-off and statistical learning theory using simulation.
- **Day 24:** Showed how regularization (Ridge, Lasso, ElasticNet) helps mitigate overfitting.
- **Day 25:** Focused on model selection and hyperparameter tuning via cross-validation and grid search.
- **Day 26:** Explored special topics such as permutation feature importance to assess predictor influence.
- **Day 27:** Covered effective interpretation and communication of statistical results through visualization and clear summaries.
- **Day 28:** Introduced fairness, bias, and interpretability considerations in AI with tools like SHAP.
- **Day 29:** Integrated statistics into a complete machine learning pipeline from EDA to evaluation.
- **Day 30:** Reviewed all learned concepts in a capstone project that reinforces the end-to-end process.

---
---


