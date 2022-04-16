#include <iostream>
#include <list>
#include <algorithm>
// #include <chrono>
#include <time.h>

/*
Name: Mustafa Izzet Mustu
ID: 504211564
*/

// Nondirectional graph
class Graph{
public:
    int num_vertices;
    int** graph;        // Adjacency matrix 
    
    Graph(int num_vertices){
        this->num_vertices = num_vertices+1;
        graph = new int*[this->num_vertices];
        for(int i = 0; i < this->num_vertices; i++){
            graph[i] = new int[this->num_vertices];
            for(int j = 0; j < this->num_vertices; j++){
                graph[i][j] = 0;
            }
        }   
    }
    
    // Adding edge function with cost
    void add_edge(int v1, int v2, int cost){
        graph[v1][v2] = cost;
        graph[v2][v1] = cost;
    }

    // Print adjacency matrix
    void printMatrix(){
        std::cout << "Adjacency Matrix" << std::endl;
        for(int i = 1; i < num_vertices; i++){
            std::cout << i << "\t";
            for(int j = 1; j < num_vertices; j++){
                std::cout << graph[i][j] << " ";
            }
            std::cout << std::endl;
        }
    }

    ~Graph(){
        for(int i = 0; i < num_vertices; i++){
            delete[] graph[i];
        }
        delete[] graph;
    }
};

// Function to find the smallest missing positive number from an unsorted array
int findSmallestMissing(const std::list<int>& nums)
{
    int start = 1;
    for(int i = 0; i<nums.size(); i++){
        for(std::list<int>::const_iterator it = nums.begin(); it != nums.end(); it++){
            if((*it) == start) {
                start++;
            }
        }
    }
    return start;
}

// Greedy Algorithm 1
void GA1(const Graph& g){
    int current_vertex = 1;
    int vertex_num = g.num_vertices;
    int vertex_colors[vertex_num];
    vertex_colors[current_vertex] = 1;
    std::cout << "Vertex " << current_vertex << " --> Color " << 1 << std::endl;
    current_vertex++;
    int max_color = 1;
    for(int i = current_vertex; i<vertex_num; i++){
        std::list<int> colors;
        for(int j = 1; j<vertex_num; j++){
            if(g.graph[i][j] == 1){
                if(vertex_colors[j] != 0){
                    colors.push_back(vertex_colors[j]);
                }
            }
        }
        int color = findSmallestMissing(colors);
        if(color > max_color){
            max_color = color;
        }
        vertex_colors[i] = color;
        std::cout << "Vertex " << i << " --> Color " << color << std::endl;
    }
    std::cout << std::endl <<"Number of different colors: " << max_color << std::endl;
}

int main(){
    int num_vertices = 7;
    Graph G1(num_vertices); // Creating edges
    G1.add_edge(1, 2, 1);
    G1.add_edge(1, 4, 1);
    G1.add_edge(1, 5, 1);
    G1.add_edge(2, 3, 1);
    G1.add_edge(2, 4, 1);
    G1.add_edge(3, 4, 1);
    G1.add_edge(3, 6, 1);
    G1.add_edge(3, 7, 1);
    G1.add_edge(4, 5, 1);
    G1.add_edge(4, 6, 1);
    G1.add_edge(5, 6, 1);
    G1.add_edge(6, 7, 1);

    // Starting timer
    // std::chrono::time_point<std::chrono::system_clock> start, end;
    // start = std::chrono::system_clock::now();
    clock_t start_time, end_time, run_time;
    start_time = (double)clock();
    GA1(G1);    // Calling algorithm
    end_time = (double)clock();
    run_time = (double)(end_time - start_time);
    std::cout << "Time in ms. " << ((double)run_time/CLOCKS_PER_SEC)*1000.0 << std::endl;
    // end = std::chrono::system_clock::now();
    // std::chrono::duration<double> elapsed_seconds = end - start;
    // std::cout << "Time in ms. " << elapsed_seconds.count()*1000 << std::endl;

    return 0;
}