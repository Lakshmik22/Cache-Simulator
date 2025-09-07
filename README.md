# Cache Simulator  

This project implements a cache simulator in C++ to evaluate memory performance under different configurations. It supports multiple replacement policies, configurable cache parameters, and detailed performance metrics. The simulator was developed as part of **COP 5611: Advanced Computer Architecture** at the University of Central Florida.  

---

## Features  
- Configurable cache size, block size, and associativity  
- Replacement policies: FIFO, LRU, Optimal, Clock (1-bit and 2-bit), LIFO  
- Tracks hit and miss rates with a breakdown of compulsory, capacity, and conflict misses  
- Reports eviction counts and provides an optional debug mode for detailed cache state analysis  

---

## Technologies  
- **C++** – Core implementation  
- **Makefile** – Automated compilation  
- **Trace files** – Input memory access patterns for simulation  

---

## Project Structure  
cache-simulator/
├── cache_sim.cpp # Main simulation driver
├── cache.h / cache.cpp # Cache logic and helper classes
├── trace-MINIFE.txt # Sample trace file
├── Makefile # Build instructions
└── README.md # Documentation


---

## Output  
- Cache hit and miss rates  
- Miss classifications: compulsory, capacity, conflict  
- Eviction statistics  
- Optional detailed trace output  

---

## Academic Context  
Developed as a final project for **COP 5611: Advanced Computer Architecture** to demonstrate applied knowledge of cache design, memory hierarchy, and performance analysis.  
