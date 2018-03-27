#ifndef AI_H
#define AI_H

class Worker;

class Ai
{
public:
	struct JOB
	{
		unsigned int jobId;
		std::list<Board::PAWN_MOVE> moves;
		Board board;
	};

	Ai();
	~Ai();
	
	void SetBoard(const Board &boardValue) { board = boardValue; }
	void Start(Board board);
	bool Tick();
	Board::PAWN_MOVE GetMove() const { return move; }
	void CallBack(const std::string &str);
	void GetJob(std::string &job);
	
	Board::PAWN_MOVE GetResult() { return bestMove; }

	void Stop();
	
private:
	Board board;
	Board::PAWN_MOVE move;
	
	Board::PAWN_MOVE bestMove;
	int bestScore;
	
	Worker *worker;
	std::mutex mtx;

	std::list<JOB> jobs;
	std::unordered_map<std::string, std::list<Board::PAWN_MOVE>> waits;
	std::list<std::string> results;

	unsigned int jobId = 0;
	unsigned int GetJobId() { return jobId++; }
  
  bool isStop;
};

#endif // AI_H