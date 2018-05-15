#ifndef PAWM_MOVE_H
#define PAWM_MOVE_H

class Move
{
public:
	class MovePawn
	{
	public:
		uchar x;
		uchar y;
		Pawn pawn;

		MovePawn()
		{
			x = 0;
			y = 0;
			pawn = PawnDef::NONE;
		}

		bool operator==(const MovePawn& rhs) const
		{
			return (
				x == rhs.x &&
				y == rhs.y &&
				pawn == rhs.pawn
				);
		}
		bool operator!=(const MovePawn& rhs) const
		{
			return !(*this == rhs);
		}

		bool operator<(const MovePawn& rhs) const
		{
			if (x < rhs.x)
			{
				return true;
			}
			if (x > rhs.x)
			{
				return false;
			}

			if (y < rhs.y)
			{
				return true;
			}
			if (y > rhs.y)
			{
				return false;
			}

			if (pawn < rhs.pawn)
			{
				return true;
			}
			if (pawn > rhs.pawn)
			{
				return false;
			}

			return false;
		}
		bool operator>(const MovePawn& rhs) const
		{
			return !(*this < rhs) && *this != rhs;
		}
	};

	Pawn reserve;
	MovePawn from;
	MovePawn to;
	bool upgrade = false;
	int priority = 0;

	Move() : reserve(PawnDef::NONE), upgrade(false), priority(0)
	{
		from = MovePawn();
		to = MovePawn();
	}

	Move(Pawn reserveValue, uchar fromx, uchar fromy, uchar tox, uchar toy, Pawn fromPawn, Pawn toPawn, bool upgradeValue, int priorityValue)
		: reserve(reserveValue), upgrade(upgradeValue), priority(priorityValue)
	{
		from.x = fromx;
		from.y = fromy;
		from.pawn = fromPawn;
		to.x = tox;
		to.y = toy;
		to.pawn = toPawn;
	}

	Move(std::string str)
	{
		from.x = std::stoi(str.substr(0, 1));
		from.y = std::stoi(str.substr(1, 1));
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
		to.x = std::stoi(str.substr(3, 1));
		to.y = std::stoi(str.substr(4, 1));
		to.pawn = charToPawn[str[5]];
		upgrade = (str[6] == 't');
	}

	std::string DebugString() const
	{
		if (reserve == PawnDef::NONE && from.x == to.x && from.y == to.y)
		{
			return "ZERO";
		}

		std::string str = "";
		//uchar tox, toy;
		str += numberToZenkaku[(uchar)to.x - 1] + numberToKanji[(uchar)to.y - 1];

		if (reserve != PawnDef::NONE)
		{
			//Pawn_ROLL reserve;
			str += " " + Pawn_KANJI[reserve] + " ‘Å‚¿";
		}
		else
		{
			str += " " + Pawn_KANJI[from.pawn];
			str += "(" + std::to_string(BoardDef::WIDTH - from.x + 1) + "," + std::to_string(from.y) + ")";

			//bool upgrade;
			if (upgrade)
			{
				str += " ¬‚è";
			}
		}

		return str;
	}

	bool operator==(const Move& rhs) const
	{
		return (
			reserve == rhs.reserve &&
			from == rhs.from &&
			to == rhs.to &&
			upgrade == rhs.upgrade
			);
	}
	bool operator!=(const Move& rhs) const
	{
		return !(*this == rhs);
	}

	bool operator<(const Move& rhs) const
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

		if (from < rhs.from)
		{
			return true;
		}
		if (from > rhs.from)
		{
			return false;
		}
		if (to < rhs.to)
		{
			return true;
		}
		if (to > rhs.to)
		{
			return false;
		}

		if (upgrade == false && rhs.upgrade == true)
		{
			return true;
		}

		return false;
	}
	bool operator>(const Move& rhs) const
	{
		return !(*this < rhs) && *this != rhs;
	}

	operator std::string() const
	{
		std::string stream = "";

		stream += std::to_string(from.x);
		stream += std::to_string(from.y);
		if (from.x == 0 && from.y == 0)
		{
			stream += Pawn_CHAR[reserve];
		}
		else
		{
			stream += Pawn_CHAR[from.pawn];
		}
		stream += std::to_string(to.x);
		stream += std::to_string(to.y);
		stream += Pawn_CHAR[to.pawn];
		if (upgrade)
		{
			stream += "t";
		}
		else
		{
			stream += "f";
		}

		return stream;
	}

	static const int MOVES_MAX = (BoardDef::WIDTH + BoardDef::HEIGHT - 2) * BoardDef::WIDTH * BoardDef::HEIGHT * 2 + (PawnDef::CAPTURE_MAX - 1) * BoardDef::WIDTH * BoardDef::HEIGHT;
};

static const Move MOVE_ZERO(PawnDef::NONE, 0, 0, 0, 0, PawnDef::NONE, PawnDef::NONE, false, 99999);



#endif // PAWM_MOVE_H