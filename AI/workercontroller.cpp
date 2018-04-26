#include <iostream>
#include <sstream>
#include <list>
#include <vector>
#include <unordered_map>
#include <thread>
#include "../definitions.h"
#include "../pawnmove.h"
#include "../board.h"
#include "score.h"
#include "worker.h"
#include "aiworker.h"
#include "workercontroller.h"

WorkerController::WorkerController(Ai *aiValue)
{
	worker = std::shared_ptr<AiWorker>(new AiWorker(aiValue));
}

WorkerController::~WorkerController()
{
	Stop();
}

void WorkerController::Start()
{
	th = std::thread(&WorkerController::SearchLoop, this);
	isStart = true;
}

void WorkerController::Stop()
{
	if (isStart)
	{
		th.join();
		isStart = false;
	}
}

void WorkerController::SearchLoop()
{
	while (worker->Search() == false)
	{
		// ���܂ɃX���[�v�����
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}
}
