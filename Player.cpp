#include "Player.h"
#include "Board.h"
#include "Game.h"
#include "globals.h"
#include <iostream>
#include <string>

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
{
}

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
    // AwfulPlayer completely ignores the result of any attack
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

// TODO:  You need to replace this with a real class declaration and
//        implementation.


class HumanPlayer : public Player
{
public:
    HumanPlayer(string nm, const Game& g);
    virtual bool isHuman() const;
    virtual bool placeShips(Board& b);
    virtual Point recommendAttack();
    virtual void recordAttackResult(Point p, bool validShot, bool shotHit,
        bool shipDestroyed, int shipId);
    virtual void recordAttackByOpponent(Point p);
};

HumanPlayer::HumanPlayer(string nm, const Game& g)
    : Player(nm, g)
{
}
bool HumanPlayer::isHuman() const{ 
    return true;
}

bool HumanPlayer::placeShips(Board& b)
{
    cout << name() << " needs to place " << game().nShips() << " ship(s).\n";
    b.display(false);

    for (int k = 0; k < game().nShips(); k++)
    {
        int shipLen = game().shipLength(k);
        string shipNm = game().shipName(k);
        Direction d;


        while (true)
        {
            string dirInput;
            cout << "Enter h or v for direction of " << shipNm << " (length " << shipLen << "): ";

            cin >> dirInput;
            cin.ignore(10000, '\n');
            char dir = dirInput[0];

            if (dir == 'h') {
                d = HORIZONTAL;
                break;
            }
            
            else if (dir == 'v') {
                d = VERTICAL;
                break;
            }
            else
            {
                cout << "Direction must be h or v.\n";
            }
        }

            while (true)
            {
            int r, c;
            if (d == HORIZONTAL) {
                cout << "Enter row and column of leftmost cell (e.g., 3 5): ";
            }
            else cout << "Enter row and column of topmost cell (e.g., 3 5): ";

            if (!(cin >> r >> c))
            {
                cout << "You must enter two integers.\n";

                cin.clear(); 
                cin.ignore(10000, '\n'); 

                continue;
            }
            cin.ignore(10000, '\n');
            if (b.placeShip(Point(r, c), k, d))
            {
                cout << "The ship has been placed.\n";
                b.display(false);
                break;
            }
            else
            {
                cout << "The ship cannot be placed there.\n";
            }
        }
    }
    return true;
}

Point HumanPlayer::recommendAttack()
{
    int r, c;
    cout << "Enter the row and column to attack (e.g., 3 5): ";
    getLineWithTwoIntegers(r, c);
    return Point(r, c);
}
void HumanPlayer::recordAttackResult(Point, bool, bool, bool, int) {}
void HumanPlayer::recordAttackByOpponent(Point) {}


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
    MediocrePlayer(string nm, const Game& g);
    virtual bool placeShips(Board& b);
    bool placeShipsHelper(Board& b, int shipId);
    virtual Point recommendAttack();
    virtual void recordAttackResult(Point p, bool validShot, bool shotHit, bool shipDestroyed, int shipId);
    virtual void recordAttackByOpponent(Point p);
private:
    bool m_attacked[MAXROWS][MAXCOLS];
    int   m_state;
    Point m_hitPoint;

};

MediocrePlayer::MediocrePlayer(string nm, const Game& g)
    : Player(nm, g), m_state(1), m_hitPoint(0, 0)
{
    for (int r = 0; r < MAXROWS; r++)
        for (int c = 0; c < MAXCOLS; c++)
            m_attacked[r][c] = false;
}
bool MediocrePlayer::placeShipsHelper(Board& b, int shipId)
{
    if (shipId == game().nShips())
        return true; 

    for (int r = 0; r < game().rows(); r++)
    {
        for (int c = 0; c < game().cols(); c++)
        {
            for (int d = 0; d < 2; d++)
            {
                Direction dir;
                if (d == 0) {
                    dir = HORIZONTAL;
                }
                else {
                    dir = VERTICAL;
                }
                if (b.placeShip(Point(r, c), shipId, dir))
                {
                    if (placeShipsHelper(b, shipId + 1))
                        return true;
                    b.unplaceShip(Point(r, c), shipId, dir);
                }
            }
        }
    }
    return false;
}


bool MediocrePlayer::placeShips(Board& b)
{
    b.clear();
    for (int attempt = 0; attempt < 50; attempt++)
    {
        b.block();
        if (placeShipsHelper(b, 0))
        {
            b.unblock();
            return true;
        }

        b.unblock();
        b.clear();
    }

    return false;
}

Point MediocrePlayer::recommendAttack()
{
    if (m_state == 1)
    {
        while (true)
        {
            Point p = game().randomPoint();
            if (!m_attacked[p.r][p.c])
                return p;
        }
    }
    else
    {
        vector<Point> candidates;
        int hr = m_hitPoint.r;
        int hc = m_hitPoint.c;
        for (int i = -4; i <= 4; i++)
        {
            if (i == 0) continue;
            Point ph(hr, hc + i);
            if (game().isValid(ph) && !m_attacked[ph.r][ph.c])
                candidates.push_back(ph);
            Point pv(hr + i, hc);
            if (game().isValid(pv) && !m_attacked[pv.r][pv.c])
                candidates.push_back(pv);
        }

        if (candidates.empty())
        {
            m_state = 1;
            return recommendAttack();
        }


        return candidates[randInt(candidates.size())];
    }
}

