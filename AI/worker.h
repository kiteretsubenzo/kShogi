#ifndef WORKER_H
#define WORKER_H

class Ai;

struct Score
{
	static const int SCORE_WIN = 99999;

	int score;
	int deep;
	int nodeid;

	Score Negate() const
	{
		return Score(-score, -deep, -nodeid);
	}

	Score(const int &scoreValue, const int &deepValue, const int &nodeidValue = 0)
	{
		score = scoreValue;
		deep = deepValue;
		nodeid = nodeidValue;
	}

	Score(const std::string &json)
	{
		std::unordered_map<std::string, std::string> strs = fromJson(json);
		score = std::stoi(strs["score"]);
		deep = std::stoi(strs["deep"]);
		nodeid = std::stoi(strs["nodeid"]);
	}

	std::string toJson() const
	{
		std::string str = "{";
		str += "score:" + std::to_string(score);
		str += ",deep:" + std::to_string(deep);
		str += ",nodeid:" + std::to_string(nodeid);
		return str + "}";
	}

	bool operator==(const Score& rhs) const
	{
		return (
			score == rhs.score &&
			deep == rhs.deep &&
			nodeid == rhs.nodeid
			);
	}

	bool operator!=(const Score& rhs) const
	{
		return (
			score != rhs.score ||
			deep != rhs.deep ||
			nodeid != rhs.nodeid
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
		if (deep > rhs.deep)
		{
			return true;
		}
		if (deep < rhs.deep)
		{
			return false;
		}
		return (nodeid < rhs.nodeid);
	}

	operator std::string() const
	{
		std::string str;
		str = std::to_string(score) + ":" + std::to_string(deep) + ":" + std::to_string(nodeid);
		return str;
	}
};

static const Score SCORE_NONE(std::numeric_limits<int>::max() - 1, 0);

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