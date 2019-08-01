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


// Mine
// choice 1 is hand index of treasure to hand in
// choice 2 is hand index of treasure to recv
int main() {

    // Game setup
    int i;
    int seed = 1000;
    int numPlayer = 2;
    int p, r;
    int k[10] = {adventurer, council_room, feast, gardens, minion,
                 remodel, smithy, village, baron, great_hall};

    int hand[10] = {copper, copper, silver, copper, gold, silver, silver, silver, copper, gold};

    int treasure[6] = {copper, silver, gold, baron, adventurer, minion};

    struct gameState *G;
    G = newGame();

    int handCount = 10;
    int fails = 0;

    int choice1HandCount;
    int choice2HandCount;
    int prevHandCount;

    int c1, c2, temp;
    
    


    printf("\n\nTESTING mineEffect():\n");

    if (initializeGame(numPlayer, k, seed, G) == -1) {          // init the game
        printf("Initialization Failure\n");
        return 1;
    }

    // setup player hands
    for (p=0; p<numPlayer; p++) {
        G->handCount[p] = handCount;
        memcpy(G->hand[p], hand, sizeof(int) * handCount);
    }

    /* -----------------------------------------------------------------------
    * Testing choice 1: adding two coins
    * 
    * Assertions
    *   nothing happens if choice 1 is bad treasure
    *   nothing happens if choice 2 is bad treasure
    *   nothing happens if choice 1 + 3 is > choice 2
    *   choice 2 count has gone up 1
    *   played card has been discarded
    *   choice 1 has been discarded
    * -----------------------------------------------------------------------
    */
    for (i=0; i<2; i++) {
        for (p=0; p<numPlayer; p++) {

            printf("\nPlayer %d\n", p);
   
            prevHandCount = G->handCount[p];

            // select cards to test
            c1 = treasure[rand() % 6];
            do {
                c2 = treasure[rand() % 6];
            } while (c2 != c1);
            

            // store the count of c1 and c2 at the start of the turn
            choice1HandCount = cardCount(G, p, c1);
            choice2HandCount = cardCount(G, p, c2);

            // run the effect
            r = Mine(c1, c2, p, G, 0, 0);

            // assertions
            if (c1 < copper || c1 > gold) {
                printf("Asserting that nothing happened due to invalid c1\n");
                fails += assertCustom(r == -1, "mineEffect Did Not Return -1");

                printf("Asserting that the handcount is the same as it was\n");
                fails += assertCustom(G->handCount[p] == prevHandCount, "Players Hand Count Was Changed");
            }

            else if (c2 > treasure_map || c2 < curse) {
                printf("Asserting that nothing happened due to invalid c2\n");
                fails += assertCustom(r == -1, "mineEffect Did Not Return -1");

                printf("Asserting that the handcount is the same as it was\n");
                fails += assertCustom(G->handCount[p] == prevHandCount, "Players Hand Count Was Changed");
            }

            else if (c2 > c1+3) {
                printf("Asserting that nothing happened because c2 was too large\n");
                fails += assertCustom(r == -1, "mineEffect Did Not Return -1");

                printf("Asserting that the handcount is the same as it was\n");
                fails += assertCustom(G->handCount[p] == prevHandCount, "Players Hand Count Was Changed");
            }

            else {

                printf("Asserting that the c2 count has incremented\n");
                temp = cardCount(G, p, c2);
                fails += assertCustom(temp == choice2HandCount+1, "Players C2 count was not incremented");

                printf("Asserting that the c1 count has decremented\n");
                temp = cardCount(G, p, c1);
                fails += assertCustom(temp == choice1HandCount-1, "Players C1 Count Was Not Incremented");

                printf("Asserting that the handcount has decremented\n");
                fails += assertCustom(G->handCount[p] == prevHandCount-1, "Players Hand Count Was Not Decremented");

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
