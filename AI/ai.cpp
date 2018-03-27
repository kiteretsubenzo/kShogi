#include <iostream>
#include <list>
#include <vector>
#include <thread>
#include <algorithm>
#include <mutex>
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

	mtx.lock();
	std::vector<Board::PAWN_MOVE> moveList;
	board.GetMoveList(moveList);
	for( unsigned int i=0; i<moveList.size(); i++ )
	{
		Board::PAWN_MOVE &move = moveList[i];
		board.PrintKihu(move);
		board.Move(move);
		jobs.push_back(Board::MoveToString(moveList[i]) + "\n" + board.BoardToString());
		board.Back(move);
		break;
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
		job = jobs.front();
    waits.push_back(job);
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
		std::list<std::string>::iterator ite = std::find( waits.begin(), waits.end(), result );
		ite = waits.begin();
		if( ite != waits.end() )
		{
			waits.erase(ite);
		}
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