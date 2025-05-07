## `server.py`

```python:server.py
from flask import Flask, redirect, request, jsonify, send_file, render_template
from flask_cors import CORS
import whisper
from gtts import gTTS
import os
import tempfile
import logging
from pydub import AudioSegment
import torch

app = Flask(__name__, static_folder='.', static_url_path='')
CORS(app, resources={r"/*": {"origins": "*"}})

# Load the Whisper model
model = whisper.load_model("base", device="cpu", in_memory=True)
model.eval()
model = model.to(torch.float32)
logging.debug("Whisper model loaded successfully in FP32")

logging.basicConfig(level=logging.DEBUG)

@app.route('/')
def index():
    return app.send_static_file('index.html')

@app.route('/stt', methods=['POST'])
def speech_to_text():
    if 'audio' not in request.files:
        return jsonify({"error": "No audio file provided"}), 400
    
    audio_file = request.files['audio']
    
    temp_dir = tempfile.mkdtemp()
    temp_path = os.path.join(temp_dir, "temp_audio.wav")
    
    try:
        # Convert audio to WAV using pydub
        audio = AudioSegment.from_file(audio_file)
        audio.export(temp_path, format="wav")
        logging.debug(f"Saved audio file to: {temp_path}")
        
        result = model.transcribe(temp_path)
        logging.debug(f"Transcription result: {result['text']}")
        
        return jsonify({"transcription": result["text"]})
    
    except Exception as e:
        logging.error(f"Error during transcription: {str(e)}")
        return jsonify({"error": f"Transcription failed: {str(e)}"}), 500
    
    finally:
        try:
            os.remove(temp_path)
            os.rmdir(temp_dir)
        except Exception as e:
            logging.warning(f"Failed to remove temporary file or directory: {str(e)}")

@app.route('/tts', methods=['POST'])
def text_to_speech():
    data = request.json
    if not data or 'text' not in data:
        return jsonify({"error": "No text provided"}), 400
    
    text = data['text']
    
    with tempfile.NamedTemporaryFile(delete=False, suffix=".mp3") as temp_audio:
        tts = gTTS(text=text, lang='en')
        tts.save(temp_audio.name)
        
        return send_file(temp_audio.name, mimetype="audio/mpeg")

@app.before_request
def before_request():
    if not request.is_secure:
        url = request.url.replace('http://', 'https://', 1)
        return redirect(url, code=301)

if __name__ == '__main__':
    app.run(debug=True, port=3000, host='0.0.0.0', ssl_context=('cert.pem', 'key.pem'))
```

### Explanation

The `server.py` file sets up a Flask-based backend server that handles two main functionalities:

1. **Speech-to-Text (STT)**: Converts uploaded audio files into text using OpenAI's Whisper model.
2. **Text-to-Speech (TTS)**: Converts input text into speech using Google's Text-to-Speech (`gTTS`) library.

Here's a detailed breakdown of each part of the code:

#### Imports and Setup

```python
from flask import Flask, redirect, request, jsonify, send_file, render_template
from flask_cors import CORS
import whisper
from gtts import gTTS
import os
import tempfile
import logging
from pydub import AudioSegment
import torch
```

- **Flask and Related Modules**: For creating the web server and handling HTTP requests.
- **CORS**: To handle Cross-Origin Resource Sharing, allowing the frontend to communicate with the backend.
- **whisper**: OpenAI's Whisper model for speech recognition.
- **gTTS**: Google's Text-to-Speech library to convert text into audio.
- **os, tempfile**: For file and directory operations.
- **logging**: To log information, debug messages, and errors.
- **pydub**: To handle audio file conversions.
- **torch**: PyTorch library, required by Whisper.

```python
app = Flask(__name__, static_folder='.', static_url_path='')
CORS(app, resources={r"/*": {"origins": "*"}})
```

- **Flask App Initialization**: Sets up the Flask app, serving static files from the current directory.
- **CORS Setup**: Allows all origins to access the server's resources.

#### Loading the Whisper Model

```python
# Load the Whisper model
model = whisper.load_model("base", device="cpu", in_memory=True)
model.eval()
model = model.to(torch.float32)
logging.debug("Whisper model loaded successfully in FP32")
```

- **Loading the Model**: Loads the base version of the Whisper model, using CPU for computation.
- **Model Evaluation Mode**: Sets the model to evaluation mode, which is suitable for inference.
- **Data Type Conversion**: Converts the model's parameters to 32-bit floating point for better precision.
- **Logging**: Confirms successful loading of the model.

```python
logging.basicConfig(level=logging.DEBUG)
```

- **Logging Configuration**: Sets the logging level to DEBUG to capture detailed information during execution.

#### Defining Routes

##### Home Route

```python
@app.route('/')
def index():
    return app.send_static_file('index.html')
```

