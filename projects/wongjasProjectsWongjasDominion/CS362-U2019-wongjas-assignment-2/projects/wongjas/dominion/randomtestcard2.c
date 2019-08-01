/* -----------------------------------------------------------------------
 * Random Test of the minionEffect function from dominion.c
 * -----------------------------------------------------------------------
*/

/*
 * Prototype:       int minionEffect(struct gameState *state, int currentPlayer, int handPos, int choice1, int choice2)
 * Inputs:          game state pointer
 *                      number of actions
 *                      discard count
 * 
 *                      Choice 1
 *                      coins
 * 
 *                      Choice 2
 *                      all players hands (counts)
 *                      number of players
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
    * Returns either 1 or 2
    * -----------------------------------------------------------------------
    */


    return rand() % 2 + 1;      // return value [1 or 2]

}


int randomHandPosition(int handSize) {

    /* -----------------------------------------------------------------------
    * Function to randomly select a hand position
    * 
    * Returns a number between 0 and handsize-1
    * -----------------------------------------------------------------------
    */


    if (handSize == 0)
        return 0;

    return rand() % handSize;

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
    s->numActions = 0;

    // choice 1: add 2 coins
    if (choiceNumber == 1) {

        s->coins = 0;

    }

    // player has opted to add an estate to their hand
    else {

        // set all discards to 0 and
        // build a random hand for each player
        for (i=0; i<numPlayers; i++) {
            randomHand(s, i);
            s->discardCount[i] = 0;
        }
    }

    return s;

}


void compareStates(struct gameState *s, struct gameState* ogState, int choice, int returnVal) {

    assertCustom(ogState->numActions + 1 == s->numActions, "Number of actions incremented");

    // choice 1: add 2 coins
    if (choice == 1) {

        int p = s->whoseTurn;

        // check coins were added
        assertCustom(ogState->coins + 2 == s->coins, "Choice 1: 2 Coins added");

        // check a card was removed
        assertCustom(ogState->handCount[p] - 1 == s->handCount[p], "Choice 1: Hand count decremented");


    }

    // choice 2: hands discarded and redrawn
    else {

        int p;

        for (p=0; p<s->numPlayers; p++) {

            if (p == s->whoseTurn) {
                // current players hand count is now 4
                assertCustom(s->handCount[s->whoseTurn] == 4, "Choice 2: Current player's hand count set to 4");
            }

            else {
                // if other players hand count was 5 or more, their hand count is now 4
                if (ogState->handCount[p] > 4) {
                    assertCustom(s->handCount[p] == 4, "Choice 2: Other players hand count was greater than 4 and is now 4");
                }
                // otherwise nothing happens to their hand
                else {
                    assertCustom(s->handCount[p] < 5, "Choice 2: Other player's hand count is less than 5");
                    assertCustom(ogState->handCount[p] == s->handCount[p], "Choice 2: Other player's hand count less than 5 and not changed");
                }
            }
        }
    }
}



// MinionEffect()
int main() {

    // Variable Declaration
    int choice;
    int playerID;
    int returnVal;
    int handPosition;
    int numPlayers;
    struct gameState *s, *copy;

    int i;
    int numTests = 1000;
    for (i=0; i<numTests; i++) {

        printf("\nTrial %d\n", i);

        // generate random inputs
        numPlayers = rand() % 3 + 2;                                    // number of players
        choice = randomChoiceNumber();                                  // choice number
        playerID = randomPlayerID(numPlayers);                          // player ID
        s = randomGameState(numPlayers, choice, playerID);              // game state
        handPosition = randomHandPosition(s->handCount[s->whoseTurn]);  // hand position of current player
        copy = malloc(sizeof(struct gameState));                        // allocate a copy game state
        memcpy(copy, s, sizeof(struct gameState));                      // copy game state

        // run function
        if (choice == 1)
            returnVal = Minion(1, 0, s, handPosition, playerID);
        else if (choice == 2)
            returnVal = Minion(0, 1, s, handPosition, playerID);

        // make assertions
        compareStates(s, copy, choice, returnVal);

        // cleanup
        free(s);
        free(copy);
    }
    
    return 0;
}