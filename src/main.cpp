#include <iostream>
#include <unordered_map>
#include <functional>

#include "states.h"

int main() {

    std::unordered_map<State, std::function<State()>> handlers = {
        { State::INIT, handle_init },
        { State::HUMAN_TURN, handle_human_turn },
        { State::ENGINE_TURN, handle_engine_turn },
        { State::GAME_OVER, handle_game_over }
    };

    State current_state = State::INIT;

    while (true) {
        current_state = handlers[current_state]();
        if (current_state == State::GAME_OVER) {
            handlers[current_state]();
            break;
        }
    }

    return 0;
}
