#include<iostream>
#include<vector>
#include<list>
#include<fstream>
#include<queue>

#define MAX 100005
std::vector<std::vector<bool>> adjMatrix(MAX, std::vector<bool>(MAX, false));
std::vector<bool> isUsed(MAX, false);

bool BFS(int numOfVertex)
{
    std::queue<int> vertex;
    vertex.push(0);

    
    isUsed[0] = true;

    int currentVertex;
    int countVertex = 0;
    while (!vertex.empty())
    {
        currentVertex = vertex.front();
        //std::cout << currentVertex + 1 << " : ";
        ++countVertex;
        //std::cout << currentVertex + 1 << ' ';
        for (int v = 0; v < numOfVertex; ++v)
        {
            if (adjMatrix[currentVertex][v] && !isUsed[v])
            {
                vertex.push(v);
                isUsed[v] = true;
                //std::cout << v + 1 << ' ';
            }
        }
        //std::cout << '\n';

        vertex.pop();
    }
    std::cout << countVertex << '\n';
    return (countVertex == numOfVertex);
}


int main()
{
    int numOfVertex, numOfEdge;

    // file input
    std::ifstream inf("dothi.txt");
    if (inf.fail())
    {
        std::cout << "Cant open input file";
        exit(-1);
    }

    inf >> numOfVertex >> numOfEdge;

    //  Set up adjacent list by using an array of linked list 
    

    int x, y;
    for (int i = 0; i < numOfEdge; ++i)
    {
        inf >> x >> y;
        adjMatrix[x - 1][y - 1] = true;
        adjMatrix[y - 1][x - 1] = true;
    }

    // Reading input completed, so close input file
    inf.close();

    std::cout << std::boolalpha << BFS(numOfVertex);

    return 0;


}