#ifndef WORKER_H
#define WORKER_H

class Ai;

class Worker
{
public:
	struct NODE
	{
		Board::PAWN_MOVE move;
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