#include "unit_test_textbuffer.h"

map<string, bool (T_TextBuffer::*)()> T_TextBuffer::TESTS;

bool T_TextBuffer::textbuffer1() {
    string name = "textbuffer1";
    TextBuffer tb;
    string expect = "";
    string output = tb.getContent();
    return printResult(output, expect, name);
}

bool T_TextBuffer::textbuffer2() {
    string name = "textbuffer2";
    TextBuffer tb;
    tb.insert('A');
    string expect = "A";
    string output = tb.getContent();
    return printResult(output, expect, name);
}

bool T_TextBuffer::textbuffer3() {
    string name = "textbuffer3";
    TextBuffer tb;
    tb.insert('A');
    tb.insert('B');
    tb.insert('C');
    string expect = "ABC";
    string output = tb.getContent();
    return printResult(output, expect, name);
}

bool T_TextBuffer::textbuffer4() {
    string name = "textbuffer4";
    TextBuffer tb;
    tb.insert('A');
    tb.deleteChar();
    string expect = "";
    string output = tb.getContent();
    return printResult(output, expect, name);
}

bool T_TextBuffer::textbuffer5() {
    string name = "textbuffer5";
    TextBuffer tb;
    tb.insert('A');
    tb.insert('B');
    tb.moveCursorLeft();
    tb.insert('C');
    string expect = "ACB";
    string output = tb.getContent();
    return printResult(output, expect, name);
}

bool T_TextBuffer::textbuffer6() {
    string name = "textbuffer6";
    TextBuffer tb;
    tb.insert('A');
    tb.insert('B');
    tb.moveCursorLeft();
    tb.deleteChar();
    string expect = "B";
    string output = tb.getContent();
    return printResult(output, expect, name);
}

bool T_TextBuffer::textbuffer7() {
    string name = "textbuffer7";
    TextBuffer tb;
    tb.insert('X');
    tb.insert('Y');
    tb.insert('Z');
    tb.moveCursorTo(0);
    tb.insert('A');
    string expect = "AXYZ";
    string output = tb.getContent();
    return printResult(output, expect, name);
}

bool T_TextBuffer::textbuffer8() {
    string name = "textbuffer8";
    TextBuffer tb;
    tb.insert('a');
    tb.insert('b');
    tb.insert('c');
    string expect = "3";
    string output = to_string(tb.getCursorPos());
    return printResult(output, expect, name);
}

bool T_TextBuffer::textbuffer9() {
    string name = "textbuffer9";
    TextBuffer tb;
    tb.insert('A');
    tb.insert('B');
    tb.insert('C');
    tb.moveCursorLeft();
    tb.moveCursorLeft();
    string expect = "1";
    string output = to_string(tb.getCursorPos());
    return printResult(output, expect, name);
}

bool T_TextBuffer::textbuffer10() {
    string name = "textbuffer10";
    TextBuffer tb;
    tb.insert('D');
    tb.insert('E');
    tb.insert('F');
    string expect = "0";
    string output = to_string(tb.findFirstOccurrence('D'));
    return printResult(output, expect, name);
}

bool T_TextBuffer::textbuffer11() {
    string name = "textbuffer11";
    TextBuffer tb;
    tb.insert('a');
    tb.insert('b');
    tb.insert('c');
    string expect = "-1";
    string output = to_string(tb.findFirstOccurrence('z'));
    return printResult(output, expect, name);
}

bool T_TextBuffer::textbuffer12() {
    string name = "textbuffer12";
    TextBuffer tb;
    tb.insert('a');
    tb.insert('b');
    tb.insert('a');
    int count = 0;
    int* indices = tb.findAllOccurrences('a', count);
    string expect = "0 2 ";
    string output = "";
    for (int i = 0; i < count; ++i) output += to_string(indices[i]) + " ";
    delete[] indices;
    return printResult(output, expect, name);
}

bool T_TextBuffer::textbuffer13() {
    string name = "textbuffer13";
    TextBuffer tb;
    tb.insert('z');
    tb.insert('a');
    tb.insert('A');
    tb.insert('b');
    tb.insert('B');
    tb.sortAscending();
    string expect = "AaBbz";
    string output = tb.getContent();
    return printResult(output, expect, name);
}

bool T_TextBuffer::textbuffer14() {
    string name = "textbuffer14";
    TextBuffer tb;
    tb.insert('a');
    tb.insert('a');
    tb.insert('b');
    tb.insert('a');
    tb.deleteAllOccurrences('a');
    string expect = "b";
    string output = tb.getContent();
    return printResult(output, expect, name);
}

