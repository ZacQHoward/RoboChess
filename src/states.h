#pragma once

enum class State {
    INIT,
    HUMAN_TURN,
    ENGINE_TURN,
    GAME_OVER
};

// Declare all state handler functions
State handle_init();
State handle_human_turn();
State handle_engine_turn();
State handle_game_over();
