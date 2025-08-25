

## Sample Scenario

**You're sending an email from your office computer to Gmail**

- Your computer IP: `192.168.10.25`
- Gmail server IP: `172.217.164.109`
- Your subnet: `192.168.10.0/24` (subnet mask: `255.255.255.0`)
- Default gateway: `192.168.10.1`

---

## 1. Basic Network Components

```
┌────────────────────────────────────────────────────────────────┐
│                    COMPLETE NETWORK OVERVIEW                   │
├────────────────────────────────────────────────────────────────┤
│                                                                │
│  Your Office Subnet: 192.168.10.0/24                           │
│  ┌─────────────────────────────────────────────────────────┐   │
│  │                                                         │   │
│  │  [Computer A]     [Your PC]      [Printer]    [Server]  │   │
│  │  192.168.10.20   192.168.10.25  192.168.10.30    .40    │   │
│  │       │              │              │           │       │   │
│  │       └──────────────┼──────────────┼────────── ┘       │   │
│  │                      │              │                   │   │
│  │              ┌───────┴──────────────┴────────┐          │   │
│  │              │        SWITCH                 │          │   │
│  │              │     (Layer 2 Device)          │          │   │
│  │              └───────────────┬───────────────┘          │   │
│  │                              │                          │   │
│  │              ┌───────────────┴────────────────┐         │   │
│  │              │    ROUTER/GATEWAY              │         │   │
│  │              │    192.168.10.1                │         │   │
│  │              │  (Default Gateway)             │         │   │
│  │              └───────────────┬────────────────┘         │   │
│  └──────────────────────────────┼──────────────────────────┘   │
│                                 │                              │
│                    ┌────────────┴─────────────┐                │
│                    │      INTERNET/WAN        │                │
│                    │   (Multiple Routers)     │                │
│                    └────────────┬─────────────┘                │
│                                 │                              │
│  Gmail's Network: Different Subnet                             │
│  ┌──────────────────────────────┼──────────────────────────┐   │
│  │                              │                          │   │
│  │              ┌───────────────┴────────────────┐         │   │
│  │              │    GMAIL ROUTER/GATEWAY        │         │   │
│  │              │    172.217.164.1               │         │   │
│  │              └───────────────┬────────────────┘         │   │
│  │                              │                          │   │
│  │              ┌───────────────┴────────────────┐         │   │
│  │              │        SWITCH                  │         │   │
│  │              └───────┬───────────────┬────────┘         │   │
│  │                      │               │                  │   │
│  │              [Gmail Server]  [Gmail Server]             │   │
│  │              172.217.164.109  172.217.164.110           │   │
│  │                                                         │   │
│  └─────────────────────────────────────────────────────────┘   │
└────────────────────────────────────────────────────────────────┘
```

---

## 2. Subnet Breakdown with Subnet Mask

```
IP Address: 192.168.10.25
Subnet Mask: 255.255.255.0 (or /24)

Binary Representation:
┌─────────────────────────────────────────────────────────────┐
│ IP Address:  192    .168    .10     .25                     │
│ Binary:   11000000.10101000.00001010.00011001               │
│                                                             │
│ Subnet Mask: 255    .255    .255    .0                      │
│ Binary:   11111111.11111111.11111111.00000000               │
│                                                             │
│ Result:                                                     │
│ Network:       11000000.10101000.00001010.00000000          │
│ (192.168.10.0) ↑──────Network Part──────↑──Host Part──      │
│                                                             │
│ This means:                                                 │
│ • Network Address: 192.168.10.0                             │
│ • Broadcast Address: 192.168.10.255                         │
│ • Usable IPs: 192.168.10.1 to 192.168.10.254                │
│ • Total devices possible: 254                               │
└─────────────────────────────────────────────────────────────┘

Same Network Check:
Your PC: 192.168.10.25     Network: 192.168.10.0
Printer: 192.168.10.30     Network: 192.168.10.0  ✓ Same subnet
Gmail:   172.217.164.109   Network: 172.217.164.0 ✗ Different subnet
```

---

## 3. Step-by-Step Email Journey

### Step 1: Your Computer Determines Destination

