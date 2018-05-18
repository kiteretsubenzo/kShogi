#include <iostream>
#include <sstream>
#include <iomanip>
#include <list>
#include <unordered_map>
#include <set>
#include <thread>
#include <mutex>
#include <algorithm>
#include <random>
#include "core/json.hpp"
#include "Shogi/definitions.h"
#include "Shogi/move.hpp"
#include "Shogi/board.hpp"
#include "core/score.hpp"
#include "core/worker.hpp"
#include "aiworker.h"
#include "workercontroller.h"
#include "ai.h"
#include "Test/test.h"

std::vector<std::string> testProblem3test
{
	// No.1
	"h18 y04 e04 g04 u00 r00 k03\n"
	" . . . . . . . . .\n"
	" . . . . . .r_ . .\n"
	" . . . . . .U_ .o_\n"
	" . . . . . . .^r .\n"
	" . . . . . .^U . .\n"
	" . . . . . . . . .\n"
	" . . . . . . . . .\n"
	" . . . . . . . . .\n"
	" . . . . . . . . .\n"
	"h00 y00 e00 g00 u00 r00 k01\n"
	"first"
	":"
	"２三 飛(2,4) 成り"
	,
	// No.2
	"h16 y02 e04 g04 u02 r01 k03\n"
	" . . . . .^R . .y_\n"
	" . . . . . . .o_h_\n"
	" . . . . . . . . .\n"
	" . . . . . . .h_ .\n"
	" . . . . . . .^h .\n"
	" . . . . . .^y . .\n"
	" . . . . . . . . .\n"
	" . . . . . . . . .\n"
	" . . . . . . . . .\n"
	"h00 y00 e00 g00 u00 r00 k01\n"
	"first"
	":"
	"２一 龍(4,1)"
	,
	// No.3
	"h18 y04 e04 g04 u00 r00 k04\n"
	" . . . . . . . . .\n"
	" . . . . . .^U^R .\n"
	" . . . . . . . . .\n"
	" . . . . . . . . .\n"
	" . . . . . .^uR_o_\n"
	" . . . . . . . . .\n"
	" . . . . . . . . .\n"
	" . . . . . . . . .\n"
	" . . . . . . . . .\n"
	"h00 y00 e00 g00 u00 r00 k00\n"
	"first"
	":"
	"１四 馬(3,2)"
};

