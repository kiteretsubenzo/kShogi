#include <iostream>
#include <sstream>
#include <iomanip>
#include <list>
#include <unordered_map>
#include <set>
#include <algorithm>
#include <thread>
#include <mutex>
#include <chrono>
#include "../definitions.h"
#include "../board.h"
#include "../AI/worker.h"
#include "../AI/ai.h"
#include "move.h"
#include "put.h"
#include "escape.h"
#include "scout.h"
#include "problem_3.h"

bool Test()
{
	Board board;
	
	// 差し手のテスト
	// 移動テスト
#if false
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
		std::list<Board::PAWN_MOVE> moveListTmp = board.GetMoveList();
		std::vector<Board::PAWN_MOVE> moveList;
		for (std::list<Board::PAWN_MOVE>::iterator ite = moveListTmp.begin(); ite != moveListTmp.end(); ++ite)
		{
			moveList.push_back(*ite);
		}
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
#endif
#if false
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
		std::list<Board::PAWN_MOVE> moveListTmp = board.GetMoveList();
		std::vector<Board::PAWN_MOVE> moveList;
		for (std::list<Board::PAWN_MOVE>::iterator ite = moveListTmp.begin(); ite != moveListTmp.end(); ++ite)
		{
			moveList.push_back(*ite);
		}
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
#endif
#if false
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
		std::list<Board::PAWN_MOVE> moveListTmp = board.GetMoveList();
		std::vector<Board::PAWN_MOVE> moveList;
		for (std::list<Board::PAWN_MOVE>::iterator ite = moveListTmp.begin(); ite != moveListTmp.end(); ++ite)
		{
			moveList.push_back(*ite);
		}
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
#endif

	// スカウトテスト
	Ai ai;
	/*
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
	*/
	// 3手詰めテスト
#if true
	std::chrono::system_clock::time_point  start, end;
	start = std::chrono::system_clock::now();

	ai.SetDebug(false);
	std::cout << "problem 3 test" << std::endl;
	for (unsigned int i = 0; i < testProblem3.size(); i++)
	{
		std::cout << "NO." << (i + 1) << std::endl;

		std::vector<std::string> strs = split(testProblem3[i], ':');

		board.Init(strs[0]);

		ai.SetMode("scout");
		ai.SetSearchScore(0);
		ai.Start(board);

		while (ai.Tick() == false) {
			std::this_thread::sleep_for(std::chrono::milliseconds(1));
		}

		Board::PAWN_MOVE scoutMove;
		int scoutScore;
		ai.GetResult(scoutMove, scoutScore);
		
		std::cout << scoutScore << std::endl;
		
		ai.SetMode("move");
		ai.SetSearchScore(-scoutScore);
		ai.Start(board);

		while (ai.Tick() == false) {
			std::this_thread::sleep_for(std::chrono::milliseconds(1));
		}

		ai.GetResult(scoutMove, scoutScore);
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
	ai.Stop();
	
	return true;
}