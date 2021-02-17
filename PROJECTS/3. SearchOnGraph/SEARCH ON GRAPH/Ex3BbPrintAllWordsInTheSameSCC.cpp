// This program reads input from file words.txt

#include<iostream>
#include<fstream>
#include<queue>
#include<string>
#include<vector>

//#define NUMOFWORDS 5757
int NUMOFWORDS;
// This array saves all words
std::vector<std::string> words;
// Adjacent list, but uses index (int) of word in words array instead of using directly word (string)
std::vector<std::vector<int>> adjList;
std::vector<std::vector<int>> adjListReverse;
// This array saves frequency of each letter in a string, is used in CheckString function
std::vector<int> alphabet(26, 0);
std::vector<int> postVisit;
// stronglyConnectedComponent[i] saves the index of SCC which words[i] belongs to
std::vector<int> stronglyConnectedComponent;


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
// Check if edge a -> b exist or not
bool CheckString(const std::string& a, const std::string& b)
{
    // Reset alphabet
    for (int i = 0; i < 26; ++i)
    {
        alphabet[i] = 0;
    }

    // frequency of each letter in string b
    for (int i = 0; i < 5; ++i)
    {
        ++alphabet[int(b[i] - 'a')];
    }

    for (int i = 1; i < 5; ++i)
    {
        // if a[i] appeared in string b
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
    adjList.resize(NUMOFWORDS, std::vector<int>());
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
    adjListReverse.resize(NUMOFWORDS, std::vector<int>());
    for (int i = 0; i < NUMOFWORDS; ++i)
    {
        for (const int& j : adjList[i])
        {
            adjListReverse[j].push_back(i);
        }
    }
}

// Find index of a word in words array, if the word doesn't exist, return -1
int FindIndexOf(const std::string& word)
{
    for (int i = 0; i < NUMOFWORDS; ++i)
    {
        if (words[i] == word)
        {
            return i;
        }
    }

    return -1;
}

void PostVisit(int vertex, int& clock)
{
    postVisit[vertex] = clock;
    ++clock;
}

void Explore(int vertex, int& clock, std::vector<bool>& isVisited, const std::vector<std::vector<int>>& adjList, int sCC = 0)
{
    isVisited[vertex] = true;
    stronglyConnectedComponent[vertex] = sCC;
    // This program doesn't use preVisit
    ++clock;
    for (const int& v : adjList[vertex])
    {
        if (!isVisited[v])
        {
            Explore(v, clock, isVisited, adjList, sCC);
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

int CountStrongConnectedComponent()
{
    std::vector<bool> isVisited(NUMOFWORDS, false);
    int countSCC = 0;
    int clock = 1;

    // This variable saves the index of word which has the biggest postVisit
    int indexOfMaxPostVisit = 0;
    while (true)
    {
        indexOfMaxPostVisit = 0;
        // Find the first word which hasn't been visited
        while (indexOfMaxPostVisit < NUMOFWORDS && isVisited[indexOfMaxPostVisit]) ++indexOfMaxPostVisit;

        if (indexOfMaxPostVisit >= NUMOFWORDS) // => all words have been visited
            break;

        // Find index of word that has biggest postVisit and hasn't been visited 
        for (int i = indexOfMaxPostVisit + 1; i < NUMOFWORDS; ++i)
        {
            if (!isVisited[i] && postVisit[i] > postVisit[indexOfMaxPostVisit])
            {
                indexOfMaxPostVisit = i;
            }
        }

        // Explore all words in the same SCC 
        Explore(indexOfMaxPostVisit, clock, isVisited, adjList, countSCC);
        ++countSCC;
    }

    return countSCC;
}

void PrintAllWordsInTheSameSCC(const std::string& word)
{
    int indexOfGivenWord = FindIndexOf(word);
    if (indexOfGivenWord == -1)
    {
        std::cout << "Word not found";
        return;
    }
    
    int sCCOfGivenWord = stronglyConnectedComponent[indexOfGivenWord];

    for (int i = 0; i < NUMOFWORDS; ++i)
    {
        if (stronglyConnectedComponent[i] == sCCOfGivenWord && i != indexOfGivenWord)
            std::cout << words[i] << '\n';
    }
}

int main()
{
    std::cout << "Please waiting...\n";

    std::ifstream infile("words.txt");
    if (infile.fail())
    {
        std::cout << "Cant open file";
        exit(-1);
    }

    Input(infile);
    infile.close();

    CreateAdjacentList();
    CreateAdjacentListReverse();

    postVisit.resize(NUMOFWORDS);
    stronglyConnectedComponent.resize(NUMOFWORDS);
    DFS(adjListReverse);

    CountStrongConnectedComponent();

    std::cout << "Press \'q\' to exit\n";
    std::string word;
    while (true)
    {
        std::cout << "\nWord : ";
        getline(std::cin, word);
        if (word == "q")
            break;

        PrintAllWordsInTheSameSCC(word);
    }
    
    return 0;
}