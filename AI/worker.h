#ifndef WORKER_H
#define WORKER_H

class Ai;

class Worker
{
public:
	Worker(Ai *aiValue);

	void Start();
	void Stop();
		
private:
	struct NODE
	{
		MoveList moves;
		int score;
	};

	Ai *ai = nullptr;
	Board board;

	std::thread th;
	bool isStart = false;

	bool debug = true;

	void Search();
	void SearchImplementation(const std::string &job);
};

#endif // WORKER_H