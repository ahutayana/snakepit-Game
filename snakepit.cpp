// snakepit.cpp

// Portions you are to complete are marked with a TODO: comment.
// We've provided some incorrect return statements (so indicated) just
// to allow this skeleton program to compile and run, albeit incorrectly.
// The first thing you probably want to do is implement the trivial
// functions (marked TRIVIAL).  Then get Pit::display going.  That gives you
// more flexibility in the order you tackle the rest of the functionality.
// As you finish implementing each TODO: item, remove its TODO: comment.

//GAME = (int rows, int columns, int numSnakes)
//PIT = (



#include <iostream>
#include <string>
#include <random>
#include <utility>
#include <cstdlib>
using namespace std;

///////////////////////////////////////////////////////////////////////////
// Manifest constants
///////////////////////////////////////////////////////////////////////////

const int MAXROWS = 20;     // max number of rows in the pit
const int MAXCOLS = 40;     // max number of columns in the pit
const int MAXSNAKES = 150;  // max number of snakes allowed

const int UP = 0;
const int DOWN = 1;
const int LEFT = 2;
const int RIGHT = 3;
const int NUMDIRS = 4;

///////////////////////////////////////////////////////////////////////////
// Type definitions
///////////////////////////////////////////////////////////////////////////

class Pit;  // This is needed to let the compiler know that Pit is a
// type name, since it's mentioned in the Snake declaration.

class Snake
{
public:
    // Constructor
    Snake(Pit* pp, int r, int c);

    // Accessors
    int  row() const;
    int  col() const;

    // Mutators
    void move();

private:
    Pit* m_pit;
    int  m_row;
    int  m_col;
};

class Player
{
public:
    // Constructor
    Player(Pit* pp, int r, int c);

    // Accessors
    int  row() const;
    int  col() const;
    int  age() const;
    bool isDead() const;

    // Mutators
    void   stand();
    void   move(int dir);
    void   setDead();

private:
    Pit* m_pit;
    int   m_row;
    int   m_col;
    int   m_age;
    bool  m_dead;
};

class Pit
{
public:
    // Constructor/destructor
    Pit(int nRows, int nCols);
    ~Pit();

    // Accessors
    int     rows() const;
    int     cols() const;
    Player* player() const;
    int     snakeCount() const;
    int     numberOfSnakesAt(int r, int c) const;
    void    display() const;

    // Mutators
    bool   addSnake(int r, int c);
    bool   addPlayer(int r, int c);
    bool   destroyOneSnake(int r, int c);
    bool   moveSnakes();

private:
    int     m_rows;
    int     m_cols;
    Player* m_player;
    Snake* m_snakes[MAXSNAKES];
    int     m_nSnakes;

    // Helper functions
    bool isPosInBounds(int r, int c) const;
};

class Game
{
public:
    // Constructor/destructor
    Game(int rows, int cols, int nSnakes);
    ~Game();

    // Mutators
    void play();

private:
    Pit* m_pit;
};

///////////////////////////////////////////////////////////////////////////
//  Auxiliary function declarations
///////////////////////////////////////////////////////////////////////////

int randInt(int min, int max);
int decodeDirection(char dir);
bool directionToDeltas(int dir, int& rowDelta, int& colDelta);
void clearScreen();

///////////////////////////////////////////////////////////////////////////
//  Snake implementation
///////////////////////////////////////////////////////////////////////////

Snake::Snake(Pit* pp, int r, int c)
{
    if (pp == nullptr)
    {
        cout << "***** A snake must be created in some Pit!" << endl;
        exit(1);
    }
    if (r < 1 || r > pp->rows() || c < 1 || c > pp->cols())
    {
        cout << "***** Snake created with invalid coordinates (" << r << ","
            << c << ")!" << endl;
        exit(1);
    }
    m_pit = pp;
    m_row = r;
    m_col = c;
}

int Snake::row() const
{
    return m_row;
}

int Snake::col() const
{
    return m_col;
}

