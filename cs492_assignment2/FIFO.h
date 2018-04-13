/*
** Matthew Aquiles, Thomas Falsone
** 4/13/2018
** 
*/

#ifndef _fifo_
	#define _fifo_
   
	#ifndef NULL
		#define NULL ((void*) 0)
	#endif /*NULL*/	
   
	#include <errno.h>  
	#include <stdio.h>
	#include <stdlib.h>
	#include <unistd.h>

	typedef struct fifo_node
	{
		void* object;
		struct fifo_node *previous;
		struct fifo_node *next;
	} 	fifo_node;

	typedef struct fifo_queue
	{
		fifo_node *head;
		int size;
	}	fifo_queue;

	int fifo_init(fifo_queue *);
	int fifo_push(fifo_queue *, void *);
	void* fifo_pop(fifo_queue *);

	void prepaging_fifo();
	void demand_fifo();

#endif /*_fifo_*/
