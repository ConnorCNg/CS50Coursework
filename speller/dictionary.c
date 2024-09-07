// Implements a dictionary's functionality

#include <ctype.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
} node;

// TODO: Choose number of buckets in hash table
// const unsigned int N = 701; // 26^2 + 26 - 1.
const unsigned int N = 552880; // sum of 27^i for i from 0 to 4 (minus 1)
// const unsigned int N = 14803587; // sum of 27^i for i from 0 to 5 (minus 1)
//const unsigned long N = 293087920184;

// Hash table
node *table[N];

bool loaded = false;
int numloaded = 0;

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    // TODO
    int hashnum = hash(word);
    int slen = strlen(word);
    char wordtmp[LENGTH + 1];
    for (int i = 0; i < LENGTH + 1; i++)
    {
        if (i < slen)
        {
            wordtmp[i] = tolower(word[i]);
        }
        else
        {
            wordtmp[i] = '\0';
        }
    }
    for (node *ptr = table[hashnum]; ptr != NULL; ptr = ptr->next)
    {
        // printf("test %s\n", ptr->word);
        int compare = strcmp(wordtmp, ptr->word);
        // printf("%i\n", compare);
        if (compare > 0)
        {
            return false;
        }
        else if (compare == 0)
        {
            return true;
        }
    }
    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    // TODO: Improve this hash function
    int slen = strlen(word);

    if (slen == 1)
    {
        return abs(toupper(word[0]) - 'A');
    }
    else if (slen == 2)
    {
        return abs((toupper(word[0]) - 'A')) + (27 * (abs((toupper(word[1]) - 'A')) + 1)) - 1;
    }
    else if (slen == 3)
    {
        return abs((toupper(word[0]) - 'A')) + (27 * (abs((toupper(word[1]) - 'A')) + 1)) + (729 * (abs((toupper(word[2]) - 'A')) + 1)) - 1;
    }
    /*
    else if (slen == 4)
    {
        return abs((toupper(word[0]) - 'A')) + (27 * (abs((toupper(word[1]) - 'A')) + 1)) + (729 * (abs((toupper(word[2]) - 'A')) + 1)) + (19683 * (abs((toupper(word[3]) - 'A')) + 1)) - 1;
    }
    return abs((toupper(word[0]) - 'A')) + (27 * (abs((toupper(word[1]) - 'A')) + 1)) + (729 * (abs((toupper(word[2]) - 'A')) + 1)) + (19683 * (abs((toupper(word[3]) - 'A')) + 1)) + (531441 * (abs((toupper(word[4]) - 'A')) + 1)) - 1;
    */
    return abs((toupper(word[0]) - 'A')) + (27 * (abs((toupper(word[1]) - 'A')) + 1)) + (729 * (abs((toupper(word[2]) - 'A')) + 1)) + (19683 * (abs((toupper(word[3]) - 'A')) + 1)) - 1;


    /*
    if (strlen(word) == 1)
    {
        return 676 + (toupper(word[0]) - 'A');
    }
    //printf("%s\n", word);
    //printf("%i\n", (26 * (toupper(word[0]) - 'A')));
    //printf("%i\n", (toupper(word[1]) - 'A'));
    return ((26 * (toupper(word[0]) - 'A')) + (toupper(word[1]) - 'A'));
    */
   /*
    if (slen > 4)
    {
        slen = 4;
    }
    int hashval = 0;
    for (int i = 0; i < slen; i++)
    {
        hashval = hashval + (pow(27, i) * ((abs(toupper(word[i]) - 'A')) + 1));
        //printf("%i\n", hashval - 1);
    }
    //printf("%i\n", hashval - 1);
    return hashval - 1;
    */

}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    FILE *file = fopen(dictionary, "r");
    if (file == NULL)
    {
        printf("Dictionary could not be opened");
        return false;
    }
    char *word = NULL;
    int endcheck;
    while ((endcheck = fscanf(file, "%ms", &word)))
    {
        if (endcheck == EOF)
        {
            break;
        }
        unsigned int hashnum = hash(word);
        node *n = malloc(sizeof(node));
        strcpy(n->word, word);
        if (table[hashnum] != NULL)
        {
            n->next = table[hashnum];
        }
        else
        {
            n->next = NULL;
        }

        table[hashnum] = n;
        numloaded++;
        free(word);
        word = NULL;
    }
    fclose(file);
    if (endcheck == EOF)
    {
        loaded = true;
        return true;
    }
    return false;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    // TODO
    if (loaded == true)
    {
        return numloaded;
    }
    return 0;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    // TODO
    for (int i = 0; i < N - 1; i++)
    {
        if (table[i] != NULL)
        {
            node *ptr = table[i];
            while (ptr != NULL)
            {
                node *next = ptr->next;
                free(ptr);
                ptr = next;
            }
        }
    }
    return true;
}
