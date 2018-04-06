#include <iostream>
#include <list>
#include <vector>
#include <thread>
#include <algorithm>
#include <mutex>
#include <unordered_map>
#include <random>
#include <iomanip>
#include <sstream>
#include "../definitions.h"
#include "../board.h"
#include "worker.h"
#include "ai.h"

Ai::Ai()
{
	worker = new Worker(*this);
	worker->Start();
}

Ai::~Ai()
{
	delete worker;
}

void Ai::Start(Board boardValue)
{
	jobs.clear();
	waits.clear();
	results.clear();
	isStop = false;
	board = boardValue;
	bestMove = PAWN_MOVE_ZERO;
	bestScore = std::numeric_limits<int>::min();

	std::vector<Board::PAWN_MOVE> moveList = board.GetMoveList();
	
	mtx.lock();
	/*
	for( unsigned int i=0; i<moveList.size(); i++ )
	{
		Board::PAWN_MOVE &move = moveList[i];
		//board.PrintKihu(move);
		board.Move(move);
		std::list<Board::PAWN_MOVE> moves;
		moves.push_back(move);
		JOB job = { GetJobId(), moves, board };
		if( i < 4 )
		{
			jobs.push_back(job);
		}
		board.Back(move);
	}
	*/
	std::list<Board::PAWN_MOVE> moves;
	moves.push_back(PAWN_MOVE_ZERO);
	if (mode == "scout")
	{
		bestScore = 0;
		JOB job = { GetJobId(), { PAWN_MOVE_ZERO }, -bestScore, 4, board };
		jobs.push_back(job);
	}
	else
	{
		JOB job = { GetJobId(), moves, SCORE_NONE, 4, board };
		jobs.push_back(job);
	}

	mtx.unlock();
}

void Ai::CallBack(const std::string &str)
{
	mtx.lock();
	results.push_back(str);
	mtx.unlock();
}
 
void Ai::GetJob(std::string &job)
{
  if( isStop )
  {
    job = "stop";
    return;
  }
	mtx.lock();
	if( 0 < jobs.size() )
	{
		JOB jobStruct = jobs.front();
		std::string jobIdString = std::to_string(jobStruct.jobId);
		job = jobIdString;
		job += ":" + std::to_string(jobStruct.window);
		job += ":" + std::to_string(jobStruct.deep);
		if (debug)
		{
			job += ":true";
		}
		else
		{
			job += ":false";
		}
		job += ":" + jobStruct.board.BoardToString();
		waits[jobIdString] = jobStruct.moves;
		jobs.pop_front();
	}
  else
  {
    job = "empty";
  }
	mtx.unlock();
	return;
}

void Ai::GetResult(Board::PAWN_MOVE &moveValue, int &scoreValue)
{
	moveValue = bestMove;
	scoreValue = bestScore;
}

bool Ai::Tick()
{
	mtx.lock();
	// 結果を回収
	while( 0 < results.size() )
	{
		std::string result = results.front();
		size_t index = result.find("\n");
		std::string jobId = result.substr(0, index);
		std::string scoreString = result.substr(index+1);
		int score = stoi(scoreString);
		if (mode == "scout")
		{
			if (debug)
			{
				std::cout << "score is " << score << " best score is " << bestScore << std::endl;
			}
			//std::string str;
			//std::cin >> str;
			if (bestScore == score)
			{
				bestMove = waits[jobId].front();
				bestScore = -score;
			}
			else
			{
				bestScore = score;
				JOB job = { GetJobId(), { PAWN_MOVE_ZERO }, -bestScore, 4, board };
				jobs.push_back(job);
			}
		}
		else
		{
			if (debug)
			{
				std::cout << "score is " << score << " best score is " << bestScore;
			}
			if (bestScore < -score)
			{
				bestMove = waits[jobId].front();
				bestScore = -score;
			}
		}
		waits.erase(jobId);
		results.pop_front();
	}
	mtx.unlock();
	//std::cout << " " << jobs.size() << " " << waits.size() << " " << results.size() << std::endl;
	if( 0 < jobs.size() || 0 < waits.size() || 0 < results.size() )
	{
		return false;
	}
	
	if (debug)
	{
		std::cout << "-> best score is " << bestScore << std::endl;
	}
	
	return true;
}

void Ai::Stop()
{
  isStop = true;
	worker->Join();
}