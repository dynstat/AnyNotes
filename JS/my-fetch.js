/**
 * Custom Fetch Function - myfetch
 * 
 * This function mimics the native fetch API using XMLHttpRequest.
 * It supports both Promise-based (.then) and async/await syntax.
 * 
 * @param {string} url - The URL to fetch.
 * @param {object} options - Optional configurations for the request.
 * @returns {Promise} - A Promise that resolves to the response data.
 */
function myfetch(url, options = {}) {
    return new Promise((resolve, reject) => {
        // Create a new XMLHttpRequest object
        const xhr = new XMLHttpRequest();

        // Set the HTTP method, defaulting to 'GET' if not specified
        const method = options.method || 'GET';
        xhr.open(method, url, true); // true for asynchronous request

        // Set request headers if provided in options
        if (options.headers) {
            for (let header in options.headers) {
                if (options.headers.hasOwnProperty(header)) {
                    xhr.setRequestHeader(header, options.headers[header]);
                }
            }
        }

        // Define the callback function that will be called when the request's state changes
        xhr.onreadystatechange = function () {
            // Ready state 4 means the request is done
            if (xhr.readyState === 4) {
                // HTTP status 200-299 are considered successful
                if (xhr.status >= 200 && xhr.status < 300) {
                    // Try to parse the response as JSON, otherwise return as text
                    try {
                        const data = JSON.parse(xhr.responseText);
                        resolve(data); // Resolve the Promise with the parsed data
                    } catch (e) {
                        resolve(xhr.responseText); // Resolve with plain text if JSON parsing fails
                    }
                } else {
                    // Reject the Promise with the status text if the request failed
                    reject(new Error(xhr.statusText));
                }
            }
        };

        // Handle network errors
        xhr.onerror = function () {
            reject(new Error('Network Error'));
        };

        // Send the request with the provided body or null
        xhr.send(options.body || null);
    });
}

// Example usage with .then
myfetch('https://api.example.com/data')
    .then(response => {
        console.log('Data received:', response);
    })
    .catch(error => {
        console.error('Error:', error);
    });

// Example usage with async/await
async function fetchData() {
    try {
        const response = await myfetch('https://api.example.com/data');
        console.log('Data received:', response);
    } catch (error) {
        console.error('Error:', error);
    }
}

fetchData();