#ifndef INVENTORY_COMPRESSOR_H
#define INVENTORY_COMPRESSOR_H

#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>
#include <stdexcept>
#include <utility>
#include "inventory.h"
#include "hash/xMap.h"
#include "heap/Heap.h"
#include "list/XArrayList.h"

inline int hashChar(char& key, int capacity) {
    return (int)key % capacity;
}

inline bool charEqual(char& lhs, char& rhs) {
    return lhs == rhs;
}

inline bool stringEqual(std::string& lhs, std::string& rhs) {
    return lhs == rhs;
}

inline bool intValueEqual(int& lhs, int& rhs) {
    return lhs == rhs;
}

template<int treeOrder>
class HuffmanTree {
public:
    struct HuffmanNode {
        char symbol;
        int freq;
        int order;
        XArrayList<HuffmanNode*> children;

        HuffmanNode(char s, int f) : symbol(s), freq(f), children() {}; //Leaf node
        HuffmanNode(int f, const  XArrayList<HuffmanNode*>& childs) : symbol('\0'), freq(f), children(childs) {}; //Internal node
    };

    HuffmanTree();
    ~HuffmanTree();

    void build(XArrayList<pair<char, int>>& symbolsFreqs);
    void generateCodes(xMap<char, std::string>& table);
    std::string decode(const std::string& huffmanCode);
    // Đặt deleteTree và traverse ở đây
    void deleteTree(HuffmanNode* node);
    void traverse(HuffmanNode* node, const string& code, xMap<char, string>& table);


private:
    HuffmanNode* root;
    int globalOrder = 0; 
};

template<int treeOrder>
class InventoryCompressor {
public:
    InventoryCompressor(InventoryManager* manager);
    ~InventoryCompressor();

    void buildHuffman();
    void printHuffmanTable();
    std::string productToString(const List1D<InventoryAttribute>& attributes, const std::string& name);
    std::string encodeHuffman(const List1D<InventoryAttribute>& attributes, const std::string& name);
    std::string decodeHuffman(const std::string& huffmanCode, List1D<InventoryAttribute>& attributesOutput, std::string& nameOutput);

private:
    xMap<char, std::string>* huffmanTable;
    InventoryManager* invManager;
    HuffmanTree<treeOrder>* tree;
};


#endif // INVENTORY_COMPRESSOR_H

template <int treeOrder>
HuffmanTree<treeOrder>::HuffmanTree() 
{
    root = nullptr;
    globalOrder = 0;
}

template <int treeOrder>
void HuffmanTree<treeOrder>::deleteTree(HuffmanNode* node) {
    if (!node) return;
    for (int i = 0; i < node->children.size(); ++i) {
        deleteTree(node->children.get(i));
    }
    delete node;
}

template <int treeOrder>
void HuffmanTree<treeOrder>::traverse(HuffmanNode* node, const string& code, xMap<char, string>& table) {
    if (!node) return;

    // Nếu là nút lá, lưu mã Huffman vào bảng
    if (node->children.empty()) {
        table.put(node->symbol, code);  // Cho phép lưu cả ký tự '\0'
        return;
    }

    for (int i = 0; i < node->children.size(); ++i) {
        char nextCode;
        if (i < 10) {
            nextCode = '0' + i;
        } else if (i < 36) {
            nextCode = 'a' + (i - 10);
        } else if (i < 62) {
            nextCode = 'A' + (i - 36);
        } else {
            throw std::runtime_error("Tree order exceeds supported encoding range.");
        }

        traverse(node->children.get(i), code + nextCode, table);
    }
}

template <int treeOrder>
HuffmanTree<treeOrder>::~HuffmanTree()
{
    //TODO
    deleteTree(root);
}

