#include <iostream>
#include <vector>
#include <functional>
#include <string>
#include <exception>

#include "test/test_insert.h"
#include "test/test_delete.h"
#include "test/test_combined.h"
#include "test/test_history_manager.h"
#include "test/test_iterator.h"

#define RED "\033[31m"
#define GREEN "\033[32m"
#define CYAN "\033[36m"
#define RESET "\033[0m"
#define BOLD "\033[1m"

//! Build: g++ -std=c++17 -o main main.cpp
//! Run: ./main all/nameTest

int main(int argc, char *argv[])
{
    std::vector<std::pair<std::string, std::function<void()>>> tests = {
        {"insertAtHead_basic", test_insertAtHead_basic},
        {"insertAtTail_basic", test_insertAtTail_basic},
        {"insertAt_middle", test_insertAt_middle},
        {"insertAt_beginning", test_insertAt_beginning},
        {"insertAt_end", test_insertAt_end},
        {"combined_insert_1", test_combined_insert_1},
        {"combined_insert_2", test_combined_insert_2},
        {"combined_insert_3", test_combined_insert_3},
        {"combined_insert_4", test_combined_insert_4},
        {"combined_insert_5", test_combined_insert_5},
        {"insertAt_negative_index", test_insertAt_negative_index},
        {"insertAt_too_large_index", test_insertAt_too_large_index},
        {"insertAt_index_0_and_size", test_insertAt_index_0_and_size},

        {"deleteAt_head", test_deleteAt_head},
        {"deleteAt_tail", test_deleteAt_tail},
        {"deleteAt_middle", test_deleteAt_middle},
        {"deleteAt_only_one_element", test_deleteAt_only_one_element},
        {"deleteAt_multiple_deletes", test_deleteAt_multiple_deletes},
        {"deleteAt_negative_index", test_deleteAt_negative_index},
        {"deleteAt_out_of_bound_index", test_deleteAt_out_of_bound_index},
        {"delete_insert_delete", test_delete_insert_delete},
        {"delete_full_sequence", test_delete_full_sequence},

        {"get_valid_index", test_get_valid_index},
        {"get_first_and_last", test_get_first_and_last},
        {"get_invalid_index", test_get_invalid_index},

        {"indexOf_found", test_indexOf_found},
        {"indexOf_not_found", test_indexOf_not_found},
        {"indexOf_first_match", test_indexOf_first_match},

        {"contains_true", test_contains_true},
        {"contains_false", test_contains_false},

        {"toString_with_converter", test_toString_with_converter},
        {"toString_empty_list", test_toString_empty_list},

        {"reverse_multiple_elements", test_reverse_multiple_elements},
        {"reverse_empty_list", test_reverse_empty_list},
        {"reverse_single_element", test_reverse_single_element},

        {"test_add_single_action", test_add_single_action},
        {"test_add_multiple_actions", test_add_multiple_actions},
        {"test_print_history_format", test_print_history_format},
        {"test_add_sort_and_jump_actions", test_add_sort_and_jump_actions},

        {"test_iterator_forward", test_iterator_forward},
        {"test_iterator_backward", test_iterator_backward},
        {"test_iterator_dereference", test_iterator_dereference},
        {"test_iterator_increment", test_iterator_increment},
        {"test_iterator_removeCurrent", test_iterator_removeCurrent},
        {"test_iterator_remove_all_ones", test_iterator_remove_all_ones},
        {"test_getContent_foreach", test_getContent_foreach},
        {"test_findAllOccurrences_foreach", test_findAllOccurrences_foreach},
        {"test_deleteAllOccurrences_foreach", test_deleteAllOccurrences_foreach},
        {"test_deleteAllOccurrences_no_foreach", test_deleteAllOccurrences_no_foreach},
    };

    if (argc < 2)
    {
        std::cout << "Usage:\n  ./main all\n  ./main <name>\nAvailable tests:\n";
        for (const auto &[name, _] : tests)
            std::cout << "  - " << name << "\n";
        return 0;
    }

    std::string arg = argv[1];
    bool run_all = (arg == "all");

    for (const auto &[name, func] : tests)
    {
        if (run_all || arg == name)
        {
            std::cout << CYAN << "=== Running test: " << name << " ===" << RESET << "\n";
            try
            {
                func();
                std::cout << GREEN << "✔ PASSED\n"
                          << RESET;
            }
            catch (const std::exception &e)
            {
                std::cerr << RED << "✘ FAILED: " << e.what() << RESET << "\n";
                return 1; // Dừng ngay khi gặp lỗi
            }
        }

        if (!run_all && arg == name)
            break;
    }

    return 0;
}