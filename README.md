# 🚌 Bus Tracker — Circular Queue System

> A full-stack bus management system demonstrating the **Circular Queue** data structure.  
> **Backend: Pure C Language** | **Frontend: HTML + CSS + JavaScript**

---

## 📸 Project Preview

```
╔══════════════════════════════════════════════════════════════════╗
║          HALIYAL BUS STAND — Available Buses                    ║
╠═══╦════════════════╦══════════════╦═══════════╦════════╦════════╣
║ # ║   Bus ID       ║ Destination  ║ Departure ║  Dist  ║ Seats  ║
╠═══╬════════════════╬══════════════╬═══════════╬════════╬════════╣
║ 1 ║ KA-25-F-101    ║ Dharwad      ║ 06:30 AM  ║ 98.5km ║  0/20  ║
║ 2 ║ KA-25-F-202    ║ Dandeli      ║ 07:15 AM  ║ 24.0km ║  0/20  ║
║ 3 ║ KA-25-F-303    ║ Belgaum      ║ 08:00 AM  ║148.3km ║  0/20  ║
║ 4 ║ KA-25-F-404    ║ Hubli        ║ 09:00 AM  ║120.0km ║  0/20  ║
║ 5 ║ KA-25-F-505    ║ Ramnagar     ║ 10:30 AM  ║312.0km ║  0/20  ║
╚═══╩════════════════╩══════════════╩═══════════╩════════╩════════╝
```

---

## 📋 Table of Contents

