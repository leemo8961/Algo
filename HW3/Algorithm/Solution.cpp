#include <iostream>
#include <stack>
using namespace std;

#define SIZE 6
#define INF 999999

typedef enum Vertex
{
	A, B, C, D, E, F
};

char vertex[SIZE] = { 'A', 'B', 'C', 'D', 'E', 'F' };

void printResult(const int start, const int dist[SIZE], const int path[SIZE])
{
	stack<int> stack;
	cout << "Src->Dst   Dist   Path" << endl;
	for (int i = 0; i < SIZE; i++)
	{
		if (start == i)
			continue;

		cout << "  " << vertex[start] << "->" << vertex[i] << "      " << dist[i] << "     " << vertex[start] << " ";

		int tmp = i;
		while (path[tmp] != start)
		{
			stack.push(tmp);
			tmp = path[tmp];
		}
		stack.push(tmp);

		while (!stack.empty())
		{
			int idx = stack.top();
			stack.pop();
			cout << vertex[idx] << " ";
		}
		cout << endl;
	}
}

void Dijkstra(const int start, const int graph[SIZE][SIZE])
{
	int dist[SIZE];
	int path[SIZE];
	bool found[SIZE];

	fill(dist, dist + SIZE, INF);
	fill(path, path + SIZE, 0);
	fill(found, found + SIZE, false);
	
	dist[start] = 0;
	path[start] = start;

	while (true)
	{
		int shortest = INF;
		int currVertex = -1;

		for (int i = 0; i < SIZE; i++)
		{
			if (found[i])
				continue;

			if (dist[i] == INF || dist[i] >= shortest)
				continue;

			shortest = dist[i];
			currVertex = i;
		}

		if (currVertex == -1)
			break;

		found[currVertex] = true;

		for (int i = 0; i < SIZE; i++)
		{
			if (graph[currVertex][i] == 0)
				continue;

			if (found[i])
				continue;

			int newDist = dist[currVertex] + graph[currVertex][i];
			if (newDist < dist[i])
			{
				dist[i] = newDist;
				path[i] = currVertex;
			}
		}
	}
	printResult(start, dist, path);
}

int main()
{
	int fstCase[SIZE][SIZE] = { {0, 5, 0, 2, 4, 0},
								{5, 0, 3, 3, 0, 0},
								{0, 3, 0, 3, 4, 2},
								{2, 3, 3, 0, 1, 0},
								{4, 0, 4, 1, 0, 2},
								{0, 0, 2, 0, 2, 0} };

	int secCase[SIZE][SIZE] = { {0, 3, 0, 5, 1, 0},
								{3, 0, 7, 9, 0, 0},
								{0, 7, 0, 6, 2, 5},
								{5, 9, 6, 0, 4, 0},
								{1, 0, 2, 4, 0, 8},
								{0, 0, 5, 0, 8, 0} };

	int thdCase[SIZE][SIZE] = { {0, 4, 0, 3, 2, 0},
								{4, 0, 5, 6, 0, 0},
								{0, 5, 0, 4, 3, 3},
								{3, 6, 4, 0, 2, 0},
								{2, 0, 3, 2, 0, 5},
								{0, 0, 3, 0, 5, 0} };

	Vertex start = A;

	cout << "First Case" << endl;
	Dijkstra(start, fstCase);
	cout << endl;

	cout << "Second Case" << endl;
	Dijkstra(start, secCase);
	cout << endl;

	cout << "Third Case" << endl;
	Dijkstra(start, thdCase);
	cout << endl;

	return 0;
}