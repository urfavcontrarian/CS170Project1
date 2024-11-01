#include <vector>
#include <iostream>
#include <queue>
#include <cmath>
#include <unordered_set>
#include <functional>

using namespace std;

struct Node {
    vector<vector<int>> state;  // 3x3 puzzle board
    // example input --> vector<vector<int>> puzzle
    //                    = {{1, 0, 3}, {4, 2, 6}, {7, 5, 8}};
    int Gcost;                 // Cost from the start node
    int Hcost;              // We'll use this later 
    Node* parent;

    Node(vector<vector<int>> puzzle, int g = 0, int h = 0, Node* p = nullptr)
        : state(puzzle), Gcost(g), Hcost(h), parent(p) {}
    // Comparator for priority queue (for UCS, no heuristic so just Gcost)
    int f(){
        return Gcost + Hcost;
    }
};

class Problem {
public:
    vector<vector<int>> initialState;
    vector<vector<int>> goalState;

    Problem(vector<vector<int>> init, vector<vector<int>> goal)
        : initialState(init), goalState(goal) {}

    // Check if the current state is the goal state
    bool isGoal(const vector<vector<int>>& state) const {
        return state == goalState;
    }

    int h_euclid_dist(vector<vector<int>> state, vector<vector<int>> goal){
        int distance = 0;
        for (int i = 0; i < 3; i++){
            for (int j = 0; j < 3; j++){
                int val = state[i][j];
                if (val != 0) {
                    for(int k = 0; k < 3; k++){
                        for(int l = 0; l < 3; l++){
                            if(goal[k][l] == val){
                                distance += abs(i-k) + abs(j-l);
                                break;
                            }
                        }
                    }
                }
            }
        }
        return distance;
    }

    // Find blank space position in the puzzle
    pair<int, int> findBlank(const vector<vector<int>>& state) const {
        for (int i = 0; i < 3; ++i)
            for (int j = 0; j < 3; ++j)
                if (state[i][j] == 0) return {i, j};

        cout << endl<< "ERROR, blank space does not exist" << endl;
        return {-1, -1}; // Should never reach here if input is valid
    }

    vector<vector<int>> SwapState (vector<vector<int>> state, int x1, int y1, int x2, int y2) {
        int temp = state[x2][y2];
        state[x2][y2] = state[x1][y2];
        state[x1][y1] = temp;
        return state;
    };


   /* vector<vector<int>> SwapUp (const vector<vector<int>>& constState, pair<int, int> current) {
        vector<vector<int>> state = constState;
        int currY = current.first;
        int currX = current.second;

        if(currY == 0) {
            cout << "ERROR" << endl;
            return;
        }

        int nextY = currY - 1;

        int temp1 = state[currY][currX]; // This is the original state

        state[currY][currX] = state[nextY][currX];

        return state;
    }

    vector<vector<int>> SwapDown (const vector<vector<int>>& constState, pair<int, int> current) {
        vector<vector<int>> state = constState;
        int currY = current.first;
        int currX = current.second;

        if(currY == 2) {
            cout << "ERROR" << endl;
            return;
        }

        int nextY = currY + 1;

        int temp1 = state[currY][currX]; // This is the original state

        state[currY][currX] = state[nextY][currX];

        return state;
    }

    vector<vector<int>> SwapRight (const vector<vector<int>>& constState, pair<int, int> current) {
        vector<vector<int>> state = constState;
        int currY = current.first;
        int currX = current.second;

        if(currX == 2) {
            cout << "ERROR" << endl;
            return;
        }

        int nextX = currX + 1;

        int temp1 = state[currY][currX]; // This is the original state

        state[currY][currX] = state[currY][nextX];

        return state;

    }

    vector<vector<int>> SwapLeft(const vector<vector<int>>& constState, pair<int, int> current) {
        vector<vector<int>> state = constState;
        int currY = current.first;
        int currX = current.second;

        if(currX == 0) {
            cout << "ERROR" << endl;
            return;
        }

        int nextX = currX - 1;

        int temp1 = state[currY][currX]; // This is the original state

        state[currY][currX] = state[currY][nextX];
        
        return state;

    } */

    void Moves(Node& currentNode) {

        vector<vector<int>> currentState = currentNode.state;
        pair<int, int> ghost = findBlank(currentState);

        // if(ghost.first == 0){
            // if(ghost.second == 0){
            //     vector<vector<int>> rightState = SwapRight(currentState, ghost);
            //     vector<vector<int>> downState = SwapDown(currentState, ghost);
            // }
            // else if(ghost.second == 2) {

            // }
            // vector<vector<int>> leftState = SwapLeft(currentState, ghost);
            // vector<vector<int>> rightState = SwapRight(currentState, ghost);
            // vector<vector<int>> downState = SwapDown(currentState, ghost);
        // }

        if(ghost.first != 0) {

            // currentNode->up =
            Node upNode = currentNode; 
            upNode.state = SwapState(upNode.state, ghost.first, ghost.second, ghost.first - 1, ghost.second);
            upNode.parent = &currentNode;
        }
        if(ghost.first != 2) {
            Node downNode = currentNode;
            downNode.state = SwapState(downNode.state, ghost.first, ghost.second, ghost.first + 1, ghost.second);
            downNode.parent = &currentNode;
        }
        if(ghost.second != 0) {
            Node leftNode = currentNode;
            leftNode.state = SwapState(leftNode.state, ghost.first, ghost.second, ghost.first, ghost.second - 1);
            leftNode.parent = &currentNode;
            
        }
        if(ghost.second != 2) {
            Node rightNode = currentNode;
            rightNode.state = SwapState(rightNode.state, ghost.first, ghost.second, ghost.first, ghost.second + 1);
            rightNode.parent = &currentNode;
        }
    }

    // 1 0 3
    // 4 2 6
    // 7 5 8

};

vector<vector<int>> createPuzzle(){
    
}

void menu(){
    const vector<vector<int>> Goal;
    string in = "";
    while (in != "3"){
        cout << "Hello! Welcome to the 8 puzzle generator! Please input either:" << endl;
        cout << "1. For a default puzzle" << endl << "2. For a custom puzzle" << endl << "or 3. To quit";
        cin >> in;
        // if(in == 1){
        //     vector<vector<int>> defaultPuzz{{1,0,3},{4,2,6},{7,5,8}};
        //     Node defaultNode(puzzle = defaultPuzz);
        // }
    }
}






int main() {





    return 0;
}