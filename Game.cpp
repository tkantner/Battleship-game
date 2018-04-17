#include "Game.h"
#include "Board.h"
#include "Player.h"
#include "globals.h"
#include <iostream>
#include <string>
#include <cstdlib>
#include <cctype>
#include <vector>
#include <algorithm>

using namespace std;

class Ship;

class GameImpl
{
public:
	GameImpl(int nRows, int nCols);
	int rows() const;
	int cols() const;
	bool isValid(Point p) const;
	Point randomPoint() const;
	bool addShip(int length, char symbol, string name);
	int nShips() const;
	int shipLength(int shipId) const;
	char shipSymbol(int shipId) const;
	string shipName(int shipId) const;
	Player* play(Player* p1, Player* p2, Board& b1, Board& b2, bool shouldPause);
private:
	int g_rows;
	int g_cols;
	vector<Ship> g_Ships;
};

class Ship   //Ship class
{
public:
	Ship(int length, char symbol, string name, int id);
	int getLength() const { return s_length; };
	string getName() const { return s_name; };
	char getSymbol() const { return s_symbol; };
	int getShipID() const { return s_shipID; };
private:
	int s_length;
	string s_name;
	char s_symbol;
	int s_shipID;
};

Ship::Ship(int length, char symbol, string name, int id)
{
	s_length = length;
	s_symbol = symbol;
	s_name = name;
	s_shipID = id;
}



void waitForEnter()
{
	cout << "Press enter to continue: ";
	cin.ignore(10000, '\n');
}

GameImpl::GameImpl(int nRows, int nCols)
{
	g_rows = nRows;
	g_cols = nCols;
}

int GameImpl::rows() const
{
	return g_rows;
}

int GameImpl::cols() const
{
	return g_cols;
}

bool GameImpl::isValid(Point p) const
{
	return p.r >= 0 && p.r < rows() && p.c >= 0 && p.c < cols();
}

Point GameImpl::randomPoint() const
{
	return Point(randInt(rows()), randInt(cols()));
}

bool GameImpl::addShip(int length, char symbol, string name) 
{
	bool answer = true;

	if (length < 0 || (length > g_rows && length > g_cols)) //If two big or negative...
		answer = false;
	 
	if (symbol == 'X' || symbol == 'o' || symbol == '.')  //If any of the displays...
		answer = false;

	for (int i = 0; (g_Ships.begin() + i) != g_Ships.end(); i++)  //Cycle through Ships
	{
		if (symbol == g_Ships[i].getSymbol())  //Check to see if Symbol exists
		{
			answer = false;
			break;
		}
	}

	if (answer)  //If we can make a ship, add it to the vector
	{
		g_Ships.push_back(Ship(length, symbol, name, g_Ships.size()));
	}
	return answer;
}

int GameImpl::nShips() const
{
	return g_Ships.size();  //Return the size of the vector
}

int GameImpl::shipLength(int shipId) const
{
	int answer = 0;
	for (int i = 0; (g_Ships.begin() + i) != g_Ships.end(); i++)  //Cycle through Ships
	{
		if (shipId == g_Ships[i].getShipID())  //Find the right Ship
		{
			answer = g_Ships[i].getLength();
			break;
		}
	}
	return answer;
}

char GameImpl::shipSymbol(int shipId) const
{
	char answer = '~'; //If there is no ship with that ID
	for (int i = 0; (g_Ships.begin() + i) != g_Ships.end(); i++)  //Cycle through Ships
	{
		if (shipId == g_Ships[i].getShipID()) //Find right ID
		{
			answer = g_Ships[i].getSymbol();
			break;
		}
	}
	return answer;
}

string GameImpl::shipName(int shipId) const
{
	string answer;
	for (int i = 0; (g_Ships.begin() + i) != g_Ships.end(); i++)  //Cycle through Ships
	{
		if (shipId == g_Ships[i].getShipID()) //Find right ID
		{
			answer = g_Ships[i].getName();
			break;
		}
	}
	return answer;
}

