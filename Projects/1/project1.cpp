#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <stack>
#include <time.h>
/*
Name: Mustafa Izzet Mustu
ID: 504211564
*/

class Coordinate2D{
public:
    int x, y;
    Coordinate2D(int x, int y): x(x), y(y) {}
    Coordinate2D(): x(0), y(0) {}
    ~Coordinate2D() {};
    bool operator==(const Coordinate2D& other){
        if((this->x == other.x) && (this->y == other.y)) {
            return true;
        } else {
            return false;
        }
    }
    Coordinate2D operator=(const Coordinate2D& other){
        this->x = other.x;
        this->y = other.y;
        return *this;
    }  
};

class Ship{
public:
    Coordinate2D startCoordinate;
    Coordinate2D endCoordinate;
    std::vector<Coordinate2D> coordinates;
    Ship(Coordinate2D start, Coordinate2D end){
        startCoordinate = start;
        endCoordinate = end;
        for(int j = startCoordinate.x; j <= endCoordinate.x; ++j){
            for(int k = startCoordinate.y; k <= endCoordinate.y; ++k){
                coordinates.push_back(Coordinate2D(j,k));
            }
        }        
    }

};

class Player{
public:
    Coordinate2D init;
    int numShips;
    std::vector<Ship> ships;
    int shipArea;
    Player(Coordinate2D init, int numShips){
        this->init = init;
        this->numShips = numShips;
        this->shipArea = 0;
    }
    Player(const Player& other){
        this->init = other.init;
        this->numShips = other.numShips;
        this->ships = other.ships;
        this->shipArea = other.shipArea;
    }
    ~Player(){}
};

struct Node{
    int vertice_number;
    Coordinate2D coordinate;
    int player1ShipNumber;
    int player2ShipNumber;
    std::vector<Node*> connectedNodes; // top > left > bottom > right
};

class GameBoard{
public:
    int boardSize;
    int numNode;
    std::vector<Node> graph;
    Player* player1;
    Player* player2;
    GameBoard(int boardSize, Player* p1, Player* p2);
    void BFS(int p1index, int p2index);
    void DFS(int p1index, int p2index);
    ~GameBoard() {
    };
};

GameBoard::GameBoard(int boardSize, Player* p1, Player* p2){
    player1 = p1;
    player2 = p2;
    this->boardSize = boardSize;
    numNode = boardSize*boardSize;
    graph = std::vector<Node>(numNode);

    for(int i = 0; i < boardSize; ++i){
        for(int j = 0; j < boardSize; ++j){
            int index = i*boardSize + j;
            int top = (i-1)*boardSize + j;
            int left = i*boardSize + j-1;
            int bottom = (i+1)*boardSize + j;
            int right = i*boardSize + j+1;
            graph[index].vertice_number = index;
            graph[index].coordinate.x = i;
            graph[index].coordinate.y = j;
            
            // CREATING EDGES WITH PRIORITIES, LOWEST INDEX HAS HIGHEST PRIORITY
            if(top>=0 && top<numNode){
                graph[index].connectedNodes.push_back(&graph[top]);
            }
            if(left>=0 && left<numNode && (j!=0)){
                graph[index].connectedNodes.push_back(&graph[left]);
            }
            if(bottom>=0 && bottom<numNode){
                graph[index].connectedNodes.push_back(&graph[bottom]);
            }
            if(right>=0 && right<numNode && (j!=boardSize-1)){
                graph[index].connectedNodes.push_back(&graph[right]);
            }

            // ADD SHIPS ON THE NODE IF THERE IS ONE
            for(auto it = player1->ships.begin(); it!=player1->ships.end(); it++){
                for(auto it2 = it->coordinates.begin(); it2 != it->coordinates.end(); it2++) {
                    if(graph[index].coordinate == *it2){
                        graph[index].player1ShipNumber++;
                    }
                }
            }
            for(auto it = player2->ships.begin(); it!=player2->ships.end(); it++){
                for(auto it2 = it->coordinates.begin(); it2 != it->coordinates.end(); it2++) {
                    if(graph[index].coordinate == *it2){
                        graph[index].player2ShipNumber++;
                    }
                }
            }

        }
    }
}

