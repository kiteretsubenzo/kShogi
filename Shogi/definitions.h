#ifndef DEFINITIONS_H
#define DEFINITIONS_H

typedef unsigned char uchar;
typedef unsigned char PAWN;
typedef unsigned char PLAYER;

class BoardDef
{
public:
	static const int WIDTH = 9;
	static const int HEIGHT = 9;
};

class PawnDef
{
public:
	static const PAWN HU = 0;
	static const PAWN KYOH = 1;
	static const PAWN KEI = 2;
	static const PAWN GIN = 3;
	static const PAWN KAKU = 4;
	static const PAWN HI = 5;
	static const PAWN KIN = 6;
	static const PAWN GYOKU = 7;
	static const PAWN HUN = 8;
	static const PAWN KYOHN = 9;
	static const PAWN KEIN = 10;
	static const PAWN GINN = 11;
	static const PAWN UMA = 12;
	static const PAWN RYU = 13;
	static const PAWN MAX = 14;
	static const PAWN NONE = MAX;
	static const PAWN CAPTURE_MAX = GYOKU;
};

class PlayerDef
{
public:
	static const PLAYER FIRST = 0;
	static const PLAYER SECOND = 1;
	static const PLAYER NONE = 2;
	static const PLAYER WALL = 3;

	static const PLAYER MAX = 2;
};

static const char PAWN_CHAR[PawnDef::MAX + 1] =
{
	'h', 'y', 'e', 'g', 'u', 'r', 'k', 'o', 'H', 'Y', 'E', 'G', 'U', 'R', 'n'
};

static const std::string PAWN_KANJI[PawnDef::MAX] =
{
	"•à", "ˆÇ", "Œj", "‹â", "Šp", "”ò", "‹à", "‹Ê", "‚Æ", "—ß", "‰î", "‘S", "”n", "—´"
};