template <int treeOrder>
void HuffmanTree<treeOrder>::build(XArrayList<pair<char, int>>& symbolsFreqs)
{
    //TODO
    if (symbolsFreqs.size() == 0) return; // Không có dữ liệu
    
    XArrayList<HuffmanNode*> nodes;
    
    // Bước 1: Khởi tạo các nút lá từ symbolsFreqs
    for (int i = 0; i < symbolsFreqs.size(); ++i) {
        char symbol = symbolsFreqs.get(i).first;
        int freq = symbolsFreqs.get(i).second;
        HuffmanNode* node = new HuffmanNode(symbol, freq);
        node->order = globalOrder++;
        nodes.add(node);
    }
    
    // Bước 2: Bổ sung dummy nodes nếu cần
    int L = nodes.size();
    int remainder = (L - 1) % (treeOrder - 1);
    if (remainder != 0) {
        int dummies = (treeOrder - 1) - remainder;
        for (int i = 0; i < dummies; ++i) {
            HuffmanNode* dummy = new HuffmanNode('\0', 0);
            dummy->order = globalOrder++;
            nodes.add(dummy);
        }
    }
    
    // Bước 3: Đưa tất cả vào Heap
    Heap<HuffmanNode*> heap([](HuffmanNode*& a, HuffmanNode*& b) -> int {
        if (a->freq != b->freq) return a->freq < b->freq ? -1 : 1;
        return a->order < b->order ? -1 : 1;
    });    
    
    for (int i = 0; i < nodes.size(); ++i) {
        heap.push(nodes.get(i));
    }
    
    // Bước 4: Xây dựng cây Huffman
    while (heap.size() > 1) {
        XArrayList<HuffmanNode*> selectedNodes;
        int take = std::min(treeOrder, heap.size());
        int totalFreq = 0;
        
        for (int i = 0; i < take; ++i) {
            HuffmanNode* node = heap.pop();
            totalFreq += node->freq;
            selectedNodes.add(node);
        }
        
        // *** Phải sort lại selectedNodes ***
        for (int i = 0; i < selectedNodes.size() - 1; ++i) {
            for (int j = i + 1; j < selectedNodes.size(); ++j) {
                HuffmanNode* a = selectedNodes.get(i);
                HuffmanNode* b = selectedNodes.get(j);
    
                if (a->freq > b->freq || (a->freq == b->freq && a->order > b->order)) {
                    selectedNodes.get(i) = b;
                    selectedNodes.get(j) = a;
                }
            }
        }
    
        HuffmanNode* internalNode = new HuffmanNode(totalFreq, selectedNodes);
        internalNode->order = globalOrder++; // nhớ gán order cho node mới luôn
        heap.push(internalNode);
    }
    
    
    // Bước 5: Nút còn lại là root
    root = heap.pop();
}

template <int treeOrder>
void HuffmanTree<treeOrder>::generateCodes(xMap<char, std::string> &table)
{
    //TODO
    if (!root) return;

    // Nếu cây chỉ có một nút (nút gốc là nút lá)
    if (root->children.empty()) {
        if (root->symbol != '\0') { // Đảm bảo không lưu dummy nodes
            table.put(root->symbol, "2"); // Gán mã mặc định là "2"
        }
        return;
    }

    // Duyệt cây để sinh mã Huffman
    traverse(root, "", table);
}

template <int treeOrder>
std::string HuffmanTree<treeOrder>::decode(const std::string &huffmanCode)
{
    //TODO
    if (!root) return "";

    // Trường hợp cây chỉ có một nút (nút lá duy nhất)
    if (root->children.empty()) {
        return std::string(huffmanCode.length(), root->symbol);
    }

    std::string result = "";
    HuffmanNode* current = root;

    for (char c : huffmanCode) {
        int idx;
        if ('0' <= c && c <= '9') {
            idx = c - '0';
        } else if ('a' <= c && c <= 'z') {
            idx = 10 + (c - 'a');
        } else if ('A' <= c && c <= 'Z') {
            idx = 36 + (c - 'A');
        } else {
            throw std::runtime_error("Invalid character in Huffman code");
        }

        if (idx < 0 || idx >= current->children.size()) {
            throw std::runtime_error("Invalid Huffman code");
        }

        current = current->children.get(idx);

        if (current->children.empty()) {
            // Cho phép symbol là '\0' nếu là leaf
            result += current->symbol;
            current = root;
        }
    }

    return result;
}

template <int treeOrder>
InventoryCompressor<treeOrder>::InventoryCompressor(InventoryManager *manager) : invManager(manager), huffmanTable(nullptr), tree(nullptr)
{
    //TODO
    this->invManager = manager;
    this->huffmanTable = new xMap<char, std::string>(hashChar, 0.75, stringEqual, nullptr, charEqual, nullptr);
    this->tree = new HuffmanTree<treeOrder>();
}

template <int treeOrder>
InventoryCompressor<treeOrder>::~InventoryCompressor()
{
    //TODO
    // Giải phóng bộ nhớ được cấp phát cho bảng mã Huffman và cây Huffman
    delete huffmanTable;
    delete tree;
}

