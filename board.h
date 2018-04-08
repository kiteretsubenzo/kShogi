﻿#ifndef BOARD_H
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
		PAWN_TYPE fromPawn;
		PAWN_TYPE toPawn;
		bool upgrade;
		int priority;
		
		std::string DebugString() const
		{
			if( reserve == PAWN_ROLL::NONE && pos.uint32_pos == 0 )
			{
				return "ZERO";
			}
			
			std::string str;
			//uchar tox, toy;
			str += numberToZenkaku[(uchar)pos.pos.tox] + numberToKanji[(uchar)pos.pos.toy];
			if( reserve != PAWN_ROLL::NONE )
			{
				//PAWN_ROLL reserve;
				str +=  " " + (std::string)reserve + " 打ち";
			}
			else
			{
				str += " " + (std::string)fromPawn;
				str += "(" + std::to_string(BOARD_WIDTH-pos.pos.fromx) + "," + std::to_string(pos.pos.fromy+1) + ")";
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

private:
	bool AddMove(PAWN_ROLL roll, uchar fromx, uchar fromy, char tox, char toy, bool upgrade, std::list<Board::PAWN_MOVE> &moveList);
	bool IsEnd();
	bool IsCapture( char tox, char toy, PLAYER enemy, bool &isCapture );
	
	uchar captured[(uchar)PLAYER::MAX][(uchar)PAWN_ROLL::CAPTURE_MAX];
	CELL matrix[BOARD_HEIGHT][BOARD_WIDTH];
	PLAYER turn;
};

static const Board::PAWN_MOVE PAWN_MOVE_ZERO{ PAWN_ROLL::NONE, 0, 0, 0, 0, PAWN_TYPE::NONE, PAWN_TYPE::NONE, false };

#endif // BOARD_H