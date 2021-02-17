#include<iostream>
#include<vector>
#include<fstream>
#include<queue>
#include<list>

std::vector<std::vector<int>> adjList;
std::vector<std::vector<int>> adjListNew;
std::vector<std::queue<int>> path(2);
std::vector<std::vector<int>> distances;
std::vector<std::vector<int>> journey(2, std::vector<int>());


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
		//std::cout << destination << ' ';
		//path.push(destination);
		return;
	}

	PrintPath(source, parents[destination], parents, path);
	//std::cout << destination << ' ';
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

bool CheckPos(int vertex1, int vertex2, int disMin)
{
	return (Distances(vertex1, vertex2) > disMin);
}

void CheckJourney(int currentPos[], int prevPos[], int currentRobot)
{
	int otherRobot = currentRobot ^ 1;
	if (currentPos[currentRobot] == prevPos[currentRobot])
	{
		int pos = 0;
		for (int i = journey[currentRobot].size() - 1; i >= 0; --i)
		{
			if (prevPos[currentRobot] == journey[currentRobot][i])
			{
				pos = journey[otherRobot][i];
				break;
			}
		}
		if (prevPos[otherRobot] == -1 || currentPos[otherRobot] == pos || adjList[currentPos[otherRobot]][pos] > 0)
		{
			while (journey[currentRobot].back() != prevPos[currentRobot])
			{
				journey[0].pop_back();
				journey[1].pop_back();
			}
			for (int i = 0; i <= 1; ++i) // i = 0 and i = 1
			{
				if (journey[i].size())
					prevPos[i] = -1;
				else
					prevPos[i] = journey[i][journey[i].size() - 1];
				currentPos[i] = journey[i].back();
			}
			return;
		}
	}
	
	for (int i = 0; i <= 1; ++i) // i = 0 and i = 1
	{
		if (currentPos[i] != journey[i].back())
			prevPos[i] = journey[i].back();
		journey[i].emplace_back(currentPos[i]);
	}
	
}

void CompleteJourney(int currentRobot, int currentPos[])
{
	int length = journey[0].size();
	int otherRobot = currentRobot ^ 1;

	int deletePos = -1;
	for (int j = length - 1; j >= 0; --j)
	{
		if (currentPos[currentRobot] == journey[currentRobot][j])
		{
			std::cout << "Comlete " << currentPos[currentRobot] << ' ' << journey[currentRobot][j] << '\n';
			if (currentPos[otherRobot] == journey[otherRobot][j])
			{
				deletePos = j;
				std::cout << "Complete " << journey[otherRobot].back() << ' ' << journey[otherRobot][j] << '\n';
			}
			else if (adjList[currentPos[otherRobot]][journey[otherRobot][j]] > 0)
			{
				std::cout << "Complete " << journey[otherRobot].back() << ' ' << journey[otherRobot][j] << '\n';
				deletePos = j + 1;
			}
			break;
		}
	}
	if (deletePos == -1)
	{
		journey[0].emplace_back(currentPos[0]);
		journey[1].emplace_back(currentPos[1]);
		return;
	}
	for (int i = length - 1; i >= deletePos; --i)
	{
		std::cout << "Delete " << journey[0].back() << ' ' << journey[1].back() << '\n';
		journey[0].pop_back();
		journey[1].pop_back();
	}
	
	journey[0].emplace_back(currentPos[0]);
	journey[1].emplace_back(currentPos[1]);
	
	
}

