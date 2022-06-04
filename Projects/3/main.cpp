/**
 * @file main.cpp
 * @author Mustafa Izzet Mustu (mustu18@itu.edu.tr)
 * @brief ID: 504211564
 * @version 0.1
 * @date 2022-06-05
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <deque>
#include <map>
#include <limits>
#include <iomanip>

// Some type definitions
typedef std::vector< std::vector<int> > graphMatrix;                  // Represents adjancency matrix
typedef std::vector< std::vector<float> > graphProbabilityMatrix;     // Represents probability matrix
typedef std::pair< std::pair<int, int>, float > edgeWithProbability;  // Represents edges with probability

// Directional Graph
class Graph
{
public:
    int numVertices;
    graphMatrix adjacencyMatrix;                // Adjacency matrix
    graphProbabilityMatrix probabilityMatrix;   // Probability matrix

    Graph(int num_vertices) : numVertices(num_vertices+1),
                              adjacencyMatrix(num_vertices + 1, std::vector<int>(num_vertices + 1, 0)),
                              probabilityMatrix(num_vertices + 1, std::vector<float>(num_vertices + 1, 0))
    {
    }

    // Adding edge weight
    void
    addEdge(int v1, int v2, int weight)
    {
        adjacencyMatrix[v1][v2] = weight;
    }

    // Adding edge probability
    void
    addProbability(int v1, int v2, float probability)
    {
        probabilityMatrix[v1][v2] = probability;
    }

    // Print adjacency matrix
    void
    printAdjacencyMatrix()
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
    void
    printProbabilityMatrix()
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
    bool
    BFS(std::deque<int>& path, const graphMatrix &Gf, int start, int end)
    {
        path.clear();                                           // Reset path
        std::vector<bool> visited(numVertices, false);          // Holds visited nodes
        std::vector<bool> neighbourList(numVertices, false);    // Holds queued nodes
        std::queue<int> q;                                      // Queue to visit nodes
        int prevs[numVertices];                                 // Holds path

        q.push(start);
        prevs[start] = 0;
        int u;
        bool pathExists = false;
        while (!q.empty() && !pathExists)
        {
            u = q.front();
            visited[u] = true;
            q.pop();
            for (int i = 1; i < numVertices; i++)
            {
                if ((Gf[u][i] > 0) && !visited[i] && !neighbourList[i])
                {
                    q.push(i);
                    prevs[i] = u;                                 // Add a step to path
                    neighbourList[i] = true;
                    if(i == end)                                  // If we arrived, stop searching
                    {
                        pathExists = true;
                        break;
                    }

                }
            }
        }

        // If there is a path, pass it to deque from array
        if(pathExists){                         
            path.push_front(end);
            int i = end;
            bool found;
            while (!found)
            {
                i = prevs[i];
                path.push_front(i);
                if (i == start)
                {
                    found = true;
                }
            }
        }
        return pathExists;
    }

    // Update residual graph and update flow
    int
    increaseFlow(const std::deque<int>& path, graphMatrix &Gf)
    {
        // Find minimum edge
        int minWeight = std::numeric_limits<int>::max();
        for (std::deque<int>::const_iterator it = path.begin(); it != path.end()-1; it++)
        {
            if(Gf[*it][*(it+1)] < minWeight) 
            {
                minWeight = Gf[*it][*(it+1)];
            }
        }
        
        // Update residual graph
        for (std::deque<int>::const_iterator it = path.begin(); it != path.end()-1; it++)
        {
            Gf[*it][*(it+1)] = Gf[*it][*(it+1)] - minWeight;    // Decrease forward edges
            Gf[*(it+1)][*it] = Gf[*(it+1)][*it] + minWeight;    // Increase backward edges
        }

        return minWeight;
    }

    // Ford Fulkerson algorithm
    int
    FordFulkerson(graphMatrix& graph, int start, int end)
    {
        graphMatrix residual = graph;                           // Start residual graph as capacity
        int totalFlow = 0;                                      // Total flow
        std::deque<int> path;                                   // Stores path
        bool reachable = BFS(path, residual, start, end);       // Find path

        // If there is path, update flow
        while (reachable)
        {
            totalFlow += increaseFlow(path, residual);
            reachable = BFS(path, residual, start, end);        // Find path
        }

        return totalFlow;
    }

    // Calculate all possible combinations of edge removals from probability matrix
    void
    calculateCombinations(int left, int k, std::vector<edgeWithProbability>& probabilities, std::vector<edgeWithProbability>& combTemp, std::vector< std::vector<edgeWithProbability> >& combinations) 
    {
        // If no room left, add this combination to combinations vector
        if(k == 0) {
            combinations.push_back(combTemp);
            return;
        }

        // Start from left and go 1 step right to find a combination
        for(int i = left; i <= static_cast<int>(probabilities.size() - k); i++) {
            combTemp.push_back(probabilities[i]);
            calculateCombinations(i+1, k-1, probabilities, combTemp, combinations);
            combTemp.pop_back();
        }
    }

    // Calculate Ford Fulkerson algorith for all combinations and probabilities
    void
    FordFulkersonProbabilities(int start, int end) 
    {
        std::vector<edgeWithProbability> probabilites;              // All nonzero probabilites will be stored here
        graphMatrix graphTemp;                                      // Adjacency matrix for every combination
        std::vector< std::vector<edgeWithProbability> > combinations; // All combinations will be stored here
        std::vector<edgeWithProbability> combTemp;                  // Temporary combination matrix to find a combination
        std::map<int, float> flowsWithProbs;
        float prob;                                                 // To store probability
        int maxFlow;                                                // To store flow

        // Find all non-zero probability from probabilty matrix
        for(int i = 1; i < numVertices; i++)
        {
            for(int j = 1; j < numVertices; j++)
            {
                if(probabilityMatrix[i][j] != 0)
                {
                    probabilites.push_back(edgeWithProbability(std::pair<int,int>(i,j), probabilityMatrix[i][j]));
                    prob *= probabilityMatrix[i][j];
                }
            }
        }

        // Find all combinations
        for(int r = 0; r <= static_cast<int>(probabilites.size()); r++)
        {
            calculateCombinations(0, r, probabilites, combTemp, combinations);
        }
        combTemp.clear();   // Clear it to use further

        // Iterate over all the combinations and calculate Ford Fulkerson for each
        for(int i = 0; i < static_cast<int>(combinations.size()); i++)
        {
            graphTemp = adjacencyMatrix;    // Start adjacency matrix as default
            prob = 1;

            // Iterate over each combination to select non-zero probability edges 
            for(int j = 0; j < static_cast<int>(combinations[i].size()); j++)
            {
                int vertex1 = combinations[i][j].first.first;
                int vertex2 = combinations[i][j].first.second;
                graphTemp[vertex1][vertex2] = 0;
                combTemp.push_back(combinations[i][j]);
            }

            // Iterate over all non-zero probabilites to compare current combination
            for(int k = 0; k < static_cast<int>(probabilites.size()); k++)
            {
                bool exist = false;

                for(int z = 0; z < static_cast<int>(combTemp.size()); z++)
                {
                    if((combTemp[z].first.first == probabilites[k].first.first) && (combTemp[z].first.second == probabilites[k].first.second))
                    {
                        exist = true;
                        break;
                    }
                }

                // If selected edge is in current combination, use the removal probability value
                if(exist)
                {
                    prob *= probabilites[k].second;                    
                }
                // If selected edge is not in current combination, use the not removal probability value  
                else 
                {
                    prob *= (1.0 - probabilites[k].second);
                }             
            }
            combTemp.clear();                               // Clear it to use further
            maxFlow = FordFulkerson(graphTemp, start, end); // Calculate Ford Fulkerson for this combination
            flowsWithProbs[maxFlow] += prob;                // Add max flow with probability
        }
        float expected = 0;

        // Reverse keys and values to sort by probability
        std::map<float, int> reversed;
        for(std::map<int, float>::reverse_iterator it = flowsWithProbs.rbegin(); it != flowsWithProbs.rend(); it++)
        {
            reversed[it->second] = it->first;
        }

        // Print flow with probabilities
        for(std::map<float, int>::reverse_iterator it = reversed.rbegin(); it != reversed.rend(); it++)
        {
            std::cout << std::fixed << std::setprecision(5) << "Probability of occurence: " << it->first << ", Maximum Flow: " << it->second << std::endl;
            expected += it->second * it->first;
        }
        std::cout << "Expected Maximum Flow from node " << start << " to node " << end << ": " << expected << std::endl;
    }

    // Destructor
    ~Graph()
    {

    }
};

// Main function
int
main(int argc, char **argv)
{
    // Check arguments number
    if (argc != 4)
    {
        std::cerr << "Invalid number of arguments!" << std::endl;
        std::cerr << "Correct form: ./homework3 [input_file] [src_node] [dst_node]" << std::endl;
        return -1;
    }

    // Read from file
    std::string inputFileName = argv[1];
    std::ifstream file;
    file.open(inputFileName);
    if (!file)
    {
        std::cerr << "Could not open input file!" << std::endl;
        return -1;
    }
    int s = std::stoi(argv[2]);     // Starting vertex
    int t = std::stoi(argv[3]);     // Target vertex

    // Read number of nodes and create graph
    int cardinality;
    file >> cardinality;
    Graph g1(cardinality);

    // Read file and add edges
    for (int i = 1; i <= cardinality; i++)
    {
        int tmp;
        for (int j = 1; j <= cardinality; j++)
        {
            file >> tmp;
            g1.addEdge(i, j, tmp);
        }
    }

    // Read file and add probabilities
    for (int i = 1; i <= cardinality; i++)
    {
        float tmp;
        for (int j = 1; j <= cardinality; j++)
        {
            file >> tmp;
            g1.addProbability(i, j, tmp);
        }
    }

    // Start algorithm
    g1.FordFulkersonProbabilities(s, t);

    return 0;
}