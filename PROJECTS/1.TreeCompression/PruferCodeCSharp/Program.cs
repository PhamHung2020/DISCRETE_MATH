using System;
using System.Collections.Generic;


namespace PruferCodeCSharp
{
    class Program
    {
        static void Main(string[] args)
        {
            int numOfEdges = Convert.ToInt32(Console.ReadLine());
            int numofVertrice = numOfEdges + 1;

            int[] deg = new int[numofVertrice];
            LinkedList<int>[] adjList = new LinkedList<int>[numofVertrice];
            for (int i = 0; i < numofVertrice; ++i)
            {
                adjList[i] = new LinkedList<int>();
            }

            int first, second;
            for (int i = 0; i < numOfEdges; ++i)
            {
                string[] tokens = Console.ReadLine().Split(' ');
                first = int.Parse(tokens[0]);
                second = int.Parse(tokens[1]);
                ++deg[first];
                ++deg[second];
                adjList[first].AddFirst(second);
                adjList[second].AddFirst(first);
            }

            int[] pruferCodes = CreatePruferCode(numofVertrice, deg, adjList);

            foreach (int i in pruferCodes)
            {
                Console.Write("{0} ", i);
            }


        }

        static int[] CreatePruferCode(int numOfVertrice, int[] deg, LinkedList<int>[] adjList)
        {

            int[] pruferCodes = new int[numOfVertrice - 2];

            int leaf = 1;
            while (leaf < numOfVertrice - 1 && deg[leaf] != 1) ++leaf;
            int lowestLeaf = leaf;

            for (int i = 0; i < numOfVertrice - 2; ++i)
            {
                int adjVertex = 0;
                foreach (int node in adjList[lowestLeaf])
                {
                    if (deg[node] != 0)
                    {
                        adjVertex = node;
                        break;
                    }
                }

                pruferCodes[i] = adjVertex;

                --deg[lowestLeaf];
                --deg[adjVertex];

                if (adjVertex != 0 && deg[adjVertex] == 1 && adjVertex < lowestLeaf)
                {
                    lowestLeaf = adjVertex;
                }
                else
                {
                    while (leaf < numOfVertrice - 1 && deg[leaf] != 1) ++leaf;
                    lowestLeaf = leaf;
                }
            }


            return pruferCodes;
        }
    }


}
