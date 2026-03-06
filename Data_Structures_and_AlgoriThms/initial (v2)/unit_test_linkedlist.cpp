#include "unit_test_linkedlist.h"

map<string, bool (T_LinkedList::*)()> T_LinkedList::TESTS;

bool T_LinkedList::linkedList1()
{
    string name = "test_insertAtTail";
    DoublyLinkedList<int> list;
    list.insertAtTail(1);
    list.insertAtTail(2);
    list.insertAtTail(3);

    string expect = "[1, 2, 3]\nsize=3\nempty=0";
    string output = list.toString() + "\nsize=" + to_string(list.size()) + "\nempty=" + to_string(list.empty());
    return printResult(output, expect, name);
}

bool T_LinkedList::linkedList2()
{
    string name = "test_insertAt";
    DoublyLinkedList<int> list;
    list.insertAtTail(10);
    list.insertAtTail(30);
    list.insertAt(1, 20);

    string expect = "[10, 20, 30]\nsize=3\nempty=0";
    string output = list.toString() + "\nsize=" + to_string(list.size()) + "\nempty=" + to_string(list.empty());
    return printResult(output, expect, name);
}

bool T_LinkedList::linkedList3()
{
    string name = "test_deleteAt";
    DoublyLinkedList<int> list;
    list.insertAtTail(7);
    list.insertAtTail(8);
    list.insertAtTail(9);
    list.deleteAt(1);

    string expect = "[7, 9]\nsize=2\nempty=0";
    string output = list.toString() + "\nsize=" + to_string(list.size()) + "\nempty=" + to_string(list.empty());
    return printResult(output, expect, name);
}

bool T_LinkedList::linkedList4()
{
    string name = "test_reverse";
    DoublyLinkedList<int> list;
    list.insertAtTail(1);
    list.insertAtTail(2);
    list.insertAtTail(3);
    list.reverse();

    string expect = "[3, 2, 1]\nsize=3\nempty=0";
    string output = list.toString() + "\nsize=" + to_string(list.size()) + "\nempty=" + to_string(list.empty());
    return printResult(output, expect, name);
}

bool T_LinkedList::linkedList5()
{
    string name = "test_indexOf_contains";
    DoublyLinkedList<int> list;
    list.insertAtTail(4);
    list.insertAtTail(5);
    list.insertAtTail(6);

    string expect = "indexOf=1\ncontains=1";
    string output = "indexOf=" + to_string(list.indexOf(5)) + "\ncontains=" + to_string(list.contains(5));
    return printResult(output, expect, name);
}

bool T_LinkedList::linkedList6()
{
    string name = "linkedList6";
    DoublyLinkedList<int> list;

    //! expect
    string expect = "Error: Out of range exception: Index is invalid!";

    //! output
    string output = "";
    try
    {
        list.insertAt(1, 123);
    } 
    catch (const std::out_of_range &e)
    {
        output = "Error: Out of range exception: " + string(e.what());
    }

    return printResult(output, expect, name);
}

bool T_LinkedList::linkedList7()
{
    string name = "linkedList7";
    //! data
    DoublyLinkedList<int> list;
    list.insertAt(0, 1);
    list.insertAt(1, 2);
    list.insertAt(2, 3);

    //! expect
    string expect = "Error: Out of range exception: Index is invalid!";

    //! output
    string output = "";
    try
    {
        list.insertAt(-1, 99);
    }
    catch (const std::out_of_range &e)
    {
        output = "Error: Out of range exception: " + string(e.what());
    }
    //! print result
    return printResult(output, expect, name);
}

bool T_LinkedList::linkedList8()
{
    string name = "linkedList8";
    //! data
    DoublyLinkedList<int> list;

    //! expect
    string expect = "Error: Out of range exception: Index is invalid!";

    //! output
    string output = "";
    try
    {
        list.deleteAt(0);
    }
    catch (const std::out_of_range &e)
    {
        output = "Error: Out of range exception: " + string(e.what());
    }
    //! print result
    return printResult(output, expect, name);
}

bool T_LinkedList::linkedList9()
{
    string name = "linkedList9";
    //! data
    DoublyLinkedList<int> list;
    list.insertAt(0, 1);
    list.insertAt(1, 2);
    list.insertAt(2, 3);

    //! expect
    string expect = "Error: Out of range exception: Index is invalid!";

    //! output
    string output = "";
    try
    {
        list.deleteAt(3);
    }
    catch (const std::out_of_range &e)
    {
        output = "Error: Out of range exception: " + string(e.what());
    }
    //! print result
    return printResult(output, expect, name);
}

bool T_LinkedList::linkedList10()
{
    string name = "linkedList10";
    //! data
    DoublyLinkedList<int> list;
    list.insertAtTail(10);
    list.deleteAt(0);

    //! expect
    string expect = "[]\nsize=0\nempty=1";

    //! output
    string output = list.toString() + "\nsize=" + to_string(list.size()) + "\nempty=" + to_string(list.empty());

    //! print result
    return printResult(output, expect, name);
}

