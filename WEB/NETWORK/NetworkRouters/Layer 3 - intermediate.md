# 🌍 OSI Layer 3 (Network Layer) - Complete Deep Dive

## 📋 Table of Contents
1. [Layer 3 Fundamentals](#fundamentals)
2. [IPv4 Packet Structure (Detailed)](#ipv4-structure)
3. [Byte-Level Packet Analysis](#byte-level)
4. [Practical Example: Web Request Journey](#web-journey)
5. [Routing Tables & Forwarding](#routing-tables)
6. [Advanced Topics](#advanced-topics)
7. [Troubleshooting Tools](#troubleshooting)

---

## 🎯 Layer 3 Fundamentals {#fundamentals}

**Primary Purpose**: End-to-end logical addressing and routing across multiple networks.

**Key Responsibilities**:
- Logical addressing (IP addresses)
- Path determination and routing
- Packet fragmentation and reassembly
- Traffic forwarding between networks
- Loop prevention (TTL/Hop Limit)

**Layer 3 vs Layer 2**:
```
Layer 2 (Data Link)          Layer 3 (Network)
├─ Physical addressing       ├─ Logical addressing
├─ Local network scope       ├─ Internet-wide scope
├─ MAC addresses (48-bit)    ├─ IP addresses (32/128-bit)
├─ Switch-based forwarding   ├─ Router-based forwarding
└─ Frame-based               └─ Packet-based
```

---

## 📦 IPv4 Packet Structure (Detailed) {#ipv4-structure}

### Complete IPv4 Header Format (20-60 bytes)

```
 0                   1                   2                   3
 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|Version|  IHL  |Type of Service|          Total Length         |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|         Identification        |Flags|      Fragment Offset    |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|  Time to Live |    Protocol   |         Header Checksum       |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|                       Source Address                          |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|                    Destination Address                        |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|                    Options                    |    Padding    |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
```

### Field-by-Field Breakdown

| Field | Size | Purpose | Example Values |
|-------|------|---------|----------------|
| **Version** | 4 bits | IP version | `0100` (IPv4), `0110` (IPv6) |
| **IHL** | 4 bits | Header length in 32-bit words | `0101` = 20 bytes, `1111` = 60 bytes |
| **Type of Service** | 8 bits | QoS/DSCP markings | `00000000` (best effort) |
| **Total Length** | 16 bits | Entire packet size | `0000010100000000` = 1280 bytes |
| **Identification** | 16 bits | Fragment group ID | `0x1234` (unique per original packet) |
| **Flags** | 3 bits | `[Reserved][DF][MF]` | `010` (Don't Fragment set) |
| **Fragment Offset** | 13 bits | Position in original packet | `0000000000000` (not fragmented) |
| **TTL** | 8 bits | Hops remaining | `64` (Linux default), `128` (Windows) |
| **Protocol** | 8 bits | Next layer protocol | `6` (TCP), `17` (UDP), `1` (ICMP) |
| **Header Checksum** | 16 bits | Header integrity check | Recalculated at each hop |
| **Source IP** | 32 bits | Sender's logical address | `192.168.1.10` = `C0A8010A` |
| **Destination IP** | 32 bits | Receiver's logical address | `8.8.8.8` = `08080808` |
| **Options** | 0-40 bytes | Rarely used extensions | Route recording, timestamps |

---

## 🔍 Byte-Level Packet Analysis {#byte-level}

Let's trace an actual HTTP request packet as it moves through 3 routers:

### Initial Packet: PC → Router1

**Scenario**: PC (192.168.1.10) requests www.google.com (172.217.14.142)

#### Ethernet Frame (Layer 2):
```
Destination MAC: 00:1B:21:3C:4D:5E  (Router1's LAN interface)
Source MAC:      AA:BB:CC:DD:EE:FF  (PC's NIC)
EtherType:       08:00              (IPv4)
```

#### IPv4 Packet (Layer 3) - HEX DUMP:
```
Offset  00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F  ASCII
------  ------------------------------------------------  -----
0000    45 00 00 3C 12 34 40 00 40 06 B1 E6 C0 A8 01 0A  E..<.4@.@.......
0010    AC D9 0E 8E 04 D2 00 50 00 00 00 00 00 00 00 00  .......P........
0020    A0 02 72 10 B8 5A 00 00 02 04 05 B4 04 02 08 0A  ..r..Z..........
```

#### Field Breakdown:
```
45        → Version(4) + IHL(5) = IPv4, 20-byte header
00        → Type of Service (Best effort)
00 3C     → Total Length = 60 bytes
12 34     → Identification = 0x1234
40 00     → Flags(010) + Fragment Offset(0000000000000) - DF set
40        → TTL = 64
06        → Protocol = TCP
B1 E6     → Header Checksum = 0xB1E6
C0 A8 01 0A → Source IP = 192.168.1.10
AC D9 0E 8E → Destination IP = 172.217.14.142
```

### Packet Transformation: Router1 → Router2

**Router1 Processing**:
1. Decrement TTL: 64 → 63
2. Recalculate checksum
3. Update Layer 2 headers
4. Forward based on routing table

#### New Ethernet Frame (Layer 2):
```
Destination MAC: 11:22:33:44:55:66  (Router2's interface)
Source MAC:      00:1B:21:3C:4D:5F  (Router1's WAN interface)
EtherType:       08:00              (IPv4)
```

#### Modified IPv4 Packet - HEX DUMP:
```
Offset  00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F  ASCII
------  ------------------------------------------------  -----
0000    45 00 00 3C 12 34 40 00 3F 06 B2 E6 C0 A8 01 0A  E..<.4@.?.......
0010    AC D9 0E 8E 04 D2 00 50 00 00 00 00 00 00 00 00  .......P........
0020    A0 02 72 10 B8 5A 00 00 02 04 05 B4 04 02 08 0A  ..r..Z..........
```

#### Changes Made:
```
3F        → TTL decremented: 64 → 63 (0x40 → 0x3F)
B2 E6     → New checksum (was 0xB1E6, now 0xB2E6)
```

### Packet Transformation: Router2 → Router3

#### Modified IPv4 Packet - HEX DUMP:
```
Offset  00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F  ASCII
------  ------------------------------------------------  -----
0000    45 00 00 3C 12 34 40 00 3E 06 B3 E6 C0 A8 01 0A  E..<.4@.>.......
0010    AC D9 0E 8E 04 D2 00 50 00 00 00 00 00 00 00 00  .......P........
0020    A0 02 72 10 B8 5A 00 00 02 04 05 B4 04 02 08 0A  ..r..Z..........
```

#### Changes Made:
```
3E        → TTL decremented: 63 → 62 (0x3F → 0x3E)
B3 E6     → New checksum (was 0xB2E6, now 0xB3E6)
```

### Packet Transformation: Router3 → Destination

#### Final IPv4 Packet - HEX DUMP:
```
Offset  00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F  ASCII
------  ------------------------------------------------  -----
0000    45 00 00 3C 12 34 40 00 3D 06 B4 E6 C0 A8 01 0A  E..<.4@.=.......
0010    AC D9 0E 8E 04 D2 00 50 00 00 00 00 00 00 00 00  .......P........
0020    A0 02 72 10 B8 5A 00 00 02 04 05 B4 04 02 08 0A  ..r..Z..........
```

#### Final Changes:
```
3D        → TTL decremented: 62 → 61 (0x3E → 0x3D)
B4 E6     → Final checksum (was 0xB3E6, now 0xB4E6)
```

### Checksum Calculation Process

The IPv4 header checksum is calculated as follows:
```python
# Pseudo-code for checksum calculation
def calculate_checksum(header):
    # 1. Set checksum field to 0
    # 2. Sum all 16-bit words in header
    # 3. Add carry bits to sum
    # 4. Take one's complement
    checksum = 0
    for i in range(0, len(header), 2):
        word = (header[i] << 8) + header[i+1]
        checksum += word
        checksum = (checksum & 0xFFFF) + (checksum >> 16)
    return ~checksum & 0xFFFF
```

---

## 🗺️ Practical Example: Web Request Journey {#web-journey}

### Network Topology
```
[PC Client]      [Router1]      [Router2]      [Router3]      [Google Server]
192.168.1.10 ──── 10.0.1.1 ──── 203.0.113.1 ── 8.8.4.1 ──── 172.217.14.142
     │               │               │               │               │
   LAN A         WAN Link 1      WAN Link 2      WAN Link 3        Internet
```

### Step-by-Step Packet Journey

#### Step 1: DNS Resolution
```
PC sends DNS query for www.google.com to 8.8.8.8:

Layer 2: [PC MAC → Router1 MAC]
Layer 3: [192.168.1.10 → 8.8.8.8] TTL=64
Layer 4: [UDP src:53281 dst:53]
Layer 7: [DNS Query: www.google.com A record]
```

#### Step 2: HTTP Request Initiation
```
Browser creates HTTP request to 172.217.14.142:

Application Data: "GET / HTTP/1.1\r\nHost: www.google.com\r\n..."
TCP Segment: SYN packet, seq=0, ack=0, window=65535
IP Packet: src=192.168.1.10, dst=172.217.14.142, TTL=64
Ethernet Frame: src=PC_MAC, dst=ROUTER1_MAC
```

#### Step 3: Router1 Processing
```
Incoming Interface: eth0 (LAN)
Routing Decision: dest 172.217.14.142 → next hop 10.0.1.254
Outgoing Interface: eth1 (WAN)

Actions:
1. Decrement TTL: 64 → 63
2. Recalculate IP checksum
3. Update Ethernet header: src=ROUTER1_WAN_MAC, dst=ROUTER2_MAC
4. Forward packet
```

#### Step 4: Internet Transit
The packet traverses multiple ISP routers, with TTL decreasing at each hop:
```
Router2: TTL 63 → 62
Router3: TTL 62 → 61
Router4: TTL 61 → 60
...continuing until reaching Google's network
```

### Complete Packet Flow Diagram

```
┌─────────────┐    ┌─────────────┐    ┌─────────────┐    ┌─────────────┐
│     PC      │    │   Router1   │    │   Router2   │    │   Router3   │
│192.168.1.10 │    │10.0.1.1/    │    │203.0.113.1/ │    │  8.8.4.1/   │
│             │    │192.168.1.1  │    │10.0.1.254   │    │203.0.113.254│
└──────┬──────┘    └──────┬──────┘    └──────┬──────┘    └──────┬──────┘
       │                  │                  │                  │
       │ L2: PC→R1        │ L2: R1→R2        │ L2: R2→R3        │ L2: R3→Google
       │ L3: TTL=64       │ L3: TTL=63       │ L3: TTL=62       │ L3: TTL=61
       │ Checksum: B1E6   │ Checksum: B2E6   │ Checksum: B3E6   │ Checksum: B4E6
       └──────────────────┼──────────────────┼──────────────────┼──────────────→
                         │                  │                  │
                    ┌────▼────┐        ┌────▼────┐        ┌────▼────┐
                    │Routing  │        │Routing  │        │Routing  │
                    │Table    │        │Table    │        │Table    │
                    │Lookup   │        │Lookup   │        │Lookup   │
                    └─────────┘        └─────────┘        └─────────┘
```

---

## 🗂️ Routing Tables & Forwarding {#routing-tables}

### Sample Routing Table (Router1)
```bash
$ ip route show
default via 10.0.1.254 dev eth1 proto dhcp src 10.0.1.1 metric 100
8.8.8.0/24 via 10.0.1.254 dev eth1 proto static metric 50
10.0.1.0/24 dev eth1 proto kernel scope link src 10.0.1.1 metric 100
172.16.0.0/12 via 10.0.1.254 dev eth1 proto static metric 200
192.168.1.0/24 dev eth0 proto kernel scope link src 192.168.1.1 metric 100
```

### Routing Table Breakdown
| Destination | Next Hop | Interface | Metric | Meaning |
|-------------|----------|-----------|--------|---------|
| `default` | 10.0.1.254 | eth1 | 100 | Default gateway for unknown destinations |
| `8.8.8.0/24` | 10.0.1.254 | eth1 | 50 | Google DNS subnet (static route) |
| `10.0.1.0/24` | - | eth1 | 100 | Directly connected WAN network |
| `172.16.0.0/12` | 10.0.1.254 | eth1 | 200 | Private network range |
| `192.168.1.0/24` | - | eth0 | 100 | Directly connected LAN network |

### Forwarding Decision Process
```python
def forward_packet(dest_ip, routing_table):
    best_match = None
    longest_prefix = -1
    
    for route in routing_table:
        if dest_ip in route.network:
            if route.prefix_length > longest_prefix:
                longest_prefix = route.prefix_length
                best_match = route
    
    if best_match:
        return best_match.next_hop, best_match.interface
    else:
        return None, None  # Drop packet
```

### Example Forwarding Decisions
```
Destination IP: 172.217.14.142 (Google)
├─ Check: 192.168.1.0/24 → No match
├─ Check: 10.0.1.0/24 → No match  
├─ Check: 8.8.8.0/24 → No match
├─ Check: 172.16.0.0/12 → No match (172.217 not in 172.16-172.31 range)
└─ Use: default via 10.0.1.254 → FORWARD to eth1

Destination IP: 192.168.1.50 (LAN device)
├─ Check: 192.168.1.0/24 → MATCH! (longest prefix: /24)
└─ Direct delivery via eth0
```

---

## 🔬 Advanced Topics {#advanced-topics}

### IPv4 Fragmentation

When a packet exceeds the MTU (Maximum Transmission Unit) of a link, it must be fragmented.

#### Original Packet (1500 bytes data)
```
Total Length: 1520 bytes (20 byte header + 1500 byte payload)
Identification: 0x4321
Flags: [0][0][0] (Reserved, DF=0, MF=0)
Fragment Offset: 0
```

#### After Fragmentation (MTU=1000)
```
Fragment 1:
├─ Total Length: 1000 bytes (20 header + 980 data)
├─ Identification: 0x4321 (same for all fragments)
├─ Flags: [0][0][1] (MF=1, More Fragments)
├─ Fragment Offset: 0
└─ Data: bytes 0-979

Fragment 2:
├─ Total Length: 540 bytes (20 header + 520 data)  
├─ Identification: 0x4321 (same)
├─ Flags: [0][0][0] (MF=0, Last Fragment)
├─ Fragment Offset: 122 (980/8 = 122.5, rounded down)
└─ Data: bytes 980-1499
```

### NAT (Network Address Translation)

#### NAT Translation Table
| Internal IP:Port | External IP:Port | Protocol | Timeout |
|------------------|------------------|----------|---------|
| 192.168.1.10:12345 | 203.0.113.5:54321 | TCP | 7200s |
| 192.168.1.15:8080 | 203.0.113.5:54322 | TCP | 1800s |
| 192.168.1.20:53 | 203.0.113.5:54323 | UDP | 300s |

#### Outbound NAT Process
```
Original Packet:
src: 192.168.1.10:12345 → dst: 8.8.8.8:80

After NAT:
src: 203.0.113.5:54321 → dst: 8.8.8.8:80
```

#### Return Traffic
```
Incoming Packet:
src: 8.8.8.8:80 → dst: 203.0.113.5:54321

After NAT (reverse lookup):
src: 8.8.8.8:80 → dst: 192.168.1.10:12345
```

### ICMP (Internet Control Message Protocol)

#### ICMP Message Types
| Type | Code | Description | Use Case |
|------|------|-------------|----------|
| 0 | 0 | Echo Reply | ping response |
| 3 | 0 | Network Unreachable | Routing failure |
| 3 | 1 | Host Unreachable | ARP failure |
| 3 | 3 | Port Unreachable | Service not listening |
| 8 | 0 | Echo Request | ping command |
| 11 | 0 | TTL Exceeded | traceroute |

#### ICMP Packet Structure
```
 0                   1                   2                   3
 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|     Type      |     Code      |          Checksum             |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|                             unused                            |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|      Internet Header + 64 bits of Original Data Datagram     |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
```

---

## 🛠️ Troubleshooting Tools {#troubleshooting}

### ping - Layer 3 Connectivity Test
```bash
$ ping -c 4 8.8.8.8
PING 8.8.8.8 (8.8.8.8) 56(84) bytes of data.
64 bytes from 8.8.8.8: icmp_seq=1 ttl=55 time=14.2 ms
64 bytes from 8.8.8.8: icmp_seq=2 ttl=55 time=13.8 ms
64 bytes from 8.8.8.8: icmp_seq=3 ttl=55 time=14.1 ms
64 bytes from 8.8.8.8: icmp_seq=4 ttl=55 time=13.9 ms

--- 8.8.8.8 ping statistics ---
4 packets transmitted, 4 received, 0% packet loss
time 3005ms
rtt min/avg/max/mdev = 13.814/14.000/14.213/0.176 ms
```

**Analysis**: TTL=55 indicates packet passed through 9 routers (64-55=9)

### traceroute - Path Discovery
```bash
$ traceroute 8.8.8.8
traceroute to 8.8.8.8 (8.8.8.8), 30 hops max, 60 byte packets
 1  192.168.1.1 (192.168.1.1)  1.234 ms  1.123 ms  1.089 ms
 2  10.0.1.254 (10.0.1.254)  12.456 ms  12.234 ms  12.123 ms
 3  203.0.113.1 (203.0.113.1)  23.456 ms  23.234 ms  23.123 ms
 4  72.14.204.1 (72.14.204.1)  34.567 ms  34.345 ms  34.234 ms
 5  * * *
 6  8.8.8.8 (8.8.8.8)  45.678 ms  45.456 ms  45.345 ms
```

### Wireshark Packet Capture
```
Frame 1: 74 bytes on wire (592 bits), 74 bytes captured (592 bits)
Ethernet II, Src: aa:bb:cc:dd:ee:ff, Dst: 00:1b:21:3c:4d:5e
Internet Protocol Version 4, Src: 192.168.1.10, Dst: 8.8.8.8
    0100 .... = Version: 4
    .... 0101 = Header Length: 20 bytes (5)
    Differentiated Services Field: 0x00 (DSCP: CS0, ECN: Not-ECT)
    Total Length: 60
    Identification: 0x1234 (4660)
    Flags: 0x4000, Don't fragment
    Fragment offset: 0
    Time to live: 64
    Protocol: TCP (6)
    Header checksum: 0xb1e6 [validation disabled]
    [Header checksum status: Unverified]
    Source: 192.168.1.10
    Destination: 8.8.8.8
```

### IP Configuration Commands
```bash
# View IP addresses and interfaces
ip addr show

# View routing table
ip route show

# Add static route
sudo ip route add 10.10.10.0/24 via 192.168.1.254

# View ARP table (Layer 2 to Layer 3 mapping)
ip neighbor show

# View network statistics
cat /proc/net/dev
```

### Advanced Diagnostics
```bash
# Monitor real-time packet flow
sudo tcpdump -i any -n host 8.8.8.8

# Test specific port connectivity
nmap -p 80,443 google.com

# Check MTU path discovery
tracepath google.com

# Analyze network performance
mtr google.com
```

---

## 📚 Summary

Layer 3 (Network Layer) provides the foundation for internet-wide communication through:

1. **Logical Addressing**: IP addresses enable global device identification
2. **Routing**: Determines best paths across multiple networks
3. **Packet Forwarding**: Moves packets hop-by-hop toward destinations
4. **Fragmentation**: Handles varying MTU sizes across network paths
5. **Loop Prevention**: TTL prevents infinite packet circulation

**Key Takeaways**:
- IP addresses remain constant end-to-end, MAC addresses change per hop
- TTL decrements and checksums recalculate at each router
- Routing tables use longest prefix matching for forwarding decisions
- ICMP provides essential network diagnostics and error reporting
- Understanding Layer 3 is crucial for network troubleshooting and design

This layer forms the backbone of modern internet communication, enabling billions of devices to communicate across heterogeneous networks worldwide.
