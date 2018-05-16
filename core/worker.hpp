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

	static const int DEEP_MAX = 64;
	MoveList moveListTmp;
	Score scoreTmp;

	struct Node
	{
		MoveList moves;
		Score score;

		Node()
		{
			moves.clear();
			score.clear();
		}

		void setMoves(const MoveList &movesValue)
		{
			moves.copy(movesValue);
			score.clear();
		}
	};

	class NodeStack
	{
	public:
		NodeStack() {}

		void clear()
		{
			index = -1;
		}

		void push_back(const Node &node)
		{
			index++;
			nodeStack[index] = node;
		}

		void push_back(const MoveList &moveList)
		{
			index++;
			nodeStack[index].setMoves(moveList);
		}

		unsigned int size() const
		{
			return index + 1;
		}

		Node& front()
		{
			return nodeStack[index];
		}

		Node& parent()
		{
			return nodeStack[index-1];
		}

		void pop_back()
		{
			index--;
		}

		void GetHistory(ScoreMoveList &moveList) const
		{
			moveList.clear();
			for (unsigned int i = 0; i < size(); i++)
			{
				if (0 < nodeStack[i].moves.size())
				{
					moveList.push_back(nodeStack[i].moves.front());
				}
			}
		}

		void debugPrint() const
		{
			//std::cout << '\r' << std::flush;
			std::string str = "";
			for (unsigned int i=0; i<size(); i++)
			{
				const Node& ite = nodeStack[i];
				if (0 < ite.moves.size())
				{
					str += ":" + ite.moves.front().DebugString() + "(" + (std::string)(ite.score) + ")";
				}
				else
				{
					str += ":EMPTY(" + (std::string)(ite.score) + ")";
				}
			}
			std::cout << str << std::endl;
		}

	private:
		Node nodeStack[64];
		int index = -1;
	};

	enum STATE
	{
		IDLE,
		SEARCH
	};

	STATE state = IDLE;

	std::string jobId = "";
	bool debug = true;
	Score window, windowNega;
	unsigned int deep = 0;
	Score limit;

	//std::list<Node> nodeStack;
	NodeStack nodeStack;

	void SearchInit(const std::string &job)
	{
		std::unordered_map<std::string, std::string> params = Json::fromJson(job);
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
			window.clear();
		}
		windowNega = window.Negate();

		deep = std::stoi(deepStr);

		if (limitStr != "")
		{
			limit = Score(limitStr);
		}
		else
		{
			limit.clear();
		}

		nodeStack.clear();
		// ルート
		nodeStack.push_back(Node());
		// 自分
		MoveList moveList;
		board->GetMoveList(moveList);
		nodeStack.push_back(moveList);
	}

	bool SearchImplementation()
	{
		for (int i = 0; i < 0xff; i++)
		{
			bool debugPrint = true && debug;

			// forward
			while (true)
			{
				if (debugPrint)
				{
					nodeStack.debugPrint();
				}

				// 子ノードを取得
				Node& childItr = nodeStack.front();

				// 盤面を進める
				board->Forward(childItr.moves.front());

				// 着手を取得
				board->GetMoveList(moveListTmp);

				// 新しい盤面に着手が無かったら勝負あり
				if (moveListTmp.empty())
				{
					childItr.score = Score::SCORE_WIN;
					nodeStack.GetHistory(childItr.score.moveList);
					break;
				}

				if (deep <= nodeStack.size())
				{
					// 新しい子が末端だったら追加せずに評価

					// 親ノードに評価をマージ
					if (limit.score == Score::SCORE_UNVALUED || limit <= childItr.score)
					{
						scoreTmp.setScore(board->GetEvaluate(moveListTmp));
						childItr.score = Score::NegaAndMin(childItr.score, scoreTmp);
					}

					nodeStack.GetHistory(childItr.score.moveList);

					break;
				}

				// 子供を追加してもう一回
				nodeStack.push_back(moveListTmp);
			}

			// back
			while (true)
			{
				if (debugPrint)
				{
					nodeStack.debugPrint();
				}

				// 子ノードを取得
				Node& childItr = nodeStack.front();

				// 親ノードに得点をマージ
				if (2 <= nodeStack.size())
				{
					Node& parentItr = nodeStack.parent();
					if (limit.score != Score::SCORE_UNVALUED)
					{
						if (childItr.score.Negate() < limit)
						{
							if (parentItr.score.score == Score::SCORE_UNVALUED || limit <= parentItr.score)
							{
								parentItr.score = childItr.score.Negate();
							}
							else
							{
								scoreTmp = childItr.score;
								parentItr.score = Score::NegaAndMin(parentItr.score, scoreTmp);
							}
						}
					}
					else
					{
						scoreTmp = childItr.score;
						parentItr.score = Score::NegaAndMin(parentItr.score, scoreTmp);
					}
				}

				// 子ノードの着手を戻す
				board->Back(childItr.moves.front());

				// スコアがwindowの外側だったら終わり
				if (window.score != Score::SCORE_UNVALUED && (limit.score == Score::SCORE_UNVALUED || childItr.score < limit))
				{
					if ((nodeStack.size() & 01) == 0)
					{
						if (window < childItr.score)
						{
							childItr.moves.clear();
						}
					}
					else
					{
						if (windowNega < childItr.score)
						{
							childItr.moves.clear();
						}
					}
				}

				// 次の指し手を取得
				if (1 < childItr.moves.size())
				{
					childItr.moves.pop_front();
					childItr.score.clear();
					break;
				}

				// 次の指し手が無いので今のノードは終わり
				nodeStack.pop_back();
				//std::cout << "end" << nodeStack.size() << std::endl;

				// ルートノードなので終わり
				if (nodeStack.size() <= 1)
				{
					CallBack("jobid:" + jobId + ",score:" + nodeStack.front().score.toJson() + ",count:" + std::to_string(i));
					return true;
				}
			}
		}

		// いったんお返し
		return false;
	}
};

#endif // WORKER_H