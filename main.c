#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "sort.h"

int main()
{
	srand((unsigned int)time(NULL));
	int count = 10000;
	int data[10000];
	double time[5] = { 0, 0, 0, 0, 0 };

	clock_t start, end;

	for (int i = 0; i < 5; i++)
	{
		int* copy = GenerateRandomNumbers(count);

		memcpy(data, copy, sizeof(int) * count);
		start = clock();
		ExchangeSort(data, count);
		end = clock();
		time[0] += (double)(end - start) / CLOCKS_PER_SEC;

		memcpy(data, copy, sizeof(int) * count);
		start = clock();
		MergeSort(data, count);
		end = clock();
		time[1] += (double)(end - start) / CLOCKS_PER_SEC;

		memcpy(data, copy, sizeof(int) * count);
		start = clock();
		QuickSort(data, 0, count);
		end = clock();
		time[2] += (double)(end - start) / CLOCKS_PER_SEC;

		memcpy(data, copy, sizeof(int) * count);
		start = clock();
		HeapSort(data, count);
		end = clock();
		time[3] += (double)(end - start) / CLOCKS_PER_SEC;

		memcpy(data, copy, sizeof(int) * count);
		start = clock();
		RadixSort(data, count);
		end = clock();
		time[4] += (double)(end - start) / CLOCKS_PER_SEC;
	}

	printf("Exchange Sort : %fsec\n", time[0] / 5);
	printf("Merge Sort : %fsec\n", time[1] / 5);
	printf("Quick Sort : %fsec\n", time[2] / 5);
	printf("Heap Sort : %fsec\n", time[3] / 5);
	printf("Radix Sort : %fsec\n", time[4] / 5);

	return 0;
}