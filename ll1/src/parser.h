#ifndef PARSER_H
#define PARSER_H

#include "grammar.h"
#include "predict_table.h"

void parse_tokens(const Grammar* g, const PredictTable* pt, const char* tokens[], int token_count);

#endif
