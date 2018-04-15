/*
** Matthew Aquiles, Thomas Falsone
** 4/13/2018
** 
*/

//Libraries 
#include <sys/time.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include "FIFO.h"
#include "LRU.h"
#include "CLOCK.h"
#include "assignment.h"
#include <math.h>

//function to count the number of programs in a page
int get_program_count(char* argc){
    int count = 0;
    char ch;
    FILE *fd;
    fd = fopen(argc, "r");
    if(fd == NULL){
        printf("ERROR: file could not be opened");
    }

    while ((ch = fgetc(fd)) != EOF){
        if(ch == '\n'){
            count++;
        }
    }

    fclose(fd);
    return count;
}

void init_pages(int x){
    int y;

    for (int i = 0; i < global_programs[x].total_pages; i++){
        global_programs[x].pagetable[i].pagenumber = global_pageID++;
        global_programs[x].pagetable[i].validbid = FALSE;
    }

    if(global_programs[x].init_memorylocation > global_programs[x].total_pages){
        y = global_programs[x].total_pages;
    }
    else{
        y = global_programs[x].init_memorylocation;
    }

    for (int j = 0; j < y; j++){
        global_programs[x].pagetable[j].validbid = TRUE;
        switch (global_page_alg){
        case FIFO:
            fifo_push(global_programs[x].fifo, &global_programs[x].pagetable[j]);
            break;
        case LRU:
            lru_push(global_programs[x].lru, &global_programs[x].pagetable[j], 1);
            break;
        case CLOCK:
            clock_push(global_programs[x].clk, &global_programs[x].pagetable[j]);
            break;
        }
    }
}

void init_programs(){
    FILE *fd;
    global_page_count = ceil(MAIN_MEMORY / global_page_size);
    global_program_count = get_program_count(global_plist);
    global_programs = (t_program*)malloc(sizeof(t_program) * global_program_count);
    
    fd = fopen(global_plist, "r");
    if(fd == NULL){
        printf("ERROR: file could not be opened");
    }

    for (int i = 0; i < global_program_count; i++){
        if(fscanf(fd, "%i %i", &global_programs[i].pid, &global_programs[i].total_memorylocation) != 2){ // error with &global_programs, should it have [i]?
            printf("WARNING: empty line in plist\n");                                                  // might also have to move total_memorylocation out of the global_programs struct
        }
        else{
            global_programs[i].init_memorylocation = global_page_count / (global_program_count -1);
            global_programs[i].total_pages = global_programs[i].total_memorylocation / global_page_size;

            if(global_programs[i].total_pages * global_page_size < global_programs[i].total_memorylocation){
                global_programs[i].total_pages++;
            }

            global_page_count -= global_programs[i].init_memorylocation;
            global_programs[i].pagetable = (t_page*)malloc(sizeof(t_page) * global_programs[i].total_pages);

            switch (global_page_alg){
                case FIFO:
                    global_programs[i].fifo = (fifo_queue*)malloc(sizeof(fifo_queue)+1);
                    fifo_init(global_programs[i].fifo);
                    break;
                case LRU:
                    global_programs[i].lru = (lru_queue*)malloc(sizeof(lru_queue)+1);
                    lru_init(global_programs[i].lru);
                    break;
                case CLOCK:
                    global_programs[i].clk = (clock_queue*)malloc(sizeof(clock_queue)+1);
                    clock_init(global_programs[i].clk);
                    break;
            }
            init_pages(i);
         }
    }
    fclose(fd);
}

void run_programs(){
    switch (global_page_alg){
    case FIFO:
        switch (global_page_flag){
        case PLUS:
            prepaging_fifo();
            break;
        case MINUS:
            demand_fifo();
            break;
        }
        break;
    case LRU:
        switch (global_page_flag){
        case PLUS:
            prepaging_lru();
            break;
        case MINUS:
            demand_lru();
            break;
        }
        break;
    case CLOCK:
        switch (global_page_flag){
        case PLUS:
            prepaging_clock();
            break;
        case MINUS:
            demand_clock();
            break;
        }
        break;
    }
}

int main(int argc, char *argv[]){
    if(argc != 6){
        printf("ERROR: not enough parameters");
        return 1;
    }

    global_pageID = 1;

    global_page_size = atoi(argv[3]);

    if (strcmp(argv[4], "FIFO") == 0) {
        global_page_alg = FIFO;
    } else if (strcmp(argv[4], "LRU") == 0) {
        global_page_alg = LRU;
    } else if (strcmp(argv[4], "CLOCK") == 0) {
        global_page_alg = CLOCK;
    } else {
        printf("Improper page algorithm indicated\n");
        return 1;
    }

    if (strcmp(argv[4], "+") == 0) {
        global_page_flag = PLUS;
    } else if (strcmp(argv[4], "-") == 0) {
        global_page_flag = MINUS;
    } else {
        printf("Improper flag indicated\n");
        return 1;
    }

    char* ch = argv[1];
    global_plist = (char*)malloc(sizeof(char)*(strlen(ch)+2));
    global_plist[0] = '.';
    global_plist[1] = '/';
    strcpy(global_plist+2, ch);

    ch = argv[2];
    global_ptrace = (char*)malloc(sizeof(char)*(strlen(ch)+7));
    global_ptrace[0] = '.';
    global_ptrace[1] = '/';
    strcpy(global_ptrace+2, ch);

    //initialize programs
    init_programs();

    for (int i = 0; i < global_program_count; i++){
        printf("pid: %i \t initML: %i \t totalML: %i \t totalPages: %i \t P#: %i \t Flag: %i \n",\
            global_programs[i].pid,\
            global_programs[i].init_memorylocation,\
            global_programs[i].total_memorylocation,\
            global_programs[i].total_pages,\
            global_programs[i].pagetable[global_programs[0].init_memorylocation].pagenumber,\
            global_programs[i].pagetable[global_programs[0].init_memorylocation].validbid);
    }
    run_programs();
}