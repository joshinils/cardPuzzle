#include <iostream>
#include <array>

enum Pos
{
  first = 1,
  second = 2,
  third = 3,
  fourth = 4,
};

struct CardGame
{
  bool card0 = false;
  bool card1 = false;
  bool card2 = false;
  bool card3 = false;

  bool solved = false;

public:
  CardGame(bool c0, bool c1, bool c2, bool c3)
    : card0(c0), card1(c1), card2(c2), card3(c3)
  { }

  CardGame(CardGame& cg)
  {
    if(solved) return;
    this->card0 = cg.card0;
    this->card1 = cg.card1;
    this->card2 = cg.card2;
    this->card3 = cg.card3;
  }

  ~CardGame() = default;

  void flip(int p)
  {
    if(solved) return;
    switch (p)
    {
    case Pos::first:  card0 = !card0; break;
    case Pos::second: card1 = !card1; break;
    case Pos::third:  card2 = !card2; break;
    case Pos::fourth: card3 = !card3; break;
    default: break;
    }
    solved |= (!card0 && !card1 && !card2 && !card3);
  }

  friend std::ostream& operator<<(std::ostream& ostr, CardGame const& cg)
  {
    if(cg.solved)
      return ostr;
    return ostr << cg.card0 << ' '<< cg.card1 << ' '<< cg.card2 << ' '<< cg.card3;
  }
};
const int GAME_LENGTH = 15;

typedef std::array<CardGame, 21> CardGameStack;

CardGameStack genNewGames()
{
  return CardGameStack{
  (CardGame{true,  false, false, false}),
  (CardGame{false, true,  false, false}),
  (CardGame{false, false, true,  false}),
  (CardGame{false, false, false, true }),

  (CardGame{true,  true,  false, false}),
  (CardGame{true,  false, true,  false}),
  (CardGame{true,  false, false, true }),

  (CardGame{true,  true,  false, false}),
  (CardGame{false, true,  true,  false}),
  (CardGame{false, true,  false, true }),

  (CardGame{true,  false, true,  false}),
  (CardGame{false, true,  true,  false}),
  (CardGame{false, false, true,  true }),

  (CardGame{true,  false, false, true }),
  (CardGame{false, true,  false, true }),
  (CardGame{false, false, true,  true }),

  (CardGame{false, true,  true,  true }),
  (CardGame{true,  false, true,  true }),
  (CardGame{true,  true,  false, true }),
  (CardGame{true,  true,  true,  false}),

  (CardGame{true,  true,  true,  true })};
}

void flipAll(CardGameStack& games, int p)
{
  for(auto& g : games) g.flip(p);
  games[0].flip(p);
  games[0].flip(p);
}

void printAll(CardGameStack const& games)
{
  int remaining = games.size();
  for(auto& g : games)
  {
    std::cout << g << '\n';
    if(g.solved)
      remaining--;
  }
  std::cout << "Games remaining: " << remaining << std::endl;
}


class GameSeries
{
  std::array<int, GAME_LENGTH> _moves;
  CardGameStack _games = genNewGames();
  bool gameFinished = false;
  int movesMade = 0;

public:
  GameSeries(std::array<int, GAME_LENGTH> arg)
  {
    for (size_t i = 0; i < _moves.size(); i++)
    { _moves[i] = arg[i]; }
  }
  ~GameSeries() = default;

  void playMoves()
  {
    for (size_t i = 0; i < _moves.size(); i++)
    {
      movesMade++;
      flipAll(_games, _moves[i]);
//      printAll(_games); std::cout << std::endl;
      if(_games.size() == 0)
      {
        gameFinished = true;
        return;
      }
    }
  }

  int remainingGames()
  {
    int remaining = _games.size();
    for(auto& g : _games)
    {
      if(g.solved)
        remaining--;
    }
    return remaining;
  }

  int getMovesMade()
  { return movesMade; }
};


class MoveGenerator
{
public:
  std::array<int, GAME_LENGTH> moves{1,2,1,2,1,2,1,2,1,2,1,2,1,2,1}; // first "valid" moves
  bool movesExhausted = false;

  void generateNextMove()
  {
    iterate();
    while(!valid())
    { iterate(); }
  }

  bool valid()
  {
    for (size_t i = 0; i < moves.size() -1; i++)
    { if(moves[i] == moves[i+1]) return false; }
    return true;
  }

  void iterate()
  {
    for (size_t i = 0; i < moves.size(); i++)
    {
      moves[i] += 1;
      if(moves[i] <= 4) return;
      moves[i] = 1;
    }
    movesExhausted = true;
  }

  friend std::ostream& operator<<(std::ostream& ostr, MoveGenerator const& mg)
  {
    for (size_t i = 0; i < mg.moves.size()-1; i++)
    {
      ostr << mg.moves[i] << ',';
    }
    return ostr << mg.moves[mg.moves.size()-1];
  }
  bool allMovesGenerated()
  {
    return movesExhausted;
  }
};


int main()
{
  MoveGenerator mg;
  int totalGames = 0;
  while (!mg.allMovesGenerated())
  {
    mg.generateNextMove();


    auto games = genNewGames();
    GameSeries gs(mg.moves);
    gs.playMoves();
    if(gs.remainingGames() == 0 && gs.getMovesMade() == 15)
    {
      std::cout << mg << "\n";
      totalGames++;
    }
  }
  std::cout << "done playing " << totalGames << " games" << std::endl;
  return 0;
}
