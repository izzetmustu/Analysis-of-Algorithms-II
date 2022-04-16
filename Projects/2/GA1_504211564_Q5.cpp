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
int findSmallestMissing(std::list<int> const &nums)
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
    int num_vertices = 35;
    Graph SNG(num_vertices); // Creating edges
    SNG.add_edge(1, 2, 1);
    SNG.add_edge(1, 7, 1);
    SNG.add_edge(2, 3, 1);
    SNG.add_edge(2, 6, 1);
    SNG.add_edge(3, 4, 1);
    SNG.add_edge(4, 5, 1);
    SNG.add_edge(4, 11, 1);
    SNG.add_edge(4, 12, 1);
    SNG.add_edge(5, 6, 1);
    SNG.add_edge(6, 7, 1);
    SNG.add_edge(6, 10, 1);
    SNG.add_edge(7, 8, 1);
    SNG.add_edge(7, 9, 1);
    SNG.add_edge(8, 9, 1);
    SNG.add_edge(8, 21, 1);
    SNG.add_edge(9, 10, 1);
    SNG.add_edge(9, 20, 1);
    SNG.add_edge(10, 11, 1);
    SNG.add_edge(10, 18, 1);
    SNG.add_edge(10, 20, 1);
    SNG.add_edge(11, 12, 1);
    SNG.add_edge(11, 17, 1);
    SNG.add_edge(12, 13, 1);
    SNG.add_edge(13, 14, 1);
    SNG.add_edge(13, 16, 1);
    SNG.add_edge(14, 15, 1);
    SNG.add_edge(15, 16, 1);
    SNG.add_edge(15, 27, 1);
    SNG.add_edge(16, 17, 1);
    SNG.add_edge(16, 27, 1);
    SNG.add_edge(17, 18, 1);
    SNG.add_edge(17, 25, 1);
    SNG.add_edge(17, 26, 1);
    SNG.add_edge(18, 19, 1);
    SNG.add_edge(18, 32, 1);
    SNG.add_edge(19, 23, 1);
    SNG.add_edge(19, 24, 1);
    SNG.add_edge(20, 21, 1);
    SNG.add_edge(20, 22, 1);
    SNG.add_edge(21, 22, 1);
    SNG.add_edge(22, 23, 1);
    SNG.add_edge(22, 34, 1);
    SNG.add_edge(22, 35, 1);
    SNG.add_edge(23, 24, 1);
    SNG.add_edge(23, 33, 1);
    SNG.add_edge(24, 32, 1);
    SNG.add_edge(24, 33, 1);
    SNG.add_edge(25, 26, 1);
    SNG.add_edge(25, 30, 1);
    SNG.add_edge(26, 27, 1);
    SNG.add_edge(26, 30, 1);
    SNG.add_edge(27, 28, 1);
    SNG.add_edge(27, 30, 1);
    SNG.add_edge(28, 29, 1);
    SNG.add_edge(29, 30, 1);
    SNG.add_edge(29, 31, 1);
    SNG.add_edge(30, 31, 1);
    SNG.add_edge(30, 32, 1);
    SNG.add_edge(31, 32, 1);
    SNG.add_edge(32, 33, 1);
    SNG.add_edge(33, 34, 1);
    SNG.add_edge(34, 35, 1);

    // Starting timer
    // std::chrono::time_point<std::chrono::system_clock> start, end;
    // start = std::chrono::system_clock::now();
    clock_t start_time, end_time, run_time;
    start_time = (double)clock();
    GA1(SNG);    // Calling algorithm
    end_time = (double)clock();
    run_time = (double)(end_time - start_time);
    std::cout << "Time in ms. " << ((double)run_time/CLOCKS_PER_SEC)*1000.0 << std::endl;
    // end = std::chrono::system_clock::now();
    // std::chrono::duration<double> elapsed_seconds = end - start;
    // std::cout << "Time in ms. " << elapsed_seconds.count()*1000 << std::endl;

    return 0;
}