```
┌─────────────────────────────────────┐
│     Your Computer (192.168.10.25)  │
│  ┌─────────────────────────────────┐│
│  │ Email App: "Send to Gmail"      ││
│  │ Destination: 172.217.164.109    ││
│  │                                 ││
│  │ Subnet Mask Check:              ││
│  │ My network: 192.168.10.0        ││
│  │ Gmail network: 172.217.164.0    ││
│  │ → Different networks!           ││
│  │ → Must use default gateway      ││
│  └─────────────────────────────────┘│
└─────────────────────────────────────┘
```

### Step 2: Packet Creation

```
┌──────────────────────────────────────────────────────────────┐
│                    EMAIL PACKET STRUCTURE                   │
├──────────────────────────────────────────────────────────────┤
│ Layer 2 (Data Link) - Ethernet Header                       │
│ ┌──────────────────────────────────────────────────────────┐ │
│ │ SRC MAC: AA:BB:CC:DD:EE:FF (Your Computer's MAC)        │ │
│ │ DST MAC: 11:22:33:44:55:66 (Gateway's MAC)              │ │
│ └──────────────────────────────────────────────────────────┘ │
│                                                              │
│ Layer 3 (Network) - IP Header                               │
│ ┌──────────────────────────────────────────────────────────┐ │
│ │ Source IP: 192.168.10.25                                 │ │
│ │ Dest IP: 172.217.164.109                                 │ │
│ │ Protocol: TCP (6)                                        │ │
│ └──────────────────────────────────────────────────────────┘ │
│                                                              │
│ Layer 4 (Transport) - TCP Header                            │
│ ┌──────────────────────────────────────────────────────────┐ │
│ │ Source Port: 54321                                       │ │
│ │ Dest Port: 25 (SMTP)                                     │ │
│ └──────────────────────────────────────────────────────────┘ │
│                                                              │
│ Layer 5+ (Application) - Email Data                         │
│ ┌──────────────────────────────────────────────────────────┐ │
│ │ SMTP Commands:                                           │ │
│ │ MAIL FROM: <you@company.com>                             │ │
│ │ RCPT TO: <friend@gmail.com>                              │ │
│ │ DATA: [Email content here]                               │ │
│ └──────────────────────────────────────────────────────────┘ │
└──────────────────────────────────────────────────────────────┘
```

### Step 3: Within Your Subnet (Switch Operation)

```
Your Subnet: 192.168.10.0/24
┌─────────────────────────────────────────────────────────────┐
│                                                             │
│  [Your PC]                                    [Gateway]     │
│  192.168.10.25                               192.168.10.1   │
│  MAC: AA:BB:CC:DD:EE:FF                      MAC: 11:22:... │
│       │                                           ▲         │
│       │    ┌─────────────────────────────────────┐│         │
│       │    │ 1. ARP Request (if needed):         ││         │
│       │    │    "Who has 192.168.10.1?"          ││         │
│       │    │ 2. ARP Reply:                       ││         │
│       │    │    "192.168.10.1 is at 11:22:..."  ││         │
│       └────┴─────────────────────────────────────┘│         │
│                                                    │         │
│              ┌─────────────────────────────────────┴─────┐   │
│              │            SWITCH                         │   │
│              │  ┌─────────────────────────────────────┐  │   │
│              │  │ MAC Address Table:                  │  │   │
│              │  │ AA:BB:CC:DD:EE:FF → Port 1         │  │   │
│              │  │ 11:22:33:44:55:66 → Port 4         │  │   │
│              │  │ ...                                 │  │   │
│              │  └─────────────────────────────────────┘  │   │
│              │                                           │   │
│              │ Forwards frame from Port 1 to Port 4     │   │
│              └───────────────────────────────────────────┘   │
└─────────────────────────────────────────────────────────────┘
```

### Step 4: Gateway/Router Processing

