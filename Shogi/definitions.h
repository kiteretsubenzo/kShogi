#ifndef DEFINITIONS_H
#define DEFINITIONS_H

typedef unsigned char uchar;
typedef unsigned char Pawn;
typedef unsigned char Player;

class BoardDef
{
public:
	static const int WIDTH = 9;
	static const int HEIGHT = 9;
};

class PawnDef
{
public:
	static const Pawn HU = 0;
	static const Pawn KYOH = 1;
	static const Pawn KEI = 2;
	static const Pawn GIN = 3;
	static const Pawn KAKU = 4;
	static const Pawn HI = 5;
	static const Pawn KIN = 6;
	static const Pawn GYOKU = 7;
	static const Pawn HUN = 8;
	static const Pawn KYOHN = 9;
	static const Pawn KEIN = 10;
	static const Pawn GINN = 11;
	static const Pawn UMA = 12;
	static const Pawn RYU = 13;
	static const Pawn MAX = 14;
	static const Pawn NONE = MAX;
	static const Pawn CAPTURE_MAX = GYOKU;
};

class PlayerDef
{
public:
	static const Player FIRST = 0;
	static const Player SECOND = 1;
	static const Player NONE = 2;
	static const Player WALL = 3;

	static const Player MAX = 2;
};

static const char Pawn_CHAR[PawnDef::MAX + 1] =
{
	'h', 'y', 'e', 'g', 'u', 'r', 'k', 'o', 'H', 'Y', 'E', 'G', 'U', 'R', 'n'
};

static const std::string Pawn_KANJI[PawnDef::MAX] =
{
	"•à", "ˆÇ", "Œj", "‹â", "Šp", "”ò", "‹à", "‹Ê", "‚Æ", "—ß", "‰î", "‘S", "”n", "—´"
};

static const Pawn charToPawn[256] =
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