//Name: Jack Rauch
//Dr. Steinberg
//COP3502 Spring 2022
//Programming Assignment 3 Skeleton

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

typedef struct card_s{
	int rank;    //number
	char * type; //type of card
	struct card_s * nextptr;
}card_t;

//function prototypes
void rules(); //display rules
int playRound(); //simulate round
card_t * openCardDeck(); //open the card deck and place into a linkedlist setup
card_t * insertBackSetup(card_t *node, char *name, int cardrank); //take card from orginial deck and place in back of linked list for setup of game
int empty(card_t * node); //check to see if linked list is empty
void cleanUp(card_t * head); //free memory to prevent memory leaks
int deckSize(card_t * head); //count number of nodes in the linked list
card_t * search(card_t * node, int spot); //search list for a specific spot in the card deck indexing is similar to array setup
card_t * copyCard(card_t * node); //make a deep copy of card
card_t * removeCard(card_t * node, int spot); //remove card from linkedlist
card_t * insertBackDeck(card_t *head, card_t *node); //place card at end of pile
int compareCard(card_t * cardp1, card_t * cardp2); //compare cards
card_t * moveCardBack(card_t *head); //place card at top of deck to the bottom of the deck

int main()
{
	int seed;
	printf("Enter Seed: ");
	scanf("%d", &seed);
	
	srand(seed); //seed set
	rules();
	
	int player; //1 or 2
	int result;
	
	printf("Would you like to be player 1 or 2?\n");
	printf("Enter 1 or 2: ");
	scanf("%d", &player); //choose player
	
	for(int game = 1; game <= 5; ++game) //simulate games
	{
		printf("Alright lets play game %d.\n", game);
		printf("Lets split the deck.\n");

		result = playRound(); //play game
		
		if((result == 1 && player == 1) || (result == 2 && player == 2)) //determine who won
			printf("You won game %d.\n", game);
		else
			printf("I won game %d.\n", game);
	}
	
	return 0;
}

void rules()
{
	printf("Welcome to the card game war!\n");
	printf("Here are the rules.\n");
	printf("You have a pile of cards and I have a pile of cards.\n");
	printf("We will each pull the top card off of our respective deck and compare them.\n");
	printf("The card with the highest number will win the round and take both cards.\n");
	printf("However if there is a tie, then we have to we have to place one card faced down and the next one faced up and compare the results.\n");
	printf("Winner of the tie, will grab all the cards out. However if it's a tie again, then we repeat the same action.\n");
	printf("Ready? Here we go!\n");
}

card_t * openCardDeck()
{
	FILE *fptr = fopen("deck.txt", "r");
	
	char *name = (char *) malloc(sizeof(char) * 20);
	
	if (name == NULL)
	{
		printf("Error in malloc...\n");
		exit(1);
	}
	
	card_t * head = NULL;
	
	int cardrank = 13;
	int tracker = 1;
	while(fscanf(fptr, "%s", name) == 1)
	{			
		strcat(name, " ");
		
		if(tracker % 5 == 1)
		{
			strcat(name, "of Clubs");
			head = insertBackSetup(head, name, cardrank);
		}
		else if(tracker % 5 == 2)
		{
			strcat(name, "of Diamonds");
			head = insertBackSetup(head, name, cardrank);
		}
		else if(tracker % 5 == 3)
		{
			strcat(name, "of Hearts");
			head = insertBackSetup(head, name, cardrank);
		}
		else if(tracker % 5 == 4)
		{
			strcat(name, "of Spades");
			head = insertBackSetup(head, name, cardrank);
			tracker = 0;
			--cardrank;
		}
		
		++tracker;
		
	}
	
	free(name);
	fclose(fptr);
	
	return head;
}

