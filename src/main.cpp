#include <iostream>
#include <unordered_map>
#include <functional>

#include "states.h"
#include "fen_parser.h"

// Test FEN string representing a specific chess position (mate in one for white, black to move)
const std::string FEN_STRING = "K5k1/8/8/8/8/8/6R1/5Q2 b - - 0 1";

int main() {

    FENParts fenParts = splitFEN(FEN_STRING);
    std::cout << "Board: " << fenParts.board << "\n";
    std::cout << "Active color: " << fenParts.activeColor << "\n";
    std::cout << "Castling: " << fenParts.castling << "\n";
    std::cout << "En passant: " << fenParts.enPassant << "\n";
    std::cout << "Halfmove clock: " << fenParts.halfmoveClock << "\n";
    std::cout << "Fullmove number: " << fenParts.fullmoveNumber << "\n";

    writeJSON(mapPieces(fenParts.board), "board_state.json");

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
