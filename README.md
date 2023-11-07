# MeMS: Memory Management System [CSE231 OS Assignment 3]
[Documentation](https://docs.google.com/document/d/1Gs9kC3187lLrinvK1SueTc8dHCJ0QP43eRlrCRlXiCY/edit?usp=sharing)
--

## Introduction
The MeMS (Memory Management System) is a custom memory management system implemented in C. It provides a way to allocate, deallocate, and manage memory in a controlled virtual address space.

## Implementation
MeMS consists of several functions and data structures to manage memory efficiently. Here's a brief description of the key components and functions:

### Data Structures
1. **NODE:** A data structure representing a block of memory in the main chain. It contains information about the size, virtual address range, and a linked list of sub-chains.
2. **SUB_NODE:** A data structure representing a segment within a NODE. It can be either a "PROCESS" or a "HOLE" and contains information about its size and address range.

### Functions

#### mems_init()
- Initializes the MeMS system, setting the starting virtual address and preparing the first NODE in the main chain. This function is called at the beginning of the program.

#### mems_malloc(size_t size)
- Allocates memory of the specified size, reusing a segment from the free list if possible. If no suitable segment is found, it uses the mmap system call to allocate more memory. It returns a MeMS virtual address.

#### mems_free(void *v_ptr)
- Frees the memory pointed to by a MeMS virtual address and adds it to the free list. It also includes a defragmentation process to merge adjacent free segments.

#### mems_get(void *v_ptr)
- Returns the MeMS physical address mapped to a MeMS virtual address. It allows accessing the physical address of allocated memory.

#### mems_print_stats()
- Prints statistics about the MeMS system, including the number of pages used, unused memory, and details about each NODE and sub-chains in the system.

#### mems_finish()
- Cleans up the MeMS system by unmapping all allocated memory using the munmap system call. It is called at the end of the program to release all resources.

## Example Usage
The provided example demonstrates the usage of MeMS functions. It allocates memory, assigns values, prints statistics, and releases memory.

## Building
The makefile included with the assignment can be used to compile the example code.

## Conclusion
MeMS is a basic memory management system designed to efficiently manage memory in a custom virtual address space. It provides functions for memory allocation, deallocation, and statistics reporting.