- **Route**: `/`
- **Function**: Serves the `index.html` file when the root URL is accessed.

##### Speech-to-Text Route

```python
@app.route('/stt', methods=['POST'])
def speech_to_text():
    if 'audio' not in request.files:
        return jsonify({"error": "No audio file provided"}), 400
    
    audio_file = request.files['audio']
    
    temp_dir = tempfile.mkdtemp()
    temp_path = os.path.join(temp_dir, "temp_audio.wav")
    
    try:
        # Convert audio to WAV using pydub
        audio = AudioSegment.from_file(audio_file)
        audio.export(temp_path, format="wav")
        logging.debug(f"Saved audio file to: {temp_path}")
        
        result = model.transcribe(temp_path)
        logging.debug(f"Transcription result: {result['text']}")
        
        return jsonify({"transcription": result["text"]})
    
    except Exception as e:
        logging.error(f"Error during transcription: {str(e)}")
        return jsonify({"error": f"Transcription failed: {str(e)}"}), 500
    
    finally:
        try:
            os.remove(temp_path)
            os.rmdir(temp_dir)
        except Exception as e:
            logging.warning(f"Failed to remove temporary file or directory: {str(e)}")
```

- **Route**: `/stt`
- **Method**: `POST`
- **Function**: Handles speech-to-text conversion.
  
**Flow:**
1. **Check for Audio File**: Verifies that the request contains an audio file.
2. **Temporary Directory and File**: Creates a temporary directory and file to store the uploaded audio.
3. **Audio Conversion**: Uses `pydub` to convert the uploaded audio to WAV format, saving it to the temporary file.
4. **Transcription**: Uses the Whisper model to transcribe the audio file.
5. **Response**: Returns the transcription result as JSON.
6. **Error Handling**: Catches and logs any exceptions, returning an error message if transcription fails.
7. **Cleanup**: Deletes the temporary audio file and directory.

**Example:**
- A user uploads an MP3 file containing speech. The server converts it to WAV, transcribes the speech to text, and returns the text.

##### Text-to-Speech Route

```python
@app.route('/tts', methods=['POST'])
def text_to_speech():
    data = request.json
    if not data or 'text' not in data:
        return jsonify({"error": "No text provided"}), 400
    
    text = data['text']
    
    with tempfile.NamedTemporaryFile(delete=False, suffix=".mp3") as temp_audio:
        tts = gTTS(text=text, lang='en')
        tts.save(temp_audio.name)
        
        return send_file(temp_audio.name, mimetype="audio/mpeg")
```

- **Route**: `/tts`
- **Method**: `POST`
- **Function**: Handles text-to-speech conversion.
  
**Flow:**
1. **Parse JSON Data**: Extracts the text from the JSON payload.
2. **Error Handling**: Returns an error if no text is provided.
3. **Temporary File**: Creates a temporary MP3 file to store the generated speech.
4. **Text-to-Speech Conversion**: Uses `gTTS` to convert the text to speech and saves it to the temporary file.
5. **Response**: Sends the MP3 file back to the client.
  
**Example:**
- A user sends the text "Hello, World!" to the `/tts` endpoint. The server responds with an MP3 file containing the spoken version of the text.

##### Before Request Handler

```python
@app.before_request
def before_request():
    if not request.is_secure:
        url = request.url.replace('http://', 'https://', 1)
        return redirect(url, code=301)
```

- **Function**: Ensures that all incoming requests use HTTPS.
  
**Flow:**
1. **Check Security**: If the incoming request is not secure (i.e., not HTTPS).
2. **Redirect**: Redirects the request to the HTTPS version of the URL.
  
**Purpose:**
- Enhances security by enforcing HTTPS, ensuring that data transmitted between the client and server is encrypted.

#### Running the Server

```python
if __name__ == '__main__':
    app.run(debug=True, port=3000, host='0.0.0.0', ssl_context=('cert.pem', 'key.pem'))
```

- **Condition**: Checks if the script is run directly.
- **Run Server**: Starts the Flask development server.
  
**Parameters:**
- **`debug=True`**: Enables debug mode for detailed error messages and auto-reloading.
- **`port=3000`**: Runs the server on port 3000.
- **`host='0.0.0.0'`**: Makes the server accessible externally.
- **`ssl_context=('cert.pem', 'key.pem')`**: Uses SSL certificates to enable HTTPS.

**Example:**
- Running `python server.py` starts the server, making it accessible at `https://<your-server-ip>:3000/`.

---

## `script.js`

