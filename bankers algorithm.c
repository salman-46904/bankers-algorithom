#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <stdbool.h>
#include <time.h>

int n_resrcs,
    n_prcss;
int *res;
int **res_alloc;
int **max_res_req;
int **recs_needed;
int *Sequence_of_exec;
int number_process_ran = 0;

pthread_mutex_t recrs_lk;
pthread_cond_t cnds.

bool getSequence_of_exec().

void* prcsCd(void* )

int main(int argc1, char** argv1) {
	srand(time(NULL));

        printf("\nEnter the Number of processes ? ");
        scanf("%d", &n_prcss);

        printf("\n Enter Number of resources? ");
        scanf("%d", &n_resrcs);

        res = (int *)malloc(n_resrcs * sizeof(*res));
        printf("\nEnter Currently Available resources to (R1 R2 ...)? ");
        for(int a=0; a<n_resrcs; a++)
                scanf("%d", &res[a]);

        res_alloc = (int **)malloc(n_prcss * sizeof(*res_alloc));
        for(int a=0; a<n_prcss; a++)
                res_alloc[a] = (int *)malloc(n_resrcs * sizeof(**res_alloc));

        max_res_req = (int **)malloc(n_prcss * sizeof(*max_res_req));
        for(int a=0; a<n_prcss; a++)
                max_res_req[a] = (int *)malloc(n_resrcs * sizeof(**max_res_req));

       
        printf("\n");
        for(int a=0; a<n_prcss; a++) {
                printf("\nEnter Resource allocated to process %d (R1 R2 ...)? ", a+1);
                for(int s=0; s<n_resrcs; s++)
                        scanf("%d", &res_alloc[a][s]);
        }
        printf("\n");

	
        for(int a=0; a<n_prcss; a++) {
                printf("\nEnter Maximum resource required by process %d (R1 R2 ...)? ", a+1);
                for(int s=0; s<n_resrcs; s++)
                        scanf("%d", &max_res_req[a][s]);
        }
        printf("\n");

	
        recs_needed = (int **)malloc(n_prcss * sizeof(*recs_needed));
        for(int a=0; a<n_prcss; a++)
                recs_needed[a] = (int *)malloc(n_resrcs * sizeof(**recs_needed));

        for(int a=0; a<n_prcss; a++)
                for(int s=0; s<n_resrcs; s++)
                        recs_needed[a][s] = max_res_req[a][s] - res_alloc[a][s];


	Sequence_of_exec = (int *)malloc(n_prcss * sizeof(*Sequence_of_exec));
        for(int a=0; a<n_prcss; a++) Sequence_of_exec[a] = -1;

        if(!getSequence_of_exec()) {
                printf("\nUnsafe State! The processes leads the system to a unsafe state.\n\n");
                exit(-1);
        }

        printf("\n\nProcess founded under Safe Sequence : ");
        for(int a=0; a<n_prcss; a++) {
                printf("%-3d", Sequence_of_exec[a]+1);
        }

        printf("\nExecuting the Processes...\n\n");
        sleep(1);
	
	
	pthread_t processes[n_prcss];
        pthread_attr_t attr;
        pthread_attr_init(&attr);

	int prcs_num[n_prcss];
	for(int a=0; a<n_prcss; a++) prcs_num[a] = a;

        for(int a=0; a<n_prcss; a++)
                pthread_create(&processes[a], &attr, prcsCd, (void *)(&prcs_num[a]));

        for(int a=0; a<n_prcss; a++)
                pthread_join(processes[a], NULL);

        printf("\nAll Processes Finished Succesfully\n");	
	
	
        free(res);
        for(int a=0; a<n_prcss; a++) {
                free(res_alloc[a]);
                free(max_res_req[a]);
		free(recs_needed[a]);
        }
        free(res_alloc);
        free(max_res_req);
	free(recs_needed);
        free(Sequence_of_exec);
}


bool getSequence_of_exec() {
	
        int tempRes[n_resrcs];
        for(int a=0; a<n_resrcs; a++) tempRes[a] = res[a];

        bool finished[n_prcss];
        for(int a=0; a<n_prcss; a++) finished[a] = false;
        int nfinished=0;
        while(nfinished < n_prcss) {
                bool safe = false;

                for(int a=0; a<n_prcss; a++) {
                        if(!finished[a]) {
                                bool possible = true;

                                for(int s=0; s<n_resrcs; s++)
                                        if(recs_needed[a][s] > tempRes[s]) {
                                                possible = false;
                                                break;
                                        }

                                if(possible) {
                                        for(int s=0; s<n_resrcs; s++)
                                                tempRes[s] += res_alloc[a][s];
                                        Sequence_of_exec[nfinished] = a;
                                        finished[a] = true;
                                        ++nfinished;
                                        safe = true;
                                }
                        }
                }

                if(!safe) {
                        for(int d=0; d<n_prcss; d++) Sequence_of_exec[d] = -1;
                        return false; // no safe sequence found
                }
        }
        return true
}


void* prcsCd(void *ss) {
        int so1 = *((int *) ss);

	
        pthread_mutex_lock(&recrs_lk);

        
        while(so1 != Sequence_of_exec[number_process_ran])
                pthread_cond_wait(&cnds, &recrs_lk);

	
        printf("\n--> Process %d", so1+1);
        printf("\n\tResources Allocated : ");
        for(int a=0; a<n_resrcs; a++)
                printf("%3d", res_alloc[so1][a]);

        printf("\n\t Resources Needed    : ");
        for(int a=0; a<n_resrcs; a++)
                printf("%3d", recs_needed[so1][a]);

        printf("\n\tResources Available : ");
        for(int a=0; a<n_resrcs; a++)
                printf("%3d", res[a]);

        printf("\n"); sleep(1);

        printf("\tResource Allocated!");
        printf("\n"); sleep(1);
        printf("\tProcess Code Running...");
        printf("\n"); sleep(rand()%3 + 2); 
        printf("\tProcess Code Completed...");
        printf("\n"); sleep(1);
        printf("\tProcess Releasing Resource...");
        printf("\n"); sleep(1);
        printf("\tResource has been Released!");

	for(int a=0; a<n_resrcs; a++)
                res[a] += res_alloc[so1][a];

        printf("\n\tResources available Now: ");
        for(int a=0; a<n_resrcs; a++)
                printf("%3d", res[a]);
        printf("\n\n");

        sleep(1);

	
        number_process_ran++;
        pthread_cond_broadcast(&cnds);
        pthread_mutex_unlock(&recrs_lk);
	pthread_exit(NULL);
}
