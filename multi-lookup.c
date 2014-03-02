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

int main(int argc, char *argv[]) {
	
	/* Local Vars */
	FILE* outputfp = NULL;
    queue hostqueue;
    pthread_mutex_t* lock;
    
	/* Check Arguments */
	if(argc < MINARGS) {
		fprintf(stderr, "Not enough arguments: %d\n", (argc - 1));
		fprintf(stderr, "Usage:\n %s %s\n", argv[0], USAGE);
		return EXIT_FAILURE;
	}
 
    /* Initialize File Queue */
    queue_init(&hostqueue, THREADCOUNT);
    
    /* Open Output File and Initialize File Lock */
    outputfp = fopen(argv[argc-1], "w");
    pthread_mutex_init(lock, NULL);


	return EXIT_SUCCESS;
}