void Snake::move()
{
    // Attempt to move in a random direction; if it can't move, don't move
    switch (randInt(0, NUMDIRS - 1))
    {
    case UP:
        if (m_row > 1)
        {
            m_row -= 1;
        }
        break;
    case DOWN:
        if (m_row < m_pit->rows())
        {
            m_row += 1;
        }
        break;
    case LEFT:
        if (m_col > 1)
        {
            m_col -= 1;
        }
        break;
    case RIGHT:
        if (m_col < m_pit->cols())
        {
            m_col += 1;
        }
        break;
    }
}

///////////////////////////////////////////////////////////////////////////
//  Player implementation
///////////////////////////////////////////////////////////////////////////

Player::Player(Pit* pp, int r, int c)
{
    if (pp == nullptr)
    {
        cout << "***** The player must be created in some Pit!" << endl;
        exit(1);
    }
    if (r < 1 || r > pp->rows() || c < 1 || c > pp->cols())
    {
        cout << "**** Player created with invalid coordinates (" << r
            << "," << c << ")!" << endl;
        exit(1);
    }
    m_pit = pp;
    m_row = r;
    m_col = c;
    m_age = 0;
    m_dead = false;
}

int Player::row() const
{
    return m_row;
}

int Player::col() const
{
    return m_col;
}

int Player::age() const
{
    return m_age;
}

void Player::stand()
{
    m_age++;
}

void Player::move(int dir)
{
    m_age++;

    switch (dir)
    {
    case UP:
        if (m_row > 1 && (m_pit->numberOfSnakesAt(m_row - 1, m_col)) == 0)
        {
            m_row -= 1;
            break;
        }
        if (m_row > 2 && (m_pit->numberOfSnakesAt(m_row - 1, m_col)) > 0)
        {
            m_pit->destroyOneSnake(m_row - 1, m_col);
            m_row -= 2;
            if ((m_pit->numberOfSnakesAt(m_row - 2, m_col)) > 0)
            {
                setDead();
            }
            break;
        }
        stand();
        break;
    case DOWN:
        if (m_row < m_pit->rows() && (m_pit->numberOfSnakesAt(m_row + 1, m_col)) == 0)
        {
            m_row += 1;
            break;
        }
        if (m_row < (m_pit->rows()) - 1 && (m_pit->numberOfSnakesAt(m_row + 1, m_col)) > 0)
        {
            m_pit->destroyOneSnake(m_row + 1, m_col);
            m_row += 2;
            if ((m_pit->numberOfSnakesAt(m_row + 2, m_col)) > 0)
            {
                setDead();
            }
            break;
        }
        stand();
        break;
    case LEFT:
        if (m_col > 1 && (m_pit->numberOfSnakesAt(m_row, m_col - 1)) == 0)
        {
            m_col -= 1;
            break;
        }
        if (m_col > 2 && (m_pit->numberOfSnakesAt(m_row, m_col - 1)) > 0)
        {
            m_pit->destroyOneSnake(m_row, m_col - 1);
            m_col -= 2;
            if ((m_pit->numberOfSnakesAt(m_row, m_col - 2)) > 0)
            {
                setDead();
            }
            break;
        }
        stand();
        break;
    case RIGHT:
        if (m_col < m_pit->cols() && (m_pit->numberOfSnakesAt(m_row, m_col + 1)) == 0)
        {
            m_col += 1;
            break;
        }
        if (m_col < (m_pit->cols()) - 1 && (m_pit->numberOfSnakesAt(m_row, m_col + 1)) > 0)
        {
            m_pit->destroyOneSnake(m_row, m_col + 1);
            m_col += 2;
            if ((m_pit->numberOfSnakesAt(m_row, m_col + 2)) > 0)
            {
                setDead();
            }
            break;
        }
        stand();
        break;
    }
}

bool Player::isDead() const
{
    return m_dead;
}

void Player::setDead()
{
    m_dead = true;
}

