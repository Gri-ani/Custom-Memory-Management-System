// (MeMS) Memory Management System


// Before starting the assignment clone the skeleton code for the assignment from the below github repo:

// https://github.com/Rahul-Agrawal-09/MeMS-Skeleton-code.git 


// Problem Statement:

// Implement a custom memory management system (MeMS) using the C programming language. MeMS should utilize the system calls mmap and munmap for memory allocation and deallocation, respectively. The system must satisfy the following constraints and requirements outlined below:


// Constraints and Requirements:

// MeMS can solely use the system calls mmap and munmap for memory management. The use of any other memory management library functions such as malloc, calloc, free, and realloc are STRICTLY PROHIBITED.
// MeMS should request memory from the OS using mmap in multiples of the system's PAGE_SIZE, which can be determined using the command getconf PAGE_SIZE. For most Linux distributions, the PAGE_SIZE is 4096 bytes (4 KB); however, it might differ for other systems.
// MeMS should deallocate memory only through munmap and deallocation should only occur in multiples of PAGE_SIZE.
// As the value of PAGE_SIZE can differ from system to system hence use the macro “PAGE_SIZE” provided in the template wherever you need the value of PAGE_SIZE in your code so that this size can be modified if required for evaluation purposes.
// The user program must use the functions provided by MeMS for memory allocation and deallocation. It is not allowed to use any other memory management library functions, including malloc, calloc, free, realloc, mmap, and munmap.
// Although MeMS requests memory from the OS in multiples of PAGE_SIZE, it only allocates that much amount of memory to the user program as requested by the user program. MeMS maintains a free list data structure to keep track of the heap memory which MeMS has requested from the OS. This free list keeps track of two items:
// memory allocated to each user program. We will call this memory as PROCESS in the free list (details below).
// Memory which has not been allocated to any user program. We will call this memory as a HOLE in the free list (details below).

// Free List Structure:

// Free List is represented as a doubly linked list. Let's call this doubly linked list as the main chain of the free list. The main features of the main chain are:

// Whenever MeMS requests memory from the OS (using mmap), it adds a new node to the main chain.
// Each node of the main chain points to another doubly linked list which we call as sub-chain. This sub-chain can contain multiple nodes. Each node corresponds to a segment of memory within the range of the memory defined by its main chain node. Some of these nodes (segments) in the sub-chain are mapped to the user program. We call such nodes (segments) as PROCESS nodes. Rest of the nodes in the sub-chain are not mapped to the user program and are called as HOLES or HOLE nodes.
// Whenever the user program requests for memory from MeMS, MeMS first tries to find a sufficiently large segment in any sub-chain of any node in the main chain. If a sufficiently large segment is found, MeMS uses it to allocate memory to the user program and updates the segment’s type from HOLE to PROCESS. Else, MeMS requests the OS to allocate more memory on the heap (using mmap) and add a new node corresponding to it in the main chain.


// The structure of free list looks like below:





// The main features of the chain (sub-chain) are:

// Each chain is broken into segments.
// Each segment represents a part of memory and represents whether that segment is of type PROCESS i.e. is mapped to the user process or is of type HOLE i.e. not allocated/mapped to the user program.
// The segments of type HOLE can be reallocated to any new requests by the user process. In this scenario, if some space remains after allocation then the remaining part becomes a new segment of type HOLE in that sub-chain. Graphaphically it looks something like below:


// In the above picture, the Node1 of sub-chain-4 is reused by the user process but only 600 bytes out of 1000 bytes are used. Hence a HOLE of 400 bytes is created and the node of 600 bytes is marked as PROCESS and the MeMS virtual address corresponding to 600 bytes node is returned to the user process for further use.


// NOTE: You must handle the corner cases and make sure that your system should avoids memory fragmentation within the free list.


// MeMS Virtual Address and MeMS Physical Address:


// Let us call the address (memory location) returned by mmap as the MeMS physical address. In reality, the address returned by mmap is actually a virtual address in the virtual address space of the process in which MeMS is running. But for the sake of this assignment, since we are simulating memory management by the OS, we will call the virtual address returned by mmap as MeMS physical address.


// Just like a call to mmap returns a virtual address in the virtual address space of the calling process, a call to mems_malloc will return a MeMS virtual address in the MeMS virtual address space of the calling process. For the sake of this assignment, MeMS manages heap memory for only one process at a time.


