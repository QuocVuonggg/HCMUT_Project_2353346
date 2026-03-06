#include "../utils.h"

void test_redo_insert()
{
    TextBuffer buf;
    buf.insert('a');
    buf.insert('b');
    buf.undo();
    buf.undo();

    EXPECT_EQ(buf.getContent(), "");
    EXPECT_EQ(buf.getCursorPos(), 0);

    buf.redo();
    EXPECT_EQ(buf.getContent(), "a");
    EXPECT_EQ(buf.getCursorPos(), 1);

    buf.redo();
    EXPECT_EQ(buf.getContent(), "ab");
    EXPECT_EQ(buf.getCursorPos(), 2);
}

void test_redo_deleteChar()
{
    TextBuffer buf;
    buf.insert('x');
    buf.insert('y');
    buf.insert('z');     // xyz
    buf.moveCursorTo(2); // cursor giữa x và y
    buf.deleteChar();    // xz

    EXPECT_EQ(buf.getContent(), "xz");
    EXPECT_EQ(buf.getCursorPos(), 1);
    buf.undo(); // xyz

    EXPECT_EQ(buf.getContent(), "xyz");
    EXPECT_EQ(buf.getCursorPos(), 2);

    buf.redo(); // xz
    EXPECT_EQ(buf.getContent(), "xz");
    EXPECT_EQ(buf.getCursorPos(), 1);
}

void test_redo_moveCursorLeft()
{
    TextBuffer buf;
    buf.insert('a');
    buf.insert('b');
    buf.insert('c');      // abc
    buf.moveCursorTo(3);  // sau 'c'
    buf.moveCursorLeft(); // cursor: 2
    buf.undo();           // cursor: 3

    EXPECT_EQ(buf.getCursorPos(), 3);

    buf.redo(); // quay lại 2
    EXPECT_EQ(buf.getCursorPos(), 2);
}

void test_redo_moveCursorRight()
{
    TextBuffer buf;
    buf.insert('1');
    buf.insert('2');
    buf.insert('3');       // 123
    buf.moveCursorTo(1);   // giữa 1 và 2
    buf.moveCursorRight(); // cursor: 2
    buf.undo();            // cursor: 1

    EXPECT_EQ(buf.getCursorPos(), 1);

    buf.redo(); // quay lại 2
    EXPECT_EQ(buf.getCursorPos(), 2);
}

void test_redo_moveCursorTo()
{
    TextBuffer buf;
    buf.insert('x');
    buf.insert('y');
    buf.insert('z'); // xyz

    buf.moveCursorTo(0); // cursor: 3
    buf.moveCursorTo(2); // cursor: 0
    buf.undo();

    EXPECT_EQ(buf.getCursorPos(), 0); // cursor: 0

    buf.redo(); // cursor: 2
    EXPECT_EQ(buf.getCursorPos(), 2);
}

void test_complex_undo_redo_sequence()
{
    TextBuffer buf;

    // Step 1: insert('A')
    buf.insert('A');
    EXPECT_EQ(buf.getContent(), "A");
    EXPECT_EQ(buf.getCursorPos(), 1);

    // Step 2: insert('B')
    buf.insert('B');
    EXPECT_EQ(buf.getContent(), "AB");
    EXPECT_EQ(buf.getCursorPos(), 2);

    // Step 3: insert('C')
    buf.insert('C');
    EXPECT_EQ(buf.getContent(), "ABC");
    EXPECT_EQ(buf.getCursorPos(), 3);

    // Step 4: moveCursorLeft()
    buf.moveCursorLeft();
    EXPECT_EQ(buf.getContent(), "ABC");
    EXPECT_EQ(buf.getCursorPos(), 2); // AB|C

    // Step 5: insert('X')
    buf.insert('X');
    EXPECT_EQ(buf.getContent(), "ABXC");
    EXPECT_EQ(buf.getCursorPos(), 3); // ABX|C

    // Step 6: moveCursorRight()
    buf.moveCursorRight();
    EXPECT_EQ(buf.getContent(), "ABXC");
    EXPECT_EQ(buf.getCursorPos(), 4); // ABXC|

    // Step 7: deleteChar() (delete 'C')
    buf.deleteChar();
    EXPECT_EQ(buf.getContent(), "ABX");
    EXPECT_EQ(buf.getCursorPos(), 3); // ABX|

    // Step 8: undo() - restore C
    buf.undo();
    EXPECT_EQ(buf.getContent(), "ABXC");
    EXPECT_EQ(buf.getCursorPos(), 4); // ABXC|

    // Step 9: undo() - moveCursorLeft()
    buf.undo();
    EXPECT_EQ(buf.getContent(), "ABXC");
    EXPECT_EQ(buf.getCursorPos(), 3); // ABX|C

    // Step 10: undo() - remove 'X'
    buf.undo();
    EXPECT_EQ(buf.getContent(), "ABC");
    EXPECT_EQ(buf.getCursorPos(), 2); // AB|C

    // Step 11: redo() - insert 'X'
    buf.redo();
    EXPECT_EQ(buf.getContent(), "ABXC");
    EXPECT_EQ(buf.getCursorPos(), 3); // ABX|C

    // Step 12: redo() - moveCursorRight()
    buf.redo();
    EXPECT_EQ(buf.getContent(), "ABXC");
    EXPECT_EQ(buf.getCursorPos(), 4); // ABXC|

    // Step 13: redo() - deleteChar() 'C'
    buf.redo();
    EXPECT_EQ(buf.getContent(), "ABX");
    EXPECT_EQ(buf.getCursorPos(), 3); // ABX|
}

void test_redo_cleared_after_new_insert()
{
    TextBuffer buf;

    buf.insert('A'); // A|
    buf.insert('B'); // AB|
    buf.insert('C'); // ABC|

    buf.undo(); // AB| (undo C)
    buf.undo(); // A|  (undo B)

    buf.insert('X'); // AX| (redo stack phải bị xóa)

    buf.redo(); // redo stack rỗng -> không làm gì

    EXPECT_EQ(buf.getContent(), "AX");
    EXPECT_EQ(buf.getCursorPos(), 2);
}