#include "TextBuffer.h"



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
    while(!undoStack.empty()){
        Action* act = undoStack.get(0);
        undoStack.deleteAt(0);
        delete act;
    }
    while(!redoStack.empty()){
        Action* act = redoStack.get(0);
        redoStack.deleteAt(0);
        delete act;
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
    clearRedoStack();
    buffer.insertAt(cursorPos, c);
    historyManager.addAction("insert", cursorPos, c);
    undoStack.insertAtTail(new Action("insert", cursorPos, c));
    cursorPos++;
}
    
void TextBuffer::deleteChar() {
    if (cursorPos > 0) {
        char deletedChar = buffer.get(cursorPos - 1);
        buffer.deleteAt(cursorPos - 1);
        historyManager.addAction("delete", cursorPos - 1, deletedChar);
        undoStack.insertAtTail(new Action("delete", cursorPos - 1, deletedChar));
        clearRedoStack();
        cursorPos--;
    }
}

void TextBuffer::moveCursorLeft() {
    if(cursorPos <= 0) {
        throw cursor_error();
    }
    historyManager.addAction("move",cursorPos,'L');
    cursorPos--;
}

void TextBuffer::moveCursorRight() {
    if(cursorPos >= buffer.size()) {
        throw cursor_error();
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
    char lowerA=tolower(a);
    char lowerB=tolower(b);
    if(lowerA<lowerB){
        return true;
    }
    if(lowerA>lowerB){
        return false;
    }
    return isupper(a)&&islower(b);
}

void TextBuffer::sortAscending() {
    int size = buffer.size();
    if (size <= 1) {
        cursorPos = 0;
        return;
    }
    string besort=getContent();
    char* arr = new char[size];
    for (int i = 0; i < size; i++) {
        arr[i] = buffer.get(i);
    }
    mergeSort(arr, 0, size - 1);
    buffer.clear();
    for (int i = 0; i < size; i++) {
        buffer.insertAtTail(arr[i]);
    }
    delete[] arr;
    undoStack.insertAtTail(new Action("sort", 0, '\0', besort));
    clearRedoStack();
    historyManager.addAction("sort", cursorPos, '\0');
    cursorPos = 0;
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
    if(act->actionName=="sort") {
        string afsort=getContent();
        buffer.clear();
        for(char c:act->content) {
            buffer.insertAtTail(c);
        }
        cursorPos = buffer.size();
        redoStack.insertAtTail(new Action("sort", 0, '\0', afsort));
        delete act;
        return;
    }
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
    } else if(act->actionName=="delete") {
        buffer.deleteAt(act->cursorPos);
        cursorPos = act->cursorPos;
    } else if(act->actionName=="sort") {
        buffer.clear();
        for(char c:act->content) {
            buffer.insertAtTail(c);
        }
        cursorPos = 0;
    }
    undoStack.insertAtTail(act);
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