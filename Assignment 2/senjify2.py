#!/usr/bin/env python3

import fileinput
import sys

indent = 0       # default indentation
width = 80       # default width
fixed_width = 0  # width - indent
new_width = 0    # reset after printing each word
formatline = 0   # formatting is off
newline = 1      # determine if the pointer is at start of line
newlinef = 0     # determines if one or two new lines must be printed

def print_the_words(line):
    """ This function get every line from the input file or stdin
        and stores every word in a list called words. Amongst 
        formatting the line, it sets the right width and indent 
        and turns the formatting on or off. Finally, it prints
        the output to stdout. """
    # use the global ketword to modify variables outside the function
    global indent
    global width
    global fixed_width
    global new_width
    global formatline
    global newline
    global newlinef
    # check if the line is empty
    if line in ['\n', '\r\n']:
        newline = 1
        if newlinef == 1:
            print("")
            return
        else:
            print("")
            print("")
            newlinef = 1
            return
    newlinef = 0
    # remove unnecessary whitespace at the start and end of the line
    line2 = line.strip()     
    # create a list of strings (include words separated by " ")
    words = line2.split()
    # check for width formatting
    if ((len(words) > 2) and (words[0] == "{{") and (words[1].isdigit())):
        width = int(words[1])
        formatline = 1
        newline = 1
        return
    # check for indent formatting
    if ((len(words) > 2) and (words[0] == "{{") and (words[1][0] == ">") and (words[1][1].isdigit())):
        words[1] = words[1].replace(">", "")
        indent = int(words[1])
        newline = 1
        return
    # check for indent addition 
    if ((len(words) > 2) and (words[0] == "{{") and (words[1][0] == "+") and (words[1][1] == ">") and (words[1][2].isdigit())):
        words[1] = words[1].replace("+", "")
        words[1] = words[1].replace(">", "")
        indent = indent + int(words[1])
        if indent > (width - 20):
            indent = width - 20
        newline = 1
        return
    # check for indent subtraction
    if ((len(words) > 2) and (words[0] == "{{") and (words[1][0] == "-") and (words[1][1] == ">") and (words[1][2].isdigit())):
        words[1] = words[1].replace("-", "")
        words[1] = words[1].replace(">", "")
        indent = indent - int(words[1])
        if indent < 0:
            indent = 0
        newline = 1
        return 
    # check if the formatting needs to be turned off
    if ((len(words) > 2) and (words[0] == "{{") and (words[1] == "off")):
        formatline = 0
        newline = 1
        return
    # check if the formatting needs to be turned on
    if ((len(words) > 2) and (words[0] == "{{") and (words[1] == "on")):
        formatline = 1
        newline = 1
        return
    # check for toggling the formatting mode 
    if ((len(words) > 2) and (words[0] == "{{") and (words[1] == "!")):
        newline = 1
        if (formatline == 0):
            formatline = 1
        else:
            formatline = 0
        return
    # check if the formatting is on
    if (formatline == 1):
        for i in words:
            if (newline == 1 and indent > 0):
                # prints the indentation
                for j in range(indent):
                    print(" ", end = "")
            if (newline == 1):
                fixed_width = 0
                new_width = 0
                # set the maximum possible width
                fixed_width = width - indent
                # set the changeable width based on the words' length
                new_width = new_width + len(i)
            else:
                new_width = new_width + len(i) + 1
            if (new_width > fixed_width):
                print("")
                for k in range(indent):
                    print(" ", end = "")
                print(i, end = "")
                newline = 0
                fixed_width = 0
                new_width = 0
                fixed_width = width - indent
                new_width = new_width + len(i)
            else:
                if (newline == 1):
                    print(i, end = "")
                    newline = 0
                else:
                    print(" ", end = "")
                    print(i, end = "")
                    newline = 0
    else:
        print(line, end = "")
        newline = 1   
        newlinef = 1
   
def main():
    count = 0
    # get every line from the input or stdin
    for line in fileinput.input():
        count = count + 1
        print_the_words(line)
    if(count != 0 and newlinef == 0):
        print("")

if __name__ == "__main__":
    main()