void Move(int start[], int end[], int dis)
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

	int currentPos[2] = { start[0], start[1] };
	journey[0].emplace_back(start[0]);
	journey[1].emplace_back(start[1]);
	int prevPos[2] = { -1, -1 };
	while (currentPos[0] != end[0] || currentPos[1] != end[1])
	{
		if (currentPos[currentRobot] == end[currentRobot])
		{
			int i;
			for (i = 0; i < adjList.size(); ++i)
			{
				if (adjListNew[currentPos[currentRobot]][i] > 0 && CheckPos(i, path[otherRobot].front(), dis) && CheckPos(i, currentPos[otherRobot], dis))
				{
					currentPos[currentRobot] = i;
					std::cout << currentPos[0] << ' ' << currentPos[1] << ' ' << Distances(currentPos[currentRobot], currentPos[otherRobot]) << " finish\n";
					CompleteJourney(currentRobot, currentPos);
					std::cout << currentPos[0] << ' ' << currentPos[1] << ' ' << Distances(currentPos[currentRobot], currentPos[otherRobot]) << " finish\n";
					//journey[0].emplace_back(currentPos[0]);
					//journey[1].emplace_back(currentPos[1]);
					//CheckJourney(currentPos, prevPos, currentRobot);
					//std::cout << currentPos[0] << ' ' << currentPos[1] << ' ' << Distances(currentPos[currentRobot], currentPos[otherRobot]) << " finish\n";
					break;
				}
			}
			if (i == adjList.size())
			{
				std::cout << "Impossible";
				return;
			}
		}
		else
		{
			if (path[currentRobot].empty())
			{
				if (!FindPath(currentPos[currentRobot], end[currentRobot], path[currentRobot]))
				{
					std::cout << "Impossible path";
					return;
				}
				std::cout << "###Find path ";
				std::cout << currentPos[currentRobot] << ' ';
				std::queue<int> q = path[currentRobot];
				while (!q.empty())
				{
					std::cout << q.front() << ' ';
					q.pop();
				}
				std::cout << '\n';

			}
			else if (!CheckPos(path[currentRobot].front(), currentPos[otherRobot], dis))
			{
				int nextPos = path[currentRobot].front();
				std::queue<int> emptyPath;
				path[currentRobot].swap(emptyPath);
				//int oldDis = adjList[currentPos[currentRobot]][nextPos];
				std::cout << "(" << currentPos[currentRobot] << ", " << nextPos << ") ";
				adjListNew[currentPos[currentRobot]][nextPos] = 0;
				adjListNew[nextPos][currentPos[currentRobot]] = 0;
				if (!FindPath(currentPos[currentRobot], end[currentRobot], path[currentRobot]))
				{
					std::cout << "Impossible new path";
					return;
				}
				//adjListNew[currentPos[currentRobot]][nextPos] = oldDis;
				//adjListNew[nextPos][currentPos[currentRobot]] = oldDis;
				std::cout << "###Find new path : ";
				std::cout << currentPos[currentRobot] << ' ';
				std::queue<int> q = path[currentRobot];
				while (!q.empty())
				{
					std::cout << q.front() << ' ';
					q.pop();
				}
				std::cout << '\n';
			}

			while (!path[currentRobot].empty())
			{
				int nextPos = path[currentRobot].front();
				if (Distances(nextPos, currentPos[otherRobot]) <= dis)
					break;
				currentPos[currentRobot] = nextPos;
				std::cout << currentPos[0] << ' ' << currentPos[1] << ' ' << Distances(currentPos[currentRobot], currentPos[otherRobot]) << " before\n";
				CompleteJourney(currentRobot, currentPos);
				//CheckJourney(currentPos, prevPos, currentRobot);
				//journey[0].emplace_back(currentPos[0]);
				//journey[1].emplace_back(currentPos[1]);
				path[currentRobot].pop();
				std::cout << currentPos[0] << ' ' << currentPos[1] << ' ' << Distances(currentPos[currentRobot], currentPos[otherRobot]) << '\n';

			}
		}

		currentRobot ^= 1;
		otherRobot ^= 1;
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
	
	
	int start[2];
	int end[2];

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

	std::cout << "\n\n\n";
	if (journey[0].back() != end[0] || journey[1].back() != end[1])
	{
		std::cout << "Impossible";
	}
	else
	{
		int length = journey[0].size();
		for (int i = 0; i < length; ++i)
		{
			std::cout << journey[0][i] << ' ' << journey[1][i] << ' ' << Distances(journey[0][i], journey[1][i]) << '\n';
		}
	}

	
	/*for (int i = 0; i < adjList.size(); ++i)
	{
		std::cout << adjList[4][i] << ' ';
	}*/

	
	return 0;
}