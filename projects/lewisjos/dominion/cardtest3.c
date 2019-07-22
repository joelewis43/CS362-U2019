

#include <string.h>
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

#include "dominion.h"
#include "dominion_helpers.h"
#include "rngs.h"

#define MAX_HAND 500
#define MAX_DECK 500

#define MAX_PLAYERS 4



/* -----------------------------------------------------------------------
 * Custom assertion to prevent code from exiting prematurely
 * 
 * Returns 1 when false to allow user to tally the number of failed tests
 * -----------------------------------------------------------------------
 */
int assertCustom(int x, char* msg) {
    if (!x) {
        printf("\tERROR: %s\n", msg);
        return 1;
    }
    return 0;
}

int cardCount(struct gameState *G, int p, int card) {
    int count, i;

    for (i=0; i<G->handCount[p]; i++) {
        if (G->hand[p][i] == card) {
            count++;
        }
    }

    return count;
}


// End turn

/*
 * Assertions to make
 * 
 * Players hand is empty
 * whose turn incremented
 * new player draws their hand
 * coins updates
*/
int main() {

    int i;
    int fails = 0;
    struct gameState *G;

    int tempNumPlayers = 3;
    int tempDeck[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    int tempSeed = 123;

    int currentPlayer, nextPlayer, currentHandCount, currentDiscardCount;

    // create game struct
    G = malloc(sizeof (struct gameState));
    initializeGame(tempNumPlayers, tempDeck, tempSeed, G);

    printf("\n\n\nTesting the endTurn function!\n\n");

    for (i=0; i<30; i++) {                  // run test 30 times

        currentPlayer = G->whoseTurn;
        currentHandCount = G->handCount[currentPlayer];
        currentDiscardCount = G->discardCount[currentPlayer];
        
        if (currentPlayer == G->numPlayers-1)
            nextPlayer = 0;
        else
            nextPlayer = currentPlayer + 1;

        endTurn(G);

        fails += assertCustom(G->whoseTurn == nextPlayer, "The next player was not correctly set");
        fails += assertCustom(G->handCount[currentPlayer] == 0, "Players hand count not cleared");
        fails += assertCustom(G->discardCount[currentPlayer] == currentDiscardCount+currentHandCount, "Discard count not properly incremented");
        fails += assertCustom(G->handCount[nextPlayer] == 5, "Next players hand not set properly");

    }

    

    if (fails == 0) {
        printf("All tests passed!\n");
    }
    else {
        printf("ERROR: %d tests failed!\n", fails);
    }

    

    return 0;
}