```javascript
// Text to Speech
const textInput = document.getElementById('text-input');
const speakButton = document.getElementById('speak-button');
const startRecordingButton = document.getElementById('start-recording');
const stopRecordingButton = document.getElementById('stop-recording');
const recordingStatus = document.getElementById('recording-status');
const textOutput = document.getElementById('speech-to-text-output');

let recognition;
let mediaRecorder;
let audioChunks = [];
let audioBlob;
let audioUrl;

// Text to Speech function
speakButton.addEventListener('click', () => {
    const text = textInput.value;
    if (text) {
        fetch('https://' + window.location.hostname + ':3000/tts', {
            method: 'POST',
            headers: {
                'Content-Type': 'application/json',
            },
            body: JSON.stringify({ text }),
        })
        .then(response => response.blob())
        .then(blob => {
            const audioUrl = URL.createObjectURL(blob);
            const audio = new Audio(audioUrl);
            audio.play();
        })
        .catch(error => {
            console.error('Error:', error);
            alert('Error converting text to speech');
        });
    }
});

// Speech to Text function
startRecordingButton.addEventListener('click', async () => {
    try {
        const stream = await navigator.mediaDevices.getUserMedia({ audio: true });
        mediaRecorder = new MediaRecorder(stream);

        mediaRecorder.ondataavailable = (event) => {
            audioChunks.push(event.data);
        };

        mediaRecorder.onstop = () => {
            audioBlob = new Blob(audioChunks, { type: 'audio/wav' });
            audioUrl = URL.createObjectURL(audioBlob);
            displayRecordedAudio();
            audioChunks = [];
        };

        mediaRecorder.start();
        recordingStatus.textContent = 'Recording...';
        startRecordingButton.disabled = true;
        stopRecordingButton.disabled = false;
    } catch (error) {
        console.error('Error accessing microphone:', error);
    }
});

stopRecordingButton.addEventListener('click', () => {
    if (mediaRecorder && mediaRecorder.state === 'recording') {
        mediaRecorder.stop();
        recordingStatus.textContent = 'Recording stopped';
        startRecordingButton.disabled = false;
        stopRecordingButton.disabled = true;
    }
});

function displayRecordedAudio() {
    const audioPlayer = document.createElement('audio');
    audioPlayer.controls = true;
    audioPlayer.src = audioUrl;
    
    const sendButton = document.createElement('button');
    sendButton.textContent = 'Send to Server';
    sendButton.onclick = () => sendAudioToServer(audioBlob);

    const container = document.createElement('div');
    container.appendChild(audioPlayer);
    container.appendChild(sendButton);

    const audioContainer = document.getElementById('audio-container');
    audioContainer.innerHTML = '';
    audioContainer.appendChild(container);
}

function sendAudioToServer(audioBlob) {
    console.log('Sending audio blob:', audioBlob);
    const formData = new FormData();
    formData.append('audio', audioBlob, 'recording.wav');

    isProcessing = true; // Start fast animation

    fetch('https://' + window.location.hostname + ':3000/stt', {
        method: 'POST',
        body: formData,
    })
    .then(response => response.json())
    .then(data => {
        console.log('Server response:', data);
        if (data.transcription) {
            textOutput.value = data.transcription;
            scrollToOutput();
        } else if (data.error) {
            textOutput.value = `Error: ${data.error}`;
            console.error('Server error details:', data.error);
            scrollToOutput();
        } else {
            textOutput.value = 'An unknown error occurred';
            scrollToOutput();
        }
    })
    .catch(error => {
        console.error('Fetch error:', error);
        textOutput.value = 'Error processing audio';
    })
    .finally(() => {
        isProcessing = false; // Return to normal animation speed
    });
}

// Three.js background setup
const scene = new THREE.Scene();
const camera = new THREE.PerspectiveCamera(75, window.innerWidth / window.innerHeight, 0.1, 1000);
const renderer = new THREE.WebGLRenderer({ canvas: document.getElementById('background-canvas'), alpha: true });
renderer.setSize(window.innerWidth, window.innerHeight);

const particleCount = 10000;
const positions = new Float32Array(particleCount * 3);
const sizes = new Float32Array(particleCount);

for (let i = 0; i < particleCount; i++) {
    positions[i * 3] = THREE.MathUtils.randFloatSpread(2000); // x
    positions[i * 3 + 1] = THREE.MathUtils.randFloatSpread(2000); // y
    positions[i * 3 + 2] = THREE.MathUtils.randFloatSpread(2000); // z
    sizes[i] = THREE.MathUtils.randFloat(1, 5);
}

const geometry = new THREE.BufferGeometry();
geometry.setAttribute('position', new THREE.BufferAttribute(positions, 3));
geometry.setAttribute('size', new THREE.BufferAttribute(sizes, 1));

const material = new THREE.ShaderMaterial({
    uniforms: {
        color: { value: new THREE.Color(0x4dabf7) },
    },
    vertexShader: `
        attribute float size;
        void main() {
            vec4 mvPosition = modelViewMatrix * vec4(position, 1.0);
            gl_PointSize = size * (300.0 / -mvPosition.z);
            gl_Position = projectionMatrix * mvPosition;
        }
    `,
    fragmentShader: `
        uniform vec3 color;
        void main() {
            if (length(gl_PointCoord - vec2(0.5, 0.5)) > 0.475) discard;
            gl_FragColor = vec4(color, 1.0);
        }
    `,
    transparent: true,
});

const particles = new THREE.Points(geometry, material);
scene.add(particles);

camera.position.z = 1000;

let isProcessing = false;
let normalSpeed = 0.0001;
let fastSpeed = 0.0009;

function updateParticleSpeed(speed) {
    particles.rotation.x += speed;
    particles.rotation.y += speed;
}

function animateBackground() {
    requestAnimationFrame(animateBackground);
    updateParticleSpeed(isProcessing ? fastSpeed : normalSpeed);
    renderer.render(scene, camera);
}
animateBackground();

// Resize handler
window.addEventListener('resize', () => {
    camera.aspect = window.innerWidth / window.innerHeight;
    camera.updateProjectionMatrix();
    renderer.setSize(window.innerWidth, window.innerHeight);
});

// UI enhancements
const converterBoxes = document.querySelectorAll('.converter-box');
converterBoxes.forEach(box => {
    box.addEventListener('mouseenter', () => {
        box.style.transform = 'scale(1.02)';
    });
    box.addEventListener('mouseleave', () => {
        box.style.transform = 'scale(1)';
    });
});

// Button click effect
const buttons = document.querySelectorAll('button');
buttons.forEach(button => {
    button.addEventListener('click', () => {
        button.style.transform = 'scale(0.95)';
        setTimeout(() => {
            button.style.transform = 'scale(1)';
        }, 100);
    });
});

// Smooth scroll to output
function scrollToOutput() {
    textOutput.scrollIntoView({ behavior: 'smooth', block: 'start' });
}

// Web Speech API for continuous speech recognition
recognition = new (window.SpeechRecognition || window.webkitSpeechRecognition)();
recognition.continuous = true;
recognition.interimResults = true;

recognition.onresult = (event) => {
    const result = event.results[event.results.length - 1];
    const transcript = result[0].transcript;
    textOutput.value = transcript;
};

recognition.start();
```

