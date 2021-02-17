using System;
using System.Collections.Generic;
using System.Text;
using System.IO;

namespace SearchOnGraphCSharp
{
    abstract class StringGraph
    {
        protected int NUMOFWORDS = 0;
        protected List<string> words = new List<string>();
        protected List<int>[] adjList = null;

        public StringGraph(string path = "words.txt")
        {
            FileStream infile = new FileStream(path, FileMode.Open, FileAccess.Read);
            if (infile == null)
            {
                Console.WriteLine("Cant open input file");
                return;
            }
            Input(infile);
            infile.Close();

            adjList = new List<int>[NUMOFWORDS];
            for (int i = 0; i < NUMOFWORDS; ++i)
            {
                adjList[i] = new List<int>();
            }
        }

        private void Input(FileStream infile)
        {
            StreamReader streamReader = new StreamReader(infile);
            while (!streamReader.EndOfStream)
            {
                words.Add(streamReader.ReadLine().Split('\n')[0]);
            }
            NUMOFWORDS = words.Count;
            streamReader.Dispose();
            streamReader.Close();
        }
        protected abstract bool CheckString(string a, string b);
        protected abstract void CreateAdjacentList();
        public int FindIndexOf(string str)
        {
            for (int i = 0; i < NUMOFWORDS; ++i)
            {
                if (str.Equals(words[i]))
                    return i;
            }
            return -1;
        }
        protected virtual void PrintShortestPath(int source, int destination, int[] previous)
        {
            if (source == destination)
            {
                Console.Write($"{words[destination]} ");
                return;
            }

            PrintShortestPath(source, previous[destination], previous);
            Console.Write($"{words[destination]} ");
        }
        public virtual void FindShortestPath(string source, string destination)
        {
            int[] previous = new int[NUMOFWORDS];
            for (int i = 0; i < NUMOFWORDS; ++i)
            {
                previous[i] = -1;
            }

            int indexOfSource = FindIndexOf(source);
            int indexOfDestination = FindIndexOf(destination);

            if ((indexOfSource == -1) || (indexOfDestination == -1))
            {
                Console.WriteLine("Word not found");
                return;
            }

            Queue<int> q = new Queue<int>();
            q.Enqueue(indexOfSource);

            while (q.Count != 0)
            {
                int currentVertex = q.Dequeue();
                int back = currentVertex;
                foreach (int v in adjList[currentVertex])
                {
                    if (previous[v] == -1)
                    {
                        q.Enqueue(v);
                        previous[v] = currentVertex;
                        back = v;
                    }
                    if (v == indexOfDestination)
                        break;
                }
                if (back == indexOfDestination)
                    break;
            }
            if (q.Count == 0)
            {
                Console.WriteLine("Path not found");
                return;
            }
            PrintShortestPath(indexOfSource, indexOfDestination, previous);
        }
    }
}
