#include "RopeTextBuffer.h"

// TODO DoublyLinkedList, Rope, RopeTextBuffer

// ----------------- DoublyLinkedList -----------------
template <typename T>
DoublyLinkedList<T>::DoublyLinkedList() {
    // TODO
    head=new Node();
    tail=new Node();
    head->next=tail;
    tail->prev=head;
    lsize=0;
}

template <typename T>
DoublyLinkedList<T>::~DoublyLinkedList() {
    // TODO
    Node*cur=head;
    while(cur){
        Node*nextNode=cur->next;
        delete cur;
        cur = nextNode;
    }
}

// TODO: implement other methods of DoublyLinkedList

template <typename T>
void DoublyLinkedList<T>::insertAtHead(T data) {
    Node* newNode=new Node(data,head,head->next);
    head->next->prev = newNode;
    head->next = newNode;
    lsize++;
}

template <typename T>
void DoublyLinkedList<T>::insertAtTail(T data) {
    Node* newNode = new Node(data, tail->prev, tail);
    tail->prev->next = newNode;
    tail->prev = newNode;
    lsize++;
}

template <typename T>
void DoublyLinkedList<T>::insertAt(int index, T data) {
    if (index < 0 || index > size()) {
        throw out_of_range("Index is invalid!");
    }
    Node* cur = head;
    for (int i = 0; i < index; i++) {
        cur = cur->next;
    }
    // cur là node trước vị trí chèn
    Node* newNode = new Node(data, cur, cur->next);
    cur->next->prev = newNode;
    cur->next = newNode;
    lsize++;
}

template <typename T>
void DoublyLinkedList<T>::deleteAt(int index) {
    if (index < 0 || index >= size()) {
        throw out_of_range("Index is invalid!");
    }
    Node* cur = head->next;
    for (int i = 0; i < index; i++) {
        cur = cur->next;
    }
    cur->prev->next = cur->next;
    cur->next->prev = cur->prev;
    delete cur;
    lsize--;
}

template <typename T>
T& DoublyLinkedList<T>::get(int index) const {
    if (index < 0 || index >= size()) {
        throw out_of_range("Index is invalid!");
    }
    Node* cur = head->next;
    for (int i = 0; i < index; i++) {
        cur = cur->next;
    }
    return cur->data;
}

template <typename T>
int DoublyLinkedList<T>::indexOf(T item) const {
    Node* cur = head->next;
    int index = 0;
    while (cur != tail) {
        if (cur->data == item) {
            return index;
        }
        cur = cur->next;
        index++;
    }
    return -1;
}

template <typename T>
bool DoublyLinkedList<T>::contains(T item) const {
    Node* cur = head->next;
    while (cur != tail) {
        if (cur->data == item) {
            return true;
        }
        cur = cur->next;
    }
    return false;
}

template <typename T>
int DoublyLinkedList<T>::size() const {
    return lsize;
}

template <typename T>
void DoublyLinkedList<T>::reverse() {
    Node* cur = head;
    while (cur) {
        swap(cur->next, cur->prev);
        cur = cur->prev;
    }
    Node* tmp = head;
    head = tail;
    tail = tmp;
}

template <typename T>
string DoublyLinkedList<T>::toString(string (*convert2str)(T&)) const {
    stringstream ss;
    ss << "[";
    Node* cur = head->next;
    while (cur != tail) {
        if (convert2str != nullptr) {
            ss << convert2str(cur->data);
        } else {
            ss << cur->data;
        }
        if (cur->next != tail) {
            ss << ", ";
        }
        cur = cur->next;
    }
    ss << "]";
    return ss.str();
}

// Node(private)---------------------------------------
Rope::Node::Node(){
    left=nullptr;
    right=nullptr;
    data="";
    weight=0;
    height=0;
    total=0;
    balance=EH;
}

Rope::Node::Node(const string& s){
    left=nullptr;
    right=nullptr;
    data=s;
    weight=s.length();
    height=1;
    total = s.length();
    balance=EH;
}

bool Rope::Node::isLeaf() const{
    return left == nullptr && right == nullptr;
}

//Rope(private)----------------------------------------
int Rope::height(Node* node) const{
    //O(1) - OK
    return node?node->height:0;
}

int Rope::getTotalLength(Node* node) const{
    //O(logn)
    if(!node) return 0;
    if(node->isLeaf()) return node->data.length();
    return node->weight+getTotalLength(node->right);;
}

