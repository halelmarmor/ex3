#include <stdio.h>

#ifndef ROWS
#define ROWS 6
#endif

#ifndef COLS
#define COLS 7
#endif

#define CONNECT_N 4

/* Tokens */
#define EMPTY '.'
#define TOKEN_P1 'X'
#define TOKEN_P2 'O'

#define HUMAN 1
#define COMPUTER 2

int isColumnFull(char board[][COLS], int col) {
        if (board[0][col] == TOKEN_P1 || board[0][col] == TOKEN_P2) {
            printf ("Column %d is full. Choose another column.\n", col + 1);
            return 1;
        }
    return 0;
}
int isBoardFull(char board[][COLS]) {
    for (int col=0; col < COLS; col++) {
        if (board[0][col] != TOKEN_P1 && board[0][col] != TOKEN_P2) {
            return 0;
        }
    }
    printf ("Board full and no winner. It's a tie!\n");
    return 1;
}
int isInBounds(int col)
{
    if (col > COLS || col < 1) {
        printf ("Invalid column. Choose between 1 and %d.\n", COLS);
        return 0;
    }
    return 1;
}
/* Return index of row where token will land, or -1 if column full */
int getFreeRow(char board[][COLS], int rows, int cols, int col) {
    for (int row = rows - 1; row >= 0; row--) {
        if (board[row][col] == EMPTY) {
            return row;
        }
    }
    return -1;
}

/* Place token in column (0-based). Return row index or -1 if illegal */
int makeMove(char board[][COLS], int col, char token) {
    for (int row = ROWS-1; row >= 0; row--) {
        if (board[row][col] == EMPTY) {
            board[row][col] = token;
            return row;
            }
        }
    return -1;
    }

int checkSequence(char board[][COLS], int rows, int cols, int lastRow, int lastCol, char token, int length) {
    int count, r, c;
    //horizontal check
    count = 0;
    for (c=0; c < cols; c++) {
        if (board[lastRow][c] == token) count++;
        else count = 0;
        if (count >= length) return 1;
    }
    //verticl check
    count = 0;
    for (r=0; r < rows; r++) {
        if (board[r][lastCol] == token) count++;
        else count = 0;
        if (count >= length) return 1;
    }
    //diagonal from left to right check
    count = 0;
    r = lastRow; c = lastCol;
    while (r > 0 && c > 0) {r--; c--;}
    while (r < rows && c < cols) {
       if (board[r][c] == token) count++;
        else count = 0;
        if (count >= length) return 1;
        r++; c++;
    }
    //diagonal from right to left check
    count = 0;
    r = lastRow; c = lastCol;
    while (r > 0 && c < cols -1) {r--; c++;}
    while (r < rows && c >= 0) {
        if (board[r][c] == token) count++;
        else count = 0;
        if (count >= length) return 1;
        r++; c--;
    }
    return 0;
}
int checkVictory (char board[][COLS], int rows, int cols, int lastRow, int lastCol, char token) {
    return checkSequence(board, rows, cols, lastRow, lastCol, token, CONNECT_N);
}
/* Human player: asks repeatedly until a valid non-full column is chosen (0-based) */
int humanChoose(char board[][COLS], int playerNumber) {
    int col;
    while (1) {
        printf ("Player %d (%c) turn.\nEnter column (1-%d): ",
            playerNumber, (playerNumber ==1 ? TOKEN_P1 : TOKEN_P2), COLS);
        if (scanf("%d",&col) != 1) {
            printf("Invalid input. Enter a number.\n");
            while (getchar() != '\n');
            continue;
        }
        if (!isInBounds(col)) continue;
        if (isColumnFull(board, col-1)) continue;

        return col-1;
    }
}