bool T_LinkedList::linkedList11()
{
    string name = "linkedList11";
    //! data
    DoublyLinkedList<int> list;
    list.insertAtTail(1);
    list.insertAtTail(2);
    list.insertAtTail(3);
    list.deleteAt(1);
    list.insertAt(1, 2);
    list.deleteAt(2);

    //! expect
    string expect = "[1, 2];size=2;empty=0";

    //! output
    string output = list.toString() + ";size=" + to_string(list.size()) + ";empty=" + to_string(list.empty());

    //! print result
    return printResult(output, expect, name);
}

bool T_LinkedList::linkedList12()
{
    string name = "linkedList12";
    //! data
    DoublyLinkedList<int> list;
    list.insertAtTail(9);

    //! expect
    string expect = "removeItem=1;[];size=0;empty=1";

    //! output
    string output = "removeItem=" + to_string(list.removeItem(9)) + ";";
    output += list.toString() + ";size=" + to_string(list.size()) + ";empty=" + to_string(list.empty());

    //! print result
    return printResult(output, expect, name);
}

bool T_LinkedList::linkedList13()
{
    string name = "linkedList13";
    //! data
    DoublyLinkedList<int> list;
    list.insertAtTail(1);

    //! expect
    string expect = "removeItem=0;[1];size=1;empty=0";

    //! output
    string output = "removeItem=" + to_string(list.removeItem(99)) + ";";
    output += list.toString() + ";size=" + to_string(list.size()) + ";empty=" + to_string(list.empty());

    //! print result
    return printResult(output, expect, name);
}

bool T_LinkedList::linkedList14()
{
    string name = "linkedList14";
    //! data
    DoublyLinkedList<int> list;
    list.insertAtTail(1);
    list.insertAtTail(2);
    list.insertAtTail(3);
    list.insertAtTail(2);

    //! expect
    string expect = "removeItem=1;[1, 3, 2];size=3;empty=0";

    //! output
    string output = "removeItem=" + to_string(list.removeItem(2)) + ";";
    output += list.toString() + ";size=" + to_string(list.size()) + ";empty=" + to_string(list.empty());

    //! print result
    return printResult(output, expect, name);
}

bool T_LinkedList::linkedList15()
{
    string name = "linkedList15";
    //! data
    DoublyLinkedList<int> list;
    list.insertAtTail(1);
    list.insertAtTail(2);
    list.insertAtTail(3);
    list.insertAtTail(2);

    //! expect
    string expect = "removeItem=1;[1, 3, 2];size=3;empty=0";

    //! output
    string output = "removeItem=" + to_string(list.removeItem(2)) + ";";
    output += list.toString() + ";size=" + to_string(list.size()) + ";empty=" + to_string(list.empty());

    //! print result
    return printResult(output, expect, name);
}

bool T_LinkedList::linkedList16()
{
    string name = "linkedList16";
    //! data
    DoublyLinkedList<int> list;
    list.insertAtTail(0);
    list.insertAtTail(1);
    list.insertAtTail(2);
    list.insertAtTail(1);

    //! expect
    string expect = "removeItem=0;[0, 1, 2, 1];size=4;empty=0";

    //! output
    string output = "removeItem=" + to_string(list.removeItem(-1)) + ";";
    output += list.toString() + ";size=" + to_string(list.size()) + ";empty=" + to_string(list.empty());

    //! print result
    return printResult(output, expect, name);
}

bool T_LinkedList::linkedList17()
{
    string name = "linkedList17";
    //! data
    DoublyLinkedList<int> list;
    list.clear();
    list.insertAtTail(0);
    list.insertAtTail(1);
    list.insertAtTail(2);
    list.insertAtTail(1);
    list.clear();
    list.insertAtTail(2);
    list.insertAtTail(1);
    list.clear();

    //! expect
    string expect = "[];size=0;empty=1";

    //! output
    string output = list.toString() + ";size=" + to_string(list.size()) + ";empty=" + to_string(list.empty());

    //! print result
    return printResult(output, expect, name);
}

bool T_LinkedList::linkedList18() {
    string name = "linkedList18";
    //! data
    DoublyLinkedList<int> list;

    //! expect
    string expect = "Error: Out of range exception: Index is invalid!";

    //! output
    string output = "";
    try
    {
        list.get(0);
    }
    catch (const std::out_of_range &e)
    {
        output = "Error: Out of range exception: " + string(e.what());
    }
    //! print result
    return printResult(output, expect, name);
}

bool T_LinkedList::linkedList19()
{
    string name = "linkedList19";
    //! data
    DoublyLinkedList<int> list;
    list.insertAtTail(0);
    list.insertAtTail(1);
    list.insertAtTail(2);

    //! expect
    string expect = "Error: Out of range exception: Index is invalid!";

    //! output
    string output = "";
    try
    {
        list.get(-1);
    }
    catch (const std::out_of_range &e)
    {
        output = "Error: Out of range exception: " + string(e.what());
    }
    //! print result
    return printResult(output, expect, name);
}

