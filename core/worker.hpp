#ifndef WORKER_H
#define WORKER_H

class Ai;

class Worker
{
public:
	std::shared_ptr<Board> board = nullptr;

	virtual void CallBack(const std::string &str) = 0;
	virtual void GetJob(std::string &job) = 0;
	virtual bool IsAlive(const std::string &jobId) = 0;

	bool Search()
	{
		switch (state)
		{
		case IDLE:
		{
			std::string job;
			GetJob(job);
			if (job == "empty")
			{
				return false;
			}
			if (job == "stop")
			{
				return true;
			}

			SearchInit(job);

			state = SEARCH;
		}
		break;

		case SEARCH:
			//jobが生きているか確認
			if (IsAlive(jobId) == false)
			{
				state = IDLE;
				break;
			}

			if (SearchImplementation() == true)
			{
				state = IDLE;
			}
			break;

		default:
			break;
		}

		return false;
	}
		
private:
	struct NODE
	{
		MoveList moves;
		Score score;
	};

	enum STATE
	{
		IDLE,
		SEARCH
	};

	STATE state = IDLE;

	std::string jobId = "";
	bool debug = true;
	Score window = SCORE_NONE;
	unsigned int deep = 0;
	bool limit = false;

	std::list<NODE> nodeStack;

	void SearchInit(const std::string &job)
	{
		std::unordered_map<std::string, std::string> params = fromJson(job);
		std::string windowStr = params["window"];
		std::string limitStr = params["limit"];
		std::string deepStr = params["deep"];
		std::string debugStr = params["debug"];
		std::string boardStr = params["board"];

		jobId = params["jobid"];

		debug = (debugStr == "true");

		board->Init(boardStr);
		if (debug)
		{
			//board->PrintBoard();
		}

		if (windowStr != "")
		{
			window = Score(windowStr);
		}
		else
		{
			window = SCORE_NONE;
		}

		deep = std::stoi(deepStr);
		limit = (limitStr == "true");

		nodeStack.clear();
		// ルート
		nodeStack.push_back({ {}, SCORE_NONE });
		// 自分
		nodeStack.push_back({ board->GetMoveList(), SCORE_NONE });
	}

	bool SearchImplementation()
	{
		for (int i = 0; i < 0xffff; i++)
		{
			bool debugPrint = true && debug;
			/*
			if ( 1 < nodeStack.size() )
			{
			std::list<NODE>::iterator top = nodeStack.begin();
			std::list<NODE>::iterator next = ++top;
			if (next != nodeStack.end())
			{
			Board::MOVE moveTop = *(top->moves.begin());
			if (moveTop.to.x == 7 && moveTop.to.y == 0 && moveTop.from.pawn == PAWN_RYU)
			{
			debugPrint = true;
			}
			}
			}
			*/

			// forward
			while (true)
			{
				if (debugPrint)
				{
					//std::cout << '\r' << std::flush;
					for (std::list<NODE>::iterator ite = nodeStack.begin(); ite != nodeStack.end(); ++ite)
					{
						if (0 < ite->moves.size())
						{
							std::cout << ":" << ite->moves.front().DebugString() << "(" << (std::string)(ite->score) << ")";
						}
						else
						{
							std::cout << ":EMPTY(" << (std::string)(ite->score) << ")";
						}
					}
					std::cout << std::endl;
				}

				// 子ノードを取得
				std::list<NODE>::reverse_iterator childItr = nodeStack.rbegin();

				// 盤面を進める
				board->Move(childItr->moves.front());

				// 着手を取得
				MoveList moveList = board->GetMoveList();

				// 新しい盤面に着手が無かったら勝負あり
				if (moveList.empty())
				{
					childItr->score = SCORE_WIN;
					childItr->score.moveList.clear();
					for (std::list<NODE>::const_iterator ite = nodeStack.cbegin(); ite != nodeStack.cend(); ++ite)
					{
						if (0 < ite->moves.size())
						{
							childItr->score.moveList.push_back(ite->moves.front());
						}
					}
					break;
				}

				if (deep <= nodeStack.size())
				{
					// 新しい子が末端だったら追加せずに評価
					Score score = SCORE_NONE;

					// 評価
					score = board->GetEvaluate(moveList);
					score.moveList.clear();
					for (std::list<NODE>::const_iterator ite = nodeStack.cbegin(); ite != nodeStack.cend(); ++ite)
					{
						if (0 < ite->moves.size())
						{
							childItr->score.moveList.push_back(ite->moves.front());
						}
					}

					// 親ノードに得点をマージ
					if (score != SCORE_NONE || (limit == true && (window.Negate() == childItr->score || window.Negate() < childItr->score)))
					{
						childItr->score = std::min<Score>(childItr->score, score.Negate());
					}

					break;
				}

				// 子供を追加してもう一回
				nodeStack.push_back({ std::move(moveList), SCORE_NONE });
			}

			// back
			while (true)
			{
				if (debugPrint)
				{
					//std::cout << '\r' << std::flush;
					for (std::list<NODE>::iterator ite = nodeStack.begin(); ite != nodeStack.end(); ++ite)
					{
						if (0 < ite->moves.size())
						{
							std::cout << ":" << ite->moves.front().DebugString() << "(" << (std::string)(ite->score) << ")";
						}
						else
						{
							std::cout << ":EMPTY(" << (std::string)(ite->score) << ")";
						}
					}
					std::cout << std::endl;
				}

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

					if (parentItr->score == SCORE_NONE || (limit == true && (window.Negate() == parentItr->score || window.Negate() < parentItr->score)))
					{
						parentItr->score = childItr->score.Negate();
					}
					else
					{
						parentItr->score = std::min<Score>(parentItr->score, childItr->score.Negate());
					}
				}

				// 子ノードの着手を戻す
				board->Back(childItr->moves.front());

				// スコアがwindowの外側だったら終わり
				if (childItr->score != SCORE_NONE && window != SCORE_NONE && (limit == false || childItr->score < window.Negate()))
				{
					Score windowTmp = window;

					if ((nodeStack.size() & 01) == 1)
					{
						windowTmp = window.Negate();
					}

					if (windowTmp < childItr->score)
					{
						childItr->moves.clear();
					}
				}

				// 次の指し手を取得
				if (1 < childItr->moves.size())
				{
					childItr->moves.pop_front();
					childItr->score = SCORE_NONE;
					break;
				}

				// 次の指し手が無いので今のノードは終わり
				nodeStack.pop_back();
				//std::cout << "end" << nodeStack.size() << std::endl;

				// ルートノードなので終わり
				if (nodeStack.size() <= 1)
				{
					CallBack("jobid:" + jobId + ",score:" + childItr->score.toJson() + ",count:" + std::to_string(i));
					return true;
				}
			}
		}

		// いったんお返し
		return false;
	}
};

#endif // WORKER_H