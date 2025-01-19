#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#define ROWS 10
#define COLS 10
#define MINES 15

char board[ROWS][COLS]; // The player's view of the board
char mines[ROWS][COLS]; // The actual mine locations

void initializeBoards() {
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            board[i][j] = '-';
            mines[i][j] = '0';
        }
    }
}

void placeMines() {
    srand(time(NULL));
    int count = 0;

    while (count < MINES) {
        int r = rand() % ROWS;
        int c = rand() % COLS;

        if (mines[r][c] != 'M') {
            mines[r][c] = 'M';
            count++;

            // Increment adjacent cells
            for (int i = -1; i <= 1; i++) {
                for (int j = -1; j <= 1; j++) {
                    int nr = r + i;
                    int nc = c + j;
                    if (nr >= 0 && nr < ROWS && nc >= 0 && nc < COLS && mines[nr][nc] != 'M') {
                        mines[nr][nc]++;
                    }
                }
            }
        }
    }
}

void printBoard(char b[ROWS][COLS]) {
    printf("  ");
    for (int i = 0; i < COLS; i++) {
        printf("%d ", i);
    }
    printf("\n");

    for (int i = 0; i < ROWS; i++) {
        printf("%d ", i);
        for (int j = 0; j < COLS; j++) {
            printf("%c ", b[i][j]);
        }
        printf("\n");
    }
}

bool isValidMove(int row, int col) {
    return row >= 0 && row < ROWS && col >= 0 && col < COLS && board[row][col] == '-';
}

bool revealCell(int row, int col) {
    if (!isValidMove(row, col)) {
        return false;
    }

    board[row][col] = mines[row][col];

    if (mines[row][col] == 'M') {
        return true; // Hit a mine
    }

    // If the cell is empty, reveal surrounding cells
    if (mines[row][col] == '0') {
        for (int i = -1; i <= 1; i++) {
            for (int j = -1; j <= 1; j++) {
                int nr = row + i;
                int nc = col + j;
                if (isValidMove(nr, nc)) {
                    revealCell(nr, nc);
                }
            }
        }
    }

    return false;
}

bool checkWin() {
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            if (board[i][j] == '-' && mines[i][j] != 'M') {
                return false;
            }
        }
    }
    return true;
}

int main() {
    initializeBoards();
    placeMines();

    bool gameOver = false;
    while (!gameOver) {
        printf("\nCurrent Board:\n");
        printBoard(board);

        int row, col;
        printf("Enter row and column to reveal (e.g., 2 3): ");
        scanf("%d %d", &row, &col);

        if (!isValidMove(row, col)) {
            printf("Invalid move! Try again.\n");
            continue;
        }

        gameOver = revealCell(row, col);

        if (gameOver) {
            printf("\nBOOM! You hit a mine. Game Over!\n");
            printBoard(mines);
        } else if (checkWin()) {
            printf("\nCongratulations! You cleared the board!\n");
            printBoard(mines);
            break;
        }
    }

    return 0;
}