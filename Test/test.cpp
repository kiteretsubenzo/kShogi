#include <iostream>
#include <sstream>
#include <iomanip>
#include <list>
#include <vector>
#include <map>
#include <unordered_map>
#include <algorithm>
#include <thread>
#include <mutex>
#include "../definitions.h"
#include "../board.h"
#include "../AI/worker.h"
#include "../AI/ai.h"
#include "move.h"
#include "put.h"
#include "escape.h"
#include "scout.h"

bool Test()
{
	Board board;
	
	// 差し手のテスト
	// 移動テスト
	std::cout << "move test" << std::endl;
	for( unsigned int i=0; i<testMove.size(); i++ )
	{
		//std::cout << "NO." << (i+1) << std::endl;
		std::vector<std::string> strs = split(testMove[i], ':');
		//std::cout << strs[0] << std::endl;
		board.Init(strs[0]);
		//board.PrintBoard();
		//std::cout << strs[1] << std::endl;
		std::vector<std::string> tests = split(strs[1], '\n');
		std::vector<Board::PAWN_MOVE> moveList = board.GetMoveList<std::vector<Board::PAWN_MOVE>>();
		unsigned int size = std::max<unsigned int>(moveList.size(), tests.size());
		for( unsigned int j=0; j<size; j++ )
		{
			std::string result = "NONE";
			std::string test = "NONE";
			if( j < moveList.size() )
			{
				result = moveList[j].DebugString();
			}
			if( j < tests.size() )
			{
				test = tests[j];
			}
			if( result == test )
			{
				//std::cout << "NO." << (i+1) << std::endl;
				//std::cout << result << ":" << test << " -> true" << std::endl;
			}
			else
			{
				//std::cout << "NO." << (i+1) << std::endl;
				//board.PrintBoard();
				std::cout << result << ":" << test << " -> false" << std::endl;
				//return false;
			}
		}
	}
	
	// 打ちテスト
	std::cout << "put test" << std::endl;
	for( unsigned int i=0; i<testPut.size(); i++ )
	{
		//std::cout << "NO." << (i+1) << std::endl;
		std::vector<std::string> strs = split(testPut[i], ':');
		//std::cout << strs[0] << std::endl;
		board.Init(strs[0]);
		//board.PrintBoard();
		//std::cout << strs[1] << std::endl;
		std::vector<std::string> tests = split(strs[1], '\n');
		std::vector<Board::PAWN_MOVE> moveList = board.GetMoveList<std::vector<Board::PAWN_MOVE>>();
		unsigned int size = std::max<unsigned int>(moveList.size(), tests.size());
		for( unsigned int j=0; j<size; j++ )
		{
			std::string result = "NONE";
			std::string test = "NONE";
			if( j < moveList.size() )
			{
				result = moveList[j].DebugString();
			}
			if( j < tests.size() )
			{
				test = tests[j];
			}
			if( result == test )
			{
				//std::cout << "NO." << (i+1) << std::endl;
				//std::cout << result << ":" << test << " -> true" << std::endl;
			}
			else
			{
				std::cout << "NO." << (i+1) << std::endl;
				board.PrintBoard();
				std::cout << result << ":" << test << " -> false" << std::endl;
				return false;
			}
		}
	}

	// 大手逃れテスト
	std::cout << "escape test" << std::endl;
	for( unsigned int i=0; i<testEscape.size(); i++ )
	{
		//std::cout << "NO." << (i+1) << std::endl;
		std::vector<std::string> strs = split(testEscape[i], ':');
		//std::cout << strs[0] << std::endl;
		board.Init(strs[0]);
		//board.PrintBoard();
		//std::cout << strs[1] << std::endl;
		std::vector<std::string> tests = split(strs[1], '\n');
		std::vector<Board::PAWN_MOVE> moveList = board.GetMoveList<std::vector<Board::PAWN_MOVE>>();
		unsigned int size = std::max<unsigned int>(moveList.size(), tests.size());
		for( unsigned int j=0; j<size; j++ )
		{
			std::string result = "NONE";
			std::string test = "NONE";
			if( j < moveList.size() )
			{
				result = moveList[j].DebugString();
			}
			if( j < tests.size() )
			{
				test = tests[j];
			}
			if( result == test )
			{
				//std::cout << "NO." << (i+1) << std::endl;
				//std::cout << result << ":" << test << " -> true" << std::endl;
			}
			else
			{
				std::cout << "NO." << (i+1) << std::endl;
				board.PrintBoard();
				std::cout << result << ":" << test << " -> false" << std::endl;
				return false;
			}
		}
	}

	// スカウトテスト
	Ai ai;
	ai.SetDebug(false);
	std::cout << "scout test" << std::endl;
	for (unsigned int i = 0; i < testScout.size(); i++)
	{
		board.Init(testScout[i]);

		ai.SetMode("minimax");
		ai.Start(board);

		while (ai.Tick() == false) {
			std::this_thread::sleep_for(std::chrono::milliseconds(1));
		}

		Board::PAWN_MOVE minimaxMove;
		int minimaxScore;
		ai.GetResult(minimaxMove, minimaxScore);
		
		ai.SetMode("scout");
		ai.Start(board);

		while (ai.Tick() == false) {
			std::this_thread::sleep_for(std::chrono::milliseconds(1));
		}

		Board::PAWN_MOVE scoutMove;
		int scoutScore;
		ai.GetResult(scoutMove, scoutScore);

		if (minimaxScore != scoutScore)
		{
			std::cout << "NO." << (i + 1) << std::endl;
			board.PrintBoard();
			std::cout << minimaxScore << " -> " << scoutScore << " -> false" << std::endl;
			return false;
		}
	}

	ai.Stop();
	
	return true;
}