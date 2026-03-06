#!/bin/bash

# Xóa file main cũ nếu tồn tại
if [ -f main ]; then
    echo "Removing existing executable 'main'."
    rm main
fi

# Biên dịch chương trình C++
g++ -fsanitize=address -o main -Iinclude -Isrc -std=c++17 \
    main.cpp \
    TextBuffer.cpp \
    unit_test_linkedlist.cpp \
    unit_test_textbuffer.cpp

# Kiểm tra kết quả biên dịch
if [ $? -eq 0 ]; then
    echo -e "\033[36m\033[1mCompilation successful. Run ./main to execute.\033[0m"
    echo -e "\033[32m\033[1mterminal unit test\033[0m"
    echo -e "\033[31m./main unit_test_linkedlist\033[0m"
    echo -e "\033[31m./main unit_test_textbuffer\033[0m"
else
    echo -e "\033[31m\033[1mCompilation failed.\033[0m"
fi