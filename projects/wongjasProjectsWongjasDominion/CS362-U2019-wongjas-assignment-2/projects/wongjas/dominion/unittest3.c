/* -----------------------------------------------------------------------
 * Unit Test of the ambassadorEffect function from dominion.c
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


// Ambassador
// choice 1 is hand index of card to remove
// choice 2 is number of copies to remove
int main() {

    // Game setup
    int i, j, r;
    int seed = 1000;
    int numPlayer = 2;
    int p;
    int k[10] = {adventurer, council_room, feast, gardens, minion,
                 remodel, smithy, village, baron, great_hall};

    int hand[10] = {copper, copper, silver, copper, gold, silver, silver, silver, copper, gold};

    struct gameState *G;
    G = newGame();

    int handCount = 10;
    int fails = 0;

    int choice1HandCount = 0;
    int prevHandCount = 0;
    int prevSupplyCount = 0;

    int currentHandCounts[numPlayer];
    int currentC1Counts[numPlayer];

    int c1, c2;
    
    


    printf("\n\nTESTING ambassadorEffect():\n");

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
    * Assertions
    *   if c2 > 2 nothing happens
    *   if c1 == hand position nothing happens
    *   if player has fewer than c2 c1 cards nothing happens
    *   supply of c1 went up c2
    *   all other players hand count went up one AND
    *       their c1 card count went up 1
    *   players card count went down 1+c2
    *   players c1 count went down c2
    * -----------------------------------------------------------------------
    */
    for (i=0; i<2; i++) {
        for (p=0; p<numPlayer; p++) {

            printf("\nPlayer %d\n", p);

            c1 = rand() % 10;                               // get a random hand pos            [0, 9]
            c2 = rand() % 4;                                // get a random number of copies    [0, 3] 

            for (j=0; j<numPlayer; j++) {                   // get all players current
                currentHandCounts[j] = G->handCount[j];     // hand count
                currentC1Counts[j] = cardCount(G, j, c1);   // c1 count
            }

            choice1HandCount = cardCount(G, p, c1);         // store the count of c1 at the start of the turn
            prevHandCount = G->handCount[p];                // store players hand count
            prevSupplyCount=G->supplyCount[G->hand[p][c1]]; // store the supply count

            r = Ambassador(c1, c2, G, 0, p);                // run the function

            if (c2 > 2) {                                   // check that a large c2 had no effect
                printf("Asserting that nothing happened due to c2 > 2\n");
                fails += assertCustom(r == -1, "amassadorEffect Did Not Return -1");
            }

            else if (c1 == 0) {                             // check that c1 == hand pos had no effect
                printf("Asserting that nothing happened due to c1 == handpos\n");
                fails += assertCustom(r == -1, "amassadorEffect Did Not Return -1");
            }

            else if (cardCount(G, p, c1) < c2) {            // check if player has too few c1 that nothing happens
                printf("Asserting that nothing happened due to count(c1) < c2\n");
                fails += assertCustom(r == -1, "amassadorEffect Did Not Return -1s");
            }

            else {

                printf("Asserting that the supply of c1 was increased by c2\n");
                fails += assertCustom(G->supplyCount[G->hand[p][c1]] == prevSupplyCount+c2, "Supply Count Not Increased By C2");

                printf("Asserting the handcount of other players was increased by 1 and that their c1 count has increased\n");
                for (j=0; j<numPlayer; j++) {
                    if (j != p) {
                        fails += assertCustom(G->handCount[j] == currentHandCounts[j]+1, "Other Player Hand Count Was Not Incremented");
                        fails += assertCustom(cardCount(G, j, c1) == currentC1Counts[j]+1, "Other Players C1 Count Was Not Incremented");
                    }
                }

                printf("Asserting the handcount of the player went down 1+c2\n");
                fails += assertCustom(G->handCount[p] == currentHandCounts[p]-1-c2, "Players Hand Count Did Not Decrease By 1+c2");

                printf("Asserting the players c1 card count went down by c2\n");
                fails += assertCustom(cardCount(G, p, c1) == currentC1Counts[p]-c2, "Players C1 Count Did Not Decrease By C2");

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
