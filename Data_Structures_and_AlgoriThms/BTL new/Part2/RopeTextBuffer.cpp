#include "RopeTextBuffer.h"

// ----------------- DoublyLinkedList -----------------

// Node(private)---------------------------------------
Rope::Node::Node(){
    left=nullptr;
    right=nullptr;
    data="";
    weight=0;
    height=0;
    balance=EH;
}

Rope::Node::Node(const string& s){
    left=nullptr;
    right=nullptr;
    data=s;
    weight=s.length();
    height=1;
    balance=EH;
}

bool Rope::Node::isLeaf() const{
    return left == nullptr && right == nullptr;
}

//Rope(private)----------------------------------------
int Rope::height(Node* node) const{
    return node?node->height:0;
}

int Rope::getTotalLength(Node* node) const{
    if(!node) return 0;
    if(node->isLeaf()) return node->data.length();
    return node->weight+getTotalLength(node->right);;
}

void Rope::update(Node* node){
    if(!node) return;
    if(node->isLeaf()) {
        node->weight = node->data.length();
    } else {
        node->weight = getTotalLength(node->left);
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
    x->right = y->left;
    y->left = x;
    return y;
}

Rope::Node* Rope::rotateRight(Node* y){
    Node* x = y->left;
    y->left = x->right;
    x->right = y;
    return x;
}

Rope::Node* Rope::rebalance(Node* node){
    update(node);
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
    if(!node) {
        outLeft=nullptr;
        outRight=nullptr;
        return;
    }
    if(node->isLeaf()) {
        if(index<=0) {
            outLeft=nullptr;
            outRight=node;
        } else if(index>=node->data.length()) {
            outLeft=node;
            outRight=nullptr;
        } else {
            outLeft=new Node(node->data.substr(0,index));
            outRight=new Node(node->data.substr(index));
            delete node;
        }
        return;
    }
    if (index < node->weight) {
        Node* lleft=nullptr;
        Node* lright=nullptr;
        split(node->left, index, lleft, lright);
        outRight=concatNodes(lright, node->right);
        outLeft=lleft;
    } else {
        Node*rleft=nullptr;
        Node*rright=nullptr;
        split(node->right,index-node->weight,rleft, rright);
        outLeft=concatNodes(node->left,rleft);
        outRight=rright;
    }
    delete node;
}

Rope::Node* Rope::concatNodes(Node* left, Node* right){
    if(!left) return right;
    if(!right) return left;
    Node*newRoot = new Node("");
    newRoot->left=left;
    newRoot->right=right;
    update(newRoot);
    return rebalance(newRoot);
}

char Rope::charAt(Node* node, int index) const{
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
    if(!node) return "";
    if(node->isLeaf()) return node->data;
    return toString(node->left) + toString(node->right);
}

void Rope::destroy(Node*& node){
    if(!node) return;
    destroy(node->left);
    destroy(node->right);
    delete node;
    node=nullptr;
}

string Rope::collectSubstring(Node* node, int s, int len) const {
    if (!node || len <= 0) return "";
    if (node->isLeaf()) {
        int L = (int)node->data.length();
        if (s >= L) return "";
        int take = std::min(len, L - s);
        if (take > 0) {
            return node->data.substr(s, take);
        }
        return "";
    }
    int leftLen = node->weight;
    if (s < leftLen) {
        int take = (len < (L - s) ? len : (L - s));
    int leftTake = (len < (leftLen - s) ? len : (leftLen - s));
        return collectSubstring(node->left, s, leftTake) +
               collectSubstring(node->right, 0, len - leftTake);
    } else {
        return collectSubstring(node->right, s - leftLen, len);
    }
}


//Rope(public)-----------------------------------------
Rope::Rope() {
    // TODO
    root = nullptr;
}

Rope::~Rope() {
    // TODO
    destroy(root);
}

int Rope::length() const{
    return getTotalLength(root);
}

bool Rope::empty() const{
    return root == nullptr;
}

char Rope::charAt(int index) const{
    if(index<0||index>=length()) throw out_of_range("Index is invalid!");
    return charAt(root,index);
}

string Rope::substring(int start, int length) const {
    if (start < 0 || length <= 0 || start + length > this->length())
        throw out_of_range("Invalid substring range");
    return collectSubstring(root, start, length);
}


void Rope::insert(int index, const string& s){
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

void Rope::deleteRange(int start, int len){
    if(start<0||start+len>length()) throw out_of_range("Length is invalid!");
    Node *left,*middle,*right;
    split(root,start,left,middle);
    split(middle,len,middle,right);
    destroy(middle);
    root=concatNodes(left,right);
}

string Rope::toString() const{
    return toString(root);
}
// TODO: implement other methods of DoublyLinkedList

// ----------------- RopeTextBuffer -----------------
RopeTextBuffer::RopeTextBuffer() {
    // TODO
    cursorPos = 0;
    history = new HistoryManager();
}

RopeTextBuffer::~RopeTextBuffer() {
    // TODO
    delete history;
}

void RopeTextBuffer::insert(const string& s){
    if(s.empty()) return;
    rope.insert(cursorPos, s);
    history->addAction({ "insert",cursorPos,cursorPos+(int)s.length(),s});
    cursorPos += s.length();
}

void RopeTextBuffer::deleteRange(int length){
    if (cursorPos + length > rope.length()) throw out_of_range("Length is invalid!");
    string deleted=rope.substring(cursorPos,length);
    rope.deleteRange(cursorPos,length);
    history->addAction({ "delete",cursorPos,cursorPos,deleted });
}

void RopeTextBuffer::replace(int length, const string& s){
    if (cursorPos + length > rope.length()) throw out_of_range("Length is invalid!");
    string replaced=rope.substring(cursorPos,length);
    rope.deleteRange(cursorPos,length);
    rope.insert(cursorPos,s);

    history->addAction({ "replace",cursorPos,cursorPos+(int)s.length(),replaced });
    cursorPos += s.length();
}

void RopeTextBuffer::moveCursorTo(int index){
    if (index < 0 || index > rope.length())
        throw out_of_range("Index is invalid!");
    history->addAction({ "move", cursorPos, index, "J" });
    cursorPos = index;
}

void RopeTextBuffer::moveCursorLeft(){
    if (cursorPos == 0)
        throw cursor_error();
    cursorPos--;
    history->addAction({"move", cursorPos, cursorPos-1, "L"});
}

void RopeTextBuffer::moveCursorRight(){
    if (cursorPos == rope.length())
        throw cursor_error();
    cursorPos++;
    history->addAction({"move", cursorPos-1, cursorPos, "R"});
}

int RopeTextBuffer::getCursorPos() const{
    return cursorPos;
}

string RopeTextBuffer::getContent() const{
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
    int* result = new int[count + 1]; // +1 để lưu số lượng ở đầu (hoặc bạn có thể dùng vector)
    int idx = 0;
    for (int i = 0; i < (int)content.length(); ++i) {
        if (content[i] == c) result[idx++] = i;
    }
    result[idx] = -1; // đánh dấu kết thúc
    return result;
}

void RopeTextBuffer::undo(){
    if (!history->canUndo()) return;
    // Giả sử Action có các trường: type, from, to, data
    Action a = history->undoAction();
    if (a.type == "insert") {
        rope.deleteRange(a.from, a.to - a.from);
        cursorPos = a.from;
    } else if (a.type == "delete") {
        rope.insert(a.from, a.data);
        cursorPos = a.from + a.data.length();
    } else if (a.type == "replace") {
        rope.deleteRange(a.from, a.to - a.from);
        rope.insert(a.from, a.data);
        cursorPos = a.from + a.data.length();
    } else if (a.type == "move") {
        cursorPos = a.from;
    }
}

void RopeTextBuffer::redo(){
    if (!history->canRedo()) return;
    Action a = history->redoAction();
    if (a.type == "insert") {
        rope.insert(a.from, a.data);
        cursorPos = a.from + a.data.length();
    } else if (a.type == "delete") {
        rope.deleteRange(a.from, a.data.length());
        cursorPos = a.from;
    } else if (a.type == "replace") {
        rope.deleteRange(a.from, a.data.length());
        rope.insert(a.from, a.data2); // data2 là dữ liệu mới, bạn cần bổ sung vào Action
        cursorPos = a.from + a.data2.length();
    } else if (a.type == "move") {
        cursorPos = a.to;
    }
}

void RopeTextBuffer::printHistory() const {
    history->printHistory();
}

void RopeTextBuffer::clear() {
    rope.deleteRange(0, rope.length());
    if (history) { 
        delete history; 
        history = new HistoryManager(); 
    }
    cursorPos = 0;
}

// TODO: implement other methods of TextBuffer

// ----------------- HistoryManager -----------------
RopeTextBuffer::HistoryManager::HistoryManager() {
    // TODO
    history=nullptr;
    size=0;
    cap=0;
    current=-1
}

RopeTextBuffer::HistoryManager::~HistoryManager() {
    // TODO
    if(history) delete[] history;
}

void RopeTextBuffer::HistoryManager::addAction(const Action& a){
    if (current + 1 < size) size = current + 1;
    if (size == cap) {
        int newCap = cap == 0 ? 8 : cap * 2;
        Action* newArr = new Action[newCap];
        for (int i = 0; i < size; ++i) newArr[i] = history[i];
        if (history) delete[] history;
        history = newArr;
        cap = newCap;
    }
    history[size++] = a;
    current = size - 1;
}

bool RopeTextBuffer::HistoryManager::canUndo() const{
    return current >= 0;
}

bool RopeTextBuffer::HistoryManager::canRedo() const{
    return current +1 < size;
}

void RopeTextBuffer::HistoryManager::printHistory() const{
    cout << "[";
    for (int i = 0; i < size; ++i) {
        const Action& a = history[i];
        std::cout << "(" << a.actionName << ", " << a.cursorBefore << ", " << a.cursorAfter << ", " << a.data << ")";
        if (i + 1 < size) std::cout << ", ";
    }
    cout << "]" << std::endl;
}

//TODO: implement other methods of HistoryManager
