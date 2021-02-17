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
// This array saves frequency of each letter in a string, is used in CheckString function
std::vector<int> alphabet(26, 0);
// This array saves the previous word of a word in the shortest path
std::vector<int> previousWord;


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

void PrintShortestPath(int source, int destination)
{
    if (destination == source)
    {
        std::cout << words[destination] << ' ';
        return;
    }

    PrintShortestPath(source, previousWord[destination]);
    std::cout << words[destination] << ' ';
}

void FindShortestPath(const std::string& source, const std::string& destination)
{
    int indexOfSource = FindIndexOf(source);
    int indexOfDestination = FindIndexOf(destination);

    // if source or destination doesn't exits
    if (indexOfSource == -1 || indexOfDestination == -1)
    {
        std::cout << "Word not found";
        return;
    }
 
    std::queue<int> q;
    q.push(indexOfSource);

    // Start BFS until destination is encountered
    while (!q.empty())
    {
        int currentVertex = q.front();
        for (const int& v : adjList[currentVertex])
        {
            if (previousWord[v] == -1)
            {
                q.push(v);
                previousWord[v] = currentVertex;
            }
            if (v == indexOfDestination)
                break;
        }
        if (q.back() == indexOfDestination)
            break;
        q.pop();
    }

    if (q.empty())
    {
        std::cout << "Path not found";
        return;
    }
    
    PrintShortestPath(indexOfSource, indexOfDestination);

    // Reset previousWord array
    for (int i = 0; i < NUMOFWORDS; ++i)
    {
        previousWord[i] = -1;
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

    previousWord.resize(NUMOFWORDS, -1);

    std::cout << "Press \'q\' to exit\n";
    std::string source;
    std::string destination;
    while (true)
    {
        std::cout << "Source : ";
        getline(std::cin, source);
        if (source == "q")
            break;

        std::cout << "Destination : ";
        getline(std::cin, destination);

        FindShortestPath(source, destination);
        std::cout << '\n';
    }
    
    return 0;
}