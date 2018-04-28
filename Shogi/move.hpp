#ifndef PAWM_MOVE_H
#define PAWM_MOVE_H

class MOVE
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

	MOVE() : reserve(PawnDef::NONE), upgrade(false), priority(0)
	{
		from.mem = 0;
		to.mem = 0;
	}

	MOVE(PAWN reserveValue, uchar fromx, uchar fromy, uchar tox, uchar toy, PAWN fromPawn, PAWN toPawn, bool upgradeValue, int priorityValue)
		: reserve(reserveValue), upgrade(upgradeValue), priority(priorityValue)
	{
		from.x = fromx;
		from.y = fromy;
		from.pawn = fromPawn;
		to.x = tox;
		to.y = toy;
		to.pawn = toPawn;
	}

	MOVE(std::string str)
	{
		from.x = std::stoi(str.substr(0, 1), 0, 16);
		from.y = std::stoi(str.substr(1, 1), 0, 16);
		if (from.x == 0 && from.y == 0)
		{
			reserve = charToPawn[str[2]];
			from.pawn = PawnDef::NONE;
		}
		else
		{
			reserve = PawnDef::NONE;
			from.pawn = charToPawn[str[2]];
		}
		to.x = std::stoi(str.substr(3, 1), 0, 16);
		to.y = std::stoi(str.substr(4, 1), 0, 16);
		to.pawn = charToPawn[str[5]];
		upgrade = (str[6] == 't');
	}

	std::string DebugString() const
	{
		if (reserve == PawnDef::NONE && from.x == to.x && from.y == to.y)
		{
			return "ZERO";
		}

		std::string str;
		//uchar tox, toy;
		str += numberToZenkaku[(uchar)to.x - 1] + numberToKanji[(uchar)to.y - 1];

		if (reserve != PawnDef::NONE)
		{
			//PAWN_ROLL reserve;
			str += " " + PAWN_KANJI[reserve] + " 打ち";
		}
		else
		{
			str += " " + PAWN_KANJI[from.pawn];
			str += "(" + std::to_string(BoardDef::WIDTH - from.x + 1) + "," + std::to_string(from.y) + ")";

			//bool upgrade;
			if (upgrade)
			{
				str += " 成り";
			}
		}

		return str;
	}

	bool operator==(const MOVE& rhs) const
	{
		return (
			reserve == rhs.reserve &&
			from.mem == rhs.from.mem &&
			to.mem == rhs.to.mem &&
			upgrade == rhs.upgrade
			);
	}
	bool operator!=(const MOVE& rhs) const
	{
		return !(*this == rhs);
	}

	bool operator<(const MOVE& rhs) const
	{
		if (priority > rhs.priority)
		{
			return true;
		}
		if (priority < rhs.priority)
		{
			return false;
		}

		if (reserve == PawnDef::NONE && rhs.reserve != PawnDef::NONE)
		{
			return true;
		}
		if (reserve != PawnDef::NONE && rhs.reserve == PawnDef::NONE)
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
	bool operator>(const MOVE& rhs) const
	{
		return !(*this < rhs) && *this != rhs;
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

	static const int MOVES_MAX = (BoardDef::WIDTH + BoardDef::HEIGHT - 2) * BoardDef::WIDTH * BoardDef::HEIGHT * 2 + (PawnDef::CAPTURE_MAX - 1) * BoardDef::WIDTH * BoardDef::HEIGHT;
};

static const MOVE PAWN_MOVE_ZERO(PawnDef::NONE, 0, 0, 0, 0, PawnDef::NONE, PawnDef::NONE, false, 99999);



#endif // PAWM_MOVE_H