#ifndef WORKER_H
#define WORKER_H

class Ai;

struct Score
{
	int score;
	int deep;

	Score Negate() const
	{
		return Score(-score, -deep);
	}

	Score(const int &scoreValue, const int &deepValue)
	{
		score = scoreValue;
		deep = deepValue;
	}

	Score(const std::string &json)
	{
		std::unordered_map<std::string, std::string> strs = fromJson(json);
		score = std::stoi(strs["score"]);
		deep = std::stoi(strs["deep"]);
	}

	std::string toJson() const
	{
		std::string str = "{";
		str += "score:" + std::to_string(score);
		str += ",deep:" + std::to_string(deep);
		return str + "}";
	}

	bool operator==(const Score& rhs) const
	{
		return (
			score == rhs.score &&
			deep == rhs.deep
			);
	}

	bool operator!=(const Score& rhs) const
	{
		return (
			score != rhs.score ||
			deep != rhs.deep
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
		return (deep > rhs.deep);
	}

	operator std::string() const
	{
		std::string str;
		str = std::to_string(score) + ":" + std::to_string(deep);
		return str;
	}
};

static const Score SCORE_NONE(std::numeric_limits<int>::max() - 1, 0);
static const int SCORE_WIN = 99999 * 1000;

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