### Explanation

The `script.js` file manages the frontend functionalities of the AI-Powered Speech Converter application. It handles user interactions, communicates with the backend server for STT and TTS operations, and manages the visual background using Three.js.

Let's break down each section:

#### DOM Element References

```javascript
const textInput = document.getElementById('text-input');
const speakButton = document.getElementById('speak-button');
const startRecordingButton = document.getElementById('start-recording');
const stopRecordingButton = document.getElementById('stop-recording');
const recordingStatus = document.getElementById('recording-status');
const textOutput = document.getElementById('speech-to-text-output');

let recognition;
let mediaRecorder;
let audioChunks = [];
let audioBlob;
let audioUrl;
```

- **Element References**: Grabs HTML elements by their IDs to interact with them.
- **Variables**: Initializes variables for speech recognition (`recognition`), media recording (`mediaRecorder`), and audio data storage.

#### Text-to-Speech Functionality

```javascript
speakButton.addEventListener('click', () => {
    const text = textInput.value;
    if (text) {
        fetch('https://' + window.location.hostname + ':3000/tts', {
            method: 'POST',
            headers: {
                'Content-Type': 'application/json',
            },
            body: JSON.stringify({ text }),
        })
        .then(response => response.blob())
        .then(blob => {
            const audioUrl = URL.createObjectURL(blob);
            const audio = new Audio(audioUrl);
            audio.play();
        })
        .catch(error => {
            console.error('Error:', error);
            alert('Error converting text to speech');
        });
    }
});
```

- **Event Listener**: Triggers when the "Speak" button is clicked.
- **Flow**:
  1. **Retrieve Text**: Gets the text input by the user.
  2. **Fetch Request**: Sends a POST request to the `/tts` endpoint with the text in JSON format.
  3. **Handle Response**:
      - Converts the response (audio data) into a Blob.
      - Creates an object URL for the Blob.
      - Plays the audio using the `Audio` object.
  4. **Error Handling**: Logs errors and alerts the user if the TTS conversion fails.

**Example:**
- User enters "Good morning" and clicks "Speak". The application sends the text to the server, receives the audio file, and plays "Good morning" aloud.

#### Speech-to-Text Functionality

##### Start Recording

