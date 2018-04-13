#ifndef BOARD_H
#define BOARD_H

//#define USE_PRIORITY_MULTISET
//#define USE_PRIORITY_LIST

enum class PLAYER
{
  FIRST, SECOND,
  MAX, NONE = MAX
};

struct CELL
{
  PLAYER player;
  PAWN pawn;
};

static const std::string PLAYER_STRING[(uchar)PLAYER::MAX] =
{
	"先手番", "後手番"
};

static const std::string numberToZenkaku[9] =
{
	"９", "８", "７", "６", "５", "４", "３", "２", "１"
};

static const std::string numberToKanji[9] =
{
	"一", "二", "三", "四", "五", "六", "七", "八", "九"
};

class Board
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

	struct PAWN_MOVE
	{
	public:
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

		std::string DebugString() const
		{
			if (reserve == PAWN_NONE && from.x == to.x && from.y == to.y)
			{
				return "ZERO";
			}

			std::string str;
			//uchar tox, toy;
			str += numberToZenkaku[(uchar)to.x] + numberToKanji[(uchar)to.y];

			if (reserve != PAWN_NONE)
			{
				//PAWN_ROLL reserve;
				str += " " + PAWN_KANJI[reserve] + " 打ち";
			}
			else
			{
				str += " " + PAWN_KANJI[from.pawn];
				str += "(" + std::to_string(BOARD_WIDTH - from.x) + "," + std::to_string(from.y + 1) + ")";

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
			return (priority > rhs.priority);
		}

		operator std::string() const
		{
			std::stringstream stream;

			stream << std::setfill('0') << std::setw(2) << (int)reserve;
			stream << std::setfill('0') << std::setw(2) << (int)from.x;
			stream << std::setfill('0') << std::setw(2) << (int)from.y;
			stream << std::setfill('0') << std::setw(2) << (int)to.x;
			stream << std::setfill('0') << std::setw(2) << (int)to.y;
			stream << std::setfill('0') << std::setw(2) << (int)from.pawn;
			stream << std::setfill('0') << std::setw(2) << (int)to.pawn;
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
	
	Board();
	
	void Init(const std::string str);
	// TODO
	std::string BoardToString() const;
	
#ifdef USE_PRIORITY_MULTISET
	std::multiset<Board::PAWN_MOVE> GetMoveList();
#else
	std::list<Board::PAWN_MOVE> GetMoveList();
#endif

	void Move(const PAWN_MOVE &move);
	void Back(const PAWN_MOVE &move);
	void SwitchTurn();

#ifdef USE_PRIORITY_MULTISET
	virtual int GetEvaluate(const std::multiset<Board::PAWN_MOVE> &moveList);
#else
	virtual int GetEvaluate(const std::list<Board::PAWN_MOVE> &moveList);
#endif
	virtual int GetPriority(const Board::PAWN_MOVE &move);
	
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
#ifdef USE_PRIORITY_MULTISET
	bool AddMove(PAWN roll, uchar fromx, uchar fromy, char tox, char toy, bool upgrade, std::multiset<Board::PAWN_MOVE> &moveList);
#else
	bool AddMove(PAWN roll, uchar fromx, uchar fromy, char tox, char toy, bool upgrade, std::list<Board::PAWN_MOVE> &moveList);
#endif
	bool IsEnd() const;
	bool GetCell(char tox, char toy, CELL &cell) const;
	
	uchar captured[(uchar)PLAYER::MAX][(uchar)CAPTURE_MAX];
	CELL matrix[BOARD_HEIGHT][BOARD_WIDTH];
	PLAYER turn;

	char gyokux[(uchar)PLAYER::MAX];
	char gyokuy[(uchar)PLAYER::MAX];
};

#ifdef USE_PRIORITY_MULTISET
static const Board::PAWN_MOVE PAWN_MOVE_ZERO(PAWN_NONE, 0, 0, 0, 0, PAWN_NONE, PAWN_NONE, false, 99999);
#else
static const Board::PAWN_MOVE PAWN_MOVE_ZERO( PAWN_NONE, 0, 0, 0, 0, PAWN_NONE, PAWN_NONE, false, 0 );
#endif

#endif // BOARD_H