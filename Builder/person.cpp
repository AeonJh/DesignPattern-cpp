#include <string>
#include <iostream>


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

int main() {
    Person p = Person::create()
                .lives()
                .at("123 London Road")
                .with_postcode("SW1 1GB")
                .in("London")
                .works()
                .at("PragmaSoft")
                .as_a("Consultant")
                .earning(10e6);

    std::cout << p << std::endl;
    return 0;
}
