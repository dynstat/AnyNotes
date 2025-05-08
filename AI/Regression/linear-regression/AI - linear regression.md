
## Absolute basics

### o3-mini model

Below is an extended explanation that breaks down every step in the gradient descent algorithm for simple (single‐feature) linear regression—using house size to predict house price. I'll first explain the math behind gradient descent and then show the Python code with line‐by‐line comments describing what is happening.

─────────────────────────────  
Step 0. Overview of the Problem

We have a dataset of house sizes (in ft²) and their corresponding prices (in $):

  x: House size (ft²)  
  y: House price ($)

Our linear model is:  
  ŷ = m × x + b

where  
  m (slope, in $/ft²) shows how much the price increases per extra square foot, and  
  b (intercept, in $) is the baseline price.

─────────────────────────────  
Step 1. Our Cost Function

We use the Mean Squared Error (MSE) function to measure how far off our predictions are:

  J(m, b) = (1/(2n)) × Σᵢ (m × xᵢ + b − yᵢ)²

– n is the number of data points (unitless).  
– Inside the summation, (m×xᵢ + b − yᵢ) is the error for the iᵗʰ data point (with units $).  
– Squaring that error gives us dollars squared ($²).  
– The factor 1/(2n) is chosen so that when we take the derivative, the 2 cancels out. (This is a common trick to simplify the math.)

─────────────────────────────  
Step 2. Derivatives (Partial Derivatives)

To minimize J(m, b), we use gradient descent. We need to know how J changes with small changes in m and b. In other words, we need the partial derivatives ∂J/∂m and ∂J/∂b.

• Derivative with respect to m (slope):

  ∂J/∂m = (1/n) × Σᵢ [ (m×xᵢ + b − yᵢ) × xᵢ ]

Explanation:  
 – For each data point, (m×xᵢ + b − yᵢ) is the error.  
 – We multiply the error by xᵢ because m multiplies x in our model.  
 – Averaging over n gives us the final gradient for m.

• Derivative with respect to b (intercept):

  ∂J/∂b = (1/n) × Σᵢ (m×xᵢ + b − yᵢ)

Here, we simply sum the error over all data points and average it.

─────────────────────────────  
Step 3. The Gradient Descent Update Rule

Once we know the gradients, we update m and b iteratively:

  m := m − α × (∂J/∂m)  
  b := b − α × (∂J/∂b)

where α (alpha) is the learning rate (a small, unitless number that controls how big a step we take on each iteration).

A quick numerical example:  
Imagine a single training point with x = 1000 ft² and y = $200,000.  
Suppose initially m = 0 and b = 0, so our prediction is 0. The error is 0 − 200,000 = −200,000.  
Then the derivative with respect to m would be:  
  dJ/dm = (1/1) × (error × x) = (−200,000 × 1000) = −200,000,000  
With a learning rate of α, we update:  
  m_new = 0 − α × (−200,000,000) = α × 200,000,000  
A similar idea applies for b. (As you can see, choosing an appropriate α is very important to avoid overshooting.)

─────────────────────────────  
Step 4. Python Code (Detailed, with inline explanations)

Below is the complete Python code that implements this procedure. Each line is commented for clarity.

```python
import numpy as np
import matplotlib.pyplot as plt

# -----------------------------
# Data Preparation
# -----------------------------
# These are our house sizes in square feet (ft²) and their corresponding prices in dollars ($)
x_data = np.array([1000, 1500, 1800, 2000, 2200, 2500, 3000])
y_data = np.array([200000, 300000, 330000, 380000, 400000, 460000, 540000])

# For gradient descent, we normalize the data so that the algorithm converges faster.
# Normalization rescales the values such that they have a mean of 0 and standard deviation of 1.
x_mean = np.mean(x_data)      # Calculate average house size
x_std = np.std(x_data)        # Calculate standard deviation of house sizes
x_norm = (x_data - x_mean) / x_std  # Normalize house sizes

y_mean = np.mean(y_data)      # Calculate average price
y_std = np.std(y_data)        # Calculate standard deviation of prices
y_norm = (y_data - y_mean) / y_std  # Normalize prices

# -----------------------------
# Cost Function Definition
# -----------------------------
def compute_cost(x, y, m, b):
    """
    Compute the Mean Squared Error (MSE) between predicted and actual prices.
    
    Parameters:
      x (array): Normalized house sizes
      y (array): Normalized house prices
      m (float): Current slope value (parameter for house size)
      b (float): Current intercept value (base price)
     
    Returns:
      cost (float): Computed cost based on MSE.
      
    Calculation:
      - prediction for each house is given by: prediction = m * x + b
      - error for each prediction: error = (m * x + b - y)
      - cost: average of the squared errors scaled by 1/(2*n) to simplify gradients.
    """
    n = len(x)  # Number of data points (samples)
    predictions = m * x + b    # Vector of predicted values for each house
    squared_errors = (predictions - y) ** 2  # Squared difference for each data point
    cost = (1/(2*n)) * np.sum(squared_errors)   # Mean Squared Error (MSE)
    return cost

# -----------------------------
# Gradient Descent Function
# -----------------------------
def gradient_descent(x, y, m_init, b_init, learning_rate, iterations):
    """
    Perform gradient descent to minimize the cost function and find the best m and b.
    
    Parameters:
      x (array): Normalized house sizes
      y (array): Normalized house prices
      m_init (float): Initial guess for slope
      b_init (float): Initial guess for intercept
      learning_rate (float): The step size (alpha) for each update
      iterations (int): Number of iterations to run the updates
      
    Returns:
      m (float): Optimized slope after gradient descent
      b (float): Optimized intercept after gradient descent
      costs (list): Cost computed every 100 iterations (for plotting convergence)
      
    The algorithm:
      1. Compute predictions:  prediction = m*x + b.
      2. Calculate error: error = prediction - y.
      3. Compute gradients:
            dJ/dm = (1/n) * Σ((prediction - y) * x)
            dJ/db = (1/n) * Σ(prediction - y)
         These represent the direction and magnitude needed to change m and b 
         to reduce the cost.
      4. Update m and b using:
            m = m - learning_rate * (dJ/dm)
            b = b - learning_rate * (dJ/db)
      5. Repeat for a given number of iterations.
    """
    m = m_init  # Initialize slope with initial guess
    b = b_init  # Initialize intercept with initial guess
    n = len(x)  # Number of training examples
    costs = []  # This list will store the cost at every 100th iteration

    for i in range(iterations):
        predictions = m * x + b  # Step 1: Compute predictions for current m and b
        errors = predictions - y  # Step 2: Compute error vector (difference between predicted and actual values)

        # Step 3: Calculate gradients (partial derivatives of cost function)
        # For m: Multiply each error by its corresponding x value then average.
        d_m = (1/n) * np.sum(errors * x)
        # For b: Average of the errors (since derivative with respect to b has no x multiplier).
        d_b = (1/n) * np.sum(errors)

        # Step 4: Update current parameters by stepping in the opposite direction of the gradient.
        # This brings us to a lower value of the cost function.
        m = m - learning_rate * d_m
        b = b - learning_rate * d_b

        # Every 100 iterations, compute and store the cost to track the progress.
        if i % 100 == 0:
            cost = compute_cost(x, y, m, b)
            costs.append(cost)
    
    return m, b, costs

# -----------------------------
# Running Gradient Descent
# -----------------------------
iterations = 10000
learning_rate = 0.1
initial_m = 0
initial_b = 0

# Use normalized data for gradient descent to improve convergence.
final_m_norm, final_b_norm, costs = gradient_descent(x_norm, y_norm, initial_m, initial_b, learning_rate, iterations)

# Convert the optimized parameters back to the original (non-normalized) scale.
# For a normalized model: y_norm = (m_norm)*x_norm + b_norm, 
# we recover the original parameters using the scaling factors.
final_m = final_m_norm * (y_std / x_std)
final_b = (final_b_norm * y_std) + y_mean - (final_m_norm * x_mean * y_std / x_std)

# Print the final optimized parameters.
print("Optimal parameters:")
print("m (price increase per square foot): ${:.2f}/ft²".format(final_m))
print("b (base price): ${:.2f}".format(final_b))

# -----------------------------
# Prediction and Visualization
# -----------------------------
def predict_price(size_sqft):
    """Return the predicted house price given a house size in ft²."""
    return final_m * size_sqft + final_b

# Predict for some test house sizes.
test_sizes = [1200, 2300, 3500]
for size in test_sizes:
    predicted_price = predict_price(size)
    print("A {} ft² house is predicted to cost: ${:.2f}".format(size, predicted_price))

# Plotting the regression line with the original data.
plt.figure(figsize=(15, 10))

# Subplot 1: Actual Data and Regression Line
plt.subplot(2, 1, 1)
plt.scatter(x_data, y_data, color='blue', label='Actual house prices')
plt.plot(x_data, final_m * x_data + final_b, color='red', label='Regression line')
plt.title('House Price vs. Size')
plt.xlabel('House Size (ft²)')
plt.ylabel('Price ($)')
plt.legend()
plt.grid(True)

# Mark test predictions on the graph.
for size in test_sizes:
    price = predict_price(size)
    plt.plot([size, size], [0, price], 'k--', alpha=0.3)
    plt.plot([0, size], [price, price], 'k--', alpha=0.3)
    plt.scatter(size, price, color='green')
    plt.annotate('${:.0f}'.format(price), (size, price), xytext=(10, 10), textcoords='offset points')

# Subplot 2: Cost function history (shows convergence)
plt.subplot(2, 1, 2)
plt.plot(range(0, iterations, 100), costs)
plt.title('Cost Function Over Iterations')
plt.xlabel('Iterations (×100)')
plt.ylabel('Cost (MSE)')
plt.grid(True)

plt.tight_layout()
plt.savefig('house_price_regression.png')
plt.show()
```

