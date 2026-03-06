#include "../utils.h"

void test_add_single_action()
{
    TextBuffer::HistoryManager history;
    history.addAction("insert", 0, 'A');
    EXPECT_EQ(history.size(), 1);
}

void test_add_multiple_actions()
{
    TextBuffer::HistoryManager history;
    history.addAction("insert", 0, 'A');
    history.addAction("insert", 1, 'B');
    history.addAction("insert", 2, 'C');
    history.addAction("move", 3, 'L');
    history.addAction("insert", 2, 'X');
    history.addAction("move", 3, 'R');
    history.addAction("delete", 4, 'C');
    EXPECT_EQ(history.size(), 7);
}

void test_print_history_format()
{
    TextBuffer::HistoryManager history;
    history.addAction("insert", 0, 'A');
    history.addAction("insert", 1, 'B');
    history.addAction("insert", 2, 'C');
    history.addAction("move", 3, 'L');
    history.addAction("insert", 2, 'X');
    history.addAction("move", 3, 'R');
    history.addAction("delete", 4, 'C');

    // Redirect cout to stringstream
    stringstream buffer;
    streambuf *old = cout.rdbuf(buffer.rdbuf());

    history.printHistory();

    // Restore cout
    cout.rdbuf(old);

    string expected = "[(insert, 0, A), (insert, 1, B), (insert, 2, C), (move, 3, L), (insert, 2, X), (move, 3, R), (delete, 4, C)]\n";
    EXPECT_EQ(buffer.str(), expected);
}

void test_add_sort_and_jump_actions()
{
    TextBuffer::HistoryManager history;
    history.addAction("sort", 0, '\0');
    history.addAction("move", 5, 'J');

    EXPECT_EQ(history.size(), 2);

    // Redirect cout to stringstream
    stringstream buffer;
    streambuf *old = cout.rdbuf(buffer.rdbuf());
    history.printHistory();
    cout.rdbuf(old);

    string expected = "[(sort, 0, , (move, 5, J)]\n";
    EXPECT_EQ(buffer.str(), expected);
}