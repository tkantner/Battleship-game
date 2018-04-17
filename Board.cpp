#include "Board.h"
#include "Game.h"
#include "globals.h"
#include <iostream>

using namespace std;
class Ship;

class BoardImpl
{
public:
	BoardImpl(const Game& g);
	void clear();
	void block();
	void unblock();
	bool placeShip(Point topOrLeft, int shipId, Direction dir);
	bool unplaceShip(Point topOrLeft, int shipId, Direction dir);
	void display(bool shotsOnly) const;
	bool attack(Point p, bool& shotHit, bool& shipDestroyed, int& shipId);
	bool allShipsDestroyed() const;
private:
	char m_arr[MAXROWS][MAXCOLS];
	const Game& m_game;
};

BoardImpl::BoardImpl(const Game& g)
	: m_game(g)
{
	for (int i = 0; i < m_game.rows(); i++)
	{
		for (int j = 0; j < m_game.cols(); j++)
			m_arr[i][j] = '.';
	}
}

void BoardImpl::clear()
{
	for (int i = 0; i < m_game.rows(); i++)
	{
		for (int j = 0; j < m_game.cols(); j++)
			m_arr[i][j] = '.';
	}
}

void BoardImpl::block()
{
	// Block cells with 50% probability
	for (int r = 0; r < m_game.rows(); r++)
		for (int c = 0; c < m_game.cols(); c++)
			if (randInt(2) == 0)
			{
				m_arr[r][c] = '#';
			}
}

void BoardImpl::unblock()
{
	for (int r = 0; r < m_game.rows(); r++)
		for (int c = 0; c < m_game.cols(); c++)
		{
			if (m_arr[r][c] == '#')
				m_arr[r][c] = '.';
		}
} 

//Check to make sure that these are all good
bool BoardImpl::placeShip(Point topOrLeft, int shipId, Direction dir)
{
	bool answer = true;

	if (m_game.shipLength(shipId) == 0)  //If invalid id
		answer = false;

	if (topOrLeft.r < 0 || topOrLeft.c < 0 || topOrLeft.r >= m_game.rows() || topOrLeft.c >= m_game.cols()) //If outside the board
		answer = false;

	if (dir == HORIZONTAL) //If its outside the board
	{
		if (topOrLeft.c + m_game.shipLength(shipId) - 1 >= m_game.cols())
			answer = false;
	}
	else
	{
		if (topOrLeft.r + m_game.shipLength(shipId) - 1 >= m_game.rows())
			answer = false;
	}
	
	for (int r = 0; r < m_game.rows(); r++)  //If its been placed before
	{
		for (int c = 0; c < m_game.cols(); c++)
		{
			if (m_arr[r][c] == m_game.shipSymbol(shipId))
				answer = false;
			break;
		}
	}

	if (dir == HORIZONTAL) //If it overlaps
	{
		for (int i = 0; i < m_game.shipLength(shipId); i++)
		{
			if (m_arr[topOrLeft.r][topOrLeft.c + i] != '.')
			{
				answer = false;
				break;
			}
		}
	}
	else
	{
		for (int i = 0; i < m_game.shipLength(shipId); i++)
		{
			if (m_arr[topOrLeft.r + i][topOrLeft.c] != '.')
			{
				answer = false;
				break;
			}
		}
	}

	if (answer) //Only if valid, place on the board
	{
		if (dir == HORIZONTAL)
		{
			for (int i = 0; i < m_game.shipLength(shipId); i++)
			{
				m_arr[topOrLeft.r][topOrLeft.c + i] = m_game.shipSymbol(shipId);
			}
		}
		else
		{
			for (int i = 0; i < m_game.shipLength(shipId); i++)
				m_arr[topOrLeft.r + i][topOrLeft.c] = m_game.shipSymbol(shipId);	
		}
	}

	return answer; 
}

