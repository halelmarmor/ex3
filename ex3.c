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
        }
    return 0;
}
int isBoardFull(char board[][COLS], int col) {
    for (col=0; col < COLS; col++) {
        if (board[0][col] != TOKEN_P1 && board[0][col] != TOKEN_P2) {
            return 0;
        }
    }
    printf ("Board full and no winner. It's a tie!\n", col+1);
    return 1;
}
int isInBounds(int col)
{
    if (col > COLS || col < 1) {
        printf ("Invalid column. Choose between 1 and %d.\n", COLS);
    }
    return 0;
}
/* Return index of row where token will land, or -1 if column full */
int getFreeRow(char[][COLS], int, int, int);

/* Place token in column (0-based). Return row index or -1 if illegal */
int makeMove(char[][COLS], int, int, int, char);

int checkVictory(char[][COLS], int, int, int, int, char);

/* Human player: asks repeatedly until a valid non-full column is chosen (0-based) */
int humanChoose(char board[][COLS], int col) {
    do {
        printf("Enter column (1-%d): ", COLS);
        scanf("%d",&col-1);
    }
    while (col);
        isInBounds(col);
        isColumnFull(board[][COLS], col);
    //צריך לוודא שהמשתמש מכניס מספר ולא תו
    printf("Invalid input. Enter a number.\n");
    return 0;
}

/* Computer*/
int computerChoose(char[][COLS], int, int, char, char);

void runConnectFour(char[][COLS], int, int, int, int);

void initBoard(char[][COLS], int, int);

void printBoard(char[][COLS], int, int);

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
