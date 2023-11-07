/*
All the main functions with respect to the MeMS are inplemented here
read the function discription for more details

NOTE: DO NOT CHANGE THE NAME OR SIGNATURE OF FUNCTIONS ALREADY PROVIDED
you are only allowed to implement the functions
you can also make additional helper functions a you wish

REFER DOCUMENTATION FOR MORE DETAILS ON FUNSTIONS AND THEIR FUNCTIONALITY
*/
// add other headers as required
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>

/*
Use this macro where ever you need PAGE_SIZE.
As PAGESIZE can differ system to system we should have flexibility to modify this
macro to make the output of all system same and conduct a fair evaluation.
*/
#define PAGE_SIZE 4096

typedef struct SUB_NODE
{
    char type;
    int sub_vadd_st;
    int sub_vadd_end;
    size_t size;

    struct SUB_NODE *next_sub;
    struct SUB_NODE *prev_sub;

} SUB_NODE;

typedef struct node
{

    size_t size;
    struct node *next;
    struct node *prev;
    struct SUB_NODE *sub_head;
    int vadd_st;
    int vadd_end;
    int *phy_add;
} NODE;

NODE *head_main;
int VADD_ST;
int * Main_Page;
int *copy;

/*
Initializes all the required parameters for the MeMS system. The main parameters to be initialized are:
1. the head of the free list i.e. the pointer that points to the head of the free list
2. the starting MeMS virtual address from which the heap in our MeMS virtual address space will start.
3. any other global variable that you want for the MeMS implementation can be initialized here.
Input Parameter: Nothing
Returns: Nothing
*/
void mems_init()
{

    // intialise the head of the free list
    head_main = NULL;
    // intialise the starting MeMS virtual address from which the heap in our MeMS virtual address space will start.
    VADD_ST = 1000;
    Main_Page = (int *)mmap(NULL, PAGE_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    copy= Main_Page;
    // intially sublist will have a node of size of the size of mmap
}

/*
This function will be called at the end of the MeMS system and its main job is to unmap the
allocated memory using the munmap system call.
Input Parameter: Nothing
Returns: Nothing
*/
void mems_finish()
{
    NODE *temp = (NODE *)copy;
    while(temp!=NULL){
        munmap(temp->phy_add, temp->size);
        temp=temp->next;
    }

    munmap(temp, PAGE_SIZE);
    head_main = NULL;
    // while (temp != NULL)
    // {
    //     printf("ajbdfak\n");

    //     NODE *free_node = temp;
    //     munmap(free_node->phy_add, free_node->size);
    //     SUB_NODE *temp_sub = temp->sub_head;

    //     while (temp_sub != NULL)
    //     {
    //         printf("inside\n");
    //         SUB_NODE *free_sub = temp_sub;
    //         temp_sub = temp_sub+sizeof(SUB_NODE);

    //         munmap(free_sub, sizeof(SUB_NODE));
    //     }
    //     temp = temp+sizeof(NODE);
    //     munmap(free_node, sizeof(NODE));
    //     head_main = NULL;
    // }


    // NODE *temp = copy;
    // while(temp!=NULL){
    //      \
    // }
}

/*
Allocates memory of the specified size by reusing a segment from the free list if
a sufficiently large segment is available.

Else, uses the mmap system call to allocate more memory on the heap and updates
the free list accordingly.

Note that while mapping using mmap do not forget to reuse the unused space from mapping
by adding it to the free list.
Parameter: The size of the memory the user program wants
Returns: MeMS Virtual address (that is created by MeMS)
*/
void *mems_malloc(size_t size)
{
    // printf("PAges ize%d\n",PAGE_SIZE );
    // printf("size requested %d\n", size);
    size_t size1 = (((int)(size / PAGE_SIZE)));
    if (size % PAGE_SIZE != 0)
    {
        size1++;
        size1 = size1 * PAGE_SIZE;
    }
    else
    {
        size1 = size1 * PAGE_SIZE;
    }

    // printf("size in pages %d\n", size1);
    NODE *temp = head_main;
    NODE *last_node;
    long VIRT_ADD_RET;

    // if no node exist
    if (temp == NULL)
    {
        // create a new node
        // NODE *newNode = (NODE *)mmap(NULL, PAGE_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
        NODE *newNode = (NODE *)Main_Page;
        Main_Page = Main_Page + sizeof(NODE);

        newNode->next = NULL;
        newNode->prev = NULL;
        newNode->size = size1;
        newNode->vadd_st = VADD_ST;
        newNode->vadd_end = VADD_ST + size1 - 1;

        newNode->phy_add = (int *)mmap(NULL, size1, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);

        // newNode->sub_head = (SUB_NODE *)mmap(NULL, PAGE_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);

        newNode->sub_head = (SUB_NODE *)Main_Page;
        Main_Page = Main_Page + sizeof(SUB_NODE);

        newNode->sub_head->type = 'H';
        newNode->sub_head->sub_vadd_st = newNode->vadd_st;
        newNode->sub_head->sub_vadd_end = newNode->vadd_end;
        newNode->sub_head->size = size1;
        newNode->sub_head->next_sub = NULL;
        newNode->sub_head->prev_sub = NULL;

        if (newNode->sub_head->size >= size)
        {
            newNode->sub_head->type = 'P';
            newNode->sub_head->size = size;
            newNode->sub_head->sub_vadd_end = newNode->sub_head->sub_vadd_st + size - 1;

            // SUB_NODE *newSub = (SUB_NODE *)mmap(NULL, PAGE_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
            if(newNode->size-size>0){
            SUB_NODE *newSub = (SUB_NODE *)Main_Page;
            Main_Page = Main_Page + sizeof(SUB_NODE);
            newSub->type = 'H';
            newSub->sub_vadd_st = newNode->sub_head->sub_vadd_st + size;
            newSub->sub_vadd_end = newNode->vadd_end;
            newSub->size = size1 - size;
            newSub->next_sub = NULL;
            newSub->prev_sub = newNode->sub_head;

            newNode->sub_head->next_sub = newSub;
            }
            VADD_ST = VADD_ST + size1;
            head_main = newNode;
            return (void *)(long)newNode->sub_head->sub_vadd_st;
        }

        // return newNode->vadd_st;
    }
    else
    {
        while (temp != NULL)
        {

            SUB_NODE *temp_sub = temp->sub_head;
            while (temp_sub != NULL)
            {

                if (temp_sub->type == 'H' && temp_sub->size >= size)
                {
                    temp_sub->type = 'P';
                    int size_check=temp_sub->size;
                    temp_sub->size = size;
                    // create a new sublist node of size (temp->sub_head->size - size)
                    temp_sub->sub_vadd_end = temp_sub->sub_vadd_st + size - 1;

                    // SUB_NODE *newSub = (SUB_NODE *)mmap(NULL, PAGE_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
                    if(size_check-size>0){
                    SUB_NODE *newSub =(SUB_NODE*) Main_Page;
                    Main_Page = Main_Page + sizeof(SUB_NODE);

                    newSub->type = 'H';
                    newSub->sub_vadd_st = temp_sub->sub_vadd_st + size;
                    if (temp_sub->next_sub != NULL)
                    {
                        temp_sub->next_sub->prev_sub = newSub;
                        newSub->sub_vadd_end = temp_sub->next_sub->sub_vadd_st - 1;
                    }
                    else
                    {

                        newSub->sub_vadd_end = temp->vadd_end;
                    }
                    newSub->size = newSub->sub_vadd_end - newSub->sub_vadd_st + 1;

                    newSub->next_sub = temp_sub->next_sub;

                    newSub->prev_sub = temp_sub;

                    temp_sub->next_sub = newSub;
                    }
                    return (void *)(long)temp_sub->sub_vadd_st;
                }
                temp_sub = temp_sub->next_sub;
            }
            last_node = temp;
            temp = temp->next;
        }
    }
    // if no hole is found then use mmap

    // printf("No hole found creating new main chain node\n");
    // NODE *newNode = (NODE *)mmap(NULL, PAGE_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    NODE *newNode = (NODE *)Main_Page;
    Main_Page = Main_Page + sizeof(NODE);
    newNode->next = NULL;
    newNode->prev = last_node;
    newNode->size = size1;
    newNode->vadd_st = VADD_ST;
    newNode->vadd_end = VADD_ST + size1 - 1;

    newNode->phy_add = (int *)mmap(NULL, size1, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);

    // newNode->sub_head = (SUB_NODE *)mmap(NULL, PAGE_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
     newNode->sub_head = (SUB_NODE *)Main_Page;
            Main_Page=Main_Page+ sizeof(SUB_NODE);
    newNode->sub_head->type = 'H';
    newNode->sub_head->sub_vadd_st = newNode->vadd_st;
    newNode->sub_head->sub_vadd_end = newNode->vadd_end;
    newNode->sub_head->size = size1;
    newNode->sub_head->next_sub = NULL;
    newNode->sub_head->prev_sub = NULL;

    if (newNode->size >= size)
    {
        newNode->sub_head->type = 'P';
        newNode->sub_head->size = size;
        newNode->sub_head->sub_vadd_end = newNode->sub_head->sub_vadd_st + size - 1;

        // SUB_NODE *newSub = (SUB_NODE *)mmap(NULL, PAGE_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
        if(newNode->size-size>0){
        SUB_NODE *newSub = (SUB_NODE *)Main_Page;
        Main_Page = Main_Page + sizeof(SUB_NODE);
        

        newSub->type = 'H';
        newSub->sub_vadd_st = newNode->sub_head->sub_vadd_st + size;
        newSub->sub_vadd_end = newNode->vadd_end;
        newSub->size = newNode->size - size;
        newSub->next_sub = NULL;
        newSub->prev_sub = newNode->sub_head;

        newNode->sub_head->next_sub = newSub;
        }
        VADD_ST = VADD_ST + size1;

        last_node->next = newNode;
        return (void *)(long)newNode->sub_head->sub_vadd_st;
    }

    // intially sublist will have a node of size of the size of mmap
    // aasing the first node of the sublk
}

/*
this function print the stats of the MeMS system like
1. How many pages are utilised by using the mems_malloc
2. how much memory is unused i.e. the memory that is in freelist and is not used.
3. It also prints details about each node in the main chain and each segment (PROCESS or HOLE) in the sub-chain.
Parameter: Nothing
Returns: Nothing but should print the necessary information on STDOUT
*/
void mems_print_stats()
{
    NODE *temp = head_main;
    int pages_used = 0;
    int main_len = 0;
    int sub_chainlarray[1000];
    int space_unused = 0;

    int i = 0;
    while (temp != NULL)
    {
        main_len++;
        pages_used += temp->size / PAGE_SIZE;
        printf("MAIN[%d:%d]->", temp->vadd_st, temp->vadd_end);

        int len = 0;
        SUB_NODE *temp_sub = temp->sub_head;
        while (temp_sub != NULL)
        {
            if (temp_sub->type == 'H')
            {
                space_unused += temp_sub->size;
            }
            len++;
            printf("%c[%d:%d] <-> ", temp_sub->type, temp_sub->sub_vadd_st, temp_sub->sub_vadd_end);

            temp_sub = temp_sub->next_sub;
        }
        sub_chainlarray[i] = len;
        i++;
        printf("NULL\n");

        temp = temp->next;
    }

    printf("Pages used: %d\n", pages_used);
    printf("Space unused: %d\n", space_unused);
    printf("Main chain length: %d\n", main_len);
    printf("Sub chain length array: [");
    for (int j = 0; j < i; j++)
    {
        printf("%d, ", sub_chainlarray[j]);
    }
    printf("]\n");
    printf("----------------------------------------------------\n");
}

/*
Returns the MeMS physical address mapped to ptr ( ptr is MeMS virtual address).
Parameter: MeMS Virtual address (that is created by MeMS)
Returns: MeMS physical address mapped to the passed ptr (MeMS virtual address).
*/
void *mems_get(void *v_ptr)
{   
    long v_ptr_new=(long)(v_ptr);
    NODE *temp = head_main;
    while (temp != NULL)
    {
        if (temp->vadd_st <= v_ptr_new && temp->vadd_end >= v_ptr_new)
        {
            return (void *)temp->phy_add + (size_t)v_ptr - temp->vadd_st;
        }
    }
    temp = temp->next;
}

/*
this function free up the memory pointed by our virtual_address and add it to the free list
Parameter: MeMS Virtual address (that is created by MeMS)
Returns: nothing
*/

void defragment()
{

    NODE *temp = head_main;
    while (temp != NULL)
    {
        SUB_NODE *temp_sub = temp->sub_head;
        while (temp_sub != NULL)
        {
            if (temp_sub->type == 'H')
            {
                if (temp_sub->prev_sub != NULL && temp_sub->prev_sub->type == 'H' && temp_sub->next_sub != NULL && temp_sub->next_sub->type == 'H')
                {
                    //     temp_sub->prev_sub->size += temp_sub->size + temp_sub->next_sub->size;
                    //     temp_sub->prev_sub->sub_vadd_end = temp_sub->next_sub->sub_vadd_end;
                    //     temp_sub->prev_sub->next_sub = temp_sub->next_sub->next_sub;
                    //     temp_sub->next_sub->next_sub->prev_sub = temp_sub->prev_sub;
                    //     temp_sub = temp_sub->next_sub->next_sub;
                }
                else if (temp_sub->prev_sub != NULL && temp_sub->prev_sub->type == 'H')
                {
                    temp_sub->prev_sub->size += temp_sub->size;
                    temp_sub->prev_sub->sub_vadd_end = temp_sub->sub_vadd_end;
                    temp_sub->prev_sub->next_sub = temp_sub->next_sub;
                    temp_sub->next_sub->prev_sub = temp_sub->prev_sub;
                    temp_sub = temp_sub->next_sub;
                    temp_sub = temp_sub->prev_sub;
                }
                else if (temp_sub->next_sub != NULL && temp_sub->next_sub->type == 'H')
                {
                    temp_sub->next_sub->size += temp_sub->size;
                    temp_sub->next_sub->sub_vadd_st = temp_sub->sub_vadd_st;
                    temp_sub->next_sub->prev_sub = temp_sub->prev_sub;
                    temp_sub->prev_sub->next_sub = temp_sub->next_sub;
                    temp_sub = temp_sub->next_sub;
                    temp_sub = temp_sub->prev_sub;
                }
            }
            temp_sub = temp_sub->next_sub;
        }
        temp = temp->next;
    }
}
void mems_free(void *v_ptr)
{
    long v_ptr_new=(long)(v_ptr);
    // printf("%d", (int)v_ptr);
    NODE *temp = head_main;

    while (temp != NULL)
    {
        SUB_NODE *temp_sub = temp->sub_head;
        while (temp_sub != NULL)
        {
            if (temp_sub->sub_vadd_st <= v_ptr_new && temp_sub->sub_vadd_end >= v_ptr_new && temp_sub->type == 'P')
            {
                temp_sub->type = 'H';
                break;
            }

            temp_sub = temp_sub->next_sub;
        }
        temp = temp->next;
    }
    printf("freed up\n");
    defragment();
}
