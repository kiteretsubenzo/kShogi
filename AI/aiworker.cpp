#include <sstream>
#include <list>
#include <unordered_map>
#include <thread>
#include <mutex>
#include "../definitions.h"
#include "../pawnmove.h"
#include "../board.h"
#include "../Shogi/shogiboard.h"
#include "score.h"
#include "worker.h"
#include "ai.h"
#include "aiworker.h"

AiWorker::AiWorker(Ai *aiValue) : Worker()
{
	ai = aiValue;
	board = std::shared_ptr<Board>(new ShogiBoard());
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