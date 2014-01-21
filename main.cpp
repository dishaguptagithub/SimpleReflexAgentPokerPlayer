#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

using namespace std;

enum SUIT {CLUBS, DIAMONDS, HEARTS, SPADES};
char const * const FOLD = "fold";
char const * const CALL = "call";

struct Card
{
	int rank;
	SUIT suit;	
};

struct Hand
{
	Card one;
	Card two;
};

char suitToChar(const SUIT & suit)
{
	if (suit == CLUBS)
		return 'c';
	else if (suit == DIAMONDS)
		return 'd';
	else if (suit == HEARTS)
		return 'h';
	else if (suit == SPADES)
		return 's';

	cerr << "Error in suitToChar: passed suit = " << suit << endl;
	exit(1);
	return '\0';
}

bool is_pair(const Hand & hand)
{
	return (hand.one.rank == hand.two.rank)? true : false;
}

bool same_suit(const Hand & hand)
{
	return (hand.one.suit == hand.two.suit)? true : false;
}

int low_card_rank(const Hand & hand)
{
	if (hand.one.rank <= hand.two.rank)
		return hand.one.rank;
	return hand.two.rank;
}

int high_card_rank(const Hand & hand)
{
	if (hand.one.rank >= hand.two.rank)
		return hand.one.rank;
	return hand.two.rank;
}

// any XYs
bool agent1(const Hand & hand)
{
	if (same_suit(hand))
		return true;
	return false;
}

// 44+
bool agent2(const Hand & hand)
{
	if (is_pair(hand) && low_card_rank(hand) > 3)
		return true;
	return false;
}

// plays 10's+
bool agent3(const Hand & hand)
{
	if (same_suit(hand) && low_card_rank(hand) >= 10)
		return true;
	return false;
}

// plays all is_pairs 22+, all suited gappers and connectors 23s+, and all offsuit JQo+, and A10s, AJs
bool agent4(const Hand & hand)
{
	if ((same_suit(hand) && high_card_rank(hand) - low_card_rank(hand) <= 2) ||
	    (is_pair(hand)) ||
		(high_card_rank(hand) - low_card_rank(hand) == 1 && low_card_rank(hand) >= 10) ||
		(high_card_rank(hand) == 14 && low_card_rank(hand) >= 10 && same_suit(hand)))
		return true;
	return false;
}

void write(const Hand & hand, ofstream & fout, char const * const action)
{
	stringstream ss;

	ss << suitToChar(hand.one.suit);
	ss << hand.one.rank;
	ss << ",";
	ss << suitToChar(hand.two.suit);
	ss << hand.two.rank;
	ss << ",";
	ss << action;

	fout << ss.str() << endl;
}

int main(int argc, char * argv[])
{
	ofstream fout;

	for (int agent_num = 1; agent_num <= 4; agent_num++)
	{
		string file_name = "agent_" + std::to_string(agent_num) + "_output.txt";
		fout.open(file_name);

		if (!fout)
		{
			cerr << "Error opening agent file!" << endl;
			exit(1);
		}

		for (SUIT one_suit = CLUBS; one_suit <= SPADES; one_suit++)
		{
			for (int one_rank = 2; one_rank <= 14; one_rank++)
			{
				Hand hand;
				hand.one.rank = one_rank;
				hand.one.suit = one_suit;

				for (SUIT two_suit = CLUBS; two_suit <= SPADES; two_suit++)
				{
					for (int two_rank = 2; two_rank <= 14; two_rank++)
					{
						// all permutations are valid except for same suit same rank
						if (!(one_rank == two_rank && one_suit == two_suit))
						{
							bool decision = false;
							hand.two.rank = two_rank;
							hand.two.suit = two_suit;

							switch(agent_num)
							{
								case 1:
									decision = agent1(hand);
									break;

								case 2:
									decision = agent2(hand);
									break;

								case 3:
									decision = agent3(hand);
									break;

								case 4:
									decision = agent4(hand);
									break;

								default:
									cerr << "error." << endl;
									exit(1);
							}

							if (decision)
							{
								write(hand, fout, CALL);
							}
							else
							{
								write(hand, fout, FOLD);
							}
						}
					}
				}	
			}
		}	

		fout.close();
	}

	return 0;
}
