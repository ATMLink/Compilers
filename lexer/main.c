#define UNITY_BUILD

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "./src/arena.h"
#include "./src/arena.c"
#include "./src/nfa.h"
#include "./src/nfa.c"
#include "./src/lexer.h"
#include "./src/lexer.c"
#include "./src/matcher.h"
#include "./src/matcher.c"
#include "./src/parser.h"
#include "./src/parser.c"
#include "./src/dfa.h"
#include "./src/dfa.c"

typedef struct {
    const char* regex;
    const char* input;
    int expected; // 1 for match, 0 for no match
} TestCase;

void run_nfa_test_case(TestCase test, int index, struct Arena* arena) {
    State* start = parse_regex(test.regex, arena);
    if (!start) {
        printf("[Test %02d] Regex: \"%-10s\"  Input: \"%-10s\"  → PARSE ERROR ❌\n",
               index, test.regex, test.input);
        return;
    }

    int result = simulate_nfa(start, test.input, arena);
    const char* verdict = result == test.expected ? "✅" : "❌";

    printf("[Test %02d] Regex: %-10s  Input: %-10s  → Match: %-3s  Expected: %-3s  %s\n",
           index, test.regex, test.input,
           result ? "YES" : "NO",
           test.expected ? "YES" : "NO",
           verdict);
}

void run_dfa_test_case(TestCase test, int index, struct Arena* arena) {
    State* nfa_start = parse_regex(test.regex, arena);
    if (!nfa_start) {
        printf("[Test %02d] Regex: \"%-10s\"  Input: \"%-10s\"  → PARSE ERROR ❌\n",
               index, test.regex, test.input);
        return;
    }

    NFA nfa = {nfa_start, nfa_start};
    while (nfa.accept && !nfa.accept->is_accepting && nfa.accept->transitions) {
        nfa.accept = nfa.accept->transitions->target;
    }

    DFA* dfa = create_dfa_from_nfa(arena, nfa);
    if (!dfa) {
        printf("[Test %02d] Regex: \"%-10s\"  Input: \"%-10s\"  → DFA CREATION ERROR ❌\n",
               index, test.regex, test.input);
        return;
    }

    int result = simulate_dfa(dfa, test.input);
    const char* verdict = result == test.expected ? "✅" : "❌";

    printf("[Test %02d] Regex: %-10s  Input: %-10s  → Match: %-3s  Expected: %-3s  %s\n",
           index, test.regex, test.input,
           result ? "YES" : "NO",
           test.expected ? "YES" : "NO",
           verdict);
}

int main(int argc, char* argv[]) {
    TestCase tests[] = {
        {"a",      "a",      1},
        {"a",      "b",      0},
        {"ab",     "ab",     1},
        {"ab*",    "a",      1},
        {"ab*",    "ab",     1},
        {"ab*",    "abb",    1},
        {"ab*",    "ac",     0},
        {"a*b",    "aaab",   1},
        {"a*b",    "b",      1},
        {"a+b",    "aaab",   1},
        {"a+b",    "b",      0},
        {"ab?",    "a",      1},
        {"ab?",    "ab",     1},
        {"ab?",    "abb",    0},
        {"a.b",    "acb",    1},
        {"a.b",    "ab",     0},
        {"(a|b)c", "ac",     1},
        {"(a|b)c", "bc",     1},
        {"(a|b)c", "cc",     0},
        {"a(bc)*d","ad",     1},
        {"a(bc)*d","abcd",   1},
        {"a(bc)*d","abcbcd", 1},
        {"a(bc)*d","abcbcbc",0}
    };

    int num_tests = sizeof(tests) / sizeof(TestCase);
    int run_dfa = (argc > 1 && strcmp(argv[1], "--dfa") == 0);

    printf("=== %s Regex Test Suite ===\n", run_dfa ? "DFA" : "NFA");

    for (int i = 0; i < num_tests; i++) {
        struct Arena* arena = arena_create(1024 * 10);
        if (!arena) {
            printf("[Test %02d] Regex: \"%-10s\"  Input: \"%-10s\"  → ARENA ERROR ❌\n",
                   i + 1, tests[i].regex, tests[i].input);
            continue;
        }

        if (run_dfa) {
            run_dfa_test_case(tests[i], i + 1, arena);
        } else {
            run_nfa_test_case(tests[i], i + 1, arena);
        }

        arena_free(arena);
    }

    return 0;
}