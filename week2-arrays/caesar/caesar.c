#include <cs50.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define UPPERCASE_OFFSET 65
#define LOWERCASE_OFFSET 97

bool only_digits(string s);

int main(int argc, string argv[])
{
    // Check if the user provide just one command-line argument
    if (argc != 2)
    {
        printf("Usage: ./caesar Key\n");
        return 1;
    }

    // Check if the 2nd argument (Key) is digit
    if (!(only_digits(argv[1])))
    {
        printf("Usage: ./caesar Key\n");
        return 1;
    }

    // Convert the key to int
    const int K = atoi(argv[1]);

    // Prompt the user for plaintext
    string plaintext = get_string("plaintext:  ");

    // Encipher the plaintext
    printf("ciphertext: ");
    for (int i = 0, len = strlen(plaintext); i < len; i++)
    {
        if (isupper(plaintext[i]))
        {
            // convert ASCII to alphabetical index
            int alpha_index = plaintext[i] - UPPERCASE_OFFSET;

            // Shift the alphabetical index with formula
            int ciphertext_char = (alpha_index + K) % 26;

            // convert the result to ASCII again
            ciphertext_char += UPPERCASE_OFFSET;
            printf("%c", ciphertext_char);
        }
        else if (islower(plaintext[i]))
        {
            int alpha_index = plaintext[i] - LOWERCASE_OFFSET;
            int ciphertext_char = (alpha_index + K) % 26;
            ciphertext_char += LOWERCASE_OFFSET;
            printf("%c", ciphertext_char);
        }
        else
        {
            printf("%c", plaintext[i]);
        }
    }
    printf("\n");
    return 0;
}

bool only_digits(string s)
{
    for (int i = 0, len = strlen(s); i < len; i++)
    {
        if (!(isdigit(s[i])))
        {
            return false;
        }
    }
    return true;
}
