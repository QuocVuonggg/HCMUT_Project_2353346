#ifndef __TEXT_BUFFER_H__
#define __TEXT_BUFFER_H__

#include "main.h"

template <typename T>
class DoublyLinkedList {
    // TODO: may provide some attributes
private:
    struct Node{
        T data;
        Node* next;
        Node* prev;

        Node(T d) : data(d), next(nullptr), prev(nullptr) {}
    };
    Node* head;
    Node* tail;
    int lsize;
public:
    DoublyLinkedList();
    ~DoublyLinkedList();

    void insertAtHead(T data);
    void insertAtTail(T data);
    void insertAt(int index, T data);
    void deleteAt(int index);
    T& get(int index) const;
    int indexOf(T item) const;
    bool contains(T item) const;
    int size() const;
    void reverse();
    bool removeItem(T item);
    string toString(string (*convert2str)(T&) = 0) const;
    void clear();
    bool empty();
};

class TextBuffer {
private:
    DoublyLinkedList<char> buffer; 
    int cursorPos;

    // TODO: may provide some attributes
    struct Action {
        string actionName;
        int cursorPos;
        char character;
        Action(string name, int pos, char c) : 
            actionName(name), cursorPos(pos), character(c) {}
    };
    
    DoublyLinkedList<Action*> undoStack;
    DoublyLinkedList<Action*> redoStack;
    void clearRedoStack();

public:
    TextBuffer();
    ~TextBuffer();

    void insert(char c);
    void deleteChar();
    void moveCursorLeft();
    void moveCursorRight();
    void moveCursorTo(int index);
    string getContent() const;
    int getCursorPos() const;
    int findFirstOccurrence(char c) const;
    int* findAllOccurrences(char c, int &count) const;
    void sortAscending();
    void deleteAllOccurrences(char c);
    void undo();
    void redo();
    void mergeSort(char* arr, int left, int right);
    void merge(char* arr, int left, int mid, int right);
    bool compareChars(char a, char b);
    bool contains(char c) const;
    int size() const;

public:
    class HistoryManager {
        // TODO: may provide some attributes
    private:
        struct Action{
            string actionName;
            int cursorPos;
            char character;

            Action(const string &name, int pos, char c) : 
                actionName(name), cursorPos(pos), character(c) {}
        };
        DoublyLinkedList<Action*> history;
    public:
        HistoryManager();
        ~HistoryManager();

        void addAction(const string &actionName, int cursorPos, char c);
        void printHistory() const;
        int size() const;
    };
    HistoryManager historyManager;
};

#endif // __TEXT_BUFFER_H__