///////////////////////////////////////////////////////////////////////////
//  Pit implementation
///////////////////////////////////////////////////////////////////////////

Pit::Pit(int nRows, int nCols)
{
    if (nRows <= 0 || nCols <= 0 || nRows > MAXROWS || nCols > MAXCOLS)
    {
        cout << "***** Pit created with invalid size " << nRows << " by "
            << nCols << "!" << endl;
        exit(1);
    }
    m_rows = nRows;
    m_cols = nCols;
    m_player = nullptr;
    m_nSnakes = 0;
}

Pit::~Pit()
{
    for (int i = 0; i < m_nSnakes; ++i)
    {
        delete m_snakes[i];
    }
    delete m_player;
}

int Pit::rows() const
{
    return m_rows;
}

int Pit::cols() const
{
    return m_cols;
}

Player* Pit::player() const
{
    return m_player;
}

int Pit::snakeCount() const
{
    return m_nSnakes;
}

int Pit::numberOfSnakesAt(int r, int c) const
{
    int snakeCounter = 0;
    for (int i = 0; i < m_nSnakes; ++i)
    {
        if (m_snakes[i]->row() == r && m_snakes[i]->col() == c)
        {
            ++snakeCounter;
        }
    }
    return snakeCounter;
}

void Pit::display() const
{
    // Position (row,col) in the pit coordinate system is represented in
    // the array element grid[row-1][col-1]
    char grid[MAXROWS][MAXCOLS];
    int r, c;

    // Fill the grid with dots
    for (r = 0; r < rows(); r++)
        for (c = 0; c < cols(); c++)
            grid[r][c] = '.';

    for (r = 0; r < rows(); ++r)
    {
        for (c = 0; c < cols(); ++c)
        {
            if (numberOfSnakesAt(r + 1, c + 1) == 1)
            {
                grid[r][c] = 'S';
            }
            if (numberOfSnakesAt(r + 1, c + 1) > 1 && numberOfSnakesAt(r + 1, c + 1) < 9)
            {
                grid[r][c] = '0' + numberOfSnakesAt(r + 1, c + 1);
            }
            if (numberOfSnakesAt(r + 1, c + 1) > 9)
            {
                grid[r][c] = '9';
            }
        }
    }//switch statement is too buggy to work with
    // Indicate player's position
    if (m_player != nullptr)
    {
        char& gridChar = grid[m_player->row() - 1][m_player->col() - 1];
        if (m_player->isDead())
            gridChar = '*';
        else
            gridChar = '@';
    }

    // Draw the grid
    clearScreen();
    for (r = 0; r < rows(); r++)
    {
        for (c = 0; c < cols(); c++)
            cout << grid[r][c];
        cout << endl;
    }
    cout << endl;

    // Write snake and player info
    cout << "There are " << snakeCount() << " snakes remaining." << endl;
    if (m_player == nullptr)
        cout << "There is no player." << endl;
    else
    {
        if (m_player->age() > 0)
            cout << "The player has lasted " << m_player->age() << " steps." << endl;
        if (m_player->isDead())
            cout << "The player is dead." << endl;
    }
}

bool Pit::addSnake(int r, int c)
{
    if (!isPosInBounds(r, c))
        return false;

    // Don't add a snake on a spot with a player
    if (m_player != nullptr && m_player->row() == r && m_player->col() == c)
        return false;

    if (m_nSnakes == MAXSNAKES)
    {
        return false;
    }
    m_snakes[m_nSnakes] = new Snake(this, r, c);
    ++m_nSnakes;
    return true;
}

bool Pit::addPlayer(int r, int c)
{
    if (!isPosInBounds(r, c))
        return false;

    // Don't add a player if one already exists
    if (m_player != nullptr)
        return false;

    // Don't add a player on a spot with a snake
    if (numberOfSnakesAt(r, c) > 0)
        return false;

    // Dynamically allocate new Player and add it to the pit
    m_player = new Player(this, r, c);
    return true;
}

