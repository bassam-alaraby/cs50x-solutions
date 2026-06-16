// Implements a dictionary's functionality

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
} node;

// Counter to track number of words in dictionary
unsigned int word_count = 0;

// Number of buckets in hash table
const unsigned int N = 50021;

// Prime multiplier
const unsigned int PRIME_MULTIPLIER = 37;

// Hash table
node *table[N];

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    // Hash the word to obtain its hash value
    unsigned int hash_value = hash(word);

    // Search the hash table at the location specified by the word’s hash value
    for (node *cursor = table[hash_value]; cursor != NULL; cursor = cursor->next)
    {
        if (strcasecmp(word, cursor->word) == 0)
        {
            return true;
        }
    }
    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    unsigned int hash_value = 0;
    for (int i = 0; word[i] != '\0'; i++)
    {
        hash_value = hash_value * PRIME_MULTIPLIER + tolower(word[i]);
    }
    return hash_value % N;
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    // Open the dictionary file
    FILE *source = fopen(dictionary, "r");
    if (source == NULL)
    {
        printf("Can not open the dictionary.\n");
        return false;
    }

    // Read each word in the file
    char buffer[LENGTH + 1];
    while (fscanf(source, "%s", buffer) != EOF)
    {
        // Create space for a new hash table node
        node *n = malloc(sizeof(node));
        if (n == NULL)
        {
            printf("No enough memory.\n");
            return false;
        }

        // Copy the word into the new node
        strcpy(n->word, buffer);
        n->next = NULL;

        // Hash the word to obtain its hash value
        unsigned int hash_value = hash(n->word);

        // Insert the new node into the hash table (using the index specified by its hash value)
        n->next = table[hash_value];
        table[hash_value] = n;

        word_count++;
    }

    // Close the dictionary file
    fclose(source);
    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    return word_count;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    for (int i = 0; i < N; i++)
    {
        node *cursor = table[i];
        node *temp;
        while (cursor != NULL)
        {
            temp = cursor;
            cursor = cursor->next;
            free(temp);
        }
        table[i] = NULL;
    }
    return true;
}
