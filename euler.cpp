#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <string>
#include <random>
#include <ctime>
#include "euler.h"
#include <stack>
using namespace std;
// Структура для представления ребра
struct Edge {
    int u, v; // Номера вершин
};

// Структура для представления графа
struct Graph {
    int V; // Количество вершин
    int E; // Количество ребер
    bool oriented; // Флаг ориентированности
    vector<int> isOddDegree; // Флаг нечетной степени для каждой вершины
    vector<vector<bool>> matrix; // Матрица смежности
    vector<int> inDegree; // Входящая степень для каждой вершины 
    vector<int> outDegree; // Исходящая степень для каждой вершины
    Graph(int V, bool oriented) : V(V), E(0), oriented(oriented), inDegree(V, 0), outDegree(V, 0), matrix(V, vector<bool>(V, false)) {}

    // Добавление ребра в граф
    void addEdge(int u, int v) {
        matrix[u][v] = true;
        outDegree[u]++;
        inDegree[v]++; 
        if (!oriented) { // Для неориентированного графа
            matrix[v][u] = true;
        }
        E++;
    }
};

// Подсчет вершин с нечетной степенью (входящие и выходящие ребра)
void calculateDegree(Graph& graph) {
    graph.isOddDegree.clear();
    for (int v = 0; v < graph.V; ++v)
    {
        int inDegree = 0;
        int outDegree = 0;
        for (int u = 0; u < graph.V; ++u) {
            if (graph.matrix[u][v]) { // Входящее ребро
                inDegree++;
            }
            if (graph.matrix[v][u] && graph.oriented) { // Выходящее ребро
                outDegree++;
            }
        }
        if ((inDegree + outDegree) % 2 == 1) {

            graph.isOddDegree.push_back(v);
        }
    }
}

// Проверка, является ли граф сильно связным 
bool isStronglyConnected(Graph& graph) { 
    vector<vector<bool>> matrix = graph.matrix; // Копируем матрицу смежности  
    //алгоритм для проверки связности 
    for (int k = 0; k < graph.V; ++k) { 
        for (int i = 0; i < graph.V; ++i) { 
            for (int j = 0; j < graph.V; ++j) { 
                if (matrix[i][k] && matrix[k][j]) { 
                    matrix[i][j] = true; 
                } 
            } 
        } 
    } 
 
    // Проверяем, есть ли путь из каждой вершины в каждую 
    for (int i = 0; i < graph.V; ++i) { 
        for (int j = 0; j < graph.V; ++j) { 
            if (i != j && !matrix[i][j]) { 
                return false; // Граф не сильно связан 
            } 
        } 
    } 
    return true; // Граф сильно связан 
} 

// Проверка, является ли граф эйлеровым 
bool checkCycle_for_ORIENTED(Graph& graph) { 
    // Проверка условия для Эйлерова цикла:
    // 1. Граф должен быть сильно связным или состоять из одной компоненты сильной связности, содержащей ребра, при этом все остальные компоненты являются изолированными вершинами.
    if (!isStronglyConnected(graph)) {
        // Проверка на наличие изолированных вершин
        bool hasIsolatedVertices = false;
        for (int i = 0; i < graph.V; ++i) {
            if (graph.inDegree[i] == 0 && graph.outDegree[i] == 0) {
                hasIsolatedVertices = true;
                break;
            }
        }
        if (hasIsolatedVertices) {
            // Проверка, что все вершины, не входящие в сильно связную компоненту, изолированы
            for (int i = 0; i < graph.V; ++i) {
                if (graph.inDegree[i] != graph.outDegree[i]) {
                    return false; // Граф не является эйлеровым
                }
            }
        } else {
            return false; // Граф не является эйлеровым
        }
    }
    
    // 2. Для каждой вершины графа входящая степень равна исходящей степени.
    for (int i = 0; i < graph.V; ++i) {
        if (graph.inDegree[i] != graph.outDegree[i]) {
            return false; // Граф не является эйлеровым 
        }
    }
    return true;
} 
// Проверка, является ли граф эйлеровым 
bool checkWay_for_ORIENTED(Graph& graph) { 
    // Проверка условия для Эйлерова пути:
    // 1. Граф должен быть слабо связан
    // 2. Существует ровно две вершины с неравными степенями:
    //    одна из них должна иметь степень выхода на 1 больше степени входа (начальная вершина),
    //    а другая — степень входа на 1 больше степени выхода (конечная вершина). 
    //    Все остальные вершины должны иметь равные входящие и исходящие степени
    int oddDegreeCount = 0;
    for (int i = 0; i < graph.V; ++i) {
        if (graph.inDegree[i] != graph.outDegree[i]) {
            oddDegreeCount++;
            if(graph.outDegree[i] == graph.inDegree[i] + 1)
            {
                graph.isOddDegree.push_back(i);
            }
            else if(graph.inDegree[i] == graph.outDegree[i] + 1)
            {
                
            }
            else{return false;}
        }
    }
    if (oddDegreeCount > 2) {
        return false; // Граф не является эйлеровым
    }
    return true;
} 
// Проверка, является ли граф эйлеровым
bool checkForEulerPath(Graph& graph) {
    if (graph.oriented) {
        if (!checkCycle_for_ORIENTED(graph) && !checkWay_for_ORIENTED(graph)) { 
            return false; // Граф не является эйлеровым
        } 
        return true; // Граф является эйлеровым
    }     
    else
    {
        calculateDegree(graph); 
        // Если количество вершин с нечетной степенью больше двух, то граф не является эйлеровым
        if (graph.isOddDegree.size() > 2) {
            return false;
        }
        // Проверка на связность (используем матрицу смежности)
        vector<bool> visited(graph.V, false);
        visited[0] = true; // Стартовая вершина
        for (int i = 0; i < graph.V; ++i) {
            if (visited[i]) {
                for (int j = 0; j < graph.V; ++j) {
                    if (graph.matrix[i][j] && !visited[j]) {
                        visited[j] = true;
                    }
                }
            }
        }
        for (int v = 0; v < graph.V; ++v) {
            if (graph.matrix[v][v] && !visited[v]) { // Проверяем петли
                return false;
            }
        }
        return true;
    }
}

