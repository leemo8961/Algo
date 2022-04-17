#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include "sort.h"

int* GenerateRandomNumbers(int count)
{
	int* arr = (int*)malloc(sizeof(int) * count);

	for (int i = 0; i < count; i++)
	{
		arr[i] = rand();
	}

	return arr;
}

void PrintResult(int* arr, int size)
{
	for (int i = 0; i < size; i++)
	{
		printf("%d ", arr[i]);
	}
	printf("\n");
}

void Swap(int* arr, int i, int j)
{
	int temp = arr[i];
	arr[i] = arr[j];
	arr[j] = temp;
}

void ExchangeSort(int* arr, int size)
{
	for (int i = 0; i < size - 1; i++)
	{
		for (int j = i + 1; j < size; j++)
		{
			if (arr[i] > arr[j])
			{
				Swap(arr, i, j);
			}
		}
	}
}

void MergeSort(int* arr, int size)
{
	if (size > 1)
	{
		const int L = size / 2;
		const int R = size - L;

		int* left = (int*)malloc(sizeof(int) * L);
		int* right = (int*)malloc(sizeof(int) * R);

		for (int i = 0; i < L; i++)
		{
			left[i] = arr[i];
		}

		for (int i = 0; i < R; i++)
		{
			right[i] = arr[L + i];
		}

		MergeSort(left, L);
		MergeSort(right, R);
		Merge(arr, left, right, L, R);

		free(left);
		free(right);
	}
}

void Merge(int* arr, const int* left, const int* right, int LSize, int RSize)
{
	int i = 0, j = 0, k = 0;

	while (i < LSize && j < RSize)
	{
		if (left[i] < right[j])
		{
			arr[k] = left[i];
			i++;
		}
		else
		{
			arr[k] = right[j];
			j++;
		}
		k++;
	}

	while (i < LSize)
	{
		arr[k] = left[i];
		k++;
		i++;
	}

	while (j < RSize)
	{
		arr[k] = right[j];
		k++;
		j++;
	}
}

void QuickSort(int* arr, int low, int high)
{
	if (low < high)
	{
		int Pivot = Partition(arr, low, high);
		QuickSort(arr, low, Pivot);
		QuickSort(arr, Pivot + 1, high);
	}
}

int Partition(int* arr, int low, int high)
{
	int pivotItem = arr[low];
	int pivotIndex = low;

	for (int i = low + 1; i < high; i++)
	{
		if (arr[i] < pivotItem)
		{
			pivotIndex++;
			Swap(arr, i, pivotIndex);
		}
	}
	if (low != pivotIndex)
		Swap(arr, low, pivotIndex);

	return pivotIndex;
}

void HeapSort(int* arr, int size)
{
	int* heap = (int*)malloc(sizeof(int) * size);
	for (int i = 0; i < size; i++)
	{
		heap[i] = arr[i];
	}
	MakeHeap(heap, size);
	RemoveKey(arr, heap, size);
	free(heap);
}

void MakeHeap(int* heap, int size)
{
	for (int i = size / 2; i >= 0; i--)
		ShiftDown(heap, i, size);
}

void ShiftDown(int* heap, int index, int size)
{
	int pIndex = index;
	int pValue = heap[index];
	bool isMax = false;

	while (2 * pIndex + 1 < size && !isMax)
	{
		int cIndex;

		if (2 * pIndex + 2 < size)
			cIndex = (heap[2 * pIndex + 1] > heap[2 * pIndex + 2]) ? 2 * pIndex + 1 : 2 * pIndex + 2;
		else
			cIndex = 2 * pIndex + 1;

		if (pValue < heap[cIndex])
		{
			heap[pIndex] = heap[cIndex];
			pIndex = cIndex;
		}
		else
			isMax = true;
	}
	heap[pIndex] = pValue;
}

void RemoveKey(int* arr, int* heap, int size)
{
	int heapSize = size;
	for (int i = size - 1; i >= 0; i--)
	{
		arr[i] = Root(heap, &heapSize);
	}
}

int Root(int* heap, int* heapSize)
{
	int maxValue = heap[0];
	*heapSize -= 1;
	heap[0] = heap[*heapSize];
	ShiftDown(heap, 0, *heapSize);
	return maxValue;
}

void RadixSort(int* arr, int size)
{
	struct Node* head = NULL;
	int count = GetMaxCount(arr, size);
	head = GenerateList(head);

	for (int i = 1; i <= count; i++)
	{
		head = Distribute(arr, size, head, i);
		Coalesce(arr, size, head);
	}
}

int GetMaxCount(int* arr, int size)
{
	int max = arr[0];
	for (int i = 1; i < size; i++)
	{
		if (arr[i] > max)
			max = arr[i];
	}

	int count = 0;

	while (max != 0)
	{
		max = max / 10;
		count++;
	}

	return count;
}

struct Node* GenerateList(struct Node* head)
{
	head = NULL;

	struct Node* p = NULL;
	for (int i = 0; i < 10; i++) //
	{
		struct Node* node = malloc(sizeof(struct Node));

		if (head == NULL)
		{
			head = p = node;
		}
		else
		{
			p->_next = node;
			p = p->_next;
		}
		p->_data = i;
		p->_next = NULL;
		p->_child = NULL;
	}
	return head;
}

struct Node* Distribute(int* arr, int size, struct Node* list, int digit)
{
	int div = pow(10, digit - 1);

	struct Node* p = list;
	for (int i = 0; i < size; i++)
	{
		struct Node* node = malloc(sizeof(struct Node));

		node->_data = arr[i];
		node->_next = NULL;
		node->_child = NULL;

		int index = (node->_data / div) % 10;

		for (int i = 0; i < index; i++)
		{
			p = p->_next;
		}

		if (p->_child == NULL)
		{
			p->_child = node;
		}
		else
		{
			while (p->_child != NULL)
				p = p->_child;
			p->_child = node;
		}

		p = list;
	}

	return list;
}

void Coalesce(int* arr, int* size, struct Node* list)
{
	int count = 0;
	struct Node* p = list;

	for (int i = 0; i < 10; i++)
	{
		struct Node* c = p;
		while (c->_child != NULL)
		{
			c = c->_child;
			arr[count] = c->_data;
			count++;
		}

		p->_child = NULL;
		p = p->_next;

		if (count >= size)
			break;
	}
}