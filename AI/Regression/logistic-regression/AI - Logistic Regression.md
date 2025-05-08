
---
## Purpose of Logistic Regression: Core Goals and Steps

### The Ultimate Goal
The fundamental purpose of logistic regression is to **model the probability of a binary outcome** (like yes/no, 0/1) based on one or more predictor variables.

### Is Sigmoid Always Used?
**Yes, for binary logistic regression**, the sigmoid function is always used as the final transformation. That's what makes it logistic regression.

For multi-class problems:
- In **multinomial logistic regression**, we use the softmax function instead
- In **ordinal logistic regression**, we use multiple thresholds on a single linear predictor

## Why Different Optimization Approaches?

Each optimization method (Gradient Descent, Newton's Method, etc.) exists to solve the same problem: finding the best coefficients (β values) that maximize the likelihood of observing our training data. They differ in:

1. **Computational Efficiency**: Newton's Method typically needs fewer iterations but each iteration is more computationally intensive
2. **Memory Requirements**: Gradient Descent needs less memory for large datasets
3. **Convergence Properties**: Newton's Method converges quadratically (very fast) when near the solution
4. **Practical Considerations**: Some methods work better with regularization, sparse data, or in distributed computing

## Step-by-Step Purpose of Each Phase

1. **Linear Combination**: z = β₀ + β₁x₁ + ... + βₙxₙ
   - **Purpose**: Create a weighted sum of features that can range from -∞ to +∞

2. **Sigmoid Transformation**: p = 1/(1+e^(-z))
   - **Purpose**: Convert the unbounded linear score to a probability between 0 and 1

3. **Log-Likelihood Calculation**: ℓ(β) = Σ[y·log(p) + (1-y)·log(1-p)]
   - **Purpose**: Measure how well our current model explains the observed data

4. **Optimization** (Gradient Descent or Newton's Method):
   - **Purpose**: Find the coefficients that make our predictions most closely match the observed outcomes

5. **Decision Rule**: If p ≥ 0.5, predict class 1; otherwise predict class 0
   - **Purpose**: Convert probabilities to actual classifications when needed

---

---

## Overview

## Breaking Down Logistic Regression Components

### What is z?
**z** is the **linear predictor** or **log-odds** - a weighted sum of all input features.

**Formula**: z = β₀ + β₁x₁ + β₂x₂ + ... + βₙxₙ

### What is x?
**x** represents your **input features** (independent variables) - the data you use to make predictions.

### What are coefficients?
**Coefficients** (β values) are the **weights** assigned to each feature that determine how strongly each feature influences the prediction.

### Sample Example: Medical Diagnosis

Let's predict whether a patient has diabetes (1) or not (0) based on:
- x₁: Age (years)
- x₂: BMI (kg/m²)

#### Step 1: Collect Data
```
Patient | Age (x₁) | BMI (x₂) | Has Diabetes (y)
--------|----------|----------|----------------
   1    |    45    |   29.5   |       1
   2    |    35    |   24.2   |       0
   3    |    52    |   32.1   |       1
   4    |    30    |   22.0   |       0
   5    |    62    |   28.7   |       1
```

#### Step 2: Train Model and Find Coefficients
After training, we might get:
- β₀ = -5.2 (intercept)
- β₁ = 0.08 (coefficient for Age)
- β₂ = 0.12 (coefficient for BMI)

#### Step 3: Calculate z for New Patient
For a new 50-year-old patient with BMI 30:
```
z = -5.2 + 0.08(50) + 0.12(30)
z = -5.2 + 4.0 + 3.6
z = 2.4
```

#### Step 4: Convert to Probability
```
p = 1/(1+e^(-z))
p = 1/(1+e^(-2.4))
p = 0.917 (91.7% chance of diabetes)
```

### Coefficient Interpretation
- β₁ = 0.08 means: Each additional year of age increases the log-odds of diabetes by 0.08
- In probability terms: When age increases by 1 year, the odds of diabetes multiply by e^0.08 = 1.083 (increase by 8.3%)

### Another Example: Email Spam Classification
Predicting if an email is spam (1) or not (0) based on:
- x₁: Contains "free" (0=no, 1=yes)
- x₂: Contains "meeting" (0=no, 1=yes)
- x₃: Sender is known (0=no, 1=yes)

With coefficients:
- β₀ = -1.5
- β₁ = 2.1 (strong positive effect - "free" suggests spam)
- β₂ = -1.8 (negative effect - "meeting" suggests not spam)
- β₃ = -2.4 (strong negative effect - known senders less likely spam)

For an email with "free" but no "meeting" from unknown sender:
```
z = -1.5 + 2.1(1) + (-1.8)(0) + (-2.4)(0)
z = -1.5 + 2.1
z = 0.6

p = 1/(1+e^(-0.6))
p = 0.646 (64.6% chance of being spam)
```

The coefficients reveal that "free" is a spam indicator, while "meeting" and known senders reduce spam probability.

___
___
___


Logistic regression implementation from scratch, along with explanations of the mathematics behind it.

```python
# Logistic Regression from Scratch
# with mathematical explanations and derivations

import numpy as np
import matplotlib.pyplot as plt
import seaborn as sns

# Set a random seed for reproducibility
np.random.seed(42)
```

## Mathematical Foundation of Logistic Regression

Logistic regression is a binary classification algorithm that predicts the probability of an instance belonging to a particular class. Unlike linear regression which predicts continuous values, logistic regression predicts probabilities that are constrained between 0 and 1.

### The Logistic Function (Sigmoid)

The key difference between linear and logistic regression is the use of the sigmoid function:

```python
def sigmoid(z):
    """
    Sigmoid activation function: f(z) = 1 / (1 + e^(-z))
    
    Parameters:
        z (float or array): Input value(s)
    
    Returns:
        float or array: Sigmoid output, which is always between 0 and 1
    """
    return 1 / (1 + np.exp(-z))

# Visualize the sigmoid function
z = np.linspace(-10, 10, 100)
sigmoid_values = sigmoid(z)

plt.figure(figsize=(10, 6))
plt.plot(z, sigmoid_values)
plt.axhline(y=0.5, color='r', linestyle='--', alpha=0.3)
plt.axvline(x=0, color='r', linestyle='--', alpha=0.3)
plt.title("Sigmoid Function")
plt.xlabel("z")
plt.ylabel("sigmoid(z)")
plt.grid(True)
plt.show()
```

### From Linear Regression to Logistic Regression

In linear regression, we had:
- $y_{pred} = mx + b$ (for a single feature)
- or more generally, $y_{pred} = w^T x + b$ for multiple features

In logistic regression, we pass this linear combination through the sigmoid function:
- $P(y=1|x) = \sigma(w^T x + b) = \frac{1}{1 + e^{-(w^T x + b)}}$

Where:
- $P(y=1|x)$ is the probability that the instance belongs to class 1
- $\sigma$ is the sigmoid function
- $w$ is the weight vector
- $b$ is the bias term

### The Cost Function

For linear regression, we used Mean Squared Error as the cost function. However, for logistic regression, we use the "Log Loss" or "Binary Cross-Entropy":

For a single training example:
- If $y=1$: $J(w,b) = -\log(P(y=1|x))$
- If $y=0$: $J(w,b) = -\log(1-P(y=1|x))$

These can be combined into a single formula:
- $J(w,b) = -[y\log(P(y=1|x)) + (1-y)\log(1-P(y=1|x))]$

For the entire training set:
- $J(w,b) = -\frac{1}{m}\sum_{i=1}^{m}[y^{(i)}\log(P(y=1|x^{(i)})) + (1-y^{(i)})\log(1-P(y=1|x^{(i)}))]$

Where $m$ is the number of training examples.

```python
def compute_cost(X, y, w, b):
    """
    Compute the cost function for logistic regression
    
    Parameters:
        X (ndarray): Input features, shape (n_samples, n_features)
        y (ndarray): Target labels, shape (n_samples,)
        w (ndarray): Model weights, shape (n_features,)
        b (float): Model bias
        
    Returns:
        float: Cost value
    """
    m = X.shape[0]  # number of examples
    
    # Calculate predicted probabilities
    z = np.dot(X, w) + b
    predicted_proba = sigmoid(z)
    
    # Calculate cost using binary cross-entropy
    cost = -1/m * np.sum(y * np.log(predicted_proba) + (1-y) * np.log(1-predicted_proba))
    
    return cost
```

### Gradient Descent for Logistic Regression

To find the optimal values of $w$ and $b$, we use gradient descent.

The derivatives of the cost function with respect to the parameters are:

- $\frac{\partial J}{\partial w_j} = \frac{1}{m}\sum_{i=1}^{m}(P(y=1|x^{(i)}) - y^{(i)})x_j^{(i)}$
- $\frac{\partial J}{\partial b} = \frac{1}{m}\sum_{i=1}^{m}(P(y=1|x^{(i)}) - y^{(i)})$

Where $x_j^{(i)}$ is the j-th feature of the i-th training example.

```python
def compute_gradients(X, y, w, b):
    """
    Compute the gradients of the cost function
    
    Parameters:
        X (ndarray): Input features, shape (n_samples, n_features)
        y (ndarray): Target labels, shape (n_samples,)
        w (ndarray): Model weights, shape (n_features,)
        b (float): Model bias
        
    Returns:
        tuple: (dw, db) where dw is gradient with respect to w and db is gradient with respect to b
    """
    m, n = X.shape  # m is the number of examples, n is number of features
    
    # Predicted probabilities
    z = np.dot(X, w) + b
    predicted_proba = sigmoid(z)
    
    # Calculate the error
    error = predicted_proba - y
    
    # Calculate gradients
    dw = 1/m * np.dot(X.T, error)  # shape (n_features,)
    db = 1/m * np.sum(error)       # single value
    
    return dw, db
```

### The Gradient Descent Algorithm

The gradient descent update rules are:

- $w_j := w_j - \alpha \frac{\partial J}{\partial w_j}$
- $b := b - \alpha \frac{\partial J}{\partial b}$

Where $\alpha$ is the learning rate.

```python
def gradient_descent(X, y, w_init, b_init, alpha, num_iterations, print_every=100):
    """
    Perform gradient descent to optimize the parameters
    
    Parameters:
        X (ndarray): Input features, shape (n_samples, n_features)
        y (ndarray): Target labels, shape (n_samples,)
        w_init (ndarray): Initial weights, shape (n_features,)
        b_init (float): Initial bias
        alpha (float): Learning rate
        num_iterations (int): Number of iterations to run
        print_every (int): How often to print the cost
        
    Returns:
        tuple: (w, b, costs) optimized weights, bias, and the cost history
    """
    # Initialize parameters and cost history
    w = w_init.copy()
    b = b_init
    costs = []
    
    for i in range(num_iterations):
        # Calculate gradients
        dw, db = compute_gradients(X, y, w, b)
        
        # Update parameters
        w = w - alpha * dw
        b = b - alpha * db
        
        # Calculate and store cost every `print_every` iterations
        if i % print_every == 0:
            cost = compute_cost(X, y, w, b)
            costs.append(cost)
            print(f"Iteration {i}: Cost = {cost:.4f}")
    
    return w, b, costs
```

## Creating a Simple Example Dataset

Let's create a dataset to demonstrate logistic regression:

```python
# Create a simple classification dataset with two features
np.random.seed(42)
n_samples = 100

# Class 0 (negative) examples
X_neg = np.random.multivariate_normal(
    mean=[2, 2],    # Center of negative class
    cov=[[1, 0.5], [0.5, 1]],  # Covariance matrix
    size=n_samples//2
)

# Class 1 (positive) examples
X_pos = np.random.multivariate_normal(
    mean=[6, 6],    # Center of positive class
    cov=[[1, 0.5], [0.5, 1]],  # Covariance matrix
    size=n_samples//2
)

#The matrix [[1, 0.2, 0.1], [0.2, 1, 0.3], [0.1, 0.3, 1]] is a covariance matrix for a 3D multivariate normal distribution. Let me explain what each value means:
#The covariance matrix is a square matrix whose elements represent the covariance between different variables (or the variance when on the diagonal).
#For a 3D distribution, we have 3 variables, let's call them X, Y, and Z. The covariance matrix can be labeled as:
#    [[Var(X), Cov(X,Y), Cov(X,Z)],
#     [Cov(Y,X), Var(Y), Cov(Y,Z)],
#     [Cov(Z,X), Cov(Z,Y), Var(Z)]]
# Diagonal elements (values of 1 in this case):
# The (1,1) element: Var(X) = 1, meaning variable X has variance 1
# The (2,2) element: Var(Y) = 1, meaning variable Y has variance 1
# The (3,3) element: Var(Z) = 1, meaning variable Z has variance 1
# Off-diagonal elements (the correlation values):
# The (1,2) and (2,1) elements: Cov(X,Y) = Cov(Y,X) = 0.2
# The (1,3) and (3,1) elements: Cov(X,Z) = Cov(Z,X) = 0.1
# The (2,3) and (3,2) elements: Cov(Y,Z) = Cov(Z,Y) = 0.3
# Since covariance = correlation × standard deviation of X × standard deviation of Y, and here all standard # deviations are 1 (since variances are 1), the covariance values are equal to the correlation values.
# So the correlation between:
# X and Y is 0.2 (weak positive correlation)
# X and Z is 0.1 (very weak positive correlation)
# Y and Z is 0.3 (moderate positive correlation)
# These correlation values range from -1 to 1:
# Values close to 0 indicate weak correlation
# Values close to 1 indicate strong positive correlation
# Values close to -1 indicate strong negative correlation


# Combine the two classes and create labels
X = np.vstack([X_neg, X_pos])
y = np.array([0] * (n_samples//2) + [1] * (n_samples//2))

# Visualize the dataset
plt.figure(figsize=(10, 6))
plt.scatter(X[y==0, 0], X[y==0, 1], color='blue', label='Class 0')
plt.scatter(X[y==1, 0], X[y==1, 1], color='red', label='Class 1')
plt.xlabel('Feature 1')
plt.ylabel('Feature 2')
plt.title('Classification Dataset')
plt.legend()
plt.grid(True)
plt.show()
```

## Training the Logistic Regression Model

Now, let's train our logistic regression model on this dataset:

```python
# Initialize parameters (weights and bias)
n_features = X.shape[1]
w_init = np.zeros(n_features)
b_init = 0

# Hyperparameters
alpha = 0.1       # Learning rate
num_iterations = 2000
print_every = 200

# Train the model using gradient descent
w_final, b_final, costs = gradient_descent(X, y, w_init, b_init, alpha, num_iterations, print_every)

# Print the final parameters
print(f"Final weights: {w_final}")
print(f"Final bias: {b_final:.4f}")

# Plot the cost function
plt.figure(figsize=(10, 6))
plt.plot(range(0, num_iterations, print_every), costs)
plt.xlabel('Iteration')
plt.ylabel('Cost')
plt.title('Cost Function During Training')
plt.grid(True)
plt.show()
```

## Making Predictions

To make predictions with our trained model:

```python
def predict(X, w, b, threshold=0.5):
    """
    Make predictions using the trained logistic regression model
    
    Parameters:
        X (ndarray): Input features, shape (n_samples, n_features)
        w (ndarray): Trained weights, shape (n_features,)
        b (float): Trained bias
        threshold (float): Classification threshold (default 0.5)
        
    Returns:
        ndarray: Predicted labels (0 or 1)
    """
    # Calculate the predicted probabilities
    z = np.dot(X, w) + b
    predicted_proba = sigmoid(z)
    
    # Convert probabilities to binary predictions based on threshold
    predicted_labels = (predicted_proba >= threshold).astype(int)
    
    return predicted_labels, predicted_proba
```

## Visualizing the Decision Boundary

Let's visualize the decision boundary of our trained model:

```python
def plot_decision_boundary(X, y, w, b):
    """
    Plot the decision boundary of the logistic regression model
    
    Parameters:
        X (ndarray): Input features, shape (n_samples, n_features)
        y (ndarray): Target labels, shape (n_samples,)
        w (ndarray): Trained weights, shape (n_features,)
        b (float): Trained bias
    """
    # Create a grid over the feature space
    x1_min, x1_max = X[:, 0].min() - 1, X[:, 0].max() + 1
    x2_min, x2_max = X[:, 1].min() - 1, X[:, 1].max() + 1
    xx1, xx2 = np.meshgrid(np.arange(x1_min, x1_max, 0.1),
                          np.arange(x2_min, x2_max, 0.1))
    
    # Flatten the grid points
    grid_points = np.c_[xx1.ravel(), xx2.ravel()]
    
    # Make predictions on all grid points
    predictions, _ = predict(grid_points, w, b)
    
    # Reshape the predictions to match the grid
    predictions = predictions.reshape(xx1.shape)
    
    # Plot the decision boundary and data points
    plt.figure(figsize=(10, 6))
    plt.contourf(xx1, xx2, predictions, alpha=0.3, cmap=plt.cm.coolwarm)
    plt.scatter(X[y==0, 0], X[y==0, 1], color='blue', label='Class 0')
    plt.scatter(X[y==1, 0], X[y==1, 1], color='red', label='Class 1')
    plt.xlabel('Feature 1')
    plt.ylabel('Feature 2')
    plt.title('Logistic Regression Decision Boundary')
    plt.legend()
    plt.grid(True)
    plt.show()

# Plot the decision boundary
plot_decision_boundary(X, y, w_final, b_final)
```

## Evaluating the Model

Let's evaluate the performance of our model:

```python
def evaluate_model(X, y, w, b):
    """
    Evaluate the logistic regression model
    
    Parameters:
        X (ndarray): Input features, shape (n_samples, n_features)
        y (ndarray): True labels, shape (n_samples,)
        w (ndarray): Trained weights, shape (n_features,)
        b (float): Trained bias
        
    Returns:
        dict: Performance metrics (accuracy, precision, recall, f1-score)
    """
    # Get predictions
    y_pred, y_proba = predict(X, w, b)
    
    # Calculate metrics
    accuracy = np.mean(y_pred == y)
    
    # Calculate confusion matrix elements
    true_positive = np.sum((y_pred == 1) & (y == 1))
    false_positive = np.sum((y_pred == 1) & (y == 0))
    true_negative = np.sum((y_pred == 0) & (y == 0))
    false_negative = np.sum((y_pred == 0) & (y == 1))
    
    # Calculate precision and recall
    precision = true_positive / (true_positive + false_positive) if (true_positive + false_positive) > 0 else 0
    recall = true_positive / (true_positive + false_negative) if (true_positive + false_negative) > 0 else 0
    
    # Calculate F1 score
    f1_score = 2 * precision * recall / (precision + recall) if (precision + recall) > 0 else 0
    
    return {
        'accuracy': accuracy,
        'precision': precision,
        'recall': recall,
        'f1_score': f1_score,
        'confusion_matrix': {
            'true_positive': true_positive,
            'false_positive': false_positive,
            'true_negative': true_negative,
            'false_negative': false_negative
        }
    }

# Evaluate the model
metrics = evaluate_model(X, y, w_final, b_final)

# Print the metrics
print(f"Accuracy: {metrics['accuracy']:.4f}")
print(f"Precision: {metrics['precision']:.4f}")
print(f"Recall: {metrics['recall']:.4f}")
print(f"F1 Score: {metrics['f1_score']:.4f}")
print("\nConfusion Matrix:")
print(f"True Positive: {metrics['confusion_matrix']['true_positive']}")
print(f"False Positive: {metrics['confusion_matrix']['false_positive']}")
print(f"True Negative: {metrics['confusion_matrix']['true_negative']}")
print(f"False Negative: {metrics['confusion_matrix']['false_negative']}")
```

## Comparing with scikit-learn

Let's compare our implementation with scikit-learn's implementation:

```python
from sklearn.linear_model import LogisticRegression
from sklearn.metrics import accuracy_score, precision_score, recall_score, f1_score, confusion_matrix

# Train the scikit-learn model
sk_model = LogisticRegression(random_state=42)
sk_model.fit(X, y)

# Make predictions
y_pred_sk = sk_model.predict(X)

# Calculate metrics
accuracy_sk = accuracy_score(y, y_pred_sk)
precision_sk = precision_score(y, y_pred_sk)
recall_sk = recall_score(y, y_pred_sk)
f1_sk = f1_score(y, y_pred_sk)
conf_matrix_sk = confusion_matrix(y, y_pred_sk)

# Print the metrics
print("Scikit-learn Logistic Regression Results:")
print(f"Accuracy: {accuracy_sk:.4f}")
print(f"Precision: {precision_sk:.4f}")
print(f"Recall: {recall_sk:.4f}")
print(f"F1 Score: {f1_sk:.4f}")
print("\nConfusion Matrix:")
print(conf_matrix_sk)
```

## Real-world Example: Predicting Diabetes

Let's apply our logistic regression implementation to a real-world example using the Pima Indians Diabetes dataset:

```python
# Import the Pima Indians Diabetes dataset
from sklearn.datasets import load_diabetes
from sklearn.model_selection import train_test_split
from sklearn.preprocessing import StandardScaler

# Load and prepare the diabetes dataset
diabetes = load_diabetes()
X_diabetes = diabetes.data
y_diabetes = (diabetes.target > diabetes.target.mean()).astype(int)  # Convert to binary classification

# Split the data into training and testing sets
X_train, X_test, y_train, y_test = train_test_split(
    X_diabetes, y_diabetes, test_size=0.2, random_state=42
)

# Standardize the features
scaler = StandardScaler()
X_train_scaled = scaler.fit_transform(X_train)
X_test_scaled = scaler.transform(X_test)

# Initialize the model parameters
n_features_diabetes = X_train_scaled.shape[1]
w_init_diabetes = np.zeros(n_features_diabetes)
b_init_diabetes = 0

# Hyperparameters
alpha_diabetes = 0.01
num_iterations_diabetes = 5000
print_every_diabetes = 1000

# Train the model
w_final_diabetes, b_final_diabetes, costs_diabetes = gradient_descent(
    X_train_scaled, y_train, w_init_diabetes, b_init_diabetes, 
    alpha_diabetes, num_iterations_diabetes, print_every_diabetes
)

# Evaluate the model on the test set
diabetes_metrics = evaluate_model(X_test_scaled, y_test, w_final_diabetes, b_final_diabetes)

# Print the test metrics
print("\nDiabetes Dataset Results:")
print(f"Test Accuracy: {diabetes_metrics['accuracy']:.4f}")
print(f"Test Precision: {diabetes_metrics['precision']:.4f}")
print(f"Test Recall: {diabetes_metrics['recall']:.4f}")
print(f"Test F1 Score: {diabetes_metrics['f1_score']:.4f}")
```

## Summary and Key Differences from Linear Regression

Logistic regression differs from linear regression in several key ways:

1. **Purpose**:
   - Linear Regression: Predicts continuous values (regression)
   - Logistic Regression: Predicts probabilities for binary classification

2. **Output Range**:
   - Linear Regression: Can output any real number (-∞ to +∞)
   - Logistic Regression: Outputs probabilities between 0 and 1

3. **Activation Function**:
   - Linear Regression: No activation function (linear)
   - Logistic Regression: Uses sigmoid function to squash outputs to [0,1]

4. **Cost Function**:
   - Linear Regression: Mean Squared Error
   - Logistic Regression: Binary Cross-Entropy (Log Loss)

5. **Interpretation**:
   - Linear Regression: Coefficients directly impact the predicted value
   - Logistic Regression: Coefficients impact log-odds of the positive class

The key mathematical insights are:

1. The sigmoid function transforms linear predictions into probabilities
2. The cost function penalizes incorrect predictions differently based on how confident the model was
3. Gradient descent works similarly to linear regression, but the gradients are computed differently

This implementation shows how to build logistic regression from scratch, understand the mathematics behind it, and apply it to real-world problems.