bool T_TextBuffer::textbuffer15() {
    string name = "textbuffer15";
    TextBuffer tb;
    tb.insert('C');
    tb.insert('S');
    tb.insert('A');
    tb.sortAscending();
    string expect = "ACS";
    string output = tb.getContent();
    return printResult(output, expect, name);
}

bool T_TextBuffer::textbuffer16() {
    string name = "textbuffer16";
    TextBuffer tb;
    tb.insert('A');
    tb.insert('B');
    tb.insert('C');
    tb.moveCursorLeft();
    tb.insert('X');
    tb.undo();
    string expect = "ABC";
    string output = tb.getContent();
    return printResult(output, expect, name);
}

bool T_TextBuffer::textbuffer17() {
    string name = "textbuffer17";
    TextBuffer tb;
    tb.insert('M');
    tb.insert('N');
    tb.insert('O');
    tb.moveCursorLeft();
    tb.insert('X');
    tb.undo();
    tb.redo();
    string expect = "MNXO";
    string output = tb.getContent();
    return printResult(output, expect, name);
}

bool T_TextBuffer::textbuffer18() {
    string name = "textbuffer18";
    TextBuffer tb;
    tb.insert('A');
    tb.insert('B');
    tb.insert('C');
    tb.moveCursorLeft();
    tb.insert('X');
    tb.moveCursorRight();
    tb.deleteChar();
    tb.undo();
    string expect = "ABXC";
    string output = tb.getContent();
    return printResult(output, expect, name);
}

bool T_TextBuffer::textbuffer19() {
    string name = "textbuffer19";
    TextBuffer tb;
    tb.insert('A');
    tb.insert('B');
    tb.insert('C');
    tb.moveCursorLeft();
    tb.insert('X');
    tb.undo();
    tb.redo();
    tb.moveCursorRight();
    tb.deleteChar();
    string expect = "ABX";
    string output = tb.getContent();
    return printResult(output, expect, name);
}

bool T_TextBuffer::textbuffer20() {
    string name = "textbuffer20";
    TextBuffer tb;
    tb.insert('H');
    tb.insert('E');
    tb.insert('L');
    tb.insert('L');
    tb.insert('O');
    int* indices = nullptr;
    int count = 0;
    indices = tb.findAllOccurrences('L', count);
    string expect = "2 3 ";
    string output = "";
    for (int i = 0; i < count; ++i) output += to_string(indices[i]) + " ";
    delete[] indices;
    return printResult(output, expect, name);
}

bool T_TextBuffer::textbuffer21() {
    string name = "textbuffer21";
    TextBuffer tb;
    tb.insert('x');
    tb.insert('y');
    tb.moveCursorLeft();
    tb.insert('z'); // insert tại giữa
    string expect = "xzy";
    string output = tb.getContent();
    return printResult(output, expect, name);
}

bool T_TextBuffer::textbuffer22() {
    string name = "textbuffer22";
    TextBuffer tb;
    tb.insert('a');
    tb.deleteChar(); // xóa ký tự cuối (chỉ có 1)
    string expect = "";
    string output = tb.getContent();
    return printResult(output, expect, name);
}

bool T_TextBuffer::textbuffer23() {
    string name = "textbuffer23";
    TextBuffer tb;
    tb.insert('a');
    tb.insert('b');
    tb.moveCursorTo(0); // di chuyển về đầu
    tb.insert('x');
    string expect = "xab";
    string output = tb.getContent();
    return printResult(output, expect, name);
}

bool T_TextBuffer::textbuffer24() {
    string name = "textbuffer24";
    TextBuffer tb;
    tb.insert('a');
    tb.insert('b');
    tb.insert('c');
    tb.moveCursorTo(1);
    tb.deleteChar();
    string expect = "bc";
    string output = tb.getContent();
    return printResult(output, expect, name);
}

bool T_TextBuffer::textbuffer25() {
    string name = "textbuffer25";
    TextBuffer tb;
    tb.insert('a');
    tb.insert('b');
    string expect = "Error: Index is invalid!";
    string output = "";
    try {
        tb.moveCursorTo(5);
    } catch (const std::out_of_range &e) {
        output = string("Error: ") + e.what();
    }
    return printResult(output, expect, name);
}

bool T_TextBuffer::textbuffer26() {
    string name = "textbuffer26";
    TextBuffer tb;
    tb.insert('a');
    tb.insert('b');
    tb.insert('a');
    int count = 0;
    int* arr = tb.findAllOccurrences('a', count);
    string expect = "2";
    string output = to_string(count);
    delete[] arr;
    return printResult(output, expect, name);
}

bool T_TextBuffer::textbuffer27() {
    string name = "textbuffer27";
    TextBuffer tb;
    tb.insert('x');
    tb.insert('y');
    tb.insert('x');
    tb.deleteAllOccurrences('x');
    string expect = "y";
    string output = tb.getContent();
    return printResult(output, expect, name);
}

