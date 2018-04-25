#ifndef WORKER_H
#define WORKER_H

class Ai;

class Worker
{
public:
	void Start();
	void Stop();

public:
	std::shared_ptr<Board> board = nullptr;

	virtual void CallBack(const std::string &str) = 0;
	virtual void GetJob(std::string &job) = 0;
	virtual bool IsAlive(const std::string &jobId) = 0;
		
private:
	struct NODE
	{
		MoveList moves;
		Score score;
	};

	std::thread th;
	bool isStart = false;

	std::string jobId = "";
	bool debug = true;
	Score window = SCORE_NONE;
	unsigned int deep = 0;
	bool limit = false;

	std::list<NODE> nodeStack;

	void Search();
	void SearchInit(const std::string &job);
	bool SearchImplementation();
};

#endif // WORKER_H