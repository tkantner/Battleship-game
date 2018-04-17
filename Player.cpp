#include "Player.h"
#include "Board.h"
#include "Game.h"
#include "globals.h"
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cmath>

using namespace std;

//*********************************************************************
//  AwfulPlayer
//*********************************************************************

class AwfulPlayer : public Player
{
public:
	AwfulPlayer(string nm, const Game& g);
	virtual bool placeShips(Board& b);
	virtual Point recommendAttack();
	virtual void recordAttackResult(Point p, bool validShot, bool shotHit,
		bool shipDestroyed, int shipId);
	virtual void recordAttackByOpponent(Point p);
private:
	Point m_lastCellAttacked;
};

AwfulPlayer::AwfulPlayer(string nm, const Game& g)
	: Player(nm, g), m_lastCellAttacked(0, 0)
{}

bool AwfulPlayer::placeShips(Board& b)
{
	// Clustering ships is bad strategy
	for (int k = 0; k < game().nShips(); k++)
		if (!b.placeShip(Point(k, 0), k, HORIZONTAL))
			return false;
	return true;
}

Point AwfulPlayer::recommendAttack()
{
	if (m_lastCellAttacked.c > 0)
		m_lastCellAttacked.c--;
	else
	{
		m_lastCellAttacked.c = game().cols() - 1;
		if (m_lastCellAttacked.r > 0)
			m_lastCellAttacked.r--;
		else
			m_lastCellAttacked.r = game().rows() - 1;
	}
	return m_lastCellAttacked;
}

void AwfulPlayer::recordAttackResult(Point /* p */, bool /* validShot */,
	bool /* shotHit */, bool /* shipDestroyed */,
	int /* shipId */)
{
	//Does nothing
}

void AwfulPlayer::recordAttackByOpponent(Point /* p */)
{
	// AwfulPlayer completely ignores what the opponent does
}

//*********************************************************************
//  HumanPlayer
//*********************************************************************

bool getLineWithTwoIntegers(int& r, int& c)
{
	bool result(cin >> r >> c);
	if (!result)
		cin.clear();  // clear error state so can do more input operations
	cin.ignore(10000, '\n');
	return result;
}

class HumanPlayer : public Player
{
public:
	HumanPlayer(string nm, const Game&g) : Player(nm, g) {};
	virtual bool isHuman() const { return true; };
	~HumanPlayer() {};
	virtual bool placeShips(Board& b);
	virtual Point recommendAttack();
	virtual void recordAttackResult(Point p, bool validShot, bool shotHit,
		bool shipDestroyed, int shipId);
	virtual void recordAttackByOpponent(Point p);

};

bool HumanPlayer::placeShips(Board& b)
{
	cout << name() << " must place 5 ships." << endl;
	bool answer = true;
	int r, c;
	Direction d;
	for (int i = 0; i < 5; i++)
	{
		b.display(false);
		char choice;
		switch (i)
		{
		case 0:
			cout << "Enter h or v for direction of aircraft carrier (length 5): ";
			cin >> choice;
			if (choice == 'h')
				d = HORIZONTAL;
			else if (choice == 'v')
				d = VERTICAL;
			else
			{
				cout << "Please enter a valid direction:" << endl;
				i--;
				break;
			}
			cout << "Enter row and column of leftmost cell(e.g. 3 5) :" << endl;
			getLineWithTwoIntegers(r, c);
			if (!b.placeShip(Point(r, c), i, d))
			{
				cout << "Please select a valid location" << endl;
				i--;
			}
			break;
		case 1:
			cout << "Enter h or v for direction of battleship (length 4): ";
			cin >> choice;
			if (choice == 'h')
				d = HORIZONTAL;
			else if (choice == 'v')
				d = VERTICAL;
			else
			{
				cout << "Please enter a valid direction:" << endl;
				i--;
				break;
			}
			cout << "Enter row and column of leftmost cell(e.g. 3 5) :" << endl;
			getLineWithTwoIntegers(r, c);
			if (!b.placeShip(Point(r, c), i, d))
			{
				cout << "Please select a valid location" << endl;
				i--;
			}
			break;
		case 2:
			cout << "Enter h or v for direction of destroyer (length 3): ";
			cin >> choice;
			if (choice == 'h')
				d = HORIZONTAL;
			else if (choice == 'v')
				d = VERTICAL;
			else
			{
				cout << "Please enter a valid direction:" << endl;
				i--;
				break;
			}
			cout << "Enter row and column of leftmost cell(e.g. 3 5) :" << endl;
			getLineWithTwoIntegers(r, c);
			if (!b.placeShip(Point(r, c), i, d))
			{
				cout << "Please select a valid location" << endl;
				i--;
			}
			break;
		case 3:
			cout << "Enter h or v for direction of submarine (length 3): ";
			cin >> choice;
			if (choice == 'h')
				d = HORIZONTAL;
			else if (choice == 'v')
				d = VERTICAL;
			else
			{
				cout << "Please enter a valid direction:" << endl;
				i--;
				break;
			}
			cout << "Enter row and column of leftmost cell(e.g. 3 5) :" << endl;
			getLineWithTwoIntegers(r, c);
			if (!b.placeShip(Point(r, c), i, d))
			{
				cout << "Please select a valid location" << endl;
				i--;
			}
			break;
		case 4:
			cout << "Enter h or v for direction of patrol boat (length 2): ";
			cin >> choice;
			if (choice == 'h')
				d = HORIZONTAL;
			else if (choice == 'v')
				d = VERTICAL;
			else
			{
				cout << "Please enter a valid direction:" << endl;
				i--;
				break;
			}
			cout << "Enter row and column of leftmost cell(e.g. 3 5) :" << endl;
			getLineWithTwoIntegers(r, c);
			if (!b.placeShip(Point(r, c), i, d))
			{
				cout << "Please select a valid location" << endl;
				i--;
			}
			break;
		}
	}
	return answer;
} 

