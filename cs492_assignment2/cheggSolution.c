// Program to compare the efficiency of FIFO, LRU, LFU, Optimal algorithm

#include<stdio.h>
int* lru(int n, int *p, int f) {
    int q[f],pgfaultcnt=0,c1,d,i,j,k=0,r,t,b[f],c2[f];
    static int PFcount[] = {0,0,0,0,0};
    q[k]=p[k];
    pgfaultcnt++;
    k++;
    for(i=1;i<n;i++) {
        if(i%2000 == 0) {
            PFcount[((i/2000)-1)] = pgfaultcnt;
            printf("\nLRU : %d = %d", i, pgfaultcnt);
        }
    
        c1=0;
        for(j=0;j<f;j++) {
            if(p[i]!=q[j])
                c1++;
        }
        if(c1==f) {
            pgfaultcnt++;
            if(k<f) {
                q[k]=p[i];
                k++;
            } 
            else {
                for(r=0;r<f;r++){
                    c2[r]=0;
                    for(j=i-1;j<n;j--){
                        if(q[r]!=p[j])
                            c2[r]++;
                        else
                            break;
                    }
                }
                for(r=0;r<f;r++)
                    b[r]=c2[r];
                for(r=0;r<f;r++) {
                    for(j=r;j<f;j++){
                        if(b[r]<b[j]) {
                            t=b[r];
                            b[r]=b[j];
                            b[j]=t;
                        }
                    }
                }
                for(r=0;r<f;r++) {
                    if(c2[r]==b[0])
                        q[r]=p[i];
                }
            }
        }
    }
    PFcount[4] = pgfaultcnt;
    printf("\nLRU : Total no of page faults %d",pgfaultcnt);
    return PFcount;
}

int* fifo(int n, int *a, int no) {
    int i,j,frame[no],k,avail,pgfaultcnt=0;
    static int PFcount[] = {0,0,0,0,0};
    for(i=0;i<no;i++)
        frame[i]= -1;
    j=0;
    for(i=1;i<=n;i++) {
  
        if(i%2000 == 0) {
            PFcount[((i/2000)-1)] = pgfaultcnt;
            printf("\nFIFO : %d = %d", i, pgfaultcnt);
        }
    
        avail=0;
        for(k=0;k<no;k++)
            if(frame[k]==a[i])
                avail=1;
        if (avail==0){
            frame[j]=a[i];
            j=(j+1)%no;
            pgfaultcnt++;
        }
    }
    printf("\nFIFO : Total no of page faults %d",pgfaultcnt);
    return PFcount;
}

int isHit(int data, int nf, int *p) {
    int hit=0,j;
    for( j=0; j<nf; j++) {
        if(p[j]==data) {
            hit=1;
            break;
        }
    }
    return hit;
}

int getHitIndex(int data, int nf, int *p) {
    int hitind;
    int k;
    for( k=0; k<nf; k++) {
        if(p[k]==data) {
            hitind=k;
            break;
        }
    }
    return hitind;
}

int* optimal(int n, int *in, int nf) {
    int p[nf];
    int hit=0, i,j,k, pgfaultcnt=0;
    static int PFcount[] = {0,0,0,0,0};
    int near[nf];
    
    for(i=0; i<nf; i++)
        p[i]=9999;

        for(i=0; i<n; i++) {
            if(i%2000 == 0 && i > 0) {
                PFcount[((i/2000)-1)] = pgfaultcnt;
                printf("\nOPTM : %d = %d", i, pgfaultcnt);
            }
            
            if(isHit(in[i], nf, p)==0) {
                for(j=0; j<nf; j++) {
                    int pg=p[j];
                    int found=0;
                    for(k=i; k<n; k++) {
                        if(pg==in[k]) {
                            near[j]=k;
                            found=1;
                            break;
                        }
                        else
                            found=0;
                    }
                    if(!found)
                        near[j]=9999;
                }
                int max=-9999;
                int repindex;
                for(j=0; j<nf; j++) {
                    if(near[j]>max) {
                        max=near[j];
                        repindex=j;
                    }
                }
                p[repindex]=in[i];
                pgfaultcnt++;
            }
        }
    PFcount[4] = pgfaultcnt;
    printf("\nOPTM : Total no of page faults:%d",pgfaultcnt);
    return PFcount;
}

