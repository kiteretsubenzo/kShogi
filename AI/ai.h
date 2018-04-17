#ifndef AI_H
#define AI_H

class Worker;

class Ai
{
public:
	struct JOB
	{
		unsigned int jobId;
		std::list<PAWN_MOVE> moves;
		Score window;
		int deep;
		Board board;
	};

	Ai();
	~Ai();

	void AddWorker();
	
	void SetBoard(const Board &boardValue) { board = boardValue; }
	void SetMode(const std::string &modeValue) { mode = modeValue; }
	void SetSearchScore(const Score &score) { searchScore = score; }
	void SetDebug(const bool &debugValue) { debug = debugValue; }
	void Start(Board board);
	bool Tick();
	PAWN_MOVE GetMove() const { return move; }
	void CallBack(const std::string &str);
	void GetJob(std::string &job);
	bool IsAlive(const std::string &jobId);
	
	void GetResult(PAWN_MOVE &moveValue, Score &scoreValue);

	void Stop();
	
private:
	Board board;
	PAWN_MOVE move;
	
	std::string mode = "minimax";
	Score searchScore = Score(std::numeric_limits<int>::min());
	bool debug = true;

	PAWN_MOVE bestMove;
	Score bestScore = SCORE_NONE;
	
	//Worker *worker;
	std::list<Worker> workers;
	std::mutex mtx;
	std::condition_variable cv;
	bool ready = true;

	std::list<JOB> jobs;
	std::unordered_map<std::string, std::list<PAWN_MOVE>> waits;
	std::list<std::string> results;

	unsigned int jobId = 0;
	unsigned int GetJobId() { return jobId++; }
  
	bool isStop = false;
};

#endif // AI_H