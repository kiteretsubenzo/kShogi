#include <iostream>
#include <thread>
#include <vector>
#include <list>
#include <algorithm>
#include <mutex>
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
		
		std::cout << "#prepare thread start" << std::endl;
		std::this_thread::sleep_for(std::chrono::seconds(3));
		std::cout << "#prepare thread finished" << std::endl;

		ai->CallBack("1");
	}
	
}

void Worker::Join()
{
	th.join();
}