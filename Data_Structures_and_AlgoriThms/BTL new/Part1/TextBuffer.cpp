#include "TextBuffer.h"

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


// ----------------- TextBuffer -----------------
TextBuffer::TextBuffer() {
    // TODO
    cursorPos=0;
    buffer = DoublyLinkedList<char>();
}

TextBuffer::~TextBuffer() {
    // TODO
    while(!buffer.empty()){
        buffer.deleteAt(0);
    }
}

void TextBuffer::clearRedoStack(){
    while(!redoStack.empty()){
        Action* action = redoStack.get(0);
        redoStack.deleteAt(0);
        delete action;
    }
}

void TextBuffer::insert(char c) {
    buffer.insertAt(cursorPos, c);
    historyManager.addAction("insert", cursorPos, c);
    clearRedoStack();
    cursorPos++;
}
    
void TextBuffer::deleteChar() {
    if (cursorPos > 0) {
        char deletedChar = buffer.get(cursorPos - 1);
        buffer.deleteAt(cursorPos - 1);
        historyManager.addAction("delete", cursorPos - 1, deletedChar);
        clearRedoStack();
        cursorPos--;
    }
}

void TextBuffer::moveCursorLeft() {
    if(cursorPos <= 0) {
        throw std::out_of_range("cursor_error()");
    }
    historyManager.addAction("move",cursorPos,'L');
    cursorPos--;
}

void TextBuffer::moveCursorRight() {
    if(cursorPos >= buffer.size()) {
        throw std::out_of_range("cursor_error()");
    }
    historyManager.addAction("move",cursorPos,'R');
    cursorPos++;
}

void TextBuffer::moveCursorTo(int index) {
    if(index < 0 || index > buffer.size()) {
        throw std::out_of_range("Index is invalid!");
    }
    historyManager.addAction("move", cursorPos, 'J');
    cursorPos = index;
}

string TextBuffer::getContent() const {
    stringstream ss;
    for(int i = 0; i < buffer.size(); i++) {
        ss << buffer.get(i);
    }
    return ss.str();
}

int TextBuffer::getCursorPos() const {
    return cursorPos;
}

int TextBuffer::findFirstOccurrence(char c) const {
    for(int i = 0; i < buffer.size(); i++) {
        if(buffer.get(i) == c) {
            return i;
        }
    }
    return -1;
}

int* TextBuffer::findAllOccurrences(char c, int &count) const {
    count = 0;
    int* occurrences = new int[buffer.size()];
    for(int i = 0; i < buffer.size(); i++) {
        if(buffer.get(i) == c) {
            occurrences[count++] = i;
        }
    }
    return occurrences;
}

void TextBuffer::mergeSort(char* arr, int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;
        mergeSort(arr, left, mid);
        mergeSort(arr, mid + 1, right);
        merge(arr, left, mid, right);
    }
}

void TextBuffer::merge(char* arr, int left, int mid, int right){
    int n1 = mid - left + 1;
    int n2 = right - mid;
    char* L = new char[n1];
    char* R = new char[n2];
    for (int i = 0; i < n1; i++)
        L[i] = arr[left + i];
    for (int j = 0; j < n2; j++)
        R[j] = arr[mid + 1 + j];
    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2) {
        if (compareChars(L[i], R[j])) {
            arr[k] = L[i];
            i++;
        } else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }
    while (i < n1) {
        arr[k++] = L[i++];
    }
    while (j < n2) {
        arr[k++] = R[j++];
    }
    delete[] L;
    delete[] R;
}

bool TextBuffer::compareChars(char a, char b) {
    bool aUpper = isupper(a);
    bool bUpper = isupper(b);
    if (aUpper && !bUpper) return true;
    if (!aUpper && bUpper) return false;
    return tolower(a) <= tolower(b);
}

void TextBuffer::sortAscending() {
    int size = buffer.size();
    if (size <= 1) {
        cursorPos = 0;
        return;
    }
    char* arr = new char[size];
    for (int i = 0; i < size; i++) {
        arr[i] = buffer.get(i);
    }
    mergeSort(arr, 0, size - 1);
    buffer.clear();
    for (int i = 0; i < size; i++) {
        buffer.insertAtTail(arr[i]);
    }
    cursorPos = 0;
    delete[] arr;
    historyManager.addAction("sort", cursorPos, '\0');
}

void TextBuffer::deleteAllOccurrences(char c) {
    bool found=false;
    int oCursorPos=cursorPos;
    int dint = 0;
    while (dint < buffer.size()) {
        if (buffer.get(dint) == c) {
            found=true;
            buffer.deleteAt(dint);
            historyManager.addAction("delete", dint, c);
        } else {
            dint++;
        }
    }
    cursorPos=found ? 0 : oCursorPos;
    clearRedoStack();
}

void TextBuffer::undo() {
    if(undoStack.empty()) {
        return;
    }
    Action* act=undoStack.get(undoStack.size() - 1);
    undoStack.deleteAt(undoStack.size() - 1);
    if(act->actionName == "insert") {
        buffer.deleteAt(act->cursorPos);
        cursorPos = act->cursorPos;
    } else if(act->actionName == "delete") {
        buffer.insertAt(act->cursorPos, act->character);
        cursorPos = act->cursorPos + 1;
    }
    redoStack.insertAtTail(act);
}

void TextBuffer::redo() {
    if(redoStack.empty()) {
        return;
    }
    Action* act=redoStack.get(redoStack.size()-1);
    redoStack.deleteAt(redoStack.size()-1);
    if(act->actionName=="insert"){
        buffer.insertAt(act->cursorPos, act->character);
        cursorPos = act->cursorPos + 1;
        undoStack.insertAtTail(act);
    } else if(act->actionName=="delete") {
        buffer.deleteAt(act->cursorPos);
        cursorPos = act->cursorPos;
        undoStack.insertAtTail(act);
        clearRedoStack();
    }
}

bool TextBuffer::contains(char c) const {
    return buffer.contains(c);
}

int TextBuffer::size() const {
    return buffer.size();
}

// TODO: implement other methods of TextBuffer

// ----------------- HistoryManager -----------------
TextBuffer::HistoryManager::HistoryManager() {
    // TODO
    history = DoublyLinkedList<Action*>();
}

TextBuffer::HistoryManager::~HistoryManager() {
    // TODO
    while(!history.empty()) {
        Action* act = history.get(0);
        history.deleteAt(0);
        delete act;
    }
}

//TODO: implement other methods of HistoryManager

void TextBuffer::HistoryManager::addAction(const string &actionName, int cursorPos, char c) {
    Action* newAct=new Action(actionName, cursorPos, c);
    history.insertAtTail(newAct);
}

void TextBuffer::HistoryManager::printHistory() const {
    stringstream ss;
    ss << "[";
    for(int i=0;i<history.size();i++){
        Action* act=history.get(i);
        ss<<"("<<act->actionName<<", "<<act->cursorPos<<", "<<act->character<<")";
        if(i<history.size()-1){
            ss<<", ";
        }
    }
    ss<<"]";
    cout << ss.str() << endl;
}

int TextBuffer::HistoryManager::size() const {
    return history.size();
}

// Explicit template instantiation for char, string, int, double, float, and Point
template class DoublyLinkedList<char>;
template class DoublyLinkedList<string>;
template class DoublyLinkedList<int>;
template class DoublyLinkedList<double>;
template class DoublyLinkedList<float>;
template class DoublyLinkedList<Point>;