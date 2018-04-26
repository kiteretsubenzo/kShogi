#include <iostream>
#include <sstream>
#include <list>
#include <vector>
#include <unordered_map>
#include <thread>
#include "core/definitions.h"
#include "core/pawnmove.h"
#include "core/board.h"
#include "core/score.h"
#include "core/worker.h"
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
		// ÇΩÇ‹Ç…ÉXÉäÅ[Évì¸ÇÍÇÈ
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}
}
