/* -----------------------------------------------------------------------
 * Random Test of the baronEffect function from dominion.c
 * -----------------------------------------------------------------------
*/

/*
 * Prototype:       int baronEffect(struct gameState *state, int currentPlayer, int choice1)
 * Inputs:          game state pointer
 *                      Choice 1
 *                      number of buys
 *                      current players hand (and count)
 *                      coins
 *                      current players discard (and count)
 *                      Estate supply count
 * 
 *                      Choice 2
 *                      estate supply count
 *                      current player hand
 *                  current player ID
 *                  choice ID (1 or 2)
 * 
*/

#include <string.h>
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

#include "dominion.h"
#include "dominion_helpers.h"
#include "rngs.h"


int assertCustom(int x, char* msg) {
    /* -----------------------------------------------------------------------
    * Custom assertion to prevent code from exiting prematurely
    * 
    * Returns 1 when false to allow user to tally the number of failed tests
    * -----------------------------------------------------------------------
    */

    if (!x) {
        printf("\tERROR: %s\n", msg);
        return 1;
    }
    return 0;
}


int randomChoiceNumber() {
    /* -----------------------------------------------------------------------
    * Function to randomly generate a choice number.
    * 
    * Returns either 1 or 0
    * -----------------------------------------------------------------------
    */


    return rand() % 2;      // return value [0 or 1]

}


void randomHand(struct gameState *s, int playerID) {
    /* -----------------------------------------------------------------------
    * Function to randomly generate a hand for the player. The hand size
    * is random [0, 10] and each card is randomly selected [curse, treasuremap]
    * -----------------------------------------------------------------------
    */

    int handSize = rand() % MAX_HAND;     // random hand size 0-max
    
    int i;
    for (i=0; i<handSize; i++) {
        s->hand[playerID][i] = rand() % treasure_map+1;      // assign random card curse-treasuremap
    }

    s->handCount[playerID] = handSize;


}


int randomPlayerID(int numPlayers) {
    /* -----------------------------------------------------------------------
    * Function to randomly generate a player ID
    * 
    * Returns a number between 0 and numPlayers-1
    * -----------------------------------------------------------------------
    */


    return rand() % numPlayers;

}


int specificCardCount(struct gameState *state, int playerID, int card) {

  int count=0, i;

  for (i=0; i<state->handCount[playerID]; i++) {
    if (state->hand[playerID][i] == card) {
      count++;
    }
  }

  return count;

}


struct gameState* randomGameState(int numPlayers, int choiceNumber, int playerID) {

    /* -----------------------------------------------------------------------
    * Function to randomly generate a gameState struct. Takes the choice into
    * consideration and sets up the appropriate attributes
    * 
    * Returns a dynamically allocated game state
    * -----------------------------------------------------------------------
    */


    struct gameState *s = malloc(sizeof(struct gameState));

    int i;

    s->numPlayers = numPlayers;
    s->whoseTurn = playerID;
    s->numBuys = 0;

    if (choiceNumber == 1) {

        s->numBuys = 0;
        s->coins = 0;
        s->supplyCount[estate] = rand() % 10 + 1;   // random number of estates between 1 and 10

        for (i=0; i<numPlayers; i++) {
            randomHand(s,i);
            s->discardCount[i] = 0;
        }

    }

    // player has opted to add an estate to their hand
    else {

        s->supplyCount[estate] = rand() % 10 + 1;   // random number of estates between 1 and 10
        
        // build a random hand for each player
        for (i=0; i<numPlayers; i++) {
            randomHand(s, i);
        }
    }

    return s;

}

void compareStates(struct gameState *s, struct gameState* ogState, int choice, int returnVal) {

    assertCustom(ogState->numBuys + 1 == s->numBuys, "Number of buys incremented");

    // check choice 1
    if (choice == 1) {

        int oldPlayerEstates = specificCardCount(ogState, ogState->whoseTurn, estate);

        if (oldPlayerEstates > 0) {

            int p = s->whoseTurn;

            // coins += 4
            assertCustom(ogState->coins + 4 == s->coins, "Choice 1: Coins increased 4");

            // discard count went up
            assertCustom(s->discardCount[p] == ogState->discardCount[p] + 1, "Choice 1: Discard count incremented");

            // last discard card is estate
            assertCustom(s->discard[p][s->discardCount[p] - 1] == estate, "Choice 1: Last card in discard is estate");
            
            // hand count went down 1
            assertCustom(s->handCount[p] == ogState->handCount[p] - 1, "Choice 1: Hand count decremented");

            // estae count went down 1
            int newPlayerEstates = specificCardCount(s, s->whoseTurn, estate);
            assertCustom(newPlayerEstates == oldPlayerEstates - 1, "Choice 1: Players estate count decremented");


            return;
        }


    }

    // check choice 2 AND choice 1 when no estates are in hand
    int oldSupplyEstates = ogState->supplyCount[estate];

    if (oldSupplyEstates > 0) {

        // check that player gains estate
        int oldPlayerEstates = specificCardCount(ogState, ogState->whoseTurn, estate);
        int newPlayerEstates = specificCardCount(s, s->whoseTurn, estate);
        assertCustom(newPlayerEstates == oldPlayerEstates+1, "Gain Estate: Player estate count incremented");

        // check taht estate looses an estate
        int newSupplyEstates = s->supplyCount[estate];
        assertCustom(newSupplyEstates == oldSupplyEstates-1, "Gain Estate: Supply estate count decremented");

    }

    else {
        assertCustom(returnVal == 0, "Estates not greater than 0: return 0");
    }
}



// BaronEffect()
int main() {

    // Variable Declaration
    int choice;
    int playerID;
    int returnVal;
    int numPlayers;
    struct gameState *s, *copy;

    int i;
    int numTests = 10000;
    for (i=0; i<numTests; i++) {

        printf("\nTrial %d\n", i);

        // generate random inputs
        numPlayers = rand() % 3 + 2;                                    // number of players
        choice = randomChoiceNumber();                                  // choice number
        playerID = randomPlayerID(numPlayers);                          // player ID
        s = randomGameState(numPlayers, choice, playerID);              // game state
        copy = malloc(sizeof(struct gameState));
        memcpy(copy, s, sizeof(struct gameState));                      // copy game state

        // run function
        returnVal = Baron(choice, s, playerID);

        // make assertions
        compareStates(s, copy, choice, returnVal);

        // cleanup
        free(s);
        free(copy);
    }
    
    return 0;
}