```
┌────────────────────────────────────────────────────────────┐
│                    ROUTER/GATEWAY                          │
│                   (192.168.10.1)                          │
├────────────────────────────────────────────────────────────┤
│                                                            │
│ 1. Receives Ethernet frame                                 │
│    ┌─────────────────────────────────────────────────────┐ │
│    │ DST MAC: 11:22:33:44:55:66 ← This is me!          │ │
│    │ SRC MAC: AA:BB:CC:DD:EE:FF                         │ │
│    └─────────────────────────────────────────────────────┘ │
│                                                            │
│ 2. Strips Ethernet header, examines IP packet             │
│    ┌─────────────────────────────────────────────────────┐ │
│    │ SRC IP: 192.168.10.25                              │ │
│    │ DST IP: 172.217.164.109                            │ │
│    └─────────────────────────────────────────────────────┘ │
│                                                            │
│ 3. Consults Routing Table                                  │
│    ┌─────────────────────────────────────────────────────┐ │
│    │ Destination      Next Hop        Interface          │ │
│    │ 192.168.10.0/24  Direct          eth0               │ │
│    │ 0.0.0.0/0        203.0.113.1     eth1 (Internet)   │ │
│    │                                                     │ │
│    │ Match: 172.217.164.109 → Default route (Internet)  │ │
│    └─────────────────────────────────────────────────────┘ │
│                                                            │
│ 4. NAT (Network Address Translation) - KEY STEP!           │
│    ┌─────────────────────────────────────────────────────┐ │
│    │ BEFORE (Private Network):                           │ │
│    │ SRC IP: 192.168.10.25 (Private - NOT routable)     │ │
│    │ DST IP: 172.217.164.109 (Public Gmail server)      │ │
│    │ SRC Port: 54321                                     │ │
│    │                                                     │ │
│    │ AFTER NAT (Public Internet):                        │ │
│    │ SRC IP: 203.0.113.50 (Your PUBLIC IP)              │ │
│    │ DST IP: 172.217.164.109 (Public Gmail server)      │ │
│    │ SRC Port: 12345 (Translated port)                  │ │
│    │                                                     │ │
│    │ Router keeps NAT table:                             │ │
│    │ Internal: 192.168.10.25:54321 ↔ External: :12345   │ │
│    └─────────────────────────────────────────────────────┘ │
└────────────────────────────────────────────────────────────┘
```

### Step 5: Internet Journey (Multiple Hops)

```
Internet Backbone - Multiple Router Hops
═══════════════════════════════════════

Your ISP Router → Regional Router → Backbone Router → Gmail's ISP → Gmail

┌─────────────┐   ┌──────────────┐   ┌──────────────┐   ┌─────────────┐
│ Your ISP    │   │ Regional ISP │   │ Tier-1       │   │ Google's    │
│ 203.0.113.1 │──▶│ 198.51.100.1 │──▶│ 8.8.8.1      │──▶│ Edge Router │
└─────────────┘   └──────────────┘   └──────────────┘   │ 172.217.1.1 │
                                                        └─────────────┘

Each router:
1. Receives packet
2. Checks destination IP in routing table  
3. Determines next hop
4. Updates Ethernet headers (new SRC/DST MAC)
5. Forwards packet
6. IP headers NOW show: (SRC: 203.0.113.50, DST: 172.217.164.109)

NOTE: After NAT, internet sees your PUBLIC IP (203.0.113.50), 
NOT your private IP (192.168.10.25)
```

### Step 6: Gmail's Network (Destination Subnet)

```
Gmail's Subnet: 172.217.164.0/24
┌─────────────────────────────────────────────────────────────┐
│                                                             │
│  [Gateway]                                   [Gmail Server] │
│  172.217.164.1                              172.217.164.109 │
│  MAC: 99:88:77:66:55:44                     MAC: FF:EE:...  │
│       │                                           ▲         │
│       │                                           │         │
│              ┌─────────────────────────────────────┴─────┐   │
│              │            SWITCH                         │   │
│              │                                           │   │
│              │ Forwards to Gmail server based on IP      │   │
│              │ 172.217.164.109                           │   │
│              └───────────────────────────────────────────┘   │
│                                                             │
│  Final packet delivery:                                     │
│  ┌─────────────────────────────────────────────────────────┐│
│  │ SRC IP: 203.0.113.50 (Your PUBLIC IP - what Gmail sees)││
│  │ DST IP: 172.217.164.109 (Gmail server)                 ││
│  │ Application: SMTP email data                            ││
│  └─────────────────────────────────────────────────────────┘│
└─────────────────────────────────────────────────────────────┘
```

---

## 4. Key Differences Summary

