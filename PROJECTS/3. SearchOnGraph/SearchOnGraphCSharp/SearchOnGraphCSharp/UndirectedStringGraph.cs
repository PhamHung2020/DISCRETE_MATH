using System;
using System.Collections.Generic;
using System.IO;

namespace SearchOnGraphCSharp
{
    class UndirectedStringGraph : StringGraph
    {
        public UndirectedStringGraph(string path = "words.txt") : base(path)
        {
            CreateAdjacentList();
        }

        protected override bool CheckString(string a, string b)
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
        protected override void CreateAdjacentList()
        {
            adjList = new List<int>[NUMOFWORDS];
            for (int i = 0; i < NUMOFWORDS; ++i)
            {
                adjList[i] = new List<int>();
            }

            for (int i = 0; i < NUMOFWORDS - 1; ++i)
            {
                for (int j = i + 1; j < NUMOFWORDS; ++j)
                {
                    if (CheckString(words[i], words[j]))
                    {
                        adjList[i].Add(j);
                        adjList[j].Add(i);
                    }
                }
            }
        }
        private void Explore(int vertex, bool[] isVisited)
        {
            isVisited[vertex] = true;
            foreach (int v in adjList[vertex])
            {
                if (!isVisited[v])
                {
                    Explore(v, isVisited);
                }
            }
        }
        public int CountConnectedComponent()
        {
            int connectedComponent = 0;
            bool[] isVisited = new bool[NUMOFWORDS];

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
    }
}
