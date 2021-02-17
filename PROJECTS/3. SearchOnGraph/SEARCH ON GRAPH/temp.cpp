#include<iostream>
#include<fstream>
#include<queue>
#include<string>
#include<vector>
#include<list>

#define NUMOFWORDS 4585

std::vector<std::string> words(NUMOFWORDS);
std::vector<std::vector<int>> adjList(NUMOFWORDS, std::vector<int>());
std::vector<int> alphabet(26, 0);
std::vector<int> parents(NUMOFWORDS, -1);

std::vector<int> preVisit(NUMOFWORDS);
std::vector<int> postVisit(NUMOFWORDS);


void ResetAlphabet()
{
    for (int i = 0; i < 26; ++i)
    {
        alphabet[i] = 0;
    }
}

void Input(std::ifstream& infile)
{
    for(int i = 0; i < NUMOFWORDS; ++i)
    {
        getline(infile, words[i]);
    }
}


bool CheckString(const std::string& a, const std::string& b)
{
    ResetAlphabet();

    for (int i = 0; i < 5; ++i)
    {
        ++alphabet[int(b[i] - 'a')];
    }

    for (int i = 1; i < 5; ++i)
    {
        if (alphabet[int(a[i] - 'a')] > 0)
        {
            --alphabet[int(a[i] - 'a')];
        }
        else
        {
            return false;
        }
    }

    return true;
}

void CreateAdjacentList()
{
    for (int i = 0; i < NUMOFWORDS - 1; ++i)
    {
        for (int j = i + 1; j < NUMOFWORDS; ++j)
        {
            if (CheckString(words[i], words[j]))
            {
                adjList[i].push_back(j);
            }
            if (CheckString(words[j], words[i]))
            {
                adjList[j].push_back(i);
            }
        }
    }
}


void PrintShortestPath(int source, int destination)
{
	if (destination == source)
	{
		std::cout << words[destination] << ' ';
		return;
	}

	PrintShortestPath(source, parents[destination]);
	std::cout << words[destination] << ' ';
}

void FindShortestPath(const std::string& source, const std::string& destination)
{
	int indexOfSource = -1;
	for (int i = 0; i < NUMOFWORDS; ++i)
	{
		if (words[i] == source)
		{
			indexOfSource = i;
			break;
		}
	}

	if (indexOfSource == -1)
	{
		std::cout << "Path not found";
		return;
	}

	std::queue<int> q;
	q.push(indexOfSource);
	while (!q.empty())
	{
		int currentVertex = q.front();
		for (const int& v : adjList[currentVertex])
		{
			if (parents[v] == -1)
			{
				q.push(v);
				parents[v] = currentVertex;
			}
			if (words[v] == destination)
				break;
		}
		if (words[q.back()] == destination)
			break;
		q.pop();
	}
	if (q.empty())
	{
		std::cout << "Path not found";
		return;
	}
	PrintShortestPath(indexOfSource, q.back());
	
}

void PreVisit(int vertex, int& clock)
{
    preVisit[vertex] = clock;
    ++clock;
}

void PostVisit(int vertex, int& clock)
{
    postVisit[vertex] = clock;
    ++clock;
    std::cout << words[vertex] << " : " << clock << '\n';
}

void Explore(int vertex, int& clock, std::vector<bool>& isVisited, const std::vector<std::vector<int>>& adjList)
{
    isVisited[vertex] = true;
    PreVisit(vertex, clock);
    for (const int& v : adjList[vertex])
    {
        if (!isVisited[v])
        {
            Explore(v, clock, isVisited, adjList);
        }
    }
    PostVisit(vertex, clock);
    
}

void DFS(const std::vector<std::vector<int>>& adjList)
{
    std::vector<bool> isVisited(NUMOFWORDS, false);
    int clock = 1;

    for (int i = 0; i < NUMOFWORDS; ++i)
    {
        if (!isVisited[i])
        {
            Explore(i, clock, isVisited, adjList);
        }
    }

    std::cout << "\n\n" << clock;
}

int main()
{
    std::ifstream infile("words2.txt");
    if (infile.fail())
    {
        std::cout << "Cant open file";
        exit(-1);
    }

    Input(infile);
    CreateAdjacentList();

    /*int numOfEdge = 0;
    for (int i = 0; i < NUMOFWORDS; ++i)
    {
        numOfEdge += adjList[i].size();
    }

    std::cout << numOfEdge;*/

    /*for (int i = 0; i < NUMOFWORDS; ++i)
    {
        std::cout << words[i] << " : ";
        for (const int& v : adjList[i])
        {
            std::cout << words[v] << ' ';
        }
        std::cout << '\n';
    }*/

    /*int indexOfSource = -1;
    std::string source = "chars";
    std::string destination;
	for (int i = 0; i < NUMOFWORDS; ++i)
	{
		if (words[i] == source)
		{
			indexOfSource = i;
			break;
		}
	}

    std::cout << indexOfSource;*/

    //FindShortestPath("graph", "words");
    
    DFS(adjList);

    return 0;
}