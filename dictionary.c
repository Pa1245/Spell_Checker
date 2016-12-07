/**
 * dictionary.c
 *
 * Computer Science 50
 * Problem Set 5
 *
 * Implements a dictionary's functionality.
 */

#include <stdbool.h>
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#include "dictionary.h"

#define ALPHABETS 26

typedef struct trie {
    struct trie* node[ALPHABETS+1];
    bool isWord;
}trie;

struct trie* numOfLetter[10] = {NULL};
int numOfWords = 0;

/*trie* createTrie() {
    struct trie* ptr = malloc(sizeof(trie));
    ptr->isWord = false;
    for(int i = 0; i < ALPHABETS + 1; i++)
        ptr->node[i] = NULL;
    return ptr;
    free(ptr);
}*/

/**
 * Returns true if word is in dictionary else false.
 */
bool check(const char* word)
{
    // TODO
    int len = strlen(word);
    struct trie* ptr = numOfLetter[(len - 1)%10];
    if (ptr == NULL) 
        return false;
    else {
        int i = 0, c = 0;
        while (i < len) {
            if(isalpha(word[i])) {
                if(!islower(word[i])) {
                    c = tolower(word[i]);
                    c -= 'a';
                }
                else {
                    c = word[i] - 'a';
                }
            }
            else if(word[i] == '\'')
                c = ALPHABETS;
            ptr = ptr->node[c];
            if (ptr == NULL) 
                return false;
            i++;
        }
        if(ptr->isWord)
            return true;
    }
    return false;
}

/**
 * Loads dictionary into memory.  Returns true if successful else false.
 */
bool load(const char* dictionary)
{
    // TODO
    FILE* fp = fopen(dictionary,"r");
    if (fp == NULL) {
        fclose(fp);
        return false;        
    }

    struct trie* ptr = NULL;
    int c = fgetc(fp);
    while (c != EOF) {
        char word[LENGTH + 1];
        int i = 0;
        while ((c != '\n' && c != EOF) && i < LENGTH + 1) {
            word[i] = c;
            i++;
            c = fgetc(fp);
        }
        word[i] = '\0';
        if (numOfLetter[(i-1) % 10] == NULL) {
            numOfLetter[(i-1) % 10] = malloc(sizeof(trie));
            numOfLetter[(i-1) % 10]->isWord = false;
            for(int j = 0; j < ALPHABETS + 1; j++)
                numOfLetter[(i-1) % 10]->node[j] = NULL;
        }
        ptr = numOfLetter[(i-1) % 10];
        int j = 0;
        while (j < i) {
            if (isalpha(word[j]))
                c = word[j] -'a';
            else if (word[j] == '\'')
                c = ALPHABETS;
            if (ptr->node[c] == NULL) { 
                //ptr->node[c] = createTrie();  
                ptr->node[c] = malloc(sizeof(trie));
                (ptr->node[c])->isWord = false;
                for(int i = 0; i < ALPHABETS + 1; i++)
                    (ptr->node[c])->node[i] = NULL;
            }
            ptr = ptr->node[c];
            j++;
        }
        ptr->isWord = true;
        numOfWords++;
        c = fgetc(fp);
    }
    //printf("Total number of Words loaded = %d\n",numOfWords);
    fclose(fp);
    return true;
}

/**
 * Returns number of words in dictionary if loaded else 0 if not yet loaded.
 */
unsigned int size(void)
{
    // TODO
    return numOfWords;
}

void unTrie(struct trie* branch) {
    if (branch != NULL) {
        for (int i = 0; i < ALPHABETS + 1; i++) {
            if (branch->node[i] != NULL)
                unTrie(branch->node[i]);
            if(i == ALPHABETS && branch->node[i] == NULL)
                free(branch);
        }
    }
}
    
bool unload(void){
    // TODO
    for (int i = 0; i < 10; i++) {
        if(numOfLetter[i] != NULL)
            unTrie(numOfLetter[i]);
    }
    return true;
}


