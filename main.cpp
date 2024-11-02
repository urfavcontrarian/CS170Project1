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
    int Gcost;  // Path cost from the start node
    int Hcost;  // Heuristic cost to the goal
    int depth;  // Depth of the node in the tree
    Node* parent;

    Node(vector<vector<int> > puzzle, int g = 0, int h = 0, int d = 0, Node* p = nullptr)
        : state(puzzle), Gcost(g), Hcost(h), depth(d), parent(p) {}

    int f() const { return Gcost + Hcost; }  // f(n) = g(n) + h(n)
};

// Comparator struct to compare nodes by their f(n) values for min-heap priority queue
struct CompareNodes {
    bool operator()(Node* a, Node* b) {
        return a->f() > b->f();
    }
};

class Problem {
public:
    vector<vector<int> > initialState;
    vector<vector<int> > goalState;

    Problem(vector<vector<int> > init, vector<vector<int> > goal)
        : initialState(init), goalState(goal) {}

    bool isGoal(const vector<vector<int> >& state) const {
        return state == goalState;
    }

    // Euclidean Distance Heuristic
    static int h_euclid_dist(const vector<vector<int> >& state, const vector<vector<int> >& goal) {
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

    // Misplaced Tile Heuristic
    static int h_misplaced_tiles(const vector<vector<int>>& state, const vector<vector<int>>& goal) {
        int misplaced = 0;
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                if (state[i][j] != 0 && state[i][j] != goal[i][j]) {
                    ++misplaced;
                }
            }
        }
        return misplaced;
    }

    pair<int, int> findBlank(const vector<vector<int> >& state) const {
        for (int i = 0; i < 3; ++i)
            for (int j = 0; j < 3; ++j)
                if (state[i][j] == 0) return {i, j};
        return {-1, -1};
    }

    vector<vector<int> > swapState(vector<vector<int> > state, int x1, int y1, int x2, int y2) {
        int temp = state[x2][y2];
        state[x2][y2] = state[x1][y1];
        state[x1][y1] = temp;
        return state;
    }

    vector<Node*> Moves(Node* currentNode) {
        vector<Node*> neighbors;
        vector<vector<int> > currentState = currentNode->state;
        pair<int, int> blankPos = findBlank(currentState);

        vector<pair<int, int>> directions = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};
        for (int i = 0; i < directions.size(); i++) {
            int newX = blankPos.first + directions[i].first;
            int newY = blankPos.second + directions[i].second;
            if (newX >= 0 && newX < 3 && newY >= 0 && newY < 3) {
                vector<vector<int> > newState = swapState(currentState, blankPos.first, blankPos.second, newX, newY);
                neighbors.push_back(new Node(newState, currentNode->Gcost + 1, 0, currentNode->depth + 1, currentNode));
            }
        }
        return neighbors;
    }
}

void printPuzzle(const vector<vector<int>>& puzzle) {
    for (int i = 0; i < puzzle.size(); i++) {
        for (int j = 0; j < puzzle[i].size(); j++) {
            if (puzzle[i][j] == 0) {
                cout << "b ";
            } else {
                cout << puzzle[i][j] << " ";
            }
        }
        cout << endl;
    }
    cout << endl;
}
void printSolution(const vector<vector<vector<int>>>& path) {
    for (const auto& state : path) {
        for (const auto& row : state) {
            for (int num : row) {
                cout << num << " ";
            }
            cout << endl;
        }
        cout << endl;
    }
}

void search(Problem& problem, int (*heuristic)(const vector<vector<int>>&, const vector<vector<int>>&)) {
    priority_queue<Node*, vector<Node*>, CompareNodes> frontier;
    unordered_set<string> explored;

    Node* start = new Node(problem.initialState, 0, heuristic(problem.initialState, problem.goalState));
    frontier.push(start);

    int nodesExpanded = 0;
    int maxQueueSize = 1;

    while (!frontier.empty()) {
        maxQueueSize = max(maxQueueSize, (int)frontier.size());
        Node* current = frontier.top();
        frontier.pop();

        if (problem.isGoal(current->state)) {
            cout << "Goal!!!" << endl;
            cout << "To solve this problem the search algorithm expanded a total of " << nodesExpanded << " nodes." << endl;
            cout << "The maximum number of nodes in the queue at any one time: " << maxQueueSize << "." << endl;
            cout << "The depth of the goal node was " << current->depth << "." << endl;
            vector<vector<vector<int>>> path;
            while (current.parent) {
                path.push_back(current.state);
                current = *current.parent;
            }
            path.push_back(problem.initialState);
            reverse(path.begin(), path.end());
            printSolution(path)
            return;
        }
        nodesExpanded++;
        string state_str;
        for (int i = 0; i < current->state.size(); i++) {
            for (int j = 0; j < current->state[i].size(); j++) {
                state_str += to_string(current->state[i][j]) + ",";
            }

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
        }
        explored.insert(state_str);

        vector<Node*> neighbors = problem.Moves(current);
        for (int i = 0; i < neighbors.size(); i++) {
            Node* neighbor = neighbors[i];
            neighbor->Hcost = heuristic(neighbor->state, problem.goalState);

            string neighbor_str = "";
            for (int m = 0; m < neighbor->state.size(); m++) {
                for (int n = 0; n < neighbor->state[m].size(); n++) {
                    neighbor_str += to_string(neighbor->state[m][n]) + ",";
                }
            }
            if (explored.find(neighbor_str) == explored.end()) {
                frontier.push(neighbor);
                explored.insert(neighbor_str);
            }
        }
}
};

void menu() {
    const vector<vector<int> > Goal = {{1, 2, 3}, {4, 5, 6}, {7, 8, 0}};
    cout << "Welcome to our (862260629, 862275202, 862224861) 8 puzzle solver." << endl;
    cout << "Type “1” to use a default puzzle, or “2” to enter your own puzzle." << endl;

    string choice;
    cin >> choice;

    vector<vector<int> > puzzle(3, vector<int>(3));
    if (choice == "1") {
        puzzle = {{1, 0, 3}, {4, 2, 6}, {7, 5, 8}};
    } else if (choice == "2") {
        cout << "Enter your puzzle, use a zero to represent the blank" << endl;
        cout << "Enter the first row, use space or tabs between numbers: ";
        for (int i = 0; i < 3; i++) cin >> puzzle[0][i];
        cout << "Enter the second row, use space or tabs between numbers: ";
        for (int i = 0; i < 3; i++) cin >> puzzle[1][i];
        cout << "Enter the third row, use space or tabs between numbers: ";
        for (int i = 0; i < 3; i++) cin >> puzzle[2][i];
    }

    Problem problem(puzzle, Goal);
    cin.clear()
    cout << "Enter your choice of algorithm:" << endl;
    cout << "1. Uniform Cost Search" << endl;
    cout << "2. A* with the Misplaced Tile heuristic." << endl;
    cout << "3. A* with the Euclidean distance heuristic." << endl;
    cin >> choice;

    if (choice == "1") {
        search(problem, [](const vector<vector<int>>&, const vector<vector<int>>&) { return 0; });
    } else if (choice == "2") {
        search(problem, Problem::h_misplaced_tiles);
    } else if (choice == "3") {
        search(problem, Problem::h_euclid_dist);
    }
};

int main() {
    menu();
    return 0;
}
