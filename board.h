#ifndef BOARD_H
#define BOARD_H

#define BOARD_WIDTH 9
#define BOARD_HEIGHT 9

typedef unsigned char uchar;

enum class PAWN_TYPE
{
  HU, HUN, KYOH, KYOHN, KEI, KEIN, GIN, GINN, KIN, KAKU, UMA, HI, RYU, GYOKU,
  MAX, NONE = MAX
};

enum class PAWN_ROLL
{
  HU, KYOH, KEI, GIN, KIN, HI, KAKU, GYOKU, MAX,
  NONE = MAX, CAPTURE_MAX = GYOKU
};

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

static const char PAWN_CHAR[(uchar)PAWN_TYPE::MAX] =
{
  'h', 'H', 'y', 'Y', 'e', 'E', 'g', 'G', 'k', 'u', 'U', 'r', 'R', 'o'
};

static const char ROLL_CHAR[(uchar)PAWN_ROLL::MAX] =
{
  'h', 'y', 'e', 'g', 'k', 'u', 'r', 'o'
};

static const std::string PLAYER_STRING[(uchar)PLAYER::MAX] =
{
	"先手番", "後手番"
};

static const PAWN_ROLL typeToRoll[(uchar)PAWN_TYPE::MAX] =
{
  PAWN_ROLL::HU, PAWN_ROLL::HU, PAWN_ROLL::KYOH, PAWN_ROLL::KYOH, PAWN_ROLL::KEI, PAWN_ROLL::KEI,
  PAWN_ROLL::GIN, PAWN_ROLL::GIN, PAWN_ROLL::KIN, PAWN_ROLL::KAKU, PAWN_ROLL::KAKU, PAWN_ROLL::HI, PAWN_ROLL::HI, PAWN_ROLL::GYOKU
};

static const PAWN_TYPE rollToType[(uchar)PAWN_ROLL::MAX] =
{
  PAWN_TYPE::HU, PAWN_TYPE::KYOH, PAWN_TYPE::KEI, PAWN_TYPE::GIN, PAWN_TYPE::KIN, PAWN_TYPE::KAKU, PAWN_TYPE::HI, PAWN_TYPE::GYOKU
};

static const PAWN_TYPE toUpgrade[(uchar)PAWN_TYPE::MAX] = 
{
	PAWN_TYPE::HUN, PAWN_TYPE::HUN, PAWN_TYPE::KYOHN, PAWN_TYPE::KYOHN, PAWN_TYPE::KEIN, PAWN_TYPE::KEIN,
	PAWN_TYPE::GINN, PAWN_TYPE::GINN, PAWN_TYPE::KIN, PAWN_TYPE::UMA, PAWN_TYPE::UMA, PAWN_TYPE::RYU, PAWN_TYPE::RYU, PAWN_TYPE::GYOKU
};

static const PAWN_TYPE toDowngrade[(uchar)PAWN_TYPE::MAX] = 
{
	PAWN_TYPE::HU, PAWN_TYPE::HU, PAWN_TYPE::KYOH, PAWN_TYPE::KYOH, PAWN_TYPE::KEI, PAWN_TYPE::KEI,
	PAWN_TYPE::GIN, PAWN_TYPE::GIN, PAWN_TYPE::KIN, PAWN_TYPE::KAKU, PAWN_TYPE::KAKU, PAWN_TYPE::HI, PAWN_TYPE::HI, PAWN_TYPE::GYOKU
};

static const std::string numberToZenkaku[9] =
{
	"９", "８", "７", "６", "５", "４", "３", "２", "１"
};

static const std::string numberToKanji[9] =
{
	"一", "二", "三", "四", "五", "六", "七", "八", "九"
};

static const std::string PAWN_KANJI[(uchar)PAWN_TYPE::MAX] =
{
  "歩", "と", "杏", "き", "桂", "け", "銀", "全", "金", "角", "馬", "飛", "龍", "玉"
};

static const std::string ROLL_KANJI[(uchar)PAWN_ROLL::MAX] =
{
  "歩", "杏", "桂", "銀", "金", "角", "飛", "玉"
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
		PAWN_TYPE capture;
		bool upgrade;
	};
	
	Board();
	
	void Init(const std::string str[]);
	
	void GetMoveList(std::vector<PAWN_MOVE> &moveList);
	void Move(const PAWN_MOVE &move);
	void Back(const PAWN_MOVE &move);
	
	CELL GetCell(uchar x, uchar y) { return matrix[y][x]; }
	
	void PrintBoard() const;
	static void PrintMove(const PAWN_MOVE &move);
	void PrintKihu(const PAWN_MOVE &move);

private:
	bool AddMove( uchar fromx, uchar fromy, char tox, char toy, bool upgrade, std::vector<Board::PAWN_MOVE> &moveList );
	bool IsEnd();
	bool IsCapture( char tox, char toy, PLAYER enemy, bool &isCapture );
	
	uchar captured[(uchar)PLAYER::MAX][(uchar)PAWN_ROLL::CAPTURE_MAX];
  CELL matrix[BOARD_HEIGHT][BOARD_WIDTH];
	PLAYER turn;
};

#endif // BOARD_H