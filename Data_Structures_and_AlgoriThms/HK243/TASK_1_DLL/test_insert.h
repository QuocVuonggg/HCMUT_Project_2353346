#include "utils.h"

void test_insertAtHead_basic()
{
    DoublyLinkedList<int> list;
    list.insertAtHead(10);
    EXPECT_EQ(list.size(), 1);
    EXPECT_EQ(list.get(0), 10);
}

void test_insertAtTail_basic()
{
    DoublyLinkedList<int> list;
    list.insertAtTail(20);
    EXPECT_EQ(list.size(), 1);
    EXPECT_EQ(list.get(0), 20);
}

void test_insertAt_middle()
{
    DoublyLinkedList<int> list;
    list.insertAtTail(1);
    list.insertAtTail(3);
    list.insertAt(1, 2); // insert in the middle
    EXPECT_EQ(list.size(), 3);
    EXPECT_EQ(list.get(0), 1);
    EXPECT_EQ(list.get(1), 2);
    EXPECT_EQ(list.get(2), 3);
}

void test_insertAt_beginning()
{
    DoublyLinkedList<int> list;
    list.insertAtTail(2);
    list.insertAtTail(3);
    list.insertAt(0, 1); // insert at beginning
    EXPECT_EQ(list.get(0), 1);
}

void test_insertAt_end()
{
    DoublyLinkedList<int> list;
    list.insertAtTail(1);
    list.insertAtTail(2);
    list.insertAt(2, 3); // insert at end
    EXPECT_EQ(list.get(2), 3);
}

void test_combined_insert_1()
{
    DoublyLinkedList<int> list;
    list.insertAtHead(2);
    list.insertAtTail(4);
    list.insertAt(1, 3); // between 2 and 4
    EXPECT_EQ(list.get(0), 2);
    EXPECT_EQ(list.get(1), 3);
    EXPECT_EQ(list.get(2), 4);
}

void test_combined_insert_2()
{
    DoublyLinkedList<int> list;
    list.insertAtHead(3);
    list.insertAtHead(2);
    list.insertAtHead(1);
    list.insertAt(1, 9); // 1, 9, 2, 3
    EXPECT_EQ(list.get(1), 9);
}

void test_combined_insert_3()
{
    DoublyLinkedList<int> list;
    list.insertAtTail(10);
    list.insertAtTail(20);
    list.insertAtTail(30);
    list.insertAt(0, 5); // at head
    EXPECT_EQ(list.get(0), 5);
}

void test_combined_insert_4()
{
    DoublyLinkedList<int> list;
    list.insertAtTail(100);
    list.insertAtHead(50); // becomes head
    list.insertAt(1, 75);  // between 50 and 100
    EXPECT_EQ(list.get(0), 50);
    EXPECT_EQ(list.get(1), 75);
    EXPECT_EQ(list.get(2), 100);
}

void test_combined_insert_5()
{
    DoublyLinkedList<int> list;
    list.insertAtHead(3);
    list.insertAtTail(6);
    list.insertAt(1, 4);
    list.insertAt(2, 5);
    list.insertAt(0, 2);
    list.insertAt(0, 1);
    // Final: 1 2 3 4 5 6
    for (int i = 0; i < 6; ++i)
        EXPECT_EQ(list.get(i), i + 1);
}

void test_insertAt_negative_index()
{
    DoublyLinkedList<int> list;
    list.insertAtTail(1);
    try
    {
        list.insertAt(-1, 100);
        throw std::runtime_error("Expected std::out_of_range not thrown");
    }
    catch (const std::out_of_range &e)
    {
        EXPECT_EQ(std::string(e.what()), "Index is invalid!");
    }
}

void test_insertAt_too_large_index()
{
    DoublyLinkedList<int> list;
    list.insertAtTail(1);
    try
    {
        list.insertAt(2, 100); // size = 1, index = 2 → invalid
        throw std::runtime_error("Expected std::out_of_range not thrown");
    }
    catch (const std::out_of_range &e)
    {
        EXPECT_EQ(std::string(e.what()), "Index is invalid!");
    }
}

void test_insertAt_index_0_and_size()
{
    DoublyLinkedList<int> list;
    list.insertAt(0, 1); // hợp lệ vì list đang rỗng (size = 0)
    list.insertAt(1, 3); // chèn tại cuối (index = size hiện tại = 1)
    list.insertAt(1, 2); // chèn vào giữa: 1, 2, 3
    EXPECT_EQ(list.size(), 3);
    EXPECT_EQ(list.get(0), 1);
    EXPECT_EQ(list.get(1), 2);
    EXPECT_EQ(list.get(2), 3);
}
