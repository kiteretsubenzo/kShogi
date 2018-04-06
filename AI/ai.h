﻿#ifndef AI_H
#define AI_H

class Worker;

class Ai
{
public:
	struct JOB
	{
		unsigned int jobId;
		std::list<Board::PAWN_MOVE> moves;
		int window;
		int deep;
		Board board;
	};

	Ai();
	~Ai();
	
	void SetBoard(const Board &boardValue) { board = boardValue; }
	void SetMode(const std::string &modeValue) { mode = modeValue; }
	void SetDebug(const bool &debugValue) { debug = debugValue; }
	void Start(Board board);
	bool Tick();
	Board::PAWN_MOVE GetMove() const { return move; }
	void CallBack(const std::string &str);
	void GetJob(std::string &job);
	
	void GetResult(Board::PAWN_MOVE &moveValue, int &scoreValue);

	void Stop();
	
private:
	Board board;
	Board::PAWN_MOVE move;
	
	std::string mode = "minimax";
	bool debug = true;

	Board::PAWN_MOVE bestMove;
	int bestScore;
	
	Worker *worker;
	std::mutex mtx;

	std::list<JOB> jobs;
	std::unordered_map<std::string, std::list<Board::PAWN_MOVE>> waits;
	std::list<std::string> results;

	unsigned int jobId = 0;
	unsigned int GetJobId() { return jobId++; }
  
	bool isStop;
};

#endif // AI_H