#include <string>
#include <iostream>
#include <gtest/gtest.h>


class Person;
class PersonBuilderBase;
class PersonBuilder;
class PersonAddressBuilder;
class PersonJobBuilder;

class Person {
    // address
    std::string street_address, post_code, city;

    // employment
    std::string company_name, position;
    int annual_income = 0;
    Person() { };

    friend class PersonBuilder;
    friend class PersonAddressBuilder;
    friend class PersonJobBuilder;
    friend std::ostream& operator<<(std::ostream& os, const Person& person);

public:
    static PersonBuilder create();

    // Getter methods for gtest
    const std::string_view get_street_address() const { return street_address; }
    const std::string_view get_post_code() const { return post_code; }
    const std::string_view get_city() const { return city; }
    const std::string_view get_company_name() const { return company_name; }
    const std::string_view get_position() const { return position; }
    int get_annual_income() const { return annual_income; }
};

class PersonBuilderBase {
protected:
    Person& person;
    explicit PersonBuilderBase(Person& person) : person(person) { }

public:
    operator Person() { return std::move(person); }     // implicit conversion
    // Person build() && { return std::move(person); }  // explicit conversion

    // builder facets
    PersonAddressBuilder lives() const;
    PersonJobBuilder works() const;
};

class PersonBuilder : public PersonBuilderBase {
    Person p; // object being built

public:
    PersonBuilder() : PersonBuilderBase(p) { }
};

class PersonAddressBuilder : public PersonBuilderBase {
    typedef PersonAddressBuilder self;

public:
    explicit PersonAddressBuilder(Person& person) : PersonBuilderBase(person) { }
    self& at(const std::string& street_address) {
        person.street_address = street_address;
        return *this;
    }
    self& with_postcode(const std::string& post_code) {
        person.post_code = post_code;
        return *this;
    }
    self& in(const std::string& city) {
        person.city = city;
        return *this;
    }
};

class PersonJobBuilder : public PersonBuilderBase {
    typedef PersonJobBuilder self;

public:
    explicit PersonJobBuilder(Person& person) : PersonBuilderBase(person) { }
    self& at(const std::string& company_name) {
        person.company_name = company_name;
        return *this;
    }
    self& as_a(const std::string& position) {
        person.position = position;
        return *this;
    }
    self& earning(int annual_income) {
        person.annual_income = annual_income;
        return *this;
    }
};

PersonBuilder Person::create() {
    return PersonBuilder();
}

PersonAddressBuilder PersonBuilderBase::lives() const {
    return PersonAddressBuilder(person);
}

PersonJobBuilder PersonBuilderBase::works() const {
    return PersonJobBuilder(person);
}

std::ostream& operator<<(std::ostream& os, const Person& p) {
    os << "Street Address: " << p.street_address << "\n"
       << "Post Code: " << p.post_code << "\n"
       << "City: " << p.city << "\n"
       << "Company Name: " << p.company_name << "\n"
       << "Position: " << p.position << "\n"
       << "Annual Income: " << p.annual_income << "\n";

    return os;
}

TEST(PersonBuilderTest, BuildPersonWithAddressAndJob) {
    Person p = Person::create()
                .lives()
                    .at("123 Test St")
                    .with_postcode("12345")
                    .in("Test City")
                .works()
                    .at("Test Company")
                    .as_a("Developer")
                    .earning(100000);

    EXPECT_EQ(p.get_street_address(), "123 Test St");
    EXPECT_EQ(p.get_post_code(), "12345");
    EXPECT_EQ(p.get_city(), "Test City");
    EXPECT_EQ(p.get_company_name(), "Test Company");
    EXPECT_EQ(p.get_position(), "Developer");
    EXPECT_EQ(p.get_annual_income(), 100000);
}

TEST(PersonBuilderTest, BuildPersonWithOnlyAddress) {
    Person p = Person::create()
                .lives()
                    .at("456 Test Ave")
                    .with_postcode("67890")
                    .in("Another City");

    EXPECT_EQ(p.get_street_address(), "456 Test Ave");
    EXPECT_EQ(p.get_post_code(), "67890");
    EXPECT_EQ(p.get_city(), "Another City");
    EXPECT_EQ(p.get_company_name(), "");
    EXPECT_EQ(p.get_position(), "");
    EXPECT_EQ(p.get_annual_income(), 0);
}

TEST(PersonBuilderTest, BuildPersonWithOnlyJob) {
    Person p = Person::create()
                .works()
                    .at("Another Company")
                    .as_a("Manager")
                    .earning(150000);

    EXPECT_EQ(p.get_street_address(), "");
    EXPECT_EQ(p.get_post_code(), "");
    EXPECT_EQ(p.get_city(), "");
    EXPECT_EQ(p.get_company_name(), "Another Company");
    EXPECT_EQ(p.get_position(), "Manager");
    EXPECT_EQ(p.get_annual_income(), 150000);
}

TEST(PersonBuilderTest, BuildEmptyPerson) {
    Person p = Person::create();

    EXPECT_EQ(p.get_street_address(), "");
    EXPECT_EQ(p.get_post_code(), "");
    EXPECT_EQ(p.get_city(), "");
    EXPECT_EQ(p.get_company_name(), "");
    EXPECT_EQ(p.get_position(), "");
    EXPECT_EQ(p.get_annual_income(), 0);
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
