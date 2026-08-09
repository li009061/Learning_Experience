#include <stdio.h>

#define N 9

int isSafe(int grid[N][N], int row, int col, int num) {
    for (int x = 0; x < N; x++)
        if (grid[row][x] == num || grid[x][col] == num)
            return 0;

    int startRow = row - row % 3;
    int startCol = col - col % 3;
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            if (grid[i + startRow][j + startCol] == num)
                return 0;

    return 1;
}

int solveSudoku(int grid[N][N], int row, int col) {
    if (row == N - 1 && col == N)
        return 1;

    if (col == N) {
        row++;
        col = 0;
    }

    if (grid[row][col] != 0)
        return solveSudoku(grid, row, col + 1);

    for (int num = 1; num <= 9; num++) {
        if (isSafe(grid, row, col, num)) {
            grid[row][col] = num;
            if (solveSudoku(grid, row, col + 1))
                return 1;
            grid[row][col] = 0;
        }
    }
    return 0;
}

void printGrid(int grid[N][N]) {
    for (int r = 0; r < N; r++) {
        for (int c = 0; c < N; c++)
            printf("%d ", grid[r][c]);
        printf("\n");
    }
}

int main() {
    int grid[N][N] = {
        {0,0,3,0,0,6,0,8,0},
        {0,0,0,1,0,0,2,0,6},
        {0,0,0,0,7,0,0,0,4},
        {0,0,9,0,0,8,0,6,0},
        {0,3,0,0,4,0,0,0,1},
        {0,7,0,2,0,0,0,0,0},
        {3,0,0,0,0,5,0,0,0},
        {0,0,5,0,0,0,6,0,0},
        {9,8,0,0,0,0,0,5,0}
    };

    if (solveSudoku(grid, 0, 0))
        printGrid(grid);
    else
        printf("No solution\n");

    return 0;
}