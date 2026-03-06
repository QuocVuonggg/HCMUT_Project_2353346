#include "../utils.h"

void test_sortAscending_basic()
{
    TextBuffer buf;
    buf.insert('c');
    buf.insert('a');
    buf.insert('b');
    buf.sortAscending();
    EXPECT_EQ(buf.getContent(), "abc");
    EXPECT_EQ(buf.getCursorPos(), 0);
}

void test_sortAscending_with_mixed_case()
{
    TextBuffer buf;
    buf.insert('b');
    buf.insert('A');
    buf.insert('a');
    buf.insert('B');
    buf.sortAscending();
    EXPECT_EQ(buf.getContent(), "AaBb");
}

void test_sortAscending_with_duplicates()
{
    TextBuffer buf;
    buf.insert('z');
    buf.insert('a');
    buf.insert('z');
    buf.insert('a');
    buf.insert('m');
    buf.sortAscending();
    EXPECT_EQ(buf.getContent(), "aamzz");
}

void test_sortAscending_hardcase_letters_only()
{
    TextBuffer buf;
    std::string input = "zAbYyBaaCcZxXxMmNnOoPp";
    for (char ch : input)
    {
        buf.insert(ch);
    }

    buf.sortAscending();

    EXPECT_EQ(buf.getContent(), "AaaBbCcMmNnOoPpXxxYyZz");
}

void test_sort_undo_basic()
{
    TextBuffer buf;
    buf.insert('c');
    buf.insert('a');
    buf.insert('b');

    EXPECT_EQ(buf.getContent(), "cab");
    EXPECT_EQ(buf.getCursorPos(), 3);

    buf.sortAscending();

    EXPECT_EQ(buf.getContent(), "abc");
    EXPECT_EQ(buf.getCursorPos(), 0);

    buf.undo();

    EXPECT_EQ(buf.getContent(), "cab");
    EXPECT_EQ(buf.getCursorPos(), 3);
}

void test_sort_redo_basic()
{
    TextBuffer buf;
    buf.insert('c');
    buf.insert('a');
    buf.insert('b');

    EXPECT_EQ(buf.getContent(), "cab");
    EXPECT_EQ(buf.getCursorPos(), 3);

    buf.sortAscending();

    EXPECT_EQ(buf.getContent(), "abc");
    EXPECT_EQ(buf.getCursorPos(), 0);

    buf.undo();

    EXPECT_EQ(buf.getContent(), "cab");
    EXPECT_EQ(buf.getCursorPos(), 3);

    buf.redo();

    EXPECT_EQ(buf.getContent(), "abc");
    EXPECT_EQ(buf.getCursorPos(), 0);
}

void test_redo_multiple_levels()
{
    TextBuffer buf;
    buf.insert('d');
    buf.insert('c');
    buf.insert('a');
    buf.insert('b'); // "dcab"

    buf.sortAscending(); // -> "abcd"
    EXPECT_EQ(buf.getContent(), "abcd");

    buf.undo(); // Undo sort -> "dcab"
    buf.undo(); // Undo insert 'b' -> "dca"
    buf.redo(); // Redo insert 'b' -> "dcab"
    buf.redo(); // Redo sort -> "abcd"

    EXPECT_EQ(buf.getContent(), "abcd");
}