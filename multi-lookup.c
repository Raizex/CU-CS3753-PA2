/*
 * File: multi-lookup.c
 * Author: Justin Huffman
 * Project: CSCI 3753 Programming Assignment 2
 * Create Date: 2014/02/27
 * Modify Date: 2014/02/27
 * Description:
 * 	This file contains the implementation of the multi-lookup program.
 * 	This program accepts a list of input files wich each contain a list
 * 	of hostnames.  Multi-lookup then puts each of these hostnames on to
 * 	a que, and then resolves each hostname in this que into an ipv4 ip
 * 	address, with the hostname and address being stored in ouptup.txt in
 * 	the format "hostname,address".
 */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

#include "util.h"

#define MINARGS 3 
#define USAGE "<inputFilePath> <outputFilePath>"
#define SBUFSIZE 1025
#define INPUTFS "%1024s"

int main(int argc, char *argv[]) {
	
	/* Local Vars */
	FILE* inputfp = NULL;
	FILE* outputfp = NULL;
	char hostname[SBUFFSIZE];
	char errorstr[SBUFFSIZE];
	char firstipstr[INET6_ADDRSTRLEN];
	int i;

	/* Check Arguments */
	if(argc < MINARGS
		fprintf(stderr, "Not enough arguments: %d\n", (argc - 1));
		fprintf(stderr, "Usage:\n %s %s\n", argv[0], USAGE);
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