Point HumanPlayer::recommendAttack() 
{
	int r, c;
	cout << "Enter the row and column to attack (e.g, 3 5): " << endl;
	getLineWithTwoIntegers(r, c);
	return Point(r, c);
}

void HumanPlayer::recordAttackResult(Point p, bool validShot, bool shotHit, bool shipDestroyed, int shipId)
{
	//Nothing
}

void HumanPlayer::recordAttackByOpponent(Point p)
{
	//Nothing
}

//*********************************************************************
//  MediocrePlayer
//*********************************************************************

// TODO:  You need to replace this with a real class declaration and
//        implementation.

// Remember that Mediocre::placeShips(Board& b) must start by calling
// b.block(), and must call b.unblock() just before returning.

class MediocrePlayer : public Player
{
public:
	MediocrePlayer(string nm, const Game& g) : Player(nm, g), state1(true), numShots(0)
	{
		for (int i = 0; i < 100; i++)
		{
			m_shots[i].r = 69;
			m_shots[i].c = 69;
		}
	};
	~MediocrePlayer() {};
	virtual bool placeShips(Board& b);
	virtual Point recommendAttack();
	virtual void recordAttackResult(Point p, bool validShot, bool shotHit,
		bool shipDestroyed, int shipId);
	virtual void recordAttackByOpponent(Point p);
	bool mediocreHelper(Board&b, int shipid, int r, int c , Direction lastd);
	virtual bool getState() const { return state1; };
	virtual void changeState(bool state) { state1 = state; };
	virtual Point getLastPt() const { return lstStateChg; };
	virtual void chngLastPt(Point p) { lstStateChg.r = p.r; lstStateChg.c = p.c; };
	virtual bool alreadyShot(Point p);
	virtual void addPoint(Point p);
private:
	bool state1;
	Point m_shots[100];
	Point lstStateChg;
	int numShots;
};

bool MediocrePlayer::alreadyShot(Point p)
{ 
	if (p.r >= game().rows() || p.c >= game().cols() || p.r < 0 || p.c < 0) //Or out of bounds
		return true;

	for (int i = 0; i < numShots; i++)
	{
		if (p.r == m_shots[i].r && p.c == m_shots[i].c)
			return true;
	}
	return false;
}

void MediocrePlayer::addPoint(Point p)
{
	m_shots[numShots].r = p.r;
	m_shots[numShots].c = p.c;
}