void Rope::update(Node* node){
    //O(logn) - phân vân O(1) và O(logn) nhưng đang dùng O(logn)=================================================
    if(!node) return;
    if(node->isLeaf()) {
        node->weight = node->data.length();
        node->total  = node->weight;
    } else {
        node->weight = getTotalLength(node->left);
        node->total = node->weight + getTotalLength(node->right);
    }
    int hl = height(node->left);
    int hr = height(node->right);
    node->height = 1 + (hl > hr ? hl : hr);
    int bf = hl - hr;
    if (bf > 0)      node->balance = Node::LH;
    else if (bf < 0) node->balance = Node::RH;
    else             node->balance = Node::EH;
}

Rope::Node* Rope::rotateLeft(Node* x){
    Node* y = x->right;
    if (!y) return x;
    x->right = y->left;
    y->left = x;
    update(x);
    update(y);
    return y;
}

Rope::Node* Rope::rotateRight(Node* y){
    Node* x = y->left;
    if (!x) return y;
    y->left = x->right;
    x->right = y;
    update(y);
    update(x);
    return x;
}

Rope::Node* Rope::rebalance(Node* node){
    int bf=height(node->left)-height(node->right);
    if(bf>1){
        if(height(node->left->left)<height(node->left->right)){
            node->left=rotateLeft(node->left);
        }
        return rotateRight(node);
    }
    else if(bf<-1){
        if (height(node->right->right)<height(node->right->left)){
            node->right=rotateRight(node->right);
        }
        return rotateLeft(node);
    }
    return node;
}

void Rope::split(Node* node, int index, Node*& outLeft, Node*& outRight){
    //O(logn) - OK
    if (!node) { outLeft = outRight = nullptr; return; }
    if (node->isLeaf()){
        int len = (int)node->data.size();
        if (index <= 0){
            outLeft = nullptr;
            outRight = new Node(node->data);
        } else if (index >= len){
            outLeft = new Node(node->data);
            outRight = nullptr;
        } else {
            outLeft  = new Node(node->data.substr(0, index));
            outRight = new Node(node->data.substr(index));
        }
        delete node;
        node=nullptr;
        return;
    }
    if (index < node->weight){
        Node *L = nullptr, *R = nullptr;
        split(node->left, index, L, R);
        node->left = R;
        update(node);// O(1)
        outLeft  = L;
        outRight = node;
    } else {
        Node *L = nullptr, *R = nullptr;
        split(node->right, index - node->weight, L, R);
        node->right = L;
        update(node);//O(1)
        outLeft  = node;
        outRight = R;
    }
}

Rope::Node* Rope::concatNodes(Node* left, Node* right){
    //O(logn) - OK
    if(!left) return right;
    if(!right) return left;
    Node*proot = new Node("");
    proot->left=left;
    proot->right=right;
    update(proot);
    return rebalance(proot);
}

char Rope::charAt(Node* node, int index) const{
    //O(logn) - OK
    if(!node) throw out_of_range("Index is invalid!");
    if(node->isLeaf()){
        if (index<0||index>=node->data.length())
            throw out_of_range("Index is invalid!");
        return node->data[index];
    }

    if (index<node->weight)
        return charAt(node->left,index);
    else
        return charAt(node->right,index-node->weight);
}

string Rope::toString(Node* node) const{
    //O(n) - OK
    ostringstream oss;
    toStringHelper(node, oss);
    return oss.str();
}

void Rope::destroy(Node*& node){
    //O(n) - OK
    if(!node) return;
    destroy(node->left);
    destroy(node->right);
    delete node;
    node=nullptr;
}

void Rope::substringHelper(Node* node, int s, int len, string& out, int& pos) const {
    if (!node || len <= 0) return;

    if (node->isLeaf()) {
        int L = (int)node->data.size();
        if (s >= L) return;
        int rem = L - s;
        if (rem <= 0) return;
        int take = len;
        if (take > rem) take = rem;

        const string& src = node->data;
        for (int i = 0; i < take; ++i) {
            out[pos + i] = src[s + i];
        }
        pos += take;
        return;
    }

    if (s < node->weight) {
        int availLeft = node->weight - s;
        int leftLen   = len;
        if (leftLen > availLeft) leftLen = availLeft;

        substringHelper(node->left, s, leftLen, out, pos);

        int remain = len - leftLen;
        if (remain > 0) {
            substringHelper(node->right, 0, remain, out, pos);
        }
    } else {
        substringHelper(node->right, s - node->weight, len, out, pos);
    }
}