bool Pit::destroyOneSnake(int r, int c)
{
    if (numberOfSnakesAt(r, c) == 0)
    {
        return false;
    }
    for (int i = 0; i < m_nSnakes; ++i)
    {
        if (m_snakes[i]->row() == r && m_snakes[i]->col() == c)
        {
            delete m_snakes[i];
            --m_nSnakes;
            m_snakes[i] = m_snakes[m_nSnakes];//order doesnt matter
            break;
        }
    }
    return true;
}

bool Pit::moveSnakes()
{
    for (int k = 0; k < m_nSnakes; k++)
    {
        m_snakes[k]->move();
        if (m_snakes[k]->row() == m_player->row() && m_snakes[k]->col() == m_player->col())
        {
            m_player->setDead();
        }
    }
    return !m_player->isDead();
}

bool Pit::isPosInBounds(int r, int c) const
{
    return (r >= 1 && r <= m_rows && c >= 1 && c <= m_cols);
}

///////////////////////////////////////////////////////////////////////////
//  Game implementation
///////////////////////////////////////////////////////////////////////////

Game::Game(int rows, int cols, int nSnakes)
{
    if (nSnakes < 0)
    {
        cout << "***** Cannot create Game with negative number of snakes!" << endl;
        exit(1);
    }
    if (nSnakes > MAXSNAKES)
    {
        cout << "***** Trying to create Game with " << nSnakes
            << " snakes; only " << MAXSNAKES << " are allowed!" << endl;
        exit(1);
    }
    if (rows == 1 && cols == 1 && nSnakes > 0)
    {
        cout << "***** Cannot create Game with nowhere to place the snakes!" << endl;
        exit(1);
    }

    // Create pit
    m_pit = new Pit(rows, cols);

    // Add player
    int rPlayer = randInt(1, rows);
    int cPlayer = randInt(1, cols);
    m_pit->addPlayer(rPlayer, cPlayer);

    // Populate with snakes
    while (nSnakes > 0)
    {
        int r = randInt(1, rows);
        int c = randInt(1, cols);
        // Don't put a snake where the player is
        if (r == rPlayer && c == cPlayer)
            continue;
        m_pit->addSnake(r, c);
        nSnakes--;
    }
}

Game::~Game()
{
    delete m_pit;
}

void Game::play()
{
    m_pit->display();
    Player* player = m_pit->player();
    if (player == nullptr)
        return;

    while (!m_pit->player()->isDead() && m_pit->snakeCount() > 0)
    {
        cout << "Move (u/d/l/r//q): ";
        string action;
        getline(cin, action);
        if (action.size() == 0)
            player->stand();
        else
        {
            switch (action[0])
            {
            default:   // if bad move, nobody moves
                cout << '\a' << endl;  // beep
                continue;
            case 'q':
                return;
            case 'u':
            case 'd':
            case 'l':
            case 'r':
                player->move(decodeDirection(action[0]));
                break;
            }
        }
        if (!player->isDead())
            m_pit->moveSnakes();
        m_pit->display();
    }
    if (player->isDead())
        cout << "You lose." << endl;
    else
        cout << "You win." << endl;
}

///////////////////////////////////////////////////////////////////////////
//  Auxiliary function implementation
///////////////////////////////////////////////////////////////////////////

  // Return a uniformly distributed random int from min to max, inclusive
int randInt(int min, int max)
{
    if (max < min)
        swap(max, min);
    static random_device rd;
    static default_random_engine generator(rd());
    uniform_int_distribution<> distro(min, max);
    return distro(generator);
}

int decodeDirection(char dir)
{
    switch (dir)
    {
    case 'u':  return UP;
    case 'd':  return DOWN;
    case 'l':  return LEFT;
    case 'r':  return RIGHT;
    }
    return -1;  // bad argument passed in!
}