void GameBoard::BFS(int p1index, int p2index){
    std::vector<bool> p1Visited(numNode, false);
    std::vector<bool> p1NeighbourList(numNode, false);
    std::vector<bool> p2Visited(numNode, false);
    std::vector<bool> p2NeighbourList(numNode, false);
    int p1KeptMemory = 0, p2KeptMemory = 0;
    int p1numVisited = 0, p2numVisited = 0;
    std::queue<int> p1queue;
    std::queue<int> p2queue;
    bool p1Win = false, p2Win = false;

    p1queue.push(p1index);
    p1KeptMemory++;
    p2queue.push(p2index);
    p2KeptMemory++;
    int u, v;

    while(!p1queue.empty() && !p2queue.empty() && !p1Win && !p2Win){

        //PLAYER 1 MAKES MOVE
        u = p1queue.front();
        p1numVisited++;
        p1Visited[u] = true;
        if(graph[u].player2ShipNumber > 0){
            player2->shipArea = player2->shipArea - graph[u].player2ShipNumber;
            graph[u].player2ShipNumber = 0;
            if(player2->shipArea == 0){
                p1Win = true;
                break;
            }
        }
        p1queue.pop();

        //PLAYER 2 MAKES MOVE
        v = p2queue.front();
        p2numVisited++;
        p2Visited[v] = true;
        if(graph[v].player1ShipNumber > 0){
            player1->shipArea = player1->shipArea - graph[v].player1ShipNumber;
            graph[v].player1ShipNumber = 0;
            if(player1->shipArea == 0){
                p2Win = true;
                break;
            }
        }               
        p2queue.pop();
        
        //ITERATE ADJACENT NODES, IF THEY ARE NOT VISITED ADD THEM TO QUEUE
        for(std::vector<Node*>::iterator it = graph[u].connectedNodes.begin(); it != graph[u].connectedNodes.end(); it++){         
            if(!p1Visited[(*it)->vertice_number] && !p1NeighbourList[(*it)->vertice_number]){
                p1queue.push((*it)->vertice_number);
                p1KeptMemory++;
                p1NeighbourList[(*it)->vertice_number] = true;
            }                 
        }
        for(std::vector<Node*>::iterator it2 = graph[v].connectedNodes.begin(); it2 != graph[v].connectedNodes.end(); it2++){         
            if(!p2Visited[(*it2)->vertice_number] && !p2NeighbourList[(*it2)->vertice_number]){
                p2queue.push((*it2)->vertice_number);
                p2KeptMemory++;
                p2NeighbourList[(*it2)->vertice_number] = true;
            }                    
        }        
    }
    std::cout << "The number of visited nodes: Player1: " << p1numVisited << ", Player2: " << p2numVisited <<std::endl;
    std::cout << "The number of nodes kept in the memory: Player1: " << p1KeptMemory << ", Player2: " << p2KeptMemory <<std::endl;
    if(p1Win){
        std::cout << "The result: Player1 won!" << std::endl;
    } else {
        std::cout << "The result: Player2 won!" << std::endl;
    }
}
void GameBoard::DFS(int p1index, int p2index){
    std::vector<bool> p1Visited(numNode, false);
    std::vector<bool> p2Visited(numNode, false);
    int p1KeptMemory = 0, p2KeptMemory = 0;
    int p1numVisited = 0, p2numVisited = 0;
    std::stack<int> p1stack;
    std::stack<int> p2stack;
    bool p1Win = false, p2Win = false;

    p1stack.push(p1index);
    p1KeptMemory++;
    p2stack.push(p2index);
    p2KeptMemory++;
    int u,v;
    while(!p1stack.empty() && !p2stack.empty() && !p1Win && !p2Win){
        //PLAYER 1 MAKES MOVE
        u = p1stack.top();
        p1numVisited++;
        p1Visited[u] = true;
        if(graph[u].player2ShipNumber > 0){
            player2->shipArea = player2->shipArea - graph[u].player2ShipNumber;
            graph[u].player2ShipNumber = 0;
            if(player2->shipArea == 0){
                p1Win = true;
                break;
            }
        }        
        p1stack.pop();

        //PLAYER 2 MAKES MOVE
        v = p2stack.top();
        p2numVisited++;
        p2Visited[v] = true;
        if(graph[v].player1ShipNumber > 0){
            player1->shipArea = player1->shipArea - graph[v].player1ShipNumber;
            graph[v].player2ShipNumber = 0;
            if(player1->shipArea == 0){
                p2Win = true;
                break;
            }
        }               
        p2stack.pop();

        //SINCE WE ARE USING STACK, ITERATE REVERSELY TO ESTABLISH PRIORITY
        for(std::vector<Node*>::reverse_iterator it = graph[u].connectedNodes.rbegin(); it != graph[u].connectedNodes.rend(); it++){         
            if(!p1Visited[(*it)->vertice_number]){
                p1stack.push((*it)->vertice_number);
                p1KeptMemory++;
            }                   
        }
        for(std::vector<Node*>::reverse_iterator it2 = graph[v].connectedNodes.rbegin(); it2 != graph[v].connectedNodes.rend(); it2++){         
            if(!p2Visited[(*it2)->vertice_number]){
                p2stack.push((*it2)->vertice_number);
                p2KeptMemory++;
            }                    
        }        
    }
    std::cout << "The number of visited nodes: Player1: " << p1numVisited << ", Player2: " << p2numVisited <<std::endl;
    std::cout << "The number of nodes kept in the memory: Player1: " << p1KeptMemory << ", Player2: " << p2KeptMemory <<std::endl;
    if(p1Win){
        std::cout << "The result: Player1 won!" << std::endl;
    } else {
        std::cout << "The result: Player2 won!" << std::endl;
    }
}

