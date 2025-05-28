#include <stdio.h>

#include "../src/nfa.h"
#include "../src/nfa.c"

#include "../src/arena.h"
#include "../src/arena.c"

#define MAX_STATES 1024

int get_state_id(State* state, State** id_map, int* next_id) {
    for (int i = 0; i < *next_id; ++i) {
        if (id_map[i] == state) return i;
    }
    if (*next_id >= MAX_STATES) return -1;
    int id = *next_id;
    id_map[id] = state;
    (*next_id)++;
    return id;
}


// 递归打印NFA状态及其转移
void print_state(State* state, int* visited, State** id_map, int* next_id) {
    // if(depth > 100){
    //     printf("max depth reached \n");
    //     return;
    // }

    int id = get_state_id(state, id_map, next_id);
    if (id == -1 || visited[id]) return;
    visited[id] = 1;

    printf("State[%d] %s\n", id, state->is_accepting ? "(accepting)" : "");

    for (Transition* t = state->transitions; t != NULL; t = t->next) {
        int to_id = get_state_id(t->target, id_map, next_id);
        const char* symbol_str = (t->symbol == '\0') ? "\\0" : (char[]){t->symbol, '\0'};
        printf("  --[%s]--> State[%d]\n", symbol_str, to_id);
    }

    for (Transition* t = state->transitions; t != NULL; t = t->next) {
        // printf("Entering State[%d]\n", id);
        print_state(t->target, visited, id_map, next_id);
    }
}

// 简单封装打印NFA结构
void test_print_nfa(NFA nfa, const char* desc) {
    printf("NFA for %s:\n", desc);
    int visited[MAX_STATES] = {0};
    State* id_map[MAX_STATES] = {0};
    int next_id = 0;
    print_state(nfa.start, visited, id_map, &next_id);
    printf("\n");
}

int main() {
    struct Arena* arena = arena_create(2048);
    if (!arena) {
        fprintf(stderr, "Failed to create arena\n");
        return 1;
    }

    // 1. a
    test_print_nfa(create_char_nfa(arena, 'a'), "\"a\"");

    // 2. a*
    test_print_nfa(create_star_nfa(arena, create_char_nfa(arena, 'a')), "\"a*\"");

    // 3. ab (concat)
    {
        NFA a = create_char_nfa(arena, 'a');
        NFA b = create_char_nfa(arena, 'b');
        NFA ab = create_concat_nfa(arena, a, b);
        test_print_nfa(ab, "\"ab\"");
    }

    // 4. a|b (or)
    {
        NFA a = create_char_nfa(arena, 'a');
        NFA b = create_char_nfa(arena, 'b');
        NFA a_or_b = create_or_nfa(arena, a, b);
        test_print_nfa(a_or_b, "\"a|b\"");
    }

    // 5. a+ (one or more)
    test_print_nfa(create_plus_nfa(arena, create_char_nfa(arena, 'a')), "\"a+\"");

    // 6. a? (zero or one)
    test_print_nfa(create_question_nfa(arena, create_char_nfa(arena, 'a')), "\"a?\"");

    // 7. (ab)* (star with grouping)
    {
        NFA a = create_char_nfa(arena, 'a');
        NFA b = create_char_nfa(arena, 'b');
        NFA ab = create_concat_nfa(arena, a, b);
        NFA ab_star = create_star_nfa(arena, ab);
        test_print_nfa(ab_star, "\"(ab)*\"");
    }

    // 8. dot (any character)
    test_print_nfa(create_dot_nfa(arena), "\".\"");

    arena_free(arena);
    return 0;
}
