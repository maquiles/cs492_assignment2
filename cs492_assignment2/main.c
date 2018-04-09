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

//Libraries 
#include <sys/time.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include "../lib/my.h"
#include "FIFO.h"
//#include "../lib/queue_r.h"
//#include "../lib/queue_p.h"
    
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
    //queue_p_t* p;	
    //queue_c_t* c;	
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
int std_input_check(int, char**);
int std_input_error(int);
int std_param_check(char*, int);
void read_file(char*);
int get_instance_count(char*);
void init_programs();
void init_pages(int);
void run_programs();
void demand_fifo_replacement();
void demand_clock_replacement();
void demand_lru_replacement();
void prepaging_fifo_replacement();
void prepaging_clock_replacement();
void prepaging_lru_replacement();
*/

//function to count the number of programs in a page
int get_program_count(char* argc){
    int count = 0;
    char ch;
    FILE *fd;
    fd = fopen(argc, "r");
    if(fd == NULL){
        printf("ERROR: file could not be opened");
    }
    while ((ch = fget(fd)) != EOF){
        if(ch == "\n"){
            count++;
        }
    }

    fclose(fd);
    return count;
}

void init_programs(){
    FILE *fd;
    global_page_count = MAIN_MEMORY / global_page_size;
    global_program_count = get_program_count(global_plist);
    global_programs = (t_program*)xmalloc(sizeof(t_program) * global_program_count);

    fd = fopen(global_plist, "r");
    if(fd == NULL){
        printf("ERROR: file could not be opened");
    }

    for (int i = 0; i < global_program_count; i++){
        if(fscan(fd, "%i %i", &global_programs[i].pid, &global_programs.total_memorylocation) != 2){
            printf("WARNING: empty line in plist");
        }
        else{
            global_programs[i].init_memorylocation = global_page_count / (global_program_count -1);
            global_programs[i].total_pages = global_programs[i].total_memorylocation / global_page_size;

            if(global_programs[i].total_pages * global_page_size < global_programs.total_memorylocation){
                global_programs[i].total_pages++;
            }

            global_page_count -= global_programs[i].init_memorylocation;
            global_programs[i].pagetable = (t_page*)xmalloc(sizeof(t_page) * global_programs[i].total_pages);

            switch (global_page_alg){
                case FIFO:
                    global_programs[i].fifo = (fifo_queue*)xmalloc(sizeof(fifo_queue)+1);
                    fifo_init(global_programs[i].fifo);
                    break;
                case LRU:
                    
                case CLOCK:
                    
            }
            //intit_pages();
         }
    }
    fclose(fd);
}

int main(int argc, char *argv[]){
    int i;

    if(argc != 6){
        printf("ERROR: not enough parameters");
        return 1;
    }

    global_pageID = 1;

    //initialize programs
    //init_programs();

}