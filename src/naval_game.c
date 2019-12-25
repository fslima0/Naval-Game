#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#include "naval.h"

/*  The objective of this game is to discover where the ships are hidden.
 *  All 3 (three) ships (boat, submarine and frigate) are represented by a pattern on the board.
 *  The first player to discover all ships wins the game.
 */

int main(void)
{
    Player player;                                                // Declare struct variables for players 
    Player computer;
    
    memset(&player, 0, sizeof(player));                           // Initializes player structure
    memset(&computer, 0, sizeof(computer));                       // Initializes computer structure

    srand(time(NULL));                                            // Needed to generate random X and Y values

    mainMenu();                                                   // Displays a simple menu

    initializeBoard(&computer, &player);                          // Initializes board with '.' and generates ships
    showBoard(computer.board, player.board);                      // Displays 10x10 board for Player and Computer

    do {
        playerCoordinates(&player.x, &player.y);                  // Asks for Player's coordinates.

        if (shipExists(computer.boardNum, player.x, player.y)) {  // Checks if Player discovers a ship on Computer's board
            displayShip(computer.board, computer.boardNum, player.x, player.y); // If so, display the ship
            strcpy(player.status, "hit");                         // Used for Player's status bar 
            player.hits += 1;                                     // Increments this variable; ship found.
        }
        else {                                                    // Ship not found 
            strcpy(player.status, "missed");                      // Used for Player's status bar
            player.water += 1;                                    // Increment this variable; ship not found
        }

        computerCoordinates(&computer.x, &computer.y);            // Asks for Computer's coordinates

        if (shipExists(player.boardNum, computer.x, computer.y)) {// Checks if Computer discovers a ship on Player's board
            deleteShip(player.board, player.boardNum, computer.x, computer.y); // If so, delete the ship
            strcpy(computer.status, "hit");                       // Used for Computer's status bar
            computer.hits += 1;                                   // Increments this variable; ship found.
        }                                                       
        else {                                                    // Ship not found
            strcpy(computer.status, "missed");                    // Used for Computer's status bar
            computer.water += 1;                                  // Increment this variable; ship not found
        }
        
        showBoard(computer.board, player.board);

        printf("Player-> %s target: %c %d water:%d\n", player.status, (player.x + 'A'), player.y, player.water);
        printf("Computer-> %s target: %c %d water:%d\n\n", computer.status, (computer.x + 'A'), computer.y, computer.water);
    
    } while (player.hits < MAX_NUM_SHIPS && computer.hits < MAX_NUM_SHIPS); // Continues until a player discovers all 3 ships

    if (player.hits > computer.hits)
        printf("Player has won the game!!!\n");
    else 
        printf("Computer has won the game!!!\n\n");

    return EXIT_SUCCESS;
}

void mainMenu(void)
{
    int choice;

    do {
        printf("    _   _                  _    ____                     \n"); 
        printf("   | \\ | | __ ___   ____ _| |  / ___| __ _ _ __ ___   ___\n"); 
        printf("   |  \\| |/ _` \\ \\ / / _` | | | |  _ / _` | '_ ` _ \\ / _ \\\n");
        printf("   | |\\  | (_| |\\ V / (_| | | | |_| | (_| | | | | | |  __/\n");
        printf("   |_| \\_|\\__,_| \\_/ \\__,_|_|  \\____|\\__,_|_| |_| |_|\\___|\n");
        
        printf("\n   --------------------------------\n");
        printf("   |  1. NEW GAME                 |\n");
        printf("   |  2. MULTIPLAYER              |\n");
        printf("   |  3. EXIT                     |\n");
        printf("   --------------------------------\n\n");
        printf("   Enter your choice: ");
        
        scanf(" %d", &choice);
        
        switch (choice) {
            case 1: break;
            case 2: exit(0);
            case 3: printf("   Exiting the game...\n");
                    exit(0);
            default: printf("   Invalid choice!\n");
        }
        
    } while (choice != 1);
}

void initializeBoard(struct Player *computer, struct Player *player)
{
    int i, j;

    for (i = 0; i < LINES; i++) {  
        for (j = 0; j < COLUMNS; j++) { 
            computer->board[i][j] = player->board[i][j] = '.';
            computer->boardNum[i][j] = player->boardNum[i][j] = '.';
        }
    }

    drawShips(computer->boardNum, player->boardNum);
    displayPlayerOnInit(player); // By default, Player's ships are shown on the screen
}

