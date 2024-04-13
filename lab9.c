#include <stdio.h>
#include <stdlib.h>
// RecordType
struct RecordType
{
	int		id;
	char	name;
	int		order; 
};

// Fill out this structure
struct node {
    struct RecordType data;
    struct node* next;
};

struct HashType {
    struct node** heads;
    int size;                 
};

// Compute the hash function
int hash(int x, int tableSize) {
    return x % tableSize;
}


// parses input file to an integer array
int parseData(char* inputFileName, struct RecordType** ppData)
{
	FILE* inFile = fopen(inputFileName, "r");
	int dataSz = 0;
	int i, n;
	char c;
	struct RecordType *pRecord;
	*ppData = NULL;

	if (inFile)
	{
		fscanf(inFile, "%d\n", &dataSz);
		*ppData = (struct RecordType*) malloc(sizeof(struct RecordType) * dataSz);
		// Implement parse data block
		if (*ppData == NULL)
		{
			printf("Cannot allocate memory\n");
			exit(-1);
		}
		for (i = 0; i < dataSz; ++i)
		{
			pRecord = *ppData + i;
			fscanf(inFile, "%d ", &n);
			pRecord->id = n;
			fscanf(inFile, "%c ", &c);
			pRecord->name = c;
			fscanf(inFile, "%d ", &n);
			pRecord->order = n;
		}

		fclose(inFile);
	}

	return dataSz;
}

// prints the records
void printRecords(struct RecordType pData[], int dataSz)
{
	int i;
	printf("\nRecords:\n");
	for (i = 0; i < dataSz; ++i)
	{
		printf("\t%d %c %d\n", pData[i].id, pData[i].name, pData[i].order);
	}
	printf("\n\n");
}

struct HashType* initializeHashTable(int size) {
    struct HashType* newTable = malloc(sizeof(struct HashType));
    if (newTable == NULL) {
        printf("Failure.\n");
        exit(-1);
    }
    newTable->size = size;
    newTable->heads = malloc(sizeof(struct node*) * size);
    if (newTable->heads == NULL) {
        printf("Failure.\n");
        exit(-1);
    }
    for (int i = 0; i < size; i++) {
        newTable->heads[i] = NULL;
    }
    return newTable;
}


void insertRecord(struct HashType* hashTable, struct RecordType record) {
    int index = hash(record.id, hashTable->size);
    struct node* newNode = malloc(sizeof(struct node));
    if (newNode == NULL) {
        printf("Failure.\n");
        exit(-1);
    }
    newNode->data = record;
    newNode->next = hashTable->heads[index];
    hashTable->heads[index] = newNode;
}

// display records in the hash structure
// skip the indices which are free
// the output will be in the format:
// index x -> id, name, order -> id, name, order ....
void displayRecordsInHash(struct HashType *hashTable) {
    for (int i = 0; i < hashTable->size; ++i) {
        struct node* current = hashTable->heads[i];
        if (current != NULL) {
            printf("Index %d : ", i);
            while (current != NULL) {
                printf("%d %c %d -> ", current->data.id, current->data.name, current->data.order);
                current = current->next;
            }
            printf("NULL\n");
        }
    }
}


int main(void) {
    struct RecordType *pRecords;
    int recordSz = 0;
    int hashTableSize = 11;  // can be adjusted

    // Parse data from the input file
    recordSz = parseData("input_lab_9.txt", &pRecords);

    if (recordSz == 0) {
        printf("No records.\n");
        return 0;
    }

    // Initialize the hash table
    struct HashType* hashTable = initializeHashTable(hashTableSize);

    // Insert records into the hash table
    for (int i = 0; i < recordSz; i++) {
        insertRecord(hashTable, pRecords[i]);
    }

    printRecords(pRecords, recordSz);
    displayRecordsInHash(hashTable);

    free(pRecords);
    for (int i = 0; i < hashTableSize; i++) {
        struct node* current = hashTable->heads[i];
        while (current != NULL) {
            struct node* temp = current;
            current = current->next;
            free(temp);
        }
    }
    free(hashTable->heads);
    free(hashTable);

    return 0;
}