card_t * insertBackSetup(card_t *node, char *name, int cardrank)
{
	
    if(empty(node)) //check to see if list is empty
    {
		node = (card_t *) malloc(sizeof(card_t));
		
		if(empty(node)) //check to see if malloc worked
		{
			printf("Did not allocate head successfully...");
			printf("Program Terminating...\n");
			exit(1);
		}
		else //otherwise populate data of new head node
		{
			node->type = (char *) malloc(sizeof(char) * 20);
			
			if(node->type == NULL)
			{
				printf("Error with malloc...\n");
				exit(1);
			}
			
			strcpy(node->type, name);
			node->rank = cardrank;
			node->nextptr = NULL; //must make new tail nextptr NULL!!!
			
			return node;  //terminate
		}
      
    }
	
	//here we know that the list has at least one node
	
	card_t *head = node; //keep pointer to head since we will need to return this address
	
	while(node->nextptr != NULL) //traverse to tail
		node = node->nextptr;
	
	node->nextptr = (card_t *) malloc(sizeof(card_t)); 
	
	if(node->nextptr == NULL) //see if new tail was allocated successfully
	{
		printf("Did not allocate node successfully...");
		return head; //terminate if tail didn't get created
	}
	
	//populate new node
	node->nextptr->type = (char *) malloc(sizeof(char) * 20);
	
	if(node->nextptr->type == NULL)
	{
		printf("Error with malloc...\n");
		exit(1);
	}
	
	strcpy(node->nextptr->type, name);
	node->nextptr->rank = cardrank;
	node->nextptr->nextptr = NULL; //very important
			
	return head; //return head node since we need our starting point of the linked list
}

int empty(card_t * node)
{
	return (node == NULL); //return condition result
}

int deckSize(card_t * head) //count size of deck by iterating and upping the counter each time a node is passed
{
	card_t * node = head;
	int count = 0;
	while(node != NULL) //iteration statement
	{
		count++;
		node = node->nextptr;

	}

	return count;
}

int compareCard(card_t * cardp1, card_t * cardp2) //compares the cards and returns an integer value
{
	if(cardp1->rank > cardp2->rank) //player 1 wins
	{
		return 1;
	}
	if(cardp1->rank < cardp2->rank) //player 2 wins
	{
		return 2;
	}
	if(cardp1->rank == cardp2->rank) //LETS GET READY TO RUMMMMMMMMMBLLLLLLEEEEEEEEEEEEEEEE 
	{
		return 0;
	}
}

card_t * search(card_t * node, int spot) //search for a card in a deck
{
	card_t * temp = node;

	for(int i = 0; i < spot; i++) //iterates based on the spot that is passed into it
	{
		temp = temp->nextptr;
	}

	return temp;
	

}

card_t * moveCardBack(card_t *head) //moves a card to the back of its deck (no way!)
{

	card_t * temp = head;
	card_t * temp2 = head;



	while(temp->nextptr != NULL) //iterates until end
	{
		temp = temp->nextptr;
	}
	head = head->nextptr; //sets head to heads next pointer
	temp->nextptr = temp2; //sets temp = to the head of the list (temp2)
	temp2->nextptr = NULL; //cuts the connection and makes temp2 the last node

	return head;

}

card_t * insertBackDeck(card_t *head, card_t *node) //no way! this function inserts a card into the back of another deck!
{

	card_t * temp = head;

	while(temp->nextptr != NULL) //iterates until  almost end
	{
		temp = temp->nextptr;
	}
	temp->nextptr = node; //node is put in the last position and the second to last node now points to it
	node->nextptr = NULL; //node points to NULL because it is at the end
	return head;
}

card_t * removeCard(card_t * node, int spot) //you guessed! this function removes a card!
{
    if(empty(node)) //cant remove a card if the deck is empty
	{
        return NULL;
	}
    card_t * head = node;
    card_t* tempPrev = search(node, spot-1); //these iterate to get the nodes in the right position to move the card and keep the list intact
    card_t * temp = search(node, spot);
    if(temp == NULL)
	{
        return node; //if the node does not exist you cant delete it
	}
    if(node == temp)
    {
        node = node->nextptr; //once again helps keep the lists intact
        free(temp); //deletes node
        return node;
    }
    tempPrev->nextptr = temp->nextptr; //keeps list intact again
    free(temp); //deletes node
    return node;


}

card_t * copyCard(card_t * node) //will copy the card
{
	card_t * temp = node;
	card_t * node2 = (card_t *) malloc(sizeof(card_t)); //making memory for new card
	node2->rank = node->rank;
	node2->type = node->type;		//setting all data from old node equal to data in new node
	node2->nextptr = node->nextptr;

	return node2;

}


