import sys
import re

class SENJIFY:

    def __init__(self, input_stream):
        self.input_stream = input_stream

    def format(self):
        result = []
        indent = 0
        width = 80
        fixed_width = 0
        new_width = 0
        formatline = 0
        newline = 1
        newlinef = 0
        count = 0
        newindex = 0
        for line in self.input_stream:
            count = count + 1
            matchnewline = re.match("\n", line)
            if matchnewline:
                newline = 1
                if newlinef == 1:
                    result.append("")
                    newindex += 1
                    continue
                else:
                    #result[len(result)-1] = re.sub("\n$", "", result[len(result)-1])
                    result.append("")
                    newindex += 2
                    #result[len(line)-1] += "\n"
                    #newindex += 2
                    newlinef = 1
                    continue
            newlinef = 0
            linenew = line
            linenew = re.sub("\A\n", "", linenew) # remove leading characters
            linenew = re.sub("\n$", "", linenew) # remove trailing characters
            linenew = re.sub("^\s+", "", linenew)
            words = re.split("\s+", linenew) # create a list of words in line
            if (len(words) > 2):
                matchobj = re.match("\A{{ \d", line)
                if matchobj:
                    width = int(words[1])
                    formatline = 1
                    newline = 1
                    continue
                matchobj = re.match("\A{{ >\d", line)
                if matchobj:
                    words[1] = re.sub(">", "", words[1])
                    indent = int(words[1])
                    newline = 1
                    continue
                matchobj = re.match("\A{{ \+>\d", line)
                if matchobj:
                    words[1] = re.sub("\+>", "", words[1])
                    indent = indent + int(words[1])
                    if indent > (width - 20):
                        indent = width - 20
                    newline = 1
                    continue
                matchobj = re.match("\A{{ \->\d", line)
                if matchobj:
                    words[1] = re.sub("\->", "", words[1])
                    indent = indent - int(words[1])
                    if indent < 0:
                        indent = 0
                    newline = 1
                    continue
                matchobj = re.match("\A{{ off", line)
                if matchobj:
                    formatline = 0
                    newline = 1
                    continue
                matchobj = re.match("\A{{ on", line)
                if matchobj:
                    formatline = 1
                    newline = 1
                    continue
                matchobj = re.match("\A{{ !", line)
                if matchobj:
                    newline = 1
                    if formatline == 0:
                        formatline = 1
                    else:
                        formatline = 0
                    continue
            spaces = ""   
            if formatline == 1:
                for i in words:
                    if (newline == 1 and indent > 0):
                        for j in range(indent):
                            spaces = spaces + " "
                        try:
                            result[newindex] = result[newindex] + spaces
                        except:
                            result.append(spaces)
                        spaces = ""
                    if (newline == 1):
                        fixed_width = 0
                        new_width = 0      
                        fixed_width = width - indent
                        new_width = new_width + len(i)
                    else:
                        new_width = new_width + len(i) + 1
                    if (new_width > fixed_width):
                        result.append("")
                        newindex = newindex + 1 
                        for k in range(indent):
                            spaces = spaces + " "
                        try:
                            result[newindex] = result[newindex] + spaces
                        except:
                            result.append(spaces)
                        spaces = ""
                        try:
                            result[newindex] = result[newindex] + i
                        except:
                            result.append(i)
                        newline = 0
                        fixed_width = 0 
                        new_width = 0
                        fixed_width = width - indent
                        new_width = new_width + len(i)
                    else:
                        if (newline == 1):
                            try:
                                result[newindex] = result[newindex] + i
                            except:
                                result.append(i)
                            newline = 0
                        else:
                            try:
                                result[newindex] = result[newindex] + " "
                            except:   
                                result.append(" ")
                            try:
                                result[newindex] = result[newindex] + i
                            except:   
                                result.append(i)
                            newline = 0
            else:
                try:
                    result[newindex] = result[newindex] + line
                except:
                    result.append(line)
                newline = 1
                newlinef = 1     
        if count != 0:
            result[len(result)-1] = re.sub("\n$", "", result[len(result)-1])
            result[len(result)-1] = re.sub(" $", "", result[len(result)-1])
        return result