template <int treeOrder>
void InventoryCompressor<treeOrder>::buildHuffman()
{
    //TODO
    if (!invManager) return;

    xMap<char, int> freqTable(hashChar, 0.75, intValueEqual, nullptr, charEqual, nullptr);

    // Duyệt từng sản phẩm trong kho
    for (int i = 0; i < invManager->size(); ++i) {
        std::string name = invManager->getProductName(i);
        List1D<InventoryAttribute> attrs = invManager->getProductAttributes(i);
    
        std::string productStr = productToString(attrs, name);
    
        for (char c : productStr) {
            if (freqTable.containsKey(c)) {
                int oldFreq = freqTable.get(c);
                freqTable.put(c, oldFreq + 1);
            } else {
                freqTable.put(c, 1);
            }
        }
    }

    // Chuyển freqTable thành XArrayList<pair<char,int>>
    DLinkedList<char> keys = freqTable.keys();
    XArrayList<char> sortedKeys;

    for (char c : keys) {
        sortedKeys.add(c);
    }

    // Bubble sort theo mã ASCII
    for (int i = 0; i < sortedKeys.size() - 1; ++i) {
        for (int j = i + 1; j < sortedKeys.size(); ++j) {
            char a = sortedKeys.get(i);
            char b = sortedKeys.get(j);
            if (a > b) {
                // Swap bằng cách xóa từng phần tử rồi thêm lại ở đúng vị trí
                // Ghi nhớ: phải xóa index lớn hơn trước để tránh thay đổi vị trí phần tử còn lại
                sortedKeys.removeAt(j);
                sortedKeys.removeAt(i);
                sortedKeys.add(i, b);  // b vào i
                sortedKeys.add(j, a);  // a vào j
            }
        }
    }

    XArrayList<std::pair<char, int>> symbolsFreqs;
    for (int i = 0; i < sortedKeys.size(); ++i) {
        char c = sortedKeys.get(i);
        symbolsFreqs.add(std::make_pair(c, freqTable.get(c)));
    }


    // Build Huffman Tree
    tree->build(symbolsFreqs);

    // Sinh bảng mã Huffman
    tree->generateCodes(*huffmanTable);
}

template <int treeOrder>
void InventoryCompressor<treeOrder>::printHuffmanTable() {
    DLinkedList<char> keys = huffmanTable->keys();
    for (char ch : keys) {
        std::cout << "'" << ch << "' : " << huffmanTable->get(ch) << std::endl;
    }
}

template <int treeOrder>
std::string InventoryCompressor<treeOrder>::productToString(const List1D<InventoryAttribute> &attributes, const std::string &name)
{
    //TODO
    stringstream oss;
    oss << name << ":";
    for (int i = 0; i < attributes.size(); ++i) {
        const InventoryAttribute& attribute = attributes.get(i);
        oss << "(" << attribute.name << ": " << std::fixed << std::setprecision(6) << attribute.value << ")";
        if (i < attributes.size() - 1) {
            oss << ", ";
        }
    }
    return oss.str();
}

template <int treeOrder>
std::string InventoryCompressor<treeOrder>::encodeHuffman(const List1D<InventoryAttribute> &attributes, const std::string &name)
{
    //TODO
    string productString = productToString(attributes, name);

    string huffmanCode;
    for (char ch : productString) {
        if (huffmanTable->containsKey(ch)) {
            huffmanCode += huffmanTable->get(ch);
        } else {
            throw invalid_argument("Character not found in Huffman table: " + string(1, ch));
        }
    }

    return huffmanCode;
}

template <int treeOrder>
std::string InventoryCompressor<treeOrder>::decodeHuffman(const std::string &huffmanCode, List1D<InventoryAttribute> &attributesOutput, std::string &nameOutput)
{
    //TODO
    std::string decodedString = tree->decode(huffmanCode);

    size_t colonPos = decodedString.find(':');
    if (colonPos == std::string::npos) {
        throw std::invalid_argument("Invalid decoded string format: Missing ':' for product name.");
    }

    nameOutput = decodedString.substr(0, colonPos);
    std::string attributesPart = decodedString.substr(colonPos + 1);

    for (size_t i = 0; i < attributesPart.size();) {
        if (attributesPart[i] == '(') {
            size_t j = attributesPart.find(')', i);
            if (j == std::string::npos) {
                throw std::invalid_argument("Invalid format: Missing closing ')' for attribute.");
            }

            std::string attr = attributesPart.substr(i + 1, j - i - 1);
            size_t innerColon = attr.find(':');
            if (innerColon == std::string::npos) {
                throw std::invalid_argument("Invalid attribute format: Missing ':' inside ().");
            }

            std::string attrName = attr.substr(0, innerColon);
            double value = std::stod(attr.substr(innerColon + 1));
            attributesOutput.add(InventoryAttribute(attrName, value));

            i = j + 1;
            if (i < attributesPart.size() && attributesPart[i] == ',') {
                i += 2;
            }
        } else {
            ++i;
        }
    }
    return decodedString;
}