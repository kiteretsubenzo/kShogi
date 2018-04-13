#include <iostream>
#include <vector>
#include <list>
#include <iomanip>
#include <thread>
#include <mutex>
#include <map>
#include <unordered_map>
#include <set>
#include <random>
#include <sstream>
#include "definitions.h"
#include "board.h"
#include "AI/worker.h"
#include "AI/ai.h"
#include "Test/test.h"

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
	" . . . . . . .o_y_\n"
	" . . . . . . . .h_\n"
	" . . . . . . . . .\n"
	" . . . . . . .h_ .\n"
	" . . . . . . .^h .\n"
	" . . . . . .^y . .\n"
	" . . . . . . . . .\n"
	" . . . . . . . . .\n"
	" . . . . . . . . .\n"
	"h00 y00 e00 g00 u00 r00 k01\n"
	"first"
  };
  board.Init(boardInits[3]);
  std::list<Board::PAWN_MOVE> history;
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
    
    std::vector<Board::PAWN_MOVE> moveList;
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
  Board::PAWN_MOVE move{ PAWN_ROLL::NONE, 6, 1, 7, 1, PAWN_TYPE::RYU, PAWN_TYPE::NONE, false };

  std::cout << board.BoardToString() << std::endl;
  std::cout << (std::string)move << std::endl;
  std::cout << move.DebugString() << std::endl;

  board.Back(move);

  std::cout << board.BoardToString() << std::endl;
  */
#if false
#ifdef USE_PRIORITY_MULTISET
  board.PrintBoard();
  std::multiset<Board::PAWN_MOVE> moveList = board.GetMoveList();
  for (std::multiset<Board::PAWN_MOVE>::iterator ite = moveList.begin(); ite != moveList.end(); ++ite)
  {
	  std::cout << ite->DebugString() << " " << ite->priority << std::endl;
  }
#else
  board.PrintBoard();
  std::list<Board::PAWN_MOVE> moveList = board.GetMoveList();
  for (std::list<Board::PAWN_MOVE>::iterator ite = moveList.begin(); ite != moveList.end(); ++ite)
  {
	  std::cout << ite->DebugString() << " " << ite->priority << std::endl;
  }
#endif
#endif
#if false
	Ai ai;
	ai.SetDebug(true);
	std::chrono::system_clock::time_point  start, end;
	/*
	for (int i = 0; i < 2; i++)
	{
		start = std::chrono::system_clock::now();
		board.Init(boardInits[3]);

		ai.SetMode("scout");
		ai.SetSearchScore(0);
		ai.Start(board);

		while (ai.Tick() == false) {
			//std::cout << "\r" << i++;
			std::this_thread::sleep_for(std::chrono::milliseconds(1));
		}

		Board::PAWN_MOVE aiMove;
		int aiScore;
		ai.GetResult(aiMove, aiScore);

		std::cout << "best move is " << aiMove.DebugString() << std::endl;
		std::cout << "best score is " << aiScore << std::endl;
		//board.PrintKihu(aiMove);
		
		end = std::chrono::system_clock::now();
		long long milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
		std::cout << milliseconds << std::endl;
	}
	*/

	Board::PAWN_MOVE aiMove;
	int aiScore;

	ai.SetMode("scouttest");
	ai.SetSearchScore(1);
	ai.SetDebug(true);
	ai.Start(board);

	while (ai.Tick() == false) {
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}

	ai.GetResult(aiMove, aiScore);

	std::cout << "best move is " << aiMove.DebugString() << std::endl;
	std::cout << "best score is " << aiScore << std::endl;
  
	ai.Stop();
  
	std::cout << "end" << std::endl;
#endif
  std::string str;
  std::cin >> str;
  
  return 0;
}