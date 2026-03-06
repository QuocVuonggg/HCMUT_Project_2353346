
#include "app/inventory.h"
// // -------------------- InventoryManager Method Definitions --------------------
InventoryManager::InventoryManager()
{
    // TODO
    attributesMatrix = List2D<InventoryAttribute>();
    productNames = List1D<string>();
    quantities = List1D<int>();
}

InventoryManager::InventoryManager(const List2D<InventoryAttribute> &matrix,
                                   const List1D<string> &names,
                                   const List1D<int> &quantities)
{
    // TODO
    this->attributesMatrix = matrix;
    this->productNames = names;
    this->quantities = quantities;
}

InventoryManager::InventoryManager(const InventoryManager &other)
{
    // TODO
    this->attributesMatrix = other.attributesMatrix;
    this->productNames = other.productNames;
    this->quantities = other.quantities;
}

int InventoryManager::size() const
{
    // TODO
    return productNames.size();
}

void InventoryManager::validateIndex(int index) const
{
    if (index < 0 || index >= size()){
        throw std::out_of_range("Index is invalid!");
    }
}

List1D<InventoryAttribute> InventoryManager::getProductAttributes(int index) const
{
    // TODO
    validateIndex(index);
    return attributesMatrix.getRow(index);
}

string InventoryManager::getProductName(int index) const
{
    // TODO
    validateIndex(index);
    return productNames.get(index);
}

int InventoryManager::getProductQuantity(int index) const
{
    // TODO
    validateIndex(index);
    return quantities.get(index);
}

void InventoryManager::updateQuantity(int index, int newQuantity)
{
    // TODO
    validateIndex(index);
    quantities.set(index, newQuantity);
}

void InventoryManager::addProduct(const List1D<InventoryAttribute> &attributes, const string &name, int quantity)
{
    // TODO
    productNames.add(name);
    quantities.add(quantity);
    attributesMatrix.setRow(attributesMatrix.rows(), attributes);
}

void InventoryManager::removeProduct(int index)
{
    // TODO
    validateIndex(index);
    productNames.removeAt(index);
    quantities.removeAt(index);
    attributesMatrix.removeAt(index);
}


//! tăng dần của cái gì
List1D<string> InventoryManager::query(string attributeName, const double &minValue,
                                       const double &maxValue, int minQuantity, bool ascending) const
{
    // TODO
    List1D<string> result;
    List1D<double> sortVal;
    List1D<int> sortQty;
    List1D<int> OldIndex;  // Thêm mảng lưu chỉ số ban đầu

    // Thu thập dữ liệu và lưu chỉ số ban đầu
    for (int i = 0; i < size(); i++) {
        List1D<InventoryAttribute> attrs = getProductAttributes(i);
        int qty = getProductQuantity(i);
        if (qty >= minQuantity) {
            for (int j = 0; j < attrs.size(); j++) {
                if (attrs.get(j).name == attributeName) {
                    double attributeVal = attrs.get(j).value;
                    if (attributeVal >= minValue && attributeVal <= maxValue) {
                        result.add(getProductName(i));
                        sortVal.add(attributeVal);
                        sortQty.add(qty);
                        OldIndex.add(i);  // Lưu chỉ số ban đầu
                    }
                }
            }
        }
    }

    // Bubble sort với việc duy trì thứ tự ban đầu khi giá trị bằng nhau
    for (int i = 0; i < result.size() - 1; i++) {
        for (int j = 0; j < result.size() - i - 1; j++) {
            bool shouldSwap = false;
            
            if (sortVal.get(j) == sortVal.get(j + 1)) {
                if (sortQty.get(j) == sortQty.get(j + 1)) {
                    // Nếu cả value và quantity bằng nhau, so sánh vị trí ban đầu
                    if (ascending) {
                        shouldSwap = OldIndex.get(j) > OldIndex.get(j + 1);
                    } else {
                        shouldSwap = OldIndex.get(j) < OldIndex.get(j + 1);
                    }
                } else {
                    if (ascending) {
                        shouldSwap = sortQty.get(j) > sortQty.get(j + 1);
                    } else {
                        shouldSwap = sortQty.get(j) < sortQty.get(j + 1);
                    }
                }
            } else {
                if (ascending) {
                    shouldSwap = sortVal.get(j) > sortVal.get(j + 1);
                } else {
                    shouldSwap = sortVal.get(j) < sortVal.get(j + 1);
                }
            }

            if (shouldSwap) {
                // Swap all arrays including OldIndex
                string tempName = result.get(j);
                result.set(j, result.get(j + 1));
                result.set(j + 1, tempName);

                double tempVal = sortVal.get(j);
                sortVal.set(j, sortVal.get(j + 1));
                sortVal.set(j + 1, tempVal);

                int tempQty = sortQty.get(j);
                sortQty.set(j, sortQty.get(j + 1));
                sortQty.set(j + 1, tempQty);

                int tempIdx = OldIndex.get(j);
                OldIndex.set(j, OldIndex.get(j + 1));
                OldIndex.set(j + 1, tempIdx);
            }
        }
    }

    return result;
    
}