bool T_TextBuffer::textbuffer28() {
    string name = "textbuffer28";
    TextBuffer tb;
    tb.insert('a');
    tb.insert('b');
    tb.insert('c');
    tb.moveCursorLeft();
    tb.deleteChar();
    string expect = "ac";
    string output = tb.getContent();
    return printResult(output, expect, name);
}

bool T_TextBuffer::textbuffer29() {
    string name = "textbuffer29";
    TextBuffer tb;
    tb.insert('a');
    tb.insert('b');
    tb.undo();
    string expect = "a";
    string output = tb.getContent();
    return printResult(output, expect, name);
}

bool T_TextBuffer::textbuffer30() {
    string name = "textbuffer30";
    TextBuffer tb;
    tb.insert('x');
    tb.insert('y');
    tb.insert('z');
    tb.undo();
    tb.undo();
    tb.redo();
    string expect = "xy";
    string output = tb.getContent();
    return printResult(output, expect, name);
}

bool T_TextBuffer::textbuffer31() {
    string name = "textbuffer31";
    TextBuffer tb;
    tb.insert('b');
    tb.insert('a');
    tb.insert('C');
    tb.sortAscending();
    tb.undo();
    string expect = "baC";
    string output = tb.getContent();
    return printResult(output, expect, name);
}

bool T_TextBuffer::textbuffer32() {
    string name = "textbuffer32";
    TextBuffer tb;
    tb.insert('z');
    tb.insert('a');
    tb.insert('x');
    tb.sortAscending();
    string expect = "axz";
    string output = tb.getContent();
    return printResult(output, expect, name);
}

bool T_TextBuffer::textbuffer33() {
    string name = "textbuffer33";
    TextBuffer tb;
    string expect = "0";
    string output = to_string(tb.size());
    return printResult(output, expect, name);
}

bool T_TextBuffer::textbuffer34() {
    string name = "textbuffer34";
    TextBuffer tb;
    tb.insert('x');
    string expect = "1";
    string output = to_string(tb.size());
    return printResult(output, expect, name);
}

bool T_TextBuffer::textbuffer35() {
    string name = "textbuffer35";
    TextBuffer tb;
    tb.insert('a');
    tb.insert('b');
    bool contains = tb.contains('a');
    string expect = "true";
    string output = contains ? "true" : "false";
    return printResult(output, expect, name);
}

bool T_TextBuffer::textbuffer36() {
    string name = "textbuffer36";
    TextBuffer tb;
    tb.insert('x');
    bool contains = tb.contains('z');
    string expect = "false";
    string output = contains ? "true" : "false";
    return printResult(output, expect, name);
}

bool T_TextBuffer::textbuffer37() {
    string name = "textbuffer37";
    TextBuffer tb;
    tb.insert('m');
    tb.insert('n');
    int index = tb.findFirstOccurrence('n');
    string expect = "1";
    string output = to_string(index);
    return printResult(output, expect, name);
}

bool T_TextBuffer::textbuffer38() {
    string name = "textbuffer38";
    TextBuffer tb;
    tb.insert('m');
    tb.insert('n');
    int index = tb.findFirstOccurrence('z');
    string expect = "-1";
    string output = to_string(index);
    return printResult(output, expect, name);
}

bool T_TextBuffer::textbuffer39() {
    string name = "textbuffer39";
    TextBuffer tb;
    tb.insert('a');
    tb.insert('b');
    tb.insert('c');
    tb.moveCursorLeft();
    tb.moveCursorLeft();
    tb.insert('x');
    string expect = "axbc";
    string output = tb.getContent();
    return printResult(output, expect, name);
}

bool T_TextBuffer::textbuffer40() {
    string name = "textbuffer40";
    TextBuffer tb;
    tb.insert('a');
    tb.insert('b');
    tb.moveCursorTo(0);
    tb.deleteChar();
    string expect = "ab";
    string output = tb.getContent();
    return printResult(output, expect, name);
}

bool T_TextBuffer::textbuffer41() {
    string name = "textbuffer41";
    TextBuffer tb;
    tb.insert('A'); // [A]
    tb.insert('B'); // [AB]
    tb.insert('C'); // [ABC]
    tb.moveCursorTo(1); // cursor giữa A và B
    tb.insert('X'); // [AXBC]
    tb.moveCursorTo(0);
    tb.insert('Z'); // [ZAXBC]
    tb.moveCursorTo(tb.size());
    tb.insert('Q'); // [ZAXBCQ]
    string expect = "ZAXBCQ";
    string output = tb.getContent();
    return printResult(output, expect, name);
}

