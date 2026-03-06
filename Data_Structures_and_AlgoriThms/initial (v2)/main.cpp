#include "TextBuffer.h"
#include "unit_test_linkedlist.h"
#include "unit_test_textbuffer.h"

void printTestCase();
void handleTestUnitLinkedList(int argc, char *argv[]);
void handleTestUnitTextBuffer(int argc, char *argv[]);

int main(int argc, char** argv)
{
    if (argc <= 1)
	{
		printTestCase();
		return 1;
    }

    std::string arg1 = argv[1];

    if (arg1 == "unit_test_linkedlist")
        handleTestUnitLinkedList(argc, argv);
    else if (arg1 == "unit_test_textbuffer")
        handleTestUnitTextBuffer(argc, argv);
    else
        printTestCase();

	return 0;
}

void handleTestUnitLinkedList(int argc, char *argv[])
{
    T_LinkedList unitTest;

    if (argc == 2 || (argc == 3 && std::string(argv[2]) == "all"))
    {
        std::cout << "Running all unit tests data: ----------\n";
        unitTest.runAllTests();
    }
    else if (argc == 3)
    {
        std::cout << "Running unit test data: " << argv[2] << " ----------\n";
        unitTest.runTest(argv[2]);
    }
    else
        printTestCase();
}

void handleTestUnitTextBuffer(int argc, char *argv[])
{
    T_TextBuffer unitTest;

    if (argc == 2 || (argc == 3 && std::string(argv[2]) == "all"))
    {
        std::cout << "Running all unit tests (TextBuffer): ----------\n";
        unitTest.runAllTests();
    }
    else if (argc == 3)
    {
        std::cout << "Running unit test (TextBuffer): " << argv[2] << " ----------\n";
        unitTest.runTest(argv[2]);
    }
    else
        printTestCase();
}

void printTestCase()
{
    const std::string RESET = "\033[0m";
    const std::string BOLD = "\033[1m";
    const std::string RED = "\033[31m";
    const std::string GREEN = "\033[32m";

    std::cout << GREEN << BOLD << "terminal unit test linked list" << RESET << std::endl;
    std::cout << RED << "./main unit_test_linkedlist" << RESET << std::endl;

    std::cout << GREEN << BOLD << "terminal unit test text buffer" << RESET << std::endl;
    std::cout << RED << "./main unit_test_textbuffer" << RESET << std::endl;
}