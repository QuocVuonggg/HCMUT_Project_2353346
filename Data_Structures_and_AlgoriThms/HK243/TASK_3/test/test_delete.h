#include "../utils.h"

void test_deleteAt_head()
{
    DoublyLinkedList<int> list;
    list.insertAtTail(1);
    list.insertAtTail(2);
    list.insertAtTail(3);
    list.deleteAt(0);
    EXPECT_EQ(list.get(0), 2);
    EXPECT_EQ(list.size(), 2);
}

void test_deleteAt_tail()
{
    DoublyLinkedList<int> list;
    list.insertAtTail(10);
    list.insertAtTail(20);
    list.insertAtTail(30);
    list.deleteAt(2);
    EXPECT_EQ(list.size(), 2);
    EXPECT_EQ(list.get(1), 20);
}

void test_deleteAt_middle()
{
    DoublyLinkedList<int> list;
    list.insertAtTail(5);
    list.insertAtTail(6);
    list.insertAtTail(7);
    list.deleteAt(1);
    EXPECT_EQ(list.size(), 2);
    EXPECT_EQ(list.get(1), 7);
}

void test_deleteAt_only_one_element()
{
    DoublyLinkedList<int> list;
    list.insertAtTail(42);
    list.deleteAt(0);
    EXPECT_EQ(list.size(), 0);
}

void test_deleteAt_multiple_deletes()
{
    DoublyLinkedList<int> list;
    for (int i = 0; i < 5; ++i)
        list.insertAtTail(i + 1); // 1 2 3 4 5
    list.deleteAt(0);             // 2 3 4 5
    list.deleteAt(1);             // 2 4 5
    list.deleteAt(2);             // 2 4
    EXPECT_EQ(list.size(), 2);
    EXPECT_EQ(list.get(0), 2);
    EXPECT_EQ(list.get(1), 4);
}

void test_deleteAt_negative_index()
{
    DoublyLinkedList<int> list;
    list.insertAtTail(1);
    try
    {
        list.deleteAt(-1);
        throw std::runtime_error("Expected std::out_of_range not thrown");
    }
    catch (const std::out_of_range &e)
    {
        EXPECT_EQ(std::string(e.what()), "Index is invalid!");
    }
}

void test_deleteAt_out_of_bound_index()
{
    DoublyLinkedList<int> list;
    list.insertAtTail(1);
    try
    {
        list.deleteAt(5);
        throw std::runtime_error("Expected std::out_of_range not thrown");
    }
    catch (const std::out_of_range &e)
    {
        EXPECT_EQ(std::string(e.what()), "Index is invalid!");
    }
}
void test_delete_insert_delete()
{
    DoublyLinkedList<int> list;
    list.insertAtHead(2); // 2
    list.insertAtTail(4); // 2 4
    list.insertAt(1, 3);  // 2 3 4
    list.deleteAt(1);     // 2 4
    EXPECT_EQ(list.get(1), 4);
    EXPECT_EQ(list.size(), 2);
}

void test_delete_full_sequence()
{
    DoublyLinkedList<int> list;
    for (int i = 0; i < 5; ++i)
        list.insertAtTail(i); // 0 1 2 3 4
    for (int i = 0; i < 5; ++i)
        list.deleteAt(0); // empty
    EXPECT_EQ(list.size(), 0);
}
