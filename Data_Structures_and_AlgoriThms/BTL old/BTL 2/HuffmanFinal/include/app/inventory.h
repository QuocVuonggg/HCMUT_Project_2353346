#ifndef INVENTORY_MANAGER_H
#define INVENTORY_MANAGER_H

#include "list/XArrayList.h"
#include "list/DLinkedList.h"
#include <sstream>
#include <string>
#include <iostream>
#include <iomanip>
#include <stdexcept>

using namespace std;




// -------------------- List1D --------------------
template <typename T>
class List1D
{
private:
    IList<T> *pList;

public:
    List1D();
    List1D(int num_elements);
    List1D(const T *array, int num_elements);
    List1D(const List1D<T> &other);
    virtual ~List1D();
    List1D<T> &operator=(const List1D<T> &other); 
    int size() const;
    T get(int index) const;
    void set(int index, T value);
    void removeAt(int index);
    void add(const T &value);

    //! thêm hàm này 
    string toString() const;
    template <typename U> //! thêm vào  để chạy test 
    friend ostream &operator<<(ostream &os, const List1D<T> &list);
};

template <typename T>
class List2D
{
private:
    IList<IList<T> *> *pMatrix;

public:
    List2D();
    List2D(List1D<T> *array, int num_rows);
    List2D(const List2D<T> &other);
    virtual ~List2D();
    List2D<T> &operator=(const List2D<T> &other);
    int rows() const;
    //! thêm hàm này 
    
    void setRow(int rowIndex, const List1D<T> &row);
    T get(int rowIndex, int colIndex) const;
    List1D<T> getRow(int rowIndex) const;
    void removeAt(int rowIndex);
    string toString() const;
    template <typename U> //! thêm vào  để chạy test 
    friend ostream &operator<<(ostream &os, const List2D<T> &matrix);
};
struct InventoryAttribute
{
    string name;
    double value;
    //! thêm
    InventoryAttribute() : name(""), value(0.0) {}  // Constructor mặc định
    InventoryAttribute(const string &name, double value) : name(name), value(value) {}
    string toString() const { return name + ": " + to_string(value); }
     //! thêm
     // Định nghĩa toán tử so sánh ==
     bool operator==(const InventoryAttribute& other) const {
        return name == other.name && value == other.value;
    }
      // Toán tử in ra ostream
      friend std::ostream &operator<<(std::ostream &os, const InventoryAttribute &attr) {
        return os << attr.toString();
    }
};

// -------------------- InventoryManager --------------------
class InventoryManager
{
private:
    List2D<InventoryAttribute> attributesMatrix;
    List1D<string> productNames;
    List1D<int> quantities;
    void validateIndex(int index) const;

public:
    InventoryManager();
       
    
    InventoryManager(const List2D<InventoryAttribute> &matrix,
                     const List1D<string> &names,
                     const List1D<int> &quantities);
                     
                     
    InventoryManager(const InventoryManager &other);

    int size() const;
       
    
    List1D<InventoryAttribute> getProductAttributes(int index) const;
    string getProductName(int index) const;
    int getProductQuantity(int index) const;
    void updateQuantity(int index, int newQuantity);
    void addProduct(const List1D<InventoryAttribute> &attributes, const string &name, int quantity);
    void removeProduct(int index);

    List1D<string> query(string attributeName, const double &minValue,
                         const double &maxValue, int minQuantity, bool ascending) const;

    void removeDuplicates();

    static InventoryManager merge(const InventoryManager &inv1,
                                  const InventoryManager &inv2);

    void split(InventoryManager &section1,
               InventoryManager &section2,
               double ratio) const;

    List2D<InventoryAttribute> getAttributesMatrix() const;
    List1D<string> getProductNames() const;
    List1D<int> getQuantities() const;
    string toString() const;
};
// -------------------- List1D Method Definitions --------------------
template <typename T>
List1D<T>::List1D()
{
    // TODO
    pList = new DLinkedList<T>();
}

template <typename T>
List1D<T>::List1D(int num_elements)
{
    // TODO
    pList = new XArrayList<T>();
    for (int i = 0; i < num_elements; ++i)
    {
        pList->add(T());
    }
    
}

template <typename T>
List1D<T>::List1D(const T *array, int num_elements)
{
    // TODO
    pList = new XArrayList<T>();
    for (int i = 0; i < num_elements; ++i)
    {
        pList->add(array[i]);
    }
}

template <typename T>
List1D<T>::List1D(const List1D<T> &other)
{
    // TODO
    pList = new XArrayList<T>();
    for (int i = 0; i < other.size(); ++i){
    pList->add(other.get(i));
    }

}

template <typename T>
List1D<T>::~List1D()
{
    // TODO
    delete pList;

}
template <typename T>
List1D<T> &List1D<T>::operator=(const List1D<T> &other)
{
    if (this != &other) {
        delete pList;  // Xóa danh sách hiện tại
        pList = new DLinkedList<T>();  // Tạo mới danh sách liên kết
        
        // Dùng vòng lặp for thông thường
        for (int i = 0; i < other.size(); i++) {
            T item = other.get(i);  // Lấy phần tử tại chỉ số i
            pList->add(item);  // Thêm phần tử vào danh sách
        }
    }
    return *this;
}

template <typename T>
int List1D<T>::size() const
{
    // TODO
    return pList->size();

}

template <typename T>
T List1D<T>::get(int index) const
{
    // TODO
    if (index < 0 || index >= size()) {
        throw std::out_of_range("Index is out of range!");
    }
    return pList->get(index);
   
}