// Just like OS maintains a mapping from virtual address space to physical address space, MeMS maintains a mapping from MeMS virtual address space to MeMS physical address space. So, for every MeMS physical address (which is provided by mmap), we need to assign a MeMS virtual address. As you may understand, this MeMS virtual address has no meaning outside the MeMS system.


// Any time the user process wants to write/store anything to the heap, it has to make use of the MeMS virtual address. But we cannot directly write using MeMS virtual address as the OS does not have any understanding of MeMS virtual address space. Therefore, we first need to get the MeMS physical address for that MeMS virtual address. Then, the user process needs to use this MeMS physical address to write on the heap.


// For example in the below figure

// MeMS virtual address 0 corresponds to MeMS physical address 1000
// MeMS virtual address 500 corresponds to MeMS physical address 1500
// MeMS virtual address 1024 corresponds to MeMS physical address 5024




// We can get the MeMS physical address (i.e. the actual address returned by mmap) corresponding to a MeMS virtual address by using the function mems_get function (see below for more details).



// Function Implementations:

// Implement the following functions within MeMS:

// void mems_init(): Initializes all the required parameters for the MeMS system. The main parameters to be initialized are
// the head of the free list i.e. the pointer that points to the head of the free list
// the starting MeMS virtual address from which the heap in our MeMS virtual address space will start.
// any other global variable that you want for the MeMS implementation can be initialized here.
// Input Parameter: Nothing

// Returns: Nothing

// void mems_finish(): This function will be called at the end of the MeMS system and its main job is to unmap the allocated memory using the munmap system call.
// Input Parameter: Nothing

// Returns: Nothing

// void* mems_malloc(size_t size): Allocates memory of the specified size by reusing a segment from the free list if a sufficiently large segment is available. Else, uses the mmap system call to allocate more memory on the heap and updates the free list accordingly.
// Parameter: The size of the memory the user program wants

// Returns: MeMS Virtual address (that is created by MeMS)

// void mems_free(void* ptr): Frees the memory pointed by ptr by marking the corresponding sub-chain node in the free list as HOLE. Once a sub-chain node is marked as HOLE, it becomes available for future allocations.
// Parameter: MeMS Virtual address (that is created by MeMS)

// Returns: nothing

// void mems_print_stats(): Prints the total number of mapped pages (using mmap) and the unused memory in bytes (the total size of holes in the free list). It also prints details about each node in the main chain and each segment (PROCESS or HOLE) in the sub-chain.
// Parameter: Nothing

// Returns: Nothing but should print the necessary information on STDOUT

// void *mems_get(void*v_ptr): Returns the MeMS physical address mapped to ptr ( ptr is MeMS virtual address).
// Parameter: MeMS Virtual address (that is created by MeMS)

// Returns: MeMS physical address mapped to the passed ptr (MeMS virtual address).


// Submission Guidelines:

// Please download the skeleton template from the below github repository:


// https://github.com/Rahul-Agrawal-09/MeMS-Skeleton-code


// Students are required to submit the C code containing the implementation of all the functions there in the skeleton template provided, along with a detailed explanation of their approach. Additionally, students should provide a demonstration of their implementation with test cases that validate the functionality of MeMS under different scenarios.

// (Note: Do not change any function’s name or signature provided in the skeleton template. You are free to include more files if you want.)


// Some more guidelines [Added on October 28th, 2023]


// You will be graded upon:

// Documentation
// Viva/Demo
// Error handling
// Covering all the edge cases (like joining 2 adjacent hole nodes to make a new hole node)
// Correct mems_virtual_address corresponding to mems_physical_address.
// We will run various test cases (variants of the example file which we have shared) and each of them will have individual weightage.
// Successful compilation and correct output format.
// Use the data structures which have been described in the documentation.
// Any other implementation specific detail necessary for the assignment.

// Note:

// 1. Use the PAGE_SIZE macro in the code so that we can test your code with different page size values.

// 2. Use Github for version control as TA’s might check commit log


// Suggestion:
// 1. Maintain good program structure and proper naming convention so that evaluation can be done without any hassle

