#include <vector>
#include "../utils.h"

void test_iterator_forward()
{
    DoublyLinkedList<int> list;
    list.insertAtTail(1);
    list.insertAtTail(2);
    list.insertAtTail(3);

    std::vector<int> result;
    for (auto it = list.begin(); *it != *list.end(); ++it)
        result.push_back(*it);

    EXPECT_EQ(result.size(), 3);
    EXPECT_EQ(result[0], 1);
    EXPECT_EQ(result[1], 2);
    EXPECT_EQ(result[2], 3);
}

void test_iterator_backward()
{
    DoublyLinkedList<int> list;
    list.insertAtTail(1);
    list.insertAtTail(2);
    list.insertAtTail(3);

    auto it = list.end();
    --it;
    EXPECT_EQ(*it, 3);
    --it;
    EXPECT_EQ(*it, 2);
    --it;
    EXPECT_EQ(*it, 1);
}

void test_iterator_dereference()
{
    DoublyLinkedList<char> list;
    list.insertAtTail('A');
    auto it = list.begin();
    EXPECT_EQ(*it, 'A');
}

void test_iterator_increment()
{
    DoublyLinkedList<char> list;
    list.insertAtTail('X');
    list.insertAtTail('Y');
    auto it = list.begin();
    EXPECT_EQ(*it, 'X');
    ++it;
    EXPECT_EQ(*it, 'Y');
}

void test_iterator_removeCurrent()
{
    DoublyLinkedList<char> list;
    list.insertAtTail('A');
    list.insertAtTail('B');
    list.insertAtTail('C');

    auto it = list.begin(); // 'A'
    ++it;                   // 'B'
    it.removeCurrent();     // Xóa 'B'

    EXPECT_EQ(list.size(), 2);
    auto check = list.begin();
    EXPECT_EQ(*check, 'A');
    ++check;
    EXPECT_EQ(*check, 'C');
}

void test_iterator_remove_all_ones()
{
    DoublyLinkedList<int> list;
    list.insertAtTail(1);
    list.insertAtTail(2);
    list.insertAtTail(1);
    list.insertAtTail(3);
    list.insertAtTail(1);

    auto it = list.begin();
    while (it != list.end())
    {
        if (*it == 1)
        {
            it.removeCurrent();
        }
        else
        {
            ++it;
        }
    }

    EXPECT_EQ(list.size(), 2);
    EXPECT_EQ(list.get(0), 2);
    EXPECT_EQ(list.get(1), 3);
}

void test_getContent_foreach()
{
    DoublyLinkedList<char> list;
    std::string input = "abcabca";
    for (char c : input)
        list.insertAtTail(c);

    std::stringstream ss;
    for (char c : list)
        ss << c;

    EXPECT_EQ(ss.str(), "abcabca");
}

void test_findAllOccurrences_foreach()
{
    DoublyLinkedList<char> list;
    std::string input = "abcabca";
    for (char c : input)
        list.insertAtTail(c);

    std::vector<int> positions;
    int idx = 0;
    std::stringstream ss;
    for (char c : list)
    {
        if (c == 'a')
            ss << idx;
        idx++;
    }
    EXPECT_EQ(ss.str(), "036");
}

void test_deleteAllOccurrences_foreach()
{
    DoublyLinkedList<char> list;
    std::string input = "abcabca";
    for (char c : input)
        list.insertAtTail(c);

    for (auto it = list.begin(); *it != *list.end(); ++it)
    {
        if (*it == 'a')
            it.removeCurrent();
    }

    std::stringstream ss;
    for (char c : list)
        ss << c;

    EXPECT_EQ(ss.str(), "bcbc");
}

void test_deleteAllOccurrences_no_foreach()
{
    DoublyLinkedList<char> list;
    std::string input = "aa";
    for (char c : input)
        list.insertAtTail(c);

    for (auto it = list.begin(); *it != *list.end(); ++it)
    {
        if (*it == 'a')
            it.removeCurrent();
    }

    std::stringstream ss;
    for (char c : list)
        ss << c;

    EXPECT_EQ(ss.str(), "");
}