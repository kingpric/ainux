# AINUX

### AI-Integrated Next-generation UNIX

AINUX is a proprietary operating system project focused on exploring
**computer architecture, kernel development, and low-level systems
programming** by building a UNIX-inspired operating system from scratch.

The project aims to combine **UNIX design philosophy** with **modern
experimental system architecture**, enabling future exploration of
intelligent system behavior and advanced kernel capabilities.

------------------------------------------------------------------------

# 🚀 Vision

AINUX is designed as a **learning-driven and research-oriented operating
system** that investigates:

-   Modern kernel architecture
-   Hardware-level programming
-   UNIX-inspired system design
-   Experimental intelligent system capabilities

The objective is not only to build an operating system but to deeply
understand **how computers actually work at the lowest levels**.

------------------------------------------------------------------------

# 🧠 Philosophy

AINUX follows a few core principles:

**Understand the Machine**\
Everything runs directly on hardware with minimal abstraction.

**Build From First Principles**\
Bootloader → CPU setup → Memory → Kernel → Drivers → Userspace.

**Transparency**\
The system is designed to be educational and understandable.

**Experimentation**\
AINUX serves as a playground for advanced systems ideas.

------------------------------------------------------------------------

# 🏗 Architecture Overview

    +-----------------------+
    |      User Space       |
    |   Applications / CLI  |
    +-----------------------+
    |      System Calls     |
    +-----------------------+
    |        Kernel         |
    |  Scheduler / Memory   |
    |  Interrupts / Drivers |
    +-----------------------+
    |      Hardware         |
    |  CPU / Disk / Memory  |
    +-----------------------+

Current architecture goals:

-   UNIX-like kernel design
-   Interrupt-driven system model
-   Modular kernel subsystems
-   Clean separation between hardware and kernel services

------------------------------------------------------------------------

# ⚙ Current Progress

### Boot Process

-   BIOS boot
-   Stage‑1 bootloader (16-bit assembly)
-   Stage‑2 loader

### CPU Setup

-   Protected Mode enabled
-   A20 line enabled
-   Global Descriptor Table (GDT)

### Disk

-   ATA PIO disk driver

### Kernel

-   Kernel loaded at **0x100000**
-   Freestanding **C kernel**
-   Cross‑compiler toolchain

### Toolchain

-   `i686-elf-gcc`
-   Custom linker script

------------------------------------------------------------------------

# 🛠 Technology Stack

Languages - C - x86 Assembly

Tooling - i686-elf-gcc - GNU ld - NASM

Emulation / Debugging - QEMU - Bochs

Build System - Makefile

------------------------------------------------------------------------

# 📂 Project Structure

    ainux
    │
    ├─ boot/
    │   ├─ stage1/
    │   └─ stage2/
    │
    ├─ kernel/
    │   ├─ arch/
    │   ├─ interrupts/
    │   ├─ memory/
    │   └─ drivers/
    │
    ├─ libc/
    │
    ├─ include/
    │
    ├─ build/
    │
    └─ tools/

------------------------------------------------------------------------

# 🧩 Development Roadmap

## Phase 1 --- Core Kernel

-   IDT implementation
-   Interrupt handling
-   Timer (PIT)
-   Basic keyboard driver

## Phase 2 --- Memory Management

-   Physical memory manager
-   Paging
-   Virtual memory

## Phase 3 --- Process System

-   Scheduler
-   Task switching
-   System calls

## Phase 4 --- Filesystem

-   Disk abstraction layer
-   Filesystem driver
-   File API

## Phase 5 --- Userspace

-   ELF loader
-   Basic shell
-   User programs

------------------------------------------------------------------------

# 🧪 Running AINUX

Build the project:

    make

Run with QEMU:

    qemu-system-i386 -kernel build/kernel.bin

------------------------------------------------------------------------

# 🔒 License & Ownership

AINUX is currently a **proprietary project**.

All source code, documentation, and associated materials are the
**intellectual property of Praveen Paleria**.\
The project is **not open-source and not free software at this stage**.

Future licensing decisions may change depending on the direction of the
project.

------------------------------------------------------------------------

# 👨‍💻 Author

**Praveen Paleria**

Systems enthusiast exploring:

-   Operating system internals
-   CPU architecture
-   Kernel development
-   Low‑level systems engineering

GitHub\
https://github.com/kingpric

------------------------------------------------------------------------

# 🌌 AINUX

**AI-Integrated Next-generation UNIX**

Understanding computers by building one.