bool MediocrePlayer::mediocreHelper(Board&b, int shipid, int r, int c, Direction lastd)
{

	if (shipid == game().nShips() - 1) //Base case 2: we're at the last ship
	{
		for (int r = 0; r < game().rows(); r++)  //Loop through the points
		{
			for (int c = 0; c < game().cols(); c++)
			{
				if (b.placeShip(Point(r, c), shipid, HORIZONTAL)) //Try to place horizontally
					return true;
				if (b.placeShip(Point(r, c), shipid, VERTICAL))  //Try to place vertically
					return true;
			}

		}
	}


	int r_og = r;
	int c_og = c;
	bool success = false;

	if (shipid < game().nShips() - 1)
	{
		for (int r = 0; r < game().rows(); r++)  //Loop through the points
		{
			for (int c = 0; c < game().cols(); c++)
			{
				if (b.placeShip(Point(r, c), shipid, HORIZONTAL)) //Try to place horizontally
				{
					lastd = HORIZONTAL;
					success = true;
					break;
				}
				if (b.placeShip(Point(r, c), shipid, VERTICAL))  //Try to place vertically
				{
					lastd = VERTICAL;
					success = true;
					break;
				}
			}
			if (success)
				break;

		}
	}

if (success) //If successful, lets move on
return mediocreHelper(b, shipid + 1, 0, 0, lastd);

else if (!success && shipid > 0) //If not successful, lets backtrack
{
	b.unplaceShip(Point(r_og, c_og), shipid - 1, lastd);
	return mediocreHelper(b, shipid - 1, r_og, c_og, lastd);
}

else  //Otherwise its the first ship and we fucked bro
	return false;


}

bool MediocrePlayer::placeShips(Board& b)
{
	int count = 0;
	while (count < 50)
	{
		b.block();
		if (mediocreHelper(b, 0, 0, 0, VERTICAL))
		{
			b.unblock();
			return true;
		}
		else
			count++;
		b.unblock();
	}
	b.unblock();
	return false;
}

Point MediocrePlayer::recommendAttack()
{
	int x = randInt(4) + 1;
	Point ans(1, 1);
	int rand1 = randInt(2);
	int rand2 = randInt(2);
	int count = 0;

	if (!getState()) //If theres no options left and ship length is greater than 5
	{
		for (int i = 1; i < 5; i++)
		{
			if (!alreadyShot(Point(getLastPt().r + i, getLastPt().c)))
				break;
			if (!alreadyShot(Point(getLastPt().r - i, getLastPt().c)))
				break;
			if (!alreadyShot(Point(getLastPt().r, getLastPt().c + i)))
				break;
			if (!alreadyShot(Point(getLastPt().r, getLastPt().c - i)))
				break;
			
			count++;
		}
		if (count == 4)
			changeState(true);
	}

	if (getState())
	{
	 Point rand(randInt(game().rows()), randInt(game().cols()));
	 if (alreadyShot(rand))
		 return recommendAttack();
	 else
		 return rand;
	}
	else
	{

		if (rand1 == 0) // Horizontal
		{
			if (rand2 == 1)
			{
				ans.r = getLastPt().r;
				ans.c = getLastPt().c + x;
			}
			else
			{
				ans.r = getLastPt().r;
				ans.c = getLastPt().c - x;
			}
		}
		else
		{

			if (rand2 == 0)
			{
				ans.r = getLastPt().r + x;
				ans.c = getLastPt().c;
			}
			else
			{
				ans.r = getLastPt().r - x;
				ans.c = getLastPt().c;
			}

		}
		
		if (alreadyShot(ans))  //If already shot or out of bounds
			return recommendAttack();
		else
			return ans;
	}

}   //fix this

void MediocrePlayer::recordAttackResult(Point p, bool validShot, bool shotHit, bool shipDestroyed, int shipId)
{
	if (!validShot) //If it was invalid, we don't care
		return;

	if (!alreadyShot(p))
	{
		addPoint(p);
		numShots++;
	}
	

	if (shotHit && getState() == true)
	{
		chngLastPt(p);
		changeState(false);  //If we hit a ship, turn to state 2

	}
	
	if (shipDestroyed)
		changeState(true);  //If we destroy a ship, back to state 1

	return;

}

void MediocrePlayer::recordAttackByOpponent(Point p)
{
	//Does Nothing...
}

//*********************************************************************
//  GoodPlayer
//*********************************************************************

