/* -----------------------------------------------------------------------
 * Random Test of the tributeEffect function from dominion.c
 * -----------------------------------------------------------------------
*/

/*
 * Prototype:       int tributeEffect(struct gameState *state, int currentPlayer, int nextPlayer)
 * Inputs:          current player ID
 *                  other player ID
 *                  game state pointer
 *                      discard count
 *                      discard
 *                      deck count
 *                      deck
 *                      playedCards count
 *                      coins
 *                      actions
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


void randomDiscard(struct gameState *s, int playerID) {
    /* -----------------------------------------------------------------------
    * Function to randomly generate a discard pile for the player. The hand size
    * is random [0, MAX-1] and each card is randomly selected [curse, treasuremap]
    * -----------------------------------------------------------------------
    */

    int handSize = rand() % MAX_HAND;     // random hand size 0-max
    
    int i;
    for (i=0; i<handSize; i++) {
        s->discard[playerID][i] = rand() % treasure_map+1;      // assign random card curse-treasuremap
    }

    s->discardCount[playerID] = handSize;


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


struct gameState* randomGameState(int numPlayers, int playerID) {

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
    s->coins = 0;
    s->playedCardCount = 0;

    for (i=0; i<numPlayers; i++) {

        randomDiscard(s, i);
        randomHand(s, i);

    }




    return s;

}


void compareStates(struct gameState *s, struct gameState* ogState, int p1, int p2) {

    int tributeNumber;
    int tributeCards[2] = {-1, -1};

    // check if p2 has less than 2 cards
    if (ogState->discardCount[p2] + ogState->deckCount[p2] < 2) {

        // check if one card is in the deck
        if (ogState->deckCount[p2] == 1) {
            tributeCards[0] = ogState->deck[p2][0];
            tributeNumber = 1;
        }

        // check if one card is in the discard
        if (ogState->discardCount[p2] == 1) {
            tributeCards[0] = ogState->discard[p2][0];
            tributeNumber = 1;
        }

        else {
            tributeNumber = 0;
        }

    }

    // p2 has as least 2 cards
    else {

        tributeNumber = 2;
        tributeCards[0] = ogState->deck[p2][ogState->deckCount[p2]-1];
        tributeCards[1] = ogState->deck[p2][ogState->deckCount[p2]-2];

    }

    if (tributeCards[0] == tributeCards[1]) {
        assertCustom(ogState->playedCardCount + 1 == s->playedCardCount, "Same card, played card count");
        tributeCards[1] = -1;
        tributeNumber = 1;
    }

    int goldChange = 0;
    int cardChange = 0;
    int actionChange = 0;

    int i;
    for (i=0; i<tributeNumber; i++) {

        // chcek for treasure
        if (tributeCards[i] >= copper && tributeCards[i] <= gold) {
            goldChange += 2;
        }

        // check for victory card
        else if (tributeCards[i] == estate || tributeCards[i] == duchy || tributeCards[i] == province || tributeCards[i] == great_hall) {
            cardChange += 2;
        }

        // ensure the card is initialized
        else if (tributeCards[i] != -1) {
            actionChange += 2;
        }

    }

    assertCustom(ogState->coins + goldChange == s->coins, "Added coins");
    assertCustom(ogState->handCount[p1] + cardChange == s->handCount[p1], "Drawn cards");
    assertCustom(ogState->numActions + actionChange == s->numActions, "Added actions");

}



// TributeEffect()
int main() {

    // Variable Declaration
    int playerID;
    int otherPlayerID;
    int numPlayers;
    struct gameState *s, *copy;

    int i;
    int numTests = 1000;
    for (i=0; i<numTests; i++) {

        printf("\nTrial %d\n", i);

        // generate random inputs
        numPlayers = rand() % 3 + 2;                                    // number of players
        playerID = randomPlayerID(numPlayers);                          // player ID

        do { otherPlayerID = randomPlayerID(numPlayers); }              // other player ID
        while(playerID == otherPlayerID);                               // ensure they arent the same

        s = randomGameState(numPlayers, playerID);                      // game state
        copy = malloc(sizeof(struct gameState));                        // allocate a copy game state
        memcpy(copy, s, sizeof(struct gameState));                      // copy game state

        // run function
        tributeEffect(s, playerID, otherPlayerID);

        // make assertions
        compareStates(s, copy, playerID, otherPlayerID);

        // cleanup
        free(s);
        free(copy);
    }
    
    return 0;
}