#ifndef NAVAL_H                      // INCLUDE GUARD 
#define NAVAL_H

#define LINES 10                     // Used for 10x10 board 
#define COLUMNS 10                   // Used for 10x10 board
#define REPEAT -1                    // On init, if there's a ship on location, pick different location
#define MAX_NUM_SHIPS 3              // Maximum number of ships (boat, submarine and frigate)

typedef struct Player {              // Definition of the Player structure
    int x, y;                        // Stores player's coordinates
    char board[LINES][COLUMNS];      // Visible board that draws ships with #
    char boardNum[LINES][COLUMNS];   // Invisible board that represents a ship with number
    unsigned char hits;              // Increments every time a player finds a ship
    unsigned char water;             // Increments every time a player does not find a ship
    char status[8];                  // Stores "missed" or "hit" for HUD
} Player;

void mainMenu(void);                 
void initializeBoard(struct Player *computer, struct Player *player);
void showBoard(char computer[][COLUMNS], char player[][COLUMNS]);
void drawShips(char computer[][COLUMNS], char player[][COLUMNS]);
void submarine(char boardNum[][COLUMNS]);
void boat(char boardNum[][COLUMNS]);
void frigate(char boardNum[][COLUMNS]);
int  shipExists(char board[][COLUMNS], char x, char y);
void displayShip(char board[][COLUMNS], char boardNum[][COLUMNS], char x, char y);
void displayPlayerOnInit(struct Player *player);
void deleteShip(char board[][COLUMNS], char boardNum[][COLUMNS], char x, char y);
void playerCoordinates(int *x, int *y);
void computerCoordinates(int *x, int *y);

#endif // NAVAL_H
