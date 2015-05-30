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
#define NUM_CARDS_IN_DECK 52

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
   int value2; //11 for ace otherwise 0
   Card next; // for my pack linked list
} card;

typedef struct _player {
   Card hand[MAX_CARDS];
   int numCards;
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
int getValueOfHand (Player currPlayer);
// gets the value of a player's hand 
void makeNextAIAction(Player dealer, Pack deck); //makes next move by AI
void revealHands (Player dealer, Player user);
// similar to displayHands except used at end to determine winner
// (it shows dealers' hand)
int determineWinner (Player dealer, Player user);
// determines winner based on current hand of dealer and player


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
   printf ("%s\n", deck->head->title);
   printf ("%s\n", deck->head->next->title);
   printf ("%s\n", deck->head->next->next->title);
   printf ("%s\n", deck->head->next->next->next->title);

   int gameRunning = TRUE;
   int bust = FALSE;

   dealHand (user, deck);
   dealHand (dealer, deck);
   revealHands (dealer, user);
   while (gameRunning == TRUE && bust == FALSE) {
      displayHands(dealer, user);
      int action = requestAction();
      if (action == HIT) {
         dealHand (user, deck);
         bust = ifBust(user);
      } else {
         // action == STAND
         makeNextAIAction (dealer, deck);
         bust = ifBust(dealer);
         gameRunning = FALSE;
      }

      if (gameRunning == TRUE) {
         displayHands(dealer, user);
      }
   }

   printf ("\n");
   if (bust == TRUE) {
      displayHands(dealer, user);
      printf ("BUST! You lose.\n");
   } else {
      revealHands(dealer, user);
      int winner = determineWinner(dealer, user);
      if (winner == NO_ONE) {
         printf ("It's a draw!\n");
      } 
      else if (winner == USER) {
         printf ("You beat the dealer!\n");
      } 
      else {
         // winner == DEALER
         printf ("You lost to the dealer!\n");
      }
   }

   

   return EXIT_SUCCESS;
}

// displays rules of the game
void displayRules (void) {
   char* message =
      "1. You will play against an AI dealer \n"
      "2. Card values:\n"
      "   Ace: 1 or 11\n"
      "   Jack, Queen, King: 10\n"
      "   Others: their respective numbers\n"
      "3. Each turn you can either hit (add a card to your hand)\n"
      "   or stand (keep your hand)\n"
      "4. The aim is to get as close as possible to 21 without going\n"
      "   over. If you go over 21 you instantly lose.";

      printf ("%s\n", message);
}

// mallocs memory and sets up a new player
Player newPlayer (void) {
   Player new = malloc (sizeof (struct _player));
   new->numCards = 0;

   return new;
}

// creates a new card for purposes of helping newPack function
Card newCard (char *title, int value) {
   Card new = malloc(sizeof(struct _card));
   strncpy (new->title, title, MAX_TITLE);
   new->value = value;

   if (value == 1) {
      // ace card
      new->value2 = 11;
   } else {
      new->value2 = 0;
   }

   new->next = NULL;

   return new;
}