bool directionToDeltas(int dir, int& rowDelta, int& colDelta)
{
    switch (dir)
    {
    case UP:     rowDelta = -1; colDelta = 0; break;
    case DOWN:   rowDelta = 1; colDelta = 0; break;
    case LEFT:   rowDelta = 0; colDelta = -1; break;
    case RIGHT:  rowDelta = 0; colDelta = 1; break;
    default:     return false;
    }
    return true;
}

///////////////////////////////////////////////////////////////////////////
//  main()
///////////////////////////////////////////////////////////////////////////

void doBasicTests();
int main()
{
    doBasicTests(); // Remove this line after completing test.
    return 0;       // Remove this line after completing test.
    // Create a game
    // Use this instead to create a mini-game:   Game g(3, 3, 2);
    Game g(9, 10, 44);

    // Play the game
    g.play();
}

///////////////////////////////////////////////////////////////////////////
//  clearScreen implementation
///////////////////////////////////////////////////////////////////////////

// DO NOT MODIFY OR REMOVE ANYTHING BETWEEN HERE AND THE END OF THE FILE!!!
// THE CODE IS SUITABLE FOR VISUAL C++, XCODE, AND g++/g31 UNDER LINUX.

// Note to Xcode users:  clearScreen() will just write a newline instead
// of clearing the window if you launch your program from within Xcode.
// That's acceptable.  (The Xcode output window doesn't have the capability
// of being cleared.)

#ifdef _WIN32  //  Windows

#pragma warning(disable : 4005)
#include <windows.h>

void clearScreen()
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(hConsole, &csbi);
    DWORD dwConSize = csbi.dwSize.X * csbi.dwSize.Y;
    COORD upperLeft = { 0, 0 };
    DWORD dwCharsWritten;
    FillConsoleOutputCharacter(hConsole, TCHAR(' '), dwConSize, upperLeft,
        &dwCharsWritten);
    SetConsoleCursorPosition(hConsole, upperLeft);
}

#else  // not Windows

#include <iostream>
#include <cstring>
#include <cstdlib>

void clearScreen()  // will just write a newline in an Xcode output window
{
    static const char* term = getenv("TERM");
    if (term == nullptr || strcmp(term, "dumb") == 0)
        cout << endl;
    else
    {
        static const char* ESC_SEQ = "\x1B[";  // ANSI Terminal esc seq:  ESC [
        cout << ESC_SEQ << "2J" << ESC_SEQ << "H" << flush;
    }
}

#endif

#include <type_traits>
#include <cassert>

