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
		std::vector<Board::PAWN_MOVE> tmpMoveList = board.GetMoveList();
		tmpMoveList.insert(tmpMoveList.begin(), PAWN_MOVE_ZERO);	// ダミーの子供
		nodeStack.push_back({tmpMoveList, -SCORE_NONE});	// 自分
		
		while( true )
		{
			for( std::list<NODE>::iterator ite=nodeStack.begin(); ite != nodeStack.end(); ++ite )
			{
				if( 0 < ite->moves.size() )
				{
					//std::cout << ":" << (ite->moves.front()).DebugString() << "+" << (int)(ite->moves.size()-1) << "(" << (int)(ite->score) << ")";
					std::cout << ":" << (ite->moves.front()).DebugString() << "(" << (int)(ite->score) << ")";
				}
				else
				{
					std::cout << ":EMPTY(" << (int)(ite->score) << ")";
				}
			}
			std::cout << std::endl;
			
			// 子ノードを引っこ抜く
			std::list<NODE>::reverse_iterator childItr = nodeStack.rbegin();

			// とりあえず子ノードの着手を戻す
			if( !childItr->moves.empty() )
			{
				if( childItr->moves.front() != PAWN_MOVE_ZERO )
				{
					board.Back(childItr->moves.front());
				}
			}
			
			// 親ノードを取得
			std::list<NODE>::reverse_iterator parentItr = nodeStack.rbegin();
			if( parentItr != nodeStack.rend() )
			{
				++parentItr;
			}
			
			// 親ノードに得点をマージ
			if( parentItr != nodeStack.rend() )
			{
				parentItr->score = std::min<int>(parentItr->score, -childItr->score);
			}

			// 次の指し手を取得
			std::vector<Board::PAWN_MOVE>::iterator nextMoveItr = childItr->moves.erase(childItr->moves.begin());
			if( nextMoveItr == childItr->moves.end() )
			{
				// 次の指し手が無いので今のノードは終わり
				nodeStack.pop_back();

				// 誰も居なくなったらルートノードなので終わり
				if( nodeStack.empty() )
				{
					ai->CallBack(jobId + "\n" + std::to_string(childItr->score));
					break;
				}

				// スコアがwindowの外側だったら終わり
				/*
				if( parent->score != -SCORE_NONE && parent->score != SCORE_NONE && (parent->score <= windowMin || windowMax <= parent->score) )
				{
					continue;
				}
				*/
				
				continue;
			}
			
			// 盤面を進める
			board.Move(*nextMoveItr);
			
			// 着手を取得
			std::vector<Board::PAWN_MOVE> moveList = board.GetMoveList();

			int score = -SCORE_NONE;
				
			// 新しい盤面に着手が無かったら勝負あり
			if( moveList.empty() )
			{
				childItr->score = SCORE_WIN;
				continue;
			}
			if( 2 <= nodeStack.size() )
			{
				// 新しい子が末端だったら追加せずに評価
				std::list<NODE>::reverse_iterator ite = nodeStack.rbegin();
				// 点数計算
				// 相手が置ける場所の数
				// 評価
				moveList.clear();
				score = -1;
				childItr->score = std::max( childItr->score, score );
				continue;
			}

			// 子供を追加
			moveList.insert(moveList.begin(), PAWN_MOVE_ZERO);
			nodeStack.push_back({moveList, score});
		}
	}
}

void Worker::Join()
{
	th.join();
}