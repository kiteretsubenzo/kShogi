#ifndef AI_H
#define AI_H

class Worker;

class Ai
{
public:
	struct JOB
	{
		unsigned int jobId;
		std::list<Move> moves;
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
	void SetLimit(const bool &limitValue) { limit = limitValue; }
	void SetDebug(const bool &debugValue) { debug = debugValue; }
	void Start(Board boardValue);
	void CallBack(const std::string &str);
	void GetJob(std::string &job);
	bool IsAlive(const std::string &jobId);
	
	void GetResult(Score &scoreValue);

	bool Tick();

	void Stop();
	
private:
	Board board;
	
	std::string mode = "minimax";
	Score searchScore = Score(std::numeric_limits<int>::min());
	bool limit = false;
	bool debug = true;

	Score bestScore = SCORE_NONE;
	
	std::list<std::shared_ptr<WorkerController>> workers;
	std::mutex mtx;
	std::condition_variable cv;
	bool ready = true;

	std::list<JOB> jobs;
	std::unordered_map<std::string, std::list<Move>> waits;
	std::list<std::string> results;

	unsigned int jobId = 0;
	unsigned int GetJobId() { return jobId++; }
  
	bool isStop = false;
};

#endif // AI_H