
# Logistic Regression: Mathematics Explained Step-by-Step

## 1. Logistic Regression Fundamentals

Logistic regression is a regression model at its core, not a classifier. It estimates the probability of a binary outcome using a linear combination of features passed through the logistic function.

### The Logistic (Sigmoid) Function

The logistic function maps any real number to a probability between 0 and 1:

$$\sigma(z) = \frac{1}{1 + e^{-z}}$$

Where $z$ is a linear combination of features: $z = \beta_0 + \beta_1x_1 + \beta_2x_2 + ... + \beta_nx_n$

![Sigmoid Function](https://miro.medium.com/max/1400/1*RqXFpiNGwdiKBWyLJc_E7g.png)

### The Logit Function (Inverse Sigmoid)

The logit function transforms probabilities (0-1) to the entire real number line (-∞,+∞):

$$\text{logit}(p) = \log\left(\frac{p}{1-p}\right)$$

This is the log of the odds ratio, representing how many times more likely the outcome is to occur than not occur.

## 2. Simple Example with Vectors

Let's use a sample dataset with one feature predicting fire occurrence (1) or not (0):

```
Temperature (°C) | Fire Occurrence
-----------------+---------------
        25       |       0
        30       |       0
        35       |       0
        40       |       1
        45       |       1
```

In vector form:
- Feature vector X = [25, 30, 35, 40, 45]
- Target vector y = [0, 0, 0, 1, 1]

### Linear Predictor
For a single feature, our linear predictor is: $z = \beta_0 + \beta_1 X$

Assuming initial values $\beta_0 = -10$ and $\beta_1 = 0.3$:
- For temperature = 25°C: $z = -10 + 0.3(25) = -2.5$
- For temperature = 45°C: $z = -10 + 0.3(45) = 3.5$

### Probability Calculation with Sigmoid
- For temperature = 25°C: $p = \frac{1}{1 + e^{2.5}} = 0.076$ (7.6% chance of fire)
- For temperature = 45°C: $p = \frac{1}{1 + e^{-3.5}} = 0.971$ (97.1% chance of fire)

## 3. Mathematical Optimization

Logistic regression finds optimal coefficients by maximizing the log-likelihood function:

$$\ell(\beta) = \sum_{i=1}^{n} [y_i \log(p_i) + (1-y_i) \log(1-p_i)]$$

Where:
- $y_i$ is the actual outcome (0 or 1)
- $p_i$ is the predicted probability for observation i


---
## NOTE:
## 🧠 Part 1: What is `E[X]`?

**`E` stands for "Expected Value"**, and it’s denoted as:
$$
E[X]=E[X]
$$
It simply means:  
👉 “What do you **expect** the average value of random variable `X` to be in the long run?”

---

### 🎯 Think of it like:

If you toss a biased coin (1 for heads, 0 for tails) many times:

- 70% chance heads = 1
    
- 30% chance tails = 0
    

Then after 1000 tosses, you expect:

Average=0.7×1+0.3×0=0.7\text{Average} = 0.7 \times 1 + 0.3 \times 0 = 0.7

So,
$$
E[X]=1⋅p+0⋅(1−p)=p
$$
---

## 📐 Part 2: What is Variance, Actually?

> Variance tells **how much the values are spread out from the mean**.

Mathematically:
$$
{Var}(X) = \mathbb{E}[(X - \mu)^2]
$$
Where:

- $$μ= \mathbb{E}[X] (mean of X)
$$  
- $$ 
(X - \mu)^2 = squared difference from the mean
$$    

---

## 🧪 Why Use E[(X−μ)2]\mathbb{E}[(X - \mu)^2] as Variance?

This is because:

- Taking just `X - μ` and averaging gives 0 (since values cancel).
    
- So we **square it** to make all deviations positive → (X−μ)2(X - \mu)^2
    
- Then take the **expected value** (average of these squared deviations).
    

✅ This gives us a true sense of "spread".

---

## 🧾 Deriving Variance Formula

We have:
$$
Var(X)=E[(X−μ)2]
$$
Let's expand:
$$
=E[X2−2Xμ+μ2]
$$
Because expectation is linear:
$$
=E[X2]−2μE[X]+μ2
$$
Since $$μ=E[X]$$, we substitute:
$$
=E[X2]−2μ2+μ2=E[X2]−μ2
$$
✅ **So this gives the famous formula:**
$$
Var(X)=E[X2]−(E[X])2
$$
This is how **variance** is derived.

---

## 📊 Bernoulli Distribution: Apply All

### Random Variable X:

- X=1X = 1 with probability `p`
    
- X=0X = 0 with probability `1 - p`
    

### Step-by-step:

#### 1. Mean:
$$
μ=E[X]=1⋅p+0⋅(1−p)=p\mu = \mathbb{E}[X] = 1 \cdot p + 0 \cdot (1 - p) = p
$$
#### 2. E[X2]\mathbb{E}[X^2]:
$$
Since X2=XX^2 = X (because 0²=0, 1²=1)
$$
$$
E[X2]=E[X]=p\mathbb{E}[X^2] = \mathbb{E}[X] = p
$$
#### 3. Variance:
$$
Var(X)=E[X2]−(E[X])2=p−p2=p(1−p)
$$
#### 4. Standard Deviation:
$$
σ=p(1−p)\sigma = \sqrt{p(1 - p)}
$$
---

## 📌 Quick Summary Table

| Concept               | Formula                                                                                  | What it tells     |
| --------------------- | ---------------------------------------------------------------------------------------- | ----------------- |
| Expected Value (E[X]) | $$ ∑xi⋅P(xi)\sum x_i \cdot P(x_i)$$                                                      | Average           |
| Variance (Var(X))     | $$E[(X−μ)2]\mathbb{E}[(X - \mu)^2] or E[X2]−(E[X])2\mathbb{E}[X^2] - (\mathbb{E}[X])^2$$ | Spread            |
| Standard Deviation    | $$Var(X)\sqrt{\text{Var}(X)}$$                                                           | Typical deviation |

---

## 🎯 Analogy to Remember

Imagine a class where everyone scores:

- 10/10 → no spread → variance = 0
    
- Some score 0, some 10 → huge spread → variance high
    
- Mix of scores near average → medium variance
    

---

## 🔗 References

- [Khan Academy: Variance Explained](https://www.khanacademy.org/math/statistics-probability)
    
- [Wikipedia: Variance](https://en.wikipedia.org/wiki/Variance)
    
- [Expected Value – Brilliant](https://brilliant.org/wiki/expected-value/)
    

---
---
## Step-by-Step Derivation of the Log-Likelihood Function in Logistic Regression

### 1. Starting Point: The Bernoulli Distribution

In logistic regression for binary classification, we model the probability of an outcome (y=1) as:

$$P(y=1|x) = \frac{1}{1+e^{-(\beta_0 + \beta_1 x_1 + ... + \beta_n x_n)}} = \sigma(z)$$

For a binary outcome, the probability distribution follows the Bernoulli distribution:

$$P(y|x) = p^y \cdot (1-p)^{1-y}$$

where:
- If y = 1: P(y|x) = p
- If y = 0: P(y|x) = 1-p

### 2. From Individual Probabilities to Likelihood

The likelihood function measures how likely our observed data is, given our model parameters. For a single observation:

$$L_i(\beta) = P(y_i|x_i) = [\sigma(z_i)]^{y_i} \cdot [1-\sigma(z_i)]^{1-y_i}$$

For all n observations, assuming independence, the likelihood is the product:

$$L(\beta) = \prod_{i=1}^{n} [\sigma(z_i)]^{y_i} \cdot [1-\sigma(z_i)]^{1-y_i}$$

### 3. Why Log-Likelihood?

We take the logarithm of the likelihood function for three key reasons:

1. **Numerical stability**: Products of probabilities (small numbers) can lead to underflow
2. **Computational simplicity**: Logarithm converts products to sums
3. **Mathematical convenience**: Derivatives of logarithms are simpler

### 4. Deriving the Log-Likelihood

Taking the natural logarithm of our likelihood function:

$$\ell(\beta) = \log L(\beta) = \log \prod_{i=1}^{n} [\sigma(z_i)]^{y_i} \cdot [1-\sigma(z_i)]^{1-y_i}$$

Using the property of logarithms (log of product = sum of logs):

$$\ell(\beta) = \sum_{i=1}^{n} \log \left( [\sigma(z_i)]^{y_i} \cdot [1-\sigma(z_i)]^{1-y_i} \right)$$

Using the property of logarithms for exponents (log(a^b) = b·log(a)):

$$\ell(\beta) = \sum_{i=1}^{n} \left[ y_i \log(\sigma(z_i)) + (1-y_i) \log(1-\sigma(z_i)) \right]$$

This is our final log-likelihood function.

### 5. Comparison with Linear Regression

In linear regression, we use Mean Squared Error (MSE) as the cost function:

$$J(\beta) = \frac{1}{n} \sum_{i=1}^{n} (y_i - \hat{y}_i)^2$$

**Key differences:**

1. **Origin**: 
   - MSE comes from the assumption of normally distributed errors
   - Log-likelihood comes from Bernoulli distribution for binary outcomes

2. **Goal**:
   - Linear regression: Minimize MSE (a measure of error)
   - Logistic regression: Maximize log-likelihood (a measure of how well the model fits)

3. **Mathematical behavior**:
   - MSE penalizes errors quadratically
   - Log-likelihood penalizes extreme predictions that turn out wrong much more severely

### 6. Example with Sample Data

Let's use a simple example with 3 observations:

```
x | y
--+--
2 | 0
4 | 0
6 | 1
```

Let's assume our current model has β₀ = -3 and β₁ = 0.7

#### Step 1: Calculate z values
- z₁ = -3 + 0.7(2) = -1.6
- z₂ = -3 + 0.7(4) = -0.2
- z₃ = -3 + 0.7(6) = 1.2

#### Step 2: Calculate probabilities using sigmoid
- p₁ = 1/(1+e^1.6) = 0.168
- p₂ = 1/(1+e^0.2) = 0.450
- p₃ = 1/(1+e^-1.2) = 0.769

#### Step 3: Calculate log-likelihood
- For 1ˢᵗ observation (y₁=0): (1-0)log(1-0.168) = log(0.832) = -0.184
- For 2ⁿᵈ observation (y₂=0): (1-0)log(1-0.450) = log(0.550) = -0.598
- For 3ʳᵈ observation (y₃=1): (1)log(0.769) = -0.263

Total log-likelihood = -0.184 + (-0.598) + (-0.263) = -1.045

#### Step 4: Optimize by finding parameters that maximize this value

If we tried different values of β₀ and β₁, we'd find that the log-likelihood would be higher (closer to 0) for better-fitting parameters.

### 7. Maximizing Log-Likelihood vs. Minimizing Cross-Entropy

Maximizing the log-likelihood is equivalent to minimizing the negative log-likelihood, which is also known as cross-entropy loss:

$$-\ell(\beta) = -\sum_{i=1}^{n} \left[ y_i \log(\sigma(z_i)) + (1-y_i) \log(1-\sigma(z_i)) \right]$$

This is the form commonly seen in machine learning libraries, where we're minimizing a cost function rather than maximizing likelihood.

---
---

### Gradient Descent Method

1. Start with random coefficients $\beta$
2. Compute the gradient vector: 
   $$\nabla \ell(\beta) = X^T(y - p)$$
   where $X$ is the feature matrix, $y$ is the true label vector, and $p$ is the predicted probability vector
3. Update coefficients: $\beta^{new} = \beta^{old} + \alpha \nabla \ell(\beta)$
4. Repeat until convergence

### Newton's Method

Newton's method converges faster but is more computationally expensive:

1. Compute gradient: $\nabla \ell(\beta)$
2. Compute Hessian matrix: $H = -X^T W X$ where $W$ is diagonal with elements $W_{ii} = p_i(1-p_i)$
3. Update: $\beta^{new} = \beta^{old} - H^{-1} \nabla \ell(\beta)$

## 4. Matrix Calculation Example

For our sample dataset, let's add an intercept column to X:

```
X = [
    [1, 25],
    [1, 30],
    [1, 35],
    [1, 40],
    [1, 45]
]
```

Starting with $\beta = [-10, 0.3]$:

1. Calculate linear predictors: $z = X\beta$
   ```
   z = [
       -10 + 0.3*25 = -2.5,
       -10 + 0.3*30 = -1.0,
       -10 + 0.3*35 = 0.5,
       -10 + 0.3*40 = 2.0,
       -10 + 0.3*45 = 3.5
   ]
   ```

2. Apply sigmoid to get probabilities:
   ```
   p = [
       1/(1+e^2.5) = 0.076,
       1/(1+e^1.0) = 0.269,
       1/(1+e^-0.5) = 0.622,
       1/(1+e^-2.0) = 0.881,
       1/(1+e^-3.5) = 0.971
   ]
   ```

3. Calculate error: $y - p$
   ```
   y - p = [
       0 - 0.076 = -0.076,
       0 - 0.269 = -0.269,
       0 - 0.622 = -0.622,
       1 - 0.881 = 0.119,
       1 - 0.971 = 0.029
   ]
   ```

4. Gradient: $X^T(y - p)$
   ```
   [1, 1, 1, 1, 1] · [-0.076, -0.269, -0.622, 0.119, 0.029] = -0.819
   [25, 30, 35, 40, 45] · [-0.076, -0.269, -0.622, 0.119, 0.029] = -17.47
   ```
   Gradient = [-0.819, -17.47]

5. With learning rate α = 0.1, update β:
   ```
   β_new = [-10, 0.3] + 0.1·[-0.819, -17.47] = [-10.0819, 0.1253]
   ```

This process repeats until convergence.

## 5. Decision Boundary

When the model converges (e.g., to β = [-15, 0.4]), the decision boundary is where p = 0.5, which occurs when z = 0:

$$\beta_0 + \beta_1x = 0$$
$$-15 + 0.4x = 0$$
$$x = 37.5$$

For our fire prediction example, the model predicts a fire when temperature exceeds 37.5°C.

The logistic regression doesn't perform classification directly - it outputs probabilities. We apply a threshold (typically 0.5) to convert these probabilities to binary predictions.

---