```javascript
startRecordingButton.addEventListener('click', async () => {
    try {
        const stream = await navigator.mediaDevices.getUserMedia({ audio: true });
        mediaRecorder = new MediaRecorder(stream);

        mediaRecorder.ondataavailable = (event) => {
            audioChunks.push(event.data);
        };

        mediaRecorder.onstop = () => {
            audioBlob = new Blob(audioChunks, { type: 'audio/wav' });
            audioUrl = URL.createObjectURL(audioBlob);
            displayRecordedAudio();
            audioChunks = [];
        };

        mediaRecorder.start();
        recordingStatus.textContent = 'Recording...';
        startRecordingButton.disabled = true;
        stopRecordingButton.disabled = false;
    } catch (error) {
        console.error('Error accessing microphone:', error);
    }
});
```

- **Event Listener**: Triggers when "Start Recording" button is clicked.
- **Flow**:
  1. **Access Microphone**: Requests access to the user's microphone.
  2. **Initialize MediaRecorder**: Sets up the `MediaRecorder` to capture audio.
  3. **Data Handling**:
      - **`ondataavailable`**: Collects audio data chunks as they become available.
      - **`onstop`**: Creates a Blob from the collected audio chunks and generates an object URL. Calls `displayRecordedAudio` to show the recorded audio and resets the `audioChunks` array.
  4. **Start Recording**: Begins recording audio.
  5. **Update UI**: Changes recording status and disables/enables appropriate buttons.
- **Error Handling**: Logs errors if microphone access fails.

##### Stop Recording

```javascript
stopRecordingButton.addEventListener('click', () => {
    if (mediaRecorder && mediaRecorder.state === 'recording') {
        mediaRecorder.stop();
        recordingStatus.textContent = 'Recording stopped';
        startRecordingButton.disabled = false;
        stopRecordingButton.disabled = true;
    }
});
```

- **Event Listener**: Triggers when "Stop Recording" button is clicked.
- **Flow**:
  1. **Check Recorder State**: Ensures that recording is in progress.
  2. **Stop Recording**: Stops the `MediaRecorder`.
  3. **Update UI**: Updates recording status and button states.

#### Displaying Recorded Audio

```javascript
function displayRecordedAudio() {
    const audioPlayer = document.createElement('audio');
    audioPlayer.controls = true;
    audioPlayer.src = audioUrl;
    
    const sendButton = document.createElement('button');
    sendButton.textContent = 'Send to Server';
    sendButton.onclick = () => sendAudioToServer(audioBlob);

    const container = document.createElement('div');
    container.appendChild(audioPlayer);
    container.appendChild(sendButton);

    const audioContainer = document.getElementById('audio-container');
    audioContainer.innerHTML = '';
    audioContainer.appendChild(container);
}
```

- **Function**: Creates and displays an audio player with the recorded audio and a "Send to Server" button.
- **Flow**:
  1. **Create Audio Player**: Allows users to play back the recorded audio.
  2. **Create Send Button**: Provides a button to send the audio to the server for transcription.
  3. **Assemble Elements**: Adds the audio player and send button to a container div.
  4. **Update UI**: Inserts the container into the designated `audio-container` div in the HTML.

**Example:**
- After stopping the recording, an audio player appears with the option to play the recording and send it for transcription.

#### Sending Audio to Server for Transcription

```javascript
function sendAudioToServer(audioBlob) {
    console.log('Sending audio blob:', audioBlob);
    const formData = new FormData();
    formData.append('audio', audioBlob, 'recording.wav');

    isProcessing = true; // Start fast animation

    fetch('https://' + window.location.hostname + ':3000/stt', {
        method: 'POST',
        body: formData,
    })
    .then(response => response.json())
    .then(data => {
        console.log('Server response:', data);
        if (data.transcription) {
            textOutput.value = data.transcription;
            scrollToOutput();
        } else if (data.error) {
            textOutput.value = `Error: ${data.error}`;
            console.error('Server error details:', data.error);
            scrollToOutput();
        } else {
            textOutput.value = 'An unknown error occurred';
            scrollToOutput();
        }
    })
    .catch(error => {
        console.error('Fetch error:', error);
        textOutput.value = 'Error processing audio';
    })
    .finally(() => {
        isProcessing = false; // Return to normal animation speed
    });
}
```

- **Function**: Sends the recorded audio to the server for transcription.
- **Flow**:
  1. **Log Audio Blob**: For debugging purposes.
  2. **Create FormData**: Appends the audio Blob with the name `recording.wav`.
  3. **Set Processing State**: Flags that processing is ongoing, potentially affecting animations or UI indicators.
  4. **Fetch Request**: Sends a POST request to the `/stt` endpoint with the audio file.
  5. **Handle Response**:
      - **Success**: Displays the transcription result in the `textOutput` textarea.
      - **Error**: Shows error messages if transcription fails.
      - **Unknown Errors**: Handles any unexpected responses.
  6. **Error Handling**: Logs any network or fetch-related errors and informs the user.
  7. **Finalize**: Resets the processing state.

