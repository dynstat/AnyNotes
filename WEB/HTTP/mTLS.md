[[mTLS - Sample Code examples]]
### Visualizing the mTLS Handshake Flow

This diagram illustrates the complete, two-way authentication process. The key mTLS steps, where the client is asked for and provides its identity, are highlighted.

```
   CLIENT 🧔                                                     SERVER 🏢
      |                                                              |
      |------------------------ 1. Client Hello --------------------->|
      |   (My TLS versions, My Ciphers, I want to talk to 'server.com')|
      |                                                              |
      |<------------------- 2. Server Hello & Cert ------------------|
      |   (Let's use this cipher, Here's my ID card [📜 Server Cert])  |
      |                                                              |
      |<----------- 3. *** Certificate Request *** ------------------|
      |   (Now, show me YOUR ID card. It must be from a trusted CA.)   |
      |                                                              |
      |---------------------- 4. Client Cert ----------------------->|
      |   (OK, here is my ID card [📜 Client Cert])                   |
      |                                                              |
      |------------------ 5. Client Key Exchange ------------------->|
      |   (Here is a secret 🤫, encrypted with your public key)       |
      |                                                              |
      |------------------ 6. *** Certificate Verify *** ------------->|
      |   (Here's proof I own my ID card [✍️ Digital Signature])       |
      |                                                              |
      |----------------- 7. Change Cipher Spec & Finished ----------->|
      |   (I'm switching to encryption now! [Encrypted Finished])     |
      |                                                              |
      |<---------------- 8. Change Cipher Spec & Finished -----------|
      |   (I see your proof. I'm also switching to encryption! [Encrypted Finished])|
      |                                                              |
      |=================== SECURE CHANNEL ESTABLISHED ===============|
      |                                                              |
      |<-------------------- Encrypted Data ------------------------>|
      |   (Encrypted API requests, responses, etc.)                  |
```

-----

### Detailed Step-by-Step Breakdown with Sample Data

#### **Step 1: Client Hello**

The client initiates the conversation, announcing its capabilities and what it wants to connect to.

  * **Action:** The client sends a message containing everything the server needs to know to talk to it.
  * **Sample Packet Data Explained:**
    ```
    Handshake Type: Client Hello (1)
    Version: TLS 1.2 (0x0303)
    Random: 68 9a 1d c3 ... (32 random bytes used later to generate session keys)
    Session ID Length: 0 (Indicates a new session)
    Cipher Suites:
        TLS_ECDHE_RSA_WITH_AES_256_GCM_SHA384 (0xc030) // Client's top preference
        TLS_ECDHE_RSA_WITH_AES_128_GCM_SHA256 (0xc02f)
    Extensions:
        server_name: server.api.com // Server Name Indication (SNI) tells the server which domain the client wants.
    ```
  * **Purpose:** To tell the server, "I want to start a secure connection. I can speak these encryption languages. I'm trying to reach `server.api.com`."

-----

#### **Step 2 & 3: Server's Response (Hello, Certificate, and Certificate Request)**

The server confirms the parameters, provides its identity, and crucially, **demands the client's identity**.

  * **Action:** The server sends a series of messages in one flight.

  * **Sample Packet Data Explained:**

    1.  **Server Hello:**

        ```
        Handshake Type: Server Hello (2)
        Version: TLS 1.2 (0x0303)
        Random: 4a 1b 8f 2e ... (The server's own 32 random bytes)
        Cipher Suite: TLS_ECDHE_RSA_WITH_AES_256_GCM_SHA384 (0xc030) // The one it chose from the client's list.
        ```

          * **Purpose:** To agree on the rules of engagement.

    2.  **Certificate:**

        ```
        Handshake Type: Certificate (11)
        Certificate:
            Subject: CN=server.api.com, O=My Company, C=US
            Issuer: CN=DigiCert Global CA G2, O=DigiCert Inc, C=US
            Public Key: (A very long string of bytes representing the server's public key)
        ```

          * **Purpose:** To prove the server's identity. "I am `server.api.com`, and DigiCert vouches for me."

    3.  **Certificate Request (The Core of mTLS):**

        ```
        Handshake Type: Certificate Request (13)
        Certificate Types: RSA Sign (1), ECDSA Sign (64)
        Signature Hash Algorithms: SHA512-RSA, SHA256-RSA
        Distinguished Names (Trusted CAs):
            CN=My Company Internal CA, O=My Company, C=US
        ```

          * **Purpose:** This is the server's demand. "Now you prove who you are. Send me a certificate, and it **must** be one that was issued by `My Company Internal CA`." This prevents random clients from connecting.

-----

#### **Step 4, 5, & 6: Client's Response (Certificate, Key Exchange, and Verify)**

The client complies with the server's demand, providing its certificate and proof of ownership.

  * **Action:** The client sends its credentials and the shared secret.

  * **Sample Packet Data Explained:**

    1.  **Certificate:**

        ```
        Handshake Type: Certificate (11)
        Certificate:
            Subject: CN=client-device-001, OU=IoT Devices, O=My Company, C=US
            Issuer: CN=My Company Internal CA, O=My Company, C=US
        ```

          * **Purpose:** The client presents its ID card. "I am `client-device-001`, and I was authorized by our internal CA."

    2.  **Client Key Exchange:**

        ```
        Handshake Type: Client Key Exchange (16)
        Encrypted Pre-Master Secret: 1f 9b 3d c4 ... (A secret value, encrypted using the server's public key)
        ```

          * **Purpose:** To securely share the "seed" for the final session keys. Only the server can decrypt this.

    3.  **Certificate Verify:**

        ```
        Handshake Type: Certificate Verify (15)
        Signature: 3b 7e 1a 9f ... (A digital signature created by the client using its private key)
        ```

          * **Purpose:** This is the most critical client-side step in mTLS. The client signs a summary of all previous handshake messages. It proves the client **possesses the private key** corresponding to the public key in the certificate it just sent. This prevents an attacker from simply replaying a stolen certificate.

-----

#### **Step 7 & 8: Finalizing and Switching to Encryption**

Both sides confirm that the handshake was successful and switch to using the newly created session keys.

  * **Action:** Both client and server send two final messages.

  * **Sample Packet Data Explained:**

    1.  **Change Cipher Spec:**

        ```
        A single-byte message indicating a switch in encryption.
        ```

          * **Purpose:** A formal notification: "Everything from now on will be encrypted with the new session key."

    2.  **Finished:**

        ```
        Handshake Type: Finished (20)
        Encrypted Data: 9a c2 1f 4b ... (A hash of the entire handshake, but this time it's encrypted with the new session key)
        ```

          * **Purpose:** The final integrity check. If the server can decrypt the client's `Finished` message (and vice-versa) and the contents are correct, it confirms that both sides generated the exact same session keys and the handshake was not tampered with.

At this point, the mTLS tunnel is fully established, and secure, two-way authenticated communication can begin.