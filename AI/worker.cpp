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
		nodeStack.push_back({{}, SCORE_NONE}); // ルート
		std::vector<Board::PAWN_MOVE> tmpMoveList = board.GetMoveList();
		tmpMoveList.insert(tmpMoveList.begin(), PAWN_MOVE_ZERO);	// ダミーの子供
		nodeStack.push_back({tmpMoveList, SCORE_NONE});	// 自分
		
		std::cout << std::endl;
		while( true )
		{
			std::list<NODE>::iterator top = nodeStack.begin();
			Board::PAWN_MOVE moveTop = *(top->moves.begin());
			bool debugPrint = true;
			if( moveTop.tox == 8 && moveTop.toy == 3 && moveTop.fromPawn == PAWN_TYPE::UMA )
			{
				debugPrint = true;
			}
			
			if( debugPrint )
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
			}

			// 子ノードを引っこ抜く
			std::list<NODE>::reverse_iterator childItr = nodeStack.rbegin();
			
			// 親ノードに得点をマージ
			std::list<NODE>::reverse_iterator parentItr = childItr;
			if( parentItr != nodeStack.rend() )
			{
				++parentItr;
			}

			if( parentItr != nodeStack.rend() )
			{
				//if( debugPrint )
				//{
					//std::cout << parentItr->score << " " << -childItr->score << std::endl;
				//}
				
				if( childItr->score != SCORE_NONE )
				{
					if( parentItr->score == SCORE_NONE )
					{
						parentItr->score = -childItr->score;
					}
					else
					{
						parentItr->score = std::min<int>(parentItr->score, -childItr->score);
					}
				}
			}

			// とりあえず子ノードの着手を戻す
			if( !childItr->moves.empty() )
			{
				if( childItr->moves.front() != PAWN_MOVE_ZERO )
				{
					board.Back(childItr->moves.front());
				}
			}

			// 次の指し手を取得
			std::vector<Board::PAWN_MOVE>::iterator nextMoveItr = childItr->moves.erase(childItr->moves.begin());
			if( nextMoveItr == childItr->moves.end() )
			{
				//std::cout << "end" << nodeStack.size() << std::endl;
				// 次の指し手が無いので今のノードは終わり
				nodeStack.pop_back();
				// ルートノードなので終わり
				if( nodeStack.size() <= 1 )
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
			//std::cout << "continue" << std::endl;
			
			childItr->score = SCORE_NONE;
			
			// 盤面を進める
			board.Move(*nextMoveItr);
			
			// 着手を取得
			std::vector<Board::PAWN_MOVE> moveList = board.GetMoveList();

			// 新しい盤面に着手が無かったら勝負あり
			if( moveList.empty() )
			{
				childItr->score = SCORE_WIN;
				continue;
			}

			if( 4 <= nodeStack.size() )
			{
				// 新しい子が末端だったら追加せずに評価
				int score = SCORE_NONE;

				// TODO: 相手が置ける場所の数
				// 評価
				score = 1;

				// 親ノードに得点をマージ
				if( score != SCORE_NONE )
				{
					childItr->score = std::min<int>(childItr->score, -score);
				}
				
				continue;
			}

			// 子供を追加
			moveList.insert(moveList.begin(), PAWN_MOVE_ZERO);
			nodeStack.push_back({moveList, SCORE_NONE});
		}
	}
}

void Worker::Join()
{
	th.join();
}