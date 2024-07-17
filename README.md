# MeMS: Memory Management System.

## Introduction
The MeMS (Memory Management System) is a custom memory management system implemented in C. It provides a way to allocate, deallocate, and manage memory in a controlled virtual address space.

## Implementation
### Data Structures
- **NODE:** Represents a block of memory in the main chain, containing size, virtual address range, and a linked list of sub-chains.
- **SUB_NODE:** Represents a segment within a NODE, can be either a "PROCESS" or a "HOLE", containing size and address range information.

### Functions
- `mems_init()`: Initializes the MeMS system.
- `mems_malloc(size_t size)`: Allocates memory of the specified size.
- `mems_free(void *v_ptr)`: Frees the memory pointed to by a MeMS virtual address.
- `mems_get(void *v_ptr)`: Returns the MeMS physical address mapped to a MeMS virtual address.
- `mems_print_stats()`: Prints statistics about the MeMS system.
- `mems_finish()`: Cleans up the MeMS system.

## Example Usage
The provided example demonstrates the usage of MeMS functions: allocating memory, assigning values, printing statistics, and releasing memory.

## Building and Running
### Prerequisites
- GCC (GNU Compiler Collection)

### Instructions
1. **Clone the repository:**
   ```bash
   git clone https://github.com/Parasv17/MeMS_Custom.git
   cd mems
2. **Build And Run**
     ```bash
     make

For detailed documentation, refer to the [Documentation](https://docs.google.com/document/d/1Gs9kC3187lLrinvK1SueTc8dHCJ0QP43eRlrCRlXiCY/edit#heading=h.gixz2pyjqud).




