﻿#include <iostream>
#include <sstream>
#include <iomanip>
#include <list>
#include <unordered_map>
#include <set>
#include <thread>
#include <algorithm>
#include <mutex>
#include <condition_variable>
#include <random>
#include "../definitions.h"
#include "../board.h"
#include "worker.h"
#include "ai.h"

Ai::Ai()
{
}

Ai::~Ai()
{
	Stop();
	for (std::list<Worker>::iterator ite = workers.begin(); ite != workers.end(); ++ite)
	{
		ite->Stop();
	}
}

void Ai::AddWorker()
{
	workers.push_back(Worker(this));
	workers.rbegin()->Start();
}

void Ai::Start(Board boardValue)
{
	jobs.clear();
	waits.clear();
	results.clear();
	isStop = false;
	board = boardValue;
	bestMove = PAWN_MOVE_ZERO;
	bestScore = SCORE_NONE;

	std::unique_lock<std::mutex> uniq(mtx);
	//std::cout << "Start::lock" << std::endl;
	cv.wait(uniq, [&] {return ready;});
	ready = false;
	/*
	for( unsigned int i=0; i<moveList.size(); i++ )
	{
		PAWN_MOVE &move = moveList[i];
		//board.PrintKihu(move);
		board.Move(move);
		std::list<PAWN_MOVE> moves;
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
		std::list<PAWN_MOVE> moves;
		moves.push_back(PAWN_MOVE_ZERO);
		bestScore.score = 0;
		bestScore.moveList.clear();
		JOB job = { GetJobId(), { PAWN_MOVE_ZERO }, searchScore.Negate(), limit, 4, board };
		jobs.push_back(job);
	}
	else if(mode == "minimax")
	{
		std::list<PAWN_MOVE> moves;
		moves.push_back(PAWN_MOVE_ZERO);
		JOB job = { GetJobId(), moves, SCORE_NONE, limit, 4, board };
		jobs.push_back(job);
	}
	else if (mode == "move")
	{
		MoveList moveList = board.GetMoveList();
		for (MoveList::iterator ite = moveList.begin(); ite != moveList.end(); ++ite)
		{
			const PAWN_MOVE &move = *ite;
			//board.PrintKihu(move);
			board.Move(move);
			std::list<PAWN_MOVE> moves;
			moves.push_back(move);
			JOB job = { GetJobId(), moves, searchScore, limit, 4, board };
			jobs.push_back(job);
			board.Back(move);
		}
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

	std::unordered_map<std::string, std::string> strs = fromJson(str);
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
		if (jobStruct.limit)
		{
			job += ",limit:true";
		}
		else
		{
			job += ",limit:false";
		}
		job += ",deep:" + std::to_string(jobStruct.deep);
		if (debug)
		{
			job += ",debug:true";
		}
		else
		{
			job += ",debug:false";
		}
		job += ",board:" + jobStruct.board.BoardToString();
		waits[jobIdString] = jobStruct.moves;
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

void Ai::GetResult(PAWN_MOVE &moveValue, Score &scoreValue)
{
	moveValue = bestMove;
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
		std::unordered_map<std::string, std::string> strs = fromJson(results.front());
		std::string jobId = strs["jobid"];
		std::string scoreString = strs["score"];
		std::string countString = strs["count"];
		Score score(scoreString);
		if (mode == "scout")
		{
			if (debug)
			{
				std::cout << "score is " << score.toJson() << " best score is " << bestScore.toJson() << std::endl;
			}
			std::cout << "score is " << score.toJson() << " best score is " << bestScore.toJson() << std::endl;
			if (bestScore == score)
			{
				bestMove = waits[jobId].front();
				bestScore = score.Negate();
			}
			else
			{
				bestScore = score;
				JOB job = { GetJobId(), { PAWN_MOVE_ZERO }, bestScore.Negate(), limit, 4, board };
				jobs.push_back(job);
			}
		}
		else if (mode == "scouttest")
		{
			if (debug)
			{
				std::cout << "score is " << score.toJson() << " best score is " << bestScore.toJson() << std::endl;
			}
			std::cout << "score is " << score.toJson() << " best score is " << bestScore.toJson() << std::endl;
			if (bestScore < score.Negate())
			{
				bestMove = waits[jobId].front();
				bestScore = score.Negate();
			}
		}
		else if(mode == "minimax")
		{
			if (debug)
			{
				std::cout << "score is " << (std::string)score << " best score is " << (std::string)bestScore << std::endl;
			}
			if (bestScore < score.Negate())
			{
				bestMove = waits[jobId].front();
				bestScore = score.Negate();
			}
		}
		else if (mode == "move")
		{
			if (debug)
			{
				std::cout << waits[jobId].front().DebugString() << " score is " << (std::string)score << " " << " search score is " << (std::string)searchScore << std::endl;
			}
			if (searchScore == score.Negate())
			{
				bestMove = waits[jobId].front();
				bestScore = score.Negate();
				jobs.clear();
				waits.clear();
				results.clear();
				break;
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