class GoodPlayer : public Player
{
public:
	GoodPlayer(string nm, const Game& g) : Player(nm, g), state1(true), numShots(0), hitSinceLstChng(0)
	{
		for (int i = 0; i < (MAXROWS * MAXROWS); i++)
		{
			m_shots[i].r = 69;
			m_shots[i].c = 69;
		}
		for (int i = 0; i < game().rows(); i++)
		{
			for (int j = 0; j < game().cols(); j++)
				myShips[i][j] = '.';
		}
	};
	~GoodPlayer() {};
	virtual bool placeShips(Board& b);
	virtual Point recommendAttack();
	virtual void recordAttackResult(Point p, bool validShot, bool shotHit,
		bool shipDestroyed, int shipId);
	virtual void recordAttackByOpponent(Point p);
	virtual bool getState() const { return state1; };
	virtual void changeState(bool state) { state1 = state; };
	virtual Point getLastPt() const { return lstStateChg; };
	virtual bool alreadyShot(Point p);
	virtual void addPoint(Point p);
	bool goodHelper(Board&b, int shipid, int count, int track);
	bool checkShips(Point p, Direction d, int shipID);
	void recordShip(Point p, Direction d, int shipID);
	bool closeBy(Point p);
private:
	bool state1;
	Point m_shots[MAXCOLS * MAXROWS];
	Point lstStateChg;
	int numShots;
	vector<Point> hitShots;
	int hitSinceLstChng;
	char myShips[MAXROWS][MAXCOLS];
};

enum nextShot { LEFT, RIGHT, UP, DOWN };

bool GoodPlayer::closeBy(Point p)
{
	for (int i = 0; i < numShots; i++)
	{
			if (p.r + 1 == m_shots[i].r && p.c == m_shots[i].c)
				return true;
			if (p.r - 1 == m_shots[i].r && p.c == m_shots[i].c)
				return true;
			if (p.r == m_shots[i].r && p.c + 1 == m_shots[i].c)
				return true;
			if (p.r == m_shots[i].r && p.c - 1 == m_shots[i].c)
				return true;
	}
	return false;
}

void GoodPlayer::recordShip(Point p, Direction d, int shipID) //Places our ships in a grid to track it
{
	if (d == HORIZONTAL)
	{
		for (int i = 0; i < game().shipLength(shipID); i++)
		{
			myShips[p.r][p.c + i] = 'S';
		}
	}
	else
	{
		for (int i = 0; i < game().shipLength(shipID); i++)
		{
			myShips[p.r + i][p.c] = 'S';
		}
	}

	return;
}

bool GoodPlayer::checkShips(Point p, Direction d, int shipID)
{
	if (d == HORIZONTAL)
	{
		for (int i = 0; i < game().shipLength(shipID); i++)
		{
			if (game().isValid(Point(p.r + 2, p.c + i)))
			{
				if (myShips[p.r + 2][p.c + i] == 'S' || myShips[p.r + 1][p.c + i] == 'S')
					return true;
			}
			if (game().isValid(Point(p.r - 2, p.c + i)))
			{
				if (myShips[p.r - 2][p.c + i] == 'S' || myShips[p.r - 1][p.c + i] == 'S')
					return true;
			}
		}

		if (game().isValid(Point(p.r , p.c + game().shipLength(shipID))))
		{
			if (myShips[p.r][p.c + game().shipLength(shipID)] == 'S')
				return true;
		}
		if (game().isValid(Point(p.r, p.c - 2)))
		{
			if (myShips[p.r][p.c - 2] == 'S' || myShips[p.r][p.c - 1] == 'S')
				return true;
		}
		if (game().isValid(Point(p.r, p.c -1)))
			if (myShips[p.r][p.c - 1] == 'S')
				return true;
		if (game().isValid(Point(p.r, p.c + game().shipLength(shipID) + 1)))
			if (myShips[p.r][p.c + game().shipLength(shipID) + 1] == 'S')
				return true;
	}
	else
	{
		for (int i = 0; i < game().shipLength(shipID); i++)
		{
			if (game().isValid(Point(p.r + i, p.c + 2)))
			{
				if (myShips[p.r + i][p.c + 2] == 'S' || myShips[p.r + i][p.c + 1] == 'S')
					return true;
			}
			if (game().isValid(Point(p.r + i, p.c - 2)))
			{
				if (myShips[p.r + i][p.c - 2] == 'S')
					return true;
			}
		}
		if (game().isValid(Point(p.r + game().shipLength(shipID), p.c)))
		{
			if (myShips[p.r + game().shipLength(shipID)][p.c] == 'S')
				return true;
		}
		if (game().isValid(Point(p.r - 2, p.c)))
		{
			if (myShips[p.r - 2][p.c] == 'S' || myShips[p.r - 1][p.c] == 'S')
				return true;
		}
		if (game().isValid(Point(p.r - 2, p.c)))
			if (myShips[p.r - 2][p.c] == 'S')
				return true;
		if (game().isValid(Point(p.r + game().shipLength(shipID), p.c)))
			if (myShips[p.r + game().shipLength(shipID) + 1][p.c ] == 'S')
				return true;
	}

	return false;
}