void InventoryManager::removeDuplicates()
{
    // TODO
    int n = size();
    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; ) {
            if(getProductName(i) == getProductName(j) &&
               getProductAttributes(i).toString() == getProductAttributes(j).toString())
            {
                int newQty = getProductQuantity(i) + getProductQuantity(j);
                updateQuantity(i, newQty);
                removeProduct(j);
                n--;  // list size decreases
            } else {
                j++;
            }
        }
    }
}
//! Hàm này có gọi removeDuplicates hay không
InventoryManager InventoryManager::merge(const InventoryManager &inv1,
                                         const InventoryManager &inv2)
{
    // TODO
    InventoryManager mergedInventory;
    for (int i = 0; i < inv1.size(); i++) {
        mergedInventory.addProduct(
            inv1.getProductAttributes(i),
            inv1.getProductName(i),
            inv1.getProductQuantity(i)
        );
    }
    
    // Copy all products from inv2 (keeping duplicates)
    for (int j = 0; j < inv2.size(); j++) {
        mergedInventory.addProduct(
            inv2.getProductAttributes(j),
            inv2.getProductName(j),
            inv2.getProductQuantity(j)
        );
    }
    return mergedInventory;
    
}

void InventoryManager::split(InventoryManager &section1,
                             InventoryManager &section2,
                             double ratio) const
{
    // TODO
    if (ratio < 0){
        ratio = 0;
    } else if (ratio > 1){
        ratio = 1;
    }
    int splitIndex = size() * ratio + 0.99;
    section1 = InventoryManager();
    section2 = InventoryManager();
    for (int i = 0; i < splitIndex; ++i){
        section1.addProduct(getProductAttributes(i), getProductName(i), getProductQuantity(i));
    }
    for (int i = splitIndex; i < size(); ++i){
        section2.addProduct(getProductAttributes(i), getProductName(i), getProductQuantity(i));
    }
}

List2D<InventoryAttribute> InventoryManager::getAttributesMatrix() const
{
    // TODO
    return attributesMatrix;
}

List1D<string> InventoryManager::getProductNames() const
{
    // TODO
    return productNames;
}

List1D<int> InventoryManager::getQuantities() const
{
    // TODO
    return quantities;
}

string InventoryManager::toString() const
{
    // TODO
    string result = "InventoryManager[\n";
    result += "  AttributesMatrix: [";
    for (int i = 0; i < attributesMatrix.rows(); i++) {
        result += "[";
        List1D<InventoryAttribute> row = attributesMatrix.getRow(i); // Lấy hàng i
        for (int j = 0; j < row.size(); j++) { // Duyệt qua các phần tử trong hàng
            result += row.get(j).toString(); // Sử dụng toString() của InventoryAttribute
            if (j < row.size() - 1) {
                result += ", ";
            }
        }
        result += "]";
        if (i < attributesMatrix.rows() - 1) {
            result += ", ";
        }
    }
    result += "],\n";

    // Thêm ProductNames
    result += "  ProductNames: [";
    for (int i = 0; i < productNames.size(); i++) {
        result +=  productNames.get(i);
        if (i < productNames.size() - 1) {
            result += ", ";
        }
    }
    result += "],\n";

    // Thêm Quantities
    result += "  Quantities: [";
    for (int i = 0; i < quantities.size(); i++) {
        result += std::to_string(quantities.get(i));
        if (i < quantities.size() - 1) {
            result += ", ";
        }
    }
    result += "]\n";

    // Đóng ngoặc
    result += "]";

    return result;
}