/*
** Matthew Aquiles, Thomas Falsone
** 4/13/2018
** 
*/

//Preprocessor
#ifndef NULL
    #define NULL ((void*) 0)
#endif /*NULL*/

#include "FIFO.h"
#include "LRU.h"
#include "CLOCK.h"

#define FALSE 0
#define TRUE 1
#define POWER 5

//algorithms
#define FIFO 0
#define LRU 1
#define CLOCK 2

//paging type
#define PLUS 1  //pre-paging
#define MINUS 0 //demand

#define MAIN_MEMORY 512	
    
//structs
typedef int algorithms;
typedef int bool;

typedef struct s_page{
    int pagenumber;
    bool validbid;
} 	t_page;

typedef struct s_program{
    int pid;
    int init_memorylocation;
    int total_memorylocation;
    int total_pages;
    t_page *pagetable;
    fifo_queue* fifo;
    lru_queue* lru;
    clock_queue* clk;
} 	t_program;

//global variables
char *global_plist;         
char *global_ptrace;        
int global_page_size;       
algorithms global_page_alg; //alorithm to use (fifo, lru, clock)
bool global_page_flag;      //flag +/- flag for paging
t_program *global_programs; 
int global_page_count;      
int global_program_count;   
int global_pageID;          