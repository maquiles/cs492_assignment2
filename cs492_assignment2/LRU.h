/*
** Matthew Aquiles, Thomas Falsone
** 4/13/2018
** 
*/

#ifndef _lru_
	#define _lru_
   
	#ifndef NULL
		#define NULL ((void*) 0)
	#endif /*NULL*/	
   
	#include <errno.h>  
	#include <stdio.h>
	#include <stdlib.h>
	#include <unistd.h>

	typedef struct lru_node
	{
		int* object;
		struct lru_node *previous;
		struct lru_node *next;
		int count;
	} 	lru_node;

	typedef struct lru_queue
	{
		lru_node *head;
		int size;
	}	lru_queue;

	int lru_init(lru_queue *);
	int lru_push(lru_queue *, void *, int);
	int lru_node_update(lru_queue *, void *);
	void* lru_pop(lru_queue *);
	int lru_remove(lru_queue *, void *);

	void prepaging_lru();
	void demand_lru();

#endif /*_lru_*/
