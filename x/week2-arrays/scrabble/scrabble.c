#include <cs50.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>

// Scores array:
int scores[] = {1, 3, 3, 2, 1, 4, 2, 4, 1, 8, 5, 1, 3, 1, 1, 3, 10, 1, 1, 1, 1, 4, 4, 8, 4, 10};

string lower(string s);
int calculate_score(string s);

int main(void)
{
    // Prompt the user for two words
    string word1 = get_string("Player 1: ");
    string word2 = get_string("Player 2: ");

    // Calculate the score of each word
    int score1 = calculate_score(word1);
    int score2 = calculate_score(word2);

    // Print the winner
    if (score1 > score2)
    {
        printf("Player 1 wins!\n");
    }
    else if (score1 < score2)
    {
        printf("Player 2 wins!\n");
    }
    else
    {
        printf("Tie!\n");
    }
}

int calculate_score(string s)
{
    int score = 0;
    for (int i = 0, len = strlen(s); i < len; i++)
    {
        if (isupper(s[i]))
        {
            score += scores[s[i] - 'A'];
        }
        else if (islower(s[i]))
        {
            score += scores[s[i] - 'a'];
        }
    }
    return score;
}