bool T_TextBuffer::textbuffer42() {
    string name = "textbuffer42";
    TextBuffer tb;
    tb.insert('M');
    tb.deleteChar();
    string output1 = tb.getContent(); // ""
    tb.undo();
    string output2 = tb.getContent(); // "M"
    string expect = ""s + "|" + "M";
    string output = output1 + "|" + output2;
    return printResult(output, expect, name);
}

bool T_TextBuffer::textbuffer43() {
    string name = "textbuffer43";
    TextBuffer tb;
    tb.insert('A'); tb.insert('B'); tb.insert('C'); // [ABC]
    string output;
    try {
        tb.moveCursorTo(0);
        tb.moveCursorLeft(); // expect throw
        output = "fail";
    } catch (const cursor_error&) {
        output = "success";
    }
    return printResult(output, "success", name);
}

bool T_TextBuffer::textbuffer44() {
    string name = "textbuffer44";
    TextBuffer tb;
    tb.insert('A'); tb.insert('B'); tb.insert('C');
    tb.moveCursorLeft(); tb.insert('X'); // [ABXC]
    tb.moveCursorRight(); tb.deleteChar(); // [ABX]
    tb.undo(); // undo delete, [ABXC]
    tb.undo(); // undo insert, [ABC]
    tb.redo(); // redo insert, [ABXC]
    tb.redo(); // redo delete, [ABX]
    string expect = "ABX";
    string output = tb.getContent();
    return printResult(output, expect, name);
}

bool T_TextBuffer::textbuffer45() {
    string name = "textbuffer45";
    TextBuffer tb;
    tb.insert('D'); tb.insert('C'); tb.insert('A'); tb.insert('B'); // [DCAB]
    tb.sortAscending(); // [ABCD]
    tb.undo(); // [DCAB]
    tb.insert('Z'); // [DCABZ]
    try {
        tb.redo(); // Không thực hiện được, nên sẽ throw
    } catch (const std::out_of_range&) {
        // Bỏ qua lỗi, vì mình EXPECT redo không làm gì cả
    }
    string expect = "DCABZ";
    string output = tb.getContent();
    return printResult(output, expect, name);
}

bool T_TextBuffer::textbuffer46() {
    string name = "textbuffer46";
    TextBuffer tb;
    tb.insert('A'); tb.insert('B'); tb.insert('A'); tb.insert('C'); tb.insert('A');
    tb.deleteAllOccurrences('A'); // [BC]
    // KHÔNG undo được vì đề không yêu cầu, buffer sẽ vẫn là "BC"
    string expect = "BC";
    string output = tb.getContent();
    return printResult(output, expect, name);
}

bool T_TextBuffer::textbuffer47() {
    string name = "textbuffer47";
    TextBuffer tb;
    tb.insert('Q'); tb.deleteChar();
    tb.undo(); // Q
    tb.undo(); // buffer rỗng
    string output;
    try {
        tb.undo();
        output = "no_throw";
    } catch (const std::out_of_range&) {
        output = "throw";
    }
    return printResult(output, "throw", name);
}

bool T_TextBuffer::textbuffer48() {
    string name = "textbuffer48";
    TextBuffer tb;
    tb.insert('X'); tb.insert('Y');
    tb.undo(); tb.undo();
    tb.redo(); tb.redo();
    string output;
    try {
        tb.redo();
        output = "no_throw";
    } catch (const std::out_of_range&) {
        output = "throw";
    }
    return printResult(output, "throw", name);
}

bool T_TextBuffer::textbuffer49() {
    string name = "textbuffer49";
    TextBuffer tb;
    tb.insert('1'); tb.insert('2'); tb.insert('3'); tb.insert('4'); // [1234]
    tb.moveCursorTo(2); // cursor giữa 2-3
    tb.insert('A'); // [12A34]
    tb.moveCursorTo(1);
    tb.deleteChar(); // [1A34]
    tb.undo(); // undo delete, [12A34]
    tb.undo(); // undo insert, [1234]
    string expect = "1234";
    string output = tb.getContent();
    return printResult(output, expect, name);
}

bool T_TextBuffer::textbuffer50() {
    string name = "textbuffer50";
    TextBuffer tb;
    for (char c = 'z'; c > 'z'-10; --c) tb.insert(c); // zyxwvutsrq
    tb.sortAscending(); // [qrstuvwxyz]
    tb.undo(); // [zyxwvutsrq]
    for (int i = 0; i < 10; ++i) tb.deleteChar(); // buffer rỗng
    for (int i = 0; i < 10; ++i) tb.undo(); // trở lại [zyxwvutsrq]
    string expect = "zyxwvutsrq";
    string output = tb.getContent();
    return printResult(output, expect, name);
}