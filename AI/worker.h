#ifndef WORKER_H
#define WORKER_H

class Ai;

class Worker
{
public:
	Worker(Ai &aiValue);
	
	void Start();
	void Join();
		
private:
#if USE_PRIORITY == PRIORITY_MULTISET
	struct NODE
	{
		std::multiset<Board::PAWN_MOVE> moves;
		int score;
	};
#else
	struct NODE
	{
		std::list<Board::PAWN_MOVE> moves;
		int score;
	};
#endif

	Ai *ai;
	Board board;
	bool state;
	std::thread th;

	bool debug = true;

	void Search();
	void SearchImplementation(const std::string &job);
};

#endif // WORKER_H