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
	jobs.push_back(1);
}

void Ai::CallBack(std::string str)
{
	mtx.lock();
	results.push_back(1);
	mtx.unlock();
}

bool Ai::GetJob(int &job)
{
	bool isEmpty = true;
	mtx.lock();
	if( 0 < jobs.size() )
	{
		job = jobs.front();
    waits.push_back(job);
		jobs.pop_front();
		mtx.unlock();
		isEmpty = false;
	}
	mtx.unlock();
	return !isEmpty;
}

bool Ai::Tick()
{
	mtx.lock();
	while( 0 < results.size() )
	{
		int result = results.front();
		std::list<int>::iterator ite = std::find( waits.begin(), waits.end(), result );
		if( ite != waits.end() )
		{
			waits.erase(ite);
		}
		results.pop_front();
	}
	mtx.unlock();
	if( 0 < jobs.size() || 0 < waits.size() )
	{
		return false;
	}
	return true;
}