int main()
{ 
	if (Test())
	{
		std::cout << "test is ok" << std::endl;
		//std::string str;
		//std::cin >> str;
	}
	else
	{
		std::string str;
		std::cin >> str;
		return 1;
	}
  
	Board board;
  std::string boardInits[] =
  {
    "h01 y00 e00 g00 u00 r00 k00\n"
    "y_e_g_k_o_k_g_e_y_\n"
    " . . . . . . . . .\n"
    "h_h_h_h_ .h_h_h_h_\n"
    " . . . .y_ . . . .\n"
    " . . . . . . . . .\n"
    " . . . . . . . . .\n"
    "^h^h .^h^h^h^h^h^h\n"
    " .^u . .^o . .^r .\n"
    "^y^e^g^k .^k^g^e^y\n"
    "h01 y00 e00 g00 u00 r00 k00\n"
    "first"
  ,
    "h18 y04 e04 g04 u00 r00 k04\n"
    " . . . . . . . . .\n"
    " . . . . . .^U^R .\n"
    " . . . . . . . . .\n"
    " . . . . . . . . .\n"
    " . . . . . .^uR_o_\n"
    " . . . . . . . . .\n"
    " . . . . . . . . .\n"
    " . . . . . . . . .\n"
    " . . . . . . . . .\n"
    "h00 y00 e00 g00 u00 r00 k00\n"
    "first"
  ,
	"h18 y04 e04 g04 u00 r00 k04\n"
	" . . . . . . . . .\n"
	" . . . . . . .^R .\n"
	" . . . . . . . . .\n"
	" . . . . . . . .^U\n"
	" . . . . . .^uR_o_\n"
	" . . . . . . . . .\n"
	" . . . . . . . . .\n"
	" . . . . . . . . .\n"
	" . . . . . . . . .\n"
	"h00 y00 e00 g00 u00 r00 k00\n"
	"second"
	,
	"h16 y02 e04 g04 u02 r01 k03\n"
	" . . . . .^R . .y_\n"
	" . . . . . . .o_h_\n"
	" . . . . . . . . .\n"
	" . . . . . . .h_ .\n"
	" . . . . . . .^h .\n"
	" . . . . . .^y . .\n"
	" . . . . . . . . .\n"
	" . . . . . . . . .\n"
	" . . . . . . . . .\n"
	"h00 y00 e00 g00 u00 r00 k01\n"
	"first"
	,
	"h16 y02 e04 g04 u02 r01 k03\n"
	" . . . . . . .^Ry_\n"
	" . . . . . . .o_h_\n"
	" . . . . . . . . .\n"
	" . . . . . . .h_ .\n"
	" . . . . . . .^h .\n"
	" . . . . . .^y . .\n"
	" . . . . . . . . .\n"
	" . . . . . . . . .\n"
	" . . . . . . . . .\n"
	"h00 y00 e00 g00 u00 r00 k01\n"
	"second"
  };
  //board.Init(testProblem3test[0]);
  board.Init(boardInits[4]);
  std::list<Move> history;
  /*
  board.PrintBoard();
  if( board.IsEnd() )
  {
    std::cout << "true" << std::endl;
  }
  else
  {
    std::cout << "false" << std::endl;
  }
  */
  
  /*
  while( true )
  {
    board.PrintBoard();
    
    std::vector<Board::MOVE> moveList;
    board.GetMoveList(moveList);
    std::cout << moveList.size() << std::endl;
    for( unsigned int i=0; i<moveList.size(); i++ )
    {
      std::cout << std::setw(3) << std::right << (i+1) << " : ";
      //Board::PrintMove(moveList[i]);
      board.PrintKihu(moveList[i]);
    }

    std::string str;

    while( true )
    {
      std::cin >> str;
      
      if( str == "end" )
      {
        return 0;
      }
      
      if( str == "back" && 0 < history.size() )
      {
        board.Back(history.back());
        history.pop_back();
        break;
      }

      int num = std::atoi(str.c_str());

      if( 0 < num && num <= (int)moveList.size() )
      {
        board.Move(moveList[num-1]);
        history.push_back(moveList[num-1]);
        break;
      }
    }
  }
  */
  
  /*
  Board::MOVE move{ Pawn_ROLL::NONE, 6, 1, 7, 1, Pawn_TYPE::RYU, Pawn_TYPE::NONE, false };

  std::cout << board.BoardToString() << std::endl;
  std::cout << (std::string)move << std::endl;
  std::cout << move.DebugString() << std::endl;

  board.Back(move);

  std::cout << board.BoardToString() << std::endl;
  */
#if false
  board.PrintBoard();
  MoveList moveList = board.GetMoveList();
  for (MoveList::iterator ite = moveList.begin(); ite != moveList.end(); ++ite)
  {
	  std::cout << ite->DebugString() << " " << ite->priority << std::endl;
  }
#endif
	Ai ai;
	ai.AddWorker();
#if true
	std::chrono::system_clock::time_point start, end;

	ai.SetMode("scouttest");
	ai.SetDebug(false);

	ai.SetSearchScore(Score::SCORE_WIN);
	//ai.SetSearchScore(Score("{score:99999,moves:[82o93nf,81R84hf]}"));
	//ai.SetSearchScore(Score("{score:99999,moves:[82o81Rf,00k72nf]}"));
	//ai.SetLimit(Score());
	ai.SetLimit(Score("{score:99999,moves:[82o93nf,81R84hf]}"));
	//ai.SetLimit(Score("{score:99999,moves:[82o81Rf,00k72nf]}"));
	ai.Start(board);

	while (ai.Tick() == false) {
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}

	Score aiScore;
	ai.GetResult(aiScore);

	Move aiMove = aiScore.moveList.front();

	std::cout << "best move is " << aiMove.DebugString() << std::endl;
	std::cout << "best score is " << aiScore.toJson() << std::endl;

	board.PrintBoard();
	std::cout << aiScore.moveList.DebugString() << std::endl;
	std::cout << (std::string)aiScore.moveList << std::endl;
  
	ai.Stop();
  
	std::cout << "end" << std::endl;
#endif
#if false
	ai.SetDebug(true);

	std::chrono::system_clock::time_point start, end;
	start = std::chrono::system_clock::now();
	
	std::cout << "problem 3 test" << std::endl;
	for (unsigned int i = 0; i < testProblem3test.size(); i++)
	{
		std::cout << "NO." << (i + 1) << std::endl;

		std::vector<std::string> strs = split(testProblem3test[i], ':');

		board.Init(strs[0]);

		// 点数を求める
		ai.SetMode("scouttest");
		ai.SetSearchScore(-SCORE_WIN);
		ai.Start(board);

		while (ai.Tick() == false) {
			std::this_thread::sleep_for(std::chrono::milliseconds(1));
		}

		Score scoutScore;
		ai.GetResult(scoutScore);

		std::cout << (std::string)scoutScore << std::endl;

		MOVE scoutMove = scoutScore.moveList.front();
		std::cout << scoutMove.DebugString() << " " << strs[1] << std::endl;
		if (scoutMove.DebugString() != strs[1])
		{
			std::cout << "NO." << (i + 1) << std::endl;
			board.PrintBoard();
			std::cout << strs[1] << " -> " << scoutMove.DebugString() << " -> false" << std::endl;
			ai.Stop();
			return false;
		}
	}

	end = std::chrono::system_clock::now();
	long long milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
	std::cout << milliseconds << std::endl;
#endif

	std::string str;
	std::cin >> str;
  
	return 0;
}