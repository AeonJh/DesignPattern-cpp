#include <algorithm>
#include <string>
#include <map>
#include "gtest/gtest.h"


class Database {
protected:
    Database() { };

public:
    Database(Database const&) = delete;
    Database(Database&&) = delete;
    Database& operator=(Database const&) = delete;
    Database& operator=(Database&&) = delete;

    virtual int get_population(const std::string& name) = 0;
};

class SingletonDatabase : public Database {
    SingletonDatabase() { };
    std::map<std::string, int> capitals;

public:
    SingletonDatabase(SingletonDatabase const&) = delete;
    void operator=(SingletonDatabase const&) = delete;
    static SingletonDatabase& get() {
        // use pointer to ensure the destructor will never be called.
        static SingletonDatabase* db;
        return *db;
    }

    int get_population(const std::string& name) override {
        auto it = capitals.find(name);
        if (it != capitals.end()) {
            return it->second;
        }
        return -1;
    }
};

struct ConfigurableRecordFinder {
    Database& db;
    explicit ConfigurableRecordFinder(Database& db) : db{db} { };

    int total_population(std::vector<std::string> names) {
        int result = 0;
        for (auto& name : names) result += db.get_population(name);
        return result;
    }
};

// Dummy database for Testing
class DummyDatabase : public Database {
std::map<std::string, int> capitals;

public:
    DummyDatabase() {
        capitals["alpha"] = 1;
        capitals["beta"] = 2;
        capitals["gamma"] = 3;
    }

    int get_population(const std::string& name) override {
        return capitals[name];
    }
};

TEST(DatabaseTests, DummyDatabaseTest) {
    DummyDatabase db;
    EXPECT_EQ(db.get_population("alpha"), 1);
    EXPECT_EQ(db.get_population("beta"), 2);
    EXPECT_EQ(db.get_population("gamma"), 3);
    EXPECT_EQ(db.get_population("delta"), 0); // non-existent entry
}

TEST(DatabaseTests, ConfigurableRecordFinderTest) {
    DummyDatabase db;
    ConfigurableRecordFinder rf{db};
    
    EXPECT_EQ(rf.total_population({"alpha", "gamma"}), 4);
    EXPECT_EQ(rf.total_population({"alpha", "beta", "gamma"}), 6);
    EXPECT_EQ(rf.total_population({"delta"}), 0); // non-existent city
}

// Modify main() to run the tests
int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
