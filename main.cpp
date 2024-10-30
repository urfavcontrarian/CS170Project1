#include <vector>
#include <iostream>
#include <queue>
#include <unordered_set>
#include <functional>

using namespace std;

struct Node {
    std::vector<std::vector<int>> state;  // 3x3 puzzle board
    // example input --> vector<vector<int>> puzzle
    //                    = {{1, 0, 3}, {4, 2, 6}, {7, 5, 8}};
    int Gcost;                 // Cost from the start node
    // int Hcost;              // We'll use this later 
    Node* left;
    Node* right; 
    Node* down;
    Node* up;

    Node(std::vector<std::vector<int>> puzzle, int g = 0, Node* leftNode = nullptr, Node* rightNode = nullptr, Node* downNode = nullptr, Node* upNode = nullptr)
        : state(puzzle), Gcost(g), left(leftNode), right(rightNode), down(downNode), up(upNode) {}

    // Comparator for priority queue (for UCS, no heuristic so just Gcost)
    bool operator>(const Node& other) const {
        return Gcost > other.Gcost;
    }
};

class Problem {
public:
    std::vector<std::vector<int>> initialState;
    std::vector<std::vector<int>> goalState;

    Problem(std::vector<std::vector<int>> init, std::vector<std::vector<int>> goal)
        : initialState(init), goalState(goal) {}

    // Check if the current state is the goal state
    bool isGoal(const std::vector<std::vector<int>>& state) const {
        return state == goalState;
    }

    // Find blank space position in the puzzle
    std::pair<int, int> findBlank(const std::vector<std::vector<int>>& state) const {
        for (int i = 0; i < 3; ++i)
            for (int j = 0; j < 3; ++j)
                if (state[i][j] == 0) return {i, j};

        std::cout << std::endl<< "ERROR, blank space does not exist" << std::endl;
        return {-1, -1}; // Should never reach here if input is valid
    }

    vector<vector<int>> SwapState (pair<int, int> ) {}


    vector<vector<int>> SwapUp (const vector<vector<int>>& constState, pair<int, int> current) {
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

    }

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
            upNode.state = SwapUp(currentState, ghost);
        }
        if(ghost.first != 2) {
            Node downNode = currentNode;
            downNode.state = SwapDown(currentState, ghost);
        }
        if(ghost.second != 0) {
            Node leftNode = currentNode;
            leftNode.state = SwapLeft(currentState, ghost);
        }
        if(ghost.second != 2) {
            Node rightNode = currentNode;
            rightNode.state = SwapRight(currentState, ghost);
        }





    }

    // 1 0 3
    // 4 2 6
    // 7 5 8

};





int main() {





    return 0;
}