**Example:**
- User records a voice message and clicks "Send to Server". The audio is sent to the server, transcribed into text, and the text appears in the output area.

#### Three.js Background Setup

```javascript
const scene = new THREE.Scene();
const camera = new THREE.PerspectiveCamera(75, window.innerWidth / window.innerHeight, 0.1, 1000);
const renderer = new THREE.WebGLRenderer({ canvas: document.getElementById('background-canvas'), alpha: true });
renderer.setSize(window.innerWidth, window.innerHeight);

const particleCount = 10000;
const positions = new Float32Array(particleCount * 3);
const sizes = new Float32Array(particleCount);

for (let i = 0; i < particleCount; i++) {
    positions[i * 3] = THREE.MathUtils.randFloatSpread(2000); // x
    positions[i * 3 + 1] = THREE.MathUtils.randFloatSpread(2000); // y
    positions[i * 3 + 2] = THREE.MathUtils.randFloatSpread(2000); // z
    sizes[i] = THREE.MathUtils.randFloat(1, 5);
}

const geometry = new THREE.BufferGeometry();
geometry.setAttribute('position', new THREE.BufferAttribute(positions, 3));
geometry.setAttribute('size', new THREE.BufferAttribute(sizes, 1));

const material = new THREE.ShaderMaterial({
    uniforms: {
        color: { value: new THREE.Color(0x4dabf7) },
    },
    vertexShader: `
        attribute float size;
        void main() {
            vec4 mvPosition = modelViewMatrix * vec4(position, 1.0);
            gl_PointSize = size * (300.0 / -mvPosition.z);
            gl_Position = projectionMatrix * mvPosition;
        }
    `,
    fragmentShader: `
        uniform vec3 color;
        void main() {
            if (length(gl_PointCoord - vec2(0.5, 0.5)) > 0.475) discard;
            gl_FragColor = vec4(color, 1.0);
        }
    `,
    transparent: true,
});

const particles = new THREE.Points(geometry, material);
scene.add(particles);

camera.position.z = 1000;

let isProcessing = false;
let normalSpeed = 0.0001;
let fastSpeed = 0.0009;

function updateParticleSpeed(speed) {
    particles.rotation.x += speed;
    particles.rotation.y += speed;
}

function animateBackground() {
    requestAnimationFrame(animateBackground);
    updateParticleSpeed(isProcessing ? fastSpeed : normalSpeed);
    renderer.render(scene, camera);
}
animateBackground();
```

- **Three.js Setup**:
  1. **Scene**: Initializes a new Three.js scene.
  2. **Camera**: Sets up a perspective camera with a 75-degree field of view.
  3. **Renderer**: Creates a WebGL renderer targeting the `background-canvas` element, enabling transparency.
  4. **Canvas Size**: Adjusts the renderer size to match the window.

- **Particle System**:
  1. **Configuration**: Defines 10,000 particles with random positions and sizes.
  2. **Geometry**: Uses `BufferGeometry` to efficiently manage the particles' positions and sizes.
  3. **Shaders**:
      - **Vertex Shader**: Determines the size and position of each particle based on its depth.
      - **Fragment Shader**: Renders particles as circles with the specified color.
  4. **Material**: Applies the shader material with a blue color (`0x4dabf7`).
  5. **Points**: Creates a `Points` object with the geometry and material, adding it to the scene.

- **Animation**:
  1. **Camera Position**: Sets the camera's z-position to view the particles.
  2. **Particle Rotation**: Controls the rotation speed of the particles based on processing state.
  3. **Animate Function**: Continuously updates the rotation and renders the scene using `requestAnimationFrame`.

**Effect:**
- Creates a dynamic, rotating background of blue particles that rotate faster during processing (e.g., when handling STT or TTS requests), enhancing user feedback.

#### Resize Handler

```javascript
window.addEventListener('resize', () => {
    camera.aspect = window.innerWidth / window.innerHeight;
    camera.updateProjectionMatrix();
    renderer.setSize(window.innerWidth, window.innerHeight);
});
```

- **Event Listener**: Responds to browser window resize events.
- **Flow**:
  1. **Update Camera**: Adjusts the camera's aspect ratio based on the new window size.
  2. **Update Renderer**: Resizes the Three.js renderer to fit the new window dimensions.

**Purpose:**
- Ensures that the 3D background remains appropriately scaled and rendered regardless of window size changes.

#### UI Enhancements

##### Converter Boxes Animation

```javascript
const converterBoxes = document.querySelectorAll('.converter-box');
converterBoxes.forEach(box => {
    box.addEventListener('mouseenter', () => {
        box.style.transform = 'scale(1.02)';
    });
    box.addEventListener('mouseleave', () => {
        box.style.transform = 'scale(1)';
    });
});
```

