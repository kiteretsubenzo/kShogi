#ifndef AI_WORKER_H
#define AI_WORKER_H

class AiWorker : public Worker
{
public:
	AiWorker(Ai *aiValue);

public:
	void CallBack(const std::string &str);
	void GetJob(std::string &job);
	bool IsAlive(const std::string &jobId);

private:
	Ai *ai = nullptr;
};

#endif // AI_WORKER_H