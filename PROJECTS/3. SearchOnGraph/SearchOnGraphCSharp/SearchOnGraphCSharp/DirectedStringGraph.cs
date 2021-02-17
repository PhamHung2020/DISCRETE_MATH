using System;
using System.Collections.Generic;
using System.Text;
using System.IO;

namespace SearchOnGraphCSharp
{
    class DirectedStringGraph : StringGraph
    {
        protected List<int>[] adjListReverse = null;
        protected int[] postVisit = null;

        public DirectedStringGraph(string path = "words.txt") : base(path)
        {
            CreateAdjacentList();
            CreateAdjacentListReverse();
            postVisit = new int[NUMOFWORDS];
        }

        protected override bool CheckString(string a, string b)
        {
            AlphabetArray alphabet = new AlphabetArray();

            for (int i = 0; i < 5; ++i)
            {
                ++alphabet.alphabet[b[i] - (int)'a'];
            }

            for (int i = 1; i < 5; ++i)
            {
                // if a[i] appeared in string b
                if (alphabet.alphabet[a[i] - (int)'a'] > 0)
                {
                    --alphabet.alphabet[a[i] - (int)'a'];
                }
                else
                {
                    return false;
                }
            }
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
                    }
                    if (CheckString(words[j], words[i]))
                    {
                        adjList[j].Add(i);
                    }
                }
            }
        }
        private void CreateAdjacentListReverse()
        {
            adjListReverse = new List<int>[NUMOFWORDS];
            for (int i = 0; i < NUMOFWORDS; ++i)
            {
                adjListReverse[i] = new List<int>();
            }

            for (int i = 0; i < NUMOFWORDS; ++i)
            {
                foreach (int v in adjList[i])
                {
                    adjListReverse[v].Add(i);
                }
            }
        }
        private void PostVisit(int vertex, ref int clock)
        {
            postVisit[vertex] = clock;
            ++clock;
        }
        private void Explore(int vertex, ref int clock, bool[] isVisited, List<int>[] adjList)
        {
            isVisited[vertex] = true;
            // This program doesn't use preVisit
            ++clock;
            foreach (int v in adjList[vertex])
    {
                if (!isVisited[v])
                {
                    Explore(v, ref clock, isVisited, adjList);
                }
            }
            PostVisit(vertex, ref clock);
        }
        private void DFS(List<int>[] adjList)
        {
            bool[] isVisited = new bool[NUMOFWORDS];
            int clock = 1;

            for (int i = 0; i < NUMOFWORDS; ++i)
            {
                if (!isVisited[i])
                {
                    Explore(i, ref clock, isVisited, adjList);
                }
            }
        }
        public int CountStronglyConnectedComponent()
        {
            DFS(adjListReverse);

            bool[] isVisited = new bool[NUMOFWORDS];
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
                Explore(indexOfMaxPostVisit, ref clock, isVisited, adjList);
                ++countSCC;
            }

            return countSCC;
        }
    }

    class Person
    {
        private int _id = 5;
        private string _name = "Hung";
        public Person()
        {
            Console.WriteLine($"{_id} - {_name}");
        }
    }
}
