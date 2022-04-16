#include <iostream>
// #include <chrono>
#include <time.h>
#include <vector>

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

// Merge algorithm for 2 subarray, 
void merge(std::vector<std::pair<int,int> >* degree_list, int left, int mid, int right){
    int num_left = mid - left + 1;
    int num_right = right - mid;

    std::vector<std::pair<int,int> > sorted(num_left+num_right); // Temporary sorted array 
    int i = left;   // Left array starting index
    int j = mid+1;  // Right array starting index
    int k = left;   // Array starting index
    int s = 0;      // Sorted array starting index

    // Create an sorted array from 2 subarrays
    while(i <= mid && j <= right) {
        if((*degree_list)[i].second >= (*degree_list)[j].second){
            sorted[s].first = (*degree_list)[i].first;
            sorted[s].second = (*degree_list)[i].second;
            i++;
        } else {
            sorted[s].first = (*degree_list)[j].first;
            sorted[s].second = (*degree_list)[j].second;
            j++;
        }
        s++;
    }

    // If any left add them to sorted array
    while(i <= mid) {
        sorted[s].first = (*degree_list)[i].first;
        sorted[s].second = (*degree_list)[i].second;
        i++;
        s++;
    }
    while(j <= right) {
        sorted[s].first = (*degree_list)[j].first;
        sorted[s].second = (*degree_list)[j].second;
        j++;
        s++;
    }

    // Change original array by looking at sorted new array
    s = 0;
    for(k=left; k<=right; k++){
        (*degree_list)[k].first = sorted[s].first;
        (*degree_list)[k].second = sorted[s].second;
        s++;
    }
}

// Merge Sort Algorithm
void mergeSort(std::vector<std::pair<int,int> >* degree_list, int begin, int end){
    if(begin>=end) {
        return;
    }
    int mid = begin + (end-begin)/2;
    mergeSort(degree_list, begin, mid);
    mergeSort(degree_list, (mid+1), end);
    merge(degree_list, begin, mid, end);
}

// Greedy Algorithm 2
void GA2(const Graph& g){
    int vertex_num = g.num_vertices-1;
    int vertex_colors[vertex_num+1];

    // Step-1: Find the degree of each vertex.   
    std::vector<std::pair<int,int> > degree_list(vertex_num);   // first->vertex number, second->degree
    for(int i=0; i<vertex_num; i++){
        int degree=0;
        for(int j=0; j<vertex_num; j++){
            if(g.graph[i+1][j+1] == 1) {
                degree++;
            }
        }
        degree_list[i].first = i+1;
        degree_list[i].second = degree;
    }

    // Step-2: List the vertices in descending order of degrees in a degree_list.
    mergeSort(&degree_list, 0, vertex_num-1);
    
    int color_num = 0;
    // Step-5: For the remaining vertices repeat the process with a new color until there are no uncolored vertices, keeping in mind the following rules:
    while(!degree_list.empty()) {

        // Step-3: Color the first vertex (having the highest degree) with color 1.
        int current_vertex = degree_list.begin()->first;
        color_num++;
        vertex_colors[current_vertex] = color_num;
        std::cout << "Vertex " << current_vertex << " --> Color " << color_num << std::endl;
        std::vector<int> popped;
        popped.push_back(current_vertex);
        std::vector<std::pair<int, int> >::iterator it = degree_list.begin();
        it = degree_list.erase(it);

        // Step-4: In degree_list go down and color the vertices that are not connected to first vertex with the same color 1. Then drop the visited vertices from the degree_list.
        while(it != degree_list.end()){
            std::cout << "Checking " << it->first;
            //If current vertex is adjacent of starting index of current period, do not color it
            if(g.graph[current_vertex][it->first] != 0) {
                std::cout << " --> false" << std::endl;
                it++;
            } else {
                bool isConnected = false;   // If current vertex is adjacent of one of the new colored vertex, do not color it 
                for(std::vector<int>::iterator a = popped.begin(); a != popped.end(); a++) {
                    if(g.graph[it->first][*a] == 1){
                        std::cout << " --> false (since it is connected to " << *a << ")" << std::endl;
                        isConnected = true;
                        it++;
                        break;
                    }
                }
                if(!isConnected) {
                    std::cout << " --> true" << std::endl;
                    vertex_colors[it->first] = color_num;
                    popped.push_back(it->first);
                    std::cout << "Vertex " << it->first << " --> Color " << color_num << std::endl;
                    it = degree_list.erase(it);                    
                }
            }
        }
        
        // Printing dropped vertices from degree list
        std::cout << "Vertices ";
        std::vector<int>::iterator pit = popped.begin();
        while( pit != popped.end()){
            std::cout << *pit;
            if((pit + 1) != popped.end()){
                std::cout << ", ";
            } else {
                std::cout << " ";
            }
            pit = popped.erase(pit);
            
        }
        std::cout << "are dropped!!" << std::endl << std::endl;
    }
    
    std::cout << "Well done!! All the vertices are colored." << std::endl;
    std::cout << "Min color num:" << color_num << std::endl << std::endl;
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
    GA2(SNG);    // Calling algorithm
    end_time = (double)clock();
    run_time = (double)(end_time - start_time);
    std::cout << "Time in ms. " << ((double)run_time/CLOCKS_PER_SEC)*1000.0 << std::endl;
    // end = std::chrono::system_clock::now();
    // std::chrono::duration<double> elapsed_seconds = end - start;
    // std::cout << "Time in ms. " << elapsed_seconds.count()*1000 << std::endl;

    return 0;
}