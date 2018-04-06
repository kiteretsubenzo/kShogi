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
		
		std::vector<std::string> strs = split(job, ':');
		std::string jobId = strs[0];
		std::string windowStr = strs[1];
		std::string deepStr = strs[2];
		std::string boardStr = strs[3];
		board.Init(boardStr);
		board.PrintBoard();

		int window = SCORE_NONE;
		if (windowStr != "none")
		{
			window = std::stoi(windowStr);
		}
		const int deep = std::stoi(deepStr);
		
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
			
			bool debugPrint = true;
			/*
			if ( !top->moves.empty() )
			{
				Board::PAWN_MOVE moveTop = *(top->moves.begin());
				if (moveTop.tox == 8 && moveTop.toy == 3 && moveTop.fromPawn == PAWN_TYPE::UMA)
				{
					debugPrint = true;
				}
			}
			*/

			if( debugPrint )
			{
				//std::cout << '\r' << std::flush;
				for( std::list<NODE>::iterator ite=nodeStack.begin(); ite != nodeStack.end(); ++ite )
				{
					if( 0 < ite->moves.size() )
					{
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
			if( 2 <= nodeStack.size() && childItr->score != SCORE_NONE )
			{
				std::list<NODE>::reverse_iterator parentItr = std::next(nodeStack.rbegin());
				//if( debugPrint )
				//{
					//std::cout << parentItr->score << " " << -childItr->score << std::endl;
				//}

				if( parentItr->score == SCORE_NONE )
				{
					parentItr->score = -childItr->score;
				}
				else
				{
					parentItr->score = std::min<int>(parentItr->score, -childItr->score);
				}
			}

			// とりあえず子ノードの着手を戻す
			if (childItr->moves.front() != PAWN_MOVE_ZERO)
			{
				board.Back(childItr->moves.front());
			}
			
			// スコアがwindowの外側だったら終わり
			if ( childItr->score != SCORE_NONE && window != SCORE_NONE)
			{
				int windowTmp = window;

				if ((nodeStack.size() & 01) == 1)
				{
					windowTmp = -window;
				}

				if (windowTmp < childItr->score)
				{
					childItr->moves.erase(childItr->moves.begin() + 1, childItr->moves.end());
					std::cout << "cut" << std::endl;
				}
			}
			
			// 次の指し手を取得
			childItr->moves.erase(childItr->moves.begin());
			if( childItr->moves.empty() )
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
				
				continue;
			}
			
			childItr->score = SCORE_NONE;
			
			// 盤面を進める
			board.Move(*(childItr->moves.cbegin()));
			
			// 着手を取得
			std::vector<Board::PAWN_MOVE> moveList = board.GetMoveList();

			// 新しい盤面に着手が無かったら勝負あり
			if( moveList.empty() )
			{
				childItr->score = SCORE_WIN;
				continue;
			}

			// 新しい子が末端だったら追加せずに評価
			if( deep <= nodeStack.size() )
			{
				int score = SCORE_NONE;

				// 評価
				score = moveList.size();

				// 親ノードに得点をマージ
				if( score != SCORE_NONE )
				{
					childItr->score = std::min<int>(childItr->score, -score);
				}
				
				continue;
			}

			// 子供を追加
			moveList.insert(moveList.cbegin(), PAWN_MOVE_ZERO);
			nodeStack.push_back({std::move(moveList), SCORE_NONE});
		}
	}
}

void Worker::Join()
{
	th.join();
}