//Example output
// Allocated virtual addresses [mems_malloc]
// Virtual address: 1000
// Virtual address: 2000
// virtual address: 3000
// virtual address: 4000
// Virtual address: 5096
// virtual address: 6096
// virtual address: 7096
// Virtual address: 8096
// Virtual address: 9192
// virtual address: 10192
// ------ Assigning value to Virtual address [mems_get]
// virtual address: 1000 Physical Address: 140535789383680
// Value written: 200
// Printing Stats [mems_print_stats]
// MeMS SYSTEM STATS
// MAIN[1000:5095]-> P[1000:1999] <-> P[2000:2999] <-> P[3000:3999] <-> P[4000:4999] <-> H[5000:5095] <-> NULL
// MAIN[5096:9191]-> P[5096:6095] <-> P[6096:7095] <-> P[7096:8095] <-> P[8096:9095] <-> H[9096:9191] <-> NULL
// MAIN[9192:13287]-> P[9192:10191] <-> P[10192:11191] <-> H[11192:13287] <-> NULL
// Pages used:
// 3
// Space unsued: 2288
// Main Chain Length:
// 3
// Sub-chain Length array: [5, 5, 3, ]
// Freeing up the memory [mems_free]
// MeMS SYSTEM STATS -----
// MAIN[1000:5095]-> P[1000:1999] <-> P[2000:2999] <-> P[3000:3999] <-> H[4000:5095] <-> NULL
// MAIN [5096:9191]-> P[5096:6095] <-> P[6096:7095] <-> P[7096:8095] <-> P[8096:9095] <-> H[9096:9191] <-> NULL
// MAIN [9192:13287]-> P[9192:10191] <-> P[10192:11191] <-> H[11192:13287] <-> NULL
// Pages used:
// 3
// Space unsued: 3288
// Main Chain Length:
// 3
// Sub-chain Length array: [4, 5, 3, ]
// -----
// MeMS SYSTEM STATS
// MAIN[1000:5095]-> P[1000:1999] <-> P[2000:2999] <-> P[3000:3999] <-> P[4000:4999] <-> H[5000:5095] <-> NULL
// MAIN [5096:9191]-> P[5096:6095] <-> P[6096:7095] <-> P[7096:8095] <-> P[8096:9095] <-> H[9096:9191] <-> NULL
// MAIN [9192:13287]-> P[9192:10191] <-> P[10192:11191] <-> H[11192:13287] <-> NULL
// Pages used:
// 3
// Space unsued: 2288
// Main Chain Length:
// 3
// Sub-chain Length array: [5, 5, 3, ]
// Unmapping all memory [mems_finish]
// include other header files as needed
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/mman.h>
#include<string.h>

//dictionary
#define MAX_ENTRIES 100

// Define a struct to represent a key-value pair with void* pointers
struct KeyValuePair {
    void* key;
    void* value;
};

// Define the dictionary as an array of key-value pairs
struct KeyValuePair dictionary[MAX_ENTRIES];
int dictionarySize = 0; // Tracks the current size of the dictionary

// Function to add a key-value pair to the dictionary
int addToDictionary(void* key, void* value) {
    if (dictionarySize < MAX_ENTRIES) {
        dictionary[dictionarySize].key = key;
        dictionary[dictionarySize].value = value;
        dictionarySize++;
        // printf("Added key: %lu, value: %lu to the dictionary.\n", (unsigned long )key, (unsigned long) value);
        return 1; // Success
    } else {
        // printf("Dictionary is full. Cannot add more items.\n");
        return 0; // Failure
    }
}

// Function to retrieve a value from the dictionary based on a key
void* getFromDictionary(void* key) {
    for (int i = 0; i < dictionarySize; i++) {
        if (dictionary[i].key == key) {
            return dictionary[i].value;
        }
    }
    // Key not found, return NULL or handle the error as needed.
    // find the nearest key
    void* nearestKey = NULL;
    int nearestKeyIndex = -1;
    for (int i = 0; i < dictionarySize; i++) {
        if (dictionary[i].key < key) {
            if (nearestKey == NULL || dictionary[i].key > nearestKey) {
                nearestKey = dictionary[i].key;
                nearestKeyIndex = i;
            }
        }
    }
    if (nearestKeyIndex != -1) {
        return dictionary[nearestKeyIndex].value + (key - nearestKey);

    }

    return NULL;
}



#define PAGE_SIZE 4096


