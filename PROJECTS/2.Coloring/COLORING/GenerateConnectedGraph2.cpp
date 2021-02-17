#include<iostream>
#include<fstream>
#include<vector>
#include<queue>
#include<ctime>
#include<chrono>

#define MAX 12005


std::vector<std::vector<bool>> adjMatrix(MAX, std::vector<bool>(MAX, false));
std::vector<bool> isUsed(MAX, false);
    
void BFS(int numOfVertex, int& startVertex, int& countVertex)
{
    std::queue<int> vertex;
    vertex.push(startVertex);

    while (!vertex.empty())
    {
        startVertex = vertex.front();
        //std::cout << startVertex + 1 << " : ";
        if (!isUsed[startVertex])
        {
            isUsed[startVertex] = true;
            ++countVertex;
        }
        
        //std::cout << currentVertex + 1 << ' ';
        for (int v = 0; v < numOfVertex; ++v)
        {
            if (adjMatrix[startVertex][v] && !isUsed[v])
            {
                vertex.push(v);
                //std::cout << v + 1 << ' ';
            }
        }
        //std::cout << '\n';

        vertex.pop();
    }
}



int main()
{
    srand((unsigned int)time(NULL));

    int numOfVertex;
    int numOfEdge = 0;
    
    std::cin >> numOfVertex;

    auto t1 = std::chrono::high_resolution_clock::now();

    std::ofstream outf("dothi.txt");
    if (outf.fail())
    {
        std::cout << "Cant open file";
        exit(-1);
    }

    outf << numOfVertex;
    outf << '\n';

    int u, v;
    int startVertex = 0;
    int oldStartVertex = 0;
    int countVertex = 0;

    while (numOfVertex > countVertex)
    {
        u = 0;
        v = 0;
        while (u == v || adjMatrix[u][v])
        {
            u = rand() % numOfVertex;
            v = rand() % numOfVertex;
        }

        

        adjMatrix[u][v] = true;
        adjMatrix[v][u] = true;
        ++numOfEdge;
        outf << u + 1 << ' ' << v + 1 << '\n';

        if (isUsed[u] && isUsed[v])
        {
            continue;
        }
        else if (isUsed[u] && !isUsed[v])
        {
            isUsed[v] = true;
            ++countVertex;
            continue;
        }
        else if (isUsed[v] && !isUsed[u])
        {
            isUsed[u] = true;
            ++countVertex;
            continue;
        }
        oldStartVertex = startVertex;
        BFS(numOfVertex, startVertex, countVertex);
        if (startVertex == oldStartVertex)
        {
            --countVertex;
            isUsed[startVertex] = false;
        }
    }

    
    //outf << numOfEdge;
    std::cout << numOfEdge;

    outf.close();

    auto t2 = std::chrono::high_resolution_clock::now();

    auto duration = std::chrono::duration_cast<std::chrono::microseconds>( t2 - t1 ).count();

    std::cout << '\n' << duration;
    
    return 0;
}