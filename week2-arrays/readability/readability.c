#include <cs50.h>
#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

int count_letters(string s, int len);
int count_words(string s, int len);
int count_sentences(string s, int len);

int main(void)
{
    // Prompt the user for some text
    string txt = get_string("Text: ");

    // Calculate the lenght of the txt
    int len = strlen(txt);

    // Count the number of letters, words, sentnces in the text
    int letters = count_letters(txt, len);
    int words = count_words(txt, len);
    int sentences = count_sentences(txt, len);

    // Compute Coleman-Liau index
    float l = (letters / (float) words) * 100;
    float s = (sentences / (float) words) * 100;

    float index = 0.0588 * l - 0.296 * s - 15.8;
    int grade = round(index);

    // Print the grade level
    if (grade > 16)
    {
        printf("Grade 16+\n");
    }
    else if (grade < 1)
    {
        printf("Before Grade 1\n");
    }
    else
    {
        printf("Grade %i\n", grade);
    }
}

int count_letters(string s, int len)
{
    int letters = 0;
    for (int i = 0; i < len; i++)
    {
        if (isalpha(s[i]))
        {
            letters += 1;
        }
    }
    return letters;
}

int count_words(string s, int len)
{
    int spaces = 0;
    for (int i = 0; i < len; i++)
    {
        if (s[i] == ' ')
        {
            spaces += 1;
        }
    }
    int words = spaces + 1;
    return words;
}

int count_sentences(string s, int len)
{
    int sent = 0;
    for (int i = 0; i < len; i++)
    {
        if (s[i] == '.' || s[i] == '?' || s[i] == '!')
        {
            sent += 1;
        }
    }
    return sent;
}
