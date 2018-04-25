#ifndef BOARD_H
#define BOARD_H

#define PRIORITY_NONE		0
#define PRIORITY_LIST		1
#define PRIORITY_MULTISET	2

#define USE_PRIORITY PRIORITY_NONE

typedef unsigned char PLAYER;

#define PLAYER_FIRST	0
#define PLAYER_SECOND	1
#define PLAYER_NONE		2
#define PLAYER_WALL		3

#define PLAYER_MAX		2

struct CELL
{
	PLAYER player;
	PAWN pawn;
};

#if USE_PRIORITY == PRIORITY_MULTISET
class MoveList
{
public:
	bool empty() const
	{
		return list.empty();
	}
	size_t size() const
	{
		return list.size();
	}
	void clear()
	{
		list.clear();
	}
	void push(const PAWN_MOVE &move)
	{
		list.insert(move);
	}
	PAWN_MOVE front() const
	{
		return *(list.begin());
	}
	void pop_front()
	{
		list.erase(list.begin());
	}
	void sort() {}
private:
	std::multiset<PAWN_MOVE> list;
};
#else
class MoveList
{
public:
	bool empty() const
	{
		return list.empty();
	}
	size_t size() const
	{
		return list.size();
	}
	void clear()
	{
		list.clear();
	}
	void push(const PAWN_MOVE &move)
	{
		list.push_back(move);
	}
	PAWN_MOVE front() const
	{
		return *(list.begin());
	}
	void pop_front()
	{
		list.erase(list.begin());
	}
	void sort()
	{
		list.sort();
	}
private:
	std::list<PAWN_MOVE> list;
};
#endif


class Board
{
public:
	virtual void Init(const std::string &str) = 0;
	// TODO
	virtual std::string BoardToString() const = 0;
	
	virtual MoveList GetMoveList() = 0;

	virtual void Move(const PAWN_MOVE &move) = 0;
	virtual void Back(const PAWN_MOVE &move) = 0;

	virtual int GetEvaluate(const MoveList &moveList) = 0;
	virtual int GetPriority(const PAWN_MOVE &move) = 0;

	virtual void PrintBoard() const = 0;

	bool operator==(const Board& rhs) const
	{
		return (
			matrix == rhs.matrix &&
			turn == rhs.turn
		);
	}
	bool operator!=(const Board& rhs) const
	{
		return (
			matrix != rhs.matrix ||
			turn != rhs.turn
		);
	}

protected:
	virtual bool AddMove(PAWN roll, uchar fromx, uchar fromy, uchar tox, uchar toy, bool upgrade, MoveList &moveList) = 0;
	virtual bool IsEnd() const = 0;

	CELL GetCell(uchar x, uchar y) { return matrix[y][x]; }
	bool GetCell(uchar tox, uchar toy, CELL &cell) const
	{
		if (matrix[toy][tox].player != turn)
		{
			return false;
		}

		cell = matrix[toy][tox];

		return true;
	}

	void SwitchTurn()
	{
		if (turn == PLAYER_FIRST)
		{
			turn = PLAYER_SECOND;
			enemy = PLAYER_FIRST;
		}
		else
		{
			turn = PLAYER_FIRST;
			enemy = PLAYER_SECOND;
		}
	}
	
	CELL matrix[BOARD_HEIGHT+2][BOARD_WIDTH+2];
	PLAYER turn;
	PLAYER enemy;
};

static const PAWN_MOVE PAWN_MOVE_ZERO(PAWN_NONE, 0, 0, 0, 0, PAWN_NONE, PAWN_NONE, false, 99999);

#endif // BOARD_H