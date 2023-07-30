#include <stdio.h>
#include <stdlib.h>

// RecordType
struct RecordType
{
	int	id;
	char	name;
	int	order; 
};

// Fill out this structure
struct HashType
{
	struct RecordType records;
	int length;
	struct HashType* next;
};

// Compute the hash function
int hash(int x, int size)
{
	return x % size;
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
		if (*ppData == NULL)
		{
			printf("Cannot allocate memory\n");
			exit(-1);
		}
		for (i = 0; i < dataSz; i++)
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
	for (i = 0; i < dataSz; i++)
	{
		printf("\t%d %c %d\n", pData[i].id, pData[i].name, pData[i].order);
	}
	printf("\n\n");
}

// display records in the hash structure
// skip the indices which are free
// the output will be in the format:
// index x -> id, name, order -> id, name, order ....
void displayRecordsInHash(struct HashType *pHashArray, int hashSz)
{
	int i;

	for (i = 0; i < hashSz; i++)
	{
		if (pHashArray[i].next != NULL)
		{
			printf("Index %d -> ", i);
			struct HashType* current = pHashArray[i].next;

			while (current != NULL)
			{
				printf("ID: %d Name: %c Order: %d -> ", current->records.id, current->records.name, current->records.order);
				current = current->next;
			}

			printf("\n");
		}
	}
}

int main(void)
{
	struct RecordType *pRecords;
	int recordSz = 0;

	recordSz = parseData("input.txt", &pRecords);
	printRecords(pRecords, recordSz);
	
	struct HashType pHashArray[15];
	for (int i = 0;i<15;i++)
	{
		pHashArray[i].next = NULL;
	}

	for (int i = 0; i < recordSz; i++)
    {
        struct RecordType *pRecord = pRecords + i;
        int index = hash(pRecord->order, 15);

        struct HashType *newNode = (struct HashType*)malloc(sizeof(struct HashType));
        if (newNode == NULL)
        {
            printf("Memory allocation error.\n");
            exit(-1);
        }
        newNode->records = *pRecord;
        newNode->next = NULL;

        if (pHashArray[index].next == NULL)
        {
            pHashArray[index].next = newNode;
        }
        else
        {
            struct HashType *current = pHashArray[index].next;
            while (current->next != NULL)
            {
                current = current->next;
            }
            current->next = newNode;
        }
    }
	displayRecordsInHash(pHashArray, 15);
}