- **Element Selection**: Selects all elements with the class `converter-box`.
- **Event Listeners**:
  - **Mouse Enter**: Slightly enlarges the box to indicate interactivity.
  - **Mouse Leave**: Returns the box to its original size.

**User Experience:**
- Provides a subtle zoom effect when users hover over the converter boxes, enhancing the interactive feel.

##### Button Click Effect

```javascript
const buttons = document.querySelectorAll('button');
buttons.forEach(button => {
    button.addEventListener('click', () => {
        button.style.transform = 'scale(0.95)';
        setTimeout(() => {
            button.style.transform = 'scale(1)';
        }, 100);
    });
});
```

- **Element Selection**: Selects all `button` elements.
- **Event Listener**: Adds a quick scaling effect when buttons are clicked.

**User Experience:**
- Gives immediate visual feedback when buttons are pressed, making the interface feel more responsive.

#### Smooth Scroll to Output

```javascript
function scrollToOutput() {
    textOutput.scrollIntoView({ behavior: 'smooth', block: 'start' });
}
```

- **Function**: Scrolls the page smoothly to the `textOutput` textarea.
- **Usage**: Called after transcription results are received to bring the output into view.

**User Experience:**
- Ensures that users can easily see the transcription result without manually scrolling.

#### Web Speech API for Continuous Speech Recognition

```javascript
recognition = new (window.SpeechRecognition || window.webkitSpeechRecognition)();
recognition.continuous = true;
recognition.interimResults = true;

recognition.onresult = (event) => {
    const result = event.results[event.results.length - 1];
    const transcript = result[0].transcript;
    textOutput.value = transcript;
};

recognition.start();
```

- **Initialization**: Creates a new instance of the Web Speech API's `SpeechRecognition` object, compatible with different browsers.
- **Configuration**:
  - **`continuous`**: Keeps the recognition service active until explicitly stopped.
  - **`interimResults`**: Provides real-time transcription results as the user speaks.
- **Event Listener**:
  - **`onresult`**: Updates the `textOutput` textarea with the latest transcription result.
- **Start Recognition**: Begins listening for speech input immediately.

**Purpose:**
- Enables real-time speech-to-text functionality directly in the browser, providing immediate feedback as users speak.

**Example:**
- As a user speaks into their microphone, the recognized text appears in the `speech-to-text-output` area in real-time.

---

## `index.html`

```html:index.html
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>AI-Powered Speech Converter</title>
    <link rel="stylesheet" href="styles.css">
    <script src="https://cdnjs.cloudflare.com/ajax/libs/three.js/r128/three.min.js"></script>
</head>
<body>
    <canvas id="background-canvas"></canvas>
    <div class="container">
        <h1>AI-Powered Speech Converter</h1>
        <div class="converter-box">
            <h2>Text to Speech</h2>
            <textarea id="text-input" placeholder="Enter text to convert to speech..."></textarea>
            <button id="speak-button">Speak</button>
        </div>
        <div class="converter-box">
            <h2>Speech to Text</h2>
            <textarea id="speech-to-text-output" readonly placeholder="Transcribed text will appear here..."></textarea>
            <button id="start-recording">Start Recording</button>
            <button id="stop-recording" disabled>Stop Recording</button>
            <p id="recording-status">Not recording</p>
            <div id="audio-container"></div>
        </div>
    </div>
    <script src="script.js"></script>
</body>
</html>
```

### Explanation

The `index.html` file structures the user interface for the AI-Powered Speech Converter application, integrating style, visual effects, and functionality provided by external scripts and libraries.

#### HTML Structure

```html
<!DOCTYPE html>
<html lang="en">
<head>
    <!-- Metadata and External Resources -->
</head>
<body>
    <!-- Content and Scripts -->
</body>
</html>
```

- **DOCTYPE Declaration**: Specifies the HTML version.
- **`<html>` Tag**: Root element with language set to English.

#### Head Section

```html
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>AI-Powered Speech Converter</title>
    <link rel="stylesheet" href="styles.css">
    <script src="https://cdnjs.cloudflare.com/ajax/libs/three.js/r128/three.min.js"></script>
</head>
```

- **Character Encoding**: Sets UTF-8 for universal character support.
- **Viewport Settings**: Ensures responsive design across devices.
- **Title**: Displays "AI-Powered Speech Converter" in the browser tab.
- **Stylesheet**: Links to `styles.css` for styling the page.
- **Three.js Library**: Includes Three.js for rendering the 3D background.

#### Body Section

