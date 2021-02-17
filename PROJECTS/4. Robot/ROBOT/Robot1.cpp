#include<iostream>
#include<vector>
#include<fstream>
#include<queue>

std::vector<std::vector<int>> adjList;
std::vector<std::vector<int>> adjListNew;
std::queue<int> path1;
std::queue<int> path2;
std::vector<std::queue<int>> path(2);

int Distance(int startVertex, int endVertex)
{
	if (startVertex == endVertex)
		return 0;

	std::vector<int> distance(adjList.size(), -1);
	distance[startVertex] = 0;
	int currentVertex = startVertex;

	while (currentVertex != endVertex)
	{
		for (int i = 0; i < adjList.size(); ++i)
		{
			if (adjList[currentVertex][i] > 0)
			{
				if (distance[i] == -1 || distance[currentVertex] + adjList[currentVertex][i] < distance[i])
				{
					distance[i] = distance[currentVertex] + adjList[currentVertex][i];
				}
			}
		}

		distance[currentVertex] = 0;
		
		currentVertex = 0;
		while (distance[currentVertex] <= 0) ++currentVertex;
		for (int i = currentVertex + 1; i < adjList.size(); ++i)
		{
			if (distance[i] > 0 && distance[i] < distance[currentVertex])
			{
				currentVertex = i;
			}
		}
	}

	return distance[currentVertex];
}

void PrintPath(int source, int destination, const std::vector<int>& parents, std::queue<int>& path)
{
	if (destination == source)
	{
		std::cout << destination << ' ';
		//path.push(destination);
		return;
	}

	PrintPath(source, parents[destination], parents, path);
	std::cout << destination << ' ';
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
			if (adjListNew[currentVertex][i] > 0)
			{
				if (parents[i] == -1)
				{
					q.push(i);
					parents[i] = currentVertex;
				}
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
	if (Distance(end[0], end[1]) <= dis)
	{
		std::cout << "Impossible";
		return;
	}

	std::cout << start[0] << ' ' << start[1] << ' ' << Distance(start[0], start[1]) << '\n';

	int currentRobot = 0;
	int otherRobot = 1;

	if (start[0] == end[0] && start[1] == end[1])
		return;
	else if (start[0] == end[0])
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
				if (adjListNew[currentPos[currentRobot]][i] > 0 && Distance(i, path[otherRobot].front()) > dis && Distance(i, currentPos[otherRobot]) > dis)
				{
					currentPos[currentRobot] = i;
					std::cout << currentPos[0] << ' ' << currentPos[1] << ' '  << Distance(currentPos[currentRobot], currentPos[otherRobot]) << '\n';
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
				std::cout << "Find path ";
				std::cout << currentPos[currentRobot] << ' ';
				std::queue<int> q = path[currentRobot];
				while (!q.empty())
				{
					std::cout << q.front() << ' ';
					q.pop();
				}
				std::cout << '\n';

			}
			else if (Distance(path[currentRobot].front(), currentPos[otherRobot]) <= dis)
			{
				int nextPos = path[currentRobot].front();
				std::queue<int> emptyPath;
				path[currentRobot].swap(emptyPath);
				int oldDis = adjList[currentPos[currentRobot]][nextPos];
				std::cout << "(" << currentPos[currentRobot] << ", " << nextPos << ") ";
				adjListNew[currentPos[currentRobot]][nextPos] = 0;
				adjListNew[nextPos][currentPos[currentRobot]] = 0;
				if (!FindPath(currentPos[currentRobot], end[currentRobot], path[currentRobot]))
				{
					std::cout << "Impossible";
					return;
				}
				adjListNew[currentPos[currentRobot]][nextPos] = oldDis;
				adjListNew[nextPos][currentPos[currentRobot]] = oldDis;
				std::cout  << "Find new path : ";
				std::cout << currentPos[currentRobot] << ' ';
				std::queue<int> q = path[currentRobot];
				while (!q.empty())
				{
					std::cout << q.front() << ' ';
					q.pop();
				}
				std::cout << '\n';
			}
			
			/*while (!path[currentRobot].empty())
			{
				std::cout << path[currentRobot].front() << ' ';
				path[currentRobot].pop();
			}
			break;*/
			while (!path[currentRobot].empty())
			{
				if (Distance(path[currentRobot].front(), currentPos[otherRobot]) <= dis)
					break;
				currentPos[currentRobot] = path[currentRobot].front();
				path[currentRobot].pop();
				std::cout << currentPos[0] << ' ' << currentPos[1] << ' ' << Distance(currentPos[currentRobot], currentPos[otherRobot]) << '\n';
			}
		}

		

		if (currentRobot == 0)
		{
			currentRobot = 1;
			otherRobot = 0;
		}
		else
		{
			currentRobot = 0;
			otherRobot = 1;
		}
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

	
	//while (true)
	//{
	//	adjList[2][3] = 0;
	//	adjList[3][2] = 0;
	//	adjList[0][1] = 0;
	//	adjList[1][0] = 0;
	//	int startVertex, endVertex;
	//	std::cout << "Start vertex : ";
	//	std::cin >> startVertex;
	//	if (startVertex < 0)
	//		break;
	//	std::cout << "End vertex : ";
	//	std::cin >> endVertex;
	//	if (endVertex < 0)
	//		break;
	//	//std::cout << Distance(startVertex, endVertex) << '\n';
	//	FindPath(startVertex, endVertex, path1);
	//	while (!path1.empty())
	//	{
	//		std::cout << path1.front() << ' ';
	//		path1.pop();
	//	}
	//	std::cout << '\n';
	//}

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
	
	/*for (int i = 0; i < adjList.size(); ++i)
	{
		std::cout << adjList[4][i] << ' ';
	}*/
	
	return 0;
}