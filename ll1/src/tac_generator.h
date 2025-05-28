// tac_generator.h
#ifndef TAC_GENERATOR_H
#define TAC_GENERATOR_H

typedef struct {
    char op[10];
    char arg1[50];
    char arg2[50];
    char result[50];
} TAC;

extern TAC tacs[];
extern int tac_count;

void gen_TAC(const char* op, const char* arg1, const char* arg2, const char* result);
void print_TAC();
char* new_temp();
void optimize_TAC();

#endif
