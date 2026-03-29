#include <stdio.h>

#define SIZE 3

/* ===================== PRINT BOARD ===================== */
void printBoard(char board[SIZE][SIZE])
{
    int i, j;

    printf("\n==========================\n");
    printf("||                      ||\n");
    printf("||        0 1 2         ||\n");
    for (i = 0; i < SIZE; i++)
    {
        printf("||      %d ", i);
        for (j = 0; j < SIZE; j++)
        {
            printf("%c ", board[i][j]);
        }
        printf("        ||\n");
    }
    printf("||                      ||\n");
    printf("==========================\n");
}

/* ===================== UTILITY ===================== */

int inBounds(int r, int c)
{
    return (r >= 0 && r < 3 && c >= 0 && c < 3);
}

int checkIfFree(char board[3][3], int row, int column)
{
    int result = 0;

    if (inBounds(row, column))
    {
        if (board[row][column] == '.')
            result = 1;
    }

    return result;
}

int countFree(char board[3][3])
{
    int i, j, count = 0;

    for (i = 0; i < 3; i++)
        for (j = 0; j < 3; j++)
            if (board[i][j] == '.')
                count++;

    return count;
}

int countOwned(char board[3][3], char player)
{
    int i, j, count = 0;

    for (i = 0; i < 3; i++)
        for (j = 0; j < 3; j++)
            if (board[i][j] == player)
                count++;

    return count;
}

/* ===================== REMOVE ===================== */

void removePos(char board[3][3], int r, int c)
{
    if (inBounds(r, c))
        board[r][c] = '.';
}

/* ===================== REPLACE ===================== */

void replacePos(char board[3][3], int r, int c, char player)
{
	char opponent;
	
    if(player == 'R')
    {
        opponent = 'B';
    }
    else
    {
        opponent = 'R';
    }
    
    if(inBounds(r,c))
    {
	    if (board[r][c] == opponent)
	        board[r][c] = player;
	
	    else if (board[r][c] == '.')
	        board[r][c] = player;
	}
}

/* ===================== EXPAND ===================== */

void expand(char board[3][3], int r, int c, char player)
{
    int up = r - 1;
    int down = r + 1;
    int left = c - 1;
    int right = c + 1;

    removePos(board, r, c);

    if (player == 'R')
        replacePos(board, up, c, player);

    else
        replacePos(board, down, c, player);

    replacePos(board, r, left, player);
    replacePos(board, r, right, player);
}

/* ===================== UPDATE ===================== */

void update(char board[3][3], int r, int c, char player, int expanded[3][3])
{
    if (!expanded[r][c])
    {
        expanded[r][c] = 1;
        expand(board, r, c, player);
    }
}

/* ===================== MOVE INPUT ===================== */

void getMove(int *row, int *col)
{
    printf("Enter row: ");
    scanf("%d", row);

    printf("Enter column: ");
    scanf("%d", col);
}

/* ===================== PLAYER TURN ===================== */

void playerTurn(
    char board[3][3],
    int *moveCounter,
    int *startPhase,
    int *go,
    int expanded[3][3])
{
    int row, col;
    char player = (*go) ? 'R' : 'B';
    int validMove = 0;

    printf("\n~~~ %c'S TURN ~~~\n", player);

    while (!validMove)
    {
        getMove(&row, &col);

        if (!inBounds(row, col))
        {
            printf("\n!! Invalid coordinates. Try again.\n");
        }
        else
        {
            if (*startPhase) 													/*START PHASE*/
            {
                if (checkIfFree(board, row, col))
                {
                    board[row][col] = player;
                    (*moveCounter)++;

                    printf("\nValid move.\n");
                    printBoard(board);

                    if (countOwned(board, 'R') == 1 &&
                        countOwned(board, 'B') == 1)
                    {
                        *startPhase = 0;
                        printf("\nStart phase complete.\n");
                    }

                    *go = !(*go);
                    validMove = 1;
                }
                else
                {
                    printf("\n!! Position not free. Try again.\n");
                }
            }
            else																/*NORMAL PHASE*/
            {
                if ((*go && board[row][col] != 'R') ||
                    (!(*go) && board[row][col] != 'B'))
                {
                    printf("\n!! You can only select your own position.\n");
                }
                else
                {
                    update(board, row, col, player, expanded);
                    (*moveCounter)++;

                    printBoard(board);

                    *go = !(*go);
                    validMove = 1;
                }
            }
        }
    }
}

/* ===================== GAME OVER ===================== */

int gameOver(char board[3][3], int moveCounter, int startPhase)
{
    int free = countFree(board);
    int r = countOwned(board, 'R');
    int b = countOwned(board, 'B');
    int over = 0;

    if (free == 3)
        over = 1;

    if (moveCounter >= 20)
        over = 1;

    if (!startPhase && ((r > 0 && b == 0) || (b > 0 && r == 0)))
        over = 1;

    return over;
}

/* ===================== RESULT ===================== */

void printResult(char board[3][3])
{
    int r = countOwned(board, 'R');
    int b = countOwned(board, 'B');

    if (r > b)
        printf("\nR wins\n");

    else if (b > r)
        printf("\nB wins\n");

    else
        printf("\nDraw\n");
}

/* ===================== MAIN ===================== */

int main()
{
    char board[3][3] = {
        {'.', '.', '.'},
        {'.', '.', '.'},
        {'.', '.', '.'}};

    int expanded[3][3] = {0};

    int moveCounter = 0;
    int startPhase = 1;
    int go = 1; /* 1 = Red, 0 = Blue */

    printf("=============================================================\n");
    printf("                         START GAME                          \n");
    printf("=============================================================\n");

    printf("\nLegend:\n");
    printf("R = Red\n");
    printf("B = Blue\n");
    printf(". = Free\n");

    printf("\nRed goes first.\n");

    printBoard(board);

    while (!gameOver(board, moveCounter, startPhase))
    {
        printf("\nMove Counter: %d\n", moveCounter);

        if (startPhase)
            printf("Game Phase: START\n");
        else
            printf("Game Phase: NORMAL\n");

        playerTurn(board, &moveCounter, &startPhase, &go, expanded);
    }

    printf("\n=============================================================\n");
    printf("                           END GAME                          \n");
    printf("=============================================================\n");

    printResult(board);

    return 0;
}
