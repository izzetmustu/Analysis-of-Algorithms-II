#include <iostream>
#include <fstream>

/*
Name: Mustafa Izzet Mustu
ID: 504211564
*/

// Directional Graph
class Graph{
public:
    int num_vertices;
    int** adjacencyMatrix;          // Adjacency matrix 
    float** probabilityMatrix;      // Probability matrix 
    
    Graph(int num_vertices){
        this->num_vertices = num_vertices+1;
        adjacencyMatrix = new int*[this->num_vertices];
        probabilityMatrix = new float*[this->num_vertices];
        for(int i = 0; i < this->num_vertices; i++){
            adjacencyMatrix[i] = new int[this->num_vertices];
            probabilityMatrix[i] = new float[this->num_vertices];
            for(int j = 0; j < this->num_vertices; j++){
                adjacencyMatrix[i][j] = 0;
                probabilityMatrix[i][j] = 0;
            }
        }   
    }
    
    // Adding edge weight
    void add_edge(int v1, int v2, int weight){
        adjacencyMatrix[v1][v2] = weight;
    }

    // Adding edge removals probability
    void add_probability(int v1, int v2, float probability){
        probabilityMatrix[v1][v2] = probability;
    }

    // Print adjacency matrix
    void printAdjacencyMatrix(){
        std::cout << "Adjacency Matrix" << std::endl;
        for(int i = 1; i < num_vertices; i++){
            std::cout << i << "\t";
            for(int j = 1; j < num_vertices; j++){
                std::cout << adjacencyMatrix[i][j] << " ";
            }
            std::cout << std::endl;
        }
    }

    // Print probability matrix
    void printProbabilityMatrix(){
        std::cout << "Probability Matrix" << std::endl;
        for(int i = 1; i < num_vertices; i++){
            std::cout << i << "\t";
            for(int j = 1; j < num_vertices; j++){
                std::cout << probabilityMatrix[i][j] << " ";
            }
            std::cout << std::endl;
        }
    }

    ~Graph(){
        for(int i = 0; i < num_vertices; i++){
            delete[] adjacencyMatrix[i];
            delete[] probabilityMatrix[i];
        }
        delete[] adjacencyMatrix;
        delete[] probabilityMatrix;
    }
};




int main(int argc, char** argv){
    if(argc != 4){
        std::cerr << "Invalid number of arguments!" << std::endl;
        std::cerr << "Correct form: ./homework3 [input_file] [src_node] [dst_node]" << std::endl;
        return 0;
    }

    std::string inputFileName = argv[1];
    std::ifstream file;
    file.open(inputFileName);
    if(!file){
        std::cerr << "Could not open input file!" << std::endl;
    }
    std::cout << inputFileName << std::endl;
    int s = std::stoi(argv[2]);
    int t = std::stoi(argv[3]);
    std:: cout << "s: " << s << " t: " << t << std::endl;
    
    int cardinality;

    file >> cardinality;
    Graph g1(cardinality);
    for(int i = 1; i <= cardinality; i++){
        int tmp;
        for(int j = 1; j <= cardinality; j++){
            file >> tmp;
            g1.add_edge(i,j,tmp);
        }
    }
    g1.printAdjacencyMatrix();

    for(int i = 1; i <= cardinality; i++){
        float tmp;
        for(int j = 1; j <= cardinality; j++){
            file >> tmp;
            g1.add_probability(i,j,tmp);
        }
    }
    g1.printProbabilityMatrix();

    return 0;
}