#pragma once

int* GenerateRandomNumbers(int count);

void PrintResult(int* arr, int size);

void Swap(int* arr, int i, int j);

void ExchangeSort(int* arr, int size);

void MergeSort(int* arr, int size);
void Merge(int* arr, const int* left, const int* right, int LSize, int RSize);

void QuickSort(int* arr, int low, int high);
int Partition(int* arr, int low, int high);

void HeapSort(int* arr, int size);
void MakeHeap(int* heap, int size);
void ShiftDown(int* heap, int index, int size);
void RemoveKey(int* arr, int* heap, int size);
int Root(int* heap, int* heapSize);

struct Node
{
	int _data;
	struct Node* _next;
	struct Node* _child;
};
void RadixSort(int* arr, int size);
int GetMaxCount(int* arr, int size);
struct Node* GenerateList(struct Node* head);
struct Node* Distribute(int* arr, int size, struct Node* list, int digit);
void Coalesce(int* arr, int* size, struct Node* list);