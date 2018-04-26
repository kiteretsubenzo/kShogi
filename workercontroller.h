#ifndef WORKER_CONTROLLER_H
#define WORKER_CONTROLLER_H

class WorkerController
{
public:
	WorkerController(Ai *aiValue);
	~WorkerController();

	void Start();
	void Stop();

private:
	std::thread th;
	bool isStart = false;

	std::shared_ptr<AiWorker> worker = nullptr;

	void SearchLoop();
};

#endif // WORKER_CONTROLLER_H