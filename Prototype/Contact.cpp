#include <string>
#include <sstream>
#include <iostream>
#include "boost/archive/text_iarchive.hpp"
#include "boost/archive/text_oarchive.hpp"
#include "boost/serialization/access.hpp"
#include "boost/serialization/export.hpp"
#include <gtest/gtest.h>


template <typename T>
struct Cloneable {
    virtual T* clone() const = 0;
    virtual ~Cloneable() = default;
};

template <typename T>
struct CloneableImpl : public Cloneable<T> {
    T* clone() const override {
        return new T(*static_cast<const T*>(this));
    }
};

struct Address : public CloneableImpl<Address> {
    std::string street, city;
    int suite;

    Address() = default;
    Address(const std::string& street, const std::string& city, const int suite)
        : street{street}, city{city}, suite{suite} { }

private:
    friend class boost::serialization::access;
    template <typename Archive>
    void serialize(Archive& ar, const unsigned int version [[maybe_unused]]) {
        ar& street;
        ar& city;
        ar& suite;
    }
};

struct Contact : public CloneableImpl<Contact> {
    std::string name;
    Address* address = nullptr; // pointer (or e.g., shared_ptr)

    // Constructor
    Contact() = default;
    Contact(const std::string& name, Address* address)
        : name{name}, address{address} { }

    // 1. First Way: copy construct
    Contact(const Contact& other) 
        : name{other.name}, address{ new Address{*other.address} } { }

    // 2. Second way: clone method
    Contact* clone() const override {
        return new Contact(*this);
    }

    // Clean up
    ~Contact() {
        if (address != nullptr) {
            delete address;
        }
    }

private:
    friend class boost::serialization::access;
    template <typename Archive>
    void serialize(Archive& ar, const unsigned int version [[maybe_unused]]) {
        ar& name;
        ar& address; // no *
    }
};

// Prototype Factory
struct EmployeeFactory {
    static Contact main_offices;
    static Contact auxiliary_offices;
    static std::unique_ptr<Contact> NewMainOfficeEmployee(const std::string& name, int suite) {
        return NewEmployee(name, suite, main_offices);
    }
    static std::unique_ptr<Contact> NewAuxOfficeEmployee(const std::string& name, int suite) {
        return NewEmployee(name, suite, auxiliary_offices);
    }

private:
    static std::unique_ptr<Contact> NewEmployee(const std::string& name, int suite, 
                                                const Contact& prototype) {
        auto new_employee = std::make_unique<Contact>(prototype);
        new_employee->name = name;
        new_employee->address->suite = suite;
        return new_employee;
    }
};

auto clone_from_serialization = [](const Contact& contact) {
    // 1. Serialize the contact
    std::ostringstream oss;
    boost::archive::text_oarchive oa(oss);
    oa << contact;
    std::string serialized_data = oss.str();

    // 2. Deserialize the contact
    std::istringstream iss(serialized_data);
    boost::archive::text_iarchive ia(iss);
    Contact result;
    ia >> result;
    return result;
};

// Static members initialization
Contact EmployeeFactory::main_offices      = Contact{"", new Address{"123 Main St", "New York", 0}};
Contact EmployeeFactory::auxiliary_offices = Contact{"", new Address{"123B Aux St", "New York", 0}};

class ContactTest : public ::testing::Test {
protected:
    void SetUp() override {
        oldCoutBuf = std::cout.rdbuf(output.rdbuf());
    }

    void TearDown() override {
        std::cout.rdbuf(oldCoutBuf);
    }

    std::stringstream output;
    std::streambuf* oldCoutBuf;
};

TEST_F(ContactTest, AddressCopyConstruct) {
    Address addr1("123 Main St", "New York", 101);
    Address addr2 = addr1;

    EXPECT_EQ(addr2.street, "123 Main St");
    EXPECT_EQ(addr2.city, "New York");
    EXPECT_EQ(addr2.suite, 101);
}

TEST_F(ContactTest, ContactCopyConstruct) {
    Address* addr = new Address("123 Main St", "New York", 101);
    Contact contact1("John Doe", addr);
    Contact contact2 = contact1;

    EXPECT_EQ(contact2.name, "John Doe");
    EXPECT_EQ(contact2.address->street, "123 Main St");
    EXPECT_EQ(contact2.address->city, "New York");
    EXPECT_EQ(contact2.address->suite, 101);

    // Verify deep copy
    contact2.address->street = "456 Other St";
    EXPECT_EQ(contact1.address->street, "123 Main St");
    EXPECT_EQ(contact2.address->street, "456 Other St");
}

TEST_F(ContactTest, ContactCloneMethod) {
    Address* addr = new Address("123 Main St", "New York", 101);
    Contact contact1("John Doe", addr);
    Contact* contact2 = contact1.clone();

    EXPECT_EQ(contact2->name, "John Doe");
    EXPECT_EQ(contact2->address->street, "123 Main St");
    EXPECT_EQ(contact2->address->city, "New York");
    EXPECT_EQ(contact2->address->suite, 101);

    // Verify deep copy
    contact2->address->street = "789 New St";
    EXPECT_EQ(contact1.address->street, "123 Main St");
    EXPECT_EQ(contact2->address->street, "789 New St");

    delete contact2;
}

TEST_F(ContactTest, ContactSerialization) {
    Address* addr = new Address("123 Main St", "New York", 101);
    Contact contact1("John Doe", addr);
    Contact contact2 = clone_from_serialization(contact1);

    EXPECT_EQ(contact2.name, "John Doe");
    EXPECT_EQ(contact2.address->street, "123 Main St");
    EXPECT_EQ(contact2.address->city, "New York");
    EXPECT_EQ(contact2.address->suite, 101);

    // Verify deep copy
    contact2.address->street = "321 Clone St";
    EXPECT_EQ(contact1.address->street, "123 Main St");
    EXPECT_EQ(contact2.address->street, "321 Clone St");
}

TEST_F(ContactTest, EmployeeFactory) {
    auto contact1 = EmployeeFactory::NewMainOfficeEmployee("Jane Doe", 201);
    auto contact2 = EmployeeFactory::NewAuxOfficeEmployee("John Smith", 202);

    EXPECT_EQ(contact1->name, "Jane Doe");
    EXPECT_EQ(contact1->address->street, "123 Main St");
    EXPECT_EQ(contact1->address->suite, 201);

    EXPECT_EQ(contact2->name, "John Smith");
    EXPECT_EQ(contact2->address->street, "123B Aux St");
    EXPECT_EQ(contact2->address->suite, 202);
}

// BOOST_CLASS_EXPORT(Address)
int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
