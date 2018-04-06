#ifndef WORKER_H
#define WORKER_H

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