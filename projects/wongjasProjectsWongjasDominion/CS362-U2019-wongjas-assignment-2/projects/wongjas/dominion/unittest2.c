/* -----------------------------------------------------------------------
 * Unit Test of the minionEffect function from dominion.c
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


// Minion
int main() {

    // Game setup
    int i, j;
    int seed = 1000;
    int numPlayer = 2;
    int p;
    int k[10] = {adventurer, council_room, feast, gardens, mine,
                 remodel, smithy, village, baron, great_hall};
    int minions[10] = {minion, minion, minion, minion, minion, minion, minion, minion, minion, minion};
    struct gameState *G;
    G = newGame();
    int handCount = 10;
    int fails = 0;

    int currentCoins = 0;
    int currentActions;
    int currentHandCounts[numPlayer];
    
    


    printf("\n\nTESTING minionEffect():\n");

    if (initializeGame(numPlayer, k, seed, G) == -1) {          // init the game
        printf("Initialization Failure\n");
        return 1;
    }

    // setup player hands and coins
    for (p=0; p<numPlayer; p++) {
        G->handCount[p] = handCount;
        G->coins = 0;
        memcpy(G->hand[p], minions, sizeof(int) * handCount);
        currentHandCounts[p] = 10;
    }

    /* -----------------------------------------------------------------------
    * Testing choice 1: adding two coins
    * 
    * Assertions
    *   actions has gone up 1
    *   handCount has gone down by 1
    *   coins has gone up 2
    * -----------------------------------------------------------------------
    */
    printf("TESTING choice 1\n");

    for (i=0; i<3; i++) {
        for (p=0; p<numPlayer; p++) {

            printf("\nPlayer %d\n", p);

            currentHandCounts[p] = G->handCount[p];             // store the players handcount
            currentActions = G->numActions;                     // store the players number of actions
            currentCoins = G->coins;                            // store the players number of coins

            Minion(1, 0, G, 0, p);                              // run the effect (choice 1)
            

            // assertions
            printf("Asserting handcount has decremented\n");
            fails += assertCustom(G->handCount[p] == currentHandCounts[p]-1, "Player Hand Count Was Not Decremented");

            printf("Asserting actions has incremented\n");
            fails += assertCustom(G->numActions == currentActions+1, "Actions Not Incremented");

            printf("Asserting coins has gone up 2\n");
            fails += assertCustom(G->coins == currentCoins+2, "Coins Not Incremented By 2");

        }
    }


    /* -----------------------------------------------------------------------
    * Testing choice 2: discard hand and draw 4
    * 
    * Assertions
    *   actions has gone up 1
    *   hand count is now 4
    *   opposing hand count is 4 IF previous handcound was greater than 4
    * -----------------------------------------------------------------------
    */
    printf("\n\nTESTING choice 2\n");

    for (i=0; i<3; i++) {
        for (p=0; p<numPlayer; p++) {

            printf("\nPlayer %d\n", p);

            for (j=0; j<numPlayer; j++) {                       // get all players current hand count
                currentHandCounts[j] = G->handCount[j];
            }

            currentActions = G->numActions;                     // store the players number of actions

            Minion(0, 1, G, 0, p);                              // run the effect (choice 2)

            // assertions
            printf("Asserting actions has incremented\n");
            fails += assertCustom(G->numActions == currentActions+1, "Action Not Incremented");

            for (j=0; j<numPlayer; j++) {                       // assert player handcounts

                // check that current player's hand count is now 4
                if (j == p) {
                    printf("Asserting current players handcount\n");
                    fails += assertCustom(G->handCount[j] == 4, "Player's Hand Count Not Set To 4");
                }

                // check other players
                else {
                    printf("Asserting player %d's handcount\n", j);

                    // check that players with more than 4 cards now have 4
                    if (currentHandCounts[j] >= 5)
                        fails += assertCustom(G->handCount[j] == 4, "Other Players Hand Count Not Set To 4");

                    // check that players with 4 or less cards still have the same amount
                    else
                        fails += assertCustom(G->handCount[j] == currentHandCounts[j], "Other Players Hand Count Not The Same");
                }
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