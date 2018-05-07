#ifndef SCORE_H
#define SCORE_H

struct Score
{
	static const int SCORE_WIN = 99999;
	static const int SCORE_UNVALUED = (std::numeric_limits<int>::max() - 1);

	int score;
	// TODO: 固定バッファ化する
	std::list<Move> moveList;

	Score(const int &scoreValue)
	{
		score = scoreValue;
	}

	Score(const int &scoreValue, const std::list<Move> &moveListValue)
	{
		score = scoreValue;
		moveList = moveListValue;
	}

	Score(const std::string &json)
	{
		std::unordered_map<std::string, std::string> strs = Json::fromJson(json);
		score = std::stoi(strs["score"]);
		std::list<std::string> moves = Json::fromJsonArray(strs["moves"]);
		for (std::list<std::string>::const_iterator ite = moves.cbegin(); ite != moves.cend(); ++ite)
		{
			moveList.push_back(Move(*ite));
		}
	}

	std::string toJson() const
	{
		std::string str = "{";
		str += "score:" + std::to_string(score);
		if (0 < moveList.size())
		{
			str += ",moves:[";
			std::list<Move>::const_iterator ite = moveList.cbegin();
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
		if (moveList.size() > rhs.moveList.size())
		{
			return true;
		}
		if (moveList.size() < rhs.moveList.size())
		{
			return false;
		}
#if false
		for (std::list<MOVE>::const_iterator ite = moveList.cbegin(), rhsite = rhs.moveList.cbegin(); ite != moveList.cend() && rhsite != rhs.moveList.cend(); ++ite, ++rhsite)
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
		str = std::to_string(score) + "(";
		if (0 < moveList.size())
		{
			std::list<Move>::const_iterator ite = moveList.cbegin();
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

static const Score SCORE_NONE(Score::SCORE_UNVALUED);

#endif // SCORE_H