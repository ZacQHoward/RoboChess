#include "states.h"
#include <iostream>

// INIT State
State handle_init() {
    std::cout << "Initializing...\n";
    return State::HUMAN_TURN;
}

// HUMAN_TURN State
State handle_human_turn() {
    std::cout << "Human make your move...\n";

    // Quick logic for exiting loop
    static int count = 0;
    count++;
    if (count >= 2) {return State::GAME_OVER;}

    return State::ENGINE_TURN;
}

// ENGINE_TURN State
State handle_engine_turn() {
    std::cout << "Robot making a move...\n";
    return State::HUMAN_TURN;
}

// GAME_OVER State
State handle_game_over() {
    std::cout << "Game over.\n";
    return State::GAME_OVER;
}