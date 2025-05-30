### **Tauri Project Flow with Direction Explanation**

This guide provides a step-by-step flow of a Tauri project, explaining the direction of data flow between the frontend and backend.

---

## **Step 1: Install Required Dependencies**

Before setting up Tauri, ensure you have the following installed:

- **Node.js** (for frontend development)
- **Rust** (for Tauri’s backend)
- **Tauri CLI**

#### **Install Rust & Tauri CLI**

```sh
curl --proto '=https' --tlsv1.2 -sSf https://sh.rustup.rs | sh
cargo install tauri-cli
```

#### **Install Node.js** (if not installed)

Download from [https://nodejs.org](https://nodejs.org/) or use:

```sh
sudo apt install nodejs npm  # Linux
brew install node  # macOS
```

---

## **Step 2: Create a Tauri Project**

Run the following command to initialize a new Tauri project:

```sh
npm create tauri-app@latest my-tauri-app
cd my-tauri-app
npm install
```

This sets up a **frontend** (default is Vanilla JavaScript) and a **Rust backend**.

---

## **Step 3: Project Structure Overview**

After installation, your project structure will look like this:

```
my-tauri-app/
│── src/
│   ├── main.ts       # Entry point for frontend (JavaScript/TypeScript)
│   ├── App.svelte    # UI component (if using Svelte)
│── src-tauri/
│   ├── src/
│   │   ├── main.rs   # Rust backend for Tauri
│   ├── tauri.conf.json  # Configuration file for Tauri
│── package.json      # Node.js dependencies
│── Cargo.toml        # Rust dependencies
```

---

## **Step 4: Run the Project in Dev Mode**

To launch the app, use:

```sh
npm run tauri dev
```

This compiles the project and launches a Tauri window.

---

## **Step 5: Understanding Frontend Development**

Tauri supports any frontend framework. Let's use **Svelte** for the UI.

#### **Install Svelte**

```sh
npm create svelte@latest frontend
cd frontend
npm install
npm run dev
```

#### **Modify Svelte UI (src/App.svelte)**

```html
<script>
  let message = "Hello from Tauri!";
</script>

<main>
  <h1>{message}</h1>
  <button on:click={() => message = "Tauri is working!"}>Click Me</button>
</main>

<style>
  main {
    text-align: center;
    padding: 20px;
  }
</style>
```

Run:

```sh
npm run dev
```

This opens a **browser preview** with hot-reloading.

---

## **Step 6: Understanding the Rust Backend**

The Rust backend provides system-level APIs.

Modify **src-tauri/src/main.rs** to add a simple command:

```rust
use tauri::Manager;

#[tauri::command]
fn greet(name: &str) -> String {
    format!("Hello, {}! Welcome to Tauri.", name)
}

fn main() {
    tauri::Builder::default()
        .invoke_handler(tauri::generate_handler![greet])
        .run(tauri::generate_context!())
        .expect("error while running Tauri application");
}
```

---

## **Step 7: Connecting Frontend & Backend (Flow Direction)**

The **data flow** between frontend and backend works as follows:

1. **User Action (Frontend)**: A user interacts with the frontend UI (e.g., enters a name and clicks a button).
2. **Invoke Command (Frontend → Backend)**: The frontend sends a request to the backend using Tauri's `invoke` function.
3. **Processing (Backend)**: The Rust backend processes the request and returns a response.
4. **Update UI (Backend → Frontend)**: The frontend receives the response and updates the UI accordingly.

Modify **App.svelte** to call the Rust function:

```html
<script>
  import { invoke } from "@tauri-apps/api/tauri";
  let name = "User";
  let response = "";

  async function sendRequest() {
    response = await invoke("greet", { name });
  }
</script>

<main>
  <input bind:value={name} placeholder="Enter name" />
  <button on:click={sendRequest}>Greet</button>
  <p>{response}</p>
</main>
```

Now, clicking the button calls **Rust's backend function**, which returns a greeting message that updates the UI.

### **Data Flow Diagram:**

```
[Frontend UI] ---> (Invoke function) ---> [Rust Backend]
     |                                      |
     |  (Receives Response)                |
     v                                      v
[Update UI] <--- (Returns Response) <--- [Processing]
```

---

## **Step 8: Build for Release**

To compile a production-ready app:

```sh
npm run tauri build
```

The final `.exe` or `.app` is found in `src-tauri/target/release`.

---

