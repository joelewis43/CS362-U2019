/* -----------------------------------------------------------------------
 * Unit Test of the mineEffect function from domine.c
 * 
 * This unit test was developed following the guidelines set in
 * TestUpdateCoins.c
 * -----------------------------------------------------------------------
 */

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


// Draw Card
int main() {

    int i, p;
    int fails = 0;
    struct gameState *G;

    int tempNumPlayers = 3;
    int tempDeck[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    int tempSeed = 123;

    int testDeck[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    
    int retVal;
    int currentDeckCount, currentDiscardCount, currentHandCount;

    // create game struct
    G = malloc(sizeof (struct gameState));
    initializeGame(tempNumPlayers, tempDeck, tempSeed, G);

    printf("\n\n\nTesting the drawCard function!\n\n");

    printf("Testing functionality when the deck and discard are empty\n");
    // test when discard and deck are empty
    G->deckCount[0] = 0;
    G->discardCount[0] = 0;
    retVal = drawCard(0, G);
    fails += assertCustom(retVal == -1, "Function does not return -1 when deck and discard are empty");

    printf("Testing functionality when the deck is empty and discard is not\n");
    // test when deck is empty and discard is not
    for (i=0; i<10; i++) {
        for (p=0; p<tempNumPlayers; p++) {

            G->discardCount[p] = 10;
            G->deckCount[p] = 0;
            G->handCount[p] = 0;
            
            currentDeckCount = G->deckCount[p];
            currentDiscardCount = G->discardCount[p];
            currentHandCount = G->handCount[p];

            drawCard(p, G);

            fails += assertCustom(G->discardCount[p] == 0, "Discard not cleared");
            fails += assertCustom(G->deckCount[p] == currentDiscardCount-1, "Deck count is not set correctly");
            fails += assertCustom(G->handCount[p] == currentHandCount+1, "Hand count not set correctly");


        }
    }

    printf("Testing functionality when the deck is not empty\n");
    // test when deck has cards

    // set all players deck
    for (i=0; i<10; i++) {
        for (p=0; p<tempNumPlayers; p++) {
            G->deck[p][i] = testDeck[i];
        }
    }

    // set all players deck count
    for (p=0; p<tempNumPlayers; p++) {
        G->deckCount[p] = 10;
    }

    // run test 8 times
    for (i=0; i<8; i++) {
        for (p=0; p<tempNumPlayers; p++) {

            currentDeckCount = G->deckCount[p];
            currentHandCount = G->handCount[p];

            drawCard(p, G);

            fails += assertCustom(G->deckCount[p] == currentDeckCount-1, "Deck count did not decrement");
            fails += assertCustom(G->handCount[p] == currentHandCount+1, "Hand count did not increment");

        }
    }

    if (fails == 0) {
        printf("All tests passed!\n");
    }
    else {
        printf("ERROR: %d tests failed!\n", fails);
    }

    

    return 0;
}
