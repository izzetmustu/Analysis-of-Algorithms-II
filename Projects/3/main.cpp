#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <deque>
#include <limits>
#include <thread>
#include <chrono>
/*
Name: Mustafa Izzet Mustu
ID: 504211564
*/

// Directional Graph
class Graph
{
public:
    int numVertices;
    std::vector<std::vector<int>> adjacencyMatrix;     // Adjacency matrix
    std::vector<std::vector<float>> probabilityMatrix; // Probability matrix

    Graph(int num_vertices) : numVertices(num_vertices+1),
                              adjacencyMatrix(num_vertices + 1, std::vector<int>(num_vertices + 1, 0)),
                              probabilityMatrix(num_vertices + 1, std::vector<float>(num_vertices + 1, 0))
    {
    }

    // Adding edge weight
    void add_edge(int v1, int v2, int weight)
    {
        adjacencyMatrix[v1][v2] = weight;
    }

    // Adding edge removals probability
    void add_probability(int v1, int v2, float probability)
    {
        probabilityMatrix[v1][v2] = probability;
    }

    // Print adjacency matrix
    void printAdjacencyMatrix()
    {
        std::cout << "Adjacency Matrix" << std::endl;
        for (int i = 1; i < numVertices; i++)
        {
            std::cout << i << "\t";
            for (int j = 1; j < numVertices; j++)
            {
                std::cout << adjacencyMatrix[i][j] << " ";
            }
            std::cout << std::endl;
        }
    }

    // Print probability matrix
    void printProbabilityMatrix()
    {
        std::cout << "Probability Matrix" << std::endl;
        for (int i = 1; i < numVertices; i++)
        {
            std::cout << i << "\t";
            for (int j = 1; j < numVertices; j++)
            {
                std::cout << probabilityMatrix[i][j] << " ";
            }
            std::cout << std::endl;
        }
    }

