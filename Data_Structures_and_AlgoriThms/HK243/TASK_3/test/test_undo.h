#include "../utils.h"

void test_undo_insert()
{
    TextBuffer buf;
    buf.insert('a');
    buf.insert('b');
    EXPECT_EQ(buf.getContent(), "ab");
    EXPECT_EQ(buf.getCursorPos(), 2);

    buf.undo();
    EXPECT_EQ(buf.getContent(), "a");
    EXPECT_EQ(buf.getCursorPos(), 1);

    buf.undo();
    EXPECT_EQ(buf.getContent(), "");
    EXPECT_EQ(buf.getCursorPos(), 0);
}

void test_undo_deleteChar()
{
    TextBuffer buf;
    buf.insert('x');
    buf.insert('y');
    buf.insert('z');     // xyz
    buf.moveCursorTo(2); // cursor giữa x và y
    EXPECT_EQ(buf.getCursorPos(), 2);

    buf.deleteChar(); // xz
    EXPECT_EQ(buf.getContent(), "xz");
    EXPECT_EQ(buf.getCursorPos(), 1);

    buf.undo(); // undo delete → khôi phục y
    EXPECT_EQ(buf.getContent(), "xyz");
    EXPECT_EQ(buf.getCursorPos(), 2); // cursor sau 'y'
}

void test_undo_moveCursorLeft()
{
    TextBuffer buf;
    buf.insert('a');
    buf.insert('b');
    buf.insert('c');     // abc
    buf.moveCursorTo(3); // sau 'c'
    EXPECT_EQ(buf.getCursorPos(), 3);

    buf.moveCursorLeft(); // cursor: 2
    EXPECT_EQ(buf.getCursorPos(), 2);

    buf.undo(); // quay lại 3
    EXPECT_EQ(buf.getCursorPos(), 3);
}

void test_undo_moveCursorRight()
{
    TextBuffer buf;
    buf.insert('1');
    buf.insert('2');
    buf.insert('3');     // 123
    buf.moveCursorTo(1); // giữa 1 và 2
    EXPECT_EQ(buf.getCursorPos(), 1);

    buf.moveCursorRight(); // cursor: 2
    EXPECT_EQ(buf.getCursorPos(), 2);

    buf.undo(); // quay lại 1
    EXPECT_EQ(buf.getCursorPos(), 1);
}

void test_undo_moveCursorTo()
{
    TextBuffer buf;
    buf.insert('x');
    buf.insert('y');
    buf.insert('z'); // xyz

    buf.moveCursorTo(0);
    EXPECT_EQ(buf.getCursorPos(), 0);

    buf.moveCursorTo(2);
    EXPECT_EQ(buf.getCursorPos(), 2);

    buf.undo(); // quay lại 0
    EXPECT_EQ(buf.getCursorPos(), 0);
}