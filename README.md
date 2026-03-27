# RISC-V Emulator 

## Overview

This project is a simple **RISC-V (RV32I) emulator** written in C/C++ with a basic device model and an experimental **UART cosimulation using Verilator**.

The emulator includes:

* A minimal CPU implementation (RV32I)
* A memory system with a bus and RAM
* A memory-mapped UART device
* A reference (software) UART model
* An RTL UART model (OpenCores 16550) simulated via Verilator

The main goal is to explore how **software emulation and hardware simulation can be combined** in a single environment.

---

## Features

* RV32I instruction execution
* Memory-mapped I/O via a simple bus
* Device abstraction (RAM, UART)
* Transaction-based cosimulation (REF vs RTL)
* Integration with Verilator-generated hardware model

---

## Architecture

```
CPU
 │
 ▼
BUS
 ├── RAM
 └── UART
        ├── Reference model (C)
        └── RTL model (Verilator)
```

The bus routes memory accesses to devices.
UART accesses are intercepted and forwarded to both:

* a **reference model** (for correctness)
* an **RTL model** (for hardware-level behavior)

---


## Build Requirements

* GCC / G++
* Verilator
* RISC-V GCC toolchain (`riscv64-unknown-elf-gcc`)

---

## Build & Run

Typical flow:

```bash
make 
```

---

## Status

This is a **work-in-progress** project.

Currently:

* CPU and memory system are functional
* Reference UART works correctly
* RTL UART is integrated but timing/simulation is incomplete

---

## Goals

* Learn how emulators are structured
* Understand memory-mapped devices
* Explore hardware/software cosimulation

---

## Disclaimer

This is a **learning / hobby project ("for fun")**, not a production-quality emulator.

Expect:

* Simplifications
* Incomplete features
* Experimental code
