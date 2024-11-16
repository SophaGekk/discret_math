#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "euler.h"
using namespace std;

// Структура узла бинарного дерева
struct Node {
    int data;
    Node* left;
    Node* right;
 
    Node(int data) : data(data), left(nullptr), right(nullptr) {}
};

// Класс бинарного дерева поиска
class BinarySearchTree {
public:
    Node* root;

    BinarySearchTree() {
        this->root = nullptr;
    }

    // Вставка узла в дерево
    void insert(int data) {
        Node* new_node = new Node(data);

        if (root == nullptr) {
            root = new_node;
            return;
        }

        Node* current = root;
        Node* parent = nullptr;

        while (current != nullptr) {
            parent = current;

            if (data < current->data) {
                current = current->left;
            } else {
                current = current->right;
            }
        }

        if (data < parent->data) {
            parent->left = new_node;
        } else {
            parent->right = new_node;
        }
    }

    // Поиск узла в дереве
    Node* search(int data) {
        Node* current = root;

        while (current != nullptr) {
            if (data == current->data) {
                return current;
            } else if (data < current->data) {
                current = current->left;
            } else {
                current = current->right;
            }
        }

        return nullptr;
    }

    
    // Удаление узла из дерева 
    Node* remove(int data) {
        // находим узел
        Node* current = search(data);
        if (current == nullptr) {
            return nullptr;  // Узел не найден
        }

        Node* parent = nullptr;

        // Находим родительский узел 
        Node* temp = root; 
        while (temp != nullptr && temp->data != current->data) {
            parent = temp;
            if (data < temp->data) {
                temp = temp->left;
            } else {
                temp = temp->right;
            }
        }

        // Случай 1: Узел - лист
        if (current->left == nullptr && current->right == nullptr) {
            if (parent == nullptr) { // Корневой узел
                root = nullptr;
            } else if (parent->left == current) { 
                parent->left = nullptr;
            } else {
                parent->right = nullptr;
            }
            delete current;
        } 

        // Случай 2: Узел имеет одного потомка
        else if (current->left == nullptr) {
            if (parent == nullptr) { 
                root = current->right;
            } else if (parent->left == current) {
                parent->left = current->right;
            } else {
                parent->right = current->right;
            }
            delete current;
        }
        else if (current->right == nullptr) {
            if (parent == nullptr) {
                root = current->left;
            } else if (parent->left == current) {
                parent->left = current->left;
            } else {
                parent->right = current->left;
            }
            delete current;
        } 

        // Случай 3: Узел имеет двух потомков
        else {
            // Находим преемника (минимальное значение в правом поддереве)
            Node* successor = current->right;
            Node* successorParent = current; // для родительского узла преемника
            while (successor->left != nullptr) {
                successorParent = successor; // Обновляем родительский узел преемника
                successor = successor->left;
            }

            // Заменяем данные текущего узла на данные преемника
            current->data = successor->data;

            // Удаляем преемника 
            if (successorParent->left == successor) { // Если преемник - левый потомок
                successorParent->left = successor->right;
                delete successor;
            } else { // Если преемник - правый потомок
                successorParent->right = successor->right;
                delete successor;
            } 
        }

        return current; 
    }


    // Прямой обход дерева
    void preorder(Node* node) {
        if (node != nullptr) {
            cout << node->data << " ";
            preorder(node->left);
            preorder(node->right);
        }
    }

    // Обратный обход дерева
    void postorder(Node* node) {
        if (node != nullptr) {
            postorder(node->left);
            postorder(node->right);
            cout << node->data << " ";
        }
    }
    
    // Деструктор
    ~BinarySearchTree() {
        deleteTree(root);
    }
    
private:
    // функция для удаления всего дерева
    void deleteTree(Node* node) {
        if (node != nullptr) {
            deleteTree(node->left);
            deleteTree(node->right);
            delete node;
        }
    }
};

int main() {
    int choice, data;
    // Создание бинарного дерева поиска
    BinarySearchTree bst;

    std::cout << "Введите 1, чтобы создать дерево из файла, или 2, чтобы вводить данные с клавиатуры: ";
    std::cin >> choice;

    if (choice == 1) {
        // Создание линейного списка из файла4
        ifstream file("C:/Users/Sopha/Desktop/discretka/binary_tree/doc1.txt");
        vector<int> list;
        if (file.is_open()) {

            int data;
            while (file >> data) {
                list.push_back(data);
            }

            file.close();
        } else {
            cout << "Не удалось открыть файл" << endl;
        }

        // Построение бинарного дерева по числам из линейного списка
        for (int i = 0; i < list.size(); i++) {
            bst.insert(list[i]);
        }
    }
    else if (choice == 2) {
        // Ввод данных с клавиатуры
        std::cout << "Введите данные для нового узла (введите -1 для завершения): ";
        std::cin >> data;

        while (data != -1) {
            bst.insert(data);
            std::cout << "Введите данные для нового узла (введите -1 для завершения): ";
            std::cin >> data;
        }
    } else {
        std::cout << "Некорректный выбор. Допустимы только 1 или 2.\n";
        return 1; // Выход из программы с ошибкой
    }
    // Прямой обход дерева
    cout << "Прямой обход: ";
    bst.preorder(bst.root);
    cout << endl;

    // Обратный обход дерева
    cout << "Обратный обход: ";
    bst.postorder(bst.root);
    cout << endl;

    int search_value;
    cout << "Введите искомое значение: ";
    cin >> search_value;

    // Вызов метода поиска
    Node* found_node = bst.search(search_value);

    // Обработка результата поиска
    if (found_node != nullptr) {
        cout << "Элемент найден: " << found_node->data << endl;
    } else {
        cout << "Элемент не найден" << endl;
    }
    
    // Удаление узла
    int remove_value;
    cout << "Введите значение узла для удаления: ";
    cin >> remove_value;
    bst.remove(remove_value);

    // Вывод дерева после удаления
    cout << "Дерево после удаления:" << endl;
    cout << "Прямой обход: ";
    bst.preorder(bst.root);
    cout << endl;
    mainy();
    return 0;
}