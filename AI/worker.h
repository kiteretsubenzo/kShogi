﻿#ifndef WORKER_H
#define WORKER_H

class Ai;

class Worker
{
public:
	Worker(Ai &aiValue);
	~Worker();
	
	void Start();
	void Join();
		
private:
	struct NODE
	{
		MoveList moves;
		int score;
	};

	Ai *ai;
	Board board;
	bool state;
	std::thread th;
	bool threadCreated = false;

	bool debug = true;

	void Search();
	void SearchImplementation(const std::string &job);
};

#endif // WORKER_H