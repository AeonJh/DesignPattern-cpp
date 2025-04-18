/**
 * @file Journal.cpp
 * 
 * @brief Example of Single Responsibility Principle
 * 
 * @details This code demonstrates the Single Responsibility Principle (SRP) by separating the concerns of managing journal entries and saving them to a file.
 * The Journal class is responsible for managing journal entries, while the PersistenceManager class is responsible for saving the journal to a file.
 * This separation of concerns makes the code more maintainable and adheres to the SRP.
 *
 * @Single Responsibility Principle: There should never be more than one reason for a class to change.
 *
 * @author AeonJh
 * @date 2025-04-09
 */


#include <iostream>
#include <string>
#include <vector>
#include <string_view>
#include <fstream>
#include "boost/lexical_cast.hpp"

struct Journal {
    std::string title;
    std::vector<std::string> entries;

    explicit Journal(const std::string_view title) : title(title) {}

    void add(const std::string& entry);
    /** WARNING: According to the Single Responsibility Principle, this method should not be part of the Journal class.
     *  It is better to separate the concerns of saving and managing journal entries.
     *  Consider creating a separate class for file operations. 
     */
    // void save(const std::string& filename) const {
    //     std::ofstream ofs(filename);
    //     for (const auto& entry : entries) {
    //         ofs << entry << std::endl;
    //     }
    // }
};

struct PersistenceManager {
    static void save_to_file(const Journal& journal, const std::string& filename) {
        std::ofstream ofs(filename);
        for (const auto& entry : journal.entries) {
            ofs << entry << std::endl;
        }
    }
};


void Journal::add(const std::string& entry) {
    static int count = 1;
    entries.push_back(boost::lexical_cast<std::string>(count++) + ": " + entry);
}

int main() {
    Journal journal("Dear Journal");
    journal.add("I cried today");
    journal.add("I ate a bug");
    journal.add("I am happy");
    // journal.save("journal.txt"); // Before
    PersistenceManager::save_to_file(journal, "journal.txt"); // After

    for (const auto& entry : journal.entries) {
        std::cout << entry << std::endl;
    }

    return 0;
}