/* Computer*/
int computerChoose(char board[][COLS], int playerNumber, int opponentNumber, char myToken, char oppToken) {
    int row, col;
    //winning move
    for (col = 0; col < COLS; col++) {
        row = getFreeRow(board, ROWS, COLS, col);
        if (row == -1) continue;
        board[row][col] = myToken;
        if (checkVictory(board,ROWS, COLS, row, col, myToken)) {
            board[row][col] = EMPTY;
            return col;
        }
        board[row][col] = EMPTY;
    }
    //blocking the opponent
    for (col = 0; col < COLS; col++) {
        row = getFreeRow(board, ROWS, COLS, col);
        if (row == -1) continue;
        board[row][col] = oppToken;
        if (checkVictory(board, ROWS, COLS, row, col, oppToken)) {
            board[row][col] = EMPTY;
            return col;
        }
        board[row][col] = EMPTY;
    }
    //creating a sequence of three
    for (col = 0; col < COLS; col++) {
        row = getFreeRow(board, ROWS, COLS, col);
        if (row == -1) continue;
        board[row][col] = myToken;
        if (checkSequence(board, ROWS, COLS, row, col, myToken, CONNECT_N-1)) {
            board[row][col] = EMPTY;
            return col;
        }
        board[row][col] = EMPTY;
    }
    //blocking the opponent's sequence of three
    for (col = 0; col < COLS; col++) {
        row = getFreeRow(board, ROWS, COLS, col);
        if (row == -1) continue;
        board[row][col] = oppToken;
        if (checkSequence(board, ROWS, COLS, row, col, oppToken, CONNECT_N-1)) {
            board[row][col] = EMPTY;
            return col;
        }
        board[row][col] = EMPTY;
    }
    //arbitrary ordering rule
    int columnOrder[COLS];
    int index = 0;
    int center = (COLS-1)/2;
    columnOrder[index++] = center;
    for (int shift = 1; index < COLS; shift++) {
        if (center - shift >= 0) columnOrder[index++] = center - shift;
        if (center + shift < COLS && index < COLS) columnOrder[index++] = center + shift;
    }
    for (int i = 0; i < COLS; i++) {
    col = columnOrder[i];
    row = getFreeRow(board, ROWS, COLS, col);
    if (row != -1) return col;
    }
    return -1;
}
void printBoard(char[][COLS], int, int);

void runConnectFour(char board[][COLS], int rows, int cols, int p1Type, int p2Type) {
    int currentPlayer = 1;
    int row, col;
    char token;
    while (1) {
        token = (currentPlayer == 1) ? TOKEN_P1 : TOKEN_P2;
        if ((currentPlayer == 1 && p1Type == HUMAN) || (currentPlayer == 2 && p2Type == HUMAN)) {
           col =  humanChoose(board, currentPlayer);
        } else {
            col = computerChoose(board, currentPlayer,(currentPlayer == 1 ? 2 : 1),
                token, (currentPlayer == 1 ? TOKEN_P2 : TOKEN_P1));
        }
        row = makeMove(board, col, token);
        printBoard(board, rows, cols);
        if (checkVictory(board, rows, cols, row, col, token)) {
            printf("Player %d (%c) wins!\n", currentPlayer, token);
            break;
        }
        if (isBoardFull(board)) {
            break;
        }
        currentPlayer = (currentPlayer == 1) ? 2 : 1;
    }
}

void initBoard(char board[][COLS], int rows, int cols) {
    for (int r = 0; r < rows; r++)
        for (int c = 0; c < cols; c++)
            board[r][c] = EMPTY;
}


int getPlayerType(int);


int main() {
    char board[ROWS][COLS];
    printf("Connect Four (%d rows x %d cols)\n\n", ROWS, COLS);
    int p1Type = getPlayerType(1);
    int p2Type = getPlayerType(2);
    initBoard(board, ROWS, COLS);
    printBoard(board, ROWS, COLS);
    runConnectFour(board, ROWS, COLS, p1Type, p2Type);
    return 0;
}

void printBoard(char board[][COLS], int rows, int cols) {
    printf("\n");
    for (int r = 0; r < rows; r++) {
        printf("|");
        for (int c = 0; c < cols; c++) {
            putchar(board[r][c]);
            printf("|");
        }
        printf("\n");
    }
    for (int c = 1; c <= cols; c++) {
        printf(" %d", c % 10);
    }
    printf("\n\n");
}

int getPlayerType(int playerNumber) {
    char ch;
    while (1) {
        printf("Choose type for player %d: h - human, c - computer: ", playerNumber);
        int n = scanf(" %c", &ch);
        if (n != 1) {
            printf("Input error. Try again.\n");
            while (getchar() != '\n'); // clear input buffer
            continue;
        }
        if (ch == 'h' || ch == 'H') return HUMAN;
        if (ch == 'c' || ch == 'C') return COMPUTER;

        printf("Invalid selection. Enter h or c.\n");
        while (getchar() != '\n'); // clear rest of input
    }
}
