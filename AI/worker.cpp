#include <iostream>
#include <thread>
#include <vector>
#include <list>
#include <algorithm>
#include <mutex>
#include <unordered_map>
#include <random>
#include "../board.h"
#include "worker.h"
#include "ai.h"

Worker::Worker(Ai &aiValue)
{
	ai = &aiValue;
}

void Worker::Start()
{
	state = false;
	th = std::thread(&Worker::Test, this);
}

void Worker::Test()
{
	while(true)
	{
		std::string job;
    ai->GetJob(job);
		if( job == "empty" )
		{
			continue;
		}
    if( job == "stop" )
    {
      break;
    }
		
		std::cout << job << std::endl;
		
		size_t index = job.find("\n");
		std::string jobId = job.substr(0, index);
		std::cout << "jobId is " << jobId << std::endl;
		
		std::cout << "#prepare thread start" << std::endl;
		std::this_thread::sleep_for(std::chrono::seconds(1));
		std::cout << "#prepare thread finished" << std::endl;

		int score = rnd() % 100 + 1;
		ai->CallBack(jobId + "\n" + std::to_string(score));
	}
}

void Worker::Join()
{
	th.join();
}