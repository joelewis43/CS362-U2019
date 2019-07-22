/* -----------------------------------------------------------------------
 * Unit Test of the tributeEffect function from dominion.c
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

// set NOISY_TEST to 0 to remove printfs from output
#define NOISY_TEST 1



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


// tribute
int main() {

    // Game setup
    int i;
    int seed = 1000;
    int numPlayer = 2;
    int p;
    int k[10] = {adventurer, council_room, feast, gardens, mine,
                 remodel, smithy, village, baron, great_hall};
    int tributes[10] = {tribute, tribute, tribute, tribute, tribute, tribute, tribute, tribute, tribute, tribute};
    struct gameState *G;
    G = newGame();
    int handCount = 10;
    int fails = 0;

    int currentCoins = 0;
    int currentActions;
    int currentHandCounts[numPlayer];
    int nextPlayer;

    int discardCount;
    int totalDifference;
    
    


    printf("\n\nTESTING tributeEffect():\n");

    if (initializeGame(numPlayer, k, seed, G) == -1) {          // init the game
        printf("Initialization Failure\n");
        return 1;
    }

    // setup player hands and coins
    for (p=0; p<numPlayer; p++) {
        G->handCount[p] = handCount;
        G->coins = 0;
        memcpy(G->hand[p], tributes, sizeof(int) * handCount);
        currentHandCounts[p] = 10;
    }

    /* -----------------------------------------------------------------------
    * Assertions
    *   if next player has one card, it is added to tribute stack
    *   if next player has no cards, nothing happens
    *   if player has more than one card, 
    *       check if their hand is empty
    *           shuffle
    *       card pulled from end of deck into array
    *       last card deint and deck decremented
    *       card pulled from new end
    *       deinit and decrement
    * -----------------------------------------------------------------------
    */
    printf("TESTING choice 1\n");

    for (i=0; i<3; i++) {
        for (p=0; p<numPlayer; p++) {

            printf("\nPlayer %d\n", p);

            currentHandCounts[p] = G->handCount[p];             // store the players handcount
            currentActions = G->numActions;                     // store the players number of actions
            currentCoins = G->coins;                            // store the players number of coins

            if (p == numPlayer-1) {
                nextPlayer = 0;
            }
            else {
                nextPlayer = p+1;
            }

            currentHandCounts[nextPlayer] = G->handCount[nextPlayer];
            discardCount = G->discardCount[nextPlayer];

            tributeEffect(G, p, nextPlayer);

            // next player has one or fewer cards
            if (currentHandCounts[nextPlayer] + discardCount <= 1) {
                if (currentHandCounts[nextPlayer] == 1) {
                    // their hand decrements
                }

                else if (discardCount == 1) {
                    // difficult to assert
                }

                else {
                    // nothing happens
                }
            }

            // next player has plenty of cards
            else {
                printf("The next player had more than one card left\n");

                printf("Asserting next player handcount has gone down 2\n");
                fails += assertCustom(G->handCount[nextPlayer] == currentHandCounts[nextPlayer]-2, "Next player's hand count did not go down 2");

                printf("Asserting the player was added the correct number of items\n");
                totalDifference = G->handCount[p] - currentHandCounts[p] + G->numActions - currentActions + G->coins - currentCoins;
                fails += assertCustom(totalDifference == 4, "Player not awarded cards/coins/actions");
            }
        }
    }
      
    

    // check for failures
    if (fails == 0) {
        printf("\n\nAll tests passed!\n");
    }
    else {
        printf("\n\nFAIL: %d tests failed\n\n\n", fails);
    }

    free(G);

    return 0;
}