bool BoardImpl::unplaceShip(Point topOrLeft, int shipId, Direction dir)
{
	bool answer = true;

	if (m_game.shipLength(shipId) == 0)  //Invalid id
		answer = false;

	if (dir == HORIZONTAL)  //If its not all there
	{
		for (int i = 0; i < m_game.shipLength(shipId); i++)
			if (m_arr[topOrLeft.r][topOrLeft.c + i] != m_game.shipSymbol(shipId))
			{
				answer = false;
				break;
			}
	}
	else
	{
		for (int i = 0; i < m_game.shipLength(shipId); i++)
			if (m_arr[topOrLeft.r + i][topOrLeft.c] != m_game.shipSymbol(shipId))
			{
				answer = false;
				break;
			}
	}

	if (answer) //If its valid, lets clear dis bitch
	{
		for (int i = 0; i < m_game.rows(); i++)
		{
			for (int j = 0; j < m_game.cols(); j++)
			{
				if (m_arr[i][j] == m_game.shipSymbol(shipId))
					m_arr[i][j] == '.';
			}
		}
	}

	return answer;
}

void BoardImpl::display(bool shotsOnly) const
{
	cout << "  ";
	for (int i = 0; i < m_game.cols(); i++)  //Header line
		cout << i;
	cout << endl;

	for (int i = 0; i < m_game.rows(); i++)  //Each additional line
	{
		cout << i;
		cout <<" ";
		for (int j = 0; j < m_game.cols(); j++)
		{
			if (shotsOnly)   //If we dont want to see the ship symbols
			{
				if (isalpha(m_arr[i][j]) && m_arr[i][j] != 'X' && m_arr[i][j] != 'o')
					cout << '.';
				else
					cout << m_arr[i][j];
			}
			else
				cout << m_arr[i][j];
		}
		cout << endl;
	}
}

bool BoardImpl::attack(Point p, bool& shotHit, bool& shipDestroyed, int& shipId)
{
	bool answer = true;

	if (p.r < 0 || p.c < 0 || p.r >= m_game.rows() || p.c >= m_game.cols()) //If out of bounds
		answer = false;

	if (m_arr[p.r][p.c] == 'X' || m_arr[p.r][p.c] == 'o') //if already hit
		answer = false;

	if (answer)
	{
		if (m_arr[p.r][p.c] != '.') //if its not a dot, theres a ship
		{
			shotHit = true;
			char shipSymbol = m_arr[p.r][p.c];
			m_arr[p.r][p.c] = 'X';   //Update array

			bool doesntExist = true;
			for (int i = 0; i < m_game.rows(); i++)
			{
				for (int j = 0; j < m_game.cols(); j++)
				{
					if (m_arr[i][j] == shipSymbol)  //Check the whole thing to see if the ship is there
						doesntExist = false;
				}
			}
			if (doesntExist)  //If theres no others, we sunk dat bitch
			{
				shipDestroyed = true;
				for (int i = 0; i < m_game.nShips(); i++) {
					if (shipSymbol == m_game.shipSymbol(i))  //Loop through and find the right ship
					{
						shipId = i;
						break;
					}
				}
			}
			else
				shipDestroyed = false;	
		}
		else
		{
			shotHit = false;
			m_arr[p.r][p.c] = 'o';  //If its a miss
		}

	}

	return answer;
}

bool BoardImpl::allShipsDestroyed() const
{
	bool answer = true;

	for (int i = 0; i < m_game.rows(); i++)
	{
		for (int j = 0; j < m_game.cols(); j++) {
			if (m_arr[i][j] != 'X' && m_arr[i][j] != '.' && m_arr[i][j] != 'o') //If its not any of these, theres still ships out there
			{
				answer = false;
				break;
			}
		}
	}
	
	return answer;
}

//******************** Board functions ********************************

// These functions simply delegate to BoardImpl's functions.

Board::Board(const Game& g)
{
	m_impl = new BoardImpl(g);
}

Board::~Board()
{
	delete m_impl;
}

void Board::clear()
{
	m_impl->clear();
}

void Board::block()
{
	return m_impl->block();
}

void Board::unblock()
{
	return m_impl->unblock();
}

bool Board::placeShip(Point topOrLeft, int shipId, Direction dir)
{
	return m_impl->placeShip(topOrLeft, shipId, dir);
}

bool Board::unplaceShip(Point topOrLeft, int shipId, Direction dir)
{
	return m_impl->unplaceShip(topOrLeft, shipId, dir);
}

void Board::display(bool shotsOnly) const
{
	m_impl->display(shotsOnly);
}

bool Board::attack(Point p, bool& shotHit, bool& shipDestroyed, int& shipId)
{
	return m_impl->attack(p, shotHit, shipDestroyed, shipId);
}

bool Board::allShipsDestroyed() const
{
	return m_impl->allShipsDestroyed();
}
