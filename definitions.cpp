#include <iostream>
#include "definitions.h"

// PAWN_TYPE

PAWN_TYPE::operator char() const
{
	static const char PAWN_CHAR[(uchar)PAWN_TYPE::MAX] =
	{
		'h', 'H', 'y', 'Y', 'e', 'E', 'g', 'G', 'k', 'u', 'U', 'r', 'R', 'o'
	};
	return PAWN_CHAR[(int)type];
}

PAWN_TYPE::operator std::string() const
{
	static const std::string PAWN_KANJI[(uchar)PAWN_TYPE::MAX] =
	{
		"歩", "と", "杏", "令", "桂", "介", "銀", "全", "金", "角", "馬", "飛", "龍", "玉"
	};
	return PAWN_KANJI[(int)type];
}

PAWN_TYPE::PAWN_TYPE( const PAWN_ROLL &roll )
{
	static const PAWN_TYPE::TYPE rollToType[(uchar)PAWN_ROLL::MAX] =
	{
		PAWN_TYPE::HU, PAWN_TYPE::KYOH, PAWN_TYPE::KEI, PAWN_TYPE::GIN, PAWN_TYPE::KIN, PAWN_TYPE::KAKU, PAWN_TYPE::HI, PAWN_TYPE::GYOKU
	};
	type = rollToType[(int)roll];
}

void PAWN_TYPE::Upgrade()
{
	static const PAWN_TYPE::TYPE toUpgrade[(uchar)PAWN_TYPE::MAX] = 
	{
		PAWN_TYPE::HUN, PAWN_TYPE::HUN, PAWN_TYPE::KYOHN, PAWN_TYPE::KYOHN, PAWN_TYPE::KEIN, PAWN_TYPE::KEIN,
		PAWN_TYPE::GINN, PAWN_TYPE::GINN, PAWN_TYPE::KIN, PAWN_TYPE::UMA, PAWN_TYPE::UMA, PAWN_TYPE::RYU, PAWN_TYPE::RYU, PAWN_TYPE::GYOKU
	};
	type = toUpgrade[(int)type];
}

void PAWN_TYPE::Downgrade()
{
	static const PAWN_TYPE::TYPE toDowngrade[(uchar)PAWN_TYPE::MAX] = 
	{
		PAWN_TYPE::HU, PAWN_TYPE::HU, PAWN_TYPE::KYOH, PAWN_TYPE::KYOH, PAWN_TYPE::KEI, PAWN_TYPE::KEI,
		PAWN_TYPE::GIN, PAWN_TYPE::GIN, PAWN_TYPE::KIN, PAWN_TYPE::KAKU, PAWN_TYPE::KAKU, PAWN_TYPE::HI, PAWN_TYPE::HI, PAWN_TYPE::GYOKU
	};
	type = toDowngrade[(int)type];
}

// PAWN_ROLL

PAWN_ROLL::operator char() const
{
	const char ROLL_CHAR[(uchar)PAWN_ROLL::MAX] =
	{
		'h', 'y', 'e', 'g', 'k', 'u', 'r', 'o'
	};
	return ROLL_CHAR[(int)roll];
}

PAWN_ROLL::operator std::string () const
{
	const std::string ROLL_KANJI[(uchar)PAWN_ROLL::MAX] =
	{
		"歩", "杏", "桂", "銀", "金", "角", "飛", "玉"
	};
	return ROLL_KANJI[(int)roll];
}

PAWN_ROLL::PAWN_ROLL(const PAWN_TYPE &type)
{
	static const PAWN_ROLL::ROLL typeToRoll[(uchar)PAWN_TYPE::MAX] =
	{
		PAWN_ROLL::HU, PAWN_ROLL::HU, PAWN_ROLL::KYOH, PAWN_ROLL::KYOH, PAWN_ROLL::KEI, PAWN_ROLL::KEI,
		PAWN_ROLL::GIN, PAWN_ROLL::GIN, PAWN_ROLL::KIN, PAWN_ROLL::KAKU, PAWN_ROLL::KAKU, PAWN_ROLL::HI, PAWN_ROLL::HI, PAWN_ROLL::GYOKU
	};
	roll = typeToRoll[(int)type];
}