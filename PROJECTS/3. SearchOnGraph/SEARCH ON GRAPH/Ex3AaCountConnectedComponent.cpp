// This program reads input from file words.txt

#include<iostream>
#include<fstream>
#include<string>
#include<vector>

//#define NUMOFWORDS 5757


int NUMOFWORDS;
// This array saves all words
std::vector<std::string> words;
// Adjacent list, but uses index (int) of word in words array instead of using directly word (string)
std::vector<std::vector<int>> adjList;

// Read all words from file input (infile) to words array
void Input(std::ifstream& infile)
{
	std::string word;
	while (!infile.eof())
	{
		getline(infile, word);
		words.emplace_back(word);
	}
	NUMOFWORDS = words.size();
}

// Check if string a is different from string b at only 1 position then return true; if not, return false
bool CheckString(const std::string& a, const std::string& b)
{
	int diff = 0;
	for (int i = 0; i < 5; ++i)
	{
		if (a[i] != b[i])
			++diff;

		if (diff > 1)
			return false;
	}

	if (diff == 0)
		return false;

	return true;
}

// Create adjacent list from words array
void CreateAdjacentList()
{
	adjList.resize(NUMOFWORDS, std::vector<int>());
	for (int i = 0; i < NUMOFWORDS - 1; ++i)
	{
		for (int j = i + 1; j < NUMOFWORDS; ++j)
		{
			if (CheckString(words[i], words[j]))
			{
				adjList[i].emplace_back(j);
				adjList[j].emplace_back(i);
			}
		}
	}
}

void Explore(int vertex, std::vector<bool>& isVisited)
{
	isVisited[vertex] = true;
	for (const int& v : adjList[vertex])
	{
		if (!isVisited[v])
		{
			Explore(v, isVisited);
		}
	}
}

// Use DFS to count and return the number of connected component in the graph
int CountConnectedComponentByDFS()
{
	std::vector<bool> isVisited(NUMOFWORDS, false);
	int connectedComponent = 0;
	for (int i = 0; i < NUMOFWORDS; ++i)
	{
		if (!isVisited[i])
		{
			++connectedComponent;
			Explore(i, isVisited);
		}
	}

	return connectedComponent;
}


int main()
{
	std::cout << "Processing...\n";

	std::ifstream infile("words.txt");
	if (infile.fail())
	{
		std::cout << "Cant open file";
		exit(-1);
	}
	Input(infile);
    infile.close();

	CreateAdjacentList();

	std::cout << CountConnectedComponentByDFS();

	return 0;
}