─────────────────────────────  
Step 5. Detailed Walkthrough of the Gradient Descent Loop

Inside the loop inside `gradient_descent`:

1. `predictions = m * x + b`  
  • For every data point in the normalized list `x`, we compute the predicted value by multiplying by the slope `m` and adding the intercept `b`.

2. `errors = predictions - y`  
  • We calculate how far off each prediction is from the actual normalized price (`y`).  
  • Example: If prediction = 0.5 and actual y = 0.3 → error = 0.2.

3. `d_m = (1/n) * sum(errors * x)`  
  • We multiply each error by its associated normalized feature value. This tells us how much each error "weighs" with respect to `m`.  
  • Summing these and dividing by `n` gives the average slope adjustment needed.

4. `d_b = (1/n) * sum(errors)`  
  • For the intercept, we simply average the errors (since the derivative of (m*x + b) with respect to b is just 1).

5. Parameter updates:  
  • `m = m - learning_rate * d_m`  
  • `b = b - learning_rate * d_b`  
  • Here we move our parameters in the opposite direction of the gradient (i.e., negative gradient) to reduce the cost.
  • The learning rate controls the size of these steps: too high and you risk overshooting the minimum; too low and convergence is very slow.

6. Every 100 iterations, we compute the cost using `compute_cost(x, y, m, b)`. This cost history helps us monitor whether the algorithm is converging (i.e., the cost should decrease over iterations).

─────────────────────────────  
In Summary

- We start with an initial guess (m = 0, b = 0) and iteratively update based on the gradients of our cost function.
- The cost function (MSE) measures the error between predictions and actual values.
- The partial derivatives with respect to m and b (computed using the chain rule) tell us how to adjust our parameters so as to reduce the error.
- Repeating these updates moves us toward the best-fitting line according to the least squares criterion.

By following this detailed breakdown, you can see how each piece of the gradient descent algorithm works to "learn" the best parameters for our linear model—all using basic math and Python without any specialized ML libraries.



---
### Using sonnet 3.7
# Linear Regression Explained from Scratch: House Price Prediction

Let's build a linear regression model to predict house prices based on square footage, explaining every mathematical concept along the way.

## Basic Concept

Linear regression finds a line (or plane in higher dimensions) that best fits your data. In our house example:
- Input (x): House size in square feet (ft²)
- Output (y): House price in dollars ($)
- Goal: Find the relationship y = mx + b where:
  - m = slope ($/ft²) - how much each additional square foot adds to the price
  - b = y-intercept ($) - the base price regardless of size

## Dataset Example

```python
# House sizes (ft²)
x_data = [1000, 1500, 1800, 2000, 2200, 2500, 3000]

# Corresponding prices ($)
y_data = [200000, 300000, 330000, 380000, 400000, 460000, 540000]
```

## Mathematical Intuition

### Step 1: Understanding the Linear Model

Our model is: y = mx + b

For a house of 1500 ft²:
- If $$ m = 150 ($/ft²) , 
   b = 50000 ($) $$
- Price = 150 × 1500 + 50000 = $275,000

### Step 2: The Cost Function (Mean Squared Error)

To find the best m and b, we need to measure how wrong our predictions are using the cost function:

J(m,b) = (1/2n) × Σ(ŷᵢ - yᵢ)²

