#include "utils.h"

void test_get_valid_index()
{
    DoublyLinkedList<int> list;
    list.insertAtTail(10);
    list.insertAtTail(20);
    list.insertAtTail(30);
    EXPECT_EQ(list.get(1), 20); // get tại index = 1
}

void test_get_first_and_last()
{
    DoublyLinkedList<int> list;
    list.insertAtTail(5);
    list.insertAtTail(15);
    list.insertAtTail(25);
    EXPECT_EQ(list.get(0), 5);
    EXPECT_EQ(list.get(2), 25);
}

void test_get_invalid_index()
{
    DoublyLinkedList<int> list;
    list.insertAtTail(1);
    try
    {
        list.get(3); // vượt giới hạn
        throw std::runtime_error("Expected out_of_range not thrown");
    }
    catch (const std::out_of_range &e)
    {
        EXPECT_EQ(std::string(e.what()), "Index is invalid!");
    }
}
void test_indexOf_found()
{
    DoublyLinkedList<int> list;
    list.insertAtTail(3);
    list.insertAtTail(6);
    list.insertAtTail(9);
    EXPECT_EQ(list.indexOf(6), 1);
}

void test_indexOf_not_found()
{
    DoublyLinkedList<int> list;
    list.insertAtTail(3);
    list.insertAtTail(6);
    EXPECT_EQ(list.indexOf(10), -1);
}

void test_indexOf_first_match()
{
    DoublyLinkedList<int> list;
    list.insertAtTail(5);
    list.insertAtTail(5);
    list.insertAtTail(5);
    EXPECT_EQ(list.indexOf(5), 0);
}
void test_contains_true()
{
    DoublyLinkedList<int> list;
    list.insertAtTail(42);
    EXPECT_EQ(list.contains(42), true);
}

void test_contains_false()
{
    DoublyLinkedList<int> list;
    list.insertAtTail(1);
    list.insertAtTail(2);
    EXPECT_EQ(list.contains(99), false);
}

void test_toString_with_converter()
{
    DoublyLinkedList<int> list;
    list.insertAtTail(1);
    list.insertAtTail(2);
    list.insertAtTail(3);
    EXPECT_EQ(list.toString(intToStr), "[1, 2, 3]");
}

void test_toString_empty_list()
{
    DoublyLinkedList<int> list;
    EXPECT_EQ(list.toString(intToStr), "[]");
}

void test_reverse_multiple_elements()
{
    DoublyLinkedList<int> list;
    list.insertAtTail(1);
    list.insertAtTail(2);
    list.insertAtTail(3);
    list.reverse();
    EXPECT_EQ(list.toString(intToStr), "[3, 2, 1]");
}

void test_reverse_empty_list()
{
    DoublyLinkedList<int> list;
    list.reverse();
    EXPECT_EQ(list.toString(intToStr), "[]");
}

void test_reverse_single_element()
{
    DoublyLinkedList<int> list;
    list.insertAtTail(42);
    list.reverse();
    EXPECT_EQ(list.toString(intToStr), "[42]");
}