static const PAWN charToPawn[256] =
{
	PawnDef::NONE,	PawnDef::NONE,	PawnDef::NONE,	PawnDef::NONE,	PawnDef::NONE,	PawnDef::NONE,	PawnDef::NONE,	PawnDef::NONE,	PawnDef::NONE,	PawnDef::NONE,	PawnDef::NONE,	PawnDef::NONE,	PawnDef::NONE,	PawnDef::NONE,	PawnDef::NONE,	PawnDef::NONE,
	PawnDef::NONE,	PawnDef::NONE,	PawnDef::NONE,	PawnDef::NONE,	PawnDef::NONE,	PawnDef::NONE,	PawnDef::NONE,	PawnDef::NONE,	PawnDef::NONE,	PawnDef::NONE,	PawnDef::NONE,	PawnDef::NONE,	PawnDef::NONE,	PawnDef::NONE,	PawnDef::NONE,	PawnDef::NONE,
	PawnDef::NONE,	PawnDef::NONE,	PawnDef::NONE,	PawnDef::NONE,	PawnDef::NONE,	PawnDef::NONE,	PawnDef::NONE,	PawnDef::NONE,	PawnDef::NONE,	PawnDef::NONE,	PawnDef::NONE,	PawnDef::NONE,	PawnDef::NONE,	PawnDef::NONE,	PawnDef::NONE,	PawnDef::NONE,
	PawnDef::NONE,	PawnDef::NONE,	PawnDef::NONE,	PawnDef::NONE,	PawnDef::NONE,	PawnDef::NONE,	PawnDef::NONE,	PawnDef::NONE,	PawnDef::NONE,	PawnDef::NONE,	PawnDef::NONE,	PawnDef::NONE,	PawnDef::NONE,	PawnDef::NONE,	PawnDef::NONE,	PawnDef::NONE,
	PawnDef::NONE,	PawnDef::NONE,	PawnDef::NONE,	PawnDef::NONE,	PawnDef::NONE,	PawnDef::KEIN,	PawnDef::NONE,	PawnDef::GINN,	PawnDef::HUN,	PawnDef::NONE,	PawnDef::NONE,	PawnDef::NONE,	PawnDef::NONE,	PawnDef::NONE,	PawnDef::NONE,	PawnDef::NONE,
	PawnDef::NONE,	PawnDef::NONE,	PawnDef::RYU,	PawnDef::NONE,	PawnDef::NONE,	PawnDef::UMA,	PawnDef::NONE,	PawnDef::NONE,	PawnDef::NONE,	PawnDef::KYOHN,	PawnDef::NONE,	PawnDef::NONE,	PawnDef::NONE,	PawnDef::NONE,	PawnDef::NONE,	PawnDef::NONE,
	PawnDef::NONE,	PawnDef::NONE,	PawnDef::NONE,	PawnDef::NONE,	PawnDef::NONE,	PawnDef::KEI,	PawnDef::NONE,	PawnDef::GIN,	PawnDef::HU,	PawnDef::NONE,	PawnDef::NONE,	PawnDef::KIN,	PawnDef::NONE,	PawnDef::NONE,	PawnDef::NONE,	PawnDef::GYOKU,
	PawnDef::NONE,	PawnDef::NONE,	PawnDef::HI,	PawnDef::NONE,	PawnDef::NONE,	PawnDef::KAKU,	PawnDef::NONE,	PawnDef::NONE,	PawnDef::NONE,	PawnDef::KYOH,	PawnDef::NONE,	PawnDef::NONE,	PawnDef::NONE,	PawnDef::NONE,	PawnDef::NONE,	PawnDef::NONE,
	PawnDef::NONE,	PawnDef::NONE,	PawnDef::NONE,	PawnDef::NONE,	PawnDef::NONE,	PawnDef::NONE,	PawnDef::NONE,	PawnDef::NONE,	PawnDef::NONE,	PawnDef::NONE,	PawnDef::NONE,	PawnDef::NONE,	PawnDef::NONE,	PawnDef::NONE,	PawnDef::NONE,	PawnDef::NONE,
	PawnDef::NONE,	PawnDef::NONE,	PawnDef::NONE,	PawnDef::NONE,	PawnDef::NONE,	PawnDef::NONE,	PawnDef::NONE,	PawnDef::NONE,	PawnDef::NONE,	PawnDef::NONE,	PawnDef::NONE,	PawnDef::NONE,	PawnDef::NONE,	PawnDef::NONE,	PawnDef::NONE,	PawnDef::NONE,
	PawnDef::NONE,	PawnDef::NONE,	PawnDef::NONE,	PawnDef::NONE,	PawnDef::NONE,	PawnDef::NONE,	PawnDef::NONE,	PawnDef::NONE,	PawnDef::NONE,	PawnDef::NONE,	PawnDef::NONE,	PawnDef::NONE,	PawnDef::NONE,	PawnDef::NONE,	PawnDef::NONE,	PawnDef::NONE,
	PawnDef::NONE,	PawnDef::NONE,	PawnDef::NONE,	PawnDef::NONE,	PawnDef::NONE,	PawnDef::NONE,	PawnDef::NONE,	PawnDef::NONE,	PawnDef::NONE,	PawnDef::NONE,	PawnDef::NONE,	PawnDef::NONE,	PawnDef::NONE,	PawnDef::NONE,	PawnDef::NONE,	PawnDef::NONE,
	PawnDef::NONE,	PawnDef::NONE,	PawnDef::NONE,	PawnDef::NONE,	PawnDef::NONE,	PawnDef::NONE,	PawnDef::NONE,	PawnDef::NONE,	PawnDef::NONE,	PawnDef::NONE,	PawnDef::NONE,	PawnDef::NONE,	PawnDef::NONE,	PawnDef::NONE,	PawnDef::NONE,	PawnDef::NONE,
	PawnDef::NONE,	PawnDef::NONE,	PawnDef::NONE,	PawnDef::NONE,	PawnDef::NONE,	PawnDef::NONE,	PawnDef::NONE,	PawnDef::NONE,	PawnDef::NONE,	PawnDef::NONE,	PawnDef::NONE,	PawnDef::NONE,	PawnDef::NONE,	PawnDef::NONE,	PawnDef::NONE,	PawnDef::NONE,
	PawnDef::NONE,	PawnDef::NONE,	PawnDef::NONE,	PawnDef::NONE,	PawnDef::NONE,	PawnDef::NONE,	PawnDef::NONE,	PawnDef::NONE,	PawnDef::NONE,	PawnDef::NONE,	PawnDef::NONE,	PawnDef::NONE,	PawnDef::NONE,	PawnDef::NONE,	PawnDef::NONE,	PawnDef::NONE,
	PawnDef::NONE,	PawnDef::NONE,	PawnDef::NONE,	PawnDef::NONE,	PawnDef::NONE,	PawnDef::NONE,	PawnDef::NONE,	PawnDef::NONE,	PawnDef::NONE,	PawnDef::NONE,	PawnDef::NONE,	PawnDef::NONE,	PawnDef::NONE,	PawnDef::NONE,	PawnDef::NONE,	PawnDef::NONE
};

static const std::string numberToZenkaku[9] =
{
	"‚X", "‚W", "‚V", "‚U", "‚T", "‚S", "‚R", "‚Q", "‚P"
};

static const std::string numberToKanji[9] =
{
	"ˆê", "“ñ", "ŽO", "Žl", "ŒÜ", "˜Z", "Žµ", "”ª", "‹ã"
};

#endif // DEFINITIONS_H