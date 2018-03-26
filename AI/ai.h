#ifndef AI_H
#define AI_H

class Worker;

class Ai
{
public:
	Ai();
	~Ai();
	
	void SetBoard(const Board &boardValue) { board = boardValue; }
	void Start(Board board);
	bool Tick();
	Board::PAWN_MOVE GetMove() const { return move; }
	void CallBack(std::string str);
	bool GetJob(int &job);
	
	void Join();
	
private:
	Board board;
	Board::PAWN_MOVE move;
	
	Worker *worker;

	std::list<int> jobs;
  std::list<int> waits;
	std::list<int> results;
	
	std::mutex mtx;
};

#endif // AI_H