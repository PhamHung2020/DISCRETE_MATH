#include<iostream>
#include<vector>
#include<fstream>
#include<queue>

std::vector<std::vector<int>> adjList;
std::vector<std::vector<int>> adjListNew;
std::vector<std::queue<int>> path(2);
std::vector<std::vector<int>> distances;

void CalculateDistances(int startVertex)
{
	distances[startVertex][startVertex] = 0;
	int currentVertex = startVertex;
	int count = 0;
	int numOfVertex = adjList.size();
	std::vector<bool> isVisited(numOfVertex, false);
	isVisited[startVertex] = true;

	while (count < numOfVertex - 1)
	{
		for (int i = 0; i < numOfVertex; ++i)
		{
			if (adjList[currentVertex][i] > 0)
			{
				if (distances[startVertex][i] == -1 ||
					distances[startVertex][currentVertex] + adjList[currentVertex][i] < distances[startVertex][i])
				{
					distances[startVertex][i] = distances[startVertex][currentVertex] + adjList[currentVertex][i];
				}
			}
		}

		currentVertex = 0;
		while (isVisited[currentVertex] || distances[startVertex][currentVertex] <= 0) ++currentVertex;
		for (int i = currentVertex + 1; i < numOfVertex; ++i)
		{
			if (distances[startVertex][i] > 0 && distances[startVertex][i] < distances[startVertex][currentVertex] && !isVisited[i])
			{
				currentVertex = i;
			}
		}
		isVisited[currentVertex] = true;
		++count;
	}
}

int Distances(int startVertex, int endVertex)
{
	if (distances[startVertex][endVertex] == -1)
		CalculateDistances(startVertex);
	return distances[startVertex][endVertex];
}

void PrintPath(int source, int destination, const std::vector<int>& parents, std::queue<int>& path)
{
	if (destination == source)
	{
		return;
	}

	PrintPath(source, parents[destination], parents, path);
	path.push(destination);
}

bool FindPath(int source, int destination, std::queue<int>& path)
{
	if (source == destination)
		return true;
	
	std::vector<int> parents(adjList.size(), -1);
	std::queue<int> q;
	q.push(source);
	parents[source] = 0;

	while (!q.empty())
	{
		int currentVertex = q.front();
		for (int i = 0; i < adjList.size(); ++i)
		{
			if (adjListNew[currentVertex][i] > 0 && parents[i] == -1)
			{
				q.push(i);
				parents[i] = currentVertex;

				if (i == destination)
					break;
			}
		}

		if (q.back() == destination)
			break;
		q.pop();
	}

	if (q.empty())
	{
		std::cout << "Path not found";
		return false;
	}

	PrintPath(source, q.back(), parents, path);
	return true;
}

void Move(const std::vector<int>& start, const std::vector<int>& end, int dis)
{
	if (Distances(end[0], end[1]) <= dis)
	{
		std::cout << "Impossible";
		return;
	}

	std::cout << start[0] << ' ' << start[1] << ' ' << Distances(start[0], start[1]) << '\n';

	if (start[0] == end[0] && start[1] == end[1])
		return;

	int currentRobot = 0;
	int otherRobot = 1;
	if (start[0] == end[0])
	{
		currentRobot = 1;
		otherRobot = 0;
	};

	std::vector<int> currentPos{ start[0], start[1] };
	while (currentPos[0] != end[0] || currentPos[1] != end[1])
	{
		if (currentPos[currentRobot] == end[currentRobot])
		{
			int oldCurrentPos = currentPos[currentRobot];
			for (int i = 0; i < adjList.size(); ++i)
			{
				if (adjListNew[currentPos[currentRobot]][i] > 0 && Distances(i, path[otherRobot].front()) > dis && Distances(i, currentPos[otherRobot]) > dis)
				{
					currentPos[currentRobot] = i;
					std::cout << currentPos[0] << ' ' << currentPos[1] << ' '  << Distances(currentPos[currentRobot], currentPos[otherRobot]) << '\n';
					break;
				}
			}
			if (currentPos[currentRobot] == oldCurrentPos)
			{
				std::cout << "Impossible";
				return;
			}
		}
		else
		{
			if (path[currentRobot].empty())
			{
				FindPath(currentPos[currentRobot], end[currentRobot], path[currentRobot]);
			}
			else if (Distances(path[currentRobot].front(), currentPos[otherRobot]) <= dis)
			{
				int nextPos = path[currentRobot].front();
				std::queue<int> emptyPath;
				path[currentRobot].swap(emptyPath);
				adjListNew[currentPos[currentRobot]][nextPos] = 0;
				adjListNew[nextPos][currentPos[currentRobot]] = 0;
				if (!FindPath(currentPos[currentRobot], end[currentRobot], path[currentRobot]))
				{
					std::cout << "Impossible";
					return;
				}
			}
			
			while (!path[currentRobot].empty())
			{
				if (Distances(path[currentRobot].front(), currentPos[otherRobot]) <= dis)
					break;
				currentPos[currentRobot] = path[currentRobot].front();
				path[currentRobot].pop();
				std::cout << currentPos[0] << ' ' << currentPos[1] << ' ' << Distances(currentPos[currentRobot], currentPos[otherRobot]) << '\n';
			}
		}

		currentRobot = currentRobot ^ 1;
		otherRobot = otherRobot ^ 1;
	}
}

int main()
{
	int numOfVertex;
	int numOfEdge;
	int a, b, c = 0, d = 0, r;

	std::ifstream infile("INPUT.txt");
	if (infile.fail())
	{
		std::cout << "Cant open input file";
		exit(-1);
	}

	infile >> numOfVertex >> numOfEdge;

	adjList.resize(numOfVertex, std::vector<int>());
	adjListNew.resize(numOfVertex, std::vector<int>());
	for (int i = 0; i < numOfVertex; ++i)
	{
		adjList[i].resize(numOfVertex, -1);
		adjListNew[i].resize(numOfVertex, -1);
	}


	for (int i = 0; i < numOfEdge; ++i)
	{
		infile >> a >> b >> r;
		adjList[a][b] = r;
		adjList[b][a] = r;
		adjListNew[a][b] = r;
		adjListNew[b][a] = r;
	}

	distances.resize(numOfVertex, std::vector<int>(numOfVertex, -1));
	
	std::vector<int> start(2);
	std::vector<int> end(2);

	std::cout << "Start 1: ";
	std::cin >> start[0];
	std::cout << "Start 2: ";
	std::cin >> start[1];
	std::cout << "End 1: ";
	std::cin >> end[0];
	std::cout << "End 2: ";
	std::cin >> end[1];
	
	int dis;
	std::cout << "Distance : ";
	std::cin >> dis;
	Move(start, end, dis);
		
	return 0;
}