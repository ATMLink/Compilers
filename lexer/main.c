// tmain.c
#define UNITY_BUILD // 启用 Unity Build

#include <stdio.h>
#include <stdlib.h> // For exit, EXIT_FAILURE

// --- Include ALL necessary .h and .c files ---

// Arena
#include "src/arena.h"      // Assuming path structure
#include "src/arena.c"

// NFA (Fragment Version)
#include "src/nfa.h"
#include "src/nfa.c"

// Lexer
#include "src/lexer.h"      // Make sure lexer.h defines Token struct and types
#include "src/lexer.c"

#include "src/matcher.h"      // Make sure lexer.h defines Token struct and types
#include "src/matcher.c"

#include "src/parser.h"      // Make sure lexer.h defines Token struct and types
#include "src/parser.c"

// int main() {
//     struct Arena* arena = arena_create(1024 * 10);

//     const char* regex = "ab*a";//[aba, aa, abba, ....]
//     const char* input = "abba";

//     State* start = parse_regex(regex, arena);
//     int match = simulate_nfa(start, input, arena);

//     printf("Regex: \"%s\"\nInput: \"%s\"\nMatch: %s\n",
//         regex, input, match ? "YES" : "NO");

//     arena_free(arena);
//     return 0;
// }

// 一个简单的测试用例结构
typedef struct {
    const char* regex;
    const char* input;
    int expected; // 1 for match, 0 for no match
} TestCase;

// 测试函数
void run_test_case(TestCase test, int index) {
    struct Arena* arena = arena_create(1024 * 10); // 每次新建 Arena，避免内存污染

    State* start = parse_regex(test.regex, arena);
    if (!start) {
        printf("[Test %02d] Regex: \"%s\"  Input: \"%s\"  → PARSE ERROR ❌\n",
               index, test.regex, test.input);
        arena_free(arena);
        return;
    }

    int result = simulate_nfa(start, test.input, arena);
    const char* verdict = result == test.expected ? "✅" : "❌";

    printf("[Test %02d] Regex: %-10s  Input: %-10s  → Match: %-3s  Expected: %-3s  %s\n",
           index, test.regex, test.input,
           result ? "YES" : "NO",
           test.expected ? "YES" : "NO",
           verdict);

    arena_free(arena);
}

int main() {
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
    printf("=== NFA Regex Test Suite ===\n");
    for (int i = 0; i < num_tests; i++) {
        run_test_case(tests[i], i + 1);
    }

    return 0;
}