bool T_LinkedList::linkedList20()
{
    string name = "linkedList20";
    //! data
    DoublyLinkedList<int> list;
    list.insertAtTail(0);
    list.insertAtTail(10);
    list.insertAtTail(2);

    //! expect
    string expect = "get=10;[0, 10, 2];size=3;empty=0";

    //! output
    string output = "get=" + to_string(list.get(1)) + ";";
    output += list.toString() + ";size=" + to_string(list.size()) + ";empty=" + to_string(list.empty());

    //! print result
    return printResult(output, expect, name);
}

bool T_LinkedList::linkedList21()
{
    string name = "linkedList21";
    //! data
    DoublyLinkedList<int> list;
    list.clear();
    list.insertAtTail(0);
    list.insertAtTail(1);
    list.insertAtTail(2);
    list.insertAtTail(1);

    //! expect
    string expect = "Index of item 1: 1";

    //! output
    int index = list.indexOf(1);
    string output = "Index of item 1: " + to_string(index);

    //! print result
    return printResult(output, expect, name);
}

bool T_LinkedList::linkedList22()
{
    string name = "linkedList22";
    //! data
    DoublyLinkedList<int> list;
    list.clear();
    list.insertAtTail(0);
    list.insertAtTail(1);
    list.insertAtTail(2);
    list.insertAtTail(1);

    //! expect
    string expect = "Index of item 1: -1";

    //! output
    int index = list.indexOf(-1);
    string output = "Index of item 1: " + to_string(index);

    //! print result
    return printResult(output, expect, name);
}

bool T_LinkedList::linkedList23()
{
    string name = "linkedList23";
    //! data
    DoublyLinkedList<int> list;
    list.clear();
    list.insertAtTail(0);
    list.insertAtTail(1);
    list.insertAtTail(2);

    //! expect
    string expect = "Contains item 1: true";

    //! output
    bool contains = list.contains(1);
    string output = "Contains item 1: " + string(contains ? "true" : "false");

    //! print result
    return printResult(output, expect, name);
}

bool T_LinkedList::linkedList24()
{
    string name = "linkedList24";
    //! data
    DoublyLinkedList<int> list;
    list.clear();
    list.insertAtTail(0);
    list.insertAtTail(1);
    list.insertAtTail(2);

    //! expect
    string expect = "Contains item 1: false";

    //! output
    bool contains = list.contains(3);
    string output = "Contains item 1: " + string(contains ? "true" : "false");

    //! print result
    return printResult(output, expect, name);
}

// ------ BẮT ĐẦU TEST CASE VỚI Point ------

bool T_LinkedList::linkedList25()
{
    string name = "linkedList25";
    //! data
    DoublyLinkedList<Point> list;
    list.insertAtTail(Point(23.2, 25.4));
    list.insertAtTail(Point(24.6, 23.1));

    //! expect
    string expect = "[(23.2,25.4,0), (24.6,23.1,0)]";

    //! output
    string output = list.toString();

    //! print result
    return printResult(output, expect, name);
}

bool T_LinkedList::linkedList26()
{
    string name = "linkedList26";
    //! data
    DoublyLinkedList<Point> list;
    list.insertAtTail(Point(23.2, 25.4, 5));
    list.insertAtTail(Point(24.6, 23.1, -7));

    //! expect
    string expect = "[(23.2,25.4,5), (24.6,23.1,-7)]";

    //! output
    string output = list.toString();

    //! print result
    return printResult(output, expect, name);
}

bool T_LinkedList::linkedList27()
{
    string name = "linkedList27";
    //! data
    DoublyLinkedList<Point> list;
    list.insertAtTail(Point(3.0, 4.0));
    list.insertAtTail(Point(4.0, 3.0));

    //! expect
    string expect = "Index of item 1: -1";

    //! output
    // So sánh Point(3,5,0) (không trùng với ai trong list)
    int index = list.indexOf(Point(3.0, 5.0));
    string output = "Index of item 1: " + to_string(index);

    //! print result
    return printResult(output, expect, name);
}

bool T_LinkedList::linkedList28()
{
    string name = "linkedList28";
    //! data
    DoublyLinkedList<Point> list;
    list.insertAtTail(Point(3.0, 4.0));
    list.insertAtTail(Point(4.0, 3.0));

    //! expect
    string expect = "Index of item 1: 1";

    //! output
    int index = list.indexOf(Point(4.0, 3.0));
    string output = "Index of item 1: " + to_string(index);

    //! print result
    return printResult(output, expect, name);
}

bool T_LinkedList::linkedList29()
{
    string name = "linkedList29";
    //! data
    DoublyLinkedList<Point> list;
    list.insertAtTail(Point(3.0, 4.0));
    list.insertAtTail(Point(4.0, 3.0));

    //! expect
    string expect = "Contains item 1: true";

    //! output
    // Tìm Point(3,4,0) có tồn tại trong list
    bool contains = list.contains(Point(3.0, 4.0));
    string output = "Contains item 1: " + string(contains ? "true" : "false");

    //! print result
    return printResult(output, expect, name);
}