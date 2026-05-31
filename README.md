# 🎓 Online Examination System — C++ Backend

<p align="center">
  <img src="https://iili.io/C3vf6LN.png" alt="Online Examination System Thumbnail">
</p>

A fully self-contained, high-performance **Online Examination System** built in modern modular **C++** that runs a live HTTP web server, serves a premium interactive browser-based exam portal, and maintains a local file-based database — all inside a standalone executable with no external database dependencies.

---

## 📁 Modular Project Structure

The project has been fully refactored into a highly maintainable, modular structure, cleanly dividing the server logic, the SQL-like file-based database layer, and the premium HTML templates.

```
online_exam/
│
├── main.cpp                    ← Core HTTP server socket, routers & admin menu
├── database.h                  ← Thread-safe SQL-like file database manager & schemas
├── templates.h                 ← Declarations of front-end page layouts & variables
├── templates.cpp               ← Premium HTML page-builders, styles, and templates
│
├── templates/                  ← Auto-generated editable fallback HTML files
│   ├── login.html              ← Student login page
│   ├── exam.html               ← Interactive exam paper with countdown timer
│   ├── result.html             ← Interactive scoreboard and answer key review
│   └── already_submitted.html  ← Student portal submission lock state
│
├── data/                       ← Auto-generated local database files (pipe-delimited)
│   ├── students.txt            ← Student records (roll_number|name|password)
│   ├── questions.txt           ← Question paper database
│   └── results.txt             ← Submitted exam results
│
└── online_exam.exe             ← Compiled Windows executable
```

---

## ⚙️ How to Compile

To build the modular codebase, you need the **MinGW-W64 / GCC** C++ compiler installed on your system. Run the following command in your terminal or PowerShell inside the project directory:

```bash
g++ -std=c++11 main.cpp templates.cpp -o online_exam -lws2_32
```

### Compilation Flags Breakdown:

| Flag | Meaning |
|---|---|
| `-std=c++11` | Employs the C++11 standard, enabling multithreading, `std::mutex`, lambdas, auto types, etc. |
| `main.cpp templates.cpp` | Compiles the main entry point and modular page templates files together. |
| `-lws2_32` | Links the Windows Sockets (`ws2_32`) library, enabling networking and client sockets. |

---

## 🚀 How to Run

Launch the compiled executable directly from your terminal or command prompt:

```bash
.\online_exam.exe
```

Upon launching, the **Console Admin Panel** will boot up, displaying your interactive server management console:

```text
============================================================
          ONLINE EXAMINATION SYSTEM  —  Admin Console       
============================================================
  Server : STOPPED
------------------------------------------------------------
  SERVER
    1. Start Web Server
    2. Stop  Web Server
  DATA MANAGEMENT
    3. Manage Students      (Add / List / Delete)
    4. Manage Questions     (Add / List / Delete)
    5. View Exam Results
  UTILITIES
    6. Load Sample Data     (overwrites existing students & questions)
    7. Clear ALL Data       (wipes students, questions AND results)
    8. Exit
------------------------------------------------------------
  Enter Choice (1-8): 
```

### ⚡ Quick Start:
1. Press `6` → Instantly load robust predefined sample students and questions.
2. Press `1` → Boot up the high-performance local web server.
3. Open your browser and navigate to:
   * **Student Exam Portal**: `http://localhost:8080`
   * **Premium Web Admin Dashboard**: `http://localhost:8080/admin`

---

## 🌐 Mobile Portal Access (Same Wi-Fi)

The server binds to `INADDR_ANY`, meaning it listens on all active network interfaces. When starting the server, it auto-detects your local computer's IP address:

```text
==================================================
🌐 EXAM PORTAL ACCESS LINKS:
--------------------------------------------------
Local PC:               http://localhost:8080
Same Wi-Fi (Phone):     http://192.168.1.15:8080
==================================================
```

Simply type the **Same Wi-Fi (Phone)** link directly into your smartphone's browser to allow local students to take examinations wirelessly on their personal devices!

---

## 🔒 Premium Web Admin Dashboard

Access the admin web panel at: `http://localhost:8080/admin`

| Default Credentials | Value |
|---|---|
| **Username** | `abrar` |
| **Password** | `abrar@123` |

> ⚠️ You can easily customize these credentials at the top of `main.cpp` — look for `ADMIN_USERNAME` and `ADMIN_PASSWORD`.

### Admin Routes:

| Route | Description |
|---|---|
| `/admin/login` | Beautiful glassmorphic admin gatekeeper login |
| `/admin/dashboard` | Premium layout with real-time stats cards, pass rates, and submission summaries |
| `/admin/students` | Manage student roll logs, create/delete accounts |
| `/admin/questions` | Manage MCQ question bank, update answer keys and point weights |
| `/admin/results` | Real-time score monitoring and detailed student review logs |
| `/admin/logout` | Safely invalidate current admin session cookie |