int* lfu(int n, int *in, int nf) {
    int p[nf];
    int hit=0,i,j,k;
    int pgfaultcnt = 0;
    static int PFcount[] = {0,0,0,0,0};
    int usedcnt[nf];
    int least,repin,sofarcnt=0,bn;
   
    pgfaultcnt=0;
    for(i=0; i<nf; i++)
        p[i]=9999;
   
    for(i=0; i<nf; i++)
        usedcnt[i]=0;

    for(i=0; i<n; i++) {
  
        if(i%2000 == 0 && i > 0) {
            PFcount[((i/2000)-1)] = pgfaultcnt;
            printf("\nLFU : %d = %d", i, pgfaultcnt);
        }
    
        if(isHit(in[i], nf, p)) {
            int hitind=getHitIndex(in[i],nf, p);
            usedcnt[hitind]++;
        } 
        else {
            pgfaultcnt++;
            if(bn<nf) {
                p[bn]=in[i];
                usedcnt[bn]=usedcnt[bn]+1;
                bn++;
            } 
            else {
                least=9999;
                for(k=0; k<nf; k++)
                    if(usedcnt[k]<least) {
                        least=usedcnt[k];
                        repin=k;
                    }
                p[repin]=in[i];
                sofarcnt=0;
                for(k=0; k<=i; k++)
                    if(in[i]==in[k])
                        sofarcnt=sofarcnt+1;
                usedcnt[repin]=sofarcnt;
            }
        }
    }
    PFcount[4] = pgfaultcnt;
    printf("\nLFU : Total no of page faults:%d",pgfaultcnt);
    return PFcount;
}

int main() {
    FILE * fp, *in;
    in = fopen("input.txt","r+");
    fp = fopen ("output.txt", "w+");
    int n = 10000;
    int p[n];

    int f = 512;
    printf("Enter No. of Frames : ");
    scanf("%d",&f);

    if(f != 128 && f!= 256 && f!= 512 && f!= 1024) {
        printf("Invalid Frame Size\n Running The Program with Default Frame Size\n\n");
        f = 512;
    }

    // Reading 10000 pages
    int i = 0;
    while(!feof(in)) {
        fscanf(in,"%d",&p[i]);
        i++;
    }
    fclose(in);

    // Running Algorithms
    int *pf_lru = lru(n,p,f); // get the number of page faults for LRU
    int *pf_fifo = fifo(n,p,f); // get the number of page faults for FIFO
    int *pf_lfu = lfu(n,p,f); // get the number of page faults for LFU
    int *pf_oa = optimal(n,p,f); // get the number of page faults for Optimal

    printf("\nCompleted : 10000 Pages");

    // Write it in the file output.txt
    fprintf(fp,"\n==============================================================================\n");
    fprintf(fp,"PAGE REPLACEMENT ALGORITHMS(FRAME_SIZE = %d)\n",f);
    fprintf(fp,"==============================================================================\n");
    fprintf(fp,"Algorithm\tTotal page faults\t2000\t4000\t6000\t8000\t10000\n");
    fprintf(fp,"\n------------------------------------------------------------------------------\n");
    fprintf(fp,"\nFIFO\t\t%4d\t\t%4.4f\t\t%4.4f\t\t%4.4f\t\t%4.4f\t\t%4.4f",pf_fifo[4],(pf_fifo[0]/(2000.0)),(pf_fifo[1]/(4000.0)),(pf_fifo[2]/(6000.0)),(pf_fifo[3]/(8000.0)),(pf_fifo[4]/(10000.0)));
    fprintf(fp,"\nLRU\t\t%4d\t\t%4.4f\t\t%4.4f\t\t%4.4f\t\t%4.4f\t\t%4.4f",pf_lru[4],(pf_lru[0]/(2000.0)),(pf_lru[1]/(4000.0)),(pf_lru[2]/(6000.0)),(pf_lru[3]/(8000.0)),(pf_lru[4]/(10000.0)));
        fprintf(fp,"\nLFU\t\t%4d\t\t%4.4f\t\t%4.4f\t\t%4.4f\t\t%4.4f\t\t%4.4f",pf_lfu[4],(pf_lfu[0]/(2000.0)),(pf_lfu[1]/(4000.0)),(pf_lfu[2]/(6000.0)),(pf_lfu[3]/(8000.0)),(pf_lfu[4]/(10000.0)));
        fprintf(fp,"\nOptimal\t\t%4d\t\t%4.4f\t\t%4.4f\t\t%4.4f\t\t%4.4f\t\t%4.4f",pf_oa[4],(pf_oa[0]/(2000.0)),(pf_oa[1]/(4000.0)),(pf_oa[2]/(6000.0)),(pf_oa[3]/(8000.0)),(pf_oa[4]/(10000.0)));

    // write the output on the console
        /*printf("\n==============================================================================\n");
    printf("PAGE REPLACEMENT ALGORITHMS(FRAME_SIZE = %d)\n",f);
    printf("==============================================================================\n");
    printf("Algorithm\tTotal page faults\t%d\t4000\t6000\t8000\t10000 \n",n);
    printf("\n------------------------------------------------------------------------------\n");
    printf("\nFIFO\t\t%4d\t\t%.4lf",pf_fifo,(pf_fifo/(n*1.0)));
    printf("\nLRU\t\t%4d\t\t%.4lf",pf_lru,(pf_lru/(n*1.0)));
        printf("\nLFU\t\t%4d\t\t%4.4f",pf_lfu,(pf_lfu/(n*1.0)));
        printf("\nOptimal\t\t%4d\t\t%4.4f",pf_oa,(pf_oa/(n*1.0)));*/

    printf("\nCompleted : EVERYTHING");

    fclose(fp);
    return 0;
}