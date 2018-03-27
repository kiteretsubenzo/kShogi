#include <iostream>
#include <list>
#include <vector>
#include <thread>
#include <algorithm>
#include <mutex>
#include <unordered_map>
#include <random>
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

void Ai::Start(Board board)
{
	jobs.clear();
	waits.clear();
	results.clear();
	isStop = false;
	bestMove = { PAWN_ROLL::NONE, 0, 0, 0, 0, PAWN_TYPE::NONE, false };
	bestScore = 0;

	std::vector<Board::PAWN_MOVE> moveList;
	board.GetMoveList(moveList);
	mtx.lock();
	for( unsigned int i=0; i<moveList.size(); i++ )
	{
		Board::PAWN_MOVE &move = moveList[i];
		board.PrintKihu(move);
		board.Move(move);
		std::list<Board::PAWN_MOVE> moves;
		moves.push_back(move);
		JOB job = { GetJobId(), moves, board };
		jobs.push_back(job);
		board.Back(move);
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
		job = jobIdString + "\n" + jobStruct.board.BoardToString();
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

bool Ai::Tick()
{
	mtx.lock();
	while( 0 < results.size() )
	{
		std::string result = results.front();
		size_t index = result.find("\n");
		std::string jobId = result.substr(0, index);
		std::string scoreString = result.substr(index+1);
		int score = stoi(scoreString);
		std::cout << "score is " << score << std::endl;
		if( bestScore < score )
		{
			bestMove = waits[jobId].front();
			bestScore = score;
		}
		waits.erase(jobId);
		results.pop_front();
	}
	mtx.unlock();
	//std::cout << jobs.size() << waits.size() << results.size() << std::endl;
	if( 0 < jobs.size() || 0 < waits.size() || 0 < results.size() )
	{
		return false;
	}
	return true;
}

void Ai::Stop()
{
  isStop = true;
	worker->Join();
}