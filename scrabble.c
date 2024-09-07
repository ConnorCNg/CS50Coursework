#include <cs50.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const int numPlayers = 2;
string pointMap = "02210313074020029000033739";
const int firstLetter = 'A';

int scoreEval(string word);

int main(void)
{
    int playerCount = 0;
    int currentWinner = 0;
    int scores[numPlayers];
    int highestScore = 0;
    while (playerCount < numPlayers)
    {
        playerCount++;
        string curWord = get_string("Player %i: ", playerCount);
        int playerScore = scoreEval(curWord);
        if (playerScore > highestScore)
        {
            highestScore = playerScore;
            currentWinner = playerCount;
        }
        else if ((playerScore == highestScore) && (highestScore != 0))
        {
            currentWinner = 0;
        }
    }
    if (currentWinner == 0)
    {
        printf("Tie!\n");
    }
    else
    {
        printf("Player %i wins!\n", currentWinner);
    }
}

int scoreEval(string word)
{
    int curScore = 0;
    int mapLength = strlen(pointMap);
    for (int i = 0; i < strlen(word); i++)
    {
        int scorePosition = toupper((char) word[i]) - firstLetter;
        char pointMapScore = pointMap[scorePosition];
        int points = atoi(&pointMapScore) + 1;
        if ((scorePosition >= 0) && (scorePosition < mapLength))
        {
            curScore += points;
        }
    }
    // printf("%i\n", curScore);
    return curScore;
}
