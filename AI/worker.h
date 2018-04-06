#ifndef WORKER_H
#define WORKER_H

static int SCORE_NONE = std::numeric_limits<int>::max() - 1;
static int SCORE_WIN = 99999;

class Ai;

class Worker
{
public:
	struct NODE
	{
		std::vector<Board::PAWN_MOVE> moves;
		int score;
	};
	
	Worker(Ai &aiValue);
	
	void Start();
	void Join();
	void Test();
		
private:
	Ai *ai;
	Board board;
	bool state;
	std::thread th;
	
	std::random_device rnd;
};

#endif // WORKER_H