bool GoodPlayer::alreadyShot(Point p)
{
	if (p.r >= game().rows() || p.c >= game().cols() || p.r < 0 || p.c < 0) //if out of bounds
		return true;

	for (int i = 0; i < numShots; i++)
	{
		if (p.r == m_shots[i].r && p.c == m_shots[i].c)
			return true;
	}
	return false;
}

void GoodPlayer::addPoint(Point p)
{
	m_shots[numShots].r = p.r;
	m_shots[numShots].c = p.c;
}

bool GoodPlayer::goodHelper(Board&b, int shipid, int count, int track)
{
	if (shipid == game().nShips()) //Base case: we made it through all ships
		return true;

	Point p(5, 5);
	Direction d; 

	if (track > 50) //Emergency base case: we need to just place some ships
	{
		b.clear();
		b.placeShip(Point(2, 3), 0, HORIZONTAL);
		b.placeShip(Point(5, 6), 1, VERTICAL);
		b.placeShip(Point(4, 7), 2, VERTICAL);
		b.placeShip(Point(9, 6), 3, HORIZONTAL);
		b.placeShip(Point(0, 9), 4, VERTICAL);
		return true;
		
	}

	if (count % 5 == 0) //Top left
	{
		p.r = randInt(game().rows() / 2);
		p.c = randInt(game().cols() / 2);
	}
	else if (count % 5 == 1) //Top right
	{
		p.r = randInt(game().rows() / 2);
		p.c = randInt(game().cols() / 2) + (game().cols() / 2);
	}
	else if (count % 5 == 2) //Bottom left
	{
		p.r = randInt(game().rows() / 2) + (game().rows() / 2);
		p.c = randInt(game().cols() / 2);
	}
	else if (count % 5 == 3) //Bottom right
	{
		p.r = randInt(game().rows() / 2) + (game().rows() / 2);
		p.c = randInt(game().cols() / 2) + (game().cols() / 2);
	}
	else //Place anywhere
	{
		p.r = randInt(game().rows());
		p.c = randInt(game().cols());
	}


	switch (randInt(2))
	{
	case 0:  //Place it vertically
		if (!b.placeShip(p, shipid, VERTICAL)) //if we cant place
		{
			track++;
			return goodHelper(b, shipid, count, track);
		}
		if (checkShips(p, VERTICAL, shipid)) //Try tp
		{
			b.unplaceShip(p, shipid, VERTICAL);
			track++;
			return goodHelper(b, shipid, count, track);
		}
		d = VERTICAL;
		break;
	case 1: //Try it horizontally
		if (!b.placeShip(p, shipid, HORIZONTAL)) //if we cant place
		{
			track++;
			return goodHelper(b, shipid, count, track + 1);
		}
		if (checkShips(p, HORIZONTAL, shipid))
		{
			b.unplaceShip(p, shipid, HORIZONTAL);
			track++;
			return goodHelper(b, shipid, count, track);
		}
		d = HORIZONTAL;
		break;
	}
	
	recordShip(p, d, shipid);
	return goodHelper(b, shipid + 1, count + 1, track);

}

bool GoodPlayer::placeShips(Board& b)
{
	return goodHelper(b, 0, randInt(5), 0);
}

