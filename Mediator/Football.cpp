#include <iostream>
#include <string>
#include "boost/signals2.hpp"
#include <gtest/gtest.h>


struct EventData {
    virtual ~EventData() = default;
    virtual void print() const = 0;
};

struct PlayerScoredEventData : EventData {
    std::string player_name;
    int goals_scored_so_far;

    PlayerScoredEventData(const std::string& player_name, const int goals_scored_so_far)
        : player_name(player_name), goals_scored_so_far(goals_scored_so_far) { }

    void print() const override {
        std::cout << player_name << " has scored! (their " << goals_scored_so_far
                  << " goal)"
                  << "\n";
    }
};

struct Game {
    boost::signals2::signal<void(EventData*)> events; // observer
};

struct Player {
    std::string name;
    int goals_scored = 0;
    Game& game;

    Player(const std::string& name, Game& game)
        : name(name), game(game) { }

    void score() {
        ++goals_scored;
        PlayerScoredEventData event_data(name, goals_scored);
        event_data.print();
        game.events(&event_data);
    }
};

struct Coach {
    Game& game;

    explicit Coach(Game& game) : game(game) {
        // celebrate if player has scored < 3 goals
        game.events.connect([](EventData* event_data) {
            auto ps = dynamic_cast<PlayerScoredEventData*>(event_data);
            if (ps && ps->goals_scored_so_far < 3) {
                std::cout << "coach says: Well done! " << ps->player_name
                          << "\n";
            }
        });
    }
};

class FootballTest : public ::testing::Test {
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

TEST_F(FootballTest, PlayerScoresFirstGoal) {
    Game game;
    Player player("John", game);
    Coach coach(game);

    player.score();

    std::string expected = "John has scored! (their 1 goal)\n"
                          "coach says: Well done! John\n";
    EXPECT_EQ(output.str(), expected);
}

TEST_F(FootballTest, PlayerScoresSecondGoal) {
    Game game;
    Player player("John", game);
    Coach coach(game);

    player.score();
    output.str(""); // Clear buffer
    player.score();

    std::string expected = "John has scored! (their 2 goal)\n"
                          "coach says: Well done! John\n";
    EXPECT_EQ(output.str(), expected);
}

TEST_F(FootballTest, PlayerScoresThirdGoal) {
    Game game;
    Player player("John", game);
    Coach coach(game);

    player.score();
    player.score();
    output.str(""); // Clear buffer
    player.score();

    // Coach doesn't congratulate after 3rd goal
    std::string expected = "John has scored! (their 3 goal)\n";
    EXPECT_EQ(output.str(), expected);
}

TEST_F(FootballTest, MultiplePlayersScoring) {
    Game game;
    Player player1("John", game);
    Player player2("Jane", game);
    Coach coach(game);

    player1.score();
    output.str(""); // Clear buffer
    player2.score();

    std::string expected = "Jane has scored! (their 1 goal)\n"
                          "coach says: Well done! Jane\n";
    EXPECT_EQ(output.str(), expected);
}

TEST_F(FootballTest, PlayerGoalsCount) {
    Game game;
    Player player("John", game);
    Coach coach(game);

    EXPECT_EQ(player.goals_scored, 0);
    player.score();
    EXPECT_EQ(player.goals_scored, 1);
    player.score();
    EXPECT_EQ(player.goals_scored, 2);
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