void Rope::toStringHelper(Node* node, ostringstream& oss) const {
    if (!node) return;
    if (node->isLeaf()) {
        oss << node->data;
    } else {
        toStringHelper(node->left, oss);
        toStringHelper(node->right, oss);
    }
}

//Rope(public)-----------------------------------------
Rope::Rope() {
    // TODO
    root = nullptr;
}

Rope::~Rope() {
    // TODO
    //O(n) - OK
    destroy(root);
}

int Rope::length() const{
    //O(1) - đang là O(logn)==============================================================================
    return getTotalLength(root);
}

bool Rope::empty() const{
    //O(1) - OK
    return root == nullptr;
}

char Rope::charAt(int index) const{
    //O(logn) - OK
    if(index<0||index>=length()) throw out_of_range("Index is invalid!");
    return charAt(root,index);
}

string Rope::substring(int start, int length) const {
    //O(logn) - OK
    int n=this->length();
    if (start < 0 || start >= n) throw out_of_range("Index is invalid!");
    if (length <= 0 || length > n - start) throw out_of_range("Length is invalid!");
    string out;
    out.resize(length);
    int pos = 0;
    substringHelper(root, start, length, out, pos);
    return out;
}


void Rope::insert(int index, const string& s){
    //O(logn) - OK
    if(index<0||index>length()) throw out_of_range("Index is invalid!");
    if(s.empty()) return;
    Node *left,*right;
    split(root,index,left,right);
    const int chunkSize = 8;
    Node* mid = nullptr;
    for (int i=0;i<s.length();i+=chunkSize) {
        string chunk = s.substr(i,chunkSize);
        mid=concatNodes(mid,new Node(chunk));
    }
    root=concatNodes(concatNodes(left,mid),right);
}

void Rope::deleteRange(int start, int length){
    //O(logn) - OK
    int n=this->length();
    if (start < 0 || start > n) throw out_of_range("Index is invalid!");
    if (length < 0 || length > n - start) throw out_of_range("Length is invalid!");
    Node *left,*middle,*right;
    split(root,start,left,middle);
    split(middle,length,middle,right);
    destroy(middle);
    root=concatNodes(left,right);
}

string Rope::toString() const {
    //O(n) - OK
    ostringstream oss;
    toStringHelper(root, oss);
    return oss.str();
}

// ----------------- RopeTextBuffer -----------------
RopeTextBuffer::RopeTextBuffer() {
    // TODO
    cursorPos = 0;
    history = new HistoryManager();
}

RopeTextBuffer::~RopeTextBuffer() {
    // TODO
    //O(n) - OK
    delete history;
}

void RopeTextBuffer::insert(const string& s){
    //O(logn) - OK
    if(s.empty()) return;
    history->addAction({ "insert", cursorPos, cursorPos + (int)s.length(), s });
    rope.insert(cursorPos, s);
    cursorPos += s.length();
}

void RopeTextBuffer::deleteRange(int length){
    //O(logn) - OK
    if (length<0||cursorPos + length > rope.length()) throw out_of_range("Length is invalid!");
    string deleted = rope.substring(cursorPos, length);
    history->addAction({ "delete", cursorPos, cursorPos, deleted });
    rope.deleteRange(cursorPos, length);
}

void RopeTextBuffer::replace(int length, const string& s){
    //O(logn) - OK
    if (length<0||cursorPos + length > rope.length()) throw out_of_range("Length is invalid!");
    string replaced = rope.substring(cursorPos, length);
    RopeTextBuffer::HistoryManager::Action a = { "replace", cursorPos, cursorPos + (int)s.length(), replaced };
    a.data_replace = s;
    history->addAction(a);
    rope.deleteRange(cursorPos, length);
    rope.insert(cursorPos, s);
    cursorPos += s.length();
}

void RopeTextBuffer::moveCursorTo(int index){
    //O(1) - bị ảnh hưởng của length()
    if (index < 0 || index > rope.length())
        throw out_of_range("Index is invalid!");
    history->addAction({ "move", cursorPos, index, "J" });
    cursorPos = index;
}

void RopeTextBuffer::moveCursorLeft(){
    //O(1) - bị ảnh hưởng của length()
    if (cursorPos == 0)
        throw cursor_error();
    history->addAction({"move", cursorPos, cursorPos-1, "L"});
    cursorPos--;
}

void RopeTextBuffer::moveCursorRight(){
    //O(1) - bị ảnh hưởng của length()
    if (cursorPos == rope.length())
        throw cursor_error();
    history->addAction({"move", cursorPos, cursorPos+1, "R"});
    cursorPos++;
}