Admin authentication relies on standard, secure HTTP cookie headers (`admin_session`). Admin and student sessions run concurrently and independently.

---

## 👤 Premium Student Examination Flow

1. **Gatekeeper Login**: Students log in at `http://localhost:8080` using their Roll Number and password.
2. **Interactive Exam Suite**: Renders a beautiful visual interface, incorporating:
   * A **Real-Time Timer Widget** with a smooth color-coded countdown warning.
   * An **Interactive Navigation Grid** allowing students to skip, bookmark, and jump between questions instantly.
3. **Instant Grade Evaluation**: Submitting triggers an immediate review page displaying a visual score summary, pass/fail badges (Earth tones), and a detailed breakdown of correct answers.
4. **Cheat Prevention Lock**: If a student attempts to log in again after finishing, the system redirects them to an locked "Already Submitted" gateway detailing their recorded score.

---

## 🗄️ Thread-Safe Database Design (Plain-Text File-Based)

The backend implements a lightweight **SQL-like relational database** utilizing pipe-delimited (`|`) text files for seamless persistent storage. 

### 1. `data/students.txt`
```text
101|Abraar Beigh|1234
102|John Doe|password123
103|Alice Smith|alice123
```
*Columns: `roll_number | student_name | plain_text_password`*

### 2. `data/questions.txt`
```text
1|Which of the following is the address-of operator in C++?|&|*|->|&&|A|2
```
*Columns: `question_id | question_text | option_a | option_b | option_c | option_d | correct_option | marks`*

### 3. `data/results.txt`
```text
101|Abraar Beigh|8|10|2026-05-31 15:10:24
```
*Columns: `roll_number | student_name | points_earned | total_points | submission_timestamp`*

### 🛡️ Thread Safety and Data Integrity:
* **In-Memory Cache**: Database files are loaded into `std::vector` containers on startup for microsecond retrieval times.
* **Concurrency Protection**: Because browsers trigger async multi-threaded connection requests, every read and write transaction is safely protected using a `std::mutex` via modern C++ `std::lock_guard` context guards.

---

## 💻 Custom Web Server Networking Model

This is a complete, hand-built **HTTP/1.1 TCP Server** written directly with the Windows Sockets API (**Winsock2**).

### Client Request Lifecycle:

```text
Student Browser                        Winsock Server (C++)
───────────────                        ────────────────────
 [GET /exam]      ─────────────────►  accept() incoming request socket
                                      recv() client request headers & payload
                                      parseRequest() extracts route and cookies
                                      Routing engine calls DB evaluation
                  ◄─────────────────  send() HTTP 200 OK + Premium HTML Body
 [Render Page]
```

### Hand-rolled TCP Operations:
* `WSAStartup()`: Initializes the underlying Windows socket resources.
* `setsockopt()` with `SO_REUSEADDR`: Binds and releases the server port instantly without causing annoying "port in use" lockups.
* `std::thread` per connection: detach loops immediately create individual threads to handle separate browser assets concurrently without blocking the main socket loop.
* `std::atomic<bool> isRunning`: Safely coordinates hot-reloading and graceful server shutdowns directly from the interactive admin console menu.

---

## 👥 Shoulder-Surfing Console Privacy

To secure administrator and student credentials in lab environments, the interactive console incorporates a strict privacy masking layer. Roll books, passwords, and logs show masked bullets:
* Console password list: `Abraar Beigh  -->  ••••••••`
* Sample data logs: `Student Roll 101 Loaded (Abraar Beigh / ••••)`
* Server start status: `Admin login: abrar / ••••••••`

---
## ⚠️ Educational Disclaimer & Security Assessment
This project was built primarily as an academic showcase to demonstrate low-level network socket engineering, HTTP parsing, and thread-concurrency boundaries within standard C++. Because the core goal was engineering a framework entirely from scratch without heavy dependencies, it is not hardened for production.

## 🎓 C++ Core Concepts Utilized

This project is a pristine showcase of advanced academic C++ programming standards:
* **Object-Oriented Design**: Encapsulating data stores within classes with private variables and clean public interfaces (`DBManager`).
* **Multithreading & Sockets**: Detached thread spawning to service asynchronous HTTP clients on port 8080.
* **RAII Mutex Locking**: Protecting file pointers from thread collisions using standard template library locking tools.
* **Modern C++11**: High use of range-based loops, dynamic anonymous lambda expressions, inline auto types, and stringstream parsers.

---

*Engineered with: C++11 · Winsock2 · HTML5 · CSS3 · Multi-threading · File I/O · Session Cookies · Mutex Controls*