//mallocs memory for an entire deck linked list
Pack newPack (void) {
   Pack new = malloc (sizeof (struct _pack));

   Card nextCard = newCard ("A ", 11);
   new->head = nextCard;

   Card currentCard = new->head;

   nextCard = newCard ("A ", 11); 
   currentCard->next = nextCard;;
   currentCard = currentCard->next;

   nextCard = newCard ("A ", 11);
   currentCard->next = nextCard;
   currentCard = currentCard->next;

   nextCard = newCard ("A ", 11);
   currentCard->next = nextCard;
   currentCard = currentCard->next;


   nextCard = newCard ("2 ", 2);
   currentCard->next = nextCard;
   currentCard = currentCard->next;

   nextCard = newCard ("2 ", 2);
   currentCard->next = nextCard;
   currentCard = currentCard->next;

   nextCard = newCard ("2 ", 2);
   currentCard->next = nextCard;
   currentCard = currentCard->next;

   nextCard = newCard ("2 ", 2);
   currentCard->next = nextCard;
   currentCard = currentCard->next;


   nextCard = newCard ("3 ", 3);
   currentCard->next = nextCard;
   currentCard = currentCard->next;

   nextCard = newCard ("3 ", 3);
   currentCard->next = nextCard;
   currentCard = currentCard->next;

   nextCard = newCard ("3 ", 3);
   currentCard->next = nextCard;
   currentCard = currentCard->next;

   nextCard = newCard ("3 ", 3);
   currentCard->next = nextCard;
   currentCard = currentCard->next;


   nextCard = newCard ("4 ", 4);
   currentCard->next = nextCard;
   currentCard = currentCard->next;

   nextCard = newCard ("4 ", 4);
   currentCard->next = nextCard;
   currentCard = currentCard->next;

   nextCard = newCard ("4 ", 4);
   currentCard->next = nextCard;
   currentCard = currentCard->next;

   nextCard = newCard ("4 ", 4);
   currentCard->next = nextCard;
   currentCard = currentCard->next;


   nextCard = newCard ("5 ", 5);
   currentCard->next = nextCard;
   currentCard = currentCard->next;

   nextCard = newCard ("5 ", 5);
   currentCard->next = nextCard;
   currentCard = currentCard->next;

   nextCard = newCard ("5 ", 5);
   currentCard->next = nextCard;
   currentCard = currentCard->next;

   nextCard = newCard ("5 ", 5);
   currentCard->next = nextCard;
   currentCard = currentCard->next;


   nextCard = newCard ("6 ", 6);
   currentCard->next = nextCard;
   currentCard = currentCard->next;

   nextCard = newCard ("6 ", 6);
   currentCard->next = nextCard;
   currentCard = currentCard->next;

   nextCard = newCard ("6 ", 6);
   currentCard->next = nextCard;
   currentCard = currentCard->next;

   nextCard = newCard ("6 ", 6);
   currentCard->next = nextCard;
   currentCard = currentCard->next;


   nextCard = newCard ("7 ", 7);
   currentCard->next = nextCard;
   currentCard = currentCard->next;

   nextCard = newCard ("7 ", 7);
   currentCard->next = nextCard;
   currentCard = currentCard->next;

   nextCard = newCard ("7 ", 7);
   currentCard->next = nextCard;
   currentCard = currentCard->next;

   nextCard = newCard ("7 ", 7);
   currentCard->next = nextCard;
   currentCard = currentCard->next;


   nextCard = newCard ("8 ", 8);
   currentCard->next = nextCard;
   currentCard = currentCard->next;

   nextCard = newCard ("8 ", 8);
   currentCard->next = nextCard;
   currentCard = currentCard->next;

   nextCard = newCard ("8 ", 8);
   currentCard->next = nextCard;
   currentCard = currentCard->next;

   nextCard = newCard ("8 ", 8);
   currentCard->next = nextCard;
   currentCard = currentCard->next;


   nextCard = newCard ("9 ", 9);
   currentCard->next = nextCard;
   currentCard = currentCard->next;

   nextCard = newCard ("9 ", 9);
   currentCard->next = nextCard;
   currentCard = currentCard->next;

   nextCard = newCard ("9 ", 9);
   currentCard->next = nextCard;
   currentCard = currentCard->next;

   nextCard = newCard ("9 ", 9);
   currentCard->next = nextCard;
   currentCard = currentCard->next;


   nextCard = newCard ("10 ", 10);
   currentCard->next = nextCard;
   currentCard = currentCard->next;

   nextCard = newCard ("10 ", 10);
   currentCard->next = nextCard;
   currentCard = currentCard->next;

   nextCard = newCard ("10 ", 10);
   currentCard->next = nextCard;
   currentCard = currentCard->next;

   nextCard = newCard ("10 ", 10);
   currentCard->next = nextCard;
   currentCard = currentCard->next;


   nextCard = newCard ("J ", 10);
   currentCard->next = nextCard;
   currentCard = currentCard->next;

   nextCard = newCard ("J ", 10);
   currentCard->next = nextCard;
   currentCard = currentCard->next;

   nextCard = newCard ("J ", 10);
   currentCard->next = nextCard;
   currentCard = currentCard->next;

   nextCard = newCard ("J ", 10);
   currentCard->next = nextCard;
   currentCard = currentCard->next;


   nextCard = newCard ("Q ", 10);
   currentCard->next = nextCard;
   currentCard = currentCard->next;

   nextCard = newCard ("Q ", 10);
   currentCard->next = nextCard;
   currentCard = currentCard->next;

   nextCard = newCard ("Q ", 10);
   currentCard->next = nextCard;
   currentCard = currentCard->next;

   nextCard = newCard ("Q ", 10);
   currentCard->next = nextCard;
   currentCard = currentCard->next;


   nextCard = newCard ("K ", 10);
   currentCard->next = nextCard;
   currentCard = currentCard->next;

   nextCard = newCard ("K ", 10);
   currentCard->next = nextCard;
   currentCard = currentCard->next;

   nextCard = newCard ("K ", 10);
   currentCard->next = nextCard;
   currentCard = currentCard->next;

   nextCard = newCard ("K ", 10);
   currentCard->next = nextCard;
   currentCard = currentCard->next;

   currentCard->next = NULL;

   // add ♠♣♦♥ to the title names
   int asciiCount = 6; // alternates between ASCII 
                       //value of of ♠(6), ♣(5), ♦(4), ♥(3)
   Card curr = new->head;
   while (curr != NULL) {
      if (curr->title[0] == '1') {
         // card 10 requires different treatment as 10 is two char's
         curr->title[3] = asciiCount;
         curr->title[4] = 0;
      } else {
         curr->title[2] = asciiCount;
         curr->title[3] = 0;
      }

      curr = curr->next;
      asciiCount--;

      if (asciiCount < 3) { // loops ASCII values of card signs
         asciiCount = 6;
      }
   }

   new->numCardsLeft = 52; //number of cards in deck

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

   if (getValueOfHand (currPlayer) > 21) {
      isHandBust = TRUE;
   }

   return isHandBust;
}

// gets the value of a player's hand 
int getValueOfHand (Player currPlayer) {
   int totalValue = 0;
   int count = 0;
   while (count < currPlayer->numCards) {
      totalValue += currPlayer->hand[count]->value;
      count++;
   }

   if (totalValue > 21) {
      // scan through to check if hand contains A as it will then
      // be forced from value 11 to 1
      count = 0;
      while (count < currPlayer->numCards && totalValue > 21) {
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
   int totalValue = getValueOfHand (dealer);

   int count = dealer->numCards;
   while (totalValue < 17) {
      // same as casino standards
      dealHand(dealer, deck);
      totalValue += dealer->hand[count]->value;
      count++;
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