Where:
- n = number of data points
- ŷᵢ (predicted value) = m×xᵢ + b
- yᵢ = actual value
- Units: $ squared (because we're squaring the difference in price)

### Step 3: Gradient Descent

We'll find the optimal m and b by minimizing J(m,b):

```
For each iteration:
    ∂J/∂m = (1/n) × Σ[(m×xᵢ + b - yᵢ) × xᵢ]
    ∂J/∂b = (1/n) × Σ[(m×xᵢ + b - yᵢ)]
    
    m = m - α × ∂J/∂m
    b = b - α × ∂J/∂b
```

Where:
- α = learning rate (unitless)
- ∂J/∂m = gradient with respect to m ($/ft²)
- ∂J/∂b = gradient with respect to b ($)

## Python Implementation

```python
import numpy as np
import matplotlib.pyplot as plt

# House size (ft²) and price ($) data
x_data = np.array([1000, 1500, 1800, 2000, 2200, 2500, 3000])
y_data = np.array([200000, 300000, 330000, 380000, 400000, 460000, 540000])

# Normalize data (important for gradient descent)
# This helps gradient descent converge faster by making all features on similar scales
x_mean = np.mean(x_data)  # Average house size
x_std = np.std(x_data)    # Standard deviation of house sizes
x_norm = (x_data - x_mean) / x_std  # Normalized x values

y_mean = np.mean(y_data)  # Average price
y_std = np.std(y_data)    # Standard deviation of prices
y_norm = (y_data - y_mean) / y_std  # Normalized y values

def compute_cost(x, y, m, b):
    """
    Compute the cost function (Mean Squared Error)
    
    Parameters:
    x (array): Input features (house sizes)
    y (array): Actual values (house prices)
    m (float): Slope parameter
    b (float): Intercept parameter
    
    Returns:
    float: The cost (average squared error)
    """
    n = len(x)  # Number of training examples
    
    # Calculate predictions (y_hat) using current m and b
    predictions = m * x + b
    
    # Calculate squared differences between predictions and actual values
    squared_errors = (predictions - y) ** 2
    
    # Calculate mean squared error (divided by 2 to make derivatives cleaner)
    cost = (1/(2*n)) * np.sum(squared_errors)
    
    return cost

def gradient_descent(x, y, m_init, b_init, learning_rate, iterations):
    """
    Perform gradient descent to minimize the cost function
    
    Parameters:
    x (array): Input features (house sizes)
    y (array): Actual values (house prices)
    m_init (float): Initial slope value
    b_init (float): Initial intercept value
    learning_rate (float): How quickly to adjust parameters
    iterations (int): Number of times to update parameters
    
    Returns:
    m (float): Optimized slope parameter
    b (float): Optimized intercept parameter
    costs (list): Cost at each iteration for plotting
    """
    m = m_init  # Initial slope
    b = b_init  # Initial intercept
    n = len(x)  # Number of training examples
    costs = []   # To store cost at each iteration
    
    for i in range(iterations):
        # Calculate predictions with current parameters
        predictions = m * x + b
        
        # Calculate prediction errors
        errors = predictions - y
        
        # Calculate gradients (partial derivatives)
        # ∂J/∂m = (1/n) × Σ[(m×xi + b - yi) × xi]
        d_m = (1/n) * np.sum(errors * x)
        
        # ∂J/∂b = (1/n) × Σ[(m×xi + b - yi)]
        d_b = (1/n) * np.sum(errors)
        
        # Update parameters using gradients and learning rate
        m = m - learning_rate * d_m
        b = b - learning_rate * d_b
        
        # Calculate and store cost for this iteration
        if i % 100 == 0:
            cost = compute_cost(x, y, m, b)
            costs.append(cost)
            
    return m, b, costs

# Run gradient descent (on normalized data)
iterations = 10000
learning_rate = 0.1
initial_m = 0
initial_b = 0

# Find optimal m and b
final_m_norm, final_b_norm, costs = gradient_descent(
    x_norm, y_norm, initial_m, initial_b, learning_rate, iterations
)

# Convert the parameters back to original scale
# For a normalized model y_norm = m_norm * x_norm + b_norm
# We need to convert it back to y = m * x + b
final_m = final_m_norm * (y_std / x_std)
final_b = (final_b_norm * y_std) + y_mean - (final_m_norm * x_mean * y_std / x_std)

print(f"Optimal parameters:")
print(f"m (price increase per square foot): ${final_m:.2f}/ft²")
print(f"b (base price): ${final_b:.2f}")

# Function to predict house price based on size
def predict_price(size_sqft):
    """Predict house price based on size"""
    return final_m * size_sqft + final_b

# Make predictions for various house sizes
test_sizes = [1200, 2300, 3500]
for size in test_sizes:
    predicted_price = predict_price(size)
    print(f"A {size} ft² house is predicted to cost: ${predicted_price:.2f}")

# Visualization
plt.figure(figsize=(15, 10))

# Plot 1: Data points and regression line
plt.subplot(2, 1, 1)
plt.scatter(x_data, y_data, color='blue', label='Actual house prices')
plt.plot(x_data, final_m * x_data + final_b, color='red', label='Regression line')
plt.title('House Price vs. Size')
plt.xlabel('House Size (ft²)')
plt.ylabel('Price ($)')
plt.legend()
plt.grid(True)

# Add price predictions for specific sizes
for size in test_sizes:
    price = predict_price(size)
    plt.plot([size, size], [0, price], 'k--', alpha=0.3)
    plt.plot([0, size], [price, price], 'k--', alpha=0.3)
    plt.scatter(size, price, color='green')
    plt.annotate(f'${price:.0f}', (size, price), xytext=(10, 10), 
                 textcoords='offset points')

# Plot 2: Cost history
plt.subplot(2, 1, 2)
plt.plot(range(0, iterations, 100), costs)
plt.title('Cost Function Over Iterations')
plt.xlabel('Iterations (×100)')
plt.ylabel('Cost (MSE)')
plt.grid(True)

plt.tight_layout()
plt.savefig('house_price_regression.png')
plt.show()
```

## Cost Function Derivation (Step-by-Step)

1. **Starting with Mean Squared Error (MSE):**
   J(m,b) = (1/2n) × Σ(ŷᵢ - yᵢ)²
   
   Where:
   - J(m,b) is the cost function (units: $²)
   - n is the number of samples (unitless)
   - ŷᵢ = m×xᵢ + b is our prediction (units: $)
   - yᵢ is the actual price (units: $)

2. **Expanding the prediction:**
   J(m,b) = (1/2n) × Σ(m×xᵢ + b - yᵢ)²

3. **Finding partial derivative with respect to m:**
   
   Apply the chain rule:
   ∂J/∂m = (1/2n) × Σ[2 × (m×xᵢ + b - yᵢ) × xᵢ]
   
   Simplify:
   ∂J/∂m = (1/n) × Σ[(m×xᵢ + b - yᵢ) × xᵢ]
   
   Units: ($×ft²)/ft² = $

4. **Finding partial derivative with respect to b:**
   
   Apply the chain rule:
   ∂J/∂b = (1/2n) × Σ[2 × (m×xᵢ + b - yᵢ) × 1]
   
   Simplify:
   ∂J/∂b = (1/n) × Σ(m×xᵢ + b - yᵢ)
   
   Units: $

5. **Gradient Descent Update Rules:**
   
   m = m - α × ∂J/∂m
   b = b - α × ∂J/∂b
   
   Where α is the learning rate (unitless)

## Why Use MSE as Cost Function?

1. **Squaring errors** makes all differences positive and penalizes larger errors more heavily
2. It's **differentiable** which makes optimization possible
3. It has a **single global minimum** for linear regression
4. It has a **geometric interpretation**: finding the line that minimizes the sum of squared vertical distances

## Interpretation of Results

- Slope (m) represents the price increase per additional square foot
- Intercept (b) is the theoretical base price for a zero-square-foot house (mathematical construct)
- A good model minimizes the MSE, making the line as close as possible to all data points

This implementation allows you to understand all the mathematical foundations that modern machine learning libraries abstract away, giving you a solid foundation for understanding more complex algorithms.



---
---

# Multiple Linear Regression: Handling Multiple House Features

Yes, this absolutely comes under linear regression! When we have multiple parameters affecting house prices, it becomes **multiple linear regression** (or multivariate linear regression). It's still "linear" because the relationship between each input and the output remains linear.

## From Single to Multiple Variables

Our housing model extends from:

```
price = m × size + b
```

To:

```
price = m₁ × size + m₂ × rooms + m₃ × age + m₄ × distance_to_station + b
```

Generally written as:
```
y = m₁x₁ + m₂x₂ + ... + mₙxₙ + b
```

Where:
- y is the house price ($)
- Each xᵢ is a different feature (with different units)
- Each mᵢ is a coefficient that shows how that feature affects price
- b is still the y-intercept ($)

## Mathematical Formulation

For convenience, we often rewrite with vector notation:
```
y = Xθ
```

Where:
- X is a matrix of inputs (with first column all 1s for the intercept)
- θ is a vector of parameters [b, m₁, m₂, ...]

## Updated Cost Function

The cost function remains Mean Squared Error, but now with multiple inputs:

```
J(θ) = (1/2n) × Σ(hθ(x⁽ⁱ⁾) - y⁽ⁱ⁾)²
```

Where:
- hθ(x⁽ⁱ⁾) = m₁x₁⁽ⁱ⁾ + m₂x₂⁽ⁱ⁾ + ... + mₙxₙ⁽ⁱ⁾ + b (prediction for data point i)

## Python Implementation with Multiple Features

```python
import numpy as np
import matplotlib.pyplot as plt

# House data with multiple features
# [size_sqft, bedrooms, age_years, distance_to_station_miles]
X_data = np.array([
    [1000, 2, 10, 0.5],
    [1500, 3, 5, 1.0],
    [1800, 3, 15, 0.2],
    [2000, 4, 8, 1.5],
    [2200, 4, 2, 0.8],
    [2500, 4, 20, 0.6],
    [3000, 5, 3, 0.3]
])

# House prices ($)
y_data = np.array([200000, 300000, 330000, 380000, 400000, 460000, 540000])

# Feature names for plotting
feature_names = ['Size (ft²)', 'Bedrooms', 'Age (years)', 'Distance to station (miles)']

# Normalize features
X_mean = np.mean(X_data, axis=0)
X_std = np.std(X_data, axis=0)
X_norm = (X_data - X_mean) / X_std

# Normalize target
y_mean = np.mean(y_data)
y_std = np.std(y_data)
y_norm = (y_data - y_mean) / y_std

# Add intercept term (column of 1s)
X_norm_with_intercept = np.column_stack((np.ones(X_norm.shape[0]), X_norm))

def compute_cost(X, y, theta):
    """
    Compute Mean Squared Error cost
    
    Parameters:
    X (array): Input features with intercept term added
    y (array): Actual values
    theta (array): Parameters including intercept and coefficients
    
    Returns:
    float: Cost (MSE)
    """
    m = len(y)  # Number of training examples
    predictions = X.dot(theta)  # Matrix multiplication: X × θ = predictions
    squared_errors = (predictions - y) ** 2
    return (1/(2*m)) * np.sum(squared_errors)

def gradient_descent(X, y, theta, alpha, iterations):
    """
    Gradient descent for multiple variables
    
    Parameters:
    X (array): Input features (with intercept column)
    y (array): Actual values
    theta (array): Initial parameters
    alpha (float): Learning rate
    iterations (int): Number of updates
    
    Returns:
    theta (array): Optimized parameters
    costs (list): Cost history
    """
    m = len(y)  # Number of examples
    n = len(theta)  # Number of features + 1 (for intercept)
    costs = []
    
    for i in range(iterations):
        # Calculate predictions
        predictions = X.dot(theta)
        
        # Calculate errors
        errors = predictions - y
        
        # Update all parameters simultaneously
        # θⱼ = θⱼ - α × (1/m) × Σ[(hθ(x⁽ⁱ⁾) - y⁽ⁱ⁾) × x⁽ⁱ⁾ⱼ]
        gradient = (1/m) * X.T.dot(errors)
        theta = theta - alpha * gradient
        
        # Track cost
        if i % 100 == 0:
            cost = compute_cost(X, y, theta)
            costs.append(cost)
            
    return theta, costs

# Initialize parameters (θ) with zeros
initial_theta = np.zeros(X_norm_with_intercept.shape[1])

# Run gradient descent
iterations = 10000
learning_rate = 0.1
final_theta_norm, costs = gradient_descent(
    X_norm_with_intercept, y_norm, initial_theta, learning_rate, iterations
)

# Convert parameters back to original scale
# For intercept: b = b_norm*y_std + y_mean - Σ(m_j*X_mean_j*y_std/X_std_j)
# For coefficients: m_j = m_j_norm * y_std / X_std_j
intercept = final_theta_norm[0] * y_std + y_mean
coefficients = np.zeros(len(feature_names))

for j in range(len(coefficients)):
    coefficients[j] = final_theta_norm[j+1] * (y_std / X_std[j])
    intercept -= coefficients[j] * X_mean[j]

# Print results
print("Multiple Linear Regression Model for House Pricing:")
print(f"Intercept (base price): ${intercept:.2f}")
for j, name in enumerate(feature_names):
    print(f"Coefficient for {name}: ${coefficients[j]:.2f}")

# Function to predict house price with multiple features
def predict_price(features):
    """Predict house price based on multiple features"""
    # Make sure features have the same ordering as our training data
    return np.dot(coefficients, features) + intercept

# Example predictions
test_houses = [
    [1800, 3, 5, 0.5],   # 1800 ft², 3 bedrooms, 5 years old, 0.5 miles to station
    [2500, 4, 10, 1.0]    # 2500 ft², 4 bedrooms, 10 years old, 1 mile to station
]

for house in test_houses:
    price = predict_price(house)
    print(f"\nHouse with:")
    for i, feat in enumerate(feature_names):
        print(f"  - {feat}: {house[i]}")
    print(f"Predicted price: ${price:.2f}")

# Visualize feature importance
plt.figure(figsize=(10, 6))
plt.bar(feature_names, np.abs(coefficients) / np.sum(np.abs(coefficients)) * 100)
plt.title('Feature Importance in House Price Prediction')
plt.xlabel('Features')
plt.ylabel('Relative Importance (%)')
plt.xticks(rotation=45)
plt.tight_layout()
plt.show()
```

## Key Differences with Multiple Variables

1. **More Parameters:** Instead of just m and b, we have a coefficient for each feature plus the intercept.

2. **Feature Scaling:** Different features have different units and ranges. Normalizing becomes even more important:
   - Size: 1000-3000 ft²
   - Bedrooms: 2-5 (small range)
   - Age: 2-20 years
   - Distance: 0.2-1.5 miles

3. **Interpretation:** Each coefficient tells you how much the house price changes when that feature increases by one unit, holding other features constant.
   - e.g., $120/ft² means each additional square foot adds $120 to the price
   - -$5000/year of age means each additional year reduces price by $5000

4. **Matrix Operations:** We use matrix multiplication to efficiently compute predictions.

5. **Gradient Computation:** We update all parameters simultaneously using vector operations.

## Is it Still Linear Regression?

Yes! It's still linear regression because:

1. Each parameter enters the equation linearly (no squares, exponentials, etc.)
2. The model is a linear combination of parameters
3. The relationship is represented as a hyperplane in n-dimensional space (instead of a line)

Even with 100 features, it's still linear regression as long as the model maintains the form:
```
y = θ₀ + θ₁x₁ + θ₂x₂ + ... + θₙxₙ
```

The beauty of linear regression is that it easily extends to multiple variables while keeping the same fundamental principles and algorithm structure.



---


Below is an example of how to implement linear regression completely from scratch using only built‑in Python functions and basic mathematics. In this example, we compute the slope and y‑intercept using the least squares method, and we also include a prediction function for new data. Each section and even many individual lines include detailed comments on what is happening and why.

Below is the full code with detailed explanations:

```python
"""
Linear Regression from Scratch in Python

This script demonstrates how to implement linear regression using the least squares method,
without relying on external libraries. The goal is to find a line defined by y = m*x + b
that best fits a given set of data points.

The formulas used are:
    slope (m) = (n * Σ(x*y) - Σx * Σy) / (n * Σ(x²) - (Σx)²)
    intercept (b) = (Σy/n) - m*(Σx/n)

where:
    n is the number of data points,
    Σ denotes the sum over all data points.
    
We include detailed comments for each line to explain the process.
"""

def linear_regression(x, y):
    # Check that the lists for x and y are of the same length.
    # This is important because we need one y value for each x value.
    n = len(x)
    if n != len(y):
        raise ValueError("The lists x and y must have the same length.")
    
    # Initialize variables to hold sums that are required for the calculations.
    sum_x = 0            # Sum of all x values
    sum_y = 0            # Sum of all y values
    sum_xy = 0           # Sum of the product of each x and y pair
    sum_x_squared = 0    # Sum of squares of x values
    
    # Iterate over each index in the data set.
    # This loop calculates the sums needed for the regression equations.
    for i in range(n):
        xi = x[i]      # Current x value
        yi = y[i]      # Corresponding y value
        sum_x += xi                  # Update the cumulative sum of x values
        sum_y += yi                  # Update the cumulative sum of y values
        sum_xy += xi * yi            # Update the sum of products x*y
        sum_x_squared += xi * xi     # Update the sum of squares of x
    
    # Calculate the numerator and denominator for the slope (m) formula.
    numerator = n * sum_xy - sum_x * sum_y
    denominator = n * sum_x_squared - sum_x * sum_x
    
    # Before computing the slope, ensure the denominator is not zero.
    # A zero denominator means that all the x values are identical (vertical line) and the slope cannot be computed.
    if denominator == 0:
        raise ValueError("Cannot calculate slope because the denominator is zero (all x values may be identical).")
    
    # Calculate the slope (m) by dividing the numerator by the denominator.
    m = numerator / denominator
    
    # Calculate the mean (average) values of x and y.
    mean_x = sum_x / n
    mean_y = sum_y / n
    
    # Calculate the y-intercept (b) using the formula: intercept = mean(y) - m * mean(x)
    b = mean_y - m * mean_x
    
    # Return the calculated slope and intercept.
    return m, b

def predict(x_value, m, b):
    """
    Given a single x value, this function uses the linear regression model (y = m*x + b)
    to predict the corresponding y value.
    """
    return m * x_value + b  # Multiply x by slope and then add the intercept

if __name__ == '__main__':
    # Here we set up a simple example dataset.
    # Suppose that our data follows the relationship: y = 2*x + 1
    # We prepare a few sample data points for x and their corresponding y values.
    x_data = [1, 2, 3, 4, 5]           # List of x values
    y_data = [3, 5, 7, 9, 11]           # List of y values following the pattern 2*x+1
    
    # Compute the regression parameters (slope and intercept) using our function.
    slope, intercept = linear_regression(x_data, y_data)
    
    # Print out the computed slope and y-intercept.
    print("Calculated Linear Regression Parameters:")
    print("Slope (m) =", slope)
    print("Intercept (b) =", intercept)
    
    # Now we demonstrate making predictions.
    # We will predict the y value for several new x values using our model.
    test_x_values = [6, 7, 8]
    print("\nPredictions:")
    for x_val in test_x_values:
        predicted_y = predict(x_val, slope, intercept)
        # Output the predicted y value for the given x value.
        print(f"For x = {x_val}, predicted y = {predicted_y}")
    
    # Explanation:
    # With the expected relationship of y = 2*x + 1, we would obtain:
    #   For x = 6  -> y = 2*6 + 1 = 13
    #   For x = 7  -> y = 2*7 + 1 = 15
    #   For x = 8  -> y = 2*8 + 1 = 17
```

Explanation of the Code:
1. The function `linear_regression(x, y)` takes two lists of numbers, one for x-values and another for y-values. It computes necessary sums over the inputs and uses the least squares formula to determine the slope (`m`) and intercept (`b`).

2. A check is in place to ensure that both lists have equal lengths because misaligned data would yield incorrect calculations.

3. The code computes the sums for **Σx, Σy, Σ(x * y), and Σ(x²)** in a single loop over the data points.

4. The slope (`m`) is then calculated using the formula:
$$
   m = \frac{n\sum xy - \sum x \cdot \sum y}{n\sum x^2 - (\sum x)^2}
$$
   It checks the denominator to prevent division by zero.

5. The intercept (`b`) is computed as the difference between the mean of y values and the product of the slope and the mean of x values:
   $$
   b = \text{mean}(y) - m \times \text{mean}(x)
   $$

6. The function `predict(x_value, m, b)` uses the model **(y = m * x + b)** to compute the predicted y for any new x.

7. In the main section (within the `if __name__ == '__main__':` block), a sample dataset is defined, the regression is computed, and predictions are made for new x values with the results printed to the console.

This example should help illustrate the inner workings of linear regression, and the code is constructed entirely using Python’s built‑in capabilities.


---
---


Okay, let's break down Simple Linear Regression step-by-step and implement it from scratch using only standard Python functions and mathematical operations.

**What is Linear Regression?**

Linear regression is a fundamental statistical and machine learning technique used to model the relationship between a dependent variable (the one we want to predict, often denoted as 'Y') and one or more independent variables (the factors used to make the prediction, often denoted as 'X').

* **Simple Linear Regression:** Involves only *one* independent variable (X). We try to find a straight line that best fits the data points (X, Y).
* **Multiple Linear Regression:** Involves *two or more* independent variables. (We'll focus on Simple Linear Regression here as it's easier to implement from scratch).

**The Goal of Simple Linear Regression**

The goal is to find the equation of a straight line that best represents the relationship between X and Y. The equation of a straight line is:

`Y = mX + c`

Where:
* `Y`: The predicted value of the dependent variable.
* `X`: The value of the independent variable.
* `m`: The slope (or coefficient) of the line. It represents how much Y changes for a one-unit increase in X.
* `c`: The y-intercept (or constant) of the line. It's the predicted value of Y when X is 0.

**How do we find the "Best Fit" Line?**

The "best fit" line is the one that minimizes the difference between the actual Y values in our dataset and the Y values predicted by our line (`Y_pred = mX + c`). This difference is called the *error* or *residual*.

We specifically aim to minimize the **Sum of Squared Errors (SSE)**, also known as the Residual Sum of Squares (RSS).

`SSE = Σ (Y_actual - Y_predicted)² = Σ (Y_actual - (mX + c))²`

We use the square of the errors so that:
1.  Positive and negative errors don't cancel each other out.
2.  Larger errors are penalized more heavily.

**Calculating the Coefficients (m and c)**

Using calculus (specifically, finding the partial derivatives of the SSE with respect to 'm' and 'c', setting them to zero, and solving the resulting equations), we can derive formulas to directly calculate the 'm' and 'c' that minimize the SSE:

1.  **Calculate the mean (average) of X and Y:**
    * `mean(X) = Σ X / n`
    * `mean(Y) = Σ Y / n`
    (where 'n' is the number of data points)

2.  **Calculate the slope (m):**
    * `m = Σ [ (Xᵢ - mean(X)) * (Yᵢ - mean(Y)) ] / Σ [ (Xᵢ - mean(X))² ]`
    * The numerator is related to the covariance between X and Y.
    * The denominator is related to the variance of X.

3.  **Calculate the y-intercept (c):**
    * `c = mean(Y) - m * mean(X)`

Once we have 'm' and 'c', we have our regression line!

**Implementation from Scratch in Python**

Let's implement this using only basic Python operations and lists.

```python
# -*- coding: utf-8 -*-
"""
Simple Linear Regression Implementation from Scratch.

This script demonstrates how to perform simple linear regression analysis
without using any external machine learning or numerical computation libraries
like scikit-learn, numpy, or pandas. It relies solely on standard Python
data structures (lists) and basic mathematical operations.
"""

# -------------------------------------
# Helper Functions
# -------------------------------------

def calculate_mean(values):
  """
  Calculates the mean (average) of a list of numbers.

  Args:
    values: A list of numerical values.

  Returns:
    The mean of the values, or None if the list is empty.
  """
  # Check if the list is empty to avoid division by zero
  if not values:
    print("Error: Cannot calculate mean of an empty list.")
    return None
  # Calculate the sum of all values in the list
  total_sum = sum(values)
  # Calculate the number of values in the list
  count = len(values)
  # Return the mean (sum divided by count)
  return total_sum / float(count) # Use float() for potential decimal results

def calculate_coefficients(x_values, y_values):
  """
  Calculates the coefficients (slope 'm' and intercept 'c') for the
  linear regression line using the least squares method.

  Args:
    x_values: A list of independent variable values (X).
    y_values: A list of dependent variable values (Y).
              Must be the same length as x_values.

  Returns:
    A tuple containing (slope 'm', intercept 'c'), or (None, None) if
    input lists are invalid (e.g., different lengths, empty, or contain
    non-numeric data - basic checks included).
  """
  # --- Input Validation ---
  # Check if lists have the same number of elements
  if len(x_values) != len(y_values):
    print("Error: Input lists (x_values, y_values) must have the same length.")
    return None, None
  # Check if lists are empty
  if not x_values: # Checking one list is sufficient due to the length check above
    print("Error: Input lists cannot be empty.")
    return None, None
  # Basic check for numeric types (can be enhanced for more robustness)
  # This checks the first element, assuming homogeneous lists for simplicity
  if not isinstance(x_values[0], (int, float)) or not isinstance(y_values[0], (int, float)):
      print("Warning: Input lists should contain numeric values (int or float).")
      # Proceeding, but results might be unexpected if lists contain mixed types.

  # --- Calculate Means ---
  # Calculate the mean of the independent variable (X)
  x_mean = calculate_mean(x_values)
  # Calculate the mean of the dependent variable (Y)
  y_mean = calculate_mean(y_values)

  # Check if mean calculation failed (e.g., due to empty lists caught earlier)
  if x_mean is None or y_mean is None:
      return None, None # Error already printed by calculate_mean

  # --- Calculate Slope 'm' ---
  # Initialize numerator and denominator for the slope formula
  numerator = 0.0
  denominator = 0.0
  # Get the number of data points
  n = len(x_values)

  # Iterate through each data point (x, y)
  for i in range(n):
    # Current x value
    xi = x_values[i]
    # Current y value
    yi = y_values[i]

    # Add the term (xi - x_mean) * (yi - y_mean) to the numerator
    numerator += (xi - x_mean) * (yi - y_mean)

    # Add the term (xi - x_mean)^2 to the denominator
    denominator += (xi - x_mean) ** 2

  # Check for division by zero: This happens if all x_values are the same.
  if denominator == 0:
    print("Error: Cannot calculate slope 'm' because all x_values are the same (denominator is zero).")
    # In this case, the best fit line is vertical if x values are constant
    # and y values vary, or horizontal if y values are also constant.
    # Returning None as a standard slope isn't meaningful here.
    # A more advanced implementation might return infinity or handle it differently.
    return None, None

  # Calculate the slope 'm'
  m = numerator / denominator

  # --- Calculate Intercept 'c' ---
  # Calculate the y-intercept 'c' using the formula: c = y_mean - m * x_mean
  c = y_mean - m * x_mean

  # Return the calculated slope and intercept
  return m, c

def predict(x_new, m, c):
  """
  Predicts the Y value for a given X value using the calculated
  linear regression coefficients.

  Args:
    x_new: The new independent variable value (X) for which to predict Y.
    m: The slope of the regression line.
    c: The y-intercept of the regression line.

  Returns:
    The predicted dependent variable value (Y_pred). Returns None if
    coefficients 'm' or 'c' are None.
  """
  # Check if coefficients are valid
  if m is None or c is None:
      print("Error: Cannot predict because coefficients (m, c) are invalid or not calculated.")
      return None
  # Calculate the predicted Y using the line equation: Y = mX + c
  y_pred = m * x_new + c
  # Return the prediction
  return y_pred

# -------------------------------------
# Example Usage
# -------------------------------------

# Sample Dataset (e.g., hours studied vs. exam score)
# Independent variable X: Hours Studied
X = [1, 2, 3, 4, 5, 6, 7, 8, 9, 10]
# Dependent variable Y: Exam Score (out of 100)
Y = [30, 35, 45, 50, 60, 65, 75, 80, 85, 90]

# --- Step 1: Calculate the coefficients ---
print("Calculating regression coefficients...")
# Call the function to get the slope (m) and intercept (c)
slope_m, intercept_c = calculate_coefficients(X, Y)

# --- Step 2: Display the results ---
# Check if the coefficients were calculated successfully
if slope_m is not None and intercept_c is not None:
  # Print the calculated slope
  print(f"Calculated Slope (m): {slope_m:.4f}") # Format to 4 decimal places
  # Print the calculated intercept
  print(f"Calculated Intercept (c): {intercept_c:.4f}") # Format to 4 decimal places
  # Print the final regression equation
  print(f"Regression Line Equation: Y = {slope_m:.4f} * X + {intercept_c:.4f}")

  # --- Step 3: Make Predictions ---
  # Example: Predict the score for someone who studied for 5.5 hours
  hours_studied_new = 5.5
  # Use the predict function with the new X value and calculated coefficients
  predicted_score = predict(hours_studied_new, slope_m, intercept_c)

  # Check if prediction was successful
  if predicted_score is not None:
    # Print the prediction
    print(f"\nPrediction for X = {hours_studied_new} hours:")
    print(f"Predicted Score (Y_pred): {predicted_score:.4f}")

  # Example: Predict the score for someone who studied for 12 hours
  hours_studied_new_2 = 12
  predicted_score_2 = predict(hours_studied_new_2, slope_m, intercept_c)
  if predicted_score_2 is not None:
      print(f"\nPrediction for X = {hours_studied_new_2} hours:")
      print(f"Predicted Score (Y_pred): {predicted_score_2:.4f}")

else:
  # Print a message if coefficient calculation failed
  print("\nCould not calculate regression coefficients due to input errors.")

# --- Optional: Evaluating the Model (Simple Example: Calculating SSE) ---
if slope_m is not None and intercept_c is not None:
    sse = 0.0
    print("\nCalculating Sum of Squared Errors (SSE)...")
    # Iterate through the original data points
    for i in range(len(X)):
        # Get the actual Y value
        y_actual = Y[i]
        # Get the corresponding X value
        x_val = X[i]
        # Predict the Y value using the regression line
        y_predicted = predict(x_val, slope_m, intercept_c)
        # Calculate the squared error for this point
        error_squared = (y_actual - y_predicted) ** 2
        # Add it to the total SSE
        sse += error_squared

    # Print the calculated SSE
    print(f"Sum of Squared Errors (SSE) on the training data: {sse:.4f}")
    # Note: Lower SSE generally indicates a better fit to the training data.
    # Other metrics like R-squared or RMSE provide more context but require
    # a bit more calculation (like calculating Total Sum of Squares (SST)).
```

**Explanation of the Code:**

1.  **`calculate_mean(values)`:**
    * Takes a list of numbers (`values`).
    * Calculates the sum using `sum()`.
    * Calculates the count using `len()`.
    * Returns `sum / count`. Includes a check for an empty list to prevent `ZeroDivisionError`.

2.  **`calculate_coefficients(x_values, y_values)`:**
    * Takes the lists of X and Y values.
    * Performs basic validation (checks for equal length, non-empty lists).
    * Calls `calculate_mean()` for both `x_values` and `y_values`.
    * Initializes `numerator` and `denominator` for the slope calculation to 0.0.
    * Iterates through the data points using a `for` loop and `range(len(x_values))`.
    * Inside the loop:
        * Calculates `(xi - x_mean) * (yi - y_mean)` and adds it to `numerator`.
        * Calculates `(xi - x_mean) ** 2` and adds it to `denominator`.
    * Calculates the slope `m = numerator / denominator`. Includes a check for `denominator == 0` (which happens if all X values are identical).
    * Calculates the intercept `c = y_mean - m * x_mean`.
    * Returns the tuple `(m, c)`.

3.  **`predict(x_new, m, c)`:**
    * Takes a new X value (`x_new`) and the calculated slope (`m`) and intercept (`c`).
    * Checks if `m` and `c` are valid (not `None`).
    * Calculates the predicted Y using the formula: `y_pred = m * x_new + c`.
    * Returns the `y_pred`.

4.  **Example Usage Section:**
    * Defines sample lists `X` and `Y`.
    * Calls `calculate_coefficients()` to get `m` and `c`.
    * Prints the calculated `m`, `c`, and the resulting line equation.
    * Demonstrates making predictions using `predict()` for new X values (5.5 and 12).
    * Includes an optional section to calculate the Sum of Squared Errors (SSE) on the original data to give a basic idea of the model's fit to the data it was trained on.

This implementation provides a clear view of the mathematical steps involved in simple linear regression without relying on any specialized libraries.

---
---
## Another simple example of linear regression with multiple variables (features)
For multiple features (like house size, number of rooms, age), we'd need to implement multivariate linear regression. Here's how the implementation would change:

### From Scratch Implementation

Here's the complete implementation including the conversion of normalized coefficients back to their original scale:

```python
import numpy as np
import matplotlib.pyplot as plt

# Sample data with multiple features: [size, rooms, age]
X_data = np.array([
    [1000, 3, 15],
    [1500, 3, 10],
    [1800, 4, 5],
    [2000, 4, 7],
    [2200, 5, 3],
    [2500, 6, 2],
    [3000, 7, 1]
])
y_data = np.array([200000, 300000, 330000, 380000, 400000, 460000, 540000])

# Normalize all features
X_mean = np.mean(X_data, axis=0)
X_std = np.std(X_data, axis=0)
X_norm = (X_data - X_mean) / X_std

y_mean = np.mean(y_data)
y_std = np.std(y_data)
y_norm = (y_data - y_mean) / y_std

def compute_cost(X, y, theta):
    """Compute cost for multivariate linear regression"""
    m = len(y)
    predictions = X.dot(theta)
    squared_errors = (predictions - y) ** 2
    return (1/(2*m)) * np.sum(squared_errors)

def gradient_descent(X, y, theta, learning_rate, iterations):
    """Gradient descent for multivariate linear regression"""
    m = len(y)
    n = X.shape[1]  # Number of features
    costs = []
    
    for i in range(iterations):
        predictions = X.dot(theta)
        errors = predictions - y
        
        # Update all parameters simultaneously
        gradients = (1/m) * X.T.dot(errors)
        theta = theta - learning_rate * gradients
        
        if i % 100 == 0:
            cost = compute_cost(X, y, theta)
            costs.append(cost)
            
    return theta, costs

# Add bias term (ones column)
X_norm_with_bias = np.column_stack((np.ones(X_norm.shape[0]), X_norm))

# Initialize theta with zeros
initial_theta = np.zeros(X_norm_with_bias.shape[1])

# Run gradient descent
iterations = 10000
learning_rate = 0.1
final_theta_norm, costs = gradient_descent(X_norm_with_bias, y_norm, initial_theta, learning_rate, iterations)

# Convert normalized theta back to original scale
# For the normalized model: y_norm = theta0 + theta1*x1_norm + theta2*x2_norm + theta3*x3_norm
# where x1_norm = (x1 - x1_mean)/x1_std
# Substituting and rearranging to get y = a0 + a1*x1 + a2*x2 + a3*x3

# First, calculate the coefficient for each feature
original_coefs = np.zeros(len(final_theta_norm) - 1)  # Excluding the bias term
for i in range(1, len(final_theta_norm)):
    original_coefs[i-1] = final_theta_norm[i] * (y_std / X_std[i-1])

# Then calculate the intercept
intercept = y_mean
for i in range(len(original_coefs)):
    intercept -= original_coefs[i] * X_mean[i]

# Add the normalized bias term contribution
intercept += final_theta_norm[0] * y_std

# Now we have the equation: y = intercept + coef1*x1 + coef2*x2 + coef3*x3
print("Model in original scale:")
print(f"Intercept: ${intercept:.2f}")
print(f"Coefficient for house size: ${original_coefs[0]:.2f} per square foot")
print(f"Coefficient for number of rooms: ${original_coefs[1]:.2f} per additional room")
print(f"Coefficient for house age: ${original_coefs[2]:.2f} per year of age")

# Interpretation: 
print("\nInterpretation:")
print(f"- For each additional square foot, house price increases by ${original_coefs[0]:.2f}")
print(f"- For each additional room, house price increases by ${original_coefs[1]:.2f}")
if original_coefs[2] < 0:
    print(f"- For each additional year of age, house price decreases by ${-original_coefs[2]:.2f}")
else:
    print(f"- For each additional year of age, house price increases by ${original_coefs[2]:.2f}")

# Function to predict price using original coefficients
def predict_price(features):
    return intercept + np.dot(features, original_coefs)

# Test predictions
test_houses = [
    [1200, 3, 12],  # 1200 sq ft, 3 rooms, 12 years old
    [2300, 5, 5],   # 2300 sq ft, 5 rooms, 5 years old
    [3500, 8, 1]    # 3500 sq ft, 8 rooms, 1 year old
]

for house in test_houses:
    predicted_price = predict_price(np.array(house))
    print(f"A {house[0]} ft² house with {house[1]} rooms and {house[2]} years old is predicted to cost: ${predicted_price:.2f}")

# Visualize the coefficients
plt.figure(figsize=(10, 6))
features = ['Size (sq ft)', 'Rooms', 'Age (years)']
plt.bar(features, original_coefs)
plt.title('Feature Importance (Coefficient Values)')
plt.xlabel('Features')
plt.ylabel('Impact on Price ($)')
plt.grid(axis='y')
plt.savefig('feature_importance.png')
plt.show()

# Visualize cost history
plt.figure(figsize=(10, 6))
plt.plot(range(0, iterations, 100), costs)
plt.title('Cost Function Over Iterations')
plt.xlabel('Iterations (×100)')
plt.ylabel('Cost (MSE)')
plt.grid(True)
plt.savefig('cost_history.png')
plt.show()
```

### Explanation of Coefficient Conversion

The conversion from normalized coefficients to original scale is complex but essential for interpretation. Here's what's happening:

1. **Original equation (before normalization)**: 
   ```
   y = β₀ + β₁x₁ + β₂x₂ + β₃x₃
   ```

2. **Normalized equation**: 
   ```
   y_norm = θ₀ + θ₁x₁_norm + θ₂x₂_norm + θ₃x₃_norm
   ```
   where:
   ```
   y_norm = (y - y_mean) / y_std
   x_i_norm = (x_i - x_i_mean) / x_i_std
   ```

3. **Conversion math**:
   - Substituting normalized variables into the equation:
   ```
   (y - y_mean) / y_std = θ₀ + θ₁(x₁ - x₁_mean)/x₁_std + θ₂(x₂ - x₂_mean)/x₂_std + θ₃(x₃ - x₃_mean)/x₃_std
   ```
   
   - Multiplying both sides by y_std:
   ```
   y - y_mean = θ₀*y_std + (θ₁*y_std/x₁_std)*(x₁ - x₁_mean) + (θ₂*y_std/x₂_std)*(x₂ - x₂_mean) + (θ₃*y_std/x₃_std)*(x₃ - x₃_mean)
   ```
   
   - Rearranging to standard form:
   ```
   y = [y_mean + θ₀*y_std - (θ₁*y_std/x₁_std)*x₁_mean - (θ₂*y_std/x₂_std)*x₂_mean - (θ₃*y_std/x₃_std)*x₃_mean] + 
       (θ₁*y_std/x₁_std)*x₁ + (θ₂*y_std/x₂_std)*x₂ + (θ₃*y_std/x₃_std)*x₃
   ```

4. **Resulting coefficients**:
   - β₀ (intercept) = y_mean + θ₀*y_std - Σᵢ(θᵢ*y_std/xᵢ_std)*xᵢ_mean
   - βᵢ (feature coefficient) = θᵢ*y_std/xᵢ_std

This conversion preserves the exact same predictions but makes the coefficients directly interpretable in the original units (e.g., dollars per square foot).


### Scikit-learn Implementation

```python
import numpy as np
import matplotlib.pyplot as plt
from sklearn.linear_model import LinearRegression
from sklearn.preprocessing import StandardScaler
from sklearn.model_selection import train_test_split
from sklearn.metrics import mean_squared_error, r2_score

# Sample data with multiple features: [size, rooms, age]
X = np.array([
    [1000, 3, 15],
    [1500, 3, 10],
    [1800, 4, 5],
    [2000, 4, 7],
    [2200, 5, 3], 
    [2500, 6, 2],
    [3000, 7, 1]
])
y = np.array([200000, 300000, 330000, 380000, 400000, 460000, 540000])

# Split data
X_train, X_test, y_train, y_test = train_test_split(X, y, test_size=0.2, random_state=42)

# Standardize features
scaler = StandardScaler()
X_train_scaled = scaler.fit_transform(X_train)
X_test_scaled = scaler.transform(X_test)

# Create and train the model
model = LinearRegression()
model.fit(X_train_scaled, y_train)

# Evaluate the model
y_pred = model.predict(X_test_scaled)
mse = mean_squared_error(y_test, y_pred)
r2 = r2_score(y_test, y_pred)

print(f"Model performance:")
print(f"Mean Squared Error: {mse:.2f}")
print(f"R² Score: {r2:.4f}")

# Feature importance
feature_names = ['Size (sq ft)', 'Rooms', 'Age (years)']
coefficients = model.coef_
for feature, coef in zip(feature_names, coefficients):
    print(f"Impact of {feature}: ${coef:.2f} per unit change")

# Function to predict price
def predict_house_price(features):
    # Scale the features
    features_scaled = scaler.transform([features])
    # Make prediction
    return model.predict(features_scaled)[0]

# Test predictions
test_houses = [
    [1200, 3, 12],  # 1200 sq ft, 3 rooms, 12 years old
    [2300, 5, 5],   # 2300 sq ft, 5 rooms, 5 years old
    [3500, 8, 1]    # 3500 sq ft, 8 rooms, 1 year old
]

for house in test_houses:
    predicted_price = predict_house_price(house)
    print(f"A {house[0]} ft² house with {house[1]} rooms and {house[2]} years old is predicted to cost: ${predicted_price:.2f}")

# Visualize coefficients
plt.figure(figsize=(10, 6))
plt.bar(feature_names, model.coef_)
plt.title('Feature Importance (Coefficient Values)')
plt.xlabel('Features')
plt.ylabel('Impact on Price ($)')
plt.grid(axis='y')
plt.savefig('feature_importance.png')
plt.show()
```




### Key Differences:

1. **Data representation**: From scratch, we need to handle matrices instead of vectors.

2. **Gradient Descent**: The update step now uses matrix operations to update all parameters at once.

3. **Parameter Interpretation**: In multivariate regression, each coefficient tells us how much the price changes when that feature increases by 1 unit, holding other features constant.

4. **Scikit-learn simplifies**:
   - Feature scaling with StandardScaler
   - Model training with a single fit() method
   - Built-in train-test split functionality
   - Ready-to-use evaluation metrics
   - No need to manually implement gradient descent

5. **Extensibility**: Scikit-learn models can easily handle additional features without code changes.

---
