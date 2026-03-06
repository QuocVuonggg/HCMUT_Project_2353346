#ifndef __TEXT_BUFFER_H__
#define __TEXT_BUFFER_H__

#include "main.h"

template <typename T>
class DoublyLinkedList
{
    // TODO: may provide some attributes
private:
    struct Node
    {
        T data;
        Node *prev;
        Node *next;
        Node() : prev(nullptr), next(nullptr) {}
        Node(const T &val, Node *prev = nullptr, Node *next = nullptr) : data(val), prev(prev), next(next) {}
    };

    Node *head; // Dummy head
    Node *tail; // Dummy tail
    int length;

public:
    DoublyLinkedList();
    ~DoublyLinkedList();

    void insertAtHead(T data);
    void insertAtTail(T data);
    void insertAt(int index, T data);
    void deleteAt(int index);
    T &get(int index) const;
    int indexOf(T item) const;
    bool contains(T item) const;
    int size() const;
    void reverse();
    string toString(string (*convert2str)(T &) = 0) const;

    class Iterator
    {
    private:
        Node *current;
        DoublyLinkedList<T> *list;

    public:
        Iterator(Node *node, DoublyLinkedList<T> *list) : current(node), list(list) {}

        T &operator*() const
        {
            return current->data;
        }

        // TODO operator ++, --, !=, ==

        void removeCurrent()
        {
            Node *prev = current->prev;

            // TODO
            current = prev;
            list->length--;
        }
    };

    Iterator begin()
    {
        return Iterator(head->next, this);
    }

    Iterator end()
    {
        return Iterator(tail, this);
    }
};

class TextBuffer
{
public:
    class HistoryManager
    {
        struct Action
        {
            string actionName;
            int cursorPos;
            char c;
        };
        DoublyLinkedList<Action> actions;

    public:
        HistoryManager();
        ~HistoryManager();

        void addAction(const string &actionName, int cursorPos, char c);
        void printHistory() const;
        int size() const;
    };
};

#endif // __TEXT_BUFFER_H__
