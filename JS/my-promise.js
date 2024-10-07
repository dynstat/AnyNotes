/**
 * Custom Promise Implementation - MyPromise
 * 
 * This class mimics the native JavaScript Promise.
 * It supports the .then method for chaining and can be used with async/await.
 * 
 * Note: This is a simplified version for educational purposes and does not cover all edge cases.
 */

class MyPromise {
    /**
     * The constructor takes an executor function which receives
     * resolve and reject functions as arguments.
     * @param {function} executor - Function to execute the Promise logic.
     */
    constructor(executor) {
        // Initialize the state to 'pending'
        this.state = 'pending';
        // Initialize the value or reason to undefined
        this.value = undefined;
        this.reason = undefined;
        // Handlers to store .then callbacks
        this.onFulfilledCallbacks = [];
        this.onRejectedCallbacks = [];

        // Bind the resolve and reject functions to ensure correct 'this'
        const resolve = (value) => {
            // Only proceed if the state is pending
            if (this.state === 'pending') {
                this.state = 'fulfilled'; // Update state to fulfilled
                this.value = value; // Set the fulfillment value
                // Execute all stored onFulfilled callbacks
                this.onFulfilledCallbacks.forEach(fn => fn());
            }
        };

        const reject = (reason) => {
            // Only proceed if the state is pending
            if (this.state === 'pending') {
                this.state = 'rejected'; // Update state to rejected
                this.reason = reason; // Set the rejection reason
                // Execute all stored onRejected callbacks
                this.onRejectedCallbacks.forEach(fn => fn());
            }
        };

        try {
            // Execute the executor function with resolve and reject
            executor(resolve, reject);
        } catch (error) {
            // If an error occurs, reject the Promise
            reject(error);
        }
    }

    /**
     * The then method registers callbacks to handle the fulfilled or rejected state.
     * It returns a new Promise for chaining.
     * @param {function} onFulfilled - Function to execute on fulfillment.
     * @param {function} onRejected - Function to execute on rejection.
     * @returns {MyPromise} - A new Promise for chaining.
     */
    then(onFulfilled, onRejected) {
        // Return a new Promise for chaining
        return new MyPromise((resolve, reject) => {
            // Handle fulfillment
            const fulfilledTask = () => {
                try {
                    // Execute the onFulfilled callback with the value
                    const result = onFulfilled(this.value);
                    // Resolve the new Promise with the callback result
                    resolve(result);
                } catch (error) {
                    // If an error occurs, reject the new Promise
                    reject(error);
                }
            };

            // Handle rejection
            const rejectedTask = () => {
                try {
                    // Execute the onRejected callback with the reason
                    const result = onRejected(this.reason);
                    // Resolve the new Promise with the callback result
                    resolve(result);
                } catch (error) {
                    // If an error occurs, reject the new Promise
                    reject(error);
                }
            };

            // Depending on the current state, execute immediately or store the callbacks
            if (this.state === 'fulfilled') {
                fulfilledTask();
            } else if (this.state === 'rejected') {
                rejectedTask();
            } else {
                // If pending, store the callbacks to execute later
                this.onFulfilledCallbacks.push(fulfilledTask);
                this.onRejectedCallbacks.push(rejectedTask);
            }
        });
    }

    /**
     * The catch method registers a callback to handle rejection.
     * It is a shorthand for then(null, onRejected).
     * @param {function} onRejected - Function to execute on rejection.
     * @returns {MyPromise} - A new Promise for chaining.
     */
    catch(onRejected) {
        return this.then(null, onRejected);
    }
}

// Example usage with .then
const promise1 = new MyPromise((resolve, reject) => {
    setTimeout(() => {
        resolve('Promise 1 fulfilled!');
    }, 1000);
});

promise1
    .then(message => {
        console.log(message); // Output after 1 second: Promise 1 fulfilled!
        return 'Chained from Promise 1';
    })
    .then(chainedMessage => {
        console.log(chainedMessage); // Output: Chained from Promise 1
    })
    .catch(error => {
        console.error('Error:', error);
    });

// Example usage with async/await
const asyncFunction = async () => {
    try {
        const message = await promise1;
        console.log('Async/Await:', message); // Output after 1 second: Async/Await: Promise 1 fulfilled!
    } catch (error) {
        console.error('Error:', error);
    }
};

asyncFunction();

/**
 * Explanation:
 * 
 * 1. **MyPromise Class**:
 *    - Mimics the native Promise behavior with states: 'pending', 'fulfilled', and 'rejected'.
 *    - Stores fulfillment value and rejection reason.
 *    - Maintains arrays of callbacks for fulfillment and rejection.
 * 
 * 2. **Constructor**:
 *    - Initializes the state and callback arrays.
 *    - Defines resolve and reject functions to transition the Promise's state.
 *    - Executes the executor function with resolve and reject.
 *    - Catches any synchronous errors and rejects the Promise.
 * 
 * 3. **then Method**:
 *    - Returns a new MyPromise, enabling chaining.
 *    - Defines tasks for fulfillment and rejection that execute the provided callbacks.
 *    - Handles immediate execution if the Promise is already settled.
 *    - Stores the callbacks if the Promise is still pending.
 * 
 * 4. **catch Method**:
 *    - Shorthand for handling rejection using then(null, onRejected).
 * 
 * 5. **Usage Examples**:
 *    - Demonstrates using MyPromise with .then chaining.
 *    - Shows compatibility with async/await syntax.
 */

/**
 * Note:
 * This is a basic implementation and does not cover advanced features like:
 * - Handling asynchronous execution of callbacks as per the Promise/A+ specification.
 * - Implementing static methods like Promise.all, Promise.race, etc.
 * - Ensuring that callbacks are called in a microtask queue.
 * 
 * For a complete Promise implementation, refer to the Promise/A+ specification.
 */