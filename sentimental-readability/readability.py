from cs50 import get_string


def calcCL(text):
    lcount = 0
    wcount = 0
    scount = 0
    for i in range(0, len(text)):
        if text[i].isalpha():
            lcount += 1
        elif text[i] == " ":
            wcount += 1
        elif (text[i] == ".") or (text[i] == "!") or (text[i] == "?"):
            scount += 1
    if (text[-1] == ".") or (text[-1] == "!") or (text[-1] == "?"):
        wcount += 1

    index = round(((0.0588 * (lcount / wcount) * 100) - (0.296 * (scount / wcount) * 100) - 15.8))
    if index < 1:
        return "Before Grade 1"
    elif index >= 16:
        return "Grade 16+"
    else:
        return "Grade " + str(index)


def main():
    text = get_string("Text: ")
    print(calcCL(text))


main()
