/*
** Matthew Aquiles, Thomas Falsone
** 4/13/2018
** 
*/

#include "CLOCK.h"
#include "assignment.h"

int clock_init(clock_queue* q){
    q->size = 0;
    q->head = NULL;
    return 0;
}

int clock_node_update(clock_queue *q, void *obj){
    clock_node *node;
    node = q->head;

    if(q == NULL || (void*)obj == NULL){
        return -1;
    }

    while (node->object != (void*)obj){
        node = node->next;
    }

    node->r = 1;
    q->head = node->next;
    return 0;
}

void* clock_pop(clock_queue* q){
    void* obj;
    clock_node* node;
    clock_node* curr;

    if(q == NULL || q->head == NULL){
        return NULL;
    }

    curr = q->head;

    while (curr->r != 0 && curr->next != q->head){
        curr = curr->next;
    }

    if(curr->next == q->head){
        curr = q->head;
        do{
            curr->r = 0;
            curr = curr->next;
        } while (curr != q->head);
    }

    q->head = curr;
    node = q->head;
    obj = (void*)node->object;

    if(q->size-- == 1){
        q->head = NULL;
    }
    else{
        q->head = node->next;
        q->head->previous = node->previous;
        node->previous->next = q->head;
    }
    return (void*)obj;
}

int clock_push(clock_queue* q, void* obj){
    clock_node* node;

    if(q == NULL || (void*)obj == NULL){
        return -1;
    }

    node = (clock_node*)malloc(sizeof(clock_node)+1);
    node->object = (void*)obj;
    node->r = 1;

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
        q->head = node->next;
    }
    
    q->size++;
    return 0;
}

void prepaging_clock(){
	FILE *fd;
	int k,j;
	t_page *pop;

	unsigned int count = 0;
	unsigned int pageFault = 0;

	fd = fopen(global_ptrace, "r");
	
    if(fd == NULL){
		printf("ERROR: could not read from file.\n");
    }

	while (fscanf(fd, "%i %i" , &k, &j) == 2){
		j /= global_page_size;
        
		if(j == global_programs[k].total_pages){
			j--;
        }

		if(global_programs[k].pagetable[j].validbid == FALSE){
			pop = clock_pop(global_programs[k].clk);
			pop->validbid = 0;
			global_programs[k].pagetable[j].validbid = 1;
		
			clock_push(global_programs[k].clk, &(global_programs[k].pagetable[j]));  
			
			//find second page for pre_load
			while (global_programs[k].pagetable[++j].validbid == 1 && j == global_programs[k].total_pages){
				if(j == global_programs[k].total_pages){
					j = -1;
                }
			}
			pop = clock_pop(global_programs[k].clk);
			pop->validbid = 0;
			global_programs[k].pagetable[j].validbid = 1;
			clock_push(global_programs[k].clk, &(global_programs[k].pagetable[j])); 
			pageFault++;			
		}
        else{
			clock_node_update(global_programs[k].clk, &(global_programs[k].pagetable[j]));
		}	
		count++;
	}

	printf("Alg: %i \t Paging: %i \t Page Size: %i \t Total Records: %u \t Page Faults: %u \n ", global_page_alg, global_page_flag, global_page_size, count, pageFault);
	fclose(fd);
}	

void demand_clock(){
	FILE *fd;
	int k,j;
	t_page *pop;

	unsigned int count = 0;
	unsigned int pageFault = 0;
    
	fd = fopen(global_ptrace, "r");

	if (fd == NULL){
		printf("ERROR: clould not read from file\n");
    }

	while (fscanf(fd, "%i %i" , &k, &j)==2){
		j = j / global_page_size;
		if (j == global_programs[k].total_pages){
			j--;
        }
        
		if (global_programs[k].pagetable[j].validbid == FALSE){
			pop = clock_pop(global_programs[k].clk);
			pop->validbid = 0;
			global_programs[k].pagetable[j].validbid = 1;
			clock_push(global_programs[k].clk, &(global_programs[k].pagetable[j]));  
			pageFault++;
		}
        else{
			clock_node_update(global_programs[k].clk, &(global_programs[k].pagetable[j]));			
		}	
		count++;
	}

	printf("Alg: %i \t Paging: %i \t Page Size: %i \t Total Records: %u \t Page Faults: %u \n ", global_page_alg, global_page_flag, global_page_size, count, pageFault);
	fclose(fd);
}