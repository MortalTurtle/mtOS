# mtOS

A hobby operating system project developed to explore the core principles of OS development.

## 🎯 Project Goals

*   To understand the fundamental principles of operating system development.
*   To gain practical experience by implementing core OS components.
*   To build a multiprocessing system with **preemptive multitasking**.
*   Experimental development without strict adherence to classical OS structures.

## ✨ Features

*   **Architecture Support:** i386 (currently in development).
*   **Multitasking:** Preemptive multitasking, allowing the OS to control process switching.
*   **Multiprocessing:** Support for multiple concurrent processes.
*   **From-Scratch Design:** Built from the ground up for educational purposes.

## 🛠 Building and Running

Building requires a cross-compiler toolchain for the target CPU architecture.

### Prerequisites

*   A cross-compiler for the i386 architecture (e.g., `i686-elf-g++`).
*   An emulator (QEMU is recommended for easy testing).

### Build Instructions

In future should support build for different CPU architectures, currently built only for i386

1.  Clone the repository:
    ```bash
    git clone https://github.com/your-username/your-os-repo.git
    cd your-os-repo
    ```

2.  Build the project:
    ```bash
    make build
    ```
3.  Build the iso:
    ```bash
    make iso
    ```

4.  Run the OS in QEMU:
    ```bash
    make qemu
    ```
