//STUDENT CODE
//AMEEN BASITH (822440678)

#include <iostream>
#include <vector>
#include <string>
#include <random>
#include <stack>

#define RIGHT(y) y + 2
#define LEFT(y) y - 2
#define UP(x) x - 2
#define DOWN(x) x + 2

int size;
int length;
static std::random_device rd;           // for generating random number
static std::mt19937 mt(rd());          // for generating random number

struct Cell {
    int row;
    int column;
    bool visited;
    char value;
    std::vector<Cell *> neighbor;

    Cell(int x, int y) {
        this->row = x;
        this->column = y;
        this->visited = false;
        this->value = 'X';
    }
};

Cell *begin;

void initMaze();

void printMaze();

void setNeighbors(Cell *curNode);

void DFS();

Cell *chooseNeighbor(Cell *pType);

int randomNumGenerator(unsigned int neighborSize);

void clearBarrier(Cell *start, Cell *destination);

std::vector<std::vector<Cell>> grid;

int main(int argc, char *argv[]) {
    int numRooms;
    if (argc == 2) {
        std::cout << "You have entered " << argv[1] << std::endl;
        size = std::stoi(argv[1]);
    } else {
        std::cout << "You have not entered any number!" << std::endl;
        size = 40;
    }
    length = 2 * size + 1;

    initMaze();
    DFS();
    printMaze();

    return 0;
}

void printMaze() { //last step - printing the maze!
    for (int i = 0; i < length; i++) { //output is off for larger values (issue found only in windows)
        for (int j = 0; j < length; j++) {
            std::cout << grid[i][j].value << " ";
            if (j == length - 1)
                std::cout << "\n";
        }
    }
}


void initMaze() { //setting each index to contain a cell object
    std::vector<Cell> tempV(length, Cell(-1, -1));
    for (int i = 0; i < length; i++) {
        grid.push_back(tempV);
    }
    for (int i = 0; i < length; i++) {
        for (int j = 0; j < length; j++) {
            grid[i][j].row = i;
            grid[i][j].column = j;
            if (i > 0 && i < length - 1 && j > 0 && j < length - 1) {
                if (i % 2 != 0 && j % 2 != 0) {
                    grid[i][j].value = '.';
                }
            }
        }
    }

    for (int i = 0; i < length; i++) {
        for (int j = 0; j < length; j++) {
            if (i > 0 && i < length - 1 && j > 0 && j < length - 1) {
                if (i % 2 != 0 && j % 2 != 0) {
                    setNeighbors(&grid[i][j]);
                }
            }
        }
    }
    begin = &grid[length - 2][1];
}

// DFS traversal
void DFS() {
    std::stack<Cell *> s;
    s.push(begin);
    while (!s.empty()) {
        Cell *t = s.top();
        s.pop();
        if (t->visited) {
            continue;
        }
        for (auto it: t->neighbor) { //iterate through
            if (!it->visited)
                s.push(it); //including all cell's unvisited neighbors
        }
        if (size == 1) {
            continue;
        }
        Cell *randNeighbor = chooseNeighbor(t);
        if (randNeighbor == nullptr) {
            t->visited = true;
            continue;
        }
        clearBarrier(t, randNeighbor);
        t->visited = true;
    }
}

void clearBarrier(Cell *start, Cell *destination) {
/*    printf("start(%d, %d)\tdest(%d, %d)\n", start->row, start->column, destination->row, destination->column); //used for testing purposes*/

    if (destination->column == start->column + 2 ||
        ((destination->row == start->row) && (destination->column == start->column + 2)))
        grid[start->row][start->column + 1].value = '.'; //right

    else if (destination->column == start->column - 2 ||
             ((destination->row == start->row) && (destination->column == start->column - 2)))
        grid[start->row][start->column - 1].value = '.'; //left

    else if (destination->row == start->row - 2 ||
             ((destination->column == start->column) && (destination->row == start->row - 2)))
        grid[start->row - 1][start->column].value = '.'; //up

    else if (destination->row == start->row + 2 ||
             ((destination->column == start->column) && (destination->row == start->row + 2)))
        grid[start->row + 1][start->column].value = '.'; //down
}

Cell *chooseNeighbor(Cell *pType) {
    std::vector<Cell *> tempVec = pType->neighbor;
    for (int i = 0; i < tempVec.size(); i++) {
        int randVal = randomNumGenerator(pType->neighbor.size() - 1);
        auto randNeighbor = pType->neighbor[randVal];
        if (!randNeighbor->visited) {
//            std::cout << "random neighbor below" << std::endl; //this line and the following line are included for testing purposes
//            std::cout << randNeighbor->row << " " << randNeighbor->column << std::endl;
            return randNeighbor;
        } else {
            tempVec.erase(tempVec.begin() + i);
        }
    }
    return nullptr;
}

int randomNumGenerator(unsigned int neighborSize) {
    std::uniform_int_distribution<> dist(0, neighborSize);
    return dist(mt);;
}

void setNeighbors(Cell *curNode) {
    int x, y;
    x = curNode->row;
    y = curNode->column;

    if (UP(x) > 0) { //finding neighbors!
        curNode->neighbor.push_back(&grid[UP(x)][y]);
    }
    if (DOWN(x) < length - 1) {
        curNode->neighbor.push_back(&grid[DOWN(x)][y]);
    }
    if (LEFT(y) > 0) {
        curNode->neighbor.push_back(&grid[x][LEFT(y)]);
    }
    if (RIGHT(y) < length - 1) {
        curNode->neighbor.push_back(&grid[x][RIGHT(y)]);
    }
}
