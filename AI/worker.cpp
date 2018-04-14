#include <iostream>
#include <sstream>
#include <iomanip>
#include <list>
#include <unordered_map>
#include <set>
#include <algorithm>
#include <thread>
#include <mutex>
#include <random>
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
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
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

		/*
		std::cout << "#prepare thread start" << std::endl;
		std::cout << "#prepare thread finished" << std::endl;
		int score = rnd() % 100 + 1;
		*/

		SearchImplementation(job);
	}
}

void Worker::SearchImplementation(const std::string &job)
{
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
		//board.PrintBoard();
	}

	int window = SCORE_NONE;
	if (windowStr != "none")
	{
		window = std::stoi(windowStr);
	}
	const unsigned int deep = std::stoi(deepStr);

	std::list<NODE> nodeStack;
	// ルート
	nodeStack.push_back({ {}, SCORE_NONE });
	// 自分
	nodeStack.push_back({ board.GetMoveList(), SCORE_NONE });

	int count = 0;
	while (true)
	{
		count++;

		if ((count & 0xffff) == 0)
		{
			// たまにjobが生きているか確認
			if (ai->IsAlive(jobId) == false)
			{
				break;
			}

			// たまにスリープ入れる
			std::this_thread::sleep_for(std::chrono::milliseconds(1));
		}

		std::list<NODE>::iterator top = nodeStack.begin();

		bool debugPrint = true && debug;
		/*
		if ( 1 < nodeStack.size() )
		{
			std::list<NODE>::iterator next = ++top;
			if (next != nodeStack.end())
			{
				Board::PAWN_MOVE moveTop = *(top->moves.begin());
				if (moveTop.to.x == 7 && moveTop.to.y == 0 && moveTop.from.pawn == PAWN_RYU)
				{
					debugPrint = true;
				}
			}
		}
		*/
		if (debugPrint)
		{
			//std::cout << '\r' << std::flush;
			for (std::list<NODE>::iterator ite = nodeStack.begin(); ite != nodeStack.end(); ++ite)
			{
				if (0 < ite->moves.size())
				{
#if USE_PRIORITY == PRIORITY_MULTISET
					std::cout << ":" << ite->moves.begin()->DebugString() << "(" << (int)(ite->score) << ")";
#else
					std::cout << ":" << ite->moves.front().DebugString() << "(" << (int)(ite->score) << ")";
#endif
				}
				else
				{
					std::cout << ":EMPTY(" << (int)(ite->score) << ")";
				}
			}
			std::cout << std::endl;
		}

		// forward
		while (true)
		{
			// 子ノードを取得
			std::list<NODE>::reverse_iterator childItr = nodeStack.rbegin();

			// 盤面を進める
#if USE_PRIORITY == PRIORITY_MULTISET
			board.Move(*(childItr->moves.begin()));
#else
			board.Move(childItr->moves.front());
#endif
			// 着手を取得
			MoveList moveList = board.GetMoveList();

			// 新しい盤面に着手が無かったら勝負あり
			if (moveList.empty())
			{
				childItr->score = SCORE_WIN;
				break;
			}

			if (deep <= nodeStack.size())
			{
				// 新しい子が末端だったら追加せずに評価
				int score = SCORE_NONE;

				// 評価
				score = board.GetEvaluate(moveList);

				// 親ノードに得点をマージ
				if (score != SCORE_NONE)
				{
					childItr->score = std::min<int>(childItr->score, -score);
				}

				break;
			}

			// 子供を追加してもう一回
			nodeStack.push_back({ std::move(moveList), SCORE_NONE });
		}

		// back
		while (true)
		{
			// 子ノードを取得
			std::list<NODE>::reverse_iterator childItr = nodeStack.rbegin();

			// 親ノードに得点をマージ
			if (2 <= nodeStack.size() && childItr->score != SCORE_NONE)
			{
				std::list<NODE>::reverse_iterator parentItr = std::next(nodeStack.rbegin());
				//if( debugPrint )
				//{
				//std::cout << parentItr->score << " " << -childItr->score << std::endl;
				//}

				if (parentItr->score == SCORE_NONE)
				{
					parentItr->score = -childItr->score;
				}
				else
				{
					parentItr->score = std::min<int>(parentItr->score, -childItr->score);
				}
			}

			// 子ノードの着手を戻す
#if USE_PRIORITY == PRIORITY_MULTISET
			board.Back(*(childItr->moves.begin()));
#else
			board.Back(childItr->moves.front());
#endif

			// スコアがwindowの外側だったら終わり
			if (childItr->score != SCORE_NONE && window != SCORE_NONE)
			{
				int windowTmp = window;

				if ((nodeStack.size() & 01) == 1)
				{
					windowTmp = -window;
				}

				if (windowTmp < childItr->score)
				{
					childItr->moves.clear();
				}
			}

			// 次の指し手を取得
			if (1 < childItr->moves.size())
			{
				childItr->moves.erase(childItr->moves.begin());
				childItr->score = SCORE_NONE;
				break;
			}

			// 次の指し手が無いので今のノードは終わり
			nodeStack.pop_back();
			//std::cout << "end" << nodeStack.size() << std::endl;

			// ルートノードなので終わり
			if (nodeStack.size() <= 1)
			{
				ai->CallBack("jobid:" + jobId + ",score:" + std::to_string(childItr->score) + ",count:" + std::to_string(count));
				return;
			}
		}
	}
}

void Worker::Join()
{
	th.join();
}