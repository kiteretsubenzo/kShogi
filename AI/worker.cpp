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
#include "../definitions.h"
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
		
		//std::cout << job << std::endl;
		size_t index = job.find("\n");
		std::string jobId = job.substr(0, index);
		std::cout << "jobId is " << jobId << std::endl;
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
		nodeStack.push_back({board.GetMoveList(), -SCORE_NONE});	// 自分
		std::vector<Board::PAWN_MOVE> dummy;
		// TODO: {} でいけないか？
		nodeStack.push_back({dummy, -SCORE_NONE});	// ダミーの子供
		
		while( true )
		{
			for( std::list<NODE>::iterator ite=nodeStack.begin(); ite != nodeStack.end(); ++ite )
			{
				std::cout << ":" << (ite->moves.front()).DebugString() << "(" << ite->score << ")";
			}
			std::cout << std::endl;
			
			// 子ノードを引っこ抜く
			NODE child = nodeStack.back();
			// とりあえず子ノードの着手を戻す
			if( !child.moves.empty() )
			{
				board.Back(child.moves.front());
				//child.moves.pop_front();
				child.moves.erase(child.moves.begin());
			}
			// 次の指し手を取得
			if( child.moves.empty() )
			{
				// 指し手が無いので今のノードは終わり
				nodeStack.pop_back();
				
				// 親ノードが居なかったらルートノードなので終わり
				if( nodeStack.empty() )
				{
					ai->CallBack(jobId + "\n" + std::to_string(child.score));
					break;
				}
				
				// 親ノードに得点をマージ
				std::list<NODE>::reverse_iterator parent = nodeStack.rbegin();
				//std::cout << parent->score << ", " << -child.score << ", " << child.score << ", " << child.score*-1 << std::endl;
				parent->score = std::min<int>(parent->score, -child.score);

				// スコアがwindowの外側だったら終わり
				/*
				if( parent->score != -SCORE_NONE && parent->score != SCORE_NONE && (parent->score <= windowMin || windowMax <= parent->score) )
				{
					continue;
				}
				*/
				
				continue;
			}
			Board::PAWN_MOVE next = child.moves.front();
			//child.move.pop_front();
			child.moves.erase(child.moves.begin());
			// 盤面を進める
			board.Move(next);

			
			
			// 着手を取得
			std::vector<Board::PAWN_MOVE> moveList = board.GetMoveList();
			
			int score = -SCORE_NONE;
			
			// 新しい盤面に着手が無かったら勝負あり
			if( moveList.empty() )
			{
				//board.PrintBoard();
				//std::cout << "hohohohoh " << ite->score;
				//std::cout << " -> " << ite->score << std::endl;
				score = SCORE_WIN;
			}
			else if( 4 <= nodeStack.size() )
			{
				// 新しい子が末端だったら追加せずに評価
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
				moveList.clear();
				score = -1;
			}
			
			// 子供を追加
			nodeStack.push_back({moveList, score});
			
			
		}
	}
}

void Worker::Join()
{
	th.join();
}