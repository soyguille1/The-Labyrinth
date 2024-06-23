#include <iostream> //Libreria para entrada y salida de datos
#include <vector> //Libreria  para vectores
#include <stack> //Libreria para contenedor de pilas (plato)
#include <queue> //Libreria para contenedor de Colas (parque)
#include <random> //Libreria para aleatorizar numeros
#include <chrono> //Libreria para controlar el tiempo
#include <thread> //Libreria para incluir hilos
#include <algorithm> //Libreria para utilizar algoritmos comunes para ordear,buscar,manipilar y processar datos como arreglos,vectores y listas

using namespace std;
using namespace chrono;

const char WALL = '#';
const char PATH = ' ';
const char SOLUTION = '*';
const char ENTRANCE = 'E';
const char EXIT = 'S';

struct Cell {
    int x, y; // representan las coordenadas de una celda en el laberinto 
};

// Estructura Node para almacenar información de la posición y el camino en el laberinto
struct Node {
    int x, y; //Representa las coordenadas del nodo en el laberinto
    vector<Cell> path;
};

// Función para generar un laberinto
vector<vector<char>> generateMaze(int width, int height) {
    vector<vector<char>> maze(height, vector<char>(width, WALL));
    stack<Cell> stack;
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(0, 3);

    int directions[4][2] = {{0, 2}, {2, 0}, {0, -2}, {-2, 0}};
    Cell currentCell = {1, 1};
    maze[currentCell.y][currentCell.x] = PATH;
    stack.push(currentCell);

    while (!stack.empty()) {
        currentCell = stack.top();
        stack.pop();
        vector<int> dirOrder = {0, 1, 2, 3};
        shuffle(dirOrder.begin(), dirOrder.end(), gen);

        for (int i = 0; i < 4; ++i) {
            int dx = directions[dirOrder[i]][0];
            int dy = directions[dirOrder[i]][1];
            int nx = currentCell.x + dx;
            int ny = currentCell.y + dy;

            if (nx > 0 && nx < width - 1 && ny > 0 && ny < height - 1 && maze[ny][nx] == WALL) {
                maze[ny][nx] = PATH;
                maze[currentCell.y + dy / 2][currentCell.x + dx / 2] = PATH;
                stack.push({nx, ny});
            }
        }
    }

    maze[0][1] = PATH;  // Entrada
    maze[height - 1][width - 2] = PATH;  // Salida
    return maze;
}

// Función para imprimir el laberinto
void printMaze(const vector<vector<char>>& maze) {
    for (const auto& row : maze) {
        for (char cell : row) {
            cout << cell << ' ';
        }
        cout << endl;
    }
}

// Función para imprimir el laberinto con la solución parcial
void printSolutionStep(const vector<vector<char>>& maze, const vector<Cell>& path, int step) {
    vector<vector<char>> mazeCopy = maze;
    for (int i = 0; i <= step; ++i) {
        mazeCopy[path[i].y][path[i].x] = SOLUTION;
    }

    mazeCopy[0][1] = ENTRANCE;
    mazeCopy[mazeCopy.size() - 1][mazeCopy[0].size() - 2] = EXIT;

    for (const auto& row : mazeCopy) {
        for (char cell : row) {
            cout << cell << ' ';
        }
        cout << endl;
    }
}

// Función para resolver el laberinto paso a paso
void solveMazeStepByStep(const vector<vector<char>>& maze) {
    int width = maze[0].size(), height = maze.size();
    vector<vector<bool>> visited(height, vector<bool>(width, false));
    queue<Node> q;

    q.push({1, 1, {{1, 1}}});
    visited[1][1] = true;

    int directions[4][2] = {{0, 1}, {1, 0}, {0, -1}, {-1, 0}};

    while (!q.empty()) {
        Node node = q.front();
        q.pop();

        if (node.x == width - 2 && node.y == height - 2) {
            for (int step = 0; step < node.path.size(); ++step) {
                this_thread::sleep_for(chrono::milliseconds(200));
                printSolutionStep(maze, node.path, step);
                cout << endl;
            }
            return;
        }

        for (int i = 0; i < 4; ++i) {
            int nx = node.x + directions[i][0];
            int ny = node.y + directions[i][1];

            if (nx >= 0 && nx < width && ny >= 0 && ny < height && !visited[ny][nx] && maze[ny][nx] == PATH) {
                visited[ny][nx] = true;
                vector<Cell> newPath = node.path;
                newPath.push_back({nx, ny});
                q.push({nx, ny, newPath});

                this_thread::sleep_for(chrono::milliseconds(200));
                printSolutionStep(maze, newPath, newPath.size() - 1);
                cout << endl;
            }
        }
    }

    cout << "No se encontró solución.\n";
}

// Función para obtener el tamaño del laberinto del usuario
void getUserInput(int& width, int& height) {
    cout << "Ingrese el ancho del laberinto (número de columnas, preferiblemente impar): ";
    cin >> width;
    cout << "Ingrese el alto del laberinto (número de filas, preferiblemente impar): ";
    cin >> height;

    if (height % 2 == 0) {
        height++;
    }
    if (width % 2 == 0) {
        width++;
    }
}

// Función para imprimir los tiempos de generación y solución del laberinto
void printTime(duration<double> genTime, duration<double> solveTime) {
    cout << "Tiempo de generación del laberinto: " << genTime.count() << " segundos." << endl;
    cout << "Tiempo de resolución del laberinto: " << solveTime.count() << " segundos." << endl;
}

int main() {
    int width, height;
    getUserInput(width, height);

    auto startGen = high_resolution_clock::now();
    vector<vector<char>> maze = generateMaze(width, height);
    auto endGen = high_resolution_clock::now();
    duration<double> genTime = endGen - startGen;

    cout << "Laberinto generado:\n";
    printMaze(maze);

    auto startSolve = high_resolution_clock::now();
    cout << "Resolviendo laberinto paso a paso:\n";
    solveMazeStepByStep(maze);
    auto endSolve = high_resolution_clock::now();
    duration<double> solveTime = endSolve - startSolve;

    printTime(genTime, solveTime);

    return 0;
}
