#ifndef WORKER_H
#define WORKER_H

class Ai;

struct Score
{
	static const int SCORE_WIN = 99999;

	int score;
	std::list<PAWN_MOVE> moveList;

	Score(const int &scoreValue)
	{
		score = scoreValue;
	}

	Score(const int &scoreValue, std::list<PAWN_MOVE> moveListValue)
	{
		score = scoreValue;
		moveList = moveListValue;
	}

	Score(const std::string &json)
	{
		std::unordered_map<std::string, std::string> strs = fromJson(json);
		score = std::stoi(strs["score"]);
		std::list<std::string> moves = fromJsonArray(strs["moves"]);
		for (std::list<std::string>::const_iterator ite = moves.cbegin(); ite != moves.cend(); ++ite)
		{
			moveList.push_back(PAWN_MOVE(*ite));
		}
	}

	std::string toJson() const
	{
		std::string str = "{";
		str += "score:" + std::to_string(score);
		if (0 < moveList.size())
		{
			str += ",moves:[";
			std::list<PAWN_MOVE>::const_iterator ite = moveList.cbegin();
			str += (std::string)(*ite);
			++ite;
			while (ite != moveList.cend())
			{
				str += "," + (std::string)(*ite);
				++ite;
			}
			str += "]";
		}
		else
		{
			str += ",moves:[]";
		}
		return str + "}";
	}

	Score Negate() const
	{
		return Score(-score, moveList);
	}

	bool operator==(const Score& rhs) const
	{
		return (
			score == rhs.score &&
			moveList == rhs.moveList
			);
	}

	bool operator!=(const Score& rhs) const
	{
		return (
			score != rhs.score ||
			moveList != rhs.moveList
			);
	}

	bool operator<(const Score& rhs) const
	{
		if (score < rhs.score)
		{
			return true;
		}
		if (score > rhs.score)
		{
			return false;
		}
		if (moveList.size() > rhs.moveList.size())
		{
			return true;
		}
		if (moveList.size() < rhs.moveList.size())
		{
			return false;
		}
#if false
		for (std::list<PAWN_MOVE>::const_iterator ite = moveList.cbegin(), rhsite = rhs.moveList.cbegin(); ite != moveList.cend() && rhsite != rhs.moveList.cend(); ++ite, ++rhsite)
		{
			if (*ite < *rhsite)
			{
				return true;
			}
		}
		return false;
#else
		return (moveList < rhs.moveList);
#endif
	}

	operator std::string() const
	{
		std::string str;
		str = std::to_string(score) + "(";
		if (0 < moveList.size())
		{
			std::list<PAWN_MOVE>::const_iterator ite = moveList.cbegin();
			str += (std::string)(*ite);
			++ite;
			while (ite != moveList.cend())
			{
				str += "," + (std::string)(*ite);
				++ite;
			}
		}
		str += ")";
		return str;
	}
};

static const Score SCORE_NONE(std::numeric_limits<int>::max() - 1);

class Worker
{
public:
	Worker(Ai *aiValue);

	void Start();
	void Stop();
		
private:
	struct NODE
	{
		MoveList moves;
		Score score;
	};

	Ai *ai = nullptr;
	Board board;

	std::thread th;
	bool isStart = false;

	bool debug = true;

	void Search();
	void SearchImplementation(const std::string &job);
};

#endif // WORKER_H