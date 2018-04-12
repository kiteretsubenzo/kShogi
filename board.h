#ifndef BOARD_H
#define BOARD_H

//#define POS_TEST

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
#ifdef POS_TEST
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

		PAWN_MOVE()
			: reserve(PAWN_NONE), upgrade(false), priority(0)
		{
			from.x = 0;
			from.y = 0;
			from.pawn = PAWN_NONE;
			to.x = 0;
			to.y = 0;
			to.pawn = PAWN_NONE;
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
#else
	struct PAWN_MOVE
	{
		PAWN reserve;

		struct position {
			uchar fromx;
			uchar fromy;
			uchar tox;
			uchar toy;
		};
		union {
			struct position pos;
			uint32_t uint32_pos;
		} pos;

		PAWN fromPawn;
		PAWN toPawn;
		bool upgrade;
		int priority;
		
		std::string DebugString() const
		{
			if( reserve == PAWN_NONE && pos.uint32_pos == 0 )
			{
				return "ZERO";
			}
			
			std::string str;
			//uchar tox, toy;
			str += numberToZenkaku[(uchar)pos.pos.tox] + numberToKanji[(uchar)pos.pos.toy];

			if( reserve != PAWN_NONE )
			{
				//PAWN_ROLL reserve;
				str +=  " " + PAWN_KANJI[reserve] + " 打ち";
			}
			else
			{
				str += " " + PAWN_KANJI[fromPawn];
				str += "(" + std::to_string(BOARD_WIDTH-pos.pos.fromx) + "," + std::to_string(pos.pos.fromy+1) + ")";

				if( upgrade )
				{
					str += " 成り";
				}
			}
			
			return str;
		}
		
		bool operator==( const PAWN_MOVE& rhs ) const
		{
			return (
				reserve == rhs.reserve &&
				pos.uint32_pos == rhs.pos.uint32_pos &&
				toPawn == rhs.toPawn &&
				upgrade == rhs.upgrade
			);
		}
		bool operator!=( const PAWN_MOVE& rhs ) const
		{
			return (
				reserve != rhs.reserve ||
				pos.uint32_pos != rhs.pos.uint32_pos ||
				toPawn != rhs.toPawn ||
				upgrade != rhs.upgrade
			);
		}

		bool operator<(const PAWN_MOVE& rhs) const
		{
			return ( priority > rhs.priority );
		}
		
		operator std::string() const
		{
			std::stringstream stream;

			stream << std::setfill('0') << std::setw(2) << (int)reserve;
			stream << std::setfill('0') << std::setw(2) << (int)pos.pos.fromx;
			stream << std::setfill('0') << std::setw(2) << (int)pos.pos.fromy;
			stream << std::setfill('0') << std::setw(2) << (int)pos.pos.tox;
			stream << std::setfill('0') << std::setw(2) << (int)pos.pos.toy;
			stream << std::setfill('0') << std::setw(2) << (int)fromPawn;
			stream << std::setfill('0') << std::setw(2) << (int)toPawn;

			if( upgrade )
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
#endif
	
	Board();
	
	void Init(const std::string str);
	// TODO
	std::string BoardToString() const;
	
	std::list<Board::PAWN_MOVE> GetMoveList();

	void Move(const PAWN_MOVE &move);
	void Back(const PAWN_MOVE &move);
	void SwitchTurn();

	virtual int GetEvaluate(const std::list<Board::PAWN_MOVE> &moveList);
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
	bool AddMove(PAWN roll, uchar fromx, uchar fromy, char tox, char toy, bool upgrade, std::list<Board::PAWN_MOVE> &moveList);
	bool IsEnd() const;
	bool GetCell(char tox, char toy, CELL &cell) const;
	
	uchar captured[(uchar)PLAYER::MAX][(uchar)CAPTURE_MAX];
	CELL matrix[BOARD_HEIGHT][BOARD_WIDTH];
	PLAYER turn;

	char gyokux[(uchar)PLAYER::MAX];
	char gyokuy[(uchar)PLAYER::MAX];
};

#ifdef POS_TEST
static const Board::PAWN_MOVE PAWN_MOVE_ZERO( PAWN_NONE, 0, 0, 0, 0, PAWN_NONE, PAWN_NONE, false, 0 );
#else
static const Board::PAWN_MOVE PAWN_MOVE_ZERO{ PAWN_NONE, 0, 0, 0, 0, PAWN_NONE, PAWN_NONE, false };
#endif

#endif // BOARD_H