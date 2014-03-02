/*
 * File: multi-lookup.c
 * Author: Justin Huffman
 * Project: CSCI 3753 Programming Assignment 2
 * Create Date: 2014/02/27
 * Modify Date: 2014/03/01
 * Description:
 * 	This file contains the implementation of the multi-lookup program.
 * 	This program accepts a list of input files wich each contain a list
 * 	of hostnames.  Multi-lookup then puts each of these hostnames on to
 * 	a que, and then resolves each hostname in this que into an ipv4 ip
 * 	address, with the hostname and address being stored in ouptup.txt in
 * 	the format "hostname,address".
 */

#include "multi-lookup.h"

queue hostqueue;
pthread_mutex_t filelock = PTHREAD_MUTEX_INITIALIZER;
int requesting = 1;

int main(int argc, char *argv[]) {
	
	/* Local Vars */
	FILE* outputfp;
    pthread_t requesterThreads[THREADCOUNT];
    pthread_t resolverThreads[THREADCOUNT];
    int return_value;
    
	/* Check Arguments */
	if(argc < MINARGS) {
		fprintf(stderr, "Not enough arguments: %d\n", (argc - 1));
		fprintf(stderr, "Usage:\n %s %s\n", argv[0], USAGE);
		return EXIT_FAILURE;
	}
 
    /* Initialize Host Queue */
    if (queue_init(&hostqueue, THREADCOUNT) == QUEUE_FAILURE){
        fprintf(stderr, "Hostqueue failed to initialize \n");
    }
    
    /* Open Output File */
    outputfp = fopen(argv[argc-1], "w");
    if(outputfp == 0){
        fprintf(stderr, "Error Opening Output File");
        return EXIT_FAILURE;
    }

    /* Start Requester Threads */
    int i;
    for(i=0;i<THREADCOUNT;i++){
        return_value = pthread_create(&(requesterThreads[i]), NULL, request, argv[i+1]); 
        if (return_value){
            fprintf(stderr, "ERROR: return value from pthread_create() is %d\n", return_value);
            exit(EXIT_FAILURE);
        }
    }

    /* Start Resolver Threads */

	return EXIT_SUCCESS;
}

void* request(void* filename){
    char hostname[SBUFFSIZE];
    char* temp;
    FILE* inputfp = fopen(filename, "r");
    while(fscanf(inputfp, INPUTFS, (char*) filename) > 0)
    {
        temp = malloc(SBUFFSIZE);
        strncpy(temp, hostname, SBUFFSIZE);
        printf("HOSTNAME: %s\n", hostname);
        queue_push(&hostqueue, temp);
    }
    fclose(inputfp);
    return EXIT_SUCCESS;
}

void* resolve(void* outputfp){
    char* hostname;
    char firstipstr[INET6_ADDRSTRLEN];
    while(!queue_is_empty(&hostqueue) || requesting)
    {
        hostname = queue_pop(&hostqueue);
        if(hostname == NULL)
            return EXIT_SUCCESS;
        if(dnslookup(hostname, firstipstr, sizeof(firstipstr)) == UTIL_FAILURE){
            fprintf(stderr, "dnslookup error: %s\n", hostname);
            free(hostname);
        }

        /* Safely write output to file */
        pthread_mutex_lock(&filelock);
        fprintf(outputfp, "%s,%s\n", hostname, firstipstr);
        pthread_mutex_unlock(&filelock);

        free(hostname);
    }
    return EXIT_SUCCESS;
}