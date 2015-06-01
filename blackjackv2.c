// Leon Nguyen z5062414
// Program that plays a basic version of Blackjack against AI

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <time.h>

#define MAX_TITLE 18
#define MAX_SUIT
#define MAX_CARDS 12 
#define MAX_NAME 15
#define MAX_VALUE 21 // max value of hand is 21

#define SUIT_SIZE 13
#define DECK_SIZE 52

#define HIT 1
#define STAND 2

#define NO_ONE 0
#define USER 1
#define DEALER 2

#define TRUE 1
#define FALSE 0

typedef struct _card *Card;

typedef struct _card {
   char title[MAX_TITLE]; //e.g. Queen of Diamonds
   int value; //1-10
   Card next; // for my pack linked list
} card;

typedef struct _player {
   Card hand[MAX_CARDS];
   int numCards;
   double cash;
} *Player;

typedef struct _pack {
   Card head;
   int numCardsLeft;
} *Pack;

void displayRules (void); // displays rules of the game
Player newPlayer (void); // mallocs memory and sets up a new player
Card newCard (char *title, int value);
// creates a new card for purposes of helping newPack function
Pack newPack (void); //mallocs memory for an entire deck linked list
void dealHand (Player currPlayer, Pack deck); 
// deals a card to the player's hand
void displayHands(Player dealer, Player user); // displays all hands
int requestAction(void); // returns HIT or STAND as next action by user
int ifBust (Player currPlayer);
// calculates value of hand; if over 21 then returns true (instant lose)
int ifBlackjack (Player currPlayer);
// checks if player has J/Q/K + A (auto win)
int getValueOfHand (Player currPlayer);
// gets the value of a player's hand 
void makeNextAIAction(Player dealer, Pack deck); //makes next move by AI
void revealHands (Player dealer, Player user);
// similar to displayHands except used at end to determine winner
// (it shows dealers' hand)
int determineWinner (Player dealer, Player user);
// determines winner based on current hand of dealer and player
int freeGame (Player dealer, Player user, Pack deck);
// frees everything malloced for the game


int main (int argc, char* argv[]) {

   char requestRules;
   printf ("Would you like to see the rules? (Y/N): ");
   scanf ("%c", &requestRules);
   if (requestRules != 'n' && requestRules != 'N') {                     // accounts for likelihood of users entering lower/upper case
      displayRules();
   }
   printf ("\n");


   Player user = newPlayer();
   Player dealer = newPlayer();
   Pack deck = newPack();

   // Play the game
      double betAmount;
   while (TRUE) {
      // reset hands and pack
      user->numCards = 0;
      dealer->numCards = 0;
      deck = newPack();

      printf ("You currently have $%.2lf\n", user->cash);
      printf ("Please enter your bet size: ");
      scanf ("%lf", &betAmount);

      if (betAmount < 0) {
         betAmount = 0;
      }
      else if (betAmount > user->cash) {
         betAmount = user->cash;
      }

      user->cash -= betAmount;

      int gameRunning = TRUE;
      dealHand (user, deck);
      dealHand (dealer, deck);
      revealHands(dealer, user);
      printf ("Cash in bank: $%.2lf, Cash in play: $%.2lf\n",
         user->cash, betAmount);

      while (gameRunning == TRUE) {
         int action = requestAction();

         if (action == HIT) {
            dealHand (user, deck);
         } else {
            // action == STAND
            while (getValueOfHand(dealer) < 17 
                  && ifBlackjack(dealer) == FALSE) {
               dealHand (dealer, deck);
            }
            gameRunning = FALSE;
         }

         if (ifBust(user) == TRUE) {
            gameRunning = FALSE;
         }

         if (ifBlackjack (user)) {
            gameRunning = FALSE;
         }

         revealHands(dealer, user);
      }

      printf ("\n");
      
      if (ifBlackjack (user) == TRUE &&
         ifBlackjack (dealer) == TRUE) {
         printf ("Two blackjacks!? No winner.\n");
      }
      else if (ifBlackjack(user) == TRUE) {
         printf ("Blackjack! You win!\n");
         user->cash += 2.5 * betAmount;
      }
      else if (ifBlackjack(dealer) == TRUE) {
         printf ("Dealer has blackjack! You lose.");
      }
      else if (ifBust(user) == TRUE) {
         printf ("Bust! You lose.\n");
      } 
      else if (ifBust(dealer) == TRUE) {
         printf ("The dealer has busted! You win!\n");
         user->cash += 2 * betAmount;
      }
      else if (determineWinner(dealer, user) == NO_ONE) {
         printf ("Equal hands! No winner.\n");
      }
      else if (determineWinner(dealer, user) == USER) {
         printf ("You beat the dealer!\n");
         user->cash += 2 * betAmount;
      }
      else { // (determineWinner (dealer, user) == DEALER) 
         printf ("You lost to the dealer.\n");
      }

      printf ("\n");
   }

   return EXIT_SUCCESS;
}

