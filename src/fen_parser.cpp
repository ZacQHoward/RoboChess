#include "fen_parser.h"
#include <string>
#include <unordered_map>
#include <cctype>
#include <nlohmann/json.hpp>
#include <fstream>


// Parse a FEN string into its components
FENParts splitFEN(const std::string& fen) {
    FENParts fenParts;
    size_t start = 0;
    size_t index = 0;

    while (start < fen.length() && index < 6) {
        size_t end = fen.find(' ', start);
        std::string part = (end == std::string::npos) ? fen.substr(start) : fen.substr(start, end - start);

        switch (index) {
            case 0:
                fenParts.board = part;
                break;
            case 1:
                fenParts.activeColor = part[0];
                break;
            case 2:
                fenParts.castling = part;
                break;
            case 3:
                fenParts.enPassant = part;
                break;
            case 4:
                fenParts.halfmoveClock = std::stoi(part);
                break;
            case 5:
                fenParts.fullmoveNumber = std::stoi(part);
                break;
        }

        if (end == std::string::npos) {
            break;
        }
        start = end + 1;
        index++;
    }

    return fenParts;
}

// Map pieces from the board part of the FEN string to their positions and image paths
BoardMapping mapPieces(const std::string& board) {
    std::unordered_map<std::string, PieceInfo> boardMapping;
    const std::string files = "ABCDEFGH";
    size_t rank = 8;
    size_t file = 0;
    int id = 0;

    for (char c : board) {
        if (c == '/') {
            rank--;
            file = 0;
        } else if (isdigit(c)) {
            file += c - '0';
        } else {
            char color = isupper(c) ? 'w' : 'b';
            std::string square = std::string(1, files[file]) + std::to_string(rank);
            std::string imagePath = "images/" + std::string(1, color) + static_cast<char>(tolower(c)) + ".svg";

            PieceInfo pieceInfo = {square, imagePath, c};
            boardMapping[std::to_string(id)] = pieceInfo;
            id++;
            file++;
        }
    }

    return boardMapping;
}

// Write the board mapping to a JSON file
void writeJSON(const BoardMapping& boardMapping, const std::string& filename) {
    nlohmann::json j;
    std::string jsonPath = std::string(GUI_PATH) + "/" + filename;

    for (const auto& [id, pieceInfo] : boardMapping) {
        j[id] = {
            pieceInfo.square,
            pieceInfo.imagePath,
            std::string(1, pieceInfo.piece)
        };
    }

    std::ofstream file(jsonPath);
    file << j.dump(4);
}