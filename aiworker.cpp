#include <iostream>
#include <sstream>
#include <iomanip>
#include <list>
#include <set>
#include <unordered_map>
#include <thread>
#include <mutex>
#include <algorithm>
#include "core/json.hpp"
#include "Shogi/definitions.h"
#include "Shogi/move.hpp"
#include "Shogi/board.hpp"
#include "core/score.hpp"
#include "core/worker.hpp"
#include "aiworker.h"
#include "workercontroller.h"
#include "ai.h"

AiWorker::AiWorker(Ai *aiValue) : Worker()
{
	ai = aiValue;
	board = std::shared_ptr<Board>(new Board());
}

void AiWorker::CallBack(const std::string &str)
{
	ai->CallBack(str);
}
void AiWorker::GetJob(std::string &job)
{
	ai->GetJob(job);
}
bool AiWorker::IsAlive(const std::string &jobId)
{
	return ai->IsAlive(jobId);
}