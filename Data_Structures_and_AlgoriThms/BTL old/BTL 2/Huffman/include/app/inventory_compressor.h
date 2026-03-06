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

//! trước tiên để cái này ở đây nha 
namespace std {
    inline ostream& operator<<(ostream& os, const pair<char, int>& p) {
        os << "(" << p.first << ", " << p.second << ")";
        return os;
    }
}
//! trước tiên để cái này ở đây nha 


template<int treeOrder>
class HuffmanTree {
public:
    struct HuffmanNode {
        char symbol;
        int freq;
        XArrayList<HuffmanNode*> children;

        HuffmanNode(char s, int f); //Leaf node
        HuffmanNode(int f, const  XArrayList<HuffmanNode*>& childs); //Internal node
    };

    HuffmanTree();
    ~HuffmanTree();

    void build(XArrayList<pair<char, int>>& symbolsFreqs);
    void generateCodes(xMap<char, std::string>& table);
    std::string decode(const std::string& huffmanCode);

private:
    HuffmanNode* root;
};




#endif // INVENTORY_COMPRESSOR_H

template <int treeOrder>
HuffmanTree<treeOrder>::HuffmanTree()
{
}

template <int treeOrder>
HuffmanTree<treeOrder>::~HuffmanTree()
{
    //TODO
}

template <int treeOrder>
void HuffmanTree<treeOrder>::build(XArrayList<pair<char, int>>& symbolsFreqs)
{
    //TODO
}

template <int treeOrder>
void HuffmanTree<treeOrder>::generateCodes(xMap<char, std::string> &table)
{
    //TODO
}

template <int treeOrder>
std::string HuffmanTree<treeOrder>::decode(const std::string &huffmanCode)
{
    //TODO
    return std::string();
}

