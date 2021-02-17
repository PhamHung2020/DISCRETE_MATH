
/*
The idea of solution:
    1. Order the vertexes v1, v2, ..., vn using the technique shown in slides (proof of theory).
    (start with a vertex which has degree <= k - 1, list all neighbors of the vertex, ...)
    In this program, I call it : vertexList
    2. Use Greedy Algorithm to color all the vertexes.

About color in graphviz, because the maximum number of color is 20, so I use a vector that contains
20 color names. If number of color is more than 20/unpredictable, I will use the code at the bottom of
'main' function which are marked as comments and iomanip library.

Because source code, file input and file output are in the same folder, so I only use name of file,
don't use path
*/


#include<iostream>
#include<fstream>
//#include<iomanip>
#include<vector>
#include<list>
#include<string>

std::vector<std::string> colorNames
{
    "Blue",
    "BlueViolet",
    "Red",
    "Green",
    "Violet",
    "Orange",
    "Yellow",
    "Khaki",
    "Cyan",
    "Brown"
    "Chocolate",
    "Grey",
    "Pink",
    "Azure",
    "AquaMarine",
    "GreenYellow",
    "SkyBlue",
    "Tomato",
    "Navy",
    "FloralWhite"
};

int FindMaxdegree(const std::vector<int>& degrees)
{
    int maxdegree = degrees[0];
    for (const int& deg : degrees)
    {
        if (deg > maxdegree)
            maxdegree = deg;
    }

    return maxdegree;
}

/*
This function will order the vertexes v1, v2, ..., vn (vertexList) for NON-REGULAR graph.
If the graph is not regular, this function will be called.
It starts with startVertex (deg <= k - 1), list all its neighbors, and repeat with neighbors as startVertex.
This function ends when all vertexes are listed.
*/
std::vector<int> CreateVertexListForNonRegularGraph(int numOfVertex, const std::vector<std::list<int>>& adjList, int startVertex)
{
    // This array will be returned when this function ends
    std::vector<int> vertexList(numOfVertex);
    vertexList[numOfVertex - 1] = startVertex;

    // This array marks a vertex is listed or not
    // If a vertex v is listed, isListed[v] = true, otherwise, = false
    std::vector<bool> isListed(numOfVertex, false);
    isListed[startVertex] = true;

    // neighborIndex : index of newest neighbor vertex of current vertex in the list, start from numOfVertex - 2 to 0
    // if all vertexes are listed, neighborIndex = -1
    int currentVertexIndex = numOfVertex - 1, neighborIndex = numOfVertex - 2;
    
    while (neighborIndex >= 0)
    {
        for (const int& v : adjList[vertexList[currentVertexIndex]])
        {
            if (!isListed[v])
            {
                vertexList[neighborIndex] = v;
                --neighborIndex;
                isListed[v] = true;
            }
        }
        --currentVertexIndex;
    }

    return vertexList;
}


/*
This function will create vertexList for the graph.
If the graph is not regular, function CreateVertexListForNonRegularGraph() will be called
Else, vertexList is 1, 2, 3, ... n because, if a graph is regular, Greedy Algorithm always uses k + 1 colors,
not depends on how vertexList is.
*/
std::vector<int> CreateVertexList(int numOfVertex, const std::vector<int>& degrees, const std::vector<std::list<int>>& adjList, int maxDegree)
{
    // First vertex that has degree <= k - 1, passed into CreateVertexListForNonRegularGraph() function
    int startVertex;
    bool isRegular = true;

    // Check if the graph is regular or not
    for (int i = 0; i < numOfVertex; ++i)
    {
        if (degrees[i] < maxDegree)
        {
            isRegular = false;
            startVertex = i;
            break;
        }
    }

    if (isRegular)
    {
        std::vector<int> vertexList(numOfVertex);
        for (int i = 0; i < numOfVertex; ++i)
        {
            vertexList[i] = i;
        }

        return vertexList;
    }
    else
    {
        return CreateVertexListForNonRegularGraph(numOfVertex, adjList, startVertex);
    }
}


/*
This function will color all vertexes, use vertexList and Greedy Algorithm.
*/
std::vector<int> ColorGraph(int numOfVertex, const std::vector<std::list<int>>& adjList, const std::vector<int>& vertexList, int maxDegree)
{
    // The color list, color of vertex v is color[v], if color[v] = -1 => v hasn't been colored
    std::vector<int> colors(numOfVertex, -1);
    colors[vertexList[0]] = 0;

    int currentVertex;
    for (int i = 1; i < numOfVertex; ++i)
    {
        currentVertex = vertexList[i];

        // This array marks a color is used or not
        std::vector<bool> isUsedColor(maxDegree + 1, false);
        // Visit all neighbors of current vertex
        for (const int& v : adjList[currentVertex])
        {
            // If v has been colored
            if (colors[v] != -1)
            {
                isUsedColor[colors[v]] = true;
            }
        }

        int legalColor = 0;
        // Find the smallest legal color
        while (isUsedColor[legalColor])
        {
            ++legalColor;
        }
        colors[currentVertex] = legalColor;
    }

    return colors;
}

int main()
{
    int numOfVertex, numOfEdge;

    // File input
    std::ifstream inf("dothi.txt");
    if (inf.fail())
    {
        std::cout << "Cant open input file";
        exit(-1);
    }

    inf >> numOfVertex >> numOfEdge;

    // This array saves degrees of all vertexes in the graph, initialize to 0
    std::vector<int> degrees(numOfVertex, 0);

    //  Set up adjacent list by using an array of linked list 
    std::vector<std::list<int>> adjList(numOfVertex, std::list<int>());
    
    // Open and start writing to file output
    std::ofstream outf("dothitomau.dot");
    if (outf.fail())
    {
        std::cout << "Cant open output file";
        exit(-1);
    }

    outf << "graph dothi\n{";

    // Read edges list, write edge list to file ouput, update degree array and adjacent list
    int x, y;
    for (int i = 0; i < numOfEdge; ++i)
    {
        inf >> x >> y;
        outf << "\n\t" << x << " -- " << y;
        ++degrees[x - 1];
        ++degrees[y - 1];
        adjList[x - 1].emplace_back(y - 1);
        adjList[y - 1].emplace_back(x - 1);
    }

    // Reading input completed, so close input file
    inf.close();


    // Main activities in program
    int maxDegree = FindMaxdegree(degrees);
    std::vector<int> vertexList = CreateVertexList(numOfVertex, degrees, adjList, maxDegree);
    std::vector<int> colors = ColorGraph(numOfVertex, adjList, vertexList, maxDegree);

    /* Alternative solution if number of color is unpredictable
    const int maxColor = 0xffffff;
    int firstColor = maxColor / (maxDegree + 1);
    for (int i = 0; i < numOfVertex; ++i)
    {
        if (color[i] % 2 == 0)
            outf << "\n\t" << std::dec << i + 1 << " [fillcolor=\"#" << std::hex << firstColor * (color[i] + 1) - 100 << "\", style=filled];";
        else
            outf << "\n\t" << std::dec << i + 1 << " [fillcolor=\"#" << std::hex << firstColor * (color[i] + 1) + 100 << "\", style=filled];";
    }*/

    // Write color of all vertexes to file output
    for (int i = 0; i < numOfVertex; ++i)
    {
        outf << "\n\t" << std::dec << i + 1 << " [fillcolor=" << colorNames[colors[i]] << ", style=filled];";
    }

    outf << "\n}";

    // Writing completed
    outf.close();

    return 0;
}