    // BFS with path
    std::pair<bool, std::deque<int>> BFS(const std::vector<std::vector<int>> &Gf, int start, int end)
    {
        std::cout<< "BFS Find Path!!!!" << std::endl;
        std::deque<int> path;
        std::vector<bool> visited(numVertices, false);
        std::vector<bool> neighbourList(numVertices, false);
        std::queue<int> q;
        int prevs[numVertices];

        q.push(start);
        prevs[start] = 0;
        int u;
        bool pathExists = false;

        while (!q.empty())
        {
            u = q.front();
            visited[u] = true;
            q.pop();
            for (int i = 1; i < numVertices; i++)
            {
                if (((Gf[u][i] > 0) || (Gf[i][u] > 0)) && !visited[i] && !neighbourList[i])
                {
                    q.push(i);
                    prevs[i] = u;
                    neighbourList[i] = true;
                    if(i == end) {
                        pathExists = true;
                    }
                }
            }
        }

        if(pathExists){
            std::cout<< "BFS Path Found!!!!" << std::endl;
            path.push_front(end);
            int i = end;
            std::cout << i << std::endl;
            bool found;
            while (!found)
            {
                i = prevs[i];
                path.push_front(i);
                if (i == start)
                {
                    found = true;
                }
                std::cout << i << std::endl;
            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        return std::pair<bool, std::deque<int>>(pathExists, path);
    }


    void increaseFlow(std::deque<int> path, std::vector<std::vector<int>> &Gf, std::vector<std::vector<int>> &flow)
    {
        std::cout<< "Increase Flow!!!!" << std::endl;
        int minWeight = std::numeric_limits<int>::max();
        for (std::deque<int>::iterator it = path.begin(); it != path.end()-1; it++)
        {
            int remaining = Gf[*it][*(it+1)];
            int reverseRemaining = Gf[*(it+1)][*it];
            std::cout << "remaining: " << remaining << " reverese rem: " << reverseRemaining << std::endl;
            if ((remaining < minWeight && remaining > 0)|| (reverseRemaining < minWeight && reverseRemaining > 0))
            {
                minWeight = Gf[*it][*(it+1)];
                std::cout << "Min weight is :" << minWeight << std::endl;
            }
        }
        
        std::cout << "REsiudal Matrix" << std::endl;
        for (int i = 1; i < numVertices; i++)
        {
            std::cout << i << "\t";
            for (int j = 1; j < numVertices; j++)
            {
                std::cout << Gf[i][j] << " ";
            }
            std::cout << std::endl;
        }



        for (std::deque<int>::iterator it = path.begin(); it != path.end()-1; it++)
        {
            if (Gf[*it][*(it+1)] != 0)
            {
                flow[*it][*(it+1)] = flow[*it][*(it+1)] + minWeight;
                std::cout << "Flow updated +++++" << flow[*it][*(it+1)] << std::endl;
            }
            if (Gf[*(it+1)][*it] != 0)
            {
                flow[*(it+1)][*it] = flow[*(it+1)][*it] - minWeight;
                std::cout << "Flow updated -----" << flow[*(it+1)][*it] << std::endl;
            }
        }

        std::cout << "UPDATED fLOW Matrix" << std::endl;
        for (int i = 1; i < numVertices; i++)
        {
            std::cout << i << "\t";
            for (int j = 1; j < numVertices; j++)
            {
                std::cout << flow[i][j] << " ";
            }
            std::cout << std::endl;
        }

        for (std::deque<int>::iterator it = path.begin(); it != path.end()-1; it++)
        {
            if (Gf[*it][*(it+1)] > 0)
            {
                Gf[*it][*(it+1)] = Gf[*it][*(it+1)] - minWeight;
            }
        }

        std::cout << "UPDATED residual Matrix" << std::endl;
        for (int i = 1; i < numVertices; i++)
        {
            std::cout << i << "\t";
            for (int j = 1; j < numVertices; j++)
            {
                std::cout << Gf[i][j] << " ";
            }
            std::cout << std::endl;
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }


    void FordFulkerson(int start, int end)
    {
        std::vector<std::vector<int>> flow(numVertices, std::vector<int>(numVertices, 0));
        std::vector<std::vector<int>> residual = adjacencyMatrix;

        std::pair<bool, std::deque<int>> p = BFS(residual, start, end);
        bool reachable = p.first;
        std::deque<int> path = p.second;
        while (reachable)
        {
            increaseFlow(path, residual, flow);
            p = BFS(residual, start, end);
            reachable = p.first;
            path = p.second;
        }

        int sum = 0;
        for(int i = 1; i<numVertices; i++) {
            sum += flow[i][numVertices-1];
        }
        std::cout << sum << std::endl;
    }


    void FordFulkersonProbabilities(int start, int end) {
        std::vector<std::pair<std::pair<int, int>, float>> probabilites;
        
        for(int i = 1; i < numVertices; i++)
        {
            for(int j = 1; j < numVertices; j++)
            {
                if(probabilityMatrix[i][j] != 0)
                {
                    probabilites.push_back(std::pair<std::pair<int,int>, float>(std::pair<int,int>(i,j), probabilityMatrix[i][j]));
                }
            }
        }
        
        for(auto it = probabilites.begin(); it != probabilites.end(); it++)
        {
            std::cout << "Vertex 1: " << it->first.first << " Vertex 2: " << it->first.second << " Edge: " << it->second << std::endl;
        }


    }

    // Destructor
    ~Graph()
    {

    }
};

int main(int argc, char **argv)
{
    if (argc != 4)
    {
        std::cerr << "Invalid number of arguments!" << std::endl;
        std::cerr << "Correct form: ./homework3 [input_file] [src_node] [dst_node]" << std::endl;
        return -1;
    }

    std::string inputFileName = argv[1];
    std::ifstream file;
    file.open(inputFileName);
    if (!file)
    {
        std::cerr << "Could not open input file!" << std::endl;
        return -1;
    }
    std::cout << inputFileName << std::endl;
    int s = std::stoi(argv[2]);
    int t = std::stoi(argv[3]);
    std::cout << "s: " << s << " t: " << t << std::endl;

    int cardinality;

    file >> cardinality;
    Graph g1(cardinality);
    for (int i = 1; i <= cardinality; i++)
    {
        int tmp;
        for (int j = 1; j <= cardinality; j++)
        {
            file >> tmp;
            g1.add_edge(i, j, tmp);
        }
    }
    g1.printAdjacencyMatrix();

    for (int i = 1; i <= cardinality; i++)
    {
        float tmp;
        for (int j = 1; j <= cardinality; j++)
        {
            file >> tmp;
            g1.add_probability(i, j, tmp);
        }
    }
    g1.printProbabilityMatrix();

    g1.FordFulkerson(s, t);
    g1.FordFulkersonProbabilities(s, t);

    return 0;
}