# Cache-Simulator
Cache Simulator for Operating System Principles (COP 5611)

🧠 Cache Simulator
📄 Overview
This project implements a customizable cache simulator designed to analyze memory performance under various cache configurations. It supports multiple replacement policies and simulates memory access patterns from trace files to calculate hit/miss rates and efficiency.

🚀 Features
Configurable cache size, block size, and associativity

Multiple replacement policies:

FIFO (First-In-First-Out)

LRU (Least Recently Used)

Optimal

Clock (1-bit and 2-bit)

LIFO (Last-In-First-Out)

Tracks:

Hit rate and miss rate

Compulsory, capacity, and conflict misses

Evictions and memory access logs

🛠️ Technologies Used
C++ – Main implementation

Makefile – For compiling the simulator

Trace files – Input files simulating memory access

📁 File Structure
bash
Copy
Edit
📂cache-simulator/
│
├── cache_sim.cpp          # Main simulation code
├── cache.h / cache.cpp    # Cache logic and helper classes
├── trace-MINIFE.txt       # Sample trace file
├── Makefile               # Compilation instructions
└── README.md              # Project documentation (this file)
🧪 How to Run
Compile the project using the provided Makefile:

bash
Copy
Edit
make
Run the simulator with this format:

bash
Copy
Edit
./cache-sim <trace_file> <associativity> <cache_size> <policy_number> [-t]
Example:

bash
Copy
Edit
./cache-sim trace-MINIFE.txt 2 4096 0 -t
Where:

trace_file: Memory access file

associativity: Associativity level (1 for direct-mapped, etc.)

cache_size: Size of the cache in bytes

policy_number: Replacement policy:

0 = FIFO

1 = LRU

2 = Optimal

3 = Clock (1-bit)

4 = Clock (2-bit)

5 = LIFO

-t: Optional flag to enable trace debugging output

📊 Output
The simulator outputs the hit rate, miss rate, and cache content status (per access if -t is used).

📚 Academic Context
This project was developed for COP5611: Advanced Computer Architecture as a final project to demonstrate understanding of memory hierarchy, cache design, and performance trade-offs.

👩‍💻 Author
Lakshmi Katravulapalli
Computer Engineering | University of Central Florida
