

## MeMS (Memory Management System)

In this program, we have made a system to allocate memory. It is a C program that employs two primary data structures:

1. **MainChain**: This is for the main nodes.
2. **SubChain**: This is for the subnodes (subsegment nodes inside the main nodes).

### Functions:

1. **`mems_init`**: The `mems_init` function initializes the system. It allocates a memory page, designating it as the first main memory node. The page is divided into main segments/nodes and sub-segments. The main node's details (size, physical and virtual addresses) are set, and the sub-node is initially marked as a "HOLE" segment. This sets up the foundation for memory allocation and management in the system.

2. **`mems_malloc`**: Used for allocating memory. It identifies a hole (unused memory segment) and converts it into a process segment or it can also allocate new memory if needed. It starts by looking through the main memory segments/nodes in a linked list to find available memory segments. Within each main chunk, it checks the sub-chunks to find a segment (sub-chunk) that is marked as a "HOLE" and has enough space to accommodate the requested size of memory. If such a segment is found, it's either used entirely or split into two segments: one for the requested memory (PROCESS) and another for the remaining space (HOLE). The function returns the virtual memory address of the allocated memory. If no suitable segment is found in the existing main and sub-segments, it allocates a new main memory segment using `mmap`. This new chunk is then divided into segments for memory allocation, and one of them is marked as PROCESS. The function returns the virtual memory address of the newly allocated memory.

3. **`mems_free`**: The `mems_free` function is used to release memory in a custom memory management system. It checks if the provided memory address falls within allocated memory ranges. If it does, and it belongs to the current process, it marks it as free. If the memory is in an unallocated "HOLE" or doesn't exist in any allocated range, it prints an error message.

4. **`mems_get`**: This function is used to translate a virtual memory address (`v_ptr`) into its corresponding physical memory address. It starts by looking at the main memory nodes in the linked list, beginning with the first one (`Mainchain_h`). For each main memory node, it checks if the provided virtual memory address falls within that chunk's range. If it's within the range, it calculates an offset to find the location of the virtual address within that segment (node). It then looks at sub-nodes within the main nodes and checks if the offset falls within a sub-node's size. If the offset is within a sub-node and the sub-node belongs to the current process (PROCESS), it calculates and returns the physical memory address based on the offset. If the offset is within a sub-chunk but doesn't belong to the current process, it prints an error message, indicating that the virtual address is in a "HOLE" segment. If the virtual address is not found within any of the main or sub-chunks, or it's outside the memory range, it prints an error message and returns NULL.

5. **`mems_print_stats`**: This function provides system statistics, offering insights into memory utilization and detailed information about the individual segments, helping developers monitor MeMS's performance.

In addition to using these functions, we have also created some more functions:

- `print_sub_chain_array_length()`: This is for printing the number of sub chains in the main node.
- `print_sub_chain_array_length_f()`: This is for printing the number of sub chains in the main node for free subnodes.
- `mems_print_stats_f()`: For printing information of free sub nodes.

MeMS is a valuable tool for efficiently managing memory in applications that require dynamic memory allocation and memory tracking.