// displays rules of the game
void displayRules (void) {
   char* message =
      "\n"
      "1. You will play against an AI dealer \n"
      "2. Card values:\n"
      "   Ace: 1 or 11\n"
      "   Jack, Queen, King: 10\n"
      "   Others: their respective numbers\n"
      "3. Each turn you can either hit (add a card to your hand)\n"
      "   or stand (keep your hand)\n"
      "4. The aim is to get as close as possible to 21 without going\n"
      "   over. If you go over 21 you instantly lose.\n"
      "\n"
      "Payouts:\n"
      "Win: Win 2x of what you bet\n"
      "Blackjack win: Win 2.5x of what you bet\n"
      "Lose: Lose what you bet\n";

      printf ("%s\n", message);
}

// mallocs memory and sets up a new player
Player newPlayer (void) {
   Player new = malloc (sizeof (struct _player));
   new->numCards = 0;
   new->cash = 500;
   return new;
}

// creates a new card for purposes of helping newPack function
Card newCard (char *title, int value) {
   Card new = malloc(sizeof(struct _card));
   strncpy (new->title, title, MAX_TITLE);
   new->value = value;

   if (value == 1) {
      // ace card
      new->value = 11;
   }

   new->next = NULL;

   return new;
}


//mallocs memory for an entire deck linked list
Pack newPack (void) {
   Pack new = malloc (sizeof (struct _pack));

   Card nextCard = newCard ("A ", 11);
   nextCard->title[2] = 3; // ♠ ASCII value into title
   new->head = nextCard;

   Card currentCard = new->head;


   char titles[] = {'A', '2', '3', '4', '5', '6', '7', '8', '9', 
                   'J', 'Q', 'K'}; // will do 10 manually after
   int cardCount = 1; // increments until 52
   int titleCount = 0; // increments through titles[] array
   int suitASCII = 4; // increments ♣,♦,♥,♠,...
   while (cardCount <= DECK_SIZE - 4) { // -4 since missing 10's

      // setting up next card to add to linked list
      nextCard = malloc (sizeof (struct _card));
      if (0 <= titleCount && titleCount <= 8) {
         // Ace to 9 have values corresponding to their numbers (A = 1)
         nextCard->value = titleCount + 1;
      } else {
         // J, Q, K have value 10
         nextCard->value = 10;
      }
      nextCard->title[0] = titles[titleCount];
      nextCard->title[1] = ' ';
      nextCard->title[2] = suitASCII;
      nextCard->title[3] = 0;

      nextCard->next = NULL;

      currentCard->next = nextCard;
      currentCard = currentCard->next;

      if (cardCount%4 == 0) {
         // increment title every 4 cards
         titleCount++;
      }

      suitASCII++;
      if (suitASCII > 6) {
         // wrap around so will loop ♠. ♣, ♦, ♥, ♠, ♣, ♦, ♥, ...
         suitASCII = 3;
      }

      cardCount++;
   }

   // now to add 10's to linked list
   cardCount = 1;
   while (cardCount <= 4) {
      nextCard = newCard ("10 ", 10);
      nextCard->title[3] = suitASCII;
      nextCard->title[4] = 0;
      currentCard->next = nextCard;
      currentCard = currentCard->next;

      suitASCII++;
      if (suitASCII > 6) {
         // wrap around
         suitASCII = 3;
      }

      cardCount++;
   }
   

   new->numCardsLeft = DECK_SIZE; //number of cards in deck

   return new;
} 

