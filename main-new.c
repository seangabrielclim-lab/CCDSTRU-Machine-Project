#include <stdio.h>

#define SIZE 3

/* ===================== PRINT BOARD ===================== */
void printBoard(char board[SIZE][SIZE])
{
    int i, j;

    printf("\n==========================\n");
    printf("||        1 2 3         ||\n");
    for (i = 0; i < SIZE; i++)
    {
        printf("||      %d ", i+1);
        for (j = 0; j < SIZE; j++)
        {
            printf("%c ", board[i][j]);
        }
        printf("        ||\n");
    }
    printf("==========================\n");
}

/* ===================== UTILITY ===================== */
int inBounds(int r, int c)
{
    int result;

    if (r >= 0 && r < SIZE && c >= 0 && c < SIZE)
        result = 1;
    else
        result = 0;

    return result;
}

int checkIfFree(char board[SIZE][SIZE], int r, int c)
{
    int result;

    if (inBounds(r, c) && board[r][c] == '.')
        result = 1;
    else
        result = 0;

    return result;
}

int countFree(char board[SIZE][SIZE])
{
    int i, j, count = 0;

    for (i = 0; i < SIZE; i++)
    {
        for (j = 0; j < SIZE; j++)
        {
            if (board[i][j] == '.')
                count++;
        }
    }

    return count;
}

int countOwned(char board[SIZE][SIZE], char player)
{
    int i, j, count = 0;

    for (i = 0; i < SIZE; i++)
    {
        for (j = 0; j < SIZE; j++)
        {
            if (board[i][j] == player)
                count++;
        }
    }

    return count;
}

/* ===================== REMOVE ===================== */
void removePos(char board[SIZE][SIZE],
               int S[SIZE][SIZE],
               int T[SIZE][SIZE],
               int r, int c)
{
    if (inBounds(r, c))
    {
        board[r][c] = '.';
        S[r][c] = 0;
        T[r][c] = 0;
    }
}

/* ===================== FORWARD DECL ===================== */
void expand(char board[SIZE][SIZE],
            int S[SIZE][SIZE],
            int T[SIZE][SIZE],
            int r, int c, char player, int *found);

/* ===================== REPLACE ===================== */
void replacePos(char board[SIZE][SIZE],
                int S[SIZE][SIZE],
                int T[SIZE][SIZE],
                int r, int c, char player, int *found)
{
    char opponent;
    int valid;

    if (player == 'R')
        opponent = 'B';
    else
        opponent = 'R';

    *found = 0;
    valid = inBounds(r, c);

    if (valid)
    {
        if (board[r][c] == opponent)
        {
            board[r][c] = player;
            *found = 1;
        }
        else if (board[r][c] == player)
        {
            *found = 1;
        }
        else
        {
            board[r][c] = player;
        }

        if (*found && S[r][c] == 0)
        {
            S[r][c] = 1;
            *found = 0;
        }
        else if (*found && S[r][c] == 1 && T[r][c] == 0)
        {
            T[r][c] = 1;
            expand(board, S, T, r, c, player, found);
        }
    }
}

/* ===================== EXPAND ===================== */
void expand(char board[SIZE][SIZE],
            int S[SIZE][SIZE],
            int T[SIZE][SIZE],
            int r, int c, char player, int *found)
{
    int up, down, left, right;

    up = r - 1;
    down = r + 1;
    left = c - 1;
    right = c + 1;

    removePos(board, S, T, r, c);

    if (player == 'R')
        replacePos(board, S, T, up, c, player, found);
    else
        replacePos(board, S, T, down, c, player, found);

    replacePos(board, S, T, r, left, player, found);
    replacePos(board, S, T, r, right, player, found);
}

/* ===================== UPDATE ===================== */
void update(char board[SIZE][SIZE],
            int S[SIZE][SIZE],
            int T[SIZE][SIZE],
            int r, int c, char player, int *good)
{
    int found;
    int condition;

    *good = 0;

    if (S[r][c] == 0)
    {
        S[r][c] = 1;
        *good = 1;
    }

    condition = (!(*good) && S[r][c] == 1 && T[r][c] == 0);

    if (condition)
    {
        T[r][c] = 1;
        found = 0;
        expand(board, S, T, r, c, player, &found);
    }
}

/* ===================== INPUT ===================== */
void getMove(int *r, int *c)
{
    printf("Enter row: ");
    scanf("%d", r);
    (*r)--;

    printf("Enter column: ");
    scanf("%d", c);
    (*c)--;
}

/* ===================== PLAYER TURN ===================== */
void playerTurn(char board[SIZE][SIZE],
                int S[SIZE][SIZE],
                int T[SIZE][SIZE],
                int *val,
                int *start,
                int *go)
{
    int r, c;
    int done;
    int good;
    char player;

    done = 0;
    player = (*go) ? 'R' : 'B';

    printf("\n~~~ %c'S TURN ~~~\n", player);

    while (!done)
    {
        getMove(&r, &c);

        if (!inBounds(r, c))
        {
            printf("\n!! Invalid coordinates. Try again.\n");
        }
        else
        {
            good = 0;

            if (*start)
            {
                if (checkIfFree(board, r, c))
                {
                    board[r][c] = player;
                    S[r][c] = 1;
                    good = 1;

                    if (countOwned(board, 'R') == 1 &&
                        countOwned(board, 'B') == 1)
                    {
                        *start = 0;
                        printf("\nStart phase complete.\n");

                    }
                }
                else
                {
                    printf("\n!! Position not free. Try again.\n");
                }
            }
            else
            {
                if ((*go && board[r][c] == 'R') ||
                    (!*go && board[r][c] == 'B'))
                {
                    update(board, S, T, r, c, player, &good);
                }
                else
                {
                    printf("\n!! You can only select your own position.\n");
                }
            }

            if (good)
            {
                *go = !(*go);
                (*val)++;
                done = 1;
            }
        }
    }
}

/* ===================== GAME OVER ===================== */
int gameOver(char board[SIZE][SIZE], int val, int start)
{
    int free, r, b;
    int over;

    free = countFree(board);
    r = countOwned(board, 'R');
    b = countOwned(board, 'B');

    over = 0;

    if (free == 3)
        over = 1;

    if (val >= 20)
        over = 1;

    if (!start && ((r > 0 && b == 0) || (b > 0 && r == 0)))
        over = 1;

    return over;
}

/* ===================== RESULT ===================== */
void printResult(char board[SIZE][SIZE])
{
    int r, b;

    r = countOwned(board, 'R');
    b = countOwned(board, 'B');

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
    char board[SIZE][SIZE] = {
        {'.', '.', '.'},
        {'.', '.', '.'},
        {'.', '.', '.'}};

    int S[SIZE][SIZE] = {0};
    int T[SIZE][SIZE] = {0};

    int val, start, go;

    val = 0;
    start = 1;
    go = 1;  /* 1 = Red, 0 = Blue */

    printf("=============================================================\n");
    printf("                         START GAME                          \n");
    printf("=============================================================\n");

    printf("\nLegend:\n");
    printf("R = Red\n");
    printf("B = Blue\n");
    printf(". = Free\n");

    printf("\nRed goes first.\n");
    
    printBoard(board);

    while (!gameOver(board, val, start))
    {
        printf("\nMove Counter: %d\n", val);
        printf("Phase: %s\n", start ? "START" : "NORMAL");

        playerTurn(board, S, T, &val, &start, &go);
        printBoard(board);
    }

    printf("\n=============================================================\n");
    printf("                           END GAME                          \n");
    printf("=============================================================\n");

    printResult(board);

    return 0;
}
