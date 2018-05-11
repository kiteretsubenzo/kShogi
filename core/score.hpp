#ifndef SCORE_H
#define SCORE_H

class ScoreMoveList
{
public:
	ScoreMoveList() {}

	void clear()
	{
		index = -1;
	}

	void push_back(const Move &move)
	{
		index++;
		moveList[index] = move;
	}

	void copy(const ScoreMoveList &moveListValue)
	{
		memcpy(moveList, moveListValue.moveList, moveListValue.size()*sizeof(Move));
		index = moveListValue.index;
	}

	unsigned int size() const
	{
		return index + 1;
	}

	void pop_back()
	{
		index--;
	}

	Move& front()
	{
		return moveList[0];
	}

	std::string DebugString() const
	{
		std::string str = "";
		for (unsigned int i = 0; i<size(); i++)
		{
			str += moveList[i].DebugString() + " : ";
		}
		return str;
	}

	bool operator<(const ScoreMoveList& rhs) const
	{
		if (size() > rhs.size())
		{
			return true;
		}
		if (size() < rhs.size())
		{
			return false;
		}

		for (unsigned int i=0; i<size(); i++)
		{
			if (moveList[i] < rhs.moveList[i])
			{
				return true;
			}
		}
		return false;
	}

	bool operator>(const ScoreMoveList& rhs) const
	{
		return !(*this < rhs) && *this != rhs;
	}

	bool operator==(const ScoreMoveList& rhs) const
	{
		if (index != rhs.index)
		{
			return false;
		}

		for (int i = 0; i < index; i++)
		{
			if (moveList[i] != rhs.moveList[i])
			{
				return false;
			}
		}
		return true;
	}

	bool operator!=(const ScoreMoveList& rhs) const
	{
		return !(*this == rhs);
	}

	std::string toJson() const
	{
		std::string str = "moves:[";
		if (0 < size())
		{
			str += (std::string)(moveList[0]);
			for (unsigned int i=1; i<size(); i++)
			{
				str += "," + (std::string)(moveList[i]);
			}
		}
		str += "]";
		return str;
	}

	operator std::string() const
	{
		std::string str = "(";
		if (0 < size())
		{
			str += (std::string)(moveList[0]);
			for (unsigned int i = 1; i<size(); i++)
			{
				str += "," + (std::string)(moveList[i]);
			}
		}
		str += ")";
		return str;
	}

private:
	Move moveList[64];
	int index = -1;
};

struct Score
{
	static const int SCORE_WIN = 99999;
	static const int SCORE_UNVALUED = (std::numeric_limits<int>::max() - 1);

	int score;
	ScoreMoveList moveList;

	Score()
	{
		score = SCORE_UNVALUED;
		moveList.clear();
	}

	Score(const Score &scoreValue)
	{
		score = scoreValue.score;
		moveList.copy(scoreValue.moveList);
	}

	Score(const int &scoreValue)
	{
		score = scoreValue;
		moveList.clear();
	}

	Score(const int &scoreValue, const ScoreMoveList &moveListValue)
	{
		score = scoreValue;
		moveList.copy(moveListValue);
	}

	Score(const std::string &json)
	{
		std::unordered_map<std::string, std::string> strs = Json::fromJson(json);
		score = std::stoi(strs["score"]);
		std::list<std::string> moves = Json::fromJsonArray(strs["moves"]);
		moveList.clear();
		for (std::list<std::string>::const_iterator ite = moves.cbegin(); ite != moves.cend(); ++ite)
		{
			moveList.push_back(Move(*ite));
		}
	}

	std::string toJson() const
	{
		std::string str = "{";
		str += "score:" + std::to_string(score);
		str += "," + moveList.toJson();
		return str + "}";
	}

	void copy(const Score &scoreValue)
	{
		score = scoreValue.score;
		moveList.copy(scoreValue.moveList);
	}

	void clear()
	{
		score = SCORE_UNVALUED;
		moveList.clear();
	}

	static Score &Min(Score &lhs, Score &rhs)
	{
		if (lhs.score != SCORE_UNVALUED && rhs.score != SCORE_UNVALUED)
		{
			if (lhs < rhs)
			{
				return lhs;
			}
			return rhs;
		}

		if (lhs.score == SCORE_UNVALUED)
		{
			return rhs;
		}

		return lhs;
	}

	Score Negate() const
	{
		if (score == SCORE_UNVALUED)
		{
			return *this;
		}
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
		return !(*this == rhs);
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
		return (moveList < rhs.moveList);
	}

	bool operator>(const Score& rhs) const
	{
		return !(*this < rhs) && *this != rhs;
	}

	bool operator<=(const Score& rhs) const
	{
		return *this < rhs || *this == rhs;
	}

	bool operator>=(const Score& rhs) const
	{
		return *this > rhs || *this == rhs;
	}

	operator std::string() const
	{
		std::string str;
		str = std::to_string(score);
		str += (std::string)(moveList);
		return str;
	}
};

static const Score SCORE_NONE(Score::SCORE_UNVALUED);

#endif // SCORE_H