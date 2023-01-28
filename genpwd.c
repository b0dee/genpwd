#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include <limits.h>
#include <stdint.h>
#include "genpwd.h"

bool silent = false;
clock_t timer;

void show_help() {
	printf("genpwd.exe [-seed | -s] seed\n"
		"seed:\n"
			"\tThe seed to be used when generating password\n"
    "Warning: this program is not cryptographically secure, it uses simple methods to generate pseudorandom passwords and basic mathematics to 'randomise' based on a seed.\n"
	);
	exit(-1);
}

/* 
 Caller is responsible for freeing memory
*/
char * generate_password(char * seed) {
	char chars[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789`-=[];'#,./\\!\"$%^&*()_+{}:@~<>?|";
	char * buf = malloc(sizeof(char)*PASSLEN);
	memset(buf,'\0',sizeof(char)*PASSLEN);
	if (seed) {
		int seedlen = strlen(seed);
		for (int i = 1; i != PASSLEN; i++) { 
			int randomchar = ((((int)seed[i % seedlen] * 42) / (seedlen + i) * 3) % strlen(chars)); 
			int offset = 1;
			if (buf[i - 2] == chars[randomchar]) { 
				randomchar = ((randomchar + offset) / i) % strlen(chars);
				offset ++;
			}
			buf[i - 1] = chars[randomchar];
		}
	} else {
		for (int i = 1; i != PASSLEN; i++) { 
			int randomchar = (((PASSLEN / i % 3) * rand() * i - (rand() % i)) % strlen(chars));
			buf[i - 1] = chars[randomchar];
		}
	}
	return buf;
}

void generate_seed(char * buf, int size) {
	int j;
	memset(buf,'\0',size);
	for (int i = 0; i < size - 1; i++) {
		j = rand() % 126;
		while (j < 33)
			j+= 7;
		buf[i] = (char)j;
	}
}

void test(int count) {
	timer = clock();
	char * pass;
	char * seed = malloc(sizeof(char)*12);
	unsigned long test_count;
	if (count <= 0)
		test_count = 500000;
	else
		test_count = count;
	printf("Generating 2x %lu passwords\n", test_count);
	for (uint32_t i = 0; i < test_count; i++){
		pass = generate_password(NULL);
		if (!silent)
			printf("%s\n", pass);
		free(pass);
	}
	for (uint32_t i = 0; i < test_count; i++){
		generate_seed(seed, sizeof(char)*12);
		pass = generate_password(seed);
		if (!silent)
			printf("%s -> %s\n", seed, pass);
		free(pass);
	}
	free(seed);

	exit(0);
}

/* Not accurate time tracking, 
 * this tracks CPU time used by program, not 100% accurate to actual time taken 
*/
void showtime() {
	double time = (double)(clock() - timer) / CLOCKS_PER_SEC;
	printf("Program took %fs to complete\n", time);
}

int main(int argc, char * argv[]) {
	srand(time(NULL)); // Initialise rand() for program
	char * seed = NULL; 
	int test_count = 0;
	for (int i = 1; i < argc; i++) { // start at 1 to skip progname
		if (argv[i][0] != '-') 
			seed = argv[i]; // We only expect one input to not have a leading '-' 
    else if (strstr(argv[i], "--time")) 
			atexit(showtime);
    else if (strstr(argv[i], "--count")) 
			test_count = atoi(argv[i + 1]);
    else if (strstr(argv[i], "--test"))
			test(test_count);
    else if (strstr(argv[i], "--help"))
			test(test_count);
    else if (strstr(argv[i], "-h"))
			test(test_count);
    else if (strstr(argv[i], "--silent"))
			silent = true;
	}

	printf("Generating passwords. %s%s%s\n", seed == NULL ? "": "Using `", seed == NULL ? "": seed, seed == NULL ? "": "` as our seed");
	char * pass1 = generate_password(seed);
	printf("Password 1%s: %s\n", seed == NULL ? "":" (seeded)", pass1);
	char * pass2 = generate_password(NULL);
	printf("Password 2: %s\n", pass2);

	free(pass1);
	free(pass2);
	
	return 0;
}
