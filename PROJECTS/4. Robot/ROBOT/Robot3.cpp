#include<iostream>
#include<vector>
#include<fstream>
#include<queue>

// adjacent list, used for calculating distances
std::vector<std::vector<int>> adjList;
// another adjacent list, used for finding path
std::vector<std::vector<int>> adjListNew;
// path for each robot
std::vector<std::queue<int>> path(2);
// 2-dismention array, saves distances between 2 vetices
std::vector<std::vector<int>> distances;
// final path for robots
std::vector<std::vector<int>> journey(2, std::vector<int>());

// Set up some arrays for program
void SetUp(int numOfVertex)
{
	adjList.resize(numOfVertex, std::vector<int>());
	adjListNew.resize(numOfVertex, std::vector<int>());
	for (int i = 0; i < numOfVertex; ++i)
	{
		adjList[i].resize(numOfVertex, -1);
		adjListNew[i].resize(numOfVertex, -1);
	}
	distances.resize(numOfVertex, std::vector<int>(numOfVertex, -1));
}
// Calculate distances between startVertex and all the others
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
// Return distance between startVertex and endVertex
int Distances(int startVertex, int endVertex)
{
	if (distances[startVertex][endVertex] == -1)
		CalculateDistances(startVertex);
	return distances[startVertex][endVertex];
}
// Complete path after finding it
void CompletePath(int source, int destination, const std::vector<int>& parents, std::queue<int>& path)
{
	if (destination == source)
		return;
	
	CompletePath(source, parents[destination], parents, path);
	path.push(destination);
}
// Find path from source to destination by using BFS
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
		return false;
	CompletePath(source, q.back(), parents, path);
	return true;
}
// Check if distance between vertex1 and vertex2 <= distance in input
bool CheckPos(int vertex1, int vertex2, int disMin)
{
	return (Distances(vertex1, vertex2) > disMin);
}
// Check and delete duplicate moves
void CheckDuplicateMove(int currentRobot, int currentPos[])
{
	int length = journey[0].size();
	int otherRobot = currentRobot ^ 1;
	int deletePos = -1;
	for (int j = length - 1; j >= 0; --j)
	{
		if (currentPos[currentRobot] == journey[currentRobot][j])
		{
			if (currentPos[otherRobot] == journey[otherRobot][j])
				deletePos = j;
			else if (adjList[currentPos[otherRobot]][journey[otherRobot][j]] > 0)
				deletePos = j + 1;
			break;
		}
	}
	if (deletePos != -1)
	{
		for (int i = length - 1; i >= deletePos; --i)
        {
            journey[0].pop_back();
            journey[1].pop_back();
        }
	}
	journey[0].emplace_back(currentPos[0]);
	journey[1].emplace_back(currentPos[1]);
}
// Find path for robots
void Move(int start[], int end[], int dis)
{
	if (Distances(end[0], end[1]) <= dis)
		return;
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
					CheckDuplicateMove(currentRobot, currentPos);
					break;
				}
			}
			if (i == adjList.size())
				return;
		}
		else
		{
			if (path[currentRobot].empty())
			{
				if (!FindPath(currentPos[currentRobot], end[currentRobot], path[currentRobot]))
					return;
			}
			else if (!CheckPos(path[currentRobot].front(), currentPos[otherRobot], dis))
			{
				int nextPos = path[currentRobot].front();
				// Clear path
				std::queue<int> emptyPath;
				path[currentRobot].swap(emptyPath);
				adjListNew[currentPos[currentRobot]][nextPos] = 0;
				adjListNew[nextPos][currentPos[currentRobot]] = 0;
				if (!FindPath(currentPos[currentRobot], end[currentRobot], path[currentRobot]))
					return;
			}

			while (!path[currentRobot].empty())
			{
				int nextPos = path[currentRobot].front();
				if (Distances(nextPos, currentPos[otherRobot]) <= dis)
					break;
				currentPos[currentRobot] = nextPos;
				CheckDuplicateMove(currentRobot, currentPos);
				path[currentRobot].pop();
			}
		}
		currentRobot ^= 1;
		otherRobot ^= 1;
	}
}
// Print path of robots
void PrintResult(int end[])
{
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

}

int main()
{
	int numOfVertex;
	int numOfEdge;
	int start[2];
	int end[2];
	int distance;
	// Open file
	std::ifstream infile("INPUT.txt");
	if (infile.fail())
	{
		std::cout << "Cant open input file";
		exit(-1);
	}

	infile >> numOfVertex >> numOfEdge;
	// Set up some arrays for program
	SetUp(numOfVertex);
	// Start reading input
	int a, b, r;
	for (int i = 0; i < numOfEdge; ++i)
	{
		infile >> a >> b >> r;
		adjList[a][b] = r;
		adjList[b][a] = r;
		adjListNew[a][b] = r;
		adjListNew[b][a] = r;
	}
	infile >> start[0] >> start[1] >> end[0] >> end[1] >> distance;
	infile.close();
	// Find path for robots and print to screen
	Move(start, end, distance);
	PrintResult(end);

	return 0;
}