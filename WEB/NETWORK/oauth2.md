
**Analogy 1: Hotel Key Card System**
1. You (user) check into a hotel (main service, like Google)
2. You have your ID (username/password) that proves who you are
3. The hotel gives you a key card (access token) that:
   - Only works for your specific room (limited access)
   - Has an expiration date (token expiration)
   - Can be deactivated remotely (token revocation)
   - Doesn't reveal your personal ID (security)
4. When you want room service (third-party app) to enter:
   - You give them a temporary service key (limited scope token)
   - They can only access during specific hours (scope limitations)
   - They can't access other areas of the hotel (resource restrictions)

**Analogy 2: Concert Wristband System**
```plaintext
Real World                  OAuth2 Equivalent
-----------                 -----------------
Concert Ticket Office   →   Authorization Server
Your ID                 →   Username/Password
Wristband               →   Access Token
VIP/Regular/Backstage   →   Scopes
Security Guard          →   Resource Server
Different Areas         →   API Endpoints
Wristband Scanner       →   Token Validation
```

Here's a practical implementation showing this analogy in code:

```javascript
// Concert Wristband System Implementation
class WristbandAuthSystem {
    // Authorization Server (Ticket Office)
    static async issueWristband(credentials) {
        const { userId, accessLevel } = await validateCredentials(credentials);
        
        return {
            wristbandToken: generateToken({
                userId,
                accessLevel,
                expiresIn: '24h'
            }),
            accessAreas: getAccessAreas(accessLevel),
            expiryTime: Date.now() + 24 * 60 * 60 * 1000
        };
    }

    // Resource Server (Security Guard)
    static validateAccess(wristbandToken, requestedArea) {
        const decoded = verifyToken(wristbandToken);
        
        if (!decoded) {
            throw new Error('Invalid wristband!');
        }

        if (!hasAccessToArea(decoded.accessLevel, requestedArea)) {
            throw new Error('Area access denied!');
        }

        return true;
    }
}

// Usage Example
async function enterConcertArea() {
    try {
        // 1. Get wristband (OAuth token)
        const wristband = await WristbandAuthSystem.issueWristband({
            userId: 'user123',
            ticketType: 'VIP'
        });

        // 2. Try to access different areas (API endpoints)
        const canAccessMainStage = await WristbandAuthSystem.validateAccess(
            wristband.wristbandToken,
            'main-stage'
        );

        const canAccessVIP = await WristbandAuthSystem.validateAccess(
            wristband.wristbandToken,
            'vip-lounge'
        );

    } catch (error) {
        console.error('Access denied:', error.message);
    }
}
```

**Analogy 3: Library Card System**
```python
class LibraryCardSystem:
    def __init__(self):
        self.cards = {}
        self.resources = {}

    # Authorization Server (Library Registration Desk)
    def issue_library_card(self, user_credentials):
        """
        Similar to OAuth token issuance
        """
        if self.verify_identity(user_credentials):
            return {
                'card_number': generate_unique_id(),
                'access_level': determine_access_level(user_credentials),
                'expiry_date': get_expiry_date(),
                'borrowing_limit': set_borrowing_limit()
            }

    # Resource Server (Librarian)
    def check_out_book(self, library_card, book_id):
        """
        Similar to using OAuth token to access resources
        """
        if not self.validate_card(library_card):
            raise Exception("Invalid library card")

        if not self.has_borrowing_privileges(library_card, book_id):
            raise Exception("Insufficient privileges")

        return self.process_checkout(library_card, book_id)
```

**Real-World OAuth2 Implementation Using Library Analogy:**
```javascript
// OAuth2 Server Implementation
class OAuth2Server {
    // Authorization Endpoint (Library Card Registration)
    async authorize(client_id, scope, redirect_uri) {
        const authCode = await this.generateAuthCode({
            client_id,
            scope,
            redirect_uri
        });

        return {
            auth_code: authCode,
            expires_in: 600 // 10 minutes
        };
    }

    // Token Endpoint (Issuing Library Card)
    async issueToken(auth_code, client_id, client_secret) {
        const validClient = await this.validateClient(client_id, client_secret);
        const authCodeData = await this.validateAuthCode(auth_code);

        return {
            access_token: generateToken(),
            refresh_token: generateRefreshToken(),
            token_type: 'Bearer',
            expires_in: 3600,
            scope: authCodeData.scope
        };
    }

    // Resource Endpoint (Book Checkout System)
    async accessResource(access_token, resource) {
        const tokenData = await this.validateToken(access_token);
        
        if (!this.hasPermission(tokenData.scope, resource)) {
            throw new Error('Insufficient permissions');
        }

        return await this.getResource(resource);
    }
}

// Client Implementation
class OAuth2Client {
    async requestAccess() {
        // Step 1: Request authorization
        const authResponse = await oauth2Server.authorize(
            'client_123',
            'read_books checkout_books',
            'https://myclient.com/callback'
        );

        // Step 2: Exchange auth code for token
        const tokenResponse = await oauth2Server.issueToken(
            authResponse.auth_code,
            'client_123',
            'client_secret'
        );

        // Step 3: Use token to access resources
        const book = await oauth2Server.accessResource(
            tokenResponse.access_token,
            'book_123'
        );

        return book;
    }
}
```

These analogies help understand key OAuth2 concepts:
1. **Separation of Concerns**: Like how library cards are separate from your ID
2. **Scoped Access**: Different access levels for different resources
3. **Time-Limited Access**: Cards/tokens expire and need renewal
4. **Revocable Access**: Cards can be cancelled without affecting ID
5. **Delegated Access**: Others can use your card with limited permissions
