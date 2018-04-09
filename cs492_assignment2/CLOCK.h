/*
** Matthew Aquiles, Thomas Falsone
** 4/13/2018
** 
*/

#ifndef _clock_
	#define _clock_
   
	#ifndef NULL
		#define NULL ((void*) 0)
	#endif /*NULL*/	
   
	#include <errno.h>  
	#include <stdio.h>
	#include <stdlib.h>
	#include <unistd.h>	

	typedef struct clock_node
	{
		int* object;
		struct clock_node *previous;
		struct clock_node *next;
		int r;
	} 	clock_node;

	typedef struct clock_queue
	{
		clock_node *head;
		int size;
	}	clock_queue;

	int clock_init(clock_queue *);
	int clock_push(clock_queue *, void *);
	int clock_node_update(clock_queue *, void *);
	void* clock_pop(clock_queue *);	

    void prepaging_clock();
    void demand_clock();

#endif /*_clock_*/
