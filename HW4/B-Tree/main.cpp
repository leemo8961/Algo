#include <iostream>
#include <stdlib.h>]
#include <time.h>
using namespace std;

typedef struct Node
{
	int*   _data;
	Node** _childPtr;
	int    _size;
	bool   _isLeaf;
} Node;

Node* root = NULL;
int degree = -1;

Node* InitNode(Node* node);
void SplitNode(Node* node, bool IsLeaf);
void InsertAtSubTree(Node* node, int target);
void InsertAtRoot(int target);
int SearchData(Node* node, int target);
void DeleteAtLeaf(Node* node, int pos);
int FindPredecessor(Node* node);
int FindSuccessor(Node* node);
void Merge(Node* node, int pos, int caseNum);
void TakeLeftSibling(Node* node, int pos);
void TakeRightSibling(Node* node, int pos);
void DeleteAtInternalNode(Node* node, int pos);
void Delete(Node* node, int target);

Node* InitNode(Node* node)
{
	node = new Node;
	node->_data = new int[2 * degree - 1];

	node->_childPtr = new Node * [2 * degree];
	for (int i = 0; i < 2 * degree; i++)
	{
		node->_childPtr[i] = NULL;
	}

	node->_size = 0;
	node->_isLeaf = true;

	return node;
}

void SplitNode(Node* node, bool IsLeaf)
{
	Node* lChild = NULL; 
	Node* rChild = NULL;
	
	lChild = InitNode(lChild);
	rChild = InitNode(rChild);
	
	// data 처리
	for (int i = 0; i < 2 * degree - 1; i++)
	{
		int mid = degree - 1;

		if (i < mid)
		{
			lChild->_data[lChild->_size] = node->_data[i];
			lChild->_size++;

			node->_data[i] = -1; // 범위가 0~9999 이므로 해당하지 않는 값
			node->_size--;
		}
		else if (i == mid)
		{
			node->_data[0] = node->_data[mid];
			node->_data[mid] = -1;
		}
		else
		{
			rChild->_data[rChild->_size] = node->_data[i];
			rChild->_size++;

			node->_data[i] = -1; // 범위가 0~9999 이므로 해당하지 않는 값
			node->_size--;
		}
	}

	// childPtr 처리
	for (int i = 0; i < 2 * degree; i++)
	{
		int mid = degree;

		if (i < mid)
			lChild->_childPtr[i] = node->_childPtr[i];
		else
			rChild->_childPtr[i] = node->_childPtr[i];

		node->_childPtr[i] = NULL;
	}

	node->_childPtr[0] = lChild;
	node->_childPtr[1] = rChild;

	// isLeaf 처리
	if (!IsLeaf)
	{
		lChild->_isLeaf = false;
		rChild->_isLeaf = false;
	}
	node->_isLeaf = false;
}

void InsertAtSubTree(Node* node, int target)
{
	int full = 2 * degree - 1;
	
	if (node->_isLeaf == true)
	{
		int i = node->_size - 1;
		while (i >= 0 && target < node->_data[i])
		{
			node->_data[i + 1] = node->_data[i];
			i--;
		}

		node->_data[i + 1] = target;
		node->_size++;
	}
	else
	{
		int i = node->_size;
		while (i >= 0 && target < node->_data[i])
			i--;

		if (node->_childPtr[i]->_size == full) // child node가 가득 차 있는 경우
		{
			SplitNode(node->_childPtr[i], node->_childPtr[i]->_isLeaf);

			// 분할된 자식과 병합
			int dataIndex = node->_size - 1;
			int ptrIndex = node->_size;
			while (dataIndex > 0 && node->_childPtr[i]->_data[0] < node->_data[dataIndex])
			{
				node->_data[dataIndex + 1] = node->_data[dataIndex];
				node->_childPtr[ptrIndex + 1] = node->_childPtr[ptrIndex];
				dataIndex--;
				ptrIndex--;
			}

			node->_data[dataIndex] = node->_childPtr[i]->_data[0];
			node->_size++;
			node->_childPtr[ptrIndex] = node->_childPtr[i]->_childPtr[1];
			node->_childPtr[ptrIndex - 1] = node->_childPtr[i]->_childPtr[0];
			
			
			if (target > node->_data[i+1]) // 분할로 인해 새로 추가된 데이터보다 target이 큰 경우 인덱스 다시 1 증가
				i++;

			InsertAtSubTree(node->_childPtr[i], target);
		}
	}
}

