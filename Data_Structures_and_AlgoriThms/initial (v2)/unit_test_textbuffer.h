#ifndef UNIT_TEST_TEXTBUFFER_H
#define UNIT_TEST_TEXTBUFFER_H

#include "main.h"
#include "TextBuffer.h"
#include "utils.h"

class T_TextBuffer
{
public:
    T_TextBuffer() {
        T_TextBuffer::registerTest("textbuffer01", &T_TextBuffer::textbuffer1);
        T_TextBuffer::registerTest("textbuffer02", &T_TextBuffer::textbuffer2);
        T_TextBuffer::registerTest("textbuffer03", &T_TextBuffer::textbuffer3);
        T_TextBuffer::registerTest("textbuffer04", &T_TextBuffer::textbuffer4);
        T_TextBuffer::registerTest("textbuffer05", &T_TextBuffer::textbuffer5);
        T_TextBuffer::registerTest("textbuffer06", &T_TextBuffer::textbuffer6);
        T_TextBuffer::registerTest("textbuffer07", &T_TextBuffer::textbuffer7);
        T_TextBuffer::registerTest("textbuffer08", &T_TextBuffer::textbuffer8);
        T_TextBuffer::registerTest("textbuffer09", &T_TextBuffer::textbuffer9);
        T_TextBuffer::registerTest("textbuffer10", &T_TextBuffer::textbuffer10);
        T_TextBuffer::registerTest("textbuffer11", &T_TextBuffer::textbuffer11);
        T_TextBuffer::registerTest("textbuffer12", &T_TextBuffer::textbuffer12);
        T_TextBuffer::registerTest("textbuffer13", &T_TextBuffer::textbuffer13);
        T_TextBuffer::registerTest("textbuffer14", &T_TextBuffer::textbuffer14);
        T_TextBuffer::registerTest("textbuffer15", &T_TextBuffer::textbuffer15);
        T_TextBuffer::registerTest("textbuffer16", &T_TextBuffer::textbuffer16);
        T_TextBuffer::registerTest("textbuffer17", &T_TextBuffer::textbuffer17);
        T_TextBuffer::registerTest("textbuffer18", &T_TextBuffer::textbuffer18);
        T_TextBuffer::registerTest("textbuffer19", &T_TextBuffer::textbuffer19);
        T_TextBuffer::registerTest("textbuffer20", &T_TextBuffer::textbuffer20);
        T_TextBuffer::registerTest("textbuffer21", &T_TextBuffer::textbuffer21);
        T_TextBuffer::registerTest("textbuffer22", &T_TextBuffer::textbuffer22);
        T_TextBuffer::registerTest("textbuffer23", &T_TextBuffer::textbuffer23);
        T_TextBuffer::registerTest("textbuffer24", &T_TextBuffer::textbuffer24);
        T_TextBuffer::registerTest("textbuffer25", &T_TextBuffer::textbuffer25);
        T_TextBuffer::registerTest("textbuffer26", &T_TextBuffer::textbuffer26);
        T_TextBuffer::registerTest("textbuffer27", &T_TextBuffer::textbuffer27);
        T_TextBuffer::registerTest("textbuffer28", &T_TextBuffer::textbuffer28);
        T_TextBuffer::registerTest("textbuffer29", &T_TextBuffer::textbuffer29);
        T_TextBuffer::registerTest("textbuffer30", &T_TextBuffer::textbuffer30);
        T_TextBuffer::registerTest("textbuffer31", &T_TextBuffer::textbuffer31);
        T_TextBuffer::registerTest("textbuffer32", &T_TextBuffer::textbuffer32);
        T_TextBuffer::registerTest("textbuffer33", &T_TextBuffer::textbuffer33);
        T_TextBuffer::registerTest("textbuffer34", &T_TextBuffer::textbuffer34);
        T_TextBuffer::registerTest("textbuffer35", &T_TextBuffer::textbuffer35);
        T_TextBuffer::registerTest("textbuffer36", &T_TextBuffer::textbuffer36);
        T_TextBuffer::registerTest("textbuffer37", &T_TextBuffer::textbuffer37);
        T_TextBuffer::registerTest("textbuffer38", &T_TextBuffer::textbuffer38);
        T_TextBuffer::registerTest("textbuffer39", &T_TextBuffer::textbuffer39);
        T_TextBuffer::registerTest("textbuffer40", &T_TextBuffer::textbuffer40);
        T_TextBuffer::registerTest("textbuffer41", &T_TextBuffer::textbuffer41);
        T_TextBuffer::registerTest("textbuffer42", &T_TextBuffer::textbuffer42);
        T_TextBuffer::registerTest("textbuffer43", &T_TextBuffer::textbuffer43);
        T_TextBuffer::registerTest("textbuffer44", &T_TextBuffer::textbuffer44);
        T_TextBuffer::registerTest("textbuffer45", &T_TextBuffer::textbuffer45);
        T_TextBuffer::registerTest("textbuffer46", &T_TextBuffer::textbuffer46);
        T_TextBuffer::registerTest("textbuffer47", &T_TextBuffer::textbuffer47);
        T_TextBuffer::registerTest("textbuffer48", &T_TextBuffer::textbuffer48);
        T_TextBuffer::registerTest("textbuffer49", &T_TextBuffer::textbuffer49);
        T_TextBuffer::registerTest("textbuffer50", &T_TextBuffer::textbuffer50);
    }

private:
    bool textbuffer1();
    bool textbuffer2();
    bool textbuffer3();
    bool textbuffer4();
    bool textbuffer5();
    bool textbuffer6();
    bool textbuffer7();
    bool textbuffer8();
    bool textbuffer9();
    bool textbuffer10();
    bool textbuffer11();
    bool textbuffer12();
    bool textbuffer13();
    bool textbuffer14();
    bool textbuffer15();
    bool textbuffer16();
    bool textbuffer17();
    bool textbuffer18();
    bool textbuffer19();
    bool textbuffer20();
    bool textbuffer21();
    bool textbuffer22();
    bool textbuffer23();
    bool textbuffer24();
    bool textbuffer25();
    bool textbuffer26();
    bool textbuffer27();
    bool textbuffer28();
    bool textbuffer29();
    bool textbuffer30();
    bool textbuffer31();
    bool textbuffer32();
    bool textbuffer33();
    bool textbuffer34();
    bool textbuffer35();
    bool textbuffer36();
    bool textbuffer37();
    bool textbuffer38();
    bool textbuffer39();
    bool textbuffer40();
    bool textbuffer41();
    bool textbuffer42();
    bool textbuffer43();
    bool textbuffer44();
    bool textbuffer45();
    bool textbuffer46();
    bool textbuffer47();
    bool textbuffer48();
    bool textbuffer49();
    bool textbuffer50();

public:
    static map<string, bool (T_TextBuffer::*)()> TESTS;
    const string green = "\033[32m";
    const string red = "\033[31m";
    const string cyan = "\033[36m";
    const string reset = "\033[0m";

