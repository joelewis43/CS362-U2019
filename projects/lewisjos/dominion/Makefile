CFLAGS= -Wall -fpic -coverage -lm -std=c99

rngs.o: rngs.h rngs.c
	gcc -c rngs.c -g  $(CFLAGS)

dominion.o: dominion.h dominion.c rngs.o
	gcc -c dominion.c -g  $(CFLAGS)

playdom: dominion.o playdom.c
	gcc -o playdom playdom.c -g dominion.o rngs.o $(CFLAGS)

#To run playdom you need to entere: ./playdom <any integer number> like ./playdom 10*/
testDrawCard: testDrawCard.c dominion.o rngs.o
	gcc  -o testDrawCard -g  testDrawCard.c dominion.o rngs.o $(CFLAGS)

badTestDrawCard: badTestDrawCard.c dominion.o rngs.o
	gcc -o badTestDrawCard -g  badTestDrawCard.c dominion.o rngs.o $(CFLAGS)

testBuyCard: testDrawCard.c dominion.o rngs.o
	gcc -o testDrawCard -g  testDrawCard.c dominion.o rngs.o $(CFLAGS)

testAll: dominion.o testSuite.c
	gcc -o testSuite testSuite.c -g  dominion.o rngs.o $(CFLAGS)

interface.o: interface.h interface.c
	gcc -c interface.c -g  $(CFLAGS)

runtests: testDrawCard 
	./testDrawCard &> unittestresult.out
	gcov dominion.c >> unittestresult.out
	cat dominion.c.gcov >> unittestresult.out



# Assignment 2 - Unit Tests of Refactored Functions
baronTest: dominion.o rngs.o unittest1.c
	gcc -o baronTest unittest1.c -g dominion.o rngs.o $(CFLAGS)

minionTest: dominion.o rngs.o unittest2.c
	gcc -o minionTest unittest2.c -g dominion.o rngs.o $(CFLAGS)

ambassadorTest: dominion.o rngs.o unittest3.c
	gcc -o ambassadorTest unittest3.c -g dominion.o rngs.o $(CFLAGS)

tributeTest: dominion.o rngs.o unittest4.c
	gcc -o tributeTest unittest4.c -g dominion.o rngs.o $(CFLAGS)

mineTest: dominion.o rngs.o unittest5.c
	gcc -o mineTest unittest5.c -g dominion.o rngs.o $(CFLAGS)


# Assignment 2 - Refactored Function Unit Test Controller
unittestresults: dominion.o rngs.o baronTest minionTest mineTest ambassadorTest tributeTest
	./baronTest &> unittestresults.out
	./minionTest >> unittestresults.out
	./mineTest >> unittestresults.out
	./tributeTest >> unittestresults.out
	./ambassadorTest >> unittestresults.out
	
	echo "" >> unittestresults.out
	echo "" >> unittestresults.out
	echo "" >> unittestresults.out
	echo "" >> unittestresults.out
	echo "Branch Coverage ..." >> unittestresults.out
	echo "" >> unittestresults.out
	echo "" >> unittestresults.out
	gcov dominion.c -b -f >> unittestresults.out


# Assignment 2 - Default Function Unit Tests
shuffleTest: dominion.o rngs.o cardtest2.c
	gcc -o shuffleTest cardtest2.c -g dominion.o rngs.o $(CFLAGS)

endTurnTest: dominion.o rngs.o cardtest3.c
	gcc -o endTurnTest cardtest3.c -g dominion.o rngs.o $(CFLAGS)

drawCardTest: dominion.o rngs.o cardtest5.c
	gcc -o drawCardTest cardtest5.c -g dominion.o rngs.o $(CFLAGS)

# Assignment 2 - Default FUnction Unit Test Controller
cardtestresults: dominion.o rngs.o shuffleTest endTurnTest drawCardTest
	./shuffleTest &> cardtestresults.out
	./endTurnTest >> cardtestresults.out
	./drawCardTest >> cardtestresults.out

	echo "" >> cardtestresults.out
	echo "" >> cardtestresults.out
	echo "" >> cardtestresults.out
	echo "" >> cardtestresults.out
	echo "Branch Coverage ..." >> cardtestresults.out
	echo "" >> cardtestresults.out
	echo "" >> cardtestresults.out
	gcov dominion.c -b -f >> cardtestresults.out


# Assignment 4 - Refactored Function Random Tests
randomBaronTest: dominion.o rngs.o randomtestcard1.c
	gcc -o randomBaronTest randomtestcard1.c -g dominion.o rngs.o $(CFLAGS)

randomMinionTest: dominion.o rngs.o randomtestcard2.c
	gcc -o randomMinionTest randomtestcard2.c -g dominion.o rngs.o $(CFLAGS)

randomTributeTest: dominion.o rngs.o randomtestcard3.c
	gcc -o randomTributeTest randomtestcard3.c -g dominion.o rngs.o $(CFLAGS)

# Assignment 4 - Refactored Function Random Test Controller
randomtestresults: dominion.o rngs.o randomBaronTest randomMinionTest randomTributeTest

	./randomBaronTest &> barontestresults.out 

	./randomMinionTest &> miniontestresults.out

	./randomTributeTest &> tributetestresults.out

	gcov dominion.c -b -f &> randomtestresults.out


player: player.c interface.o
	gcc -o player player.c -g  dominion.o rngs.o interface.o $(CFLAGS)

all: playdom player

testUpdateCoins:
	gcc -o testUpdateCoins -g testUpdateCoins.c dominion.o rngs.o $(CFLAGS)

clean:
	rm -f *.o playdom.exe playdom player player.exe  *.gcov *.gcda *.gcno *.so *.out testDrawCard testDrawCard.exe badTestDrawCard testUpdateCoins minionTest *.out *Test
	clear
