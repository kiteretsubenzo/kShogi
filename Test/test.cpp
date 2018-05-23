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
#include "../core/json.hpp"
#include "../Shogi/definitions.h"
#include "../Shogi/move.hpp"
#include "../Shogi/board.hpp"
#include "../core/score.hpp"
#include "../core/worker.hpp"
#include "../aiworker.h"
#include "../workercontroller.h"
#include "../ai.h"
#include "move.h"
#include "put.h"
#include "escape.h"
#include "scout.h"
#include "problem_3.h"
#include "limit.h"

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
		std::list<Board::MOVE> moveListTmp = board.GetMoveList();
		std::vector<Board::MOVE> moveList;
		for (std::list<Board::MOVE>::iterator ite = moveListTmp.begin(); ite != moveListTmp.end(); ++ite)
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
		std::list<Board::MOVE> moveListTmp = board.GetMoveList();
		std::vector<Board::MOVE> moveList;
		for (std::list<Board::MOVE>::iterator ite = moveListTmp.begin(); ite != moveListTmp.end(); ++ite)
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
		std::list<Board::MOVE> moveListTmp = board.GetMoveList();
		std::vector<Board::MOVE> moveList;
		for (std::list<Board::MOVE>::iterator ite = moveListTmp.begin(); ite != moveListTmp.end(); ++ite)
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
	ai.AddWorker();
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

		int minimaxScore;
		ai.GetResult(minimaxScore);
		
		ai.SetMode("scout");
		ai.Start(board);

		while (ai.Tick() == false) {
			std::this_thread::sleep_for(std::chrono::milliseconds(1));
		}

		int scoutScore;
		ai.GetResult(scoutScore);

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
#if false
	std::chrono::system_clock::time_point  start, end;
	start = std::chrono::system_clock::now();

	ai.SetDebug(false);
	std::cout << "problem 3 test" << std::endl;
	for (unsigned int i = 0; i < testProblem3.size(); i++)
	{
		std::cout << "NO." << (i + 1) << std::endl;

		std::vector<std::string> strs = Json::split(testProblem3[i], ':');

		board.Init(strs[0]);

		// 点数を求める
		ai.SetMode("scouttest");
		ai.SetSearchScore(-Score::SCORE_WIN);
		ai.Start(board);

		while (ai.Tick() == false) {
			std::this_thread::sleep_for(std::chrono::milliseconds(1));
		}

		Score scoutScore;
		ai.GetResult(scoutScore);

		Move scoutMove = scoutScore.moveList.front();
		std::cout << scoutMove.DebugString() << " " << strs[1] << std::endl;
		if (scoutMove.DebugString() != strs[1])
		{
			std::cout << "NO." << (i + 1) << std::endl;
			board.PrintBoard();
			std::cout << strs[1] << " -> " << scoutMove.DebugString() << " -> false" << std::endl;
			std::cout << (std::string)scoutMove << std::endl;
			return false;
		}
	}

	end = std::chrono::system_clock::now();
	long long milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
	std::cout << milliseconds << std::endl;
#endif
	// リミットテスト
#if false
	ai.SetDebug(false);
	std::cout << "limit test" << std::endl;
	for (unsigned int i = 0; i < testLimit.size(); i++)
	{
		std::cout << "NO." << (i + 1) << std::endl;

		std::vector<std::string> strs = Json::split(testLimit[i], ';');

		board.Init(strs[0]);

		std::vector<std::string> tests = Json::split(strs[1], '\n');
		
		// 点数を求める
		Score limit = Score();
		for (int j = 0; j < tests.size(); j++)
		{
			ai.SetMode("scouttest");
			ai.SetSearchScore(Score::SCORE_WIN);
			ai.SetLimit(limit);
			ai.Start(board);

			while (ai.Tick() == false) {
				std::this_thread::sleep_for(std::chrono::milliseconds(1));
			}

			Score scoutScore;
			ai.GetResult(scoutScore);

			scoutScore.score *= -1;

			std::cout << (std::string)scoutScore << std::endl;

			if (scoutScore != Score(tests[j]))
			{
				std::cout << "NO." << (i + 1) << std::endl;
				board.PrintBoard();
				std::cout << (std::string)Score(tests[j]) << " -> " << (std::string)scoutScore << " -> false" << std::endl;
				std::cout << (std::string)scoutScore << std::endl;
				return false;
			}

			limit.copy(scoutScore);
		}
	}
#endif
	
	return true;
}