void MediocrePlayer::recordAttackResult(Point p, bool validShot, bool shotHit,  bool shipDestroyed, int shipId)
{
    if (!validShot)
        return;

    m_attacked[p.r][p.c] = true;

    if (m_state == 1)
    {
        if (shotHit && !shipDestroyed)
        {
            m_state = 2;
            m_hitPoint = p;
        }
    }
    else 
    {
        if (shipDestroyed)
        {
            m_state = 1;
        }
      
    }
}


void MediocrePlayer::recordAttackByOpponent(Point p)
{
}

//*********************************************************************
//  GoodPlayer
//*********************************************************************

class GoodPlayer : public Player
{
public:
    GoodPlayer(string nm, const Game& g);
    virtual bool placeShips(Board& b);
    virtual Point recommendAttack();
    virtual void recordAttackResult(Point p, bool validShot, bool shotHit,
        bool shipDestroyed, int shipId);
    virtual void recordAttackByOpponent(Point p);

private:
    bool placeShipsHelper(Board& b, int shipId);

    char  m_known[MAXROWS][MAXCOLS];
    bool  m_attacked[MAXROWS][MAXCOLS];

    int   m_state;
    Point m_hitPoint;

    vector<int> m_remainingLengths;
};

GoodPlayer::GoodPlayer(string nm, const Game& g)
    : Player(nm, g), m_state(1), m_hitPoint(0, 0)
{
    for (int i = 0; i < MAXROWS; i++)
        for (int j = 0; j < MAXCOLS; j++)
        {
            m_known[i][j] = '.';
            m_attacked[i][j] = false;
        }
    for (int s = 0; s < game().nShips(); s++)
        m_remainingLengths.push_back(game().shipLength(s));
}

bool GoodPlayer::placeShipsHelper(Board& b, int shipId)
{
    if (shipId == game().nShips())
        return true;
    for (int r = 0; r < game().rows(); r++)
        for (int c = 0; c < game().cols(); c++)
        {
            if (b.placeShip(Point(r, c), shipId, HORIZONTAL))
            {
                if (placeShipsHelper(b, shipId + 1)) return true;
                b.unplaceShip(Point(r, c), shipId, HORIZONTAL);
            }
            if (b.placeShip(Point(r, c), shipId, VERTICAL))
            {
                if (placeShipsHelper(b, shipId + 1)) return true;
                b.unplaceShip(Point(r, c), shipId, VERTICAL);
            }
        }
    return false;
}

bool GoodPlayer::placeShips(Board& b)
{
    for (int attempt = 0; attempt < 200; attempt++)
    {
        b.clear();
        bool ok = true;
        for (int s = 0; s < game().nShips(); s++)
        {
            bool placed = false;
            for (int tries = 0; tries < 100 && !placed; tries++)
            {
                Direction d = (randInt(2) == 0) ? HORIZONTAL : VERTICAL;
                Point p = game().randomPoint();
                if (b.placeShip(p, s, d))
                    placed = true;
            }
            if (!placed) { ok = false; break; }
        }
        if (ok) return true;
    }
    return placeShipsHelper(b, 0);
}

Point GoodPlayer::recommendAttack()
{
    if (m_state == 2)
    {
        vector<Point> candidates;
        int r0 = m_hitPoint.r, c0 = m_hitPoint.c;
        for (int i= 1; i <= 4; i++)
        {
            int dr[] = { -i, i, 0, 0 };
            int dc[] = { 0, 0, -i, i };
            for (int d = 0; d < 4; d++)
            {
                Point p(r0 + dr[d], c0 + dc[d]);
                if (game().isValid(p) && !m_attacked[p.r][p.c])
                    candidates.push_back(p);
            }
        }
        if (!candidates.empty())
            return candidates[randInt((int)candidates.size())];
        m_state = 1; 
    }

    vector<Point> pp;
    for (int r = 0; r < game().rows(); r++)
        for (int c = 0; c < game().cols(); c++)
            if (!m_attacked[r][c] && (r + c) % 2 == 0)
                pp.push_back(Point(r, c));

    if (!pp.empty())
        return pp[randInt((int)pp.size())];

    while (true)
    {
        Point p = game().randomPoint();
        if (!m_attacked[p.r][p.c])
            return p;
    }
}

void GoodPlayer::recordAttackResult(Point p, bool validShot, bool shotHit,
    bool shipDestroyed, int shipId)
{
    if (!validShot) return;
    m_attacked[p.r][p.c] = true;
    m_known[p.r][p.c] = shotHit ? 'X' : 'o';

    if (shipDestroyed && shipId >= 0 && shipId < (int)m_remainingLengths.size())
        m_remainingLengths[shipId] = 0;

    if (m_state == 1)
    {
        if (shotHit && !shipDestroyed)
        {
            m_state = 2;
            m_hitPoint = p;
        }
    }
    else
    {
        if (shipDestroyed)
            m_state = 1;
    }
}

void GoodPlayer::recordAttackByOpponent(Point) {}


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
