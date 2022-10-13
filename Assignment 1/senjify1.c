#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINES 500
#define MAX_LINE_LEN 132

int indent = 0; // set indent to 0
int width = 80; // set default width to 0
int fixed_width = 0; // set fixed_width (width-indent) to 0
int new_width = 0; // set new_width (reset after printing each word)
int format = 0; // set formatting to 0 (off)
int newline = 1; // set newline (determines if at the start of line) to 1 (on)
int newlinef = 0; // set newlinef (used after printing unformatted lines) to 0 (off)
char input_line[MAX_LINE_LEN]; // array of lines

/* the code in the get_number function is credited to Michael Zastre
   and is obtained from the lab04 files in sample.c */ 
int get_number(char * st) {
    int length = strlen(st);
    for (int j = 0; j < (length - 1); j++) {
        if (isdigit(st[j])) {
            char * temp = & st[j];
            int num = strtol(temp, & temp, 10);
            return num;
        }
    }
    return 0;
}
void print_the_words(char * line) {
    char * t;
    if (strlen(line) == 0) {
        newline = 1;
        return;
    }
    if (line[strlen(line) - 1] == '\n') {
        line[strlen(line) - 1] = '\0';
    }
    if (newlinef == 1) {
        printf("\n");
    }
    newlinef = 0;
    if ((line[0] == '{') && (line[1] == '{') && (isdigit(line[3]))) {
        width = get_number(input_line);
        format = 1;
        newline = 1;
        return;
    }
    if ((line[0] == '{') && (line[1] == '{') && (line[3] == '>') && (isdigit(line[4]))) {
        indent = get_number(input_line);
        newline = 1;
        return;
    }
    if ((line[0] == '{') && (line[1] == '{') && (line[3] == 'o') && (line[4] == 'f') && (line[5] == 'f')) {
        format = 0;
        newline = 1;
        return;
    }
    if ((line[0] == '{') && (line[1] == '{') && (line[3] == 'o') && (line[4] == 'n')) {
        format = 1;
        newline = 1;
        return;
    }
    if ((line[0] == '{') && (line[1] == '{') && (line[3] == '!')) {
        newline = 1;
        if (format == 0) {
            format = 1;
        } else {
            format = 0;
        }
        return;
    }
    if (format == 1) {
        t = strtok(line, " ");
        if (t == NULL) {
            newline = 1;
            printf("\n");
            printf("\n");
        }
        while (t != NULL) {
            if (newline == 1 && indent > 0) {
                for (int k = 0; k < indent; k++) {
                    printf(" ");
                }
            }
            if (newline == 1) {
                fixed_width = 0;
                new_width = 0;
                fixed_width = width - indent;
                new_width = new_width + strlen(t);
            } else {
                new_width = new_width + strlen(t) + 1;
            }
            if (new_width > fixed_width) {
                printf("\n");
                for (int k = 0; k < indent; k++) {
                    printf(" ");
                }
                printf("%s", t);
                newline = 0;
                fixed_width = 0;
                new_width = 0;
                fixed_width = width - indent;
                new_width = new_width + strlen(t);
            } else {
                if (newline == 1) {
                    printf("%s", t);
                    newline = 0;
                } else {
                    printf(" ");
                    printf("%s", t);
                    newline = 0;
                }
            }
            t = strtok(NULL, " ");
        }
    } else {
        printf("%s", line);
        newlinef = 1;
        newline = 1;
    }
}

int main(int argc, char * argv[]) {
    int count = 0;  // count number of lines  
    while (fgets(input_line, MAX_LINE_LEN, stdin) != 0) {
        count++;
        print_the_words(input_line);
    }
    if (count != 0) {
        printf("\n");
    }
}