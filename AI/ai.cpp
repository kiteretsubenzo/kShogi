#include <iostream>
#include <list>
#include <vector>
#include <thread>
#include <algorithm>
#include <mutex>
#include <condition_variable>
#include <map>
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

	std::unique_lock<std::mutex> uniq(mtx);
	//std::cout << "Start::lock" << std::endl;
	cv.wait(uniq, [&] {return ready; });
	ready = false;
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
	
	if (mode == "scout")
	{
		std::list<Board::PAWN_MOVE> moves;
		moves.push_back(PAWN_MOVE_ZERO);
		bestScore = 0;
		JOB job = { GetJobId(), { PAWN_MOVE_ZERO }, -searchScore, 4, board };
		jobs.push_back(job);
	}
	if (mode == "scouttest")
	{
		std::list<Board::PAWN_MOVE> moves;
		moves.push_back(PAWN_MOVE_ZERO);
		bestScore = 0;
		JOB job = { GetJobId(),{ PAWN_MOVE_ZERO }, -searchScore, 4, board };
		jobs.push_back(job);
	}
	else if(mode == "minimax")
	{
		std::list<Board::PAWN_MOVE> moves;
		moves.push_back(PAWN_MOVE_ZERO);
		JOB job = { GetJobId(), moves, SCORE_NONE, 4, board };
		jobs.push_back(job);
	}
	else if (mode == "move")
	{
		std::list<Board::PAWN_MOVE> moveList = board.GetMoveList();
		for (std::list<Board::PAWN_MOVE>::iterator ite = moveList.begin(); ite != moveList.end(); ++ite)
		{
			Board::PAWN_MOVE &move = *ite;
			//board.PrintKihu(move);
			board.Move(move);
			std::list<Board::PAWN_MOVE> moves;
			moves.push_back(move);
			JOB job = { GetJobId(), moves, searchScore, 4, board };
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
	cv.wait(uniq, [&] {return ready; });

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
	cv.wait(uniq, [&] {return ready; });
	ready = false;

	if( 0 < jobs.size() )
	{
		JOB jobStruct = jobs.front();
		std::string jobIdString = std::to_string(jobStruct.jobId);
		job = "jobid:" + jobIdString;
		job += ",window:" + std::to_string(jobStruct.window);
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

		if (mode == "scouttest")
		{
			std::cout << job << std::endl;
		}
	}
	else
	{
		job = "empty";
	}

	ready = true;
	cv.notify_all();
	//std::cout << "GetJob::unlock" << std::endl;
}

void Ai::GetResult(Board::PAWN_MOVE &moveValue, int &scoreValue)
{
	moveValue = bestMove;
	scoreValue = bestScore;
}

bool Ai::Tick()
{
	std::unique_lock<std::mutex> uniq(mtx);
	//std::cout << "Tick::lock" << std::endl;
	cv.wait(uniq, [&] {return ready; });
	ready = false;

	// 結果を回収
	while( 0 < results.size() )
	{
		std::unordered_map<std::string, std::string> strs = fromJson(results.front());
		std::string jobId = strs["jobid"];
		std::string scoreString = strs["score"];
		std::string countString = strs["count"];
		int score = stoi(scoreString);
		if (mode == "scout")
		{
			if (debug)
			{
				std::cout << "score is " << score << " best score is " << bestScore << std::endl;
			}
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
		else if (mode == "scouttest")
		{
			if (debug)
			{
				std::cout << "score is " << score << " best score is " << bestScore << std::endl;
			}
			if (bestScore < -score)
			{
				bestMove = waits[jobId].front();
				bestScore = -score;
			}
		}
		else if(mode == "minimax")
		{
			if (debug)
			{
				std::cout << "score is " << score << " best score is " << bestScore << std::endl;
			}
			if (bestScore < -score)
			{
				bestMove = waits[jobId].front();
				bestScore = -score;
			}
		}
		else if (mode == "move")
		{
			if (debug)
			{
				std::cout << waits[jobId].front().DebugString() << " score is " << score << " " << " best score is " << bestScore << std::endl;
			}
			if (searchScore == -score)
			{
				bestMove = waits[jobId].front();
				bestScore = -score;
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
		std::cout << "-> best score is " << bestScore << std::endl;
	}
	
	return true;
}

void Ai::Stop()
{
	isStop = true;
	worker->Join();
}