int RopeTextBuffer::getCursorPos() const{
    //O(1) - OK
    return cursorPos;
}

string RopeTextBuffer::getContent() const{
    //O(n) - OK
    return rope.toString();
}

int RopeTextBuffer::findFirst(char c) const{
    string content = rope.toString();
    for (int i = 0; i < (int)content.length(); ++i) {
        if (content[i] == c) return i;
    }
    return -1;
}

int* RopeTextBuffer::findAll(char c) const{
    string content = rope.toString();
    int count = 0;
    for (char ch : content) if (ch == c) ++count;
    if (count == 0) return nullptr;
    int* result = new int[count + 1];
    int idx = 0;
    for (int i = 0; i < (int)content.length(); ++i) {
        if (content[i] == c) result[idx++] = i;
    }
    result[idx] = -1; // đánh dấu kết thúc
    return result;
}

void RopeTextBuffer::undo(){
    //O(logn) - chưa check==============================================================================
    if (!history->canUndo()) return;

    RopeTextBuffer::HistoryManager::Action act = history->getUndo();

    if (act.actionName == "insert") {
        rope.deleteRange(act.cursorBefore, act.cursorAfter - act.cursorBefore);
        cursorPos = act.cursorBefore;
    } else if (act.actionName == "delete") {
        rope.insert(act.cursorBefore, act.data);
        cursorPos = act.cursorBefore;
    } else if (act.actionName == "replace") {
        rope.deleteRange(act.cursorBefore, act.cursorAfter - act.cursorBefore);
        rope.insert(act.cursorBefore, act.data);
        cursorPos = act.cursorBefore;
    } else if (act.actionName == "move") {
        cursorPos = act.cursorBefore;
    }
}

void RopeTextBuffer::redo(){
    //O(logn) - chưa check==============================================================================
    if (!history->canRedo()) return;

    RopeTextBuffer::HistoryManager::Action act = history->getRedo();

    if (act.actionName == "insert") {
        rope.insert(act.cursorBefore, act.data);
        cursorPos = act.cursorAfter;
    } 
    else if (act.actionName == "delete") {
        int len = (int)act.data.length();
        if (act.cursorBefore >= 0 && act.cursorBefore + len <= rope.length()) {
            rope.deleteRange(act.cursorBefore, len);
        }
        cursorPos = act.cursorAfter;
    } 
    else if (act.actionName == "replace") {
        int oldLen = (int)act.data.length();
        int total = rope.length();
        int available = total - act.cursorBefore;
        if (available < 0) available = 0;
        if (oldLen > available) oldLen = available;

        if (oldLen > 0) {
            rope.deleteRange(act.cursorBefore, oldLen);
        }
        rope.insert(act.cursorBefore, act.data_replace);

        cursorPos = act.cursorBefore + (int)act.data_replace.length();
    }
    else if (act.actionName == "move") {
        cursorPos = act.cursorAfter;
    }
}

void RopeTextBuffer::printHistory() const {
    //O(n)
    history->printHistory();
}

void RopeTextBuffer::clear() {
    //O(n) - OK
    rope.deleteRange(0, rope.length());
    if (history) { 
        delete history; 
        history = new HistoryManager(); 
    }
    cursorPos = 0;
}

// ----------------- HistoryManager -----------------
RopeTextBuffer::HistoryManager::HistoryManager() {
    // TODO
    currentIndex = 0;
}

RopeTextBuffer::HistoryManager::~HistoryManager() {
    // TODO
}

void RopeTextBuffer::HistoryManager::addAction(const Action& a){
    //O(1) - đang O(logn)==============================================================================
    while (actions.size() > currentIndex) {
        actions.deleteAt(actions.size() - 1);
    }
    actions.insertAtTail(a);
    currentIndex = actions.size();
}

bool RopeTextBuffer::HistoryManager::canUndo() const{
    //O(1) - OK
    return currentIndex > 0;
}

bool RopeTextBuffer::HistoryManager::canRedo() const{
    //O(1) - OK
    return currentIndex < actions.size();
}

void RopeTextBuffer::HistoryManager::printHistory() const{
    //O(n) - OK
    cout << "[";
    int i = 0;
    for (auto it = actions.begin(); it != actions.end() && i < currentIndex; ++it, ++i) {
        const Action& a = *it;
        cout << "(" << a.actionName << ", " << a.cursorBefore
                  << ", " << a.cursorAfter << ", " << a.data << ")";
        if (i != currentIndex - 1) cout << ", ";
    }
    cout << "]";
}

// TODO HistoryManager