// deals a card to the player's hand
void dealHand (Player currPlayer, Pack deck) {
   srand(time(NULL));
   int chosenIndex = (rand())%(deck->numCardsLeft);

   Card chosen;
   Card prev = NULL;
   Card curr = deck->head;
   if (chosenIndex == 0) {
      chosen = deck->head;
   } else {
      int count = 0;
      while (count < chosenIndex) {
         prev = curr;
         curr = curr->next;
         count++;
       }
      // now curr at one to be removed
       chosen = curr;
       prev->next = curr->next;
   }
   
   int handSize = currPlayer->numCards;  
   currPlayer->hand[handSize] = chosen;

   currPlayer->numCards++;
   deck->numCardsLeft--;
}

// displays all hands
void displayHands(Player dealer, Player user) {
   int dealerHandSize = dealer->numCards;
   int userHandSize = user->numCards;
   printf ("\n");
   printf ("Dealer has %d card(s)\n", dealerHandSize);

   printf ("You have: ");
   int count = 0;
   while (count < userHandSize- 1) {
      printf ("%s, ", user->hand[count]->title);
      count++;
   }
   printf ("%s\n\n", user->hand[count]->title);

}

// returns HIT or STAND as next action by user
int requestAction(void) {
   int action;

   printf ("Enter your next move (1 for hit, 2 for stand): ");
   scanf ("%d", &action);

   assert (action == 1 || action == 2);

   return action;
}

// calculates value of hand; if over 21 then returns true (instant lose)
int ifBust (Player currPlayer) {
   int isHandBust = FALSE;

   if (getValueOfHand (currPlayer) > MAX_VALUE) {
      isHandBust = TRUE;
   }

   return isHandBust;
}

// checks if player has J/Q/K + A (auto win)
int ifBlackjack (Player currPlayer) {
   int hasBlackjack = FALSE;

   if (currPlayer->numCards == 2 && 
      getValueOfHand(currPlayer) == MAX_VALUE) {
      // only possible if J/Q/K + A
      hasBlackjack = TRUE;
   }

   return hasBlackjack;
}


// gets the value of a player's hand 
int getValueOfHand (Player currPlayer) {
   int totalValue = 0;
   int count = 0;
   while (count < currPlayer->numCards) {
      totalValue += currPlayer->hand[count]->value;
      count++;
   }

   if (totalValue > MAX_VALUE) {
      // scan through to check if hand contains A as it will then
      // be forced from value 11 to 1
      count = 0;
      while (count < currPlayer->numCards && totalValue > MAX_VALUE) {
         if (currPlayer->hand[count]->value == 11) {
            totalValue -= 10;
         }
         count++;
      }

   }

   return totalValue;
}

// makes next move by AI
void makeNextAIAction(Player dealer, Pack deck) {

   while (getValueOfHand(dealer) < 17) {
      // same as casino standards
      dealHand(dealer, deck);
   }
}

// similar to displayHands except used at end to determine winner
// (it shows dealers' hand)
void revealHands (Player dealer, Player user) {
   int dealerHandSize = dealer->numCards;
   int userHandSize = user->numCards;
   printf ("\n");
   printf ("Dealer's hand: ");

   int count = 0;
   while (count < dealerHandSize - 1) {
      printf ("%s, ", dealer->hand[count]->title);
      count++;
   }
   printf ("%s\n", dealer->hand[count]->title);

   printf ("Your hand: ");
   count = 0;
   while (count < userHandSize- 1) {
      printf ("%s, ", user->hand[count]->title);
      count++;
   }
   printf ("%s\n\n", user->hand[count]->title);
}


// determines winner based on current hand of dealer and player
int determineWinner (Player dealer, Player user) {
   int winner = NO_ONE;

   if (getValueOfHand (dealer) > getValueOfHand (user)) {
      winner = DEALER;
   }
   else if (getValueOfHand (dealer) < getValueOfHand (user)) {
      winner = USER;
   }

   return winner;
}