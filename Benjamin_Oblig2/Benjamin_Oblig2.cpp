/**********************************************************************************************************
 *      2nd obligatory assignment, BlackJack
 *
 *      A program that allows you to BlackJack against the "house"
 *
 *  @file    Benjamin_Oblig2.CPP
 *  @author  Benjamin A.E. Darici
 */

#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <ctime>
#include <string>
#include <vector>
#include <algorithm>
#include <conio.h>
#include <Windows.h>

//  A struct to represent cards
struct card {
    int value = 0;
    std::string name = "";
    std::string suit = "";
};

//  A struct to represent persons playing the game
struct person {
    int cardValue = 0,
        money = 100;
    std::vector<card*> hand;
};

//  A function that returns numbers within a min and max value
//      used for betting in this case
int betting(const char* c, int p, int h) {
    char input[200] = "";
    int  number = 0;
    bool wrong = false;
    int min = 0;
    int max;
    if(p>h) //  Figures out what the maximum betting amount can be
    {
        max = h;
    }
    else 
    {
        max = p;
    }

    do {
        wrong = false;
        std::cout << c << " (" << min << " - " << max << "):  ";
        std::cin.getline(input, 200);
        number = atoi(input);
        if (number == 0 && input[0] != '0')
        {
            wrong = true;   std::cout << "\nNot a number\n\n";
        }
    } while (wrong || number < min || number > max);

    return number;
}

//  A fuction that creates a simulated deck of cards
std::vector<card*> makeDeck() 
{
    std::vector<card*> newDeck;
    std::vector<std::string> cardName{"Ace", "Jack", "Queen", "King" };         //  Vector with names of certain cards
    std::vector<std::string> cardSuit{ "Heart", "Spade", "Diamond", "Club"};    //  Vector with card suits
    for (int i = 0; i < 4; i++) //  For-loop that assigns card suits
    {
        for (int j = 0; j < 10; j++)    //  For-loop that assigns card names and value
        {
            newDeck.push_back(new card);
            newDeck[j+13*i]->value = j+1;
            newDeck[j+13*i]->suit = cardSuit[i];
            newDeck[j + 13 * i]->name = std::to_string(j+1);
            if (j == 0)
            {
                newDeck[j + 13 * i]->name = cardName[0];
            }

            if ((j+1)%10==0)    //  Checks if we are at the end of the loop
            {
                for (int k = 1; k <= 3; k++)    //  Adds the royal cards with value 10
                {
                    newDeck.push_back(new card);
                    newDeck[j+k+13*i]->value = j+1;
                    newDeck[j + k + 13 * i]->suit = cardSuit[i];
                    newDeck[j + k + 13 * i]->name = cardName[k];
                }
            }
        }
    }
    return newDeck;
}

//  A function that shuffles the deck and returns the player and house's cards
void shuffleDeck(std::vector<card*>& deck, std::vector<card*>& ph, std::vector<card*>& hh) 
{
    for (int i = 0; i < ph.size(); i++) //  For all the cards in the player's hand...
    {
        deck.push_back(ph[i]);  //  Put them in the back of the deck
    }
    ph.clear(); //  Empty player's hand

    for (int i = 0; i < hh.size(); i++) //  For all the cards in the house's hand...
    {
        deck.push_back(hh[i]);  //  Put them in the back of the deck
    }
    hh.clear(); //  Empty house's hand
    std::srand(static_cast<unsigned int>(std::time(nullptr))); // set initial seed value to system clock
    std::random_shuffle(deck.begin(), deck.end());  //  Shuffles the card deck vector
}

//  Drawing cards from the deck
void draw(std::vector<card*>& h, std::vector<card*>& d )
{
    std::srand(static_cast<unsigned int>(std::time(nullptr))); // set initial seed value to system clock
    int draw = std::rand() % d.size();
    h.push_back(d[draw]);   //  Pushes the new card to the back of the hand
    d[draw] = d[d.size()-1];    //  Sets the drawn card to equal the card in the back of the deck
    d.pop_back();   //  Removes the card in the back of the deck
}

//  Several for-loops to draw the cards on the screen
void renderCards(const std::vector<card*>& h) {
    for (int i = 0; i < h.size(); i++)
    {
        std::cout << " ____________  ";
    }
    std::cout << std::endl;
    for (int i = 0; i < h.size(); i++)
    {
        std::cout << " |  " << std::right << std::setw(8) << h[i]->suit << "|  ";
    }
    std::cout << std::endl;
    for (int i = 0; i < h.size(); i++)
    {
        std::cout << " |" << std::left << std::setw(5) << h[i]->name << "     |  ";
    }
    std::cout << std::endl;
    for (int i = 0; i < 2; i++)
    {
        for (int i = 0; i < h.size(); i++)
        {
            std::cout << " |          |  ";
        }
        std::cout << std::endl;
    }
}

