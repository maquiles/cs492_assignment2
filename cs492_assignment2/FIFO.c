/*
** Matthew Aquiles, Thomas Falsone
** 4/13/2018
** 
*/

#include "FIFO.h"
#include "assignment.h"

int fifo_init(fifo_queue* q){
    q->size = 0;
    q->head = NULL;
    return 0;
}

void* fifo_pop(fifo_queue* q){
    void *obj;
    fifo_node *node;

    if(q == NULL){
        return NULL;
    }

    node = q->head->previous;
    obj = node->object;

    if(q->size-- == 1){
        q->head = NULL;
    }
    else{
        q->head->previous = node->previous;
        node->previous->next = q->head;
    }
    return (void*)obj;
}

int fifo_push(fifo_queue* q, void* obj){
    fifo_node* node;

    if(q == NULL || obj == NULL){
        return -1;
    }

    node = (fifo_node*)malloc(sizeof(fifo_node));
    node->object = obj;

    if(q->size < 1){
        q->head = node;
        node->previous = node;
        node->next = node;
    }
    else{
        node->next = q->head;
        node->previous = q->head->previous;
        node->previous->next = node;
        node->next->previous = node;
        q->head = node;
    }
    q->size++;
    return 0;
}

void prepaging_fifo(){
    FILE *fd;
    int k, j;
    t_page *pop;

    unsigned int count = 0;
    unsigned int pageFault = 0;
    
    fd = fopen(global_ptrace, "r");

    if(fd == NULL){
        printf("ERROR: could not read from file");
    }

    while (fscanf(fd, "%i %i", &k, &j) == 2){
        j /= global_page_size;

        if(j == global_programs[k].total_pages){
            j--;
        }

        if(global_programs[k].pagetable[j].validbid == FALSE){
            pop = fifo_pop(global_programs[k].fifo);
            pop->validbid = 0;
            global_programs[k].pagetable[j].validbid = 1;
            fifo_push(global_programs[k].fifo, &(global_programs[k].pagetable[j]));

            while (global_programs[k].pagetable[++j].validbid == 1 && j == global_programs[k].total_pages){
                if(j == global_programs[k].total_pages){
                    j = -1;
                }
            }

            pop = fifo_pop(global_programs[k].fifo);
            pop->validbid = 0;
            global_programs[k].pagetable[j].validbid = 1;
            fifo_push(global_programs[k].fifo, &(global_programs[k].pagetable[j]));
            pageFault++;
        }
        else{
            //la di da
        }
        count++;
    }
    printf("Alg: %i \tPaging: %i \tPage Size: %i \tPage Swaps: %u \tPage Faults: %u \n ", global_page_alg, global_page_flag, global_page_size, count, pageFault);
    fclose(fd);
}

void demand_fifo(){
    FILE *fd;
    int k, j;
    t_page *pop;

    unsigned int count = 0;
    unsigned int pageFault = 0;
    
    fd = fopen(global_ptrace, "r");

    if(fd == NULL){
        printf("ERROR: could not read from file");
    }

    while (fscanf(fd, "%i %i", &k, &j) == 2){
        j /= global_page_size;
        if(j == global_programs[k].total_pages){
            j--;
        }

        if(global_programs[k].pagetable[j].validbid == FALSE){
            pop = fifo_pop(global_programs[k].fifo);
            pop->validbid = 0;
            global_programs[k].pagetable[j].validbid = 1;
            fifo_push(global_programs[k].fifo, &(global_programs[k].pagetable[j]));
            pageFault++;
        }
        else{
            //la di da
        }
        count++;
    }
    printf("Alg: %i \tPaging: %i \tPage Size: %i \tPage Swaps: %u \tPage Faults: %u \n ", global_page_alg, global_page_flag, global_page_size, count, pageFault);
    fclose(fd);
}