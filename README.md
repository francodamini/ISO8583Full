# PLAN

## ✅ PHASE 1: SYSTEM ARCHITECTURE

### Components:
1. **Message Builder/Parser**
   - Encode/decode messages to/from raw ISO 8583 format.
2. **Field Definitions**
   - Bitmap, MTI, and Data Elements (DEs 2 to 128).
3. **Transport Layer**
   - TCP/IP client & server (with framed messages).
4. **Transaction Dispatcher**
   - Handles MTI-based logic (authorization, reversal, etc.).
5. **Encryption Support (Phase 2)**
   - Simple 3DES encryption or simulated MAC.
6. **Simulator Tools**
   - Simulated POS (client), and simulated Host (server).


## ✅ PHASE 2: MESSAGE STRUCTURE (ISO 8583:1987)

### Typical Transaction Flow:
1. **Client (POS)** sends **0100** (Authorization Request)
2. **Server (Host)** replies with **0110** (Authorization Response)

### Basic Field Set for MTI 0100:
| Field No | Name                     | Type       | Length    |
|----------|--------------------------|------------|-----------|
| 0        | MTI                      | N          | 4         |
| 2        | PAN                      | LLVAR N    | up to 19  |
| 3        | Processing Code          | N          | 6         |
| 4        | Amount, Transaction      | N          | 12        |
| 7        | Transmission Date & Time | N          | 10        |
| 11       | STAN (Trace No)          | N          | 6         |
| 12       | Time, Local              | N          | 6         |
| 13       | Date, Local              | N          | 4         |
| 37       | Retrieval Reference No   | AN         | 12        |
| 39       | Response Code            | AN         | 2         |
| 41       | Terminal ID              | AN         | 8         |
| 49       | Currency Code            | N          | 3         |

## ✅ PHASE 3: CODE MODULES

1. **Field Definitions (C struct + constants)**
2. **Bitmap Handling Functions**
3. **Message Encode/Decode**
4. **TCP Server/Client Skeleton**
5. **Dispatcher (Handles MTI logic)**
6. **Simple POS Simulator**
