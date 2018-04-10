#ifndef DEFINITIONS_H
#define DEFINITIONS_H

#define BOARD_WIDTH 9
#define BOARD_HEIGHT 9

typedef unsigned char uchar;
typedef unsigned char PAWN;

#define PAWN_HU		0
#define PAWN_KYOH	1
#define PAWN_KEI	2
#define PAWN_GIN	3
#define PAWN_KAKU	4
#define PAWN_HI		5
#define PAWN_KIN	6
#define PAWN_GYOKU	7
#define PAWN_HUN	8
#define PAWN_KYOHN	9
#define PAWN_KEIN	10
#define PAWN_GINN	11
#define PAWN_UMA	12
#define PAWN_RYU	13
#define PAWN_MAX	14
#define PAWN_NONE	PAWN_MAX
#define CAPTURE_MAX	PAWN_GYOKU

static const char PAWN_CHAR[PAWN_MAX] =
{
	'h', 'y', 'e', 'g', 'u', 'r', 'k', 'o', 'H', 'Y', 'E', 'G', 'U', 'R'
};

static const std::string PAWN_KANJI[PAWN_MAX] =
{
	"歩", "杏", "桂", "銀", "角", "飛", "金", "玉", "と", "令", "介", "全", "馬", "龍",
};

#define Upgrade(type)	(type |= 0x08)
#define Downgrade(type)	(type &= 0x07)
#define IsUpgrade(type)	((type & 0x08) != 0)
#define IsGyokuKinUpgrade(type) (PAWN_KIN <= type)

static const int SCORE_NONE = std::numeric_limits<int>::max() - 1;
static const int SCORE_WIN = 99999;
static const int MOVES_MAX = (BOARD_WIDTH + BOARD_HEIGHT - 2) * BOARD_WIDTH * BOARD_HEIGHT * 2 + (CAPTURE_MAX - 1) * BOARD_WIDTH * BOARD_HEIGHT;

std::vector<std::string> split(std::string str, char c);
std::unordered_map<std::string, std::string> fromJson(std::string str);

#endif // DEFINITIONS_H