Point GoodPlayer::recommendAttack()
{
	nextShot nxt;
	Point ans(1, 1);

	if (getState()) 
	{
		Point rand = game().randomPoint();
			if (alreadyShot(rand))
				return recommendAttack();
			if (numShots < 30 && closeBy(rand))
				return recommendAttack();
			else
				return rand;
	}

	else  //We just hit a ship
	{
		if (hitSinceLstChng > 1)
		{
			if (hitShots[hitShots.size() - 1].r == hitShots[hitShots.size() - 2].r) //Ship is probably horizontal
			{
				if (hitShots[hitShots.size() - 1].c == hitShots[hitShots.size() - 2].c + 1) //To the right
				{
					nxt = RIGHT;
					ans.c = hitShots[hitShots.size() - 1].c + 1;
					ans.r = hitShots[hitShots.size() - 1].r;
				}
				else //to the left
				{
					nxt = LEFT;
					ans.c = hitShots[hitShots.size() - 1].c - 1;
					ans.r = hitShots[hitShots.size() - 1].r;
				}


				if (nxt == RIGHT && alreadyShot(ans)) // If Weve reached the end
					while (alreadyShot(ans))
					{
						ans.c--; //Go back left
						if (ans.c < 0)
							break;
					}
				else if (nxt == LEFT && alreadyShot(ans)) // If Weve reached the end
					while (alreadyShot(ans))
					{
						ans.c++; //Go back right
						if (ans.c > game().cols())
							break;
					}
			}
			else if (hitShots[hitShots.size() - 1].c == hitShots[hitShots.size() - 2].c) //Ship is probably vertical
			{
				if (hitShots[hitShots.size() - 1].r == hitShots[hitShots.size() - 2].r - 1) //Up
				{
					nxt = UP;
					ans.c = hitShots[hitShots.size() - 1].c;
					ans.r = hitShots[hitShots.size() - 1].r - 1;
				}
				else //Down
				{
					nxt = DOWN;
					ans.c = hitShots[hitShots.size() - 1].c;
					ans.r = hitShots[hitShots.size() - 1].r + 1;
				}

				if (nxt == DOWN && alreadyShot(ans)) // If Weve reached the end
					while (alreadyShot(ans))
					{
						ans.r--; //Go back up
						if (ans.r < 0)
							break;
					}
				else if (nxt == UP && alreadyShot(ans))  // If Weve reached the end
					while (alreadyShot(ans))
					{
						ans.r++; //Go back down
						if (ans.r > game().rows())
							break;
					}
			}
		}
		else //We just hit the target so lets circle around
		{
			ans.r = hitShots[hitShots.size() - 1].r + 1; //Look down
			ans.c = hitShots[hitShots.size() - 1].c;
			if (!alreadyShot(ans))
				return ans;

			ans.r = hitShots[hitShots.size() - 1].r;  //Look to left
			ans.c = hitShots[hitShots.size() - 1].c - 1;
			if (!alreadyShot(ans))
				return ans;

			ans.r = hitShots[hitShots.size() - 1].r - 1; //Look up
			ans.c = hitShots[hitShots.size() - 1].c;
			if (!alreadyShot(ans))
				return ans;

			ans.r = hitShots[hitShots.size() - 1].r;  //Look to right
			ans.c = hitShots[hitShots.size() - 1].c + 1;
			if (!alreadyShot(ans))
				return ans;

			else //If I fuck up, at least shoot somewhere
			{
				ans.r = randInt(game().rows());
				ans.c = randInt(game().cols());
				if (alreadyShot(ans))
					return recommendAttack();
				else
					return ans;
			}
		}
		if (alreadyShot(ans))
		{
			changeState(true);
			return recommendAttack();
		}
		return ans;
	}
}

void GoodPlayer::recordAttackResult(Point p, bool validShot, bool shotHit, bool shipDestroyed, int shipId)
{
	if (!validShot)  //If invalid wdgaf 
		return;

	if (!alreadyShot(p))
	{
		addPoint(p);  //Add the point to the array
		numShots++;
	}
	if (shotHit)
	{
		hitShots.push_back(p); //Add it to vec
		hitSinceLstChng++;
		changeState(false); //Change to state 2
	}

	if (shipDestroyed)
	{
		hitSinceLstChng = 0;
		changeState(true);  //If we destroy a ship, back to state 1
	}
	return;
}

void GoodPlayer::recordAttackByOpponent(Point p)
{

}


//*********************************************************************
//  createPlayer
//*********************************************************************

Player* createPlayer(string type, string nm, const Game& g)
{
	static string types[] = {
		"human", "awful", "mediocre", "good"
	};

	int pos;
	for (pos = 0; pos != sizeof(types) / sizeof(types[0]) &&
		type != types[pos]; pos++)
		;
	switch (pos)
	{
	case 0:  return new HumanPlayer(nm, g);
	case 1:  return new AwfulPlayer(nm, g);
	case 2:  return new MediocrePlayer(nm, g);
	case 3:  return new GoodPlayer(nm, g);
	default: return nullptr;
	}
}


