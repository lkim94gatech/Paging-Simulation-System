# Paging Simulation System

This repository contains the implementation of a **Paging Simulation System** designed in C. The project simulates memory management techniques used in operating systems, including page replacement algorithms, swapping, and statistics tracking. It provides a detailed view of how paging systems work in practice and the impact of various algorithms on performance metrics such as Average Access Time (AAT).

---

## Files Included

### Core Modules
- **pagesim.c**: Main entry point for the paging simulator.
- **pagesim.h**: Header file defining core functions and structures for the simulation.
- **paging.c**: Implements core paging functionalities such as memory access, process initialization, and cleanup.
- **paging.h**: Header file defining paging-related data structures and functions.
- **stats.c**: Computes performance statistics, including Average Access Time (AAT).
- **stats.h**: Defines the `stats_t` structure for tracking memory statistics.

### Supporting Modules
- **swap.c**: Handles swapping operations, including writing pages to disk and reading them back.
- **swap.h**: Header file for swap-related functions.
- **swapops.c**: Implements operations for managing swap space.
- **swapops.h**: Header file for swap operation definitions.
- **page_fault.c**: Handles page faults and manages the allocation of new pages.
- **page_replacement.c**: Implements page replacement algorithms.
- **page_splitting.h**: Defines macros and functions for splitting virtual addresses.
- **util.c**: Contains utility functions for memory manipulation.
- **util.h**: Header file for utility function declarations.

---

## Overview

This project provides a detailed simulation of an operating system's paging system, including:
- **Page Table Management**: Each process is assigned a page table that maps virtual addresses to physical memory.
- **Page Replacement**: Simulates various algorithms for replacing pages when physical memory is full.
- **Swapping**: Handles reading and writing pages to/from disk when required.
- **Statistics Tracking**: Tracks important metrics like reads, writes, page faults, and Average Access Time (AAT).

---

## Features

1. **Memory Management**:
   - Initializes the frame table and allocates page tables for processes.
   - Implements context switching and process cleanup.

2. **Page Fault Handling**:
   - Simulates page faults and allocates memory for missing pages.
   - Integrates swapping operations for evicted pages.

3. **Page Replacement**:
   - Supports algorithms for deciding which pages to evict during page faults.

4. **Statistics**:
   - Tracks reads, writes, page faults, and writebacks.
   - Computes Average Access Time (AAT) based on memory and disk access times.

---

## Setup Instructions

### Prerequisites

- **C Compiler**: GCC or Clang.
- **Make**: For compiling the project.

### Steps

1. **Clone the Repository**:
   ```bash
   git clone <repository_url>
   cd paging-simulation
   ```
2. **Compile the Code**:
   ```bash
   make
   ```
3. **Run the Simulator**:
   ```bash
   ./pagesim <trace_file>
   ```
4. **Clean Up**:
   ```bash
   make clean
   ```

## Modules Breakdown

### **Core Modules**
- **pagesim.c**:
  - **Purpose**: Implements the main simulator logic.
  - **Key Functionality**: Initializes the system, handles input traces, and coordinates paging operations.

- **paging.c**:
  - **Purpose**: Core memory management logic.
  - **Key Functions**:
    - `system_init()`: Initializes the frame table.
    - `proc_init(pcb_t *proc)`: Allocates page tables for new processes.
    - `context_switch(pcb_t *proc)`: Handles process context switching.
    - `mem_access(vaddr_t address, char rw, uint8_t data)`: Translates virtual addresses to physical addresses.
    - `proc_cleanup(pcb_t *proc)`: Frees memory resources when a process exits.

- **stats.c**:
  - **Purpose**: Tracks and computes performance statistics.
  - **Key Functionality**: Computes Average Access Time (AAT).

### **Supporting Modules**
- **swap.c**:
  - **Purpose**: Handles swapping pages to/from disk.
  - **Key Functions**:
    - `swap_page_out()`: Writes a page to disk.
    - `swap_page_in()`: Reads a page from disk.

- **page_fault.c**:
  - **Purpose**: Handles page faults by allocating memory for missing pages.

- **page_replacement.c**:
  - **Purpose**: Implements page replacement strategies.

- **util.c**:
  - **Purpose**: Provides utility functions for memory manipulation.

---

## Testing

### **Simulation**
- Validate the behavior of the paging system under different workloads.
- Analyze the impact of page replacement algorithms on performance.

### **Statistics**
- Compute Average Access Time (AAT) and verify accuracy.
- Track metrics like reads, writes, and page faults.

### **Page Replacement**
- Test various replacement strategies to ensure correctness.

### **Swapping**
- Verify that pages are correctly swapped to/from disk.