#define CHECKTYPE(c, f, r, a)  \
    static_assert(std::is_same<decltype(&c::f), r (c::*)a>::value, \
       "FAILED: You changed the type of " #c "::" #f);  \
    [[gnu::unused]] auto xxx##c##_##f = static_cast<r(c::*)a>(&c::f)

void thisFunctionWillNeverBeCalled()
{
    // If the student deleted or changed the interfaces to the public
    // functions, this won't compile.  (This uses magic beyond the scope
    // of CS 31.)

    Snake(static_cast<Pit*>(0), 1, 1);
    CHECKTYPE(Snake, row, int, () const);
    CHECKTYPE(Snake, col, int, () const);
    CHECKTYPE(Snake, move, void, ());

    Player(static_cast<Pit*>(0), 1, 1);
    CHECKTYPE(Player, row, int, () const);
    CHECKTYPE(Player, col, int, () const);
    CHECKTYPE(Player, age, int, () const);
    CHECKTYPE(Player, isDead, bool, () const);
    CHECKTYPE(Player, stand, void, ());
    CHECKTYPE(Player, move, void, (int));
    CHECKTYPE(Player, setDead, void, ());

    Pit(1, 1);
    CHECKTYPE(Pit, rows, int, () const);
    CHECKTYPE(Pit, cols, int, () const);
    CHECKTYPE(Pit, player, Player*, () const);
    CHECKTYPE(Pit, snakeCount, int, () const);
    CHECKTYPE(Pit, numberOfSnakesAt, int, (int, int) const);
    CHECKTYPE(Pit, display, void, () const);
    CHECKTYPE(Pit, addSnake, bool, (int, int));
    CHECKTYPE(Pit, addPlayer, bool, (int, int));
    CHECKTYPE(Pit, destroyOneSnake, bool, (int, int));
    CHECKTYPE(Pit, moveSnakes, bool, ());

    Game(1, 1, 1);
    CHECKTYPE(Game, play, void, ());
}

void doBasicTests()
{
    {
        Pit fall(10, 20);
        assert(fall.addPlayer(2, 6));
        Player* pp = fall.player();
        assert(pp->row() == 2 && pp->col() == 6 && !pp->isDead());
        pp->move(UP);
        assert(pp->row() == 1 && pp->col() == 6 && !pp->isDead());
        pp->move(UP);
        assert(pp->row() == 1 && pp->col() == 6 && !pp->isDead());
        pp->setDead();
        assert(pp->row() == 1 && pp->col() == 6 && pp->isDead());
    }
    {
        Pit tsburgh(2, 2);
        assert(tsburgh.addPlayer(1, 1));
        assert(tsburgh.addSnake(2, 2));
        Player* pp = tsburgh.player();
        assert(tsburgh.moveSnakes());
        assert(!pp->isDead());
        for (int k = 0; k < 1000 && tsburgh.moveSnakes(); k++)
            ;
        assert(pp->isDead());
    }
    {
        Pit brad(1, 5);
        assert(brad.addPlayer(1, 1));
        assert(brad.addSnake(1, 2));
        assert(brad.addSnake(1, 4));
        assert(brad.addSnake(1, 4));
        assert(brad.snakeCount() == 3 && brad.numberOfSnakesAt(1, 4) == 2);
        Player* pp = brad.player();
        pp->move(RIGHT);
        pp->move(RIGHT);
        assert(brad.snakeCount() == 1 && brad.numberOfSnakesAt(1, 4) == 1);
        pp->move(LEFT);
        assert(brad.snakeCount() == 0);
        assert(brad.addSnake(1, 5));
        assert(brad.snakeCount() == 1);
    }
    {
        Pit tBrad(1, 21);
        assert(tBrad.addPlayer(1, 21));
        for (int c = 2; c < 21; c += 2)
        {
            for (int k = 0; k < MAXSNAKES / 10; k++)
                assert(tBrad.addSnake(1, c));
        }
        assert(tBrad.snakeCount() == MAXSNAKES);
        Player* pp = tBrad.player();
        for (int k = 0; k < 10; k++)
            pp->move(LEFT);
        assert(pp->row() == 1 && pp->col() == 1);
        assert(tBrad.numberOfSnakesAt(1, 2) == MAXSNAKES / 10 - 1);
        assert(tBrad.snakeCount() == MAXSNAKES - 10);
        for (int k = 0; k < 10; k++)
            assert(tBrad.addSnake(1, 21));
        assert(tBrad.snakeCount() == MAXSNAKES);
        assert(!tBrad.addSnake(1, 21));
        assert(tBrad.snakeCount() == MAXSNAKES);
        assert(tBrad.numberOfSnakesAt(1, 21) == 10);
        // If the program crashes after leaving this compound statement, you
        // are probably messing something up when you delete a snake after
        // it dies (or you have mis-coded the destructor).
        //
        // Draw a picture of your m_snakes array before the player moves,
        // and also note the values of m_nSnakes or any other variable you
        // might have that's involved with the number of snakes.  Trace
        // through your code step by step as the player moves and snakes die,
        // updating the picture according to what the code says, not what
        // you want it to do.  If you don't see a problem then, try tracing
        // through the destruction of the pit.
        //
        // If you execute the code, use the debugger to check on the values
        // of key variables at various points.  If you didn't try to learn
        // to use the debugger, insert statements that write the values of
        // key variables to cerr so you can trace the execution of your code
        // and see the first place where something has gone amiss.
    }
    cout << "Passed all basic tests (as long as when run under g31 there is no further message after this)." << endl;
}