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
	void CallBack(const std::string &str);
	void GetJob(std::string &job);
	
	void Stop();
	
private:
	Board board;
	Board::PAWN_MOVE move;
	
	Worker *worker;

	std::list<std::string> jobs;
  std::list<std::string> waits;
	std::list<std::string> results;
	
	std::mutex mtx;
  
  bool isStop;
};

#endif // AI_H