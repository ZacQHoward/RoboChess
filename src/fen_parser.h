#pragma once

#include <string>
#include <unordered_map>

struct FENParts {
    std::string board;       
    char activeColor;         
    std::string castling;     
    std::string enPassant;    
    int halfmoveClock;
    int fullmoveNumber;
};

struct PieceInfo {
    std::string square;
    std::string imagePath;
    char piece;
};

using BoardMapping = std::unordered_map<std::string, PieceInfo>;

FENParts splitFEN(const std::string& fen);
BoardMapping mapPieces(const std::string& board);
void writeJSON(const BoardMapping& boardMapping, const std::string& filename);
