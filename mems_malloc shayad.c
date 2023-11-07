#include <stdio.h>
#include <sys/mman.h>

#define PAGE_SIZE 4096

// Structure to represent a segment in the sub-chain.
struct Segment {
    size_t size;
    int type; // 0 for HOLE, 1 for PROCESS
    void* ptr; // MeMS virtual address
    struct Segment* next;
    struct Segment* prev;
};

// Structure to represent a node in the main chain.
struct Node {
    struct Segment* sub_chain;
    struct Node* next;
    struct Node* prev;
};

struct Node* free_list_head = NULL;
void* mems_heap_start = NULL;

// Initialize MeMS
void mems_init() {
    free_list_head = NULL;
    mems_heap_start = mmap(NULL, PAGE_SIZE, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
}

// Allocate Memory
void* mems_malloc(size_t size) {
    if (free_list_head == NULL) {
        // No segments available, allocate a new one using mmap.
        void* new_segment_ptr = mmap(NULL, PAGE_SIZE, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
        struct Segment* new_segment = (struct Segment*)malloc(sizeof(struct Segment));
        new_segment->size = PAGE_SIZE;
        new_segment->type = 1; // PROCESS
        new_segment->ptr = new_segment_ptr;
        new_segment->next = NULL;
        new_segment->prev = NULL;

        // Create a new node in the main chain.
        struct Node* new_node = (struct Node*)malloc(sizeof(struct Node));
        new_node->sub_chain = new_segment;
        new_node->next = NULL;
        new_node->prev = NULL;

        free_list_head = new_node;
    }

    // Search for a suitable segment in the free list.
    struct Node* current_node = free_list_head;
    while (current_node != NULL) {
        struct Segment* current_segment = current_node->sub_chain;
        while (current_segment != NULL) {
            if (current_segment->type == 0 && current_segment->size >= size) {
                // Found a HOLE segment of sufficient size.
                current_segment->type = 1; // Change to PROCESS.
                // TODO: use the required space and make new hole and adjust the addresses.
                return current_segment->ptr;
            }
            current_segment = current_segment->next;
        }
        current_node = current_node->next;
    }

    // No suitable segment found, allocate a new one.
    void* new_segment_ptr = mmap(NULL, PAGE_SIZE, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    struct Segment* new_segment = (struct Segment*)malloc(sizeof(struct Segment));
    new_segment->size = PAGE_SIZE;
    new_segment->type = 1; // PROCESS
    new_segment->ptr = new_segment_ptr;
    new_segment->next = NULL;
    new_segment->prev = NULL;

    // Create a new node in the main chain.
    struct Node* new_node = (struct Node*)malloc(sizeof(struct Node));
    new_node->sub_chain = new_segment;
    new_node->next = NULL;
    new_node->prev = NULL;

    // Link the new node to the end of the free list.
    struct Node* last_node = free_list_head;
    while (last_node->next != NULL) {
        last_node = last_node->next;
    }
    last_node->next = new_node;
    new_node->prev = last_node;

    return new_segment_ptr;
}

int main() {
    mems_init();

    // Test mems_malloc here

    return 0;
}