void cleanUp(card_t * head) //will clean up and free memory
{
	card_t * temp = head;
	while(head != NULL)
	{	
		temp = head;
		head = head->nextptr;
		free(temp);
	}
}

int playRound()
{

    card_t* player1Deck = NULL; //initialize the decks 
    card_t* player2Deck = NULL;

    card_t* deck = openCardDeck(); //creates deck to shuffle / deal
	int currentPlayer = 2; //used for shuffling puproses
	printf("The deck has %d cards.\n", deckSize(deck));

    while(deckSize(deck) > 0) //while the deck has cards
    {
        
		int size = deckSize(deck);
        int spot = rand() % (size);			//picks a random spot in deck based on size
        card_t* card = copyCard(search(deck, spot)); //copies the card that is found based on spot
		if(currentPlayer == 1)
		{
			if(player1Deck == NULL)	//if the deck is empty this is the first card
			{
				player1Deck = card;
				player1Deck->nextptr = NULL; //basic stuff to create a linked list
				currentPlayer = 2;
			}
			else
			{
				player1Deck = insertBackDeck(player1Deck, card); //if the deck is not empty insert it to the back of the deck
				currentPlayer = 2; //set to other player to be delt to
			}
		}

		else if(currentPlayer == 2)
		{
			if(player2Deck == NULL)
			{
				player2Deck = card;					//this is the same exact thing as above just changed for player 2
				player2Deck->nextptr = NULL;
				currentPlayer = 1;
			}
			else
			{
				player2Deck = insertBackDeck(player2Deck, card);
				currentPlayer = 1;
			}
		}	
		deck = removeCard(deck, spot); //remove the card from the deck


    }
	


	while(deckSize(player1Deck) != 0 && deckSize(player2Deck) != 0) //while at least one deck isnt emopty, play the game!
	{
	    printf("Player 1 pulled out %s. 	 Player 2 pulled out %s.\n", player1Deck->type, player2Deck->type); //print the cards that are pulled out
		if(deckSize(player1Deck) == 1 || deckSize(player2Deck) == 1) //this was created to account for a one card scenario, actually saved my life LOL
		{
			int result = compareCard(player1Deck, player2Deck); 
			if(result == 1)
			{
				return 1;
			}
			if(result == 2)
			{
				return 2;
			}



		}
		int result = compareCard(player1Deck, player2Deck); //result is the integer returned by the compareCard function as described above
		if(result == 0) //LETS GET READY TO RUMMMMMMBBBLLLLLLEEEEEEEEEEEEEEEEEEEE
		{
		    card_t * wardeck1 = player1Deck; //make "war decks" to serve as placeholders
		    card_t * wardeck2 = player2Deck;
			while(compareCard(wardeck1, wardeck2) == 0) //while there is still a tie, keep running
			{
			    if(deckSize(wardeck1) == 1 || deckSize(wardeck2) == 1)
			    {
					wardeck1 = player1Deck;
					wardeck2 = player2Deck;
			    }

			    else if(deckSize(wardeck1) == 2 || deckSize(wardeck2) == 2)			//these 3 statements are to account for all 3 types of war scenarios (1 card, 2 card, and 3 card minimum)
			    {
			        wardeck1 = wardeck1->nextptr;
			        wardeck2 = wardeck2->nextptr;
			    }
			    else if(deckSize(wardeck1) > 2 && deckSize(wardeck2) > 2)
			    {
			        wardeck1 = wardeck1->nextptr->nextptr;
			        wardeck2 = wardeck2->nextptr->nextptr;
			        
			    }
			    printf("Ugh oh...We have a tie! W-A-R!\n");
				printf("Player 1 pulled out %s. 	 Player 2 pulled out %s.\n", wardeck1->type, wardeck2->type); //print out the cards that are being compared
			    if(compareCard(wardeck1, wardeck2) == 1)
			    {
					card_t * temp1 = player1Deck;
			        printf("Player 1 won that W-A-R!\n");
			        {
						while(temp1 != wardeck1)	//while the pointer at the head of the deck is not equal to the wardeck placeholder...
						{
							if(temp1->nextptr == wardeck1)
							{
								break; //if it reaches, break
							}
							player1Deck = moveCardBack(player1Deck);		//1 1 2 2 
							player1Deck = moveCardBack(player1Deck);	//cards going back into player 1's deck
							player1Deck = insertBackDeck(player1Deck, copyCard(player2Deck));	//properly inserts all the cards into the winner's deck!
							player2Deck = removeCard(player2Deck, 0); //make sure to remove the card from its original deck!!!!!
							player1Deck = insertBackDeck(player1Deck, copyCard(player2Deck));
							player2Deck = removeCard(player2Deck, 0);
							temp1 = player1Deck;
						}
			        }
			        
			

							// 1 2
					
						player1Deck = moveCardBack(player1Deck);
			        	player1Deck = insertBackDeck(player1Deck, copyCard(player2Deck)); //inserts the last cards that were used originally in the first war
			        	player2Deck = removeCard(player2Deck, 0);
					

			        break;
			    
				}
		    	else if(compareCard(wardeck1, wardeck2) == 2)
			    {
					card_t * temp2 = player2Deck;
			        printf("Player 2 won that W-A-R!\n");
			        {

			            while(temp2 != wardeck2)
						{
							if(temp2->nextptr == wardeck2)
							{
								break;
							}
							player2Deck = insertBackDeck(player2Deck, copyCard(player1Deck));
							player1Deck = removeCard(player1Deck, 0);	//cards going back into player 2's deck
							
							player2Deck = insertBackDeck(player2Deck, copyCard(player1Deck));
							player1Deck = removeCard(player1Deck, 0);
							
							player2Deck = moveCardBack(player2Deck);
							player2Deck = moveCardBack(player2Deck);
							temp2 = player2Deck;
						}
			            
																						//THIS IS LITERALLY THE EXACT SAME THING AS DESCRIBED ABOVE JUST REARRANGED FOR PLAYER 2
			            
			        }
			

					
			    		player2Deck = insertBackDeck(player2Deck, copyCard(player1Deck));
						player1Deck = removeCard(player1Deck, 0);
						player2Deck = moveCardBack(player2Deck);
					
				
					break;
				}
				
			    
			}
			
			
			
		
		    printf("Player 1 has %d cards.\n", deckSize(player1Deck)); //prints the decksize of each player!
		    printf("Player 2 has %d cards.\n", deckSize(player2Deck));
        }
		
		else if(result == 1)
		{
			printf("Player 1 won that round.\n");
			player1Deck = moveCardBack(player1Deck);
			player1Deck = insertBackDeck(player1Deck, copyCard(player2Deck));	//if player one wins, insert the cards as described by the rules
			player2Deck = removeCard(player2Deck, 0);
			printf("Player 1 has %d cards.\n", deckSize(player1Deck));
			printf("Player 2 has %d cards.\n", deckSize(player2Deck));

		}
		else if(result == 2)
		{
			printf("Player 2 won that round.\n");
			player2Deck = moveCardBack(player2Deck);
			player2Deck = insertBackDeck(player2Deck, copyCard(player1Deck));	//if player two wins, insert the cards as described by the rules
			player1Deck = removeCard(player1Deck, 0);
			printf("Player 1 has %d cards.\n", deckSize(player1Deck));
			printf("Player 2 has %d cards.\n", deckSize(player2Deck));

		}
		

	}

	if(deckSize(player1Deck) == 0) //if player 1 has no more cards left, player 2 wins!!
	{
		printf("Player 2 won that round.\n");
		printf("Player 1 has %d cards.\n", deckSize(player1Deck));
		printf("Player 2 has %d cards.\n", deckSize(player2Deck));
    	cleanUp(player1Deck);
    	cleanUp(player2Deck);
		return 2;
	}
	else if(deckSize(player2Deck) == 0) //if player 2 has no more cards left, player 1 wins!!
	{
		printf("Player 1 won that round.\n");
		printf("Player 1 has %d cards.\n", deckSize(player1Deck));
		printf("Player 2 has %d cards.\n", deckSize(player2Deck));
    	cleanUp(player1Deck);
    	cleanUp(player2Deck);
    	return 1;
	}

	return 69;

}


