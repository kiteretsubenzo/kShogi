#include <iostream>
#include <thread>
#include <vector>
#include <list>
#include <algorithm>
#include <mutex>
#include <unordered_map>
#include <random>
#include <iomanip>
#include <sstream>
#include "../board.h"
#include "worker.h"
#include "ai.h"

static int SCORE_NONE = std::numeric_limits<int>::max() - 1;
static int SCORE_WIN = 99999;

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
		
		//std::cout << job << std::endl;
		size_t index = job.find("\n");
		std::string jobId = job.substr(0, index);
		//std::cout << "jobId is " << jobId << std::endl;
		std::string boardStr = job.substr(index + 1);
		board.Init(boardStr);
		board.PrintBoard();
		
		/*
		std::cout << "#prepare thread start" << std::endl;
		std::this_thread::sleep_for(std::chrono::seconds(1));
		std::cout << "#prepare thread finished" << std::endl;
		int score = rnd() % 100 + 1;
		*/
		
		std::list<NODE> nodeStack;
		nodeStack.push_back({PAWN_MOVE_ZERO, SCORE_NONE});	// 自分
		nodeStack.push_back({PAWN_MOVE_ZERO, -SCORE_NONE});	// 次の指し手
		
		while( true )
		{
			for( std::list<NODE>::iterator ite=nodeStack.begin(); ite != nodeStack.end(); ++ite )
			{
				std::cout << ":" << (std::string)(ite->move) << "(" << ite->score << ")";
			}
			std::cout << std::endl;
			
			// 子ノードを引っこ抜く
			NODE child = nodeStack.back();
			nodeStack.pop_back();
			if( child.move != PAWN_MOVE_ZERO )
			{
				board.Back(child.move);
			}
			
			// 親ノードを取得
			// 親ノードが居なかったらルートノードなので終わり
			if( nodeStack.size() == 0 )
			{
				ai->CallBack(jobId + "\n" + std::to_string(child.score));
				break;
			}
			std::list<NODE>::reverse_iterator parent = nodeStack.rbegin();
			
			// 得点をマージ
			//std::cout << parent->score << ", " << -child.score << ", " << child.score << ", " << child.score*-1 << std::endl;
			parent->score = std::min<int>(parent->score, -child.score);
			
			// 次の指し手を取得
			std::list<NODE>::reverse_iterator ite = nodeStack.rbegin();
			Board::PAWN_MOVE next = board.GetNextMove(child.move);
			if( next == PAWN_MOVE_ZERO )
			{
				// 指し手が無いので今のノードは終わり
				continue;
			}
			
			// 次の指し手を新しい子として追加
			nodeStack.push_back({next, SCORE_NONE});
			board.Move(next);
			
			// 新しい子に着手が無かったら勝負あり
			Board::PAWN_MOVE tmp = PAWN_MOVE_ZERO;
			tmp = board.GetNextMove(tmp);
			if( tmp == PAWN_MOVE_ZERO )
			{
				std::list<NODE>::reverse_iterator ite = nodeStack.rbegin();
				board.PrintBoard();
				std::cout << "hohohohoh " << ite->score;
				ite->score = SCORE_WIN;	// 自分絶対勝つ
				std::cout << " -> " << ite->score << std::endl;
				continue;
			}
			
			// 新しい子が末端だったら評価
			if( 3 <= nodeStack.size() )
			{
				std::list<NODE>::reverse_iterator ite = nodeStack.rbegin();
				// 点数計算
				// 相手が置ける場所の数
				/*
				//std::cout << "bottom" << std::endl;
				int count = 0;
				Board::PAWN_MOVE moveTmp = PAWN_MOVE_ZERO;
				while(true)
				{
					moveTmp = board.GetNextMove(moveTmp);
					if( moveTmp == PAWN_MOVE_ZERO )
					{
						break;
					}
					count++;
				}
				*/
				// 評価
				ite->score = -1;
				// 上のノードに戻る
				continue;
			}
			
			// 新しい子を追加
			nodeStack.push_back({PAWN_MOVE_ZERO, -SCORE_NONE});
		}
	}
}

void Worker::Join()
{
	th.join();
}