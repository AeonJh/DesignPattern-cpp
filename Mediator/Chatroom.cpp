#include <iostream>
#include <string>
#include <vector>
#include <algorithm> // for std::find_if
#include <gtest/gtest.h>


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

class ChatroomTest : public ::testing::Test {
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

TEST_F(ChatroomTest, PersonJoiningChat) {
    Chatroom room;
    Person john("John");
    room.join(&john);
    
    EXPECT_EQ(room.people.size(), size_t(1));
    EXPECT_EQ(john.room, &room);
    EXPECT_EQ(output.str(), "[John's chat session] room: \"John has joined the chat\"\n");
}

TEST_F(ChatroomTest, BroadcastMessage) {
    Chatroom room;
    Person john("John");
    Person jane("Jane");
    Person bob("Bob");
    
    room.join(&john);
    room.join(&jane);
    room.join(&bob);
    EXPECT_EQ(room.people.size(), size_t(3));
    
    output.str(""); // Clear buffer
    john.say("Hello everyone!");
    
    std::string expected = "[Jane's chat session] John: \"Hello everyone!\"\n"
                          "[Bob's chat session] John: \"Hello everyone!\"\n";
    EXPECT_EQ(output.str(), expected);
}

TEST_F(ChatroomTest, PrivateMessage) {
    Chatroom room;
    Person john("John");
    Person jane("Jane");
    Person bob("Bob");
    
    room.join(&john);
    room.join(&jane);
    room.join(&bob);
    
    output.str(""); // Clear buffer
    john.pm("Jane", "Hi Jane!");
    
    EXPECT_EQ(output.str(), "[Jane's chat session] John: \"Hi Jane!\"\n");
}

TEST_F(ChatroomTest, PersonLeavingChat) {
    Chatroom room;
    Person john("John");
    Person jane("Jane");

    room.join(&john);
    room.join(&jane);
    EXPECT_EQ(room.people.size(), size_t(2));

    output.str(""); // Clear buffer
    room.leave(&john);

    EXPECT_EQ(room.people.size(), size_t(1));
    EXPECT_EQ(john.room, nullptr);
    EXPECT_EQ(output.str(), "[Jane's chat session] room: \"John has left the chat\"\n");

    room.leave(&jane);
    EXPECT_EQ(room.people.size(), size_t(0));
    EXPECT_EQ(jane.room, nullptr);
}

TEST_F(ChatroomTest, PrivateMessageToNonExistentUser) {
    Chatroom room;
    Person john("John");

    room.join(&john);

    output.str(""); // Clear buffer
    john.pm("NonExistent", "Hello!");

    EXPECT_EQ(output.str(), "[John's chat session] John: \"User NonExistent not found\"\n");
}

TEST_F(ChatroomTest, MessageStorage) {
    Chatroom room;
    Person john("John");
    Person jane("Jane");

    room.join(&john);
    room.join(&jane);

    john.say("Hello!");

    EXPECT_EQ(jane.messages.size(), size_t(2));
    EXPECT_EQ(jane.messages[0], "room: \"Jane has joined the chat\"");
    EXPECT_EQ(jane.messages[1], "John: \"Hello!\"");
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
