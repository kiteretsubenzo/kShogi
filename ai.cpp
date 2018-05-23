#include <iostream>
#include <sstream>
#include <iomanip>
#include <list>
#include <unordered_map>
#include <set>
#include <thread>
#include <algorithm>
#include <mutex>
#include <condition_variable>
#include "core/json.hpp"
#include "Shogi/definitions.h"
#include "Shogi/move.hpp"
#include "Shogi/board.hpp"
#include "core/score.hpp"
#include "core/worker.hpp"
#include "aiworker.h"
#include "workercontroller.h"
#include "ai.h"

Ai::Ai()
{
}

Ai::~Ai()
{
	Stop();
	for (std::list<std::shared_ptr<WorkerController>>::iterator ite = workers.begin(); ite != workers.end(); ++ite)
	{
		(*ite)->Stop();
	}
}

void Ai::AddWorker()
{
	workers.push_back(std::shared_ptr<WorkerController>(new WorkerController(this)));
	(*workers.rbegin())->Start();
}

void Ai::Start(Board boardValue)
{
	jobs.clear();
	waits.clear();
	results.clear();
	isStop = false;
	board = boardValue;
	bestScore = std::numeric_limits<int>::min();

	std::unique_lock<std::mutex> uniq(mtx);
	//std::cout << "Start::lock" << std::endl;
	cv.wait(uniq, [&] {return ready;});
	ready = false;
	/*
	for( unsigned int i=0; i<moveList.size(); i++ )
	{
		MOVE &move = moveList[i];
		//board.PrintKihu(move);
		board.Move(move);
		std::list<MOVE> moves;
		moves.push_back(move);
		JOB job = { GetJobId(), moves, board };
		if( i < 4 )
		{
			jobs.push_back(job);
		}
		board.Back(move);
	}
	*/
	
	if (mode == "scout" || mode == "scouttest")
	{
		bestScore = 0;
		JOB job = { GetJobId(), searchScore.Negate(), limit, 4, board };
		jobs.push_back(job);
	}
	else if(mode == "minimax")
	{
		JOB job = { GetJobId(), Score(), limit, 4, board };
		jobs.push_back(job);
	}

	ready = true;
	cv.notify_all();
	//std::cout << "Start::unlock" << std::endl;
}

void Ai::CallBack(const std::string &str)
{
	std::unique_lock<std::mutex> uniq(mtx);
	//std::cout << "CallBack::lock" << std::endl;
	cv.wait(uniq, [&] {return ready;});

	ready = false;

	std::unordered_map<std::string, std::string> strs = Json::fromJson(str);
	if (waits.find(strs["jobid"]) != waits.end())
	{
		results.push_back(str);
	}

	ready = true;
	cv.notify_all();
	//std::cout << "CallBack::unlock" << std::endl;
}
 
void Ai::GetJob(std::string &job)
{
	if( isStop )
	{
		job = "stop";
		return;
	}
	std::unique_lock<std::mutex> uniq(mtx);
	//std::cout << "GetJob::lock" << std::endl;
	cv.wait(uniq, [&] {return ready;});
	ready = false;

	if( 0 < jobs.size() )
	{
		JOB jobStruct = jobs.front();
		std::string jobIdString = std::to_string(jobStruct.jobId);
		job = "jobid:" + jobIdString;
		job += ",window:" + jobStruct.window.toJson();
		job += ",deep:" + std::to_string(jobStruct.deep);
		job += ",limit:" + jobStruct.limit.toJson();

		if (debug)
		{
			job += ",debug:true";
		}
		else
		{
			job += ",debug:false";
		}
		job += ",board:" + jobStruct.board.BoardToString();
		waits[jobIdString] = true;
		jobs.pop_front();
	}
	else
	{
		job = "empty";
	}

	ready = true;
	cv.notify_all();
	//std::cout << "GetJob::unlock" << std::endl;
}

bool Ai::IsAlive(const std::string &jobId)
{
	std::unique_lock<std::mutex> uniq(mtx);
	cv.wait(uniq, [&] {return ready;});
	ready = false;

	bool alive = (waits.find(jobId) != waits.end());
	
	ready = true;
	cv.notify_all();

	return alive;
}

void Ai::GetResult(Score &scoreValue)
{
	scoreValue = bestScore;
}

bool Ai::Tick()
{
	std::unique_lock<std::mutex> uniq(mtx);
	//std::cout << "Tick::lock" << std::endl;
	cv.wait(uniq, [&] {return ready;});
	ready = false;

	// 結果を回収
	while( 0 < results.size() )
	{
		std::unordered_map<std::string, std::string> strs = Json::fromJson(results.front());
		std::string jobId = strs["jobid"];
		std::string scoreString = strs["score"];
		std::string countString = strs["count"];
		Score score(scoreString);
		if (mode == "scout")
		{
			if (debug)
			{
				std::cout << "score is " << (std::string)score << " best score is " << (std::string)bestScore << std::endl;
			}
			std::cout << "score is " << (std::string)score << " best score is " << (std::string)bestScore << std::endl;
			if (bestScore == score)
			{
				bestScore = score.Negate();
			}
			else
			{
				bestScore = score;
				JOB job = { GetJobId(), bestScore.Negate(), limit, 4, board };
				jobs.push_back(job);
			}
		}
		else if (mode == "scouttest")
		{
			if (debug)
			{
				std::cout << "score is " << (std::string)score << " best score is " << (std::string)bestScore << std::endl;
			}
			bestScore = score.Negate();
		}
		else if(mode == "minimax")
		{
			if (debug)
			{
				std::cout << "score is " << (std::string)score << " best score is " << (std::string)bestScore << std::endl;
			}
			if (bestScore < score.Negate())
			{
				bestScore = score.Negate();
			}
		}

		waits.erase(jobId);
		results.pop_front();
	}

	ready = true;
	cv.notify_all();
	//std::cout << "Tick::unlock" << std::endl;

	//std::cout << " " << jobs.size() << " " << waits.size() << " " << results.size() << std::endl;
	if( 0 < jobs.size() || 0 < waits.size() || 0 < results.size() )
	{
		return false;
	}
	
	if (debug)
	{
		std::cout << "-> best score is " << (std::string)bestScore << std::endl;
	}
	
	return true;
}

void Ai::Stop()
{
	isStop = true;
}