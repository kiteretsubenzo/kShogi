#ifndef WORKER_H
#define WORKER_H

class Ai;

class Worker
{
public:
	Worker(Ai &aiValue);
	
	void Start();
	void Join();
	void Test();
		
private:
	Ai *ai;
	Board board;
	bool state;
	std::thread th;
};

#endif // WORKER_H