void InsertAtRoot(int target)
{
	int full = 2 * degree - 1;

	if (root == NULL) // root가 없는 경우
	{
		root = InitNode(root);
		root->_data[0] = target;
		root->_size++;
	}
	else if (root->_size == full) // root가 가득 찬 경우
	{
		SplitNode(root, root->_isLeaf);

		int i = (root->_data[0] > target) ? 0 : 1;
			
		InsertAtSubTree(root->_childPtr[i], target); // target의 값에 따라 root가 가리키는 왼쪽 자식이나 오른쪽 자식에서 Insert 시작
	}
	else // root가 가득 차지 않는 경우
	{
		InsertAtSubTree(root, target); // root부터 leaf인지 차례대로 탐색해야하므로 인자로 root를 전달
	}
}

int SearchData(Node* node, int target)
{
	int i = 0;

	while (i < node->_size && node->_data[0] < target)
		i++;

	return i;
}

void DeleteAtLeaf(Node* node, int pos)
{
	for (int i = pos + 1; i < node->_size; i++)
		node->_data[i - 1] = node->_data[i];

	node->_size--;
}

int FindPredecessor(Node* node)
{
	Node* ret = node;
	while (!ret->_isLeaf)
		ret = ret->_childPtr[ret->_size];

	return ret->_data[ret->_size - 1];
}

int FindSuccessor(Node* node)
{
	Node* ret = node;
	while (!ret->_isLeaf)
		ret = ret->_childPtr[0];

	return ret->_data[0];
}

void Merge(Node* node, int pos, int caseNum)
{
	Node* newNode = NULL;
	newNode = InitNode(newNode);
	newNode->_isLeaf = false;

	if (caseNum == 2)
	{
		// Left Child data 처리
		for (int i = 0; i < node->_childPtr[pos]->_size; i++)
		{
			newNode->_data[newNode->_size] = node->_childPtr[pos]->_data[i];
			newNode->_size++;
		}
	}
	else if (caseNum == 3)
	{
		// Left Child data 처리
		for (int i = 0; i < node->_childPtr[pos]->_size; i++)
		{
			newNode->_data[newNode->_size] = node->_childPtr[pos]->_data[i];
			newNode->_size++;
		}

		// Parent data 처리
		newNode->_data[newNode->_size] = node->_data[pos];
		newNode->_size++;		
	}

	// Right Child data 처리
	for (int i = 0; i < node->_childPtr[pos + 1]->_size; i++)
	{
		newNode->_data[newNode->_size] = node->_childPtr[pos]->_data[i];
		newNode->_size++;
	}

	if (node->_childPtr[pos]->_isLeaf) // 높이가 같으므로 한쪽만 체크
	{
		int cnt = 0;

		// Left Child childPtr 처리
		for (int i = 0; i < node->_childPtr[pos]->_size; i++)
		{
			newNode->_childPtr[cnt] = node->_childPtr[pos]->_childPtr[i];
			cnt++;
		}

		// Right Child childPtr 처리
		for (int i = 0; i < node->_childPtr[pos + 1]->_size; i++)
		{
			newNode->_data[cnt] = node->_childPtr[pos]->_data[i];
			cnt++;
		}

		newNode->_isLeaf = true;
	}

	// Parent data 처리
	for (int i = pos + 1; i < node->_size; i++)
		node->_data[i - 1] = node->_data[i];

	// Parent Child 처리
	for (int i = pos + 2; i < node->_size + 1; i++)
		node->_data[i - 1] = node->_data[i];

	node->_childPtr[node->_size] = NULL;

	node->_size--;

	node->_childPtr[pos] = newNode;
	
}

