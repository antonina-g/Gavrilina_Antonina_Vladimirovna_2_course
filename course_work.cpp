#include <iostream>
#include <queue>
#include <unordered_map> // Для хранения частот и кодов
#include <vector> // Для priority_queue
#include <memory> //для умных указателей

// Узел дерева Хаффмана
struct HuffmanNode {
    char ch; // символ
    int freq; //частота
    std::shared_ptr<HuffmanNode> left, right; //умный указатель на левого и правого потомка

    HuffmanNode(char ch, int freq) : ch(ch), freq(freq), left(nullptr), right(nullptr) {} //хранит символ (для листовых узлов)
    HuffmanNode(int freq) : ch('\0'), freq(freq), left(nullptr), right(nullptr) {} // для внутренних узлов 
};

// Компаратор для очереди с приоритетом (для того чтобы минимальный эл доставался первым)
struct Compare {
    bool operator()(const std::shared_ptr<HuffmanNode>& a, const std::shared_ptr<HuffmanNode>& b) {
        return a->freq > b->freq;
    }
};

// Генерация кодов символов (рекурсивный обход дерева)
void generateCodes(const std::shared_ptr<HuffmanNode>& root, const std::string& code, std::unordered_map<char, std::string>& codes) {
    if (!root) return; //если узла не сущетсвует - выходим
    if (!root->left && !root->right) {
        codes[root->ch] = code; //если у узла нет потомков это лист, хранящий символ
    }
    generateCodes(root->left, code + "0", codes); //влево - добавляем ноль
    generateCodes(root->right, code + "1", codes); //вправо - один
}

// Кодирование строки
std::string encode(const std::string& data, const std::unordered_map<char, std::string>& codes) {
    std::string encoded;
    for (char ch : data) {
        encoded += codes.at(ch);
    }
    return encoded;
}

// Декодирование строки
std::string decode(const std::string& encoded, const std::shared_ptr<HuffmanNode>& root) {
    std::string decoded;
    auto current = root; //начинаем с корня дерева
    for (char bit : encoded) {
        if (bit == '0') {
            current = current->left; //если равен 0 - идём влево
        } else {
            current = current->right;
        }
        if (!current->left && !current->right) {
            decoded += current->ch;
            current = root;
        }
    }
    return decoded;
}

int main() {
    std::string data = "abacabad";

    // 1. Подсчет частот символов
    std::unordered_map<char, int> freq; // Пустой словарь для частот
    for (char ch : data) {
        freq[ch]++;
    }

    // 2. Построение дерева Хаффмана
    std::priority_queue<std::shared_ptr<HuffmanNode>, std::vector<std::shared_ptr<HuffmanNode>>, Compare> heap;
    for (const auto& pair : freq) {
        heap.push(std::make_shared<HuffmanNode>(pair.first, pair.second)); //создание узлов
    }

    while (heap.size() > 1) {  //строим дерево, до тех пор пока не остался 1 элемент - корень дерева
        auto left = heap.top(); //берем самый верхний эл и удаляем его (т.е с самой маленькой частотой)
        heap.pop();
        auto right = heap.top(); //берем следующий
        heap.pop();
        auto merged = std::make_shared<HuffmanNode>(left->freq + right->freq); //создаем объединённый узел
        merged->left = left; //делаем их дочерними узлами
        merged->right = right;
        heap.push(merged); //возмращаем узел в кучу
    }

    auto root = heap.top(); //корень дерева

    // 3. Генерация кодов
    std::unordered_map<char, std::string> codes; //пустой словарь для кодов элементов
    generateCodes(root, "", codes);

    // 4. Кодирование строки
    std::string encoded = encode(data, codes);
    std::cout << "Encoded: " << encoded << std::endl;

    // 5. Декодирование
    std::string decoded = decode(encoded, root);
    std::cout << "Decoded: " << decoded << std::endl;

    // Проверка
    std::cout << (data == decoded ? "Success!" : "Error!") << std::endl;

    return 0;
}