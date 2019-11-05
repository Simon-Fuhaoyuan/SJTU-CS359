/*
 * Student Name: Haoyuan Fu
 * Student ID: 517021910753 
 */

#include "cachelab.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

typedef struct
{
    int hit_count;
    int miss_count;
    int eviction_count;
}count;

typedef struct
{
    int S;
    int E;
    int B;
    char fileName[20];
}parameter;

// Function for -h to show information
void showInfo()
{
    printf("Show information\n");
}

// Function for wrong input in command line
void showWrongInfo()
{
    printf("Wrong input, please use -h to see details.\n");
}

// Function to calculate 2 to the power of value
int power(int value)
{
    if (value == 0) return 1;
    else return 2 * power(value - 1);
}

// Function to set priority for each line in a set based on LRU algorithm
void setPriority(parameter* par, int *cachePri, int setNum, int lineNum)
{
    if(lineNum >= par->E)
    {
	printf("Wrong lineNum!\n");
	return;
    }

    int setStart = setNum * par->E;
    int iniPri = *(cachePri + setStart + lineNum); // initial priority of the visited line
  
    if(iniPri == par->E - 1)
	return; // the visited line has already been the most recent one

    for (int i = setStart; i < setStart + par-> E; i++)
    {
	// only lines with priority greater than the initial priority need change
	if(*(cachePri + i) > iniPri)
	    *(cachePri + i) = *(cachePri + i) - 1;
    }

    *(cachePri + setStart + lineNum) = par->E - 1; // the most recent one's priority
}

// Function for simulate the cache
void cacheSim(parameter* par, count* cnt, bool flag)
{
    FILE *src = fopen(par->fileName, "r"); // trace file
    char *buffer;               // buffer to skip lines in files
    int first;                  // the first character of each line
    char oper;                  // operation
    long int addr = 0;          // address
    long int *cache;            // the start address of this cache
    int *cachePri;		// the priority of each line in a set
   
    // apply memory for buffer cache and cachePri.
    buffer = (char*)malloc(256 * sizeof(char));
    cache = (long int*)malloc(par->S * par->E * par->B * sizeof(long int));
    cachePri = (int*)malloc(par->S * par->E * sizeof (int));
   
    // clear all the space in cache.
    // In my cache design, -1 means empty, stored address if used.
    // IN my cache priority design, -1 means empty, 0~4 means used, higher priority is more difficult to be evicted
    for (int i = 0; i < par->S * par->E * par-> B; ++i)
    {
        *(cache + i) = -1;
        *(cachePri + i / par->B) = -1;
    }
        
    while ((first = fgetc(src)) != EOF)
    {
	if (first == (int)'I')
	    fgets(buffer, 255, src); // for I-type instruction, skip this line
	else if (first == (int)' ')
	{
	    oper = fgetc(src);
	    fscanf(src, "%lx", &addr);
	    int size = 0;
	    fgetc(src); // skip the ','
	    fscanf(src, "%d", &size); // get the size
	    fgets(buffer, 255, src); // after address, skip this line
	    if (flag) printf("%c %lx,%d ", oper, addr, size);
	    
	    int setNum = addr % (par->S * par->B);
	    int blockNum = setNum % par->B;
	    setNum = setNum / par->B;
	    long int blockStartAddr = addr - blockNum;
	    int lineNum = 0;
	    bool hit = false;
	    bool needEvict = true;
	    for (int i = setNum * par->B * par->E; i <= (setNum + 1) * par->B * par->E; i += par->B)
	    {
		if (*(cache + i) == blockStartAddr)
		{
		    needEvict = false;
		    hit = true;
		    if (flag) printf("hit ");
		    setPriority(par, cachePri, setNum, lineNum);
		    for (int j = 0; j < par-> B; ++j)
			*(cache + i + j) = blockStartAddr + j;
		    cnt->hit_count++;
		    break;
		}
		else lineNum++;
	    }

	    if (!hit)
	    {
		if (flag) printf("miss ");
		cnt->miss_count++;
		lineNum = 0;
		for (int i = setNum * par->E; i < (setNum + 1) * par->E; i++)
		{
		    if (*(cachePri + i) == -1)
		    {
			for (int j = 0; j < par->B; ++j)
			    *(cache + j + i * par->B) = blockStartAddr + j;
			setPriority(par, cachePri, setNum, lineNum);
			needEvict = false;
			break;
		    }
		    else lineNum++;
		}
	    }

	    if (needEvict)
	    {
	        if (flag) printf("eviction ");
		cnt->eviction_count++;
	        lineNum = 0;
	        for (int i = setNum * par->E; i < (setNum + 1) * par->E; i++)
                {
                    if (*(cachePri + i) == 0)
                    {
                        for (int j = 0; j < par->B; ++j)
                            *(cache + j + i * par->B) = blockStartAddr + j;
                        setPriority(par, cachePri, setNum, lineNum);
			break;
                    }
                    else lineNum++;
                }
	    }

	    if (flag && (oper == 'L' || oper == 'S')) printf("\n");
	    else if (oper == 'M') 
	    {
		cnt->hit_count++;
		if (flag) printf("hit\n");
	    }
	}
    }

    fclose(src);
    free(buffer);
    free(cache);
    free(cachePri);
}

int main(int argc, char *argv[])
{
    // initialize part
    count cnt1;
    count *cnt = &cnt1;
    parameter par1;
    parameter *par = &par1;
    cnt->hit_count = 0;
    cnt->miss_count = 0;
    cnt->eviction_count = 0;
    
    // with -v/-h
    if (argc == 10)
    {
	if (!strcmp(argv[1], "-v") && !strcmp(argv[2], "-s") && !strcmp(argv[4], "-E") && !strcmp(argv[6], "-b") && !strcmp(argv[8], "-t")) 
	{
	    par->S = power((int)argv[3][0] - (int)'0');
	    par->E = (int)argv[5][0] - (int)'0';
	    par->B = power((int)argv[7][0] - (int)'0');
	    strcpy(par->fileName, argv[9]);
	    cacheSim(par, cnt, true);
	}
	else if (!strcmp(argv[1], "-h")) showInfo();
	else showWrongInfo();
    }

    else if (argc == 9 && !strcmp(argv[1], "-s") && !strcmp(argv[3], "-E") && !strcmp(argv[5], "-b") && !strcmp(argv[7], "-t"))
    {
	par->S = power((int)argv[2][0] - (int)'0');
	par->E = (int)argv[4][0] - (int)'0';
	par->B = power((int)argv[6][0] - (int)'0');
	strcpy(par->fileName, argv[8]);
	cacheSim(par, cnt, false);
    }
    
    else showWrongInfo();
	
   // printf("S %d, E %d, B %d, filename %s.\n", par->S, par->E, par->B, par->fileName);

    printSummary(cnt->hit_count, cnt->miss_count, cnt->eviction_count);
    return 0;
}
