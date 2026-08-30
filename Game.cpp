#include "Game.h"
#include "Board.h"
#include "Player.h"
#include "globals.h"
#include <iostream>
#include <string>
#include <cstdlib>
#include <cctype>
#include <vector>

using namespace std;

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
    int mRow, mCol;
    struct Ship {
        string name;
        int length;
        char symbol;
    };
    vector <Ship> m_ships;
};

void waitForEnter()
{
    cout << "Press enter to continue: ";
    cin.ignore(10000, '\n');
}

GameImpl::GameImpl(int nRows, int nCols)
{
    if (nRows <= MAXROWS && nCols <= MAXCOLS) {
        mRow = nRows;
        mCol = nCols;
    }

    // This compiles but may not be correct
}

int GameImpl::rows() const
{

    return mRow;  // This compiles but may not be correct
}

int GameImpl::cols() const
{
    return mCol;  // This compiles but may not be correct
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

        if (isprint(symbol) != 0 && symbol != 'X' && symbol != 'o' && symbol != '.' ) {
            for (int i = 0; i <static_cast<int>(m_ships.size()); i++) {
                if (m_ships[i].symbol== symbol) {
                    return false;
                }
            }
            Ship s;
            s.name = name;
            s.length = length;
            s.symbol = symbol;
            m_ships.push_back(s);
            return true;
        }
    
    return false;  // This compiles but may not be correct
}

int GameImpl::nShips() const
{
    return m_ships.size(); // This compiles but may not be correct
}

int GameImpl::shipLength(int shipId) const
{
    return m_ships[shipId].length;  // This compiles but may not be correct
}

string GameImpl::shipName(int shipId) const
{
    return  m_ships[shipId].name;   // This compiles but may not be correct
}
char GameImpl::shipSymbol(int shipId) const
{
    return m_ships[shipId].symbol;
}
Player* GameImpl::play(Player* p1, Player* p2, Board& b1, Board& b2, bool shouldPause)
{
    if (p1->placeShips(b1) == false || p2->placeShips(b2) == false) {
        return nullptr;
    }
    Player * players[2] = { p1,p2 };
    Board* boards[2] = { &b1,&b2 };
    int turn = 0;
    while (true) {
        Player* active = players[turn];
        Player* passive = players[1 - turn];
        Board* passBoard = boards[1 - turn];
        cout << active->name() << "'s turn. Board for " << passive->name() << ":\n";
        passBoard->display(active->isHuman());
        //first player attack
        bool shipDestroyed, validShot, shotHit;
        int destroyedId = -1;
        Point p = active->recommendAttack();

        validShot = passBoard->attack(p, shotHit, shipDestroyed, destroyedId);
        // record attack
        active->recordAttackResult(p, validShot, shotHit, shipDestroyed, destroyedId);
        passive->recordAttackByOpponent(p);
        //display result of attack
        if (!validShot)
        {
            cout << active->name() << " wasted a shot at (" << p.r << "," << p.c << ").\n";
        }
        else if (!shotHit)
        {
            cout << active->name() << " attacked (" << p.r << "," << p.c << ") and missed, resulting in:\n";
            passBoard->display(active->isHuman());

        }
        else if (shipDestroyed==false) {
            cout << active->name() << " attacked (" << p.r << "," << p.c << ") and hit something, resulting in:\n";
            passBoard->display(active->isHuman());

        }
        else if (shipDestroyed == true) {
            cout << active->name() << " attacked (" << p.r << "," << p.c << ") and destroyed the " << shipName(destroyedId)<<", resulting in:\n";
            passBoard->display(active->isHuman());

        }
        if (shouldPause) {
            waitForEnter();
        }
        if (passBoard->allShipsDestroyed())
        {
            cout << active->name() << " wins!\n";
            if (passive->isHuman())
            {
                cout << "Here is " << active->name() << "'s board:\n";
                boards[turn]->display(false);
            }
            return active;
        }
        

        turn = 1 - turn;
    }
    return nullptr;
}


   
 // This compiles but may not be correct
 
  /*If the first player is human, do not show undamaged segments
        of any ship on the opponent's board, since that would be cheating. If the
        first player is not human, then show the entire opponent's board, since
        we're just an onlooker to what the computer player will decide for itself.
        b.Make the first player's attack.
        c.Display the result of the attack.
        d.Repeat these steps with the roles of the first and second player reversed.
        5. If the losing player is human, display the winner's board, showing everything
*/


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