```
┌──────────────────────────────────────────────────────────────────┐
│                        COMPONENT ROLES                           │
├──────────────────────────────────────────────────────────────────┤
│                                                                  │
│ SUBNET (Network Boundary)                                        │
│ ┌──────────────────────────────────────────────────────────────┐ │
│ │ • Defines which devices can talk directly                    │ │
│ │ • Same subnet = direct communication via switch             │ │
│ │ • Different subnet = must go through gateway                │ │
│ │ • Example: 192.168.10.0/24 contains .1 through .254        │ │
│ └──────────────────────────────────────────────────────────────┘ │
│                                                                  │
│ SWITCH (Layer 2 - MAC addresses)                                 │
│ ┌──────────────────────────────────────────────────────────────┐ │
│ │ • Connects devices within same subnet                        │ │
│ │ • Uses MAC addresses to forward frames                       │ │
│ │ • Maintains MAC address table                                │ │
│ │ • No IP routing decisions                                    │ │
│ └──────────────────────────────────────────────────────────────┘ │
│                                                                  │
│ GATEWAY/ROUTER (Layer 3 - IP addresses)                          │
│ ┌──────────────────────────────────────────────────────────────┐ │
│ │ • Connects different subnets/networks                        │ │
│ │ • Makes routing decisions based on IP addresses             │ │
│ │ • Maintains routing table                                    │ │
│ │ • Changes Ethernet headers at each hop                      │ │
│ │ • Keeps IP headers mostly unchanged                          │ │
│ └──────────────────────────────────────────────────────────────┘ │
└──────────────────────────────────────────────────────────────────┘
```

---

## 5. Practical Troubleshooting Guide

```
Network Problem Diagnosis:
┌─────────────────────────────────────────────────────────────┐
│ Can't reach a device? Check these in order:                │
│                                                             │
│ 1. SAME SUBNET (e.g., printer not working)                 │
│    • Check if both devices have same network portion       │
│    • Verify switch connectivity                            │
│    • Check ARP table: arp -a                               │
│                                                             │
│ 2. DIFFERENT SUBNET (e.g., can't reach internet)           │
│    • Check default gateway: ipconfig /all                  │
│    • Test gateway: ping 192.168.10.1                       │
│    • Check routing: tracert 8.8.8.8                        │
│                                                             │
│ 3. SUBNET MASK ISSUES                                       │
│    • Wrong mask = wrong network boundary                    │
│    • 255.255.255.0 vs 255.255.0.0 creates different       │
│      network sizes                                          │
└─────────────────────────────────────────────────────────────┘
```

---

## 7. NAT (Network Address Translation) - The Missing Piece!