template <typename T>
void List1D<T>::removeAt(int index)
{
    if (index < 0 || index >= size())
    {
        throw std::out_of_range("Index is out of range!");
    }
    pList->removeAt(index); // Gọi phương thức `removeAt` của danh sách nội bộ
}

template <typename T>
void List1D<T>::set(int index, T value)
{
    // TODO
    if (index == size()){
        pList->add(value);
    }
    else {
        pList->get(index) = value;
    }
  
}

template <typename T>
void List1D<T>::add(const T &value)
{
    // TODO
    pList->add(value);

}

template <typename T>
string List1D<T>::toString() const
{
    // TODO
    stringstream ss;
    ss << "[";
    for (int i = 0; i < size(); ++i)
    {
        ss << get(i);
        if (i < size() - 1)
        {
            ss << ", ";
        }
    }
    ss << "]";
    return ss.str();
   
}

template <typename T>
ostream &operator<<(ostream &os, const List1D<T> &list)
{
    os << list.toString();
    return os;
}

// -------------------- List2D Method Definitions --------------------
template <typename T>
List2D<T>::List2D()
{
    // TODO
    pMatrix = new XArrayList<IList<T>*>();

}
template <typename T>
List2D<T> &List2D<T>::operator=(const List2D<T> &other)
{
    if (this != &other) { // Kiểm tra tự gán
        // Xóa ma trận hiện tại
        for (int i = 0; i < pMatrix->size(); ++i) {
            delete pMatrix->get(i); // Xóa từng hàng
        }
        delete pMatrix; // Xóa ma trận

        // Tạo mới ma trận
        pMatrix = new XArrayList<IList<T>*>();

        // Sao chép từng hàng từ ma trận khác
        for (int i = 0; i < other.rows(); i++) {
            IList<T>* newRow = new XArrayList<T>();
            List1D<T> oRow = other.getRow(i);
            for (int j = 0; j < oRow.size(); j++) {
                newRow->add(oRow.get(j));
            }
            pMatrix->add(newRow);
        }
    }
    return *this; // Trả về đối tượng hiện tại
}

template <typename T>
List2D<T>::List2D(List1D<T> *array, int num_rows)
{
    pMatrix = new DLinkedList<IList<T>*>(); 

    for (int i = 0; i < num_rows; i++) {
        IList<T>* newRow = new XArrayList<T>(); 
        
        for (int j = 0; j < array[i].size(); j++) {
            newRow->add(array[i].get(j)); 
        }

        pMatrix->add(newRow); 
    }
}


template <typename T>
List2D<T>::List2D(const List2D<T> &other)
{
    // TODO
    pMatrix = new XArrayList<IList<T>*>();
    for (int i = 0; i < other.rows(); i++) {
        IList<T>* newRow = new XArrayList<T>();
        List1D<T> oRow = other.getRow(i);
        for (int j = 0; j < oRow.size(); j++) {
            newRow->add(oRow.get(j));
        }
        pMatrix->add(newRow);
    }
    
}

template <typename T>
List2D<T>::~List2D()
{
    // TODO
    // Iterate through each row in pMatrix and delete it
    for (int i = 0; i < pMatrix->size(); ++i){
        if(pMatrix->get(i)){
            delete pMatrix->get(i); // Delete the dynamically allocated List1D<T> objects
        }
    }
    // Delete the pMatrix itself
    delete pMatrix;
    pMatrix = nullptr;
}

template <typename T>
int List2D<T>::rows() const
{
    return pMatrix->size();

}

template <typename T>
void List2D<T>::setRow(int rowIndex, const List1D<T> &row)
{
    // TODO
    if (rowIndex < pMatrix->size()) {
        delete pMatrix->get(rowIndex);
        pMatrix->removeAt(rowIndex);
    }
    IList<T>* newRow = new XArrayList<T>();
    for (int i = 0; i < row.size(); i++) {
        newRow->add(row.get(i));
    }
    pMatrix->add(rowIndex, newRow);
   
}

template <typename T>
T List2D<T>::get(int rowIndex, int colIndex) const
{
    // TODO
    if (rowIndex < 0 || rowIndex >= pMatrix->size())
    {
        throw std::out_of_range("Index is out of range!");
    }

    // Get the row at the specified rowIndex
    IList<T> *row = pMatrix->get(rowIndex);
    // Get the element at the specified colIndex in the row
    return row->get(colIndex);
}

template <typename T>
List1D<T> List2D<T>::getRow(int rowIndex) const
{
    // TODO
    List1D<T> newRow;
    IList<T>* temp = pMatrix->get(rowIndex);
    for(int i = 0; i < temp->size(); i++){
       newRow.add(temp->get(i));
    }
    return newRow;
}

template <typename T>
void List2D<T>::removeAt(int rowIndex)
{
    if (rowIndex < 0 || rowIndex >= rows())
    {
        throw std::out_of_range("Row Index is out of range!");
    }
    delete pMatrix->get(rowIndex); // Xóa hàng tại chỉ số rowIndex
    pMatrix->removeAt(rowIndex);  // Xóa con trỏ đến hàng trong ma trận
}

template <typename T>
string List2D<T>::toString() const
{
    // TODO
    stringstream ss;
    ss << "[";
    for (int i = 0; i < pMatrix->size(); ++i)
    {
        // Get the row at index i
        IList<T> *row = pMatrix->get(i);

        // Append the row's string representation
        ss << row->toString();
        if (i < pMatrix->size() - 1)
        {
            ss << ", ";
        }
    }
    ss << "]";
    return ss.str();
}

template <typename T>
ostream &operator<<(ostream &os, const List2D<T> &matrix)
{
    // TODO
    os << matrix.toString();
    return os;
}


#endif /* INVENTORY_MANAGER_H */
