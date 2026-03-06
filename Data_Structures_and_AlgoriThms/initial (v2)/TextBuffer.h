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
        string content;
        Action(string name, int pos, char c, string cont = "") : 
            actionName(name), cursorPos(pos), character(c), content(cont) {}
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


// ----------------- DoublyLinkedList -----------------
template <typename T>
DoublyLinkedList<T>::DoublyLinkedList() {
    // TODO
    head=nullptr;
    tail=nullptr;
    lsize=0;
}

template <typename T>
DoublyLinkedList<T>::~DoublyLinkedList() {
    // TODO
    Node*cur=head;
    while(cur!=nullptr){
        Node*nextNode=cur->next;
        delete cur;
        cur = nextNode;
    }
}

// TODO: implement other methods of DoublyLinkedList

template <typename T>
void DoublyLinkedList<T>::insertAtHead(T data) {
    Node* newNode=new Node(data);
    if(head==nullptr){
        head=newNode;
        tail=newNode;
    } else {
        newNode->next=head;
        head->prev=newNode;
        head=newNode;
    }
    lsize++;
}

template <typename T>
void DoublyLinkedList<T>::insertAtTail(T data) {
    if(tail==nullptr){
        insertAtHead(data);
        return;
    }
    Node* newNode=new Node (data);
    tail->next=newNode;
    newNode->prev=tail;
    tail=newNode;
    lsize++;
}

template <typename T>
void DoublyLinkedList<T>::insertAt(int index, T data) {
    if(index < 0 || index > size()) {
        throw std::out_of_range("Index is invalid!");
    }
    if(index==0){
        insertAtHead(data);
        return;
    } else if(index==size()){
        insertAtTail(data);
        return;
    }
    Node* newNode=new Node(data);
    Node* cur=head;
    for(int i=0; i<index; i++){
        if(cur==nullptr){
            delete newNode;
            throw std::out_of_range("Index is invalid!");
        }
        cur=cur->next;
    }
    newNode->next=cur;
    newNode->prev=cur->prev;
    if(cur->prev != nullptr) {
        cur->prev->next = newNode;
    }
    cur->prev=newNode;
    lsize++;
}

template <typename T>
void DoublyLinkedList<T>::deleteAt(int index) {
    if(index<0||index>=size()){
        throw std::out_of_range("Index is invalid!");
    }
    Node*cur=head;
    for(int i=0; i<index; i++){
        cur=cur->next;
    }
    if(cur==head){
        head=cur->next;
        if(head!=nullptr){
            head->prev=nullptr;
        } else{
            tail=nullptr;
        }
    } else if(cur==tail){
        tail=cur->prev;
        tail->next=nullptr;
    } else{
        cur->prev->next=cur->next;
        cur->next->prev=cur->prev;
    }
    delete cur;
    lsize--;
}

template <typename T>
T& DoublyLinkedList<T>::get(int index) const {
    if(index < 0 || index >= size()) {
        throw std::out_of_range("Index is invalid!");
    }
    Node*cur=head;
    for(int i=0; i<index; i++){
        cur=cur->next;
    }
    return cur->data;
}

template <typename T>
int DoublyLinkedList<T>::indexOf(T item) const {
    Node*cur=head;
    int index=0;
    while(cur!=nullptr){
        if(cur->data==item){
            return index;
        }
        cur=cur->next;
        index++;
    }
    return -1;
}

template <typename T>
bool DoublyLinkedList<T>::contains(T item) const {
    Node*cur=head;
    while(cur!=nullptr){
        if(cur->data==item){
            return true;
        }
        cur=cur->next;
    }
    return false;
}

template <typename T>
int DoublyLinkedList<T>::size() const {
    return lsize;
}

template <typename T>
void DoublyLinkedList<T>::reverse() {
    Node*cur=head;
    Node*tmp=nullptr;
    Node* otail = tail;
    tail=head;
    //đảo con trỏ
    while(cur!=nullptr){
        tmp=cur->prev;
        cur->prev=cur->next;
        cur->next=tmp;
        cur=cur->prev;
    }
    head = otail;
    head->prev=nullptr;
    tail->next=nullptr;
}

template <typename T>
string DoublyLinkedList<T>::toString(string (*convert2str)(T&)) const {
    stringstream ss;
    ss<<"[";
    Node*cur=head;
    while(cur!=nullptr){
        if(convert2str != nullptr) {
            ss << convert2str(cur->data);
        } else {
            ss << cur->data;
        }
        if(cur->next != nullptr) {
            ss << ", ";
        }
        cur=cur->next;
    }
    ss << "]";
    return ss.str();
}

template <typename T>
bool DoublyLinkedList<T>::empty()
{
    return lsize == 0;
}

template <typename T>
bool DoublyLinkedList<T>::removeItem(T item){
    if (head == nullptr) {
        return false;
    }
    bool check = false;
    Node* cur = head;
    while (cur != nullptr) {
        if (cur->data == item) {
            check = true;
            if (cur == head) {
                head = head->next;
                if (head) {
                    head->prev = nullptr;
                } else {
                    tail = nullptr;
                }
            } else if (cur == tail) {
                tail = tail->prev;
                tail->next = nullptr;
            } else {
                cur->prev->next = cur->next;
                cur->next->prev = cur->prev;
            }
            Node* tmp = cur;
            cur = cur->next;
            delete tmp;
            lsize--;
            return check;
        } else {
            cur = cur->next;
        }
    }
    
    return check;
}

template <typename T>
void DoublyLinkedList<T>::clear() {
    while (lsize > 0) {
        deleteAt(0);
    }
}

#endif // __TEXT_BUFFER_H__