//  The game loop
void game(std::vector<card*>& deck, person& player, person& house, int p)
{
    player.cardValue = house.cardValue = 0;
    int bet{ 0 };
    char input{' '};

    draw(house.hand, deck); //  The house draws the first card
    if (house.hand.back()->value == 1)  //  Checks for ace and assigns an appropriate value
    {
        if (house.cardValue < 11) {
            house.cardValue += 11;
        }
        else
        {
            house.cardValue += 1;
        }
    }
    else
    {
        house.cardValue += house.hand.back()->value;
    }

    do
    {
        system("CLS");

        //  Draws "UI"
        std::cout << "House funds: $" << house.money << std::endl;
        renderCards(house.hand);
        std::cout << std::endl << std::endl << "Prize pool: $" << p << std::endl << std::endl;

        draw(player.hand, deck);
        renderCards(player.hand);
        std::cout << std::endl << "Player funds: $" << player.money << std::endl;

        //  Checks for ace, and asks player if they want to make it an 11
        if (player.hand.back()->value == 1)
        {
            std::cout << "Make it 11? Y/n" << std::endl;
            input = _getch();
            if (toupper(input) == 'Y')
            {
                player.cardValue += 11;
            }
            else
            {
                player.cardValue += 1;
            }
        }
        else
        {
            player.cardValue += player.hand.back()->value;
        }

        if (player.cardValue > 20) 
        {
            break;
        }
        
        //  Asks player ig the want to hold their cards or draw another
        std::cout << "[H]old?" << std::endl;
        input = _getch();      
    } while (toupper(input)!= 'H' && player.cardValue < 21);

    //  Checks if the player busts
    if (player.cardValue > 21)
    {   
        std::cout << "Bust!" << std::endl;
        house.money += p;
        return;
    }

    //  Takes bet from player and adds it to the prize pool
    bet = betting("How much do you wish to bet?", player.money, house.money);
    player.money -= bet;
    house.money -= bet;
    p += bet * 2;

    //  Essentially the same as the players turn loop, but automated
    do
    {
        Sleep(2000);
        system("CLS");
        draw(house.hand, deck);
        if (house.hand.back()->value == 1)
        {
            if (house.cardValue < 11) {
                house.cardValue += 11;
            }
            else
            {
                house.cardValue += 1;
            }
        }
        else
        {
            house.cardValue += house.hand.back()->value;
        }

        std::cout << "House funds: $" << house.money << std::endl;
        renderCards(house.hand);
        std::cout << std::endl << std::endl << "Prize pool: $" << p << std::endl << std::endl;
        renderCards(player.hand);
        std::cout << std::endl << "Player funds: $" << player.money << std::endl;
        
    } while (house.cardValue < player.cardValue);

    //  win checks:
    if (house.cardValue < player.cardValue || house.cardValue > 21) 
    {
        std::cout << "Player wins!" << std::endl;
        player.money += p;
        return;
    }
    else if (house.cardValue > player.cardValue) 
    {
        std::cout << "House wins!" << std::endl;
        house.money += p;
        return;
    }
    else {
        std::cout << "It's a tie! Play again." << std::endl;
        shuffleDeck(deck, player.hand, house.hand);
        game(deck, player, house, p);
    }
}
/*
     __
  -=(o '.    What spaghetti nonesne is this
     '.-.\
     /|  \\
     '|  ||
      _\_):,_
*/



//  Main function with menu
int main()
{
    int prizePool{ 20 };
    char command{ ' ' };
    person player;
    person house;
    std::vector<card*> deck = makeDeck();
    std::srand(static_cast<unsigned int>(std::time(nullptr))); // set initial seed value to system clock
    std::random_shuffle(deck.begin(), deck.end());

    while (toupper(command) != 'N' && player.money > 10 && house.money > 10)
    {
        player.money -= 10;
        house.money -= 10;
        game(deck, player, house, prizePool);
        if (house.money < 10) { //  If sentance to see if there is enough money for another round
            std::cout << "\nCan't play anymore, you robbed the house!";
            break;
        }
        else if (player.money < 10) {
            std::cout << "\nCan't play anymore, you are broke!";
            break;
        }
        std::cout << "Play another round of BlackJack? 10$ entry fee. y/N" << std::endl;
        command = _getch();
        shuffleDeck(deck, player.hand, house.hand);
    }
    std::cout << "\nWelcome back again!\n\n";
}