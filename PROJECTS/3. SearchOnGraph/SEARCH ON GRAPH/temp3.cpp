#include<iostream>
#include<fstream>
#include<queue>
#include<string>
#include<vector>
#include<list>

//#define NUMOFWORDS 5757

int NUMOFWORDS;

std::ofstream outfile("output.txt");

std::vector<std::string> words(NUMOFWORDS);
std::vector<std::vector<int>> adjList(NUMOFWORDS, std::vector<int>());
std::vector<std::vector<int>> adjListReverse(NUMOFWORDS, std::vector<int>());
std::vector<int> alphabet(26, 0);

std::vector<int> preVisit(NUMOFWORDS);
std::vector<int> postVisit(NUMOFWORDS);
std::vector<int> stronglyConnectedComponent(NUMOFWORDS);

void ResetAlphabet()
{
    for (int i = 0; i < 26; ++i)
    {
        alphabet[i] = 0;
    }
}

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

void CreateAdjacentListReverse()
{
    for (int i = 0; i < NUMOFWORDS; ++i)
    {
        for (const int& j : adjList[i])
        {
            adjListReverse[j].push_back(i);
        }
    }
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
    outfile << words[vertex] << '\n';
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
}

void CountStrongConnectedComponent()
{
    std::vector<bool> isVisited(NUMOFWORDS, false);
    int countCC = 0;
    int clock = 1;

    int indexOfMaxPostVisit = 0;
    while (true)
    {
        indexOfMaxPostVisit = 0;
        while (indexOfMaxPostVisit < NUMOFWORDS && isVisited[indexOfMaxPostVisit]) ++indexOfMaxPostVisit;
        for (int i = indexOfMaxPostVisit + 1; i < NUMOFWORDS; ++i)
        {
            if (!isVisited[i] && postVisit[i] > postVisit[indexOfMaxPostVisit])
            {
                indexOfMaxPostVisit = i;
            }
        }

        if (indexOfMaxPostVisit >= NUMOFWORDS)
            break;
        outfile << words[indexOfMaxPostVisit] << " : \n";
        Explore(indexOfMaxPostVisit, clock, isVisited, adjList);
        ++countCC;
        outfile << countCC;
        outfile << "\n\n\n\n";
    }

    std::cout << countCC;
}



int main()
{
    std::ifstream infile("words.txt");
    if (infile.fail())
    {
        std::cout << "Cant open file";
        exit(-1);
    }

    Input(infile);
    CreateAdjacentList();
    CreateAdjacentListReverse();

    /*int numOfEdge = 0;
    for (int i = 0; i < NUMOFWORDS; ++i)
    {
        numOfEdge += adjListReverse[i].size();
    }

    std::cout << numOfEdge;*/
    std::ofstream outfile("output.txt");
    for (int i = 0; i < NUMOFWORDS; ++i)
    {
        outfile << words[i] << " : ";
        for (const int& v : adjListReverse[i])
        {
            outfile << words[v] << ' ';
        }
        outfile << '\n';
    }

    /*while (true)
    {
        std::string source;
        std::string destination;
        std::cout << "Source : ";
        getline(std::cin, source);
        if (source == "q")
            break;
        std::cout << "Destination : ";
        getline(std::cin, destination);
        FindShortestPath(source, destination);
        std::cout << '\n';
    }*/
    

    //DFS(adjListReverse);

    //outfile << "\n\n\n\n\n\n\n\n";

    /*for (int i = 0; i < NUMOFWORDS; ++i)
    {
        std::cout << words[i] << " : " << preVisit[i] << " : " << postVisit[i] << '\n';
    }*/

    //CountStrongConnectedComponent();

    /*int x = 0;
    for (int i = 0; i < NUMOFWORDS; ++i)
    {
        if (words[i] == "words")
        {
            x = i;
            break;
        }
    }
    
    for (const int& v : adjList[x])
    {
        std::cout << words[v] << '\n';
    }*/

    //std::cout << words[x] << " : " << preVisit[x] << " : " << postVisit[x];

    return 0;
}