- [About the Project](#about-the-project)
- [Features](#features)
- [Data Structure — Circular Queue](#data-structure--circular-queue)
- [Project Structure](#project-structure)
- [Bus Routes](#bus-routes)
- [How It Works](#how-it-works)
- [Getting Started](#getting-started)
- [C Backend API](#c-backend-api)
- [Frontend Pages](#frontend-pages)
- [Tech Stack](#tech-stack)
- [Contributing](#contributing)
- [License](#license)

---

## 🎯 About the Project

**Haliyal Bus Tracker** is a data structures project built to demonstrate the **Circular Queue** (also called a Ring Buffer) in a real-world scenario — a bus passenger seat management system.

A passenger walks up to the Haliyal Bus Stand, picks their destination, and gets automatically assigned a seat by the Circular Queue engine. The system tracks all 20 seats per bus, supports real-time add/remove operations, and visually shows the queue's `front` and `rear` pointers in the browser dashboard.

This project was built as part of a **Data Structures & Algorithms** course with:
- All core logic written strictly in **C language** (no libraries beyond `stdio.h`, `stdlib.h`, `string.h`, `time.h`)
- A visually rich **web frontend** that mirrors the C logic 1:1 in JavaScript for browser interaction

---

## ✨ Features

### 🎟️ Passenger Booking (Page 1)
- Enter passenger name and phone number
- View all **5 buses** with route, fare, distance, and live seat availability
- Seat occupancy shown as an animated progress bar
- Click a bus → hit **Book Seat** → Circular Queue assigns your seat
- A printed **Boarding Pass** pops up with your seat number, bus ID, destination, and fare

### 📡 Bus Dashboard (Page 2)
- **Live fleet sidebar** showing all 5 buses and current seat counts
- **Circular Queue memory view** — all 20 slots rendered visually (orange = occupied)
- **Add Passenger** — `pq_enqueue()` assigns next available slot
- **Remove Passenger** — click Remove on any row to free a seat (slot becomes reusable!)
- **Dequeue Front** — `pq_dequeue()` removes the front-most passenger (simulates arrival)
- **Bus Info Panel** — shows route, departure time, distance, fare, occupied/free count
- **C Backend Simulation Log** — every queue operation logged with timestamp

---

## 🔄 Data Structure — Circular Queue

The core of this project is the **Circular Queue**, also known as a **Ring Buffer**.

### What is a Circular Queue?

A Circular Queue is a linear data structure where the last position connects back to the first, forming a circle. It solves the problem of wasted space in a regular queue — when elements are removed from the front, that space is **reused** by new elements at the rear.

```
Circular Queue — 10 slots example:

  Index:  [ 0 ] [ 1 ] [ 2 ] [ 3 ] [ 4 ] [ 5 ] [ 6 ] [ 7 ] [ 8 ] [ 9 ]
  Data:   [RAM] [SUN] [VIJ] [ — ] [ — ] [ — ] [ — ] [ANU] [MOH] [KAV]
                                                       ↑                ↑
                                                     front             rear

  After dequeue(front):
  front moves → index 8, slot 7 becomes free
  New enqueue → goes to slot 3 (wraps around!)
```

### Circular Queue vs Regular Queue

| Feature | Regular Queue | Circular Queue |
|---|---|---|
| Removed slots reused? | ❌ No | ✅ Yes |
| Memory waste | High | None |
| `rear` pointer wraps | ❌ No | ✅ Yes (`% capacity`) |
| Full detection | `rear == MAX` | `size == capacity` |

### Key Formula

```c
rear  = (rear  + 1) % capacity;   // advance rear (wraps around)
front = (front + 1) % capacity;   // advance front (wraps around)
```

### C Implementation

```c
/* Enqueue — adds passenger to next free slot */
int pq_enqueue(PassengerQueue *q, const char *name) {
    if (pq_full(q)) return -1;            // Bus full!
    for (int i = 0; i < q->capacity; i++) {
        if (!q->seats[i].is_occupied) {   // First free slot
            strncpy(q->seats[i].name, name, MAX_NAME_LEN - 1);
            q->seats[i].seat_num    = i + 1;
            q->seats[i].is_occupied = 1;
            q->size++;
            return i + 1;                 // Return seat number
        }
    }
    return -1;
}

/* Dequeue — removes passenger from front pointer */
int pq_dequeue(PassengerQueue *q, Passenger *out) {
    if (pq_empty(q)) return -1;
    for (int i = 0; i < q->capacity; i++) {
        int idx = (q->front + i) % q->capacity;   // Wrap-around!
        if (q->seats[idx].is_occupied) {
            *out = q->seats[idx];
            q->seats[idx].is_occupied = 0;
            q->front = (idx + 1) % q->capacity;   // Advance front
            q->size--;
            return 0;
        }
    }
    return -1;
}
```

---

## 📁 Project Structure

```
haliyal-bus-tracker/
│
├── bus_backend.c            ← C Backend (Pure C, all logic here)
├── haliyal_bus_tracker.html ← Frontend (HTML + CSS + JS)
└── README.md                ← This file
```

> **Note:** The JavaScript in the HTML file is a direct mirror of `bus_backend.c` — every function (`pq_enqueue`, `pq_dequeue`, `pq_remove_by_name`) has an identical JS counterpart so the browser can simulate the C logic interactively.

---

## 🗺️ Bus Routes

All buses depart from **Haliyal Bus Stand**, Uttara Kannada, Karnataka.

| # | Bus ID | Destination | Departure | Distance | Fare |
|---|--------|------------|-----------|----------|------|
| 1 | `KA-25-F-101` | Dharwad | 06:30 AM | 98.5 km | ₹120 |
| 2 | `KA-25-F-202` | Dandeli | 07:15 AM | 24.0 km | ₹35 |
| 3 | `KA-25-F-303` | Belgaum | 08:00 AM | 148.3 km | ₹180 |
| 4 | `KA-25-F-404` | Hubli | 09:00 AM | 120.0 km | ₹140 |
| 5 | `KA-25-F-505` | Ramnagar | 10:30 AM | 312.0 km | ₹380 |

Each bus has **20 seats** managed by an independent Circular Queue instance.

---

## ⚙️ How It Works

### Passenger Journey (Step by Step)

```
Step 1: Passenger arrives at Haliyal Bus Stand
        → Opens Page 1, enters their name

Step 2: Passenger selects their destination bus
        → e.g., Haliyal → Dharwad (KA-25-F-101)

Step 3: System calls pq_enqueue() on that bus's Circular Queue
        → Finds first free slot in seats[] array
        → Assigns seat number (e.g., Seat 4)
        → Records boarding timestamp

Step 4: Boarding Pass is issued
        → Shows: Name, Bus ID, Seat No., Route, Fare, Time

Step 5: Passenger boards and sits at their assigned seat
        → Dashboard updates in real time

Step 6: On arrival, conductor calls pq_dequeue()
        → Front passenger's slot is freed
        → front pointer advances (wraps with % capacity)
        → Freed slot available for next passenger
```

### Circular Queue in Action

```
Initial state (empty bus):
seats[] = [ — , — , — , — , — , — , — , — , — , — , ... , — ]
             0   1   2   3   4   5   6   7   8   9        19
front=0, size=0

After 3 passengers board:
seats[] = [RAM, SUN, VIJ, — , — , — , — , — , — , — , ... , — ]
front=0, size=3

After RAM is dequeued (front passenger):
seats[] = [ — , SUN, VIJ, — , — , ...] → front=1, size=2

New passenger PRIYA boards → takes slot 0 (circular wrap!):
seats[] = [PRY, SUN, VIJ, — , — , ...] → size=3
```

---

## 🚀 Getting Started

### Run the C Backend

**Requirements:** GCC compiler

```bash
# Clone the repository
git clone https://github.com/yourusername/haliyal-bus-tracker.git
cd haliyal-bus-tracker

# Compile the C backend
gcc bus_backend.c -lm -o bus_backend

# Run the demo
./bus_backend
```

**Expected output:**
```
[FLEET] 5 buses initialized from Haliyal.

+---+----------------+--------------+-----------+----------+--------+
| # |   Bus ID       | Destination  | Departure | Distance | Seats  |
+---+----------------+--------------+-----------+----------+--------+
| 1 | KA-25-F-101    | Dharwad      | 06:30 AM  |   98.5km |  0/20  |
...

=== Boarding Demo ===
[BUS-1] Ramesh Kumar -> Seat 1
[BUS-1] Sunita Patil -> Seat 2
[BUS-1] Vijay Naik   -> Seat 3
...
```

### Open the Frontend

No build step needed — just open the HTML file in any browser:

```bash
# Option 1: Double-click the file
open haliyal_bus_tracker.html

# Option 2: Serve with Python
python3 -m http.server 8080
# Then visit: http://localhost:8080/haliyal_bus_tracker.html
```

---

## 📚 C Backend API

All functions in `bus_backend.c`:

### Struct Definitions

```c
typedef struct {
    int  seat_num;
    char name[40];
    char boarded_at[20];
    int  is_occupied;
} Passenger;

typedef struct {
    Passenger seats[20];
    int front, rear, size, capacity;
} PassengerQueue;

typedef struct {
    char bus_id[14];
    char from[32], to[32];
    char departure[12];
    float distance_km, fare_rs;
    PassengerQueue pq;
} Bus;
```

### Queue Functions

| Function | Description | Returns |
|---|---|---|
| `pq_init(q, cap)` | Initialize queue with capacity | `void` |
| `pq_enqueue(q, name)` | Add passenger, find first free slot | Seat number or `-1` if full |
| `pq_dequeue(q, out)` | Remove front passenger | `0` success, `-1` empty |
| `pq_remove_by_name(q, name)` | Remove passenger by name | Seat number freed or `-1` |
| `pq_full(q)` | Check if queue is full | `1` or `0` |
| `pq_empty(q)` | Check if queue is empty | `1` or `0` |

### Helper Functions

| Function | Description |
|---|---|
| `init_fleet()` | Initialize all 5 buses with routes and fares |
| `display_fleet()` | Print ASCII table of all buses |
| `display_bus_passengers(idx)` | Print passenger list for one bus |

---

## 🖥️ Frontend Pages

### Page 1 — Booking

| Element | Purpose |
|---|---|
| Name input | Enter passenger name (linked to ticket) |
| 5 Bus Cards | Each shows route, departure, fare, seat bar |
| Seat progress bar | Visual indicator of how full each bus is |
| Confirm bar | Shows selection summary before booking |
| Book Seat button | Triggers `pq_enqueue()`, shows ticket |
| Boarding Pass modal | Displays seat number and trip details |

### Page 2 — Dashboard

| Element | Purpose |
|---|---|
| Fleet sidebar | Lists all 5 buses with seat counts |
| Queue memory view | 20 slot grid — orange = occupied |
| Bus info header | Shows full bus details + stats |
| Add Passenger | Calls `pq_enqueue()` on selected bus |
| Dequeue Front | Calls `pq_dequeue()` — simulates arrival |
| Remove button | Calls `pq_remove_by_name()` per row |
| C Backend log | Timestamped log of every queue operation |

---

## 🛠️ Tech Stack

| Layer | Technology |
|---|---|
| **Backend Logic** | C (C99 standard) — `bus_backend.c` |
| **Frontend** | HTML5, CSS3, Vanilla JavaScript |
| **Fonts** | Baloo 2, JetBrains Mono (Google Fonts) |
| **Build Tools** | None — single HTML file, zero dependencies |
| **Compiler** | GCC (`gcc bus_backend.c -lm -o bus_backend`) |

---

## 📐 Concepts Demonstrated

- ✅ **Circular Queue** (Ring Buffer) — core data structure
- ✅ **Enqueue / Dequeue** — O(1) and O(n) operations
- ✅ **Wrap-around pointer arithmetic** — `index % capacity`
- ✅ **Struct-based data modeling** in C
- ✅ **Dynamic seat reuse** — freed slots taken by new passengers
- ✅ **Full / Empty detection** using `size` counter
- ✅ **Frontend–backend logic parity** — JS mirrors C exactly

---

## 🤝 Contributing

Contributions, issues, and feature requests are welcome!

1. Fork the repo
2. Create your feature branch: `git checkout -b feature/add-radar-map`
3. Commit your changes: `git commit -m 'Add: radar map view for bus positions'`
4. Push to the branch: `git push origin feature/add-radar-map`
5. Open a Pull Request

### Ideas for Contributions
- Add a third page with a radar/map view of bus positions
- Export passenger list to CSV from the dashboard
- Add multi-language support (Kannada / English toggle)
- Add a ticket QR code generator
- Port the C backend to a REST API using `libmicrohttpd`

---

## 👤 Author

**Shreyas SY**  
📧 yshreyas841@gmail.com  


---



## 🙏 Acknowledgements

- Karnataka State Road Transport Corporation (KSRTC) for route inspiration
- Data Structures & Algorithms course — Circular Queue module
- Haliyal, Uttara Kannada — the origin of all routes in this project

---

<div align="center">

**⭐ Star this repo if it helped you understand Circular Queues!**

`Made with 🧡 in Karnataka`

</div>
