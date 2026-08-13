#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stddef.h>


typedef struct block_meta{
    size_t size;
    int free;
    struct block_meta *next;
} block_meta;


static block_meta *global_head = NULL;

block_meta *find_free_block(block_meta **last, size_t size ){
    block_meta *current = global_head;
    while(current && !(current -> free && current -> size >= size)){
        *last = current;
        current = current -> next;    
    } return current;
}

 block_meta *request_space(block_meta *last, size_t size) {
     block_meta *block = (block_meta *)sbrk(0);
    
    void *request = sbrk(size + sizeof(block_meta));
    if (request == (void *)-1) {
        return NULL; 
    }

    if (last) {
        last->next = block;
    }
    block->size = size;
    block->free = 0;
    block->next = NULL;

    return block;
}

void *custom_malloc(size_t size){
    if(size == 0){
        return NULL;
    }
    block_meta *block;
    if(!global_head){
        block = request_space(NULL,size);
        if(!block) return NULL;
        global_head = block;
    }else{
        block_meta *last= global_head;
        block = find_free_block(&last, size);
        if(!block){
            block = request_space(last,size);
            if(!block)return NULL;
        }else{
            block -> free = 0;
        }
    }
    return (void *)(block + 1);
}

void custom_free(void *ptr) {
    if (!ptr) return;

     block_meta *block_ptr = (block_meta *)ptr - 1;
    
    block_ptr->free = 1;
}

int main(void) {
    printf("====================================================\n");
    printf("        USER-SPACE MEMORY ALLOCATOR TEST            \n");
    printf("====================================================\n");
    printf("Header Size: %zu bytes\n\n", sizeof(struct block_meta));

    // Step A: Allocate 3 separate memory blocks
    char *str1 = (char *)custom_malloc(64);
    char *str2 = (char *)custom_malloc(64);
    char *str3 = (char *)custom_malloc(64);

    if (!str1 || !str2 || !str3) {
        printf("Allocation failed!\n");
        return 1;
    }


    strcpy(str1, "Block 1: Systems Programming");
    strcpy(str2, "Block 2: Virtual Memory");
    strcpy(str3, "Block 3: Operating Systems");


    printf("Initial Allocations:\n");
    printf("  str1 Payload Address: %p | Data: \"%s\"\n", (void *)str1, str1);
    printf("  str2 Payload Address: %p | Data: \"%s\"\n", (void *)str2, str2);
    printf("  str3 Payload Address: %p | Data: \"%s\"\n", (void *)str3, str3);

   
    ptrdiff_t distance = (char *)str2 - (char *)str1;
    printf("\nDistance between Payload 1 & Payload 2: %td bytes\n", distance);
    printf("  (64 payload bytes + %zu header bytes = %td bytes)\n\n", 
           sizeof(struct block_meta), distance);


    printf("--> Freeing str2 (%p)...\n\n", (void *)str2);
    custom_free(str2);

    
    printf("Allocating str4 (64 bytes)...\n");
    char *str4 = (char *)custom_malloc(64);
    strcpy(str4, "Block 4: Reused Memory!");

    printf("  str4 Payload Address: %p | Data: \"%s\"\n\n", (void *)str4, str4);


    if (str4 == str2) {
        printf("[SUCCESS] str4 recycled the freed memory address of str2!\n");
    } else {
        printf("[EXPANSION] str4 was allocated at a new heap boundary.\n");
    }
    printf("====================================================\n");

    return 0;
}