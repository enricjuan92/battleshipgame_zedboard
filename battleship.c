#include "battleship.h"
#include "system_lib.h"
#include "bns.h"

int total_hits = 0;

void attack2target(msg_attack attack, coordinate *target)
{
    target->row    = attack[0];
    target->column = attack[1];
}

int getRandomNumber(int lowest, int highest)
{
    if (lowest == 0)
        return rand() % ++highest;
    
    if (lowest > 0)
        return rand() % ++highest + lowest;
    
    return 0;
}

coordinate generatePosition(int direction, int length)
{
    coordinate position;
    
    if (direction == HORIZONTAL) {
        position.row = getRandomNumber(0, ROWS);
        position.column = getRandomNumber(0, COLS - length);
        
    } else { /* VERTICAL */
        position.row = getRandomNumber(0, ROWS - length);
        position.column = getRandomNumber(0, COLS);
    }
    
    return position;
}

int isValidLocation(gameboard *gameBoard, coordinate position, int direction, int length)
{
    int i = length - 1;
    int isValid = TRUE;
    
    for (i = 0; isValid && i < length; i++)
    {
        if (direction == HORIZONTAL) {
            if ((*gameBoard)[position.row][position.column + i].symbol != WATER && (position.column + i) < COLS)
                isValid = FALSE;
            
        } else { /* VERTICAL */
            if ((*gameBoard)[position.row][position.column + i].symbol != WATER && (position.row + i) < ROWS)
                isValid = FALSE;
        }
    }
    
    return isValid;
}

void putShipOnGameBoard(gameboard *gameBoard, watercraft ship, coordinate position, int direction)
{
    int i = ship.length - 1;
    
    for (i = 0; i < ship.length; i++)
    {
        if (direction == HORIZONTAL){
            (*gameBoard)[position.row][position.column + i].symbol = ship.symbol;
        }else{
            /* VERTICAL */
            (*gameBoard)[position.row + i][position.column].symbol = ship.symbol;
        }
    }
}

void randomlyPlaceShipsOnGameBoard(gameboard *gameBoard, watercraft ship[])
{
    coordinate position;
    int direction = -1;
    int i = 0;
    
    for (i = 0; i < NUM_OF_SHIPS; i++) {
        while (TRUE) {
            direction = getRandomNumber(0, 1); /* 0 -> horizontal, 1 -> vertical */
            position = generatePosition(direction, ship[i].length);
            
            if (isValidLocation(gameBoard, position, direction, ship[i].length))
                break;
        }
        
        putShipOnGameBoard(gameBoard, ship[i], position, direction);
    }
}

void initializeGameBoard(gameboard *gameBoard)
{
    int i = 0, j = 0;
    
    for (i = 0; i < ROWS; i++)
        for (j = 0; j < COLS; j++)
        {
            /* To Be Defined */
            (*gameBoard)[i][j].symbol = WATER;
            (*gameBoard)[i][j].position.row = i;
            (*gameBoard)[i][j].position.column = j;
        }
}

void generate_board(fleet ships, gameboard *myBoard, gameboard *enemyBoard)
{
    /* Boards Initialization */
	initializeGameBoard(myBoard);
	initializeGameBoard(enemyBoard);
	randomlyPlaceShipsOnGameBoard(myBoard, ships);
    
	xil_printf("> Game board has been correctly generated.\n");
}

void updateBoard(gameboard *gameBoard, msg_attack attack)
{
    coordinate target;
    int sym = (int) attack[2];
    attack2target(attack, &target);
    
	if (sym == 0xFF) {
		switch ((*gameBoard)[target.row][target.column].symbol)
		{
			/* miss */
			case WATER:
				(*gameBoard)[target.row][target.column].symbol = MISS;
			break;

			/* hit */
			case CARRIER:
			case BATTLESHIP:
			case CRUISER:
			case SUBMARINE:
			case DESTROYER:
				(*gameBoard)[target.row][target.column].symbol = HIT;
			break;

			case HIT:
			case MISS:
			default:
			break;
		}
	}
    else{
    	switch (sym){
			/* miss */
    		case HIT:
    			(*gameBoard)[target.row][target.column].symbol = HIT;
    			break;
    		case MISS:
    			(*gameBoard)[target.row][target.column].symbol = MISS;
    			break;
			default:
				break;
		}
    }
}