```
┌──────────────────────────────────────────────────────────────────┐
│                    WHY YOU HAVE A PUBLIC IP                     │
├──────────────────────────────────────────────────────────────────┤
│                                                                  │
│ PRIVATE IP RANGES (RFC 1918) - NOT routable on internet:        │
│ • 192.168.0.0   to 192.168.255.255 (192.168.x.x)              │
│ • 172.16.0.0    to 172.31.255.255  (172.16-31.x.x)            │
│ • 10.0.0.0      to 10.255.255.255  (10.x.x.x)                 │
│                                                                  │
│ PUBLIC IP RANGES - Routable on internet:                        │
│ • Everything else (like 203.0.113.50, 8.8.8.8, etc.)          │
└──────────────────────────────────────────────────────────────────┘

YOUR HOME/OFFICE NETWORK WITH NAT:
┌────────────────────────────────────────────────────────────────────┐
│                     PRIVATE NETWORK                               │
│  ┌─────────────────────────────────────────────────────────────┐   │
│  │ Device A: 192.168.10.20                                     │   │
│  │ Your PC:  192.168.10.25  ←── This is PRIVATE              │   │
│  │ Phone:    192.168.10.30                                     │   │
│  │ Printer:  192.168.10.40                                     │   │
│  └─────────────────────────┬───────────────────────────────────┘   │
│                            │                                       │
│  ┌─────────────────────────┴───────────────────────────────────┐   │
│  │                    ROUTER/NAT DEVICE                       │   │
│  │                                                             │   │
│  │ LAN Side (Private):     │  WAN Side (Public):               │   │
│  │ IP: 192.168.10.1        │  IP: 203.0.113.50 ←── PUBLIC    │   │
│  │                         │                                   │   │
│  │               NAT TRANSLATION TABLE                         │   │
│  │ ┌─────────────────────────────────────────────────────────┐ │   │
│  │ │ Internal IP:Port    │ External Port │ External IP      │ │   │
│  │ │ 192.168.10.25:54321 │     :12345    │ 203.0.113.50    │ │   │
│  │ │ 192.168.10.20:35678 │     :23456    │ 203.0.113.50    │ │   │
│  │ │ 192.168.10.30:44444 │     :34567    │ 203.0.113.50    │ │   │
│  │ └─────────────────────────────────────────────────────────┘ │   │
│  └─────────────────────────────────────────────────────────────┘   │
│                            │                                       │
└────────────────────────────┼───────────────────────────────────────┘
                             │
        ┌────────────────────┴─────────────────────┐
        │              INTERNET                    │
        │    (Only sees 203.0.113.50)             │
        └────────────────────┬─────────────────────┘
                             │
┌────────────────────────────┼───────────────────────────────────────┐
│                            │                     GMAIL NETWORK     │
│  ┌─────────────────────────┴───────────────────────────────────┐   │
│  │ Gmail Server: 172.217.164.109                               │   │
│  │                                                             │   │
│  │ When Gmail replies, it sends to: 203.0.113.50:12345        │   │
│  │ (It has NO IDEA about your private IP 192.168.10.25)       │   │
│  └─────────────────────────────────────────────────────────────┘   │
└───────────────────────────────────────────────────────────────────┘
```

## 8. What Different Entities See

```
┌──────────────────────────────────────────────────────────────────┐
│                      WHO SEES WHAT IP?                          │
├──────────────────────────────────────────────────────────────────┤
│                                                                  │
│ 1. YOUR COMPUTER (192.168.10.25) sees:                          │
│    • Its own IP: 192.168.10.25                                  │
│    • Gateway: 192.168.10.1                                      │
│    • Other local devices: 192.168.10.x                          │
│                                                                  │
│ 2. YOUR ROUTER sees:                                             │
│    • LAN side: All private IPs (192.168.10.x)                   │
│    • WAN side: Its public IP (203.0.113.50)                     │
│    • Maintains translation between them                          │
│                                                                  │
│ 3. GMAIL SERVER (172.217.164.109) sees:                         │
│    • Source: 203.0.113.50 (your public IP)                      │
│    • Destination: 172.217.164.109 (itself)                      │
│    • NO KNOWLEDGE of 192.168.10.25                              │
│                                                                  │
│ 4. INTERNET ROUTERS see:                                         │
│    • Only public IPs: 203.0.113.50 → 172.217.164.109           │
│    • Private IPs are "invisible" to internet                    │
│                                                                  │
│ 5. WEBSITES (like whatismyip.com) see:                           │
│    • Your public IP: 203.0.113.50                               │
│    • This is what shows up in server logs                       │
└──────────────────────────────────────────────────────────────────┘
```

---

## 9. Real-World IP Examples

```
Check your IPs right now:

PRIVATE IP (Internal):
┌─────────────────────────────────────────┐
│ Windows: ipconfig                       │
│ Mac/Linux: ifconfig or ip addr          │
│                                         │
│ Result might be:                        │
│ • 192.168.1.100 (home router)          │
│ • 192.168.0.50  (home router)          │
│ • 10.0.0.25     (office network)       │
│ • 172.16.5.100  (office network)       │
└─────────────────────────────────────────┘

PUBLIC IP (External):
┌─────────────────────────────────────────┐
│ Visit: whatismyip.com                   │
│                                         │
│ Result might be:                        │
│ • 203.0.113.50   (your ISP assigned)   │
│ • 198.51.100.25  (your ISP assigned)   │
│ • 8.8.4.4        (if using VPN)        │
└─────────────────────────────────────────┘

Multiple devices behind same router = Same public IP!
• Your laptop: Private 192.168.1.100 → Public 203.0.113.50
• Your phone:  Private 192.168.1.101 → Public 203.0.113.50  
• Your tablet: Private 192.168.1.102 → Public 203.0.113.50
```