#ifndef BOARD_H
#define BOARD_H

enum class PLAYER
{
  FIRST, SECOND,
  MAX, NONE = MAX
};

struct CELL
{
  PLAYER player;
  PAWN_TYPE pawn;
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
	struct PAWN_MOVE
	{
		PAWN_ROLL reserve;
		uchar fromx;
		uchar fromy;
		uchar tox;
		uchar toy;
		PAWN_TYPE fromPawn;
		PAWN_TYPE toPawn;
		bool upgrade;
		
		std::string DebugString() const
		{
			if( reserve == PAWN_ROLL::NONE && fromx == 0 && fromy == 0 && tox == 0 && toy == 0 )
			{
				return "ZERO";
			}
			
			std::string str;
			//uchar tox, toy;
			str += numberToZenkaku[(uchar)tox] + numberToKanji[(uchar)toy];
			if( reserve != PAWN_ROLL::NONE )
			{
				//PAWN_ROLL reserve;
				str +=  " " + (std::string)reserve + " 打ち";
			}
			else
			{
				str += " " + (std::string)fromPawn;
				str += "(" + std::to_string(BOARD_WIDTH-fromx) + "," + std::to_string(fromy+1) + ")";
				//bool upgrade;
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
					fromx == rhs.fromx &&
					fromy == rhs.fromy &&
					tox == rhs.tox &&
					toy == rhs.toy &&
					fromPawn == rhs.fromPawn &&
					toPawn == rhs.toPawn &&
					upgrade == rhs.upgrade
				);
		}
		bool operator!=( const PAWN_MOVE& rhs ) const
		{
			return (
					reserve != rhs.reserve ||
					fromx != rhs.fromx ||
					fromy != rhs.fromy ||
					tox != rhs.tox ||
					toy != rhs.toy ||
					fromPawn != rhs.fromPawn ||
					toPawn != rhs.toPawn ||
					upgrade != rhs.upgrade
				);
		}
		