void showBoard(gameboard gameBoard, int showPegs)
{
	int i = 0, j = 0;

	xil_printf("  0 1 2 3 4 5 6 7\n");

	for (i = 0; i < ROWS; i++) {
		xil_printf("%d ", i);

		for (j = 0; j < COLS; j++) {
			if (showPegs == TRUE)
				xil_printf("%c ", gameBoard[i][j].symbol);
			else {
				switch (gameBoard[i][j].symbol) {
				case HIT:
					xil_printf("%c ", HIT);
					break;
				case MISS:
					xil_printf("%c ", MISS);
					break;
				case WATER:
				default:
					xil_printf("%c ", WATER);
					break;
				}
			}
		}

		putchar('\n');
	}
}

void show_myBoard(gameboard gameBoard)
{
	int i = 0, j = 0;

	xil_printf("  0 1 2 3 4 5 6 7\n");

	for (i = 0; i < ROWS; i++)
    {
		xil_printf("%d ", i);

		for (j = 0; j < COLS; j++)
        {
			switch (gameBoard[i][j].symbol)
            {
			case HIT:
				xil_printf("%c ", HIT);
				break;
			case MISS:
				xil_printf("%c ", MISS);
				break;
			case WATER:
				xil_printf("%c ", WATER);
				break;
			case CARRIER:
				xil_printf("%c ", CARRIER);
				break;
			case BATTLESHIP:
				xil_printf("%c ", BATTLESHIP);
				break;
			case CRUISER:
				xil_printf("%c ", CRUISER);
				break;
			case SUBMARINE:
				xil_printf("%c ", SUBMARINE);
				break;
			case DESTROYER:
				xil_printf("%c ", DESTROYER);
				break;
			default:
				xil_printf("%c ", WATER);
				break;
			}
		}
		putchar('\n');
	}

	xil_printf("\n");
}


void check_sunken_ship(fleet *ships, char type)
{
    int index = 0;
    
    switch (type) {
        case CARRIER:
            index = 0;
            break;
        case BATTLESHIP:
            index = 1;
            break;
        case CRUISER:
            index = 2;
            break;
        case SUBMARINE:
            index = 3;
            break;
        case DESTROYER:
            index = 4;
            break;
    }
    
    (*ships)[index].hits++;
    
    if((*ships)[index].length == (*ships)[index].hits)
    {
    	xil_printf("\nThe startship %s has been sunken!!!!.\n\n", (*ships)[index].name);
    	(*ships)[index].isSunken = TRUE;
    }
}

int is_winner(fleet ships)
{
    if(ships[0].isSunken == TRUE)
    {
        return 1;
    }else
        return 0;
}

void check_attack(gameboard *gameBoard, fleet *ships, msg_attack *packet)
{
	/* Recorrer tablero y mirar si en la posici—n en la que estoy coincide con las del ataque */
    /* mirar si esa posici—n en la que han atacado, hay barcos y si los hay si es tocado o tocado y hundido */
    /* comprobar si estan todos los barcos hundidos para ver si hay ganador */
    int i = 0, j = 0;
    
    for (i = 0; i < ROWS; i++)
    {
        if(i == (*packet)[0])
        {
            for (j = 0; j < COLS; j++)
            {
                if(j == (*packet)[1])
                {
                    /* Check position where enemy has been attack */
                    switch((*gameBoard)[i][j].symbol)
                    {
                        case HIT:
                            xil_printf("This position has been attack before.\n");
                            (*packet)[2] = HIT;

                            break;
                            
                        case WATER:
                            xil_printf("Ups, it's so embarrassing but you hit on water.\n");
                            (*packet)[2] = MISS;
                            
                            break;
                            
                        case CARRIER:
                            check_sunken_ship(ships, CARRIER);
                            (*packet)[2] = HIT;
                            xil_printf("You have hit a carrier.\n");

                            break;
                            
                        case BATTLESHIP:
                            check_sunken_ship(ships, BATTLESHIP);
                            (*packet)[2] = HIT;
                            xil_printf("You have hit a battleship.\n");
                            
                            break;
                            
                        case CRUISER:
                            check_sunken_ship(ships, CRUISER);
                            (*packet)[2] = HIT;
                            xil_printf("You have hit a cruiser.\n");
                            
                            break;
                            
                        case SUBMARINE:
                            check_sunken_ship(ships, SUBMARINE);
                            (*packet)[2] = HIT;
                            xil_printf("You have hit a submarine.\n");
                            
                            break;
                            
                        case DESTROYER:
                            check_sunken_ship(ships, DESTROYER);
                            (*packet)[2] = HIT;
                            xil_printf("You have hit a destroyer.\n");

                            break;
                        default:
                        	xil_printf("Symbol unrecognized");
                        	break;
                    }
                }
            }
        }
    }
}