typedef struct sub_node
{
    void *start;
    void *start_va;
    size_t size;
    void *end;
    void *end_va;
    int type; // 0 for HOLE, 1 for PROCESS
    struct sub_node *next;
    struct sub_node *prev;
} sub_node;

typedef struct main_node
{
    void *start;
    void *start_va;
    void *current_ptr;
    void *end;
    void *end_va;
    size_t size;
    struct main_node *sub_chain;
    struct main_node *next;
    struct main_node *prev;
    sub_node *sub_head;
} main_node;
void *starting_virtual_address = 1000;
main_node *head = NULL;
main_node *tail = NULL;
static void *main_current_ptr = NULL;
void mems_init(){
main_node *head = NULL;
main_node *tail = NULL;
void *starting_virtual_address = 1000;
}

void* mems_malloc(size_t size){
    //check if there is a hole of size greater than or equal to size
    //if yes then allocate the memory from that hole
    if(head != NULL){
        // printf("head is not null\n");
    main_node *temp = head;
    while(temp != NULL){
        // printf("temp size: %lu\n", (unsigned long)temp->size);
        // printf("temp start va: %lu\n", (unsigned long)temp->start_va);
        // printf("temp end va: %lu\n", (unsigned long)(temp->end));
        main_current_ptr = temp->current_ptr;
        // printf("main current ptr: %lu\n", (unsigned long)main_current_ptr);
        // printf("size + main current ptr: %lu\n", (unsigned long)(size + main_current_ptr));
        // printf("temp end va: %lu\n", (unsigned long)temp->end_va);
        sub_node *temp_sub = temp->sub_head;
        while(temp_sub != NULL){
            // printf("temp sub start va: %lu\n", (unsigned long)temp_sub->start_va);
            // printf("temp sub end va: %lu\n", (unsigned long)temp_sub->end_va);
            // printf("temp sub type: %d\n", temp_sub->type);
            // printf("temp sub size: %lu\n", (unsigned long)temp_sub->size);
        
    if(temp_sub->start_va + size -1 <= temp_sub->end_va && temp_sub->type == 0){
        // printf("hole is found\n");
        //allocate the memory from the hole
        sub_node *process = (sub_node *)(temp_sub->start);
        process->start = temp_sub->start;
        // printf("process start: %lu\n", (unsigned long)process->start);
        process->start_va = temp_sub->start_va;
        // printf("process start va: %lu\n", (unsigned long)process->start_va);
        addToDictionary(process->start_va, process->start);
        process->end = process->start + size -1;
        // printf("process end: %lu\n", (unsigned long)process->end);
        process->end_va = process->start_va + size -1;
        // printf("process end va: %lu\n", (unsigned long)process->end_va);
        addToDictionary(process->end_va, process->end);
        process->type = 1;
        main_current_ptr = process->end;
        //update the sub chain
        sub_node *hole = (sub_node *)(process->end + 1);
        hole->start = process->end + 1;
        // printf("hole start: %lu\n", (unsigned long)hole->start);
        hole->start_va = process->end_va + 1;
        // printf("hole start va: %lu\n", (unsigned long)hole->start_va);
        // printf("temp sub hole size: %lu\n", (unsigned long)temp_sub->size);
        addToDictionary(hole->start_va, hole->start);
        hole->end = process->end + temp_sub->size - size;
        // printf("hole end: %lu\n", (unsigned long)hole->end);
        hole->end_va = process->end_va + temp_sub->size - size;
        addToDictionary(hole->end_va, hole->end);
        // printf("hole end va: %lu\n", (unsigned long)hole->end_va);
        hole->type = 0;
        hole->size = temp_sub->size - size;
        process->size = size;
        //update the main chain
        temp_sub = process;
        process->next = hole;
        process->prev = NULL;
        hole->next = NULL;
        hole->prev = process;
        temp->current_ptr = process->end_va;
        // printf("Hole size: %lu\n", (unsigned long)hole->size);
        //update the main current pointer
        //update the starting virtual address
        // starting_virtual_address = starting_virtual_address + size;
        return process->start_va;}
            temp_sub = temp_sub->next;
    }
    temp = temp->next;
    }
    }
    size_t main_node_size = ((size + PAGE_SIZE - 1) / PAGE_SIZE) * PAGE_SIZE;
    //if no hole is able to allocate the memory size then we need to create a new main node()
    main_node *new_main_node = (main_node *)mmap(NULL, main_node_size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    // printf("Virtual address: %lu\n", (unsigned long)new_main_node);
    new_main_node->size = main_node_size;
    // printf("Virtual address: size %lu\n", (unsigned long)new_main_node->size);

    new_main_node->start = new_main_node;
    // printf("Virtual address: %lu\n", (unsigned long)new_main_node->start);
    new_main_node->end = new_main_node->start + main_node_size -1;
    // printf("Virtual address: %lu\n", (unsigned long)new_main_node->end);
    new_main_node->start_va = starting_virtual_address;
    // printf("Virtual address start: %lu\n", (unsigned long)new_main_node->start_va);
    new_main_node->end_va = new_main_node->start_va + main_node_size -1 ;
    // printf("Virtual address end: %lu\n", (unsigned long)new_main_node->end_va);
    // sub_node *new_sub_node = (sub_node *)(new_main_node->start);
    // new_sub_node->start = new_main_node->start;
    // printf("Virtual address: %lu\n", (unsigned long)new_sub_node->start);
    // new_sub_node->end = new_main_node->end;
    // printf("Virtual address: %lu\n", (unsigned long)new_sub_node->end);
    // new_sub_node->type = 0;
    // dividing this hole into a process and new hole
    sub_node *process = (sub_node *)(new_main_node->start);
    process->start = new_main_node->start;
    process->start_va = new_main_node->start_va;
    addToDictionary(process->start_va, process->start);
    // printf("Virtual address: process start %lu\n", (unsigned long)process->start_va);
    process->end = new_main_node->start + size -1;
    // new_main_node->current_ptr = process->end;
    process->end_va = new_main_node->start_va + size -1;
    addToDictionary(process->end_va, process->end);
    process->size = size;
    new_main_node->current_ptr = process->end_va;
    // printf("Virtual address: process end %lu\n", (unsigned long)process->end_va);
    process->type = 1;
    sub_node *hole = (sub_node *)(new_main_node->start + size);
    hole->start = new_main_node->start + size;
    hole->start_va = new_main_node->start_va + size;
    addToDictionary(hole->start_va, hole->start);    
    // printf("Virtual address: hole start %lu\n", (unsigned long)hole->start_va);
    // printf("new main node end: %lu\n", (unsigned long)new_main_node->end_va);
    // printf("new main node ending address: %lu\n", (unsigned long)new_main_node->end);
    hole->end = new_main_node->start + main_node_size -1;
    hole->end_va = new_main_node->start_va + main_node_size -1;

    addToDictionary(hole->end_va, hole->end);
    hole->size = main_node_size - size;
    // printf("Virtual address: hole end %lu\n", (unsigned long)hole->end_va);
    // printf("Virtual address:  ssstart %lu\n", (unsigned long)new_main_node->start_va);
    // printf("Virtual address:  end %lu\n", (unsigned long)new_main_node->end_va);
    hole->type = 0;
    //connet process to mainnode
    new_main_node->sub_head = process;
    process->next = hole;
    process->prev = NULL;
    hole->next = NULL;
    hole->prev = process;
    //connect main node to main chain
    if(head == NULL){
        // printf("head is null\n");
        head = new_main_node;
        tail = new_main_node;
        new_main_node->next = NULL;
        new_main_node->prev = NULL;
        
    }
    else{
        tail->next = new_main_node;
        new_main_node->prev = tail;
        new_main_node->next = NULL;
        tail = new_main_node;
    }
    //update the starting virtual address
    starting_virtual_address = hole->end_va + 1;
return process->start_va;
    





}
void *mems_get(void*v_ptr){
    // printf("v_ptr: %lu\n", (unsigned long)v_ptr);
    void *p_ptr = getFromDictionary(v_ptr);
    // printf("p_ptr: %lu\n", (unsigned long)p_ptr);
    return p_ptr;
}
void mems_print_stats(){
    //For every Subchain in the free list print the data as follows
// MAIN[starting_mems_vitual_address:ending_mems_vitual_address] -> <HOLE or PROCESS>[starting_mems_vitual_address:ending_mems_vitual_address] <-> ..... <-> NULL
// After printing the whole freelist print the following stats
// Page used: <Total Pages used by the user>
// Space unused: <Total space mapped but not used by user>
// Main Chain Length: <Length of the main chain>
// Sub-chain Length array: <Array denoting the length of the subchains>
// For example:
// MeMS SYSTEM STATS
// MAIN[1000:5095]-> P[1000:1999] <-> P[2000:2999] <-> P[3000:3999] <-> P[4000:4999] <-> H[5000:5095] <-> NULL
// MAIN[5096:9191]-> P[5096:6095] <-> P[6096:7095] <-> P[7096:8095] <-> P[8096:9095] <-> H[9096:9191] <-> NULL
// MAIN[9192:13287]-> P[9192:10191] <-> P[10192:11191] <-> H[11192:13287] <-> NULL
// Pages used: 3
// Space unused: 2288
// Main Chain Length: 3
// Sub-chain Length array: [5, 5, 3, ]
main_node *temp2 = head;
int count = 0;
int subcount = 0;
while(temp2 != NULL){
    count++;
    sub_node *temp_sub3 = temp2->sub_head;
    while(temp_sub3 != NULL){
        subcount++;
        if(temp_sub3->type == 0){
            // printf("H[%lu:%lu] <-> ", (unsigned long)temp_sub3->start_va, (unsigned long)temp_sub3->end_va);
        }
        else{
            // printf("P[%lu:%lu] <-> ", (unsigned long)temp_sub3->start_va, (unsigned long)temp_sub3->end_va);
        }
        temp_sub3 = temp_sub3->next;
    }
    temp2 = temp2->next;
}
int sub_chain_length_array[count];
int i=0;

    main_node *temp = head;
    unsigned long a=(unsigned long)temp->start_va;
    unsigned long b;
    unsigned long x=0;
    unsigned long y=0;
    printf("MeMS SYSTEM STATS\n");
    while(temp != NULL){
        sub_node *temp_sub2 = temp->sub_head;
        while(temp_sub2-> next != NULL){

            temp_sub2 = temp_sub2->next;
        }
        printf("MAIN[%lu:%lu]-> ", (unsigned long)temp->start_va, (unsigned long)temp_sub2->end_va);
        y++;
        sub_node *temp_sub = temp->sub_head;
        while(temp_sub != NULL){
            i++;
            if(temp_sub->type == 0){
                printf("H[%lu:%lu] <-> ", (unsigned long)temp_sub->start_va, (unsigned long)temp_sub->end_va);
                x+=(unsigned long)temp_sub->end_va-(unsigned long)temp_sub->start_va+1;
            }
            else{
                printf("P[%lu:%lu] <-> ", (unsigned long)temp_sub->start_va, (unsigned long)temp_sub->end_va);
            }
            sub_chain_length_array[y-1]=i;
            b=(unsigned long)temp_sub->end_va;
            temp_sub = temp_sub->next;
            
        }i=0;

        printf("NULL\n");
        temp = temp->next;
    }

    int c=(b-a+1)/PAGE_SIZE;
    printf("Pages used: %lu\n", c);
    printf("Space unused: %lu\n", x);
    printf("Main Chain Length: %lu\n", y);
    printf("Sub-chain Length array: [");
    for(int j=0;j<count;j++){
        printf("%d, ", sub_chain_length_array[j]);
    }
    printf("]\n");

}
void mems_free(void* ptr){
    main_node *temp = head;
    while(temp != NULL){
        // printf("temp start va: %lu\n", (unsigned long)temp->start_va);
        sub_node *temp_sub = temp->sub_head;
        while(temp_sub != NULL){
            // printf("temp sub start va: %lu\n", (unsigned long)temp_sub->start_va);
            // printf("ptr: %lu\n", (unsigned long)ptr );
            if(temp_sub->start_va == ptr){
                // printf("ptr is found\n");
                // printf("temp_sub->start_va: %lu\n", (unsigned long)temp_sub->start_va);
                // printf("temp_sub->end_va: %lu\n", (unsigned long)temp_sub->end_va);
                temp_sub->type = 0;
            }
            temp_sub = temp_sub->next;
        }
        temp = temp->next;
    }
    //check if there are two adjacent holes
    main_node *temp2 = head;
    while(temp2 != NULL){
        sub_node *temp_sub2 = temp2->sub_head;
        while(temp_sub2 != NULL){
            if(temp_sub2->type == 0 && temp_sub2->next != NULL && temp_sub2->next->type == 0){
                // printf("two adjacent holes are found\n");
                // printf("temp_sub2->size: %lu\n", (unsigned long)temp_sub2->size);
                // printf("temp_sub2->next->size: %lu\n", (unsigned long)temp_sub2->next->size);
                temp_sub2->size = temp_sub2->size + temp_sub2->next->size;
                // printf("temp_sub2->size: %lu\n", (unsigned long)temp_sub2->size);    
                // printf("temp_sub2->next->size: %lu\n", (unsigned long)temp_sub2->next->size);
                temp_sub2->end = temp_sub2->end + temp_sub2->next->size;
                // printf("temp_sub2->end: %lu\n", (unsigned long)temp_sub2->end);
                temp_sub2->end_va = temp_sub2->end_va + temp_sub2->next->size;
                // printf("temp_sub2->end_va: %lu\n", (unsigned long)temp_sub2->end_va);
                if(temp_sub2->next->next != NULL){
                    temp_sub2->next->next->prev = temp_sub2;
                    temp_sub2->next = temp_sub2->next->next;
                }
                else{
                    temp_sub2->next = NULL;

                }
                // temp_sub2->next = temp_sub2->next->next;
                // printf("temp_sub2->next: %lu\n", (unsigned long)temp_sub2->next);
                // printf("temp_sub2->next->next: %lu\n", (unsigned long)temp_sub2->next->next);
                // temp_sub2->next->prev = temp_sub2;
                // printf("temp_sub2->next->prev: %lu\n", (unsigned long)temp_sub2->next->prev);
                // printf("temp_sub2->next->next: %lu\n", (unsigned long)temp_sub2->next->next
                // printf("temp_sub2->next->next->prev: %lu\n", (unsigned long)temp_sub2->next->next->prev);
                // printf("temp_sub2->next->next->next: %lu\n", (unsigned long)temp_sub2->next->next->next);
                // printf("temp_sub2->next->next->next->prev: %lu\n", (unsigned long)temp_sub2->next->next->next->prev);

            }
            temp_sub2 = temp_sub2->next;
        }
        temp2 = temp2->next;
}
}
void mems_finish(){
    main_node *temp = head;
    while(temp != NULL){
        sub_node *temp_sub = temp->sub_head;
        while(temp_sub != NULL){
            sub_node *temp_sub2 = temp_sub;
            temp_sub = temp_sub->next;
            munmap(temp_sub2->start, temp_sub2->size);
            // printf("temp_sub2->start: %lu\n", (unsigned long)temp_sub2->start);

        }
        main_node *temp2 = temp;
        while(temp != NULL){
        sub_node *temp_sub2 = temp->sub_head;
        while(temp_sub2-> next != NULL){

            temp_sub2 = temp_sub2->next;
        }
        // printf("MAIN[%lu:%lu]-> ", (unsigned long)temp->start_va, (unsigned long)temp_sub2->end_va);
        size_t size = temp_sub2->end_va - temp->start_va + 1;
        temp = temp->next;
        munmap(temp2->start, size);
    }
}
}

    


int main(int argc, char const *argv[])
{
    // initialise the MeMS system 
    mems_init();
    int* ptr[10];
    

    /*
    This allocates 10 arrays of 250 integers each
    */
    printf("\n------- Allocated virtual addresses [mems_malloc] -------\n");
    for(int i=0;i<10;i++){
        ptr[i] = (int*)mems_malloc(sizeof(int)*250);
        printf("Virtual address : %lu\n", (unsigned long)ptr[i]);
    }


    /*
    In this section we are tring to write value to 1st index of array[0] (here it is 0 based indexing).
    We get get value of both the 0th index and 1st index of array[0] by using function mems_get.
    Then we write value to 1st index using 1st index pointer and try to access it via 0th index pointer.

    This section is show that even if we have allocated an array using mems_malloc but we can 
    retrive MeMS physical address of any of the element from that array using mems_get. 
    */
    printf("\n------ Assigning value to Virtual address [mems_get] -----\n");
    // // how to write to the virtual address of the MeMS (this is given to show that the system works on arrays as well)
    //printf("Virtual address1: %lu\n", (unsigned long)ptr[0]);
    // printf("Virtual address2: %lu\n", (unsigned long)ptr[0][1]);
    int* phy_ptr= (int*) mems_get(&ptr[0][1]); // get the address of index 1
    phy_ptr[0]=200; // put value at index 1
    int* phy_ptr2= (int*) mems_get(&ptr[0][0]); // get the address of index 0
    printf("Virtual address: %lu\tPhysical Address: %lu\n",(unsigned long)ptr[0],(unsigned long)phy_ptr2);
    printf("Value written: %d\n", phy_ptr2[1]); // print the address of index 1 

    /*
    This shows the stats of the MeMS system.  
    */
    printf("\n--------- Printing Stats [mems_print_stats] --------\n");
    mems_print_stats();

    // /*
    // This section shows the effect of freeing up space on free list and also the effect of 
    // reallocating the space that will be fullfilled by the free list.
    // */
    printf("\n--------- Freeing up the memory [mems_free] --------\n");
    mems_free(ptr[3]);
    mems_print_stats();
    ptr[3] = (int*)mems_malloc(sizeof(int)*250);
    mems_print_stats();

    printf("\n--------- Unmapping all memory [mems_finish] --------\n\n");
    mems_finish();
    return 0;
}


// //1. mems_finish
// Purpose: Clean up and free all allocated memory structures when the memory management system is no longer needed.

// Steps:
// Iterate Through Main Nodes:

// Start from the head of the main node linked list.
// For each main node, iterate through its associated subnodes.
// Free Subnodes:

// For each subnode, free the allocated memory for the subnode itself.
// If the subnode has a dynamic memory area (e.g., the allocated space), free that memory as well.
// Free Main Nodes:

// After all subnodes are freed, free the main node itself.
// Set Head to NULL:

// Set the head pointer of the main node linked list to NULL to indicate that the memory management system is finished.
// Optionally Print a Message:

// Print a message indicating that the memory management has been successfully cleaned up.




// 2. mems_malloc
// Purpose: Allocate a specified amount of memory and return a pointer to it.

// Steps:
// Calculate Required Size:

// Determine the total size needed, which might include the requested size and any overhead for managing the memory block.
// Search for Free Space:

// Iterate through the linked list of main nodes and their subnodes to find a suitable hole (free block) that can accommodate the requested size.
// Allocate Memory:

// If a suitable hole is found:
// Update the subnode to mark it as allocated (change its type).
// If necessary, split the hole into a new smaller hole and the allocated block.
// Return Pointer:

// Return a pointer to the start of the allocated block (adjusted for any overhead if necessary).
// If no suitable hole is found, you may need to request additional pages from the operating system (e.g., using mmap or malloc).




// 3. mems_free
// Purpose: Deallocate previously allocated memory and mark it as free.

// Steps:
// Locate the Block:

// Start from the head of the main node linked list.
// For each main node, iterate through its subnodes to find the one corresponding to the pointer ptr passed to mems_free.
// Mark as Free:

// If the block is found, set its type to 0 (indicating it is now a hole).
// Merge Adjacent Holes:

// Check if the current block has adjacent holes (previous or next subnode) and merge them into a single larger hole if they are both marked as free.
// Update Links:

// Adjust the linked list pointers to maintain the integrity of the list after merging.




// 4. mems_print_stats
// Purpose: Print statistics about memory usage and the current state of allocated and free blocks.

// Steps:
// Initialize Counters:

// Set counters for the number of main nodes, subnodes, total pages used, and total space unused.
// Iterate Through Main Nodes:

// For each main node, iterate through its subnodes.
// Print the virtual addresses and types (P for process and H for hole) of each subnode.
// Calculate Statistics:

// For each hole, add to the total unused space.
// Count the number of pages used based on the size of each main node.
// Print Final Statistics:

// Print the total pages used, total space unused, main chain length, and an array denoting the length of the subchains.



// 5. mems_get
// Purpose: Retrieve information about a specific memory block.

// Steps:
// Locate the Block:

// Start from the head of the main node linked list.
// For each main node, iterate through its subnodes to find the block corresponding to the pointer ptr.
// Return Block Information:

// If found, return relevant information about the block, such as its start and end addresses, size, and whether it is allocated or free.
// Handle Not Found Case:

// If the block is not found, return an error or NULL to indicate that the pointer is invalid.
