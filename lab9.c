#include <stdio.h>
#include <stdlib.h>

struct RecordType
{
    int identifier;
    char label;
    int sequence;
};

struct HashNode
{
    struct RecordType data;
    struct HashNode *next;
};

struct HashTable
{
    struct HashNode *bucket;
};

int hash(int x)
{
    return x % 23; 
}

int parseInputFile(char *fileName, struct RecordType **pData)
{
    FILE *file = fopen(fileName, "r");
    int i;
    if (file == NULL)
    {
        printf("Error opening file.\n");
        exit(EXIT_FAILURE);
    }

    int dataSize;
    fscanf(file, "%d\n", &dataSize);

    *pData = (struct RecordType *)malloc(sizeof(struct RecordType) * dataSize);
    if (*pData == NULL)
    {
        printf("Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }

    for (i = 0; i < dataSize; ++i)
    {
        fscanf(file, "%d %c %d", &(*pData)[i].identifier, &(*pData)[i].label, &(*pData)[i].sequence);
    }

    fclose(file);

    return dataSize;
}

void printRecords(struct RecordType data[], int dataSize)
{
    int i;
    printf("Data Items:\n");
    for (i = 0; i < dataSize; ++i)
    {
        printf("\t%d %c %d\n", data[i].identifier, data[i].label, data[i].sequence);
    }
}

void displayRecordsInHash(struct HashTable *hashArray[], int hashSize)
{
    int i;
    for (i = 0; i < hashSize; ++i)
    {
        if (hashArray[i] != NULL)
        {
            printf("Index %d -> ", i);
            struct HashNode *current = hashArray[i]->bucket;
            while (current != NULL)
            {
                printf("%d %c %d", current->data.identifier, current->data.label, current->data.sequence);
                current = current->next;

                if (current != NULL)
                    printf("-> ");
                else
                    break;
            }
            printf("\n");
        }
    }
}

int main(void)
{
    struct RecordType *dataItems;
    int dataSize = 0;

    dataSize = parseInputFile("input_lab_9.txt", &dataItems);

    printRecords(dataItems, dataSize);

    struct HashTable *hashTable[23] = {NULL}; 
    int i;

    for (i = 0; i < dataSize; ++i)
    {
        int index = hash(dataItems[i].identifier);
        if (hashTable[index] == NULL)
        {
            hashTable[index] = (struct HashTable *)malloc(sizeof(struct HashTable));
            hashTable[index]->bucket = NULL;
        }
        struct HashNode *newNode = (struct HashNode *)malloc(sizeof(struct HashNode));
        newNode->data = dataItems[i];
        newNode->next = hashTable[index]->bucket;
        hashTable[index]->bucket = newNode;
    }


    displayRecordsInHash(hashTable, 23);

    for (i = 0; i < 23; ++i)
    {
        if (hashTable[i] != NULL)
        {
            struct HashNode *current = hashTable[i]->bucket;
            while (current != NULL)
            {
                struct HashNode *temp = current;
                current = current->next;
                free(temp);
            }
            free(hashTable[i]);
        }
    }
    free(dataItems);

    return 0;
}