void TakeLeftSibling(Node* node, int pos)
{
	Node* child = node->_childPtr[pos];
	Node* leftSibling = node->_childPtr[pos - 1];

	// Child data 처리
	for (int i = child->_size; i > 0; i--)
	{
		child->_data[i] = child->_data[i - 1];
	}

	child->_size++;

	// Child childPtr 처리
	for (int i = child->_size + 1; i > 0; i--)
	{
		child->_childPtr[i] = child->_childPtr[i - 1];
	}

	// Parent data 처리
	child->_data[0] = node->_data[pos];

	// Left Sibling childPtr 처리
	child->_childPtr[0] = leftSibling->_childPtr[leftSibling->_size];
	leftSibling->_childPtr[leftSibling->_size] = NULL;


	// Left Sibling data 처리
	node->_data[pos] = leftSibling->_data[leftSibling->_size-1];
	
	leftSibling->_size--;
}

void TakeRightSibling(Node* node, int pos)
{
	Node* child = node->_childPtr[pos];
	Node* rightSibling = node->_childPtr[pos + 1];

	// Child data 처리
	child->_data[child->_size] = node->_data[pos];
	child->_size++;

	// Child childPtr 처리
	child->_childPtr[child->_size] = rightSibling->_childPtr[0];

	// Parent data 처리
	node->_data[pos] = rightSibling->_data[0];

	// Right Sibling data 정보 삭제
	for (int i = 1; i < rightSibling->_size; i++)
	{
		rightSibling->_data[i - 1] = rightSibling->_data[i];
	}

	// Right Sibling childPtr 정보 삭제
	for (int i = 1; i < rightSibling->_size + 1; i++)
	{
		rightSibling->_childPtr[i - 1] = rightSibling->_childPtr[i];
	}

	rightSibling->_size--;	
}

void DeleteAtInternalNode(Node* node, int pos)
{
	if (node->_childPtr[pos]->_size >= degree) // Case 2a
	{
		int data = FindPredecessor(node->_childPtr[pos]);
		node->_data[pos] = data;
		Delete(node->_childPtr[pos], data);

	}
	else if (node->_childPtr[pos + 1]->_size >= degree) // Case 2b
	{
		int data = FindSuccessor(node->_childPtr[pos]);
		node->_data[pos] = data;
		Delete(node->_childPtr[pos + 1], data);
	}
	else
	{
		Merge(node, pos, 2); // Case 2c
	}
}

void Delete(Node* node, int target)
{
	int pos = SearchData(node, target);

	if (pos < node->_size && node->_data[pos] == target)
	{
		if (node->_isLeaf) 
			DeleteAtLeaf(node, pos); // Case 1
		else
			DeleteAtInternalNode(node, pos); // Case 2
	}
	else // Case 3
	{
		if (node->_isLeaf) // No key
			return;

		if (node->_childPtr[pos]->_size >= degree)
			Delete(node->_childPtr[pos], target);
		else
		{
			if (pos > 0 && node->_childPtr[pos - 1]->_size >= degree) // Case 3a Left Sibling
			{
				TakeLeftSibling(node, pos);
			}
			else if (pos < node->_size && node->_childPtr[pos + 1]->_size >= degree) // Case 3a Right Sibling
			{
				TakeRightSibling(node, pos);
			}
			else if (pos < node->_size)// Case 3b
			{
				Merge(node, pos, 3);
			}

			Delete(node->_childPtr[pos], target);
		}
	}
}

int main()
{
	int deg[3] = { 5, 7, 9 };

	srand(time(NULL));

	clock_t start, end;

	for (int i = 0; i < 3; i++)
	{
		start = clock();

		root = NULL;
		degree = deg[i];
		for (int j = 0; j < 10000000; j++)
		{
			int target = rand() % 10000;
			InsertAtRoot(target);
		}

		for (int j = 0; j < 5000000; j++)
		{
			int target = rand() % 10000;
			Delete(root, target);
		}

		end = clock();

		cout << "Degree: " << degree << "  " << "Time: " << (double)(end - start) / CLOCKS_PER_SEC << endl;
	}

	return 0;
}