```html
<body>
    <canvas id="background-canvas"></canvas>
    <div class="container">
        <h1>AI-Powered Speech Converter</h1>
        <div class="converter-box">
            <h2>Text to Speech</h2>
            <textarea id="text-input" placeholder="Enter text to convert to speech..."></textarea>
            <button id="speak-button">Speak</button>
        </div>
        <div class="converter-box">
            <h2>Speech to Text</h2>
            <textarea id="speech-to-text-output" readonly placeholder="Transcribed text will appear here..."></textarea>
            <button id="start-recording">Start Recording</button>
            <button id="stop-recording" disabled>Stop Recording</button>
            <p id="recording-status">Not recording</p>
            <div id="audio-container"></div>
        </div>
    </div>
    <script src="script.js"></script>
</body>
```

- **Canvas for Background**:
  - **`<canvas id="background-canvas"></canvas>`**: Serves as the rendering surface for the Three.js particle background.

- **Main Container**:
  - **`<div class="container">`**: Encapsulates the main content, likely styled for layout and alignment.

- **Header**:
  - **`<h1>AI-Powered Speech Converter</h1>`**: Displays the main title of the application.

- **Converter Boxes**:
  - **Text to Speech**:
    ```html
    <div class="converter-box">
        <h2>Text to Speech</h2>
        <textarea id="text-input" placeholder="Enter text to convert to speech..."></textarea>
        <button id="speak-button">Speak</button>
    </div>
    ```
    - **`<h2>`**: Subheading for the Text to Speech feature.
    - **`<textarea id="text-input">`**: Input area for users to enter text they want to convert to speech.
    - **`<button id="speak-button">Speak</button>`**: Button to trigger the TTS conversion.

  - **Speech to Text**:
    ```html
    <div class="converter-box">
        <h2>Speech to Text</h2>
        <textarea id="speech-to-text-output" readonly placeholder="Transcribed text will appear here..."></textarea>
        <button id="start-recording">Start Recording</button>
        <button id="stop-recording" disabled>Stop Recording</button>
        <p id="recording-status">Not recording</p>
        <div id="audio-container"></div>
    </div>
    ```
    - **`<h2>`**: Subheading for the Speech to Text feature.
    - **`<textarea id="speech-to-text-output" readonly>`**: Displays the transcribed text from audio input.
    - **Buttons**:
      - **`<button id="start-recording">Start Recording</button>`**: Initiates audio recording.
      - **`<button id="stop-recording" disabled>Stop Recording</button>`**: Stops audio recording; initially disabled.
    - **Recording Status**:
      - **`<p id="recording-status">Not recording</p>`**: Shows current recording status.
    - **Audio Container**:
      - **`<div id="audio-container"></div>`**: Holds the recorded audio player and send button after recording.

- **Script Inclusion**:
  - **`<script src="script.js"></script>`**: Links to the JavaScript file that provides interactive functionality.

#### User Interface Flow

1. **Text to Speech**:
    - User enters text into the textarea.
    - Clicks the "Speak" button.
    - The application sends the text to the server, receives an audio file, and plays it.

2. **Speech to Text**:
    - User clicks "Start Recording" to begin capturing audio.
    - While recording, the status updates to "Recording...", and the "Stop Recording" button becomes enabled.
    - User speaks into the microphone.
    - Clicking "Stop Recording" stops the capture.
    - An audio player appears with the option to play back the recording and send it to the server for transcription.
    - Transcribed text appears in the output textarea.

#### Visual Enhancements

- **Three.js Particle Background**:
  - Renders a dynamic, interactive background that responds to user interactions and processing states, enhancing the application's aesthetic appeal.

- **CSS Styling (Assumed in `styles.css`)**:
  - While not provided, the CSS likely styles the layout, colors, fonts, and interactive elements to create a user-friendly interface.

---

## Summary

The provided code sets up a comprehensive AI-powered speech converter application with both frontend and backend components:

- **Backend (`server.py`)**:
  - Handles speech-to-text and text-to-speech conversions using AI models.
  - Ensures secure communication via HTTPS.
  - Manages audio file handling with proper error handling and cleanup.

- **Frontend (`index.html` & `script.js`)**:
  - Provides an intuitive user interface for interacting with the conversion features.
  - Utilizes browser APIs for recording and transcribing speech in real-time.
  - Enhances user experience with visual feedback, animations, and dynamic backgrounds using Three.js.

**Example Workflow:**

1. **Text to Speech**:
    - A user types "Hello, how are you?" into the text input field.
    - Clicking "Speak" sends the text to the server, which returns an audio file.
    - The audio is played back, vocalizing the input text.

2. **Speech to Text**:
    - A user clicks "Start Recording" and speaks into the microphone.
    - Upon stopping the recording, an audio player appears with the recorded audio.
    - Clicking "Send to Server" sends the audio file to the server.
    - The server transcribes the audio and returns the text, which is displayed in the output area.

This seamless integration of frontend and backend technologies provides users with a powerful tool for converting between text and speech, leveraging AI capabilities for enhanced functionality.