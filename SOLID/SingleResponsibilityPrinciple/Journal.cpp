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
#include <gtest/gtest.h>

struct Journal {
    std::string title;
    std::vector<std::string> entries;
    int count = 1;

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
    entries.push_back(boost::lexical_cast<std::string>(count++) + ": " + entry);
}

class JournalTest : public ::testing::Test {
protected:
    void TearDown() override {
        // Clean up any test files
        std::remove("test_journal.txt");
    }
};

TEST_F(JournalTest, AddSingleEntry) {
    Journal journal("Test Journal");
    journal.add("First entry");

    EXPECT_EQ(journal.entries.size(), size_t(1));
    EXPECT_EQ(journal.entries[0], "1: First entry");
}

TEST_F(JournalTest, AddMultipleEntries) {
    Journal journal("Test Journal");
    journal.add("First entry");
    journal.add("Second entry");
    journal.add("Third entry");

    EXPECT_EQ(journal.entries.size(), size_t(3));
    EXPECT_EQ(journal.entries[0], "1: First entry");
    EXPECT_EQ(journal.entries[1], "2: Second entry");
    EXPECT_EQ(journal.entries[2], "3: Third entry");
}

TEST_F(JournalTest, JournalTitle) {
    Journal journal("My Diary");
    EXPECT_EQ(journal.title, "My Diary");
}

TEST_F(JournalTest, PersistenceManagerSave) {
    Journal journal("Test Journal");
    journal.add("Test entry 1");
    journal.add("Test entry 2");

    PersistenceManager::save_to_file(journal, "test_journal.txt");

    // Read the file and verify contents
    std::ifstream file("test_journal.txt");
    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string content = buffer.str();

    EXPECT_TRUE(content.find("1: Test entry 1") != std::string::npos);
    EXPECT_TRUE(content.find("2: Test entry 2") != std::string::npos);
}

TEST_F(JournalTest, EmptyJournal) {
    Journal journal("Empty Journal");
    EXPECT_EQ(journal.entries.size(), size_t(0));

    PersistenceManager::save_to_file(journal, "test_journal.txt");

    std::ifstream file("test_journal.txt");
    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string content = buffer.str();

    EXPECT_EQ(content, "");
}

TEST_F(JournalTest, EntryNumbering) {
    Journal journal("Test Journal");
    for (int i = 1; i <= 5; i++) {
        journal.add("Entry");
    }

    for (size_t i = 0; i < journal.entries.size(); i++) {
        std::string expected = std::to_string(i + 1) + ": Entry";
        EXPECT_EQ(journal.entries[i], expected);
    }
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
