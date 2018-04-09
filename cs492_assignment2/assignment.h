/*
** Matthew Aquiles, Thomas Falsone
** 4/13/2018
** 
*/

//Preprocessor
#ifndef NULL
    #define NULL ((void*) 0)
#endif /*NULL*/

#define FALSE 0
#define TRUE 1
#define POWER 5

#define FIFO 0
#define LRU 1
#define CLOCK 2
#define PLUS 1
#define MINUS 0

#define MAIN_MEMORY 512	
    
//structs
typedef int algorithms;
typedef int bool;

struct timeval tvbegin;

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
algorithms global_page_alg;
bool global_page_flag;
t_program *global_programs;
int global_page_count;
int global_program_count;
int global_pageID;

/*
int std_param_check(char*, int);
void read_file(char*);
int get_instance_count(char*);
void run_programs();
void demand_fifo_replacement();
void demand_clock_replacement();
void demand_lru_replacement();
void prepaging_fifo_replacement();
void prepaging_clock_replacement();
void prepaging_lru_replacement();
*/
