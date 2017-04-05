#ifndef BATTLESHIP_H
#define BATTLESHIP_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "system_lib.h"

/* CONSTANT MACROS */

#define NUM_OF_SHIPS  5

#define HORIZONTAL    0
#define VERTICAL      1

#define WATER         '~'
#define HIT           '*'
#define MISS          'm'

#define CARRIER       'c'
#define BATTLESHIP    'b'
#define CRUISER       'r'
#define SUBMARINE     's'
#define DESTROYER     'd'

#define NORTH 0
#define SOUTH 1
#define WEST  2
#define EAST  3

#define TRUE  1
#define FALSE 0

/* STRUCT TYPES */

typedef struct WaterCraft {
    char   symbol;
    short  length;
    char   *name;
    int    hits;
    int    isSunken;
} watercraft;

typedef struct Stats {
    int        numHits;
    int        numMisses;
    int        totalShots;
    double     hitMissRatio;
} state;

typedef struct Coordinate {
    int        row;
    int        column;
} coordinate;

typedef struct Cell {
    char       symbol;
    coordinate position;
} cell;

typedef cell gameboard[ROWS][COLS];
typedef watercraft fleet[NUM_OF_SHIPS];


/* FUNCTION PROTOTYPES */
int 	     getRandomNumber				(int lowest, int highest);
coordinate   generatePosition               (int direction, int length);
int          isValidLocation                (gameboard *gameBoard, coordinate position, int direction, int length);
void         putShipOnGameBoard             (gameboard *gameBoard, watercraft ship, coordinate position, int direction);
void         randomlyPlaceShipsOnGameBoard  (gameboard *gameBoard, watercraft ship[]);
void         initializeGameBoard            (gameboard *gameBoard);
void 		 generate_board                 (fleet ships, gameboard *myBoard, gameboard *enemyBoard);
void         updateBoard                    (gameboard *gameBoard, msg_attack attack);
void         check_sunken_ship              (fleet *ships, char type);
void         check_attack                   (gameboard *gameBoard, fleet *ships, msg_attack *packet);
void         showBoard                      (gameboard gameBoard, int showPegs);
void         show_myBoard                   (gameboard gameBoard);
void         attack2target                  (msg_attack attack, coordinate *target);
int          is_winner                      (fleet ships);

#endif
