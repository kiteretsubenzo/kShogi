#ifndef WORKER_H
#define WORKER_H

class Ai;

class Worker
{
public:
	std::shared_ptr<Board> board = nullptr;

	virtual void CallBack(const std::string &str) = 0;
	virtual void GetJob(std::string &job) = 0;
	virtual bool IsAlive(const std::string &jobId) = 0;

	bool Search();
		
private:
	struct NODE
	{
		MoveList moves;
		Score score;
	};

	enum STATE
	{
		IDLE,
		SEARCH
	};

	STATE state = IDLE;

	std::string jobId = "";
	bool debug = true;
	Score window = SCORE_NONE;
	unsigned int deep = 0;
	bool limit = false;

	std::list<NODE> nodeStack;

	void SearchInit(const std::string &job);
	bool SearchImplementation();
};

#endif // WORKER_H