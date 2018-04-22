#ifndef BOARD_H
#define BOARD_H

typedef unsigned char PLAYER;

#define PLAYER_FIRST	0
#define PLAYER_SECOND	16
#define PLAYER_NONE		32
#define PLAYER_WALL		64
/*
#define PLAYER_SHIFT_FIRST	0
#define PLAYER_SHIFT_SECOND	16
#define PLAYER_SHIFT_NONE	32
#define PLAYER_SHIFT_WALL	64
*/
#define PLAYER_MAX		2

#define PlayerShiftToIndex(player)	( player >> 4 )

struct CELL
{
	PLAYER player;
	PAWN pawn;
};

static const std::string numberToZenkaku[9] =
{
	"９", "８", "７", "６", "５", "４", "３", "２", "１"
};

static const std::string numberToKanji[9] =
{
	"一", "二", "三", "四", "五", "六", "七", "八", "九"
};

struct PAWN_MOVE
{
public:
	union MOVE_PAWN_MEMORY
	{
		struct
		{
			// ※アライメント注意！sizeof(int)バイトをまたがないように
			uchar x : 4;
			uchar y : 4;
			PAWN pawn;
		};
		unsigned short mem;
	};

	PAWN reserve;
	MOVE_PAWN_MEMORY from;
	MOVE_PAWN_MEMORY to;
	bool upgrade = false;
	int priority = 0;

	PAWN_MOVE() : reserve(PAWN_NONE), upgrade(false), priority(0)
	{
		from.mem = 0;
		to.mem = 0;
	}

	PAWN_MOVE(PAWN reserveValue, uchar fromx, uchar fromy, uchar tox, uchar toy, PAWN fromPawn, PAWN toPawn, bool upgradeValue, int priorityValue)
		: reserve(reserveValue), upgrade(upgradeValue), priority(priorityValue)
	{
		from.x = fromx;
		from.y = fromy;
		from.pawn = fromPawn;
		to.x = tox;
		to.y = toy;
		to.pawn = toPawn;
	}

	PAWN_MOVE(std::string str)
	{
		from.x = std::stoi(str.substr(0, 1), 0, 16);
		from.y = std::stoi(str.substr(1, 1), 0, 16);
		if (from.x == 0 && from.y == 0)
		{
			reserve = charToPawn[str[2]];
			from.pawn = PAWN_NONE;
		}
		else
		{
			reserve = PAWN_NONE;
			from.pawn = charToPawn[str[2]];
		}
		to.x = std::stoi(str.substr(3, 1), 0, 16);
		to.y = std::stoi(str.substr(4, 1), 0, 16);
		to.pawn = charToPawn[str[5]];
		upgrade = (str[6] == 't');
	}

	std::string DebugString() const
	{
		if (reserve == PAWN_NONE && from.x == to.x && from.y == to.y)
		{
			return "ZERO";
		}

		std::string str;
		//uchar tox, toy;
		str += numberToZenkaku[(uchar)to.x-1] + numberToKanji[(uchar)to.y-1];

		if (reserve != PAWN_NONE)
		{
			//PAWN_ROLL reserve;
			str += " " + PAWN_KANJI[reserve] + " 打ち";
		}
		else
		{
			str += " " + PAWN_KANJI[from.pawn];
			str += "(" + std::to_string(BOARD_WIDTH - from.x + 1) + "," + std::to_string(from.y) + ")";

			//bool upgrade;
			if (upgrade)
			{
				str += " 成り";
			}
		}

		return str;
	}

	bool operator==(const PAWN_MOVE& rhs) const
	{
		return (
			reserve == rhs.reserve &&
			from.mem == rhs.from.mem &&
			to.mem == rhs.to.mem &&
			upgrade == rhs.upgrade
			);
	}
	bool operator!=(const PAWN_MOVE& rhs) const
	{
		return (
			reserve != rhs.reserve ||
			from.mem != rhs.from.mem ||
			to.mem != rhs.to.mem ||
			upgrade != rhs.upgrade
			);
	}

	bool operator<(const PAWN_MOVE& rhs) const
	{
		if (priority > rhs.priority)
		{
			return true;
		}
		if (priority < rhs.priority)
		{
			return false;
		}

		if (reserve == PAWN_NONE && rhs.reserve != PAWN_NONE)
		{
			return true;
		}
		if (reserve != PAWN_NONE && rhs.reserve == PAWN_NONE)
		{
			return false;
		}
		if (reserve < rhs.reserve)
		{
			return true;
		}
		if (reserve > rhs.reserve)
		{
			return false;
		}

		if (from.mem < rhs.from.mem)
		{
			return true;
		}
		if (from.mem > rhs.from.mem)
		{
			return false;
		}
		if (to.mem < rhs.to.mem)
		{
			return true;
		}
		if (to.mem > rhs.to.mem)
		{
			return false;
		}

		if (upgrade == false && rhs.upgrade == true)
		{
			return true;
		}

		return false;
	}

	operator std::string() const
	{
		std::stringstream stream;

		stream << std::hex << (int)from.x;
		stream << std::hex << (int)from.y;
		if (from.x == 0 && from.y == 0)
		{
			stream << PAWN_CHAR[reserve];
		}
		else
		{
			stream << PAWN_CHAR[from.pawn];
		}
		stream << std::hex << (int)to.x;
		stream << std::hex << (int)to.y;
		stream << PAWN_CHAR[to.pawn];
		if (upgrade)
		{
			stream << "t";
		}
		else
		{
			stream << "f";
		}

		return stream.str();
	}
};

class Board
{
public:
	
	Board();
	
	void Init(const std::string &str);
	// TODO
	std::string BoardToString() const;
	
	MoveList GetMoveList();

	void Move(const PAWN_MOVE &move);
	void Back(const PAWN_MOVE &move);
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

	virtual int GetEvaluate(const MoveList &moveList);
	virtual int GetPriority(const PAWN_MOVE &move);
	
	CELL GetCell(uchar x, uchar y) { return matrix[y][x]; }
	
	void PrintBoard() const;

	bool operator==(const Board& rhs) const
	{
		return (
			captured == rhs.captured &&
			matrix == rhs.matrix &&
			turn == rhs.turn
		);
	}
	bool operator!=(const Board& rhs) const
	{
		return (
			captured != rhs.captured ||
			matrix != rhs.matrix ||
			turn != rhs.turn
		);
	}

private:
	bool AddMove(PAWN roll, uchar fromx, uchar fromy, uchar tox, uchar toy, bool upgrade, MoveList &moveList);
	bool IsEnd() const;
	bool GetCell(uchar tox, uchar toy, CELL &cell) const
	{
		if (matrix[toy][tox].player != turn)
		{
			return false;
		}

		cell = matrix[toy][tox];

		return true;
	}
	
	uchar captured[PLAYER_MAX][(uchar)CAPTURE_MAX];
	CELL matrix[BOARD_HEIGHT+2][BOARD_WIDTH+2];
	PLAYER turn;
	PLAYER enemy;

	char gyokux[PLAYER_MAX];
	char gyokuy[PLAYER_MAX];
};

#if USE_PRIORITY == PRIORITY_MULTISET
static const PAWN_MOVE PAWN_MOVE_ZERO(PAWN_NONE, 0, 0, 0, 0, PAWN_NONE, PAWN_NONE, false, 99999);
#else
static const PAWN_MOVE PAWN_MOVE_ZERO( PAWN_NONE, 0, 0, 0, 0, PAWN_NONE, PAWN_NONE, false, 0 );
#endif

#endif // BOARD_H