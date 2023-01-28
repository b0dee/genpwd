#ifndef _GENPWD_H
#define _GENPWD_H


#define PASSLEN 32

void show_help();
char * generate_password(char * seed);
void test();
void generate_seed(char * buf, int size);
void showtime();


#endif