		operator std::string() const
		{
			std::stringstream stream;
			
			stream << std::setfill('0') << std::setw(2) << (int)reserve;
			stream << std::setfill('0') << std::setw(2) << (int)fromx;
			stream << std::setfill('0') << std::setw(2) << (int)fromy;
			stream << std::setfill('0') << std::setw(2) << (int)tox;
			stream << std::setfill('0') << std::setw(2) << (int)toy;
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
	
	Board();
	
	void Init(const std::string str);
	// TODO
	std::string BoardToString() const;
	
	template <typename T> T GetMoveList()
	{
		T moveList;
		uchar lineMax, lineMin, lineTop, lineMid;

		if (turn == PLAYER::FIRST)
		{
			lineMax = 2;
			lineMin = 0;
			lineTop = 0;
			lineMid = 1;
		}
		else
		{
			lineMax = BOARD_HEIGHT - 1;
			lineMin = BOARD_HEIGHT - 3;
			lineTop = BOARD_HEIGHT - 1;
			lineMid = BOARD_HEIGHT - 2;
		}
		int forward = -1;
		if (turn == PLAYER::SECOND)
		{
			forward = +1;
		}
		for (char j = 0; j<BOARD_HEIGHT; j++)
		{
			for (char i = 0; i<BOARD_WIDTH; i++)
			{
				CELL cell = matrix[(uchar)j][(uchar)i];
				if (cell.player != turn)
				{
					continue;
				}
				PAWN_TYPE pawn = cell.pawn;
				int x, y;
				switch ((PAWN_TYPE::TYPE)pawn)
				{
				case PAWN_TYPE::HU:
					x = i;
					y = j + forward;
					if (y != lineTop)
					{
						AddMove(PAWN_ROLL::NONE, i, j, x, y, false, moveList);
					}
					if (lineMin <= y && y <= lineMax)
					{
						AddMove(PAWN_ROLL::NONE, i, j, x, y, true, moveList);
					}
					break;
				case PAWN_TYPE::KYOH:
					y = j + forward;
					for (bool ret = true; 0 <= (int)y && (int)y < BOARD_HEIGHT && ret; y += forward)
					{
						if (y != lineTop)
						{
							ret &= AddMove(PAWN_ROLL::NONE, i, j, i, y, false, moveList);
						}
						if (lineMin <= y && y <= lineMax)
						{
							ret &= AddMove(PAWN_ROLL::NONE, i, j, i, y, true, moveList);
						}
					}
					break;
				case PAWN_TYPE::KEI:
					x = i - 1;
					y = j - forward - forward;
					if (y != lineTop && y != lineMid)
					{
						AddMove(PAWN_ROLL::NONE, i, j, x, y, false, moveList);
					}
					if (lineMin <= y && y <= lineMax)
					{
						AddMove(PAWN_ROLL::NONE, i, j, x, y, true, moveList);
					}
					break;
				case PAWN_TYPE::GIN:
					AddMove(PAWN_ROLL::NONE, i, j, i - 1, j + forward, false, moveList);
					AddMove(PAWN_ROLL::NONE, i, j, i, j + forward, false, moveList);
					AddMove(PAWN_ROLL::NONE, i, j, i + 1, j + forward, false, moveList);
					AddMove(PAWN_ROLL::NONE, i, j, i - 1, j - forward, false, moveList);
					AddMove(PAWN_ROLL::NONE, i, j, i + 1, j - forward, false, moveList);
					if ((lineMin <= (j + forward) && (j + forward) <= lineMax) || (lineMin <= j && j <= lineMax))
					{
						AddMove(PAWN_ROLL::NONE, i, j, i - 1, j + forward, true, moveList);
						AddMove(PAWN_ROLL::NONE, i, j, i, j + forward, true, moveList);
						AddMove(PAWN_ROLL::NONE, i, j, i + 1, j + forward, true, moveList);
						AddMove(PAWN_ROLL::NONE, i, j, i - 1, j - forward, true, moveList);
						AddMove(PAWN_ROLL::NONE, i, j, i + 1, j - forward, true, moveList);
					}
					break;
				case PAWN_TYPE::KIN:
				case PAWN_TYPE::HUN:
				case PAWN_TYPE::KYOHN:
				case PAWN_TYPE::KEIN:
				case PAWN_TYPE::GINN:
					AddMove(PAWN_ROLL::NONE, i, j, i - 1, j + forward, false, moveList);
					AddMove(PAWN_ROLL::NONE, i, j, i, j + forward, false, moveList);
					AddMove(PAWN_ROLL::NONE, i, j, i + 1, j + forward, false, moveList);
					AddMove(PAWN_ROLL::NONE, i, j, i - 1, j, false, moveList);
					AddMove(PAWN_ROLL::NONE, i, j, i + 1, j, false, moveList);
					AddMove(PAWN_ROLL::NONE, i, j, i, j - forward, false, moveList);
					break;
				case PAWN_TYPE::KAKU:
					x = i + 1; y = j + 1;
					for (bool ret = true; x < BOARD_WIDTH && y < BOARD_HEIGHT && ret; x++, y++)
					{
						ret &= AddMove(PAWN_ROLL::NONE, i, j, x, y, false, moveList);
						if (lineMin <= y && y <= lineMax)
						{
							ret &= AddMove(PAWN_ROLL::NONE, i, j, x, y, true, moveList);
						}
					}
					x = i + 1; y = j - 1;
					for (bool ret = true; x < BOARD_WIDTH && 0 <= y && ret; x++, y -= 1)
					{
						ret &= AddMove(PAWN_ROLL::NONE, i, j, x, y, false, moveList);
						if (lineMin <= y && y <= lineMax)
						{
							ret &= AddMove(PAWN_ROLL::NONE, i, j, x, y, true, moveList);
						}
					}
					x = i - 1; y = j + 1;
					for (bool ret = true; 0 <= x && y < BOARD_HEIGHT && ret; x -= 1, y++)
					{
						ret &= AddMove(PAWN_ROLL::NONE, i, j, x, y, false, moveList);
						if (lineMin <= y && y <= lineMax)
						{
							ret &= AddMove(PAWN_ROLL::NONE, i, j, x, y, true, moveList);
						}
					}
					x = i - 1; y = j - 1;
					for (bool ret = true; 0 <= x && 0 <= y && ret; x -= 1, y -= 1)
					{
						ret &= AddMove(PAWN_ROLL::NONE, i, j, x, y, false, moveList);
						if (lineMin <= y && y <= lineMax)
						{
							ret &= AddMove(PAWN_ROLL::NONE, i, j, x, y, true, moveList);
						}
					}
					break;
				case PAWN_TYPE::UMA:
					x = i + 1; y = j + 1;
					for (bool ret = true; x < BOARD_WIDTH && y < BOARD_HEIGHT && ret; x++, y++)
					{
						ret &= AddMove(PAWN_ROLL::NONE, i, j, x, y, false, moveList);
					}
					x = i + 1; y = j - 1;
					for (bool ret = true; x < BOARD_WIDTH && 0 <= y && ret; x++, y -= 1)
					{
						ret &= AddMove(PAWN_ROLL::NONE, i, j, x, y, false, moveList);
					}
					x = i - 1; y = j + 1;
					for (bool ret = true; 0 <= x && y < BOARD_HEIGHT && ret; x -= 1, y++)
					{
						ret &= AddMove(PAWN_ROLL::NONE, i, j, x, y, false, moveList);
					}
					x = i - 1; y = j - 1;
					for (bool ret = true; 0 <= x && 0 <= y && ret; x -= 1, y -= 1)
					{
						ret &= AddMove(PAWN_ROLL::NONE, i, j, x, y, false, moveList);
					}
					AddMove(PAWN_ROLL::NONE, i, j, i + 1, j, false, moveList);
					AddMove(PAWN_ROLL::NONE, i, j, i - 1, j, false, moveList);
					AddMove(PAWN_ROLL::NONE, i, j, i, j + 1, false, moveList);
					AddMove(PAWN_ROLL::NONE, i, j, i, j - 1, false, moveList);
					break;
				case PAWN_TYPE::HI:
					x = i + 1; y = j;
					for (bool ret = true; x < BOARD_WIDTH && ret; x++)
					{
						ret &= AddMove(PAWN_ROLL::NONE, i, j, x, y, false, moveList);
						if (lineMin <= y && y <= lineMax)
						{
							ret &= AddMove(PAWN_ROLL::NONE, i, j, x, y, true, moveList);
						}
					}
					x = i - 1; y = j;
					for (bool ret = true; 0 <= x && ret; x -= 1)
					{
						ret &= AddMove(PAWN_ROLL::NONE, i, j, x, y, false, moveList);
						if (lineMin <= y && y <= lineMax)
						{
							ret &= AddMove(PAWN_ROLL::NONE, i, j, x, y, true, moveList);
						}
					}
					x = i; y = j + 1;
					for (bool ret = true; y < BOARD_HEIGHT && ret; y++)
					{
						ret &= AddMove(PAWN_ROLL::NONE, i, j, x, y, false, moveList);
						if (lineMin <= y && y <= lineMax)
						{
							ret &= AddMove(PAWN_ROLL::NONE, i, j, x, y, true, moveList);
						}
					}
					x = i; y = j - 1;
					for (bool ret = true; 0 <= y && ret; y -= 1)
					{
						ret &= AddMove(PAWN_ROLL::NONE, i, j, x, y, false, moveList);
						if (lineMin <= y && y <= lineMax)
						{
							ret &= AddMove(PAWN_ROLL::NONE, i, j, x, y, true, moveList);
						}
					}
					break;
				case PAWN_TYPE::RYU:
					x = i + 1; y = j;
					for (bool ret = true; x < BOARD_WIDTH && ret; x++)
					{
						ret &= AddMove(PAWN_ROLL::NONE, i, j, x, y, false, moveList);
					}
					x = i - 1; y = j;
					for (bool ret = true; 0 <= x && ret; x -= 1)
					{
						ret &= AddMove(PAWN_ROLL::NONE, i, j, x, y, false, moveList);
					}
					x = i; y = j + 1;
					for (bool ret = true; y < BOARD_HEIGHT && ret; y++)
					{
						ret &= AddMove(PAWN_ROLL::NONE, i, j, x, y, false, moveList);
					}
					x = i; y = j - 1;
					for (bool ret = true; 0 <= y && ret; y -= 1)
					{
						ret &= AddMove(PAWN_ROLL::NONE, i, j, x, y, false, moveList);
					}
					AddMove(PAWN_ROLL::NONE, i, j, i + 1, j + 1, false, moveList);
					AddMove(PAWN_ROLL::NONE, i, j, i - 1, j + 1, false, moveList);
					AddMove(PAWN_ROLL::NONE, i, j, i + 1, j - 1, false, moveList);
					AddMove(PAWN_ROLL::NONE, i, j, i - 1, j - 1, false, moveList);
					break;
				case PAWN_TYPE::GYOKU:
					AddMove(PAWN_ROLL::NONE, i, j, i - 1, j - 1, false, moveList);
					AddMove(PAWN_ROLL::NONE, i, j, i, j - 1, false, moveList);
					AddMove(PAWN_ROLL::NONE, i, j, i + 1, j - 1, false, moveList);
					AddMove(PAWN_ROLL::NONE, i, j, i - 1, j, false, moveList);
					AddMove(PAWN_ROLL::NONE, i, j, i + 1, j, false, moveList);
					AddMove(PAWN_ROLL::NONE, i, j, i - 1, j + 1, false, moveList);
					AddMove(PAWN_ROLL::NONE, i, j, i, j + 1, false, moveList);
					AddMove(PAWN_ROLL::NONE, i, j, i + 1, j + 1, false, moveList);
					break;
				default:
					break;
				}
			}
		}

		PAWN_MOVE move = PAWN_MOVE_ZERO;

		for (uchar roll = 0; roll<(uchar)PAWN_ROLL::CAPTURE_MAX; roll++)
		{
			if (captured[(uchar)turn][roll] == 0)
			{
				continue;
			}
			move.reserve = (PAWN_ROLL)roll;
			for (uchar j = 0; j<BOARD_HEIGHT; j++)
			{
				for (uchar i = 0; i<BOARD_WIDTH; i++)
				{
					if (matrix[j][i].player != PLAYER::NONE || matrix[j][i].pawn != PAWN_TYPE::NONE)
					{
						continue;
					}

					switch (roll)
					{
					case PAWN_ROLL::HU:
						if (j != lineTop)
						{
							uchar k;
							for (k = 0; k<BOARD_HEIGHT; k++)
							{
								if (matrix[k][i].player == turn && matrix[k][i].pawn == PAWN_TYPE::HU)
								{
									break;
								}
							}
							if (BOARD_HEIGHT <= k)
							{
								AddMove((PAWN_ROLL)roll, 0, 0, i, j, false, moveList);
							}
						}
						break;
					case PAWN_ROLL::KYOH:
						if (j != lineTop)
						{
							AddMove((PAWN_ROLL)roll, 0, 0, i, j, false, moveList);
						}
						break;
					case PAWN_ROLL::KEI:
						if (j != lineTop && j != lineMid)
						{
							AddMove((PAWN_ROLL)roll, 0, 0, i, j, false, moveList);
						}
						break;
						break;
					case PAWN_ROLL::GIN:
					case PAWN_ROLL::KIN:
					case PAWN_ROLL::KAKU:
					case PAWN_ROLL::HI:
						AddMove((PAWN_ROLL)roll, 0, 0, i, j, false, moveList);
						break;
					default:
						break;
					}
				}
			}
		}

		return moveList;
	}

	void Move(const PAWN_MOVE &move);
	void Back(const PAWN_MOVE &move);
	void SwitchTurn();
	
	CELL GetCell(uchar x, uchar y) { return matrix[y][x]; }
	
	void PrintBoard() const;

private:
	bool AddMove(PAWN_ROLL roll, uchar fromx, uchar fromy, char tox, char toy, bool upgrade, std::list<Board::PAWN_MOVE> &moveList);
	bool AddMove(PAWN_ROLL roll, uchar fromx, uchar fromy, char tox, char toy, bool upgrade, std::vector<Board::PAWN_MOVE> &moveList);
	bool AddMove(PAWN_ROLL roll, uchar fromx, uchar fromy, char tox, char toy, bool upgrade, std::multimap<int, Board::PAWN_MOVE> &moveMap );
	bool IsEnd();
	bool IsCapture( char tox, char toy, PLAYER enemy, bool &isCapture );
	
	uchar captured[(uchar)PLAYER::MAX][(uchar)PAWN_ROLL::CAPTURE_MAX];
	CELL matrix[BOARD_HEIGHT][BOARD_WIDTH];
	PLAYER turn;
};

static const Board::PAWN_MOVE PAWN_MOVE_ZERO{ PAWN_ROLL::NONE, 0, 0, 0, 0, PAWN_TYPE::NONE, PAWN_TYPE::NONE, false };

#endif // BOARD_H