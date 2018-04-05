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
	
	std::vector<PAWN_MOVE> GetMoveList();
	void Move(const PAWN_MOVE &move);
	void Back(const PAWN_MOVE &move);
	
	CELL GetCell(uchar x, uchar y) { return matrix[y][x]; }
	
	void PrintBoard() const;

private:
	bool AddMove( uchar fromx, uchar fromy, char tox, char toy, bool upgrade, std::vector<Board::PAWN_MOVE> &moveList );
	bool IsEnd();
	bool IsCapture( char tox, char toy, PLAYER enemy, bool &isCapture );
	
	uchar captured[(uchar)PLAYER::MAX][(uchar)PAWN_ROLL::CAPTURE_MAX];
  CELL matrix[BOARD_HEIGHT][BOARD_WIDTH];
	PLAYER turn;
};

static const Board::PAWN_MOVE PAWN_MOVE_ZERO{ PAWN_ROLL::NONE, 0, 0, 0, 0, PAWN_TYPE::NONE, PAWN_TYPE::NONE, false };

#endif // BOARD_H