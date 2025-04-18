#include <iostream>
#include <string>
#include <vector>
#include <algorithm> // for std::find_if


struct Person;

struct Chatroom {
    std::vector<Person*> people;
    void join(Person* person);
    void broadcast(const std::string& origin, const std::string& message);
    void message(const std::string& origin, const std::string& who, const std::string& message);
    void leave(Person* person);
};

struct Person {
    std::string name;
    Chatroom* room = nullptr;
    std::vector<std::string> messages;
    Person(const std::string& name) : name(name) { }
    void receive(const std::string& origin, const std::string& message);
    void say(const std::string& message) const;
    void pm(const std::string& who, const std::string& message) const;
};

void Chatroom::join(Person* person) {
    people.push_back(person);
    person->room = this;
    broadcast("room", person->name + " has joined the chat");
}

void Chatroom::broadcast(const std::string& origin, const std::string& message) {
    for (auto person : people) {
        if (person->name != origin) person->receive(origin, message);
    }
}

void Chatroom::message(const std::string& origin, const std::string& who,
                       const std::string& message) {
    auto targetPerson = std::find_if(std::begin(people), std::end(people),
                               [&](const Person* person) { return person->name == who; });
    if (targetPerson != std::end(people)) {
        (*targetPerson)->receive(origin, message);
    } else {
        auto originPerson = std::find_if(std::begin(people), std::end(people),
                                          [&](const Person* person) { return person->name == origin; });
        if (originPerson != std::end(people)) {
            (*originPerson)->receive(origin, "User " + who + " not found");
        }
    }
}

void Person::say(const std::string& message) const {
    if (room) room->broadcast(name, message);
}

void Person::pm(const std::string& who, const std::string& message) const {
    if (room) {
        room->message(name, who, message);
    }
}

void Person::receive(const std::string& origin, const std::string& message) {
    std::string s{origin + ": \"" + message + "\""};
    messages.emplace_back(s);
    std::cout << "[" << name << "'s chat session] " << s << "\n";
}

void Chatroom::leave(Person* person) {
    auto it = std::remove(people.begin(), people.end(), person);
    if (it != people.end()) {
        people.erase(it, people.end());
        broadcast("room", person->name + " has left the chat");
        person->room = nullptr;
    }
}


int main() {
    Chatroom chatroom;
    Person alice("Alice");
    Person bob("Bob");
    Person charlie("Charlie");

    chatroom.join(&alice);
    chatroom.join(&bob);
    chatroom.join(&charlie);

    alice.say("Hello everyone!");
    bob.pm("Alice", "Hi Alice!");
    charlie.pm("Bob", "Hey Bob!");

    chatroom.leave(&bob);
    alice.say("Goodbye Bob!");

    return 0;
}
