#include <iostream>
#include <string>
#include "boost/signals2.hpp"


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

int main() {
    Game game;
    Player player("John", game);
    Coach coach(game);

    player.score();
    player.score();
    player.score(); // ignored by the coach

    return 0;
}
