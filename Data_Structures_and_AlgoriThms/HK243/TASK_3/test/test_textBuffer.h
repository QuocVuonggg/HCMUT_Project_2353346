#include "../utils.h"

void test_insert()
{
    TextBuffer buf;

    buf.insert('a');
    buf.insert('b');
    EXPECT_EQ(buf.getContent(), "ab");

    buf.moveCursorTo(1);
    buf.insert('x');
    EXPECT_EQ(buf.getContent(), "axb");
}

void test_deleteChar()
{
    TextBuffer buf;
    buf.insert('a');
    buf.insert('b');
    buf.insert('c');
    buf.moveCursorTo(2); // cursor giữa b và c
    buf.deleteChar();
    EXPECT_EQ(buf.getContent(), "ac");

    buf.moveCursorTo(1);
    buf.deleteChar();
    EXPECT_EQ(buf.getContent(), "c");
}

void test_moveCursorLeftRight()
{
    TextBuffer buf;
    buf.insert('x');
    buf.insert('y');
    buf.insert('z');
    buf.moveCursorTo(3); // cursor sau z

    buf.moveCursorLeft(); // -> 2
    EXPECT_EQ(buf.getCursorPos(), 2);
    buf.moveCursorRight(); // -> 3
    EXPECT_EQ(buf.getCursorPos(), 3);
}

void test_moveCursorTo()
{
    TextBuffer buf;
    buf.insert('a');
    buf.insert('b');
    buf.insert('c');
    buf.moveCursorTo(1);
    EXPECT_EQ(buf.getCursorPos(), 1);

    buf.moveCursorTo(3);
    EXPECT_EQ(buf.getCursorPos(), 3);
}

void test_getContent()
{
    TextBuffer buf;
    EXPECT_EQ(buf.getContent(), "");

    buf.insert('x');
    buf.insert('y');
    buf.insert('z');
    EXPECT_EQ(buf.getContent(), "xyz");
}

void test_getCursorPos()
{
    TextBuffer buf;
    EXPECT_EQ(buf.getCursorPos(), 0);

    buf.insert('a');
    EXPECT_EQ(buf.getCursorPos(), 1);
}

void test_findFirstOccurrence()
{
    TextBuffer buf;
    buf.insert('a');
    buf.insert('b');
    buf.insert('a');
    buf.insert('c');

    EXPECT_EQ(buf.findFirstOccurrence('a'), 0);
    EXPECT_EQ(buf.findFirstOccurrence('z'), -1);
}

void test_findAllOccurrences()
{
    TextBuffer buf;
    buf.insert('x');
    buf.insert('a');
    buf.insert('b');
    buf.insert('a');
    buf.insert('x');

    int count = 0;
    int *result = buf.findAllOccurrences('a', count);
    EXPECT_EQ(count, 2);
    EXPECT_EQ(result[0], 1)
    EXPECT_EQ(result[1], 3);
    delete[] result;

    int count2 = 0;
    int *result2 = buf.findAllOccurrences('z', count2);
    EXPECT_EQ(count2, 0);
    delete[] result2;
}

void test_deleteChar_index_0()
{
    TextBuffer buf;
    buf.insert('a');
    buf.insert('b');
    buf.insert('c');
    buf.moveCursorTo(0);

    try
    {
        buf.deleteChar();
        throw std::runtime_error("Expected out_of_range not thrown");
    }
    catch (const std::out_of_range &e)
    {
        EXPECT_EQ(std::string(e.what()), "Index is invalid!");
    }
}

void test_cursor_error()
{
    TextBuffer buf;
    buf.insert('a');
    buf.insert('b');
    buf.insert('c');

    try
    {
        buf.moveCursorRight();
        throw std::runtime_error("Expected cursor_error not thrown");
    }
    catch (const cursor_error &e)
    {
        EXPECT_EQ(std::string(e.what()), "Cursor error!");
    }
}

void test_delete_all_occurrences_basic()
{
    TextBuffer buf;
    buf.insert('a');
    buf.insert('b');
    buf.insert('a');
    buf.insert('c');
    buf.insert('a');

    buf.deleteAllOccurrences('a');

    EXPECT_EQ(buf.getContent(), "bc");
}

void test_delete_all_occurrences_not_found()
{
    TextBuffer buf;
    buf.insert('x');
    buf.insert('y');
    buf.insert('z');

    buf.deleteAllOccurrences('a');

    EXPECT_EQ(buf.getContent(), "xyz");
}
