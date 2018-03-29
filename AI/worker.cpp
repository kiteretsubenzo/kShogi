#include <iostream>
#include <thread>
#include <vector>
#include <list>
#include <algorithm>
#include <mutex>
#include <unordered_map>
#include <random>
#include <iomanip>
#include <sstream>
#include "../board.h"
#include "worker.h"
#include "ai.h"

Worker::Worker(Ai &aiValue)
{
	ai = &aiValue;
}

void Worker::Start()
{
	state = false;
	th = std::thread(&Worker::Test, this);
}

void Worker::Test()
{
	while(true)
	{
		std::string job;
    ai->GetJob(job);
		if( job == "empty" )
		{
			continue;
		}
    if( job == "stop" )
    {
      break;
    }
		
		//std::cout << job << std::endl;
		size_t index = job.find("\n");
		std::string jobId = job.substr(0, index);
		//std::cout << "jobId is " << jobId << std::endl;
		std::string boardStr = job.substr(index + 1);
		board.Init(boardStr);
		board.PrintBoard();
		
		/*
		std::cout << "#prepare thread start" << std::endl;
		std::this_thread::sleep_for(std::chrono::seconds(1));
		std::cout << "#prepare thread finished" << std::endl;
		int score = rnd() % 100 + 1;
		*/
		
		std::list<NODE> nodeStack;
		nodeStack.push_back({PAWN_MOVE_ZERO, 0});
		
		while( true )
		{
			for( std::list<NODE>::iterator ite=nodeStack.begin(); ite != nodeStack.end(); ++ite )
			{
				std::cout << ":" << ite->move << "(" << ite->score << ")";
			}
			std::cout << std::endl;
			
			NODE node = nodeStack.back();
			nodeStack.pop_back();
			
			if( node.move != PAWN_MOVE_ZERO )
			{
				board.Back(node.move);
			}
			//std::cout << Board::MoveToString(move) << " -> " << std::endl;
			//board.PrintBoard();
			Board::PAWN_MOVE move = board.GetNextMove(node.move);
			if( move == PAWN_MOVE_ZERO )
			{
				//std::cout << "zero" << std::endl;
				if( nodeStack.size() == 0 )
				{
					break;
				}
				continue;
			}
			nodeStack.push_back({move, node.score});
			board.Move(move);
			// eval
			if( 2 <= nodeStack.size() )
			{
				//std::cout << "bottom" << std::endl;
				int count = 0;
				Board::PAWN_MOVE moveTmp = PAWN_MOVE_ZERO;
				while(true)
				{
					moveTmp = board.GetNextMove(moveTmp);
					if( moveTmp == PAWN_MOVE_ZERO )
					{
						break;
					}
					count++;
				}
				std::list<NODE>::reverse_iterator ite = nodeStack.rbegin();
				ite->score = count;
			}
			else
			{
				nodeStack.push_back({PAWN_MOVE_ZERO, node.score});
				//std::cout << "add zero" << std::endl;
			}
		}
		
		ai->CallBack(jobId + "\n" + std::to_string(0));
	}
}

void Worker::Join()
{
	th.join();
}