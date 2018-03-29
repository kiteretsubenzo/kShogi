#include <iostream>
#include <vector>
#include <list>
#include <iomanip>
#include <thread>
#include <mutex>
#include <unordered_map>
#include <random>
#include <iomanip>
#include <sstream>
#include "board.h"
#include "AI/worker.h"
#include "AI/ai.h"

int main()
{ 
  Board board;
  std::string boardInits[] =
  {
    "h01 y00 e00 g00 k00 u00 r00\n"
    "y_e_g_k_o_k_g_e_y_\n"
    " . . . . . . . . .\n"
    "h_h_h_h_ .h_h_h_h_\n"
    " . . . .y_ . . . .\n"
    " . . . . . . . . .\n"
    " . . . . . . . . .\n"
    "^h^h .^h^h^h^h^h^h\n"
    " .^u . .^o . .^r .\n"
    "^y^e^g^k .^k^g^e^y\n"
    "h01 y00 e00 g00 k00 u00 r00\n"
    "first"
  ,
    "h00 y00 e00 g00 k00 u00 r00\n"
    " . . . . . . . . .\n"
    " . . . . . .r_ . .\n"
    " . . . . . .U_ .o_\n"
    " . . . . . . .^r .\n"
    " . . . . . .^U . .\n"
    " . . . . . . . . .\n"
    " . . . . . . . . .\n"
    " . . . . . . . . .\n"
    " . . . .^o . . . .\n"
    "h00 y00 e00 g00 k00 u00 r00\n"
    "first"
  };
  board.Init(boardInits[1]);
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
  int count = 0;
  Board::PAWN_MOVE move{ PAWN_ROLL::NONE, 6, 4, 7, 5, PAWN_TYPE::NONE };
  while(count < 30)
  {
    move = board.GetNextMove(move);
    if( move == PAWN_MOVE_ZERO )
    {
      break;
    }
    std::cout << move << std::endl;
    count++;
  }
  */
  
  Ai ai;
  ai.Start(board);

  int i = 0;
  while( ai.Tick() == false ) {
    //std::cout << "\r" << i++;
  }
  
  Board::PAWN_MOVE aiMove = ai.GetResult();
  board.PrintKihu(aiMove);
  
  ai.Stop();
  
  std::cout << "end" << std::endl;
}