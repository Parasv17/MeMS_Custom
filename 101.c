// #include"mems.h"
#include<stdio.h>

#include <stdlib.h>
#include <sys/mman.h>



typedef struct sub_node{
    int data;
    char type;
    int size;
    struct sub_node *next_sub;
    struct sub_node *prev_sub;


}SUB_NODE;
typedef struct node {
    int data;
    struct node *next;
    struct node *prev;
    struct sub_node *sub_head;
}NODE;



// Function to display the sub-nodes of a NODE


NODE *createNode() {
    NODE *newNode = (NODE *)mmap(NULL, 4096, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    if (newNode == MAP_FAILED) {
        perror("mmap");
        exit(EXIT_FAILURE);
    }
    newNode->next = NULL;
    newNode->prev = NULL;
    newNode->sub_head = NULL;
    return newNode;
}

// Function to create a new SUB_NODE using mmap()
SUB_NODE *createSubList(){

}


int main(){

    printf("startnn n %lu\n", sizeof(NODE));
    NODE *head = createNode();
    NODE *temp = createNode();

    printf("%p\n", &head);
    printf("%p\n", temp);



}