Player* GameImpl::play(Player* p1, Player* p2, Board& b1, Board& b2, bool shouldPause)
{
	b1.clear();
	b2.clear();

	if (!p1->placeShips(b1))
		return nullptr;
	if (!p2->placeShips(b2))
		return nullptr;

	while (!b1.allShipsDestroyed() && !b2.allShipsDestroyed()) //While ships still exist
	{
		if (shouldPause)  //If Paused
			waitForEnter();

		if (p1->isHuman())
			b2.display(true);
		else
			b2.display(false);
	
		Point p = p1->recommendAttack();
		bool shotHit1 = false;
		bool shipDestroyed1 = false;
		int shipId1 = 10;

		bool result1 = b2.attack(p, shotHit1, shipDestroyed1, shipId1); //Attack
		p1->recordAttackResult(p, result1, shotHit1, shipDestroyed1, shipId1);
		p2->recordAttackByOpponent(p);

			if(result1)  //If attack was valid
			{
				if (shotHit1)  //if the shot was a success
				{
					if (!shipDestroyed1)  //If the ship wasn't destroyed
					{
						cout << p1->name() << " attacked (" << p.r << ",";
						cout << p.c << ") and hit something, resulting in: " << endl;
					}
					
					if (shipDestroyed1)  //If we destroyed a ship
					{
						cout << p1->name() << " attacked (" << p.r << ",";
						cout << p.c << ") and destroyed the " << shipName(shipId1) << ", resulting in: " << endl;
					}	
				}
				else  //If it missed
				{
					cout << p1->name() << " attacked (" << p.r << ",";
					cout << p.c << ") and missed, resulting in: " << endl;
				}
				if (p1->isHuman())
					b2.display(true);
				else
					b2.display(false);
			}
			else {  //If not valid
				cout << p1->name() << " wasted a shot at (" << p.r << ",";
				cout << p.c << ")." << endl;
			}


			if (shouldPause)  //If Paused
				waitForEnter();

			if (b2.allShipsDestroyed())
				break;

			if (p2->isHuman())
				b1.display(true);
			else
				b1.display(false);

			Point f = p2->recommendAttack();
			bool shotHit2 = false;
			bool shipDestroyed2 = false;
			int shipId2 = 10;
			
			bool result2 = b1.attack(f, shotHit2, shipDestroyed2, shipId2); //Attack
			p2->recordAttackResult(f, result2, shotHit2, shipDestroyed2, shipId2);
			p1->recordAttackByOpponent(p);

			if (result2)  //If attack was valid
			{
				if (shotHit2)  //if the shot was a success
				{
					if (!shipDestroyed2)  //If the ship wasn't destroyed
					{
						cout << p2->name() << " attacked (" << f.r << ",";
						cout << f.c << ") and hit something, resulting in: " << endl;
					}

					if (shipDestroyed2)  //If we destroyed a ship
					{
						cout << p2->name() << " attacked (" << f.r << ",";
						cout << f.c << ") and destroyed the " << shipName(shipId2) << ", resulting in: " << endl;
					}
				}
				else  //If it missed
				{
					cout << p2->name() << " attacked (" << f.r << ",";
					cout << f.c << ") and missed, resulting in: "<< endl;
				}
				if (p2->isHuman())
					b1.display(true);
				else
					b1.display(false);
			}
			else {  //If not valid
				cout << p2->name() << " wasted a shot at (" << f.r << ",";
				cout << f.c << ")." << endl;
			}
		}
	
	if (b1.allShipsDestroyed())
	{
		if (p2->isHuman())
			b2.display(false);

		return p2;
	}
	else
	{
		if (p1->isHuman())
			b1.display(false);
		return p1;
	}
}

//******************** Game functions *******************************

// These functions for the most part simply delegate to GameImpl's functions.
// You probably don't want to change any of the code from this point down.

Game::Game(int nRows, int nCols)
{
	if (nRows < 1 || nRows > MAXROWS)
	{
		cout << "Number of rows must be >= 1 and <= " << MAXROWS << endl;
		exit(1);
	}
	if (nCols < 1 || nCols > MAXCOLS)
	{
		cout << "Number of columns must be >= 1 and <= " << MAXCOLS << endl;
		exit(1);
	}
	m_impl = new GameImpl(nRows, nCols);
}

Game::~Game()
{
	delete m_impl;
}

int Game::rows() const
{
	return m_impl->rows();
}

int Game::cols() const
{
	return m_impl->cols();
}

bool Game::isValid(Point p) const
{
	return m_impl->isValid(p);
}

Point Game::randomPoint() const
{
	return m_impl->randomPoint();
}

bool Game::addShip(int length, char symbol, string name)
{
	if (length < 1)
	{
		cout << "Bad ship length " << length << "; it must be >= 1" << endl;
		return false;
	}
	if (length > rows() && length > cols())
	{
		cout << "Bad ship length " << length << "; it won't fit on the board"
			<< endl;
		return false;
	}
	if (!isascii(symbol) || !isprint(symbol))
	{
		cout << "Unprintable character with decimal value " << symbol
			<< " must not be used as a ship symbol" << endl;
		return false;
	}
	if (symbol == 'X' || symbol == '.' || symbol == 'o')
	{
		cout << "Character " << symbol << " must not be used as a ship symbol"
			<< endl;
		return false;
	}
	int totalOfLengths = 0;
	for (int s = 0; s < nShips(); s++)
	{
		totalOfLengths += shipLength(s);
		if (shipSymbol(s) == symbol)
		{
			cout << "Ship symbol " << symbol
				<< " must not be used for more than one ship" << endl;
			return false;
		}
	}
	if (totalOfLengths + length > rows() * cols())
	{
		cout << "Board is too small to fit all ships" << endl;
		return false;
	}
	return m_impl->addShip(length, symbol, name);
}

int Game::nShips() const
{
	return m_impl->nShips();
}

int Game::shipLength(int shipId) const
{
	assert(shipId >= 0 && shipId < nShips());
	return m_impl->shipLength(shipId);
}

char Game::shipSymbol(int shipId) const
{
	assert(shipId >= 0 && shipId < nShips());
	return m_impl->shipSymbol(shipId);
}

string Game::shipName(int shipId) const
{
	assert(shipId >= 0 && shipId < nShips());
	return m_impl->shipName(shipId);
}

Player* Game::play(Player* p1, Player* p2, bool shouldPause)
{
	if (p1 == nullptr || p2 == nullptr || nShips() == 0)
		return nullptr;
	Board b1(*this);
	Board b2(*this);
	return m_impl->play(p1, p2, b1, b2, shouldPause);
}


