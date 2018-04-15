/*
** Matthew Aquiles, Thomas Falsone
** 4/13/2018
** 
*/

#include "LRU.h"
#include "assignment.h"

int lru_init(lru_queue* q){
    q->size = 0;
    q->head = NULL;
    return 0;
}

int lru_node_update(lru_queue *q, void *obj){
    lru_node *node;
    node = q->head;

    if(q == NULL || obj == NULL){
        return -1;
    }

    while (node->object != obj){
        node = node->next;
    }

    node->count++;
    lru_remove(q, obj);
    lru_push(q, obj, node->count);
    return node->count;
}

void* lru_pop(lru_queue* q){
    void* obj;
    lru_node* node;

    if(q == NULL || q->head == NULL){
        return NULL;
    }

    node = q->head;
    obj = node->object;

    if(q->size-- == 1){
        q->head = NULL;
    }
    else{
        q->head = node->next;
        q->head->previous = node->previous;
        node->previous->next = q->head;
    }
    return obj;
}

int lru_push(lru_queue* q, void* obj, int priority){
    lru_node* node;
    lru_node* curr;
    lru_node* tail;

    if(q == NULL || q == NULL){
        return -1;
    }

    node = (lru_node*)malloc(sizeof(lru_node));
    node->object = obj;
    node->count = priority;
    curr = q->head;

    if(q->head != NULL){
        tail = curr->previous;
    }
    
    if(curr == NULL){
        node->previous = node;
        node->next = node;
        q->head = node;
    }
    else if(tail == curr){
        if(node->count < curr->count){
            q->head = node;
        }

        node->next = curr;
        node->previous = curr;
        curr->next = node;
        curr->previous = node;
    }
    else{
        while (node->count >= curr->count && tail != curr){
            curr = curr->next;
        }

        if(node->count < curr->count){
            node->next = curr;
            node->previous = curr->previous;
            node->previous->next = node;
            node->next->previous = node;
        }
        else{
            node->next = curr->next;
            node->previous = curr;
            node->previous->next = node;
            node->next->previous = node;
        }
    }
    q->size++;
    return 0;
}

int lru_remove(lru_queue *q, void *obj){
    lru_node *node;
    lru_node *curr;

    if(q == NULL || obj == NULL){
        return -1;
    }

    curr = q->head;
    node = q->head;

    if(q->size-- == 1){
        q->head = NULL;
    }
    else{
        if(q->head->object == obj){
            q->head = node->next;
            q->head->previous = node->previous;
            node->previous->next = q->head;
        }
        else{
            while (node->object != obj){
                node = node->next;
            }
            curr = node->next;
            curr->previous = node->previous;
            node->previous->next = curr;
        }
    }
    return 0;
}

void prepaging_lru(){
	FILE *fd;
	int k,j, z;
	t_page *pop;

	unsigned int count = 0;
	unsigned int pageFault = 0;

    fd = fopen(global_ptrace, "r");
	
    if (fd == NULL)
		printf("ERROR: could not read from file.\n");

	while (fscanf(fd, "%i %i" , &k, &j)==2){
		j /= global_page_size;
        
		if (j == global_programs[k].total_pages){
			j--;
        }

		if (global_programs[k].pagetable[j].validbid == FALSE){
			pop = lru_pop(global_programs[k].lru);
			pop->validbid = 0;
			global_programs[k].pagetable[j].validbid = 1;
			lru_push(global_programs[k].lru, &(global_programs[k].pagetable[j]),1);  
			
			while (global_programs[k].pagetable[++j].validbid == 1 && j == global_programs[k].total_pages){
				if (j == global_programs[k].total_pages)
					j = -1;
			}
			pop = lru_pop(global_programs[k].lru);
			pop->validbid = 0;
			global_programs[k].pagetable[j].validbid = 1;
			lru_push(global_programs[k].lru, &(global_programs[k].pagetable[j]), 1); 
			pageFault++;
		}else{
			z = lru_node_update(global_programs[k].lru, &(global_programs[k].pagetable[j]));
		}	
		count++;
	}

	printf("Alg: %i \t Paging: %i \t Page Size: %i \t Total Records: %u \t Page Faults: %u \n ", global_page_alg, global_page_flag, global_page_size, count, pageFault);
	fclose(fd);
}	

void demand_lru(){
	FILE *fd;
	int k,j, z;
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
			pop = lru_pop(global_programs[k].lru);
			pop->validbid = 0;
			global_programs[k].pagetable[j].validbid = 1;
			lru_push(global_programs[k].lru, &(global_programs[k].pagetable[j]),1);  
			pageFault++;
		}
        else{
			z = lru_node_update(global_programs[k].lru, &(global_programs[k].pagetable[j]));
			
		}	
		count++;
	}

	printf("Alg: %i \t Paging: %i \t Page Size: %i \t Total Records: %u \t Page Faults: %u \n ", global_page_alg, global_page_flag, global_page_size, count, pageFault);
	fclose(fd);
}	