// Поиск эйлерова цикла или пути
void findEulerPath(Graph& graph) {
    // Проверка, является ли граф эйлеровым
    if (!checkForEulerPath(graph)) {
        cout << "Граф не является эйлеровым" << endl;
        return;
    }

    // Находим вершину с нечетной степенью (если она есть)
    int startVertex = 0;
    if(graph.isOddDegree.size() != 0)
    {
        startVertex = graph.isOddDegree[0];   
    }
    string str = "";
    // Инициализация стека и массивов для отслеживания посещенных ребер
    stack<int> S;
    vector<vector<bool>> vis = graph.matrix;  // Копируем матрицу смежности для отслеживания посещенных ребер
    // Добавляем начальную вершину в стек
    S.push(startVertex);

    // Основной цикл обхода
    while (!S.empty()) {
        int w = S.top();
        // Ищем доступное ребро из текущей вершины
        bool foundEdge = false;
        for (int u = 0; u < graph.V; ++u) {
            if (graph.matrix[w][u] && vis[w][u]) { 
                // Добавляем новую вершину в стек
                S.push(u);
                // Помечаем ребро как посещенное
                vis[w][u] = false;
                if (!graph.oriented) { // Для неориентированного графа
                    vis[u][w] = false; 
                }
                foundEdge = true;
                break;
            }
        }

        // Если не нашлось доступного ребра, то выводим текущую вершину и удаляем ее из стека
        if (!foundEdge) {
            S.pop();
            str+= std::to_string(w) + " ";
            cout << w << " ";
        }
    }
    cout << endl;
    reverse(str.begin(), str.end()); 
    cout<<str<<endl;
}

int mainy() {
    int V;
    bool oriented, isoriented;
    int choice1, choice;
    do {
        cout << "\nМеню:\n";

        cout << "1. Ориентированный:\n";
        cout << "2. Неориентированный:\n";
        cout << "Введите ваш выбор: ";
        cin >> choice1;
        
        switch (choice1) {
            case 1: {
                oriented = true;
                break;
            }
            case 2: {
                oriented = false;
                break;
            }
            default:
                cout << "Неверный выбор.\n";
        }
    } while (choice1 != 1 && choice1 != 2);
    cout << "Выбрано: " << (oriented ? "ориентированный" : "неориентированный") << endl;
    cout << "Введите количество вершин: ";
    cin >> V;
    Graph graph(V,oriented);

    do {
        cout << "\nМеню:\n";
        cout << "1. Добавить ребро\n";
        cout << "2. Найти эйлеров цикл\n";
        cout << "3. Выход\n";
        cout << "Введите ваш выбор: ";
        cin >> choice;

        switch (choice) {
            case 1: {
                int u, v;
                cout << "Введите номера вершин для ребра (u v): ";
                cin >> u >> v;
                graph.addEdge(u, v);
                break;
            }
            case 2: {
                findEulerPath(graph);
                break;
            }
            case 3:
                cout << "Выход из программы.\n";
                break;
            default:
                cout << "Неверный выбор.\n";
        }
    } while (choice != 3);

    return 0;
}
