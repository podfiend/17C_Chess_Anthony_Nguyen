#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <algorithm>

using namespace std;

// Constants
const int BOARD_SIZE = 8;

// Piece types
enum class PieceType { KING, QUEEN, ROOK, BISHOP, KNIGHT, PAWN, NONE };

// Piece colors
enum class PieceColor { WHITE, BLACK, NONE };

// Define piece structure
struct Piece {
    PieceType type = PieceType::NONE;
    PieceColor color = PieceColor::NONE;
};

// Chessboard
vector<vector<Piece>> board(BOARD_SIZE, vector<Piece>(BOARD_SIZE));

// Map to convert piece type to symbol
map<PieceType, string> pieceSymbols = {
    {PieceType::KING, "K"},
    {PieceType::QUEEN, "Q"},
    {PieceType::ROOK, "R"},
    {PieceType::BISHOP, "B"},
    {PieceType::KNIGHT, "N"},
    {PieceType::PAWN, "P"},
    {PieceType::NONE, " "}
};

// Function prototype for isValidKingMove
bool isValidKingMove(int startX, int startY, int endX, int endY);

// Function to initialize the chessboard
void initializeBoard() {
    // Initialize pawns
    for (int col = 0; col < BOARD_SIZE; col++) {
        board[1][col] = {PieceType::PAWN, PieceColor::WHITE};
        board[6][col] = {PieceType::PAWN, PieceColor::BLACK};
    }

    // Initialize major pieces
    board[0][0] = {PieceType::ROOK, PieceColor::WHITE};
    board[0][1] = {PieceType::KNIGHT, PieceColor::WHITE};
    board[0][2] = {PieceType::BISHOP, PieceColor::WHITE};
    board[0][3] = {PieceType::QUEEN, PieceColor::WHITE};
    board[0][4] = {PieceType::KING, PieceColor::WHITE};
    board[0][5] = {PieceType::BISHOP, PieceColor::WHITE};
    board[0][6] = {PieceType::KNIGHT, PieceColor::WHITE};
    board[0][7] = {PieceType::ROOK, PieceColor::WHITE};

    board[7][0] = {PieceType::ROOK, PieceColor::BLACK};
    board[7][1] = {PieceType::KNIGHT, PieceColor::BLACK};
    board[7][2] = {PieceType::BISHOP, PieceColor::BLACK};
    board[7][3] = {PieceType::QUEEN, PieceColor::BLACK};
    board[7][4] = {PieceType::KING, PieceColor::BLACK};
    board[7][5] = {PieceType::BISHOP, PieceColor::BLACK};
    board[7][6] = {PieceType::KNIGHT, PieceColor::BLACK};
    board[7][7] = {PieceType::ROOK, PieceColor::BLACK};
}

// Function to display the chessboard
void displayBoard() {
    cout << "  a b c d e f g h" << endl;
    for (int row = 0; row < BOARD_SIZE; row++) {
        cout << 8 - row << " ";
        for (int col = 0; col < BOARD_SIZE; col++) {
            cout << pieceSymbols[board[row][col].type] << " ";
        }
        cout << 8 - row << endl;
    }
    cout << "  a b c d e f g h" << endl;
}

// Function to convert algebraic notation to board coordinates
bool isValidCoordinate(int x, int y) {
    return x >= 0 && x < BOARD_SIZE && y >= 0 && y < BOARD_SIZE;
}

// Function to check if a move is valid
bool isValidMove(PieceType type, int startX, int startY, int endX, int endY) {
    // Validate coordinates
    if (!isValidCoordinate(startX, startY) || !isValidCoordinate(endX, endY)) {
        cout << "Invalid Move. Try again." << endl;
        return false;
    }

    // Check if moving to the same position
    if (startX == endX && startY == endY) {
        cout << "Invalid Move. Try again." << endl;
        return false;
    }

    // Placeholder logic for valid moves (simplified rules)
    switch (type) {
        case PieceType::KING:
            // Implement king movement rules
            return isValidKingMove(startX, startY, endX, endY);
        case PieceType::QUEEN:
            // Implement queen movement rules
            // return isValidQueenMove(startX, startY, endX, endY);
            return true; // Placeholder for now
        case PieceType::ROOK:
            // Implement rook movement rules
            // return isValidRookMove(startX, startY, endX, endY);
            return true; // Placeholder for now
        case PieceType::BISHOP:
            // Implement bishop movement rules
            // return isValidBishopMove(startX, startY, endX, endY);
            return true; // Placeholder for now
        case PieceType::KNIGHT:
            // Implement knight movement rules
            // return isValidKnightMove(startX, startY, endX, endY);
            return true; // Placeholder for now
        case PieceType::PAWN:
            // Implement pawn movement rules
            // return isValidPawnMove(startX, startY, endX, endY);
            return true; // Placeholder for now
        default:
            return false; // Invalid type
    }
}

// Function to check if a move is valid for the king
bool isValidKingMove(int startX, int startY, int endX, int endY) {
    int dx = abs(endX - startX);
    int dy = abs(endY - startY);
    if ((dx == 1 || dy == 1) && (dx <= 1 && dy <= 1)) {
        // Valid king move
        return true;
    } else if (dx == 2 && dy == 0 && startY == 0) {
        // Check for castling
        // Placeholder logic for castling
        return true;
    } else {
        cout << "Invalid Move. Try again." << endl;
        return false;
    }
}

// Function to check if a move is valid for the queen
bool isValidQueenMove(int startX, int startY, int endX, int endY) {
    int dx = abs(endX - startX);
    int dy = abs(endY - startY);
    return (dx == dy) || (startX == endX) || (startY == endY);
}

// Function to check if a move is valid for the rook
bool isValidRookMove(int startX, int startY, int endX, int endY) {
    return (startX == endX) || (startY == endY);
}

// Function to check if a move is valid for the bishop
bool isValidBishopMove(int startX, int startY, int endX, int endY) {
    int dx = abs(endX - startX);
    int dy = abs(endY - startY);
    return dx == dy;
}

// Function to check if a move is valid for the knight
bool isValidKnightMove(int startX, int startY, int endX, int endY) {
    int dx = abs(endX - startX);
    int dy = abs(endY - startY);
    return ((dx == 1 && dy == 2) || (dx == 2 && dy == 1)) && (dx + dy == 3);
}

// Function to check if a move is valid for the pawn
bool isValidPawnMove(int startX, int startY, int endX, int endY, PieceColor color) {
    int dx = endX - startX;
    int dy = endY - startY;

    // Check direction based on piece color
    int forwardDir = (color == PieceColor::WHITE) ? 1 : -1;

    // Check if move is within bounds
    if (!isValidCoordinate(startX, startY) || !isValidCoordinate(endX, endY)) {
        cout << "Invalid Move. Try again." << endl;
        return false;
    }

    // Check if moving to the same position
    if (startX == endX && startY == endY) {
        cout << "Invalid Move. Try again." << endl;
        return false;
    }

    // Pawn moves forward by 1 square
    if (dy == forwardDir && dx == 0) {
        return true;
    }

    // Pawn moves forward by 2 squares from starting position
    if (dy == 2 * forwardDir && dx == 0 && startY == 1 && color == PieceColor::WHITE) {
        return true;
    } else if (dy == 2 * forwardDir && dx == 0 && startY == 6 && color == PieceColor::BLACK) {
        return true;
    }

    // Pawn captures diagonally
    if (abs(dy) == 1 && abs(dx) == 1) {
        // Check if capturing an opponent's piece
        Piece targetPiece = board[endY][endX];
        if (targetPiece.color != color && targetPiece.type != PieceType::NONE) {
            return true;
        } else {
            cout << "Invalid Move. Try again." << endl;
            return false;
        }
    }

    // Invalid pawn move
    cout << "Invalid Move. Try again." << endl;
    return false;
}

// Function to convert algebraic notation to board coordinates
pair<int, int> convertAlgebraicToCoordinates(const string& move) {
    if (move.length() != 2) {
        return {-1, -1};
    }
    char colChar = move[0];
    int row = move[1] - '0';
    if (row < 1 || row > 8) {
        return {-1, -1};
    }
    int col = -1;
    switch (colChar) {
        case 'a': col = 0; break;
        case 'b': col = 1; break;
        case 'c': col = 2; break;
        case 'd': col = 3; break;
        case 'e': col = 4; break;
        case 'f': col = 5; break;
        case 'g': col = 6; break;
        case 'h': col = 7; break;
        default: return {-1, -1};
    }
    return {BOARD_SIZE - row, col};
}

// Function to check if a move is valid and update the board
bool makeMove(const string& start, const string& end) {
    pair<int, int> startCoord = convertAlgebraicToCoordinates(start);
    pair<int, int> endCoord = convertAlgebraicToCoordinates(end);
    if (startCoord.first == -1 || startCoord.second == -1 || endCoord.first == -1 || endCoord.second == -1) {
        cout << "Invalid Move. Try again." << endl;
        return false;
    }
    Piece startPiece = board[startCoord.first][startCoord.second];
    Piece endPiece = board[endCoord.first][endCoord.second];
    if (startPiece.type == PieceType::NONE || startPiece.color == PieceColor::NONE) {
        cout << "No piece at the starting position. Try again." << endl;
        return false;
    }
    if (!isValidMove(startPiece.type, startCoord.first, startCoord.second, endCoord.first, endCoord.second)) {
        cout << "Invalid Move. Try again." << endl;
        return false;
    }
    board[startCoord.first][startCoord.second] = {PieceType::NONE, PieceColor::NONE}; // Remove piece from start
    board[endCoord.first][endCoord.second] = startPiece; // Move piece to end
    return true;
}

int main() {
    initializeBoard();
    displayBoard();

    while (true) {
        string start, end;
        cout << "Enter move (e.g., a2 a4): ";
        cin >> start >> end;
        if (makeMove(start, end)) {
            displayBoard();
        }
    }

    return 0;
}
