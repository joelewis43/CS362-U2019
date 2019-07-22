
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

void randomDeck(int deck[10]) {

    int i;
    for (i=0; i<10; i++) {
        deck[i] = rand() % 20 + 1;      // [1, 20]
    }

}

int decksNotEqual(int deck1[10], int deck2[10]) {

    int i;

    for (i=0; i<10; i++) {

        if (deck1[i] != deck2[i])
            return 1;

    }
    return 0;
}


// Shuffle
int main() {

    int i, j;
    int fails = 0;
    int deck[MAX_DECK] = {};
    struct gameState *G;

    int tempNumPlayers = 3;
    int tempDeck[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    int tempSeed = 123;

    // create game struct
    G = malloc(sizeof (struct gameState));
    initializeGame(tempNumPlayers, tempDeck, tempSeed, G);

    printf("\n\n\nTesting the shuffle function!\n\n");

    // run 10 tests
    for (i=0; i<10; i++) {

        randomDeck(deck);               // build random deck

        for (j=0; j<10; j++) {
            G->deck[0][j] = deck[j];
        }

        G->deckCount[0] = 10;

        shuffle(0, G);                  // run the function

        fails += assertCustom(decksNotEqual(G->deck[0], deck), "The decks were equal (possible as deck is shuffled by random chance)");


    }



    if (fails == 0) {
        printf("All tests passed!\n");
    }
    else {
        printf("ERROR: %d tests failed!\n", fails);
    }

    

    return 0;
}
