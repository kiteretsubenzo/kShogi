#include <iostream>
#include <thread>
#include <vector>
#include <list>
#include <algorithm>
#include <mutex>
#include <map>
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
	th = std::thread(&Worker::Search, this);
}

void Worker::Search()
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
		
		std::unordered_map<std::string, std::string> params = fromJson(job);
		std::string jobId = params["jobid"];
		std::string windowStr = params["window"];
		std::string deepStr = params["deep"];
		std::string debugStr = params["debug"];
		std::string boardStr = params["board"];

		if (debugStr == "true")
		{
			debug = true;
		}
		else
		{
			debug = false;
		}

		board.Init(boardStr);
		if (debug)
		{
			board.PrintBoard();
		}

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
		// ルート
		nodeStack.push_back({{}, SCORE_NONE});
		// 自分
		nodeStack.push_back({ board.GetMoveList(), SCORE_NONE});
		nodeStack.rbegin()->moves.insert(nodeStack.rbegin()->moves.cbegin(), PAWN_MOVE_ZERO);
		
		int count = 0;
		if (debug)
		{
			std::cout << std::endl;
		}
		while( true )
		{
			count++;

			std::list<NODE>::iterator top = nodeStack.begin();
			
			bool debugPrint = false && debug;
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
						std::cout << ":" << ite->moves.front().DebugString() << "(" << (int)(ite->score) << ")";
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
					std::list<Board::PAWN_MOVE>::iterator ite = childItr->moves.begin();
					++ite;
					childItr->moves.erase(ite, childItr->moves.end());
					if (debugPrint)
					{
						std::cout << "cut" << std::endl;
					}
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
					ai->CallBack("jobid:" + jobId + ",score:" + std::to_string(childItr->score) + ",count:" + std::to_string(count));
					break;
				}
				
				continue;
			}
			
			childItr->score = SCORE_NONE;
			
			// 盤面を進める
			board.Move(childItr->moves.front());
			
			// 着手を取得
			std::list<Board::PAWN_MOVE> moveList = board.GetMoveList();
			
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
			nodeStack.push_back({std::move(moveList), SCORE_NONE});
			nodeStack.rbegin()->moves.insert(nodeStack.rbegin()->moves.cbegin(), PAWN_MOVE_ZERO);
		}
	}
}

void Worker::Join()
{
	th.join();
}