#ifndef BOARD_H
#define BOARD_H

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

#if USE_PRIORITY == PRIORITY_MULTISET
static const PAWN_MOVE PAWN_MOVE_ZERO(PAWN_NONE, 0, 0, 0, 0, PAWN_NONE, PAWN_NONE, false, 99999);
#else
static const PAWN_MOVE PAWN_MOVE_ZERO( PAWN_NONE, 0, 0, 0, 0, PAWN_NONE, PAWN_NONE, false, 0 );
#endif

#endif // BOARD_H