// Implements a dictionary's functionality

#include "dictionary.h"

//declare structs for linked list
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

//declare array of nodes/buckets
node hashArr[26];

// Returns true if word is in dictionary else false
bool check(const char *word)
{
    //find out what bucket hash belongs in
    //convert first letter to lowercase
    char first = word[0];
    first = tolower(first);
    int hash = 0;
    hash = first;
    hash = hash - 97;
    //check if word is in hashtable
    node *end = &hashArr[hash];

    while (end != NULL)
    {
        if (strcasecmp(end->word, word) == 0)
        {
            return true;
        }
        end = end->next; //traverse to next link
    }

    //printf("%s", word);
    return false;
}

// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{

    //'dictionary' is the text file containing words
    FILE *fp = fopen(dictionary, "r");

    if (fp == NULL)
    {
        printf("File does not exist.\n");
        return false;
    }
    //fill struct array with place holder values
    for (int i = 0; i < 26; i++)
    {
        hashArr[i].word[0] = '-';
        hashArr[i].next = NULL;
    }

    char line[50]; //array holds words
    int hash = 0;

    while (fscanf(fp, "%s", line) != EOF)
    {
        //create new node and insert word
        node *newnode = malloc(sizeof(node));
        newnode->next = NULL;
        //check if malloc failed to create new node
        if (newnode == NULL)
        {
            unload();
            return false;
        }
        strcpy(newnode->word, line);

        //hash & insert into hash table
        hash = hashfunc(line);

        if (hashArr[hash].word[0] == '-')
        {
            //set newnode to root of this bucket
            hashArr[hash] = *newnode;
        }
        else
        {
            node *end = &hashArr[hash];

            while (end->next != NULL)
            {
                end = end->next;
            }
            //set the end equal to the new node
            end->next = newnode;
        }
    }
    fclose(fp);
    return true;
}

// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
    int size = 0;
    for (int i = 0; i < 26; i++)
    {
        node *cursor = &hashArr[i];
        while (cursor != NULL && cursor->word[0] != '-')
        {
            cursor = cursor->next;
            size++;
        }
    }
    return size;
}

// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
    //free the linked list
    printf("Made it to unload!\n");
    int count = 0;

    for (int i = 0; i < 26; i++)
    {
        printf("Iteration: %d\n", count);
        node *cursor = &hashArr[i];
        printf("Assigned cursor to index %d\n", i);
        printf("current word: %s\n", cursor->word);
        while (cursor != NULL)
        {
            node *temp = cursor;
            printf("Assigned temp to cursor!\n");
            cursor = cursor->next;
            printf("value of temp: %s\n", temp->word);
            free(temp);
            printf("Free'd temp!!\n");
        }
        count++;
        free(cursor);
    }
    printf("Finished unloading ~~~\n");
    return true;
}

int hashfunc(char line[])
{
    int ascii = 0;
    ascii = line[0];
    ascii = ascii - 97; //subtract 97 from ascii value
    return ascii; //to keep numbers below 26
}