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
    bool moved = false; // For tracking pawn moves and castling eligibility
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

// Map to convert algebraic notation to board coordinates
map<char, int> colToIndex = {
    {'a', 0}, {'b', 1}, {'c', 2}, {'d', 3},
    {'e', 4}, {'f', 5}, {'g', 6}, {'h', 7}
};

// Function to initialize the chessboard
void initializeBoard() {
    // Clear the board
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            board[i][j] = {PieceType::NONE, PieceColor::NONE};
        }
    }

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
pair<int, int> convertAlgebraicToCoordinates(const string& position) {
    if (position.size() != 2) return {-1, -1};
    int col = colToIndex[position[0]];
    int row = BOARD_SIZE - (position[1] - '0');
    return {row, col};
}

// Function to convert board coordinates to algebraic notation
string convertCoordinatesToAlgebraic(int row, int col) {
    string result;
    result += ('a' + col);
    result += ('0' + (BOARD_SIZE - row));
    return result;
}

// Function to check if a coordinate is valid
bool isValidCoordinate(int x, int y) {
    return x >= 0 && x < BOARD_SIZE && y >= 0 && y < BOARD_SIZE;
}

// Function to check if a move is within the board bounds
bool isValidMove(int startX, int startY, int endX, int endY) {
    return isValidCoordinate(startX, startY) && isValidCoordinate(endX, endY);
}

// Function to check if a square is under attack by an opponent's piece
bool isUnderAttack(int x, int y, PieceColor attackingColor) {
    // Check for attacks from opponent's pawns
    int forwardDir = (attackingColor == PieceColor::WHITE) ? -1 : 1;
    int pawnX[2] = {x - 1, x + 1};
    for (int i = 0; i < 2; i++) {
        int pawnY = y + forwardDir;
        if (isValidCoordinate(pawnX[i], pawnY) &&
            board[pawnY][pawnX[i]].type == PieceType::PAWN &&
            board[pawnY][pawnX[i]].color == attackingColor) {
            return true;
        }
    }

    // Check for attacks from opponent's knights
    int knightX[8] = {x - 2, x - 2, x - 1, x - 1, x + 1, x + 1, x + 2, x + 2};
    int knightY[8] = {y - 1, y + 1, y - 2, y + 2, y - 2, y + 2, y - 1, y + 1};
    for (int i = 0; i < 8; i++) {
        if (isValidCoordinate(knightX[i], knightY[i]) &&
            board[knightY[i]][knightX[i]].type == PieceType::KNIGHT &&
            board[knightY[i]][knightX[i]].color == attackingColor) {
            return true;
        }
    }

    // Check for attacks from opponent's bishops and queens (diagonal attacks)
    for (int dx = -1; dx <= 1; dx += 2) {
        for (int dy = -1; dy <= 1; dy += 2) {
            for (int dist = 1; dist < BOARD_SIZE; dist++) {
                int bx = x + dx * dist;
                int by = y + dy * dist;
                if (!isValidCoordinate(bx, by)) {
                    break; // Stop if out of board bounds
                }
                if (board[by][bx].type == PieceType::NONE) {
                    continue; // Empty square, keep checking
                } else if (board[by][bx].color == attackingColor &&
                           (board[by][bx].type == PieceType::BISHOP || board[by][bx].type == PieceType::QUEEN)) {
                    return true; // Opponent's bishop or queen can attack this square
                } else {
                    break; // Reached a piece that blocks further diagonal movement
                }
            }
        }
    }

    // Check for attacks from opponent's rooks and queens (horizontal and vertical attacks)
    for (int dir = -1; dir <= 1; dir += 2) {
        for (int dist = 1; dist < BOARD_SIZE; dist++) {
            int rx = x + dir * dist;
            int ry = y;
            if (!isValidCoordinate(rx, ry)) {
                break; // Stop if out of board bounds
            }
            if (board[ry][rx].type == PieceType::NONE) {
                continue; // Empty square, keep checking
            } else if (board[ry][rx].color == attackingColor &&
                       (board[ry][rx].type == PieceType::ROOK || board[ry][rx].type == PieceType::QUEEN)) {
                return true; // Opponent's rook or queen can attack this square
            } else {
                break; // Reached a piece that blocks further horizontal/vertical movement
            }
        }
        for (int dist = 1; dist < BOARD_SIZE; dist++) {
            int rx = x;
            int ry = y + dir * dist;
            if (!isValidCoordinate(rx, ry)) {
                break; // Stop if out of board bounds
            }
            if (board[ry][rx].type == PieceType::NONE) {
                continue; // Empty square, keep checking
            } else if (board[ry][rx].color == attackingColor &&
                       (board[ry][rx].type == PieceType::ROOK || board[ry][rx].type == PieceType::QUEEN)) {
                return true; // Opponent's rook or queen can attack this square
            } else {
                break; // Reached a piece that blocks further horizontal/vertical movement
            }
        }
    }

    // Check for attacks from opponent's kings
    int kingX[8] = {x - 1, x - 1, x - 1, x, x, x + 1, x + 1, x + 1};
    int kingY[8] = {y - 1, y, y + 1, y - 1, y + 1, y - 1, y, y + 1};
    for (int i = 0; i < 8; i++) {
        if (isValidCoordinate(kingX[i], kingY[i]) &&
            board[kingY[i]][kingX[i]].type == PieceType::KING &&
            board[kingY[i]][kingX[i]].color == attackingColor) {
            return true;
        }
    }

    // No attacks found
    return false;
}

// Function to check if a move is valid for the king
bool isValidKingMove(int startX, int startY, int endX, int endY) {
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

    // Check if the destination square is under attack
    PieceColor color = board[startY][startX].color;
    if (isUnderAttack(endX, endY, (color == PieceColor::WHITE) ? PieceColor::BLACK : PieceColor::WHITE)) {
        cout << "Invalid Move. Your king is in check." << endl;
        return false;
    }

    // Placeholder logic for valid king moves (simplified rules)
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
bool isValidPawnMove(int startX, int startY, int endX, int endY, PieceColor color, bool enPassantAllowed) {
    int dx = endX - startX;
    int dy = endY - startY;

    // Check direction based on piece color
    int forwardDir = (color == PieceColor::WHITE) ? 1 : -1;

    // Pawn moves forward by 1 square
    if (dy == forwardDir && dx == 0 && board[endX][endY].type == PieceType::NONE) {
        return true;
    }

    // Pawn moves forward by 2 squares from starting position
    if (dy == 2 * forwardDir && dx == 0 && startY == 1 && color == PieceColor::WHITE &&
        board[startX + forwardDir][startY].type == PieceType::NONE && board[endX][endY].type == PieceType::NONE) {
        return true;
    } else if (dy == 2 * forwardDir && dx == 0 && startY == 6 && color == PieceColor::BLACK &&
               board[startX + forwardDir][startY].type == PieceType::NONE && board[endX][endY].type == PieceType::NONE) {
        return true;
    }

    // Pawn captures diagonally
    if (abs(dx) == 1 && abs(dy) == 1 && board[endX][endY].type != PieceType::NONE) {
        return true;
    }

    // En passant capture
    if (enPassantAllowed && abs(dx) == 1 && abs(dy) == 1 && board[endX][endY].type == PieceType::NONE &&
        board[startX][endY].type == PieceType::PAWN && board[startX][endY].moved) {
        return true;
    }

    return false;
}

// Function to check if a move puts the king in check
bool isMoveLeavesKingInCheck(int startX, int startY, int endX, int endY, PieceColor color) {
    // Temporarily make the move
    Piece tempPiece = board[endX][endY];
    board[endX][endY] = board[startX][startY];
    board[startX][startY] = {PieceType::NONE, PieceColor::NONE};

    // Check if the king is under attack after the move
    bool isCheck = isUnderAttack(board[endX][endY].type == PieceType::KING ? endX : -1,
                                 board[endX][endY].type == PieceType::KING ? endY : -1, color);

    // Undo the move
    board[startX][startY] = board[endX][endY];
    board[endX][endY] = tempPiece;

    return isCheck;
}

// Function to check if a move is valid (including special moves)
bool isValidMove(PieceType type, int startX, int startY, int endX, int endY, PieceColor color, bool enPassantAllowed) {
    // Validate coordinates
    if (!isValidMove(startX, startY, endX, endY)) {
        return false;
    }

    // Check if moving to the same position
    if (startX == endX && startY == endY) {
        return false;
    }

    // Get the piece being moved
    Piece& piece = board[startX][startY];

    // Check if the piece is of the correct color
    if (piece.color != color) {
        return false;
    }

    // Check if the destination square has the same color piece
    if (board[endX][endY].color == piece.color) {
        return false;
    }

    // Placeholder logic for valid moves (simplified rules)
    switch (type) {
        case PieceType::KING:
          /*return isValidKingMove(startX, startY, endX, endY, color) &&
                   !isMoveLeavesKingInCheck(startX, startY, endX, endY, color);*/
            return isValidKingMove(startX, startY, endX, endY) && 
                    !isMoveLeavesKingInCheck(startX, startY, endX, endY, color);
            
        case PieceType::QUEEN:
            return isValidQueenMove(startX, startY, endX, endY) &&
                   !isMoveLeavesKingInCheck(startX, startY, endX, endY, color);
        case PieceType::ROOK:
            return isValidRookMove(startX, startY, endX, endY) &&
                   !isMoveLeavesKingInCheck(startX, startY, endX, endY, color);
        case PieceType::BISHOP:
            return isValidBishopMove(startX, startY, endX, endY) &&
                   !isMoveLeavesKingInCheck(startX, startY, endX, endY, color);
        case PieceType::KNIGHT:
            return isValidKnightMove(startX, startY, endX, endY) &&
                   !isMoveLeavesKingInCheck(startX, startY, endX, endY, color);
        case PieceType::PAWN:
            return isValidPawnMove(startX, startY, endX, endY, color, enPassantAllowed) &&
                   !isMoveLeavesKingInCheck(startX, startY, endX, endY, color);
        default:
            return false; // Invalid type
    }
}

// Function to handle pawn promotion
void promotePawn(int endX, int endY, PieceColor color) {
    char promotionChoice;
    cout << "Pawn promotion! Choose a piece to promote to (Q, R, B, N): ";
    cin >> promotionChoice;

    PieceType promotedType = PieceType::NONE;
    switch (promotionChoice) {
        case 'Q':
            promotedType = PieceType::QUEEN;
            break;
        case 'R':
            promotedType = PieceType::ROOK;
            break;
        case 'B':
            promotedType = PieceType::BISHOP;
            break;
        case 'N':
            promotedType = PieceType::KNIGHT;
            break;
        default:
            promotedType = PieceType::QUEEN; // Default to queen
            break;
    }

    board[endX][endY] = {promotedType, color};
}

// Function to handle en passant capture
void handleEnPassant(int startX, int startY, int endX, int endY) {
    if (board[startX][startY].type == PieceType::PAWN && abs(startX - endX) == 2) {
        board[startX][startY].moved = true; // Set the pawn's moved flag for en passant
    }
}

// Function to handle castling
void handleCastling(int startX, int startY, int endX, int endY) {
    if (board[startX][startY].type == PieceType::KING && abs(startY - endY) == 2) {
        // Kingside castling
        if (endY > startY) {
            board[startX][endY - 1] = board[startX][BOARD_SIZE - 1];
            board[startX][BOARD_SIZE - 1] = {PieceType::NONE, PieceColor::NONE};
        }
        // Queenside castling
        else {
            board[startX][endY + 1] = board[startX][0];
            board[startX][0] = {PieceType::NONE, PieceColor::NONE};
        }
        board[startX][startY].moved = true; // Set the king's moved flag for castling
    }
}

// Function to make a move
bool makeMove(int startX, int startY, int endX, int endY, PieceColor color, bool& enPassantAllowed) {
    if (!isValidMove(board[startX][startY].type, startX, startY, endX, endY, color, enPassantAllowed)) {
        cout << "Invalid move. Try again." << endl;
        return false;
    }

    // Handle special moves
    handleEnPassant(startX, startY, endX, endY);
    handleCastling(startX, startY, endX, endY);

    // Check for pawn promotion
    if (board[startX][startY].type == PieceType::PAWN && (endX == 0 || endX == BOARD_SIZE - 1)) {
        promotePawn(endX, endY, color);
    }

    // Make the move
    board[endX][endY] = board[startX][startY];
    board[startX][startY] = {PieceType::NONE, PieceColor::NONE};
    board[endX][endY].moved = true;

    return true;
}

// Function to check if a player is in checkmate
bool isCheckmate(PieceColor color) {
    // Check if the king is in check
    for (int row = 0; row < BOARD_SIZE; row++) {
        for (int col = 0; col < BOARD_SIZE; col++) {
            if (board[row][col].type == PieceType::KING && board[row][col].color == color) {
                if (isUnderAttack(row, col, color)) {
                    // Check if the king has any valid moves left
                    for (int dx = -1; dx <= 1; dx++) {
                        for (int dy = -1; dy <= 1; dy++) {
                            int newX = row + dx;
                            int newY = col + dy;
                            if (isValidMove(PieceType::KING, row, col, newX, newY, color, false) &&
                                !isMoveLeavesKingInCheck(row, col, newX, newY, color)) {
                                return false; // King can escape check
                            }
                        }
                    }

                    // Check if any other piece can capture or block the attacker
                    for (int i = 0; i < BOARD_SIZE; i++) {
                        for (int j = 0; j < BOARD_SIZE; j++) {
                            if (board[i][j].color == color && isValidMove(board[i][j].type, i, j, row, col, color, false) &&
                                !isMoveLeavesKingInCheck(i, j, row, col, color)) {
                                return false; // Piece can capture/block the attacker
                            }
                        }
                    }

                    return true; // King is in checkmate
                }
            }
        }
    }

    return false; // King is not in checkmate
}

// Function to check if the game is in stalemate
bool isStalemate(PieceColor color) {
    // Check if the king is not in check but has no valid moves left
    for (int row = 0; row < BOARD_SIZE; row++) {
        for (int col = 0; col < BOARD_SIZE; col++) {
            if (board[row][col].color == color) {
                for (int i = 0; i < BOARD_SIZE; i++) {
                    for (int j = 0; j < BOARD_SIZE; j++) {
                        if (isValidMove(board[row][col].type, row, col, i, j, color, false) &&
                            !isMoveLeavesKingInCheck(row, col, i, j, color)) {
                            return false; // Player has at least one valid move
                        }
                    }
                }
            }
        }
    }

    return true; // Stalemate
}

// Function to handle the player's turn
void playerTurn(PieceColor color, bool& enPassantAllowed) {
    while (true) {
        string move;
        cout << (color == PieceColor::WHITE ? "White's move: " : "Black's move: ");
        cin >> move;

        // Check if the move is in algebraic notation
        if (move.size() != 4) {
            cout << "Invalid move format. Use algebraic notation (e.g., e2e4)." << endl;
            continue;
        }

        // Convert algebraic notation to coordinates
        pair<int, int> start = convertAlgebraicToCoordinates(move.substr(0, 2));
        pair<int, int> end = convertAlgebraicToCoordinates(move.substr(2));

        if (!isValidCoordinate(start.first, start.second) || !isValidCoordinate(end.first, end.second)) {
            cout << "Invalid coordinates. Try again." << endl;
            continue;
        }

        if (makeMove(start.first, start.second, end.first, end.second, color, enPassantAllowed)) {
            break;
        }
    }
}

// Function to simulate a basic AI player's turn
void aiTurn(PieceColor color, bool& enPassantAllowed) {
    // Placeholder logic for AI's move (random valid move)
    vector<pair<pair<int, int>, pair<int, int>>> validMoves;
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            if (board[i][j].color == color) {
                for (int x = 0; x < BOARD_SIZE; x++) {
                    for (int y = 0; y < BOARD_SIZE; y++) {
                        if (isValidMove(board[i][j].type, i, j, x, y, color, enPassantAllowed) &&
                            !isMoveLeavesKingInCheck(i, j, x, y, color)) {
                            validMoves.push_back({{i, j}, {x, y}});
                        }
                    }
                }
            }
        }
    }

    if (!validMoves.empty()) {
        int randomIndex = rand() % validMoves.size();
        auto move = validMoves[randomIndex];
        makeMove(move.first.first, move.first.second, move.second.first, move.second.second, color, enPassantAllowed);
    }
}

// Function to play a game of chess
void playChessGame() {
    initializeBoard();
    displayBoard();
    bool gameOver = false;
    bool whiteTurn = true;
    bool enPassantAllowed = false;

    while (!gameOver) {
        if (whiteTurn) {
            playerTurn(PieceColor::WHITE, enPassantAllowed);
        } else {
            aiTurn(PieceColor::BLACK, enPassantAllowed);
        }

        displayBoard();
        if (isCheckmate(whiteTurn ? PieceColor::WHITE : PieceColor::BLACK)) {
            gameOver = true;
            cout << (whiteTurn ? "Black" : "White") << " wins by checkmate!" << endl;
        } else if (isStalemate(whiteTurn ? PieceColor::WHITE : PieceColor::BLACK)) {
            gameOver = true;
            cout << "Stalemate!" << endl;
        }

        whiteTurn = !whiteTurn;
    }
}

int main() {
    playChessGame();
    return 0;
}
