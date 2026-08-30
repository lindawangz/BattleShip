#include "Board.h"
#include "Game.h"
#include "globals.h"
#include <iostream>

using namespace std;

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
    // TODO:  Decide what private members you need.  Here's one that's likely
    //        to be useful:
    const Game& m_game;
    char b[MAXROWS+1][MAXCOLS+1];
    bool m_shipPlaced[MAXROWS * MAXCOLS];
};

BoardImpl::BoardImpl(const Game& g)
    : m_game(g)
{
    for (int i = 0; i < m_game.rows(); i++) {
        for (int j = 0; j < m_game.cols(); j++) {
            b[i][j] = '.';
        }
    }
    for (int i = 0; i < MAXROWS * MAXCOLS; i++)
        m_shipPlaced[i] = false;
    // This compiles, but may not be correct
}

void BoardImpl::clear()
{
    for (int i = 0; i < m_game.rows(); i++) {
        for (int j = 0; j < m_game.cols(); j++) {
            b[i][j] = '.';
        }
    }
    for (int i = 0; i < MAXROWS * MAXCOLS; i++)
        m_shipPlaced[i] = false;
    // This compiles, but may not be correct
}

void BoardImpl::block()
{
    int totalBlock = m_game.rows() * m_game.cols() / 2;
    int blockCount = 0;
    while (blockCount < totalBlock) {
        int rr = randInt(m_game.rows());
        int rc = randInt(m_game.cols());

        if (b[rr][rc] == '.'){
            b[rr][rc] = '#';
            blockCount++;
        }
    }
}
        
    // TODO:  Replace this with code to block half of the cells on the board

void BoardImpl::unblock()
{
    for (int i = 0; i < m_game.rows(); i++) {
        for (int j = 0; j < m_game.cols(); j++) {
            if (b[i][j] == '#') {
            b[i][j] = '.';
             
            }
        }
    }
    // TODO:  Replace this with code to unblock all blocked cells
}

bool BoardImpl::placeShip(Point topOrLeft, int shipId, Direction dir)
{            
  
    if (shipId < 0 || shipId > m_game.nShips()-1) {
        return false;
    }
    if (m_shipPlaced[shipId])
        return false;

  
    if (dir == VERTICAL) {
        if (topOrLeft.r < 0 || topOrLeft.r + m_game.shipLength(shipId) > m_game.rows()) {
            return false;
        }
        if (topOrLeft.c < 0 || topOrLeft.c >=  m_game.cols()) {
            return false;
        }
        for (int i = topOrLeft.r; i < topOrLeft.r + m_game.shipLength(shipId); i++) {
            if (b[i][topOrLeft.c] != '.') {
                return false;
            }
        }
        for (int i = topOrLeft.r; i < topOrLeft.r + m_game.shipLength(shipId); i++) {
            b[i][topOrLeft.c] = m_game.shipSymbol(shipId);
        }
    }
    if (dir == HORIZONTAL) {
        if (topOrLeft.r < 0 || topOrLeft.r >= m_game.rows()) {
            return false;
        }
        if (topOrLeft.c < 0 || topOrLeft.c + m_game.shipLength(shipId) > m_game.cols()) {
            return false;
        }
        for (int i = topOrLeft.c; i < topOrLeft.c + m_game.shipLength(shipId); i++) {
            if (b[topOrLeft.r][i] != '.') {
                return false;
            }
        }
        for (int i = topOrLeft.c; i < topOrLeft.c + m_game.shipLength(shipId); i++) {
            b[topOrLeft.r][i] = m_game.shipSymbol(shipId);

        }
    }
    m_shipPlaced[shipId] = true;

    return true; // This compiles, but may not be correct
}

bool BoardImpl::unplaceShip(Point topOrLeft, int shipId, Direction dir)
{

    if (shipId < 0 || shipId > m_game.nShips() - 1) {
        return false;
    }

    if (!m_shipPlaced[shipId])  
        return false;

    if (dir == VERTICAL) {
            if (topOrLeft.r + m_game.shipLength(shipId) > m_game.rows()) {
                return false;
            }
            for (int i = topOrLeft.r; i < topOrLeft.r + m_game.shipLength(shipId); i++) {
                if (b[i][topOrLeft.c] != m_game.shipSymbol(shipId)) {
                    return false;
                }
            }
            for (int i = topOrLeft.r; i < topOrLeft.r + m_game.shipLength(shipId); i++) {
                b[i][topOrLeft.c] = '.';
            }
        }
    if (dir == HORIZONTAL) {
        if (topOrLeft.c + m_game.shipLength(shipId) > m_game.cols()) {
            return false;
        }
        for (int i = topOrLeft.c; i < topOrLeft.c + m_game.shipLength(shipId); i++) {
            if (b[topOrLeft.r][i] != m_game.shipSymbol(shipId)) {
                return false;
            }
        }
        for (int i = topOrLeft.c; i < topOrLeft.c + m_game.shipLength(shipId); i++) {
            b[topOrLeft.r][i] = '.';

        }
    }
    m_shipPlaced[shipId] = false;

    return true; // This compiles, but may not be correct
}

void BoardImpl::display(bool shotsOnly) const {
    cout << "  ";
    for (int i = 0; i < m_game.cols(); i++) {
        cout << i;
    }
    cout << "\n";
    for (int i = 0; i < m_game.rows(); i++) {
        cout << i << " ";
        for (int j = 0; j < m_game.cols(); j++) {

            if (b[i][j] == 'X' || b[i][j] == 'o') {
                cout << b[i][j];
            }
            else if (b[i][j] == '#' || b[i][j]=='.') {
                cout << '.';
            }
            else {
                if (shotsOnly) {
                    cout << '.';

                }
                else cout << b[i][j];
            }
        }
        cout << "\n";
    }
}
    // This compiles, but may not be correct

bool BoardImpl::attack(Point p, bool& shotHit, bool& shipDestroyed, int& shipId)
{
    if (p.c < 0 || p.c >= m_game.cols() || p.r < 0 || p.r >= m_game.rows() || b[p.r][p.c] == 'X' || b[p.r][p.c] == 'o') {
        return false;
    }
    if (b[p.r][p.c] == '.' || b[p.r][p.c] == '#')
    {
        // Miss
        shotHit = false;
        shipDestroyed = false;
        b[p.r][p.c] = 'o';
    }
    else
    {
        shipDestroyed = false;
        shotHit = true;
        int id = -1;
        for (int i = 0; i < m_game.nShips(); i++) {
            if (b[p.r][p.c] == m_game.shipSymbol(i)) {
                id = i;
                break;
            }
        }
        b[p.r][p.c] = 'X';
        bool anyRemaining = false;
        for (int i = 0; i < m_game.rows(); i++) {
            for (int j = 0; j < m_game.cols(); j++) {
                if (b[i][j] == m_game.shipSymbol(id)) {
                    anyRemaining = true;
            }
            }
        }
        if (!anyRemaining) {
            shipId = id;
            shipDestroyed = true;
        }

    }
    return true;// This compiles, but may not be correct
}

bool BoardImpl::allShipsDestroyed() const
{
    for (int i = 0; i< m_game.rows(); i++) {
        for (int j = 0; j < m_game.cols(); j++) {
            if (b[i][j] != '.' && b[i][j] != '#' && b[i][j] != 'X' && b[i][j] != 'o') {
                return false;
            }
        }
   }
    return true; // This compiles, but may not be correct
}

//******************** Board functions ********************************

// These functions simply delegate to BoardImpl's functions.
// You probably don't want to change any of this code.

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
