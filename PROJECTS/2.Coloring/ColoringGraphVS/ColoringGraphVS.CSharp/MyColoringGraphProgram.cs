using System;
using System.IO;
using System.Collections.Generic;

namespace ColoringGraphVS.CSharp
{
    class MyColoringGraphProgram
    {
        public static string[] colorNames = new string[]
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
            "Brown",
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


        static void Main(string[] args)
        {
            FileStream infile = new FileStream("graph.txt", FileMode.Open, FileAccess.Read);
            if (infile == null)
            {
                Console.WriteLine("Cant open file");
                return;
            }
            StreamReader streamReader = new StreamReader(infile);

            string[] vertriceAndEdges = streamReader.ReadLine().Split(' ');
            int numOfVertrice = int.Parse(vertriceAndEdges[0]);
            int numOfEdges = int.Parse(vertriceAndEdges[1]);

            int[] degrees = new int[numOfVertrice];
            List<int>[] adjList = new List<int>[numOfVertrice];
            
            for (int i = 0; i < numOfVertrice; ++i)
            {
                adjList[i] = new List<int>();
            }

            FileStream outfile = new FileStream("coloringGraph.dot", FileMode.OpenOrCreate, FileAccess.Write);
            if (outfile == null)
            {
                Console.WriteLine("Cant open output file");
                return;
            }
            StreamWriter streamWriter = new StreamWriter(outfile);
            streamWriter.Write("graph dothi\n{");
            int x, y;
            for (int i = 0; i < numOfEdges; ++i)
            {
                string[] tokens = streamReader.ReadLine().Split(' ');
                x = int.Parse(tokens[0]);
                y = int.Parse(tokens[1]);

                streamWriter.Write($"\n\t{x} -- {y}");
                ++degrees[x - 1];
                ++degrees[y - 1];
                adjList[x - 1].Add(y - 1);
                adjList[y - 1].Add(x - 1);
            }
            infile.Close();

            int maxDegree = FindMaxDegree(degrees);
            int[] vertexList = CreateVertexList(numOfVertrice, degrees, adjList, maxDegree);
            int[] colors = ColorGraph(numOfVertrice, adjList, vertexList, maxDegree);

            for (int i = 0; i < numOfVertrice; ++i)
            {
                streamWriter.Write($"\n\t{ i + 1 } [fillcolor={colorNames[colors[i]]}, style=filled];");
            }

            streamWriter.Write("\n}");
            streamWriter.Flush();
            outfile.Close();
        }

        static int FindMaxDegree(int[] degrees)
        {
            int maxDegree = degrees[0];
            foreach (int deg in degrees)
            {
                if (deg > maxDegree)
                    maxDegree = deg;
            }
            return maxDegree;
        }

        static int[] CreateVertexListForNonRegularGraph(int numOfVertrice, List<int>[] adjList, int startVertex)
        {
            int[] vertexList = new int[numOfVertrice];
            vertexList[numOfVertrice - 1] = startVertex;

            bool[] isListed = new bool[numOfVertrice];
            isListed[startVertex] = true;

            int currentVertexIndex = numOfVertrice - 1, neighborIndex = numOfVertrice - 2;

            while (neighborIndex >= 0)
            {
                foreach (int v in adjList[vertexList[currentVertexIndex]])
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

        static int[] CreateVertexList(int numOfVertrice, int[] degrees, List<int>[] adjList, int maxDegree)
        {
            // First vertex that has degree <= k - 1, passed into CreateVertexListForNonRegularGraph() function
            int startVertex = 0;
            bool isRegular = true;

            // Check if the graph is regular or not
            for (int i = 0; i < numOfVertrice; ++i)
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
                int[] vertexList = new int[numOfVertrice];
                for (int i = 0; i < numOfVertrice; ++i)
                {
                    vertexList[i] = i;
                }

                return vertexList;
            }
            else
            {
                return CreateVertexListForNonRegularGraph(numOfVertrice, adjList, startVertex);
            }
        }

        static int[] ColorGraph(int numOfVertrice, List<int>[] adjList, int[] vertexList, int maxDegree)
        {
            // The color list, color of vertex v is color[v], if color[v] = -1 => v hasn't been colored
            int[] colors = new int[numOfVertrice];
            for (int i = 0; i < numOfVertrice; ++i)
            {
                colors[i] = -1;
            }
            colors[vertexList[0]] = 0;

            int currentVertex;
            for (int i = 1; i < numOfVertrice; ++i)
            {
                currentVertex = vertexList[i];

                // This array marks a color is used or not
                bool[] isUsedColor = new bool[maxDegree + 1];
                // Visit all neighbors of current vertex
                foreach (int v in adjList[currentVertex])
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
    }
}
