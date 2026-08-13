#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define BUFFER_SIZE 100

typedef struct Node {
    char data[BUFFER_SIZE];
    struct Node* next;
} Node;

int main(){
    Node *head = NULL;
    Node *tail = NULL;
    char buffer[BUFFER_SIZE];
    printf("--- Dynamic Linked List Builder ---\n");
    printf("Type words/lines to add to the list. Type 'EXIT' to finish.\n\n");
    while(1){
        printf("> ");
        if(fgets(buffer,sizeof(buffer),stdin) == NULL){
            printf("Error reading input. Exiting.\n");
            break;
        }
        if(strcmp(buffer,"EXIT\n") == 0){
            printf("Finished building the list.\n");
            break;
        }

        Node *new_node = (Node*)malloc(sizeof(Node));
        if(new_node == NULL){
            printf("Memory allocation failed. Exiting.\n");
            break;
        }

        strncpy(new_node -> data, buffer, BUFFER_SIZE - 1);
        new_node -> data[BUFFER_SIZE - 1] = '\0'; // Ensure null termination
        new_node -> next = NULL;

        if(head == NULL){
            head = new_node;
            tail = new_node;
        } else {
            tail -> next = new_node;
            tail = new_node;
        }
    }
    printf("\n=========================================\n");
    printf("            YOUR LINKED LIST             \n");
    printf("=========================================\n");

    Node *current = head;
    int count = 1;
    while(current != NULL){
        printf("Node %d: %s", count, current -> data);
        current = current -> next;
        count++;
    }

    current = head;
    while(current != NULL){
        Node *temp = current;
        current = current -> next;
        free(temp);
    }
    head = NULL;
    tail = NULL;
    printf("\nAll nodes have been freed. Exiting program.\n");
    return 0;
}