int main(int argc, char** argv) {

    if(argc != 3){
        std::cerr << "Missing arguments!" << std::endl;
        exit(1);
    }

    std::ifstream player1input;
    std::ifstream player2input;
    player1input.open(argv[1]);
    player2input.open(argv[2]);
    if (!player1input || !player2input) {
        std::cerr << "Unable to open input files" << std::endl;
        exit(1);
    }   

    std::string algorithmName;
    int boardSize;
    Coordinate2D player1Coord;
    Coordinate2D player2Coord;
    int player1NumShips;
    int player2NumShips;
    player1input >> algorithmName >> player1Coord.x >> player1Coord.y >> boardSize >> player1NumShips;
    player2input >> algorithmName >> player2Coord.x >> player2Coord.y >> boardSize >> player2NumShips;
    Player player1(player1Coord, player1NumShips);
    Player player2(player2Coord, player2NumShips);
    
    for(int i = 0; i< player1NumShips; ++i){
        Coordinate2D tmpStart, tmpEnd;
        player1input >> tmpStart.x >> tmpStart.y >> tmpEnd.x >> tmpEnd.y;
        player1.ships.push_back(Ship(tmpStart, tmpEnd));
        player1.shipArea += player1.ships[i].coordinates.size();
    }
    for(int i = 0; i< player2NumShips; ++i){
        Coordinate2D tmpStart, tmpEnd;
        player2input >> tmpStart.x >> tmpStart.y >> tmpEnd.x >> tmpEnd.y;
        player2.ships.push_back(Ship(tmpStart, tmpEnd));
        player2.shipArea += player2.ships[i].coordinates.size();
    }    

    GameBoard board(boardSize, &player1, &player2);
    std::cout << "The algorithm: Player1: " << algorithmName << " Player2 " << algorithmName << std::endl;
    clock_t start_time, end_time, run_time;
    start_time = (double)clock();
    if(algorithmName == std::string("BFS")){
        board.BFS((player1.init.x*boardSize + player1.init.y), (player2.init.x*boardSize + player2.init.y));
    } else {
        board.DFS((player1.init.x*boardSize + player1.init.y), (player2.init.x*boardSize + player2.init.y));
    }
    end_time = (double)clock();
    run_time = (double)(end_time - start_time);
    std::cout << "The running time: " << ((double)run_time/CLOCKS_PER_SEC)*1000.0 << "ms" << std::endl;

    return 0;
}





