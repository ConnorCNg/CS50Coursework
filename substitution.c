#include <cs50.h>
#include <ctype.h>
#include <search.h>
#include <stdio.h>
#include <string.h>

const int keyLenReq = 26;

void badArg(void);

int main(int keyc, string keyv[])
{
    if (keyc != 2) // Return error if more or less than 1 command-line argument
    {
        printf("You did not enter the correct # of arguments\n");
        badArg();
        return 1;
    }

    int keyLen = strlen(keyv[1]);
    if (keyLen != keyLenReq)
    {
        printf("Your key was not 26 digits\n");
        badArg();
        return 1;
    }

    /*
    ENTRY e;
    ENTRY *ep;
    hcreate(26);
    */
    char dups[keyLen];

    for (int i = 0; i < (keyLen); i++)
    {
        char letter = tolower(keyv[1][i]);
        if (!isalpha(letter))
        {
            printf("%c is not alphabetical\n", letter);
            badArg();
            return 1;
        }
        dups[i] = tolower(letter);
        for (int j = 0; j < i; j++) // j less than i so never comparing to itself
        {
            if (dups[j] == letter)
            {
                printf("The characters in position %i and %i are duplicates\n", j + 1, i + 1);
                return 1;
            }
        }
        // Search for dups

        /*
        char buffer[2] = {tolower(keyv[1][i]), '\0'};
        e.key = buffer;
        e.data = (void *)i;
        ep = hsearch(e, FIND);
        if (ep == NULL)
        {
            hsearch(e, ENTER);
        }
        else
        {
            printf("%c already exists in position %i\n", keyv[1][i], i);
            return 1;
        }
        */
    }
    // hdestroy();
    string plaintext = get_string("plaintext:  ");
    int wordLen = strlen(plaintext);
    char ciphertext[wordLen + 1];
    for (int count = 0; count <= wordLen; count++)
    {
        char plainchar = plaintext[count];
        if (isupper(plainchar) && isalpha(plainchar))
        {
            int position = plainchar - 65;
            char convert = keyv[1][position];
            ciphertext[count] = toupper(convert);
        }
        else if (islower(plainchar) && isalpha(plainchar))
        {
            int position = plainchar - 97;
            char convert = keyv[1][position];
            ciphertext[count] = tolower(convert);
        }
        else
        {
            ciphertext[count] = plaintext[count];
        }
    }
    // ciphertext[wordLen] = '\0';
    printf("ciphertext: %s\n", ciphertext);
}

void badArg(void)
{
    printf("Please enter a single, 26-character alphabetical cypher key in the following format:\n");
    printf("./substituion ABCDEFGHIJKLMNOPQRSTUVWXYZ\n");
}
