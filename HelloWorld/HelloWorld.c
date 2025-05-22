#include <stdio.h>
#include <ctype.h>
#include <string.h>

// 定义token类型
typedef enum {
    TOKEN_IDENTIFIER,  // 标识符
    TOKEN_NUMBER,      // 数字
    TOKEN_OPERATOR,    // 运算符
    TOKEN_STRING,      // 字符串
    TOKEN_END,         // 输入结束
    TOKEN_ERROR        // 错误
} TokenType;

// 定义token结构体
typedef struct {
    TokenType type;
    char value[256];
} Token;

// 状态枚举
typedef enum {
    STATE_START,       // 开始状态
    STATE_IN_ID,       // 识别标识符
    STATE_IN_NUM,      // 识别数字
    STATE_IN_OP,       // 识别运算符
    STATE_IN_STRING,   // 识别字符串
    STATE_DONE         // 完成状态
} State;

// 获取下一个token的主函数
Token getNextToken(const char** input) {
    Token token = {TOKEN_END, ""};
    State state = STATE_START;
    char* pos = token.value;
    char c;
    
    // 跳过前导空白
    while (isspace(**input)) (*input)++;
    
    if (**input == '\0') {
        return token;  // 输入结束
    }

    while (1) {
        c = **input;
        
        switch (state) {
            case STATE_START:
                if (isalpha(c) || c == '_') {
                    state = STATE_IN_ID;
                    *pos++ = c;
                }
                else if (isdigit(c)) {
                    state = STATE_IN_NUM;
                    *pos++ = c;
                }
                else if (strchr("+-*/=<>!", c)) {
                    state = STATE_IN_OP;
                    *pos++ = c;
                }
                else if (c == '"') {
                    state = STATE_IN_STRING;
                }
                else if (c == '\0') {
                    state = STATE_DONE;
                }
                else {
                    token.type = TOKEN_ERROR;
                    *pos++ = c;
                    state = STATE_DONE;
                }
                break;

            case STATE_IN_ID:
                if (isalnum(c) || c == '_') {
                    *pos++ = c;
                }
                else {
                    token.type = TOKEN_IDENTIFIER;
                    state = STATE_DONE;
                }
                break;

            case STATE_IN_NUM:
                if (isdigit(c)) {
                    *pos++ = c;
                }
                else {
                    token.type = TOKEN_NUMBER;
                    state = STATE_DONE;
                }
                break;

            case STATE_IN_OP:
                token.type = TOKEN_OPERATOR;
                state = STATE_DONE;
                break;

            case STATE_IN_STRING:
                if (c == '"') {
                    token.type = TOKEN_STRING;
                    state = STATE_DONE;
                    (*input)++;
                }
                else if (c == '\0') {
                    token.type = TOKEN_ERROR;
                    state = STATE_DONE;
                }
                else {
                    *pos++ = c;
                }
                break;

            case STATE_DONE:
                *pos = '\0';
                return token;
        }

        if (state != STATE_DONE) {
            (*input)++;
        }
        
        if (c == '\0') {
            *pos = '\0';
            if (state == STATE_IN_ID) token.type = TOKEN_IDENTIFIER;
            if (state == STATE_IN_NUM) token.type = TOKEN_NUMBER;
            return token;
        }
    }
}

int main() {
    const char* input = "int x = 123 + \"hello world\"";
    const char* ptr = input;
    Token token;

    printf("Input: %s\n", input);
    printf("Tokens:\n");

    do {
        token = getNextToken(&ptr);
        
        switch (token.type) {
            case TOKEN_IDENTIFIER:
                printf("IDENTIFIER: %s\n", token.value);
                break;
            case TOKEN_NUMBER:
                printf("NUMBER: %s\n", token.value);
                break;
            case TOKEN_OPERATOR:
                printf("OPERATOR: %s\n", token.value);
                break;
            case TOKEN_STRING:
                printf("STRING: %s\n", token.value);
                break;
            case TOKEN_END:
                printf("END OF INPUT\n");
                break;
            case TOKEN_ERROR:
                printf("ERROR: %s\n", token.value);
                break;
        }
    } while (token.type != TOKEN_END && token.type != TOKEN_ERROR);

    return 0;
}