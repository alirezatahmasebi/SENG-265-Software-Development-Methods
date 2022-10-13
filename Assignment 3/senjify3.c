#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "list.h"

// comments about variables included in the main function
typedef struct variables {
    int indent; 
    int width; 
    int fixed_width; 
    int new_width; 
    int format; 
    int newline;
    int newlinef;
    int newlinecheck; 
    node_t *list; 
} Variables_struct;

/* the code in the get_number function is credited to Michael Zastre
   and is obtained from sample.c in the lab04 files  */ 
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

void print_the_words(char * line, Variables_struct *x) {
    x->list = add_front(x->list, new_node(line)); // take the input line and store it in a new node in a linked list
    node_t *p = peek_front(x->list); // the first element (most recent line) in the list is in node p
    char *t;  
    // check if the line is empty
    if (strlen(p->word) == 0) { 
        x->newline = 1;
        return;
    }
    // replace the newline at the end of each line with null character
    if (p->word[strlen(p->word) - 1] == '\n') { 
        p->word[strlen(p->word) - 1] = '\0';
    }
    // print a newline after printing unformatted lines
    if (x->newlinef == 1) { 
        printf("\n");
    }
    x->newlinef = 0;
    // check for width command which sets the max width for following lines
    if ((p->word[0] == '{') && (p->word[1] == '{') && (isdigit(p->word[3]))) {
        x->width = get_number(p->word);
        x->format = 1;
        x->newline = 1; 
        return;
    }
    // check for the indent command which sets the indentation (number of spaces) before printing lines
    if ((p->word[0] == '{') && (p->word[1] == '{') && (p->word[3] == '>') && (isdigit(p->word[4]))) {
        x->indent = get_number(p->word);
        x->newline = 1;
        return;
    }
    // check for a positive change in indentation (increase in # of spaces printed from the left-hand margin)
    if ((p->word[0] == '{') && (p->word[1] == '{') && (p->word[3] == '+') && (p->word[4] == '>') && (isdigit(p->word[5]))) {
        x->indent = x->indent + get_number(p->word);
        if (x->indent > (x->width - 20)){
            x->indent = x->width - 20;
        }
        x->newline = 1;
        return;
    }
    // check for a negative change in indentation (increase in # of spaces printed from the left-hand margin)
    if ((p->word[0] == '{') && (p->word[1] == '{') && (p->word[3] == '-') && (p->word[4] == '>') && (isdigit(p->word[5]))) {
        x->indent = x->indent - get_number(p->word);
        if (x->indent < 0){
            x->indent = 0;
        }
        x->newline = 1;
        return;
    }
    // check if the formatting is turned off (no formatting up to an on command or width command)
    if ((p->word[0] == '{') && (p->word[1] == '{') && (p->word[3] == 'o') && (p->word[4] == 'f') && (p->word[5] == 'f')) {
        x->format = 0;
        x->newline = 1;
        return;
    }
    // check if the formatting is turned on (formatting given the current width and indent up to an off command)
    if ((p->word[0] == '{') && (p->word[1] == '{') && (p->word[3] == 'o') && (p->word[4] == 'n')) {
        x->format = 1;
        x->newline = 1;
        return;
    }
    // toggle the formatting mode (if on then turn it off and if off then turn it on)
    if ((p->word[0] == '{') && (p->word[1] == '{') && (p->word[3] == '!')) {
        x->newline = 1;
        if (x->format == 0) {
            x->format = 1;
        } 
        else {
            x->format = 0;
        }
        return;
    }
    // if formatting is on then split the line into series of words separated by single spaces (" ")
    if (x->format == 1) {
        t = strtok(p->word, " ");
        if (t == NULL) {
            x->newline = 1;
            if (x->newlinecheck == 0){
              printf("\n");
              printf("\n");
              x->newlinecheck = 1;
            }
            else{
              printf("\n");
            }
        }
        while (t != NULL) {
            x->newlinecheck = 0;
            // print the indentation (# of spaces on the left-side margin)
            if (x->newline == 1 && x->indent > 0) {
                for (int k = 0; k < x->indent; k++) {
                    printf(" ");
                }
            }
            // check if at the start of a new empty line
            if (x->newline == 1) {
                x->fixed_width = 0;
                x->new_width = 0;
                x->fixed_width = x->width - x->indent;  // set the maximum possible width
                x->new_width = x->new_width + strlen(t); // set the changeable width based on the words' length
            } 
            // set the changeable width based on the words' length and single line spaces
            else {
                x->new_width = x->new_width + strlen(t) + 1; 
            }
            // check for exceeding the maximum width and print a new line with the appropiate indentation
            if (x->new_width > x->fixed_width) {
                printf("\n");
                for (int k = 0; k < x->indent; k++) {
                    printf(" ");
                }
                printf("%s", t); // print the first word in the line
                x->newline = 0;
                x->fixed_width = 0;
                x->new_width = 0;
                x->fixed_width = x->width - x->indent;
                x->new_width = x->new_width + strlen(t);
            } 
            else {
                // print the first word in the line 
                if (x->newline == 1) {
                    printf("%s", t);
                    x->newline = 0;
                } 
                // print a space and the next word in the line
                else {
                    printf(" ");
                    printf("%s", t);
                    x->newline = 0;
                }
            }
            // keep tokenizing the string (line)
            t = strtok(NULL, " ");
        }
    } 
    else {
        // if the formatting is off then print the line without any changes
        printf("%s", p->word);
        x->newlinef = 1;
        x->newline = 1;
    }
}

int main(int argc, char *argv[]) {
    Variables_struct var[20];
    var[0].indent = 0; // set indent to 0 (retrieved from the indent commands curly brackets)
    var[0].width = 80; // set default width to 80 (retrieved from width commands in curly brackets)
    var[0].fixed_width = 0; // set fixed_width (width-indent) to 0
    var[0].new_width = 0; // set new_width (reset after printing each word)
    var[0].format = 0; // set formatting to 0 (off)
    var[0].newline = 1; // set newline (determines if at the start of line) to 1 (on)
    var[0].newlinef = 0; // set newlinef (used after printing unformatted lines) to 0 (off)
    var[0].newlinecheck = 0; // check if the last printed line was a newline
    node_t *list = NULL; // empty linked list
    /* getline() is obtained from 05-c-dynamic-memory-DRAFT-1 and is credited to Michael Zastre */
    FILE *input_stream = NULL;
    char * line = NULL;
    size_t len = 0; // unsigned int
    size_t read; // function may return a size or a negative number
    // if no filename is provided as an argument for input then get input from stdin
    if (argc < 2) {
        input_stream = stdin;
    } 
    else {
        input_stream = fopen(argv[1], "r"); // get input_stream from the input files (in##.txt)
        // check for if there is an error when trying to read the files
        if (input_stream == NULL) {
            fprintf(stderr, "%s: unable to open %s\n",
                argv[0], argv[1]);
            exit(1);
        }
    }
    int count = 0;
    while ((read = getline(&line, &len, input_stream)) != -1) {     
        count++;
        print_the_words(line, &var[0]);
    }
    if (count != 0){
        printf("\n");
    }
    node_t  *temp_n = NULL;
    // go through the linked list and free each node 
    for ( ; list != NULL; list = temp_n ) {
        temp_n = list->next;
        free(list->word);
        free(list);
    } 
    // free the line obtained from input
    if (line) {
        free(line);
    } 
    fclose(input_stream);
    exit(0); 
}