    bool printResult(string output, string expect, string name)
    {
        if (expect == output)
        {
            cout << name + " --------------- PASS" << "\n";
            return true;
        }
        else
        {
            cout << name + " --------------- FAIL" << "\n";
            cout << "\texpect : " << expect << endl;
            cout << "\toutput : " << output << endl;
            return false;
        }
    }

    static void registerTest(string name, bool (T_TextBuffer::*function)())
    {
        if (TESTS.find(name) != TESTS.end())
            throw std::runtime_error("Test with name '" + name + "' already exists.");

        TESTS[name] = function;
    }

    void runTest(const std::string &name)
    {
        auto it = TESTS.find(name);

        if (it != TESTS.end())
            (this->*(it->second))();
        else
            throw std::runtime_error("Test with name '" + name + "' does not exist.");
    }

    void runAllTests()
    {
        vector<string> fails;
        for (const auto &test : TESTS)
        {
            if (!(this->*(test.second))())
                fails.push_back(test.first);
        }

        cout << cyan << "\nResult -------------------------" << reset << endl;

        if (fails.empty())
            cout << green << "All tests passed!" << reset << endl;
        else
        {
            int totalTests = TESTS.size();
            int failedTests = fails.size();
            int passedTests = totalTests - failedTests;
            double passRate = (totalTests > 0) ? (static_cast<double>(passedTests) / totalTests) * 100.0 : 0.0;
            cout << red << "Some tests failed:";

            for (const auto &fail : fails)
                cout << "  " << fail;
            cout << cyan << "\nPass rate: " << passRate << "%" << reset << endl;
        }
    }
};

#endif  // UNIT_TEST_TEXTBUFFER_H