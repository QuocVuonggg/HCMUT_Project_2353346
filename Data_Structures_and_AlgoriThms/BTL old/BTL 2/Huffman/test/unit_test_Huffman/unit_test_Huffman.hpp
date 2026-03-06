#ifndef UNIT_TEST_Huffman_HPP
#define UNIT_TEST_Huffman_HPP

#include"list/DLinkedList.h"
#include"list/XArrayList.h"
#include "app/inventory_compressor.h"
#include "unit_test.hpp"

// Macro to simplify test registration
#define REGISTER_TEST(func) registerTest(#func, [this]() { return func(); })

class UNIT_TEST_Huffman : public UNIT_TEST
{
public:
  UNIT_TEST_Huffman()
  {
    REGISTER_TEST(Huffman01);

    REGISTER_TEST(Huffman02);

    REGISTER_TEST(Huffman03);

    REGISTER_TEST(Huffman04);

    REGISTER_TEST(Huffman05);


  }

private:
  bool Huffman01();

  bool Huffman02();

  bool Huffman03();

  bool Huffman04();

  bool Huffman05();

 
};
int charHashFunc(char& key, int tablesize);
typedef HuffmanTree<3> HTree;
typedef HTree::HuffmanNode HNode;
// typedef InventoryCompressor<4> InvCompressor;
#endif // UNIT_TEST_Huffman_HPP

