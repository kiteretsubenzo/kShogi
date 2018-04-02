#include <iostream>
#include <sstream>
#include <iomanip>
#include <vector>
#include "../definitions.h"
#include "../board.h"
#include "move.h"
#include "put.h"
#include "escape.h"

void Test()
{
	Board board;
	
	// 差し手のテスト
	// 移動テスト
	/*
	std::cout << "move test" << std::endl;
	for( unsigned int i=0; i<testMove.size(); i++ )
	{
		std::cout << "NO." << (i+1) << std::endl;
		std::vector<std::string> strs = split(testMove[i], ':');
		//std::cout << strs[0] << std::endl;
		board.Init(strs[0]);
		board.PrintBoard();
		//std::cout << strs[1] << std::endl;
		std::vector<std::string> tests = split(strs[1], '\n');
		std::vector<Board::PAWN_MOVE> moveList = board.GetMoveList();
		unsigned int size = std::max<unsigned int>(moveList.size(), tests.size());
		for( unsigned int i=0; i<size; i++ )
		{
			std::string result = "NONE";
			std::string test = "NONE";
			if( i < moveList.size() )
			{
				result = moveList[i].DebugString();
			}
			if( i < tests.size() )
			{
				test = tests[i];
			}
			std::cout << result << ":" << test;
			if( result == test )
				std::cout << " -> true" << std::endl;
			else
			{
				std::cout << " -> false" << std::endl;
			}
		}
	}
	*/
	
	// 打ちテスト
	std::cout << "put test" << std::endl;
	for( unsigned int i=0; i<testPut.size(); i++ )
	{
		std::cout << "NO." << (i+1) << std::endl;
		std::vector<std::string> strs = split(testPut[i], ':');
		//std::cout << strs[0] << std::endl;
		board.Init(strs[0]);
		board.PrintBoard();
		//std::cout << strs[1] << std::endl;
		std::vector<std::string> tests = split(strs[1], '\n');
		std::vector<Board::PAWN_MOVE> moveList = board.GetMoveList();
		unsigned int size = std::max<unsigned int>(moveList.size(), tests.size());
		for( unsigned int i=0; i<size; i++ )
		{
			std::string result = "NONE";
			std::string test = "NONE";
			if( i < moveList.size() )
			{
				result = moveList[i].DebugString();
			}
			if( i < tests.size() )
			{
				test = tests[i];
			}
			std::cout << result << ":" << test;
			if( result == test )
				std::cout << " -> true" << std::endl;
			else
			{
				std::cout << " -> false" << std::endl;
			}
		}
	}
}