void showBoard(char computer[][COLUMNS], char player[][COLUMNS])
{
    char i, j;

    system("clear");
    printf("  Computer\t\tPlayer\n");
    printf("  0 1 2 3 4 5 6 7 8 9\t0 1 2 3 4 5 6 7 8 9\n");

    for (i = 0, j = 'A'; i < LINES; i++) {
        printf("%c %c %c %c %c %c %c %c %c %c %c %c %c %c %c %c %c %c %c %c %c %c\n",
               j, computer[i][0], computer[i][1], computer[i][2], computer[i][3], computer[i][4],
               computer[i][5], computer[i][6],computer[i][7],computer[i][8], computer[i][9],
               j, player[i][0], player[i][1], player[i][2], player[i][3],player[i][4], player[i][5],
               player[i][6],player[i][7],player[i][8], player[i][9]);
 
               j += 1;
    }
    putchar('\n');
}

void drawShips(char computer[][COLUMNS], char player[][COLUMNS])
{
    submarine(player); boat(player); frigate(player);
    submarine(computer); boat(computer); frigate(computer);
}

void submarine(char boardNum[][COLUMNS])
{
    int x, y, i;

    do {
        x = rand() % 10;
        y = rand() % 10;

        if ((y + 4) > 9 || x == 0) {
            x = REPEAT;
            continue;
        }

        for (i = 0; i < 5; i++)  
            if (shipExists(boardNum, x, y + i)) { 
                x = REPEAT;
                break;
            }

        if (i == 5 && shipExists(boardNum, x - 1, y + 4))  
            x = REPEAT;

    } while (x == REPEAT);

    for (i = 0; i < 5; i++) { 
        boardNum[x][y + i] = '2';
    }

    boardNum[x - 1][y + 4] = '2';
}

void boat(char boardNum[][COLUMNS])
{
    int x, y, i;

    do {
        x = rand() % 10;
        y = rand() % 10;

        if (x == 9 || (y + 2) > 9) {      
            x = REPEAT;                   
            continue;
        }

        for (i = 0; i < 3; i++)                     
            if (shipExists(boardNum, x, y + i)) {
                x = REPEAT;                        
                break;
            }

        if (i == 3 && shipExists(boardNum, x + 1, y)) 
            x = REPEAT;                               

    } while (x == REPEAT);

    for (i = 0; i < 3; i++) {
        boardNum[x][y + i] = '1';
    }

    boardNum[x + 1][y] = '1';
}

void frigate(char boardNum[][COLUMNS])
{
    int x, y, i, j;

    do {
        x = rand() % 10;
        y = rand() % 10;

        if ((y + 4) > 9 || x == 9 || (y - 2) < 0) {
            x = REPEAT;
            continue;
        }

        for (i = 0; i < 5; i++) 
            if (shipExists(boardNum, x, y+i)) {
                x = REPEAT;
                break;
            }

        for (j = 0; i == 5 && j <= 2; j++) 
            if (shipExists(boardNum, x + 1, y - j)) {
                x = REPEAT;
                break;
            }

    } while (x == REPEAT);

    for (i = 0; i < 5; i++) { 
        boardNum[x][y + i] = '3';
    }

    for (j = 0; j <= 2; j++) { 
        boardNum[x + 1][y - j] = '3';
    }
}

int shipExists(char boardNum[][COLUMNS], char x, char y)
{
    if (boardNum[x][y] >= '1' && boardNum[x][y] <= '3') 
        return 1;
    else 
        return 0;
}

void displayShip(char board[][COLUMNS], char boardNum[][COLUMNS], char x, char y)
{
    int i, j;
    char num;

    num = boardNum[x][y];

    for (i = 0; i < LINES; i++)           // Refreshes the Computer's board
        for (j = 0; j < COLUMNS; j++)  
            if (boardNum[i][j] == num) { 
                board[i][j] = '#';        // Display ship on visible board
                boardNum[i][j] = '.';     // Remove ship from invisible board
            }
}

void displayPlayerOnInit(struct Player *player)
{
    int i, j;
   
    for (i = 0; i < LINES; i++) 
        for (j = 0; j < COLUMNS; j++)  
            if (shipExists(player->boardNum, i, j)) { 
                player->board[i][j] = '#';
            }
}

void deleteShip(char board[][COLUMNS], char boardNum[][COLUMNS], char x, char y)
{
    int i, j;
    char num;

    num = boardNum[x][y];

    for (i = 0; i < LINES; i++)        // Refreshes the Player's board   
        for (j = 0; j < COLUMNS; j++) 
            if (boardNum[i][j] == num) {
                board[i][j] = '.';     // Deletes ship from Player's visible board
                boardNum[i][j] = '.';  // Removes ship from invisible board
            }
}

void playerCoordinates(int *x, int *y)
{
     do {
         printf("Enter coordinates: ");
         scanf(" %c %d", (char* )x, y);

         if (*x < 'A' && *x > 'J') 
             printf("Wrong coordinates. Example: B 5\n");
        
         *x = *x - 'A';
         
     } while (*x < -1 || *x > 9 || *y < -1 || *y > 9);
}

void computerCoordinates(int *x, int *y)
{
    *x = rand() % 10;
    *y = rand() % 10;
}
