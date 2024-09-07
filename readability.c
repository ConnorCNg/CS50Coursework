#include <cs50.h>
#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

const int wordDelimit = 32;
const char alphaLower = 'A';
const char alphaUpper = 'Z';
// const char sentenceEnd = '.';

int main(void)
{
    int letterCount = 0;
    int wordCount = 0;
    int sentenceCount = 0;
    float letterAvg = 0;
    float sentenceAvg = 0;

    string text = get_string("Text: ");
    int textLen = strlen(text);

    for (int i = 0; i <= textLen; i++) // <= since  \0 is one of the "word-end" indicators"
    {
        char upperChar = toupper(text[i]);
        if ((upperChar >= alphaLower) && (upperChar <= alphaUpper)) // 'A' to 'Z'
        {
            letterCount++;
        }
        else if ((upperChar == 33) || (upperChar == 63) || (upperChar == 46)) // period, exclamation mark, quesiton mark
        {
            sentenceCount++;
        }
        else if ((upperChar == wordDelimit) || (upperChar == 0)) // 32 for a space or 0 for text end
        {
            wordCount++;
        }
    }
    // Coleman-Liau Index Calculation
    float L = (((float) letterCount / (float) wordCount) * 100);
    float S = (((float) sentenceCount / (float) wordCount) * 100);
    float index = (0.0588 * L) - (0.296 * S) - 15.8;

    if (index < 1)
    {
        printf("Before Grade 1\n");
    }
    else if (index >= 16)
    {
        printf("Grade 16+\n");
    }
    else
    {
        printf("Grade %i\n", (int) round(index));
    }
}
