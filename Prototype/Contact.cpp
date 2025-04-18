#include <string>
#include <sstream>
#include <iostream>
#include "boost/archive/text_iarchive.hpp"
#include "boost/archive/text_oarchive.hpp"
#include "boost/serialization/access.hpp"
#include "boost/serialization/export.hpp"


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

BOOST_CLASS_EXPORT(Address)
int main() {
    Address* address = new Address{"123 Main St", "New York", 101};

    Contact contact1{"John Doe", address};

    // Copy construct
    {
        Contact contact2 = contact1;
        contact2.name = "Carl Mar";    // Change name of copied contact

        // Clone the contact
        Contact* contact3 = contact1.clone();
        contact3->address->street = "456 Elma St"; // Change address of cloned contact
        contact3->address->suite = 202;           // Change suite of cloned contact

        std::cout << "Contact 2: " << contact2.name << ", " << contact2.address->street << ", " << contact2.address->city << ", " << contact2.address->suite << std::endl;
        std::cout << "Contact 3: " << contact3->name << ", " << contact3->address->street << ", " << contact3->address->city << ", " << contact3->address->suite << std::endl;

        // Clean up
        delete contact3;
    }
    // Clone from serialization
    Contact contact4 = clone_from_serialization(contact1);
    contact4.name = "Gimi Mar"; // Change name of cloned contact
    contact4.address->street = "789 Elma St"; // Change address of cloned contact
    std::cout << "Contact 4: " << contact4.name << ", " << contact4.address->street << ", " << contact4.address->city << ", " << contact4.address->suite << std::endl;
    std::cout << "Contact 1: " << contact1.name << ", " << contact1.address->street << ", " << contact1.address->city << ", " << contact1.address->suite << std::endl;

    // Using factory
    auto contact5 = EmployeeFactory::NewMainOfficeEmployee("Carl Mar", 101);
    auto contact6 = EmployeeFactory::NewAuxOfficeEmployee("Gimi Mar", 102);
    std::cout << "Contact 5: " << contact5->name << ", " << contact5->address->street << ", " << contact5->address->city << ", " << contact5->address->suite << std::endl;
    std::cout << "Contact 6: " << contact6->name << ", " << contact6->address->street << ", " << contact6->address->city << ", " << contact6->address->suite << std::endl;

    return 0;
}
// Output:
// Contact 2: Carl Mar, 123 Main St, New York, 101
// Contact 3: John Doe, 456 Elma St, New York, 202
// Contact 4: Gimi Mar, 789 Elma St, New York, 101
// Contact 1: John Doe, 123 Main St, New York, 101
// Contact 5: Carl Mar, 123 Main St, New York, 101
// Contact 6: Gimi Mar, 123B Aux St, New York, 102
