# OS Resource Management Simulator

A modular C++ project for studying Operating Systems concepts through CPU scheduling and memory allocation simulation.

## Features

### CPU Scheduling
- FCFS
- SJF (Non-Preemptive)
- SRTF (Preemptive)
- Priority Scheduling (Non-Preemptive)
- Round Robin
- Gantt chart
- Completion, Turnaround, Waiting and Response Time
- CPU utilization
- Algorithm comparison

### Memory Management
- First Fit
- Best Fit
- Worst Fit
- Next Fit
- Memory utilization
- Allocation comparison

## Project Structure

```text
OS-Resource-Management-Simulator/
├── include/
│   ├── Process.h
│   ├── MemoryBlock.h
│   ├── Scheduler.h
│   └── MemoryManager.h
├── src/
│   ├── main.cpp
│   ├── Scheduler.cpp
│   └── MemoryManager.cpp
├── data/
├── .gitignore
└── README.md
```

## Compile

### Linux / macOS / MinGW

```bash
g++ -std=c++17 src/main.cpp src/Scheduler.cpp src/MemoryManager.cpp -Iinclude -o os_simulator
```

### Run

```bash
./os_simulator
```

### Windows

```bash
g++ -std=c++17 src/main.cpp src/Scheduler.cpp src/MemoryManager.cpp -Iinclude -o os_simulator.exe
os_simulator.exe
```

## Concepts Demonstrated

- CPU scheduling
- Preemptive vs non-preemptive scheduling
- Process queues
- Scheduling metrics
- Memory allocation strategies
- Internal fragmentation concepts
- C++ STL vectors and queues
- Modular C++ design

## Suggested Extensions

- Add CSV input/output
- Add external-fragmentation analysis
- Add paging simulation
- Add a graphical visualization layer
- Add automated unit tests

## Note

This is a learning project. Understand and test the implementation before using it in academic, interview, or professional contexts.
