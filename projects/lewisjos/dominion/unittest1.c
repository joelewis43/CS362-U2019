/* -----------------------------------------------------------------------
 * Unit Test of the baronEffect function from dominion.c
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

int cardCount(struct gameState *G, int p, int card) {
    int count, i;

    for (i=0; i<G->handCount[p]; i++) {
        if (G->hand[p][i] == card) {
            count++;
        }
    }

    return count;
}


// Baron
// a two player game starts with 8 estates
int main() {

    // Game setup
    int i;
    int seed = 1000;
    int numPlayer = 2;
    int p;
    int k[10] = {adventurer, council_room, feast, gardens, mine,
                 remodel, smithy, village, baron, great_hall};

    int hand[10] = {estate, baron, baron, baron, baron, baron, baron, baron, estate, baron};

    struct gameState *G;
    G = newGame();
    int handCount = 10;
    int fails = 0;

    int currentCoins = 0;
    int currentHandCounts[numPlayer];
    int estateCount;
    int supplyEstateCount;
    
    


    printf("\n\nTESTING baronEffect():\n");

    if (initializeGame(numPlayer, k, seed, G) == -1) {          // init the game
        printf("Initialization Failure\n");
        return 1;
    }

    // setup player hands and coins
    for (p=0; p<numPlayer; p++) {
        G->handCount[p] = handCount;
        G->coins = 0;
        memcpy(G->hand[p], hand, sizeof(int) * handCount);
    }

    /* -----------------------------------------------------------------------
    * Testing choice 1: discard estate for coins
    * 
    * Assertions
    *   if the player has an estate
    *       player hand/estate count --
    *       coins += 4
    *   if the player does not have an estate
    *       if the supply does not have an estate, the game ended
    *       if the supply does have an estate
    *           player hand/estate count ++
    *           supply estate count --
    *           if no more estates in supply, game ended
    * -----------------------------------------------------------------------
    */
    printf("TESTING choice 1\n");

    for (i=0; i<5; i++) {
        for (p=0; p<numPlayer; p++) {

            currentCoins = G->coins;
            estateCount = cardCount(G, p, estate);
            currentHandCounts[p] = G->handCount[p];
            supplyEstateCount = supplyCount(estate, G);

            baronEffect(G, p, 1);                               // choice 1

            if (estateCount) {                                  // player has an estate

                printf("The player chose choice 1 and had an estate\n\n");

                printf("Asserting that the players coins were increased by 4\n");
                fails += assertCustom(G->coins == currentCoins+4, "Coins did not increase 4");

                printf("Asserting that the estate card was removed from the players hand\n");
                fails += assertCustom(cardCount(G, p, estate) == estateCount-1, "The estate count did not decrease");
                fails += assertCustom(G->handCount[p] == currentHandCounts[p]-2, "The hand count did not decrease");

            }

            else {                                              // player has no estate cards

                printf("The player chose choice 1 and did not have an estate\n\n");

                if (supplyEstateCount) {                        // there was an estate in the supply

                    printf("Asserting that the estate card was added to the players hand\n");
                    fails += assertCustom(cardCount(G, p, estate) == 1, "The estate count does not equal 1");
                    fails += assertCustom(G->handCount[p] == currentHandCounts[p], "The hand count is not the same");

                    printf("Asserting that the supply's estate count went down 1\n");
                    fails += assertCustom(supplyCount(estate, G) == supplyEstateCount-1, "The supply count did not decrement");



                }


            }




        }
    }


    /* -----------------------------------------------------------------------
    * Testing choice 2: enherit an estate
    * 
    * Assertions
    *   if there is no estate in the supply, the game ended
    *   if there is an estate in the supply,
    *       player hand count ++
    *       player estate count ++
    *       supply estate count --
    *       if the supply is now empty, the game ended
    * -----------------------------------------------------------------------
    */
    printf("\n\nTESTING choice 2\n");

    for (i=0; i<3; i++) {
        for (p=0; p<numPlayer; p++) {

            currentCoins = G->coins;
            estateCount = cardCount(G, p, estate);
            currentHandCounts[p] = G->handCount[p];
            supplyEstateCount = supplyCount(estate, G);

            baronEffect(G, p, 0);                               // choice 2

            if (supplyEstateCount) {                            // there was an estate in the supply

                printf("Asserting that the estate card was added to the players hand\n");
                fails += assertCustom(cardCount(G, p, estate) == 1, "The estate count does not equal 1");
                fails += assertCustom(G->handCount[p] == currentHandCounts[p], "The hand count did not decrease");

                printf("Asserting that the supply's estate count went down 1\n");
                fails += assertCustom(supplyCount(estate, G) == supplyEstateCount-1, "The supply count did not decrement");

            }

            else {
                printf("The supply is empty....\n");
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