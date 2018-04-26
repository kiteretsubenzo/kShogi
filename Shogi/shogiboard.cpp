#include <iostream>
#include <sstream>
#include <iomanip>
#include <list>
#include <vector>
#include <unordered_map>
#include <set>
#include <cassert>
#include "../core/definitions.h"
#include "../core/pawnmove.h"
#include "../core/board.h"
#include "shogiboard.h"

//#define USE_SHOGI_PRIORITY

#define Upgrade(type)	(type |= 0x08)
#define Downgrade(type)	(type &= 0x07)
#define Down(type)		(type & 0x07)
#define IsUpgrade(type)	((type & 0x08) != 0)
#define IsGyokuKinUpgrade(type) (PAWN_KIN <= type)

ShogiBoard::ShogiBoard()
{
	for (uchar i = 0; i<PLAYER_MAX; i++)
	{
		for (uchar j = 0; j<(uchar)CAPTURE_MAX; j++)
		{
			captured[i][j] = 0;
		}
	}

	for (uchar j = 1; j <= BOARD_HEIGHT; j++)
	{
		for (uchar i = 1; i <= BOARD_WIDTH; i++)
		{
			matrix[j][i] = { PLAYER_NONE, PAWN_NONE };
		}
	}

	for (uchar i = 0; i < BOARD_WIDTH + 2; i++)
	{
		matrix[0][i] = { PLAYER_WALL, PAWN_NONE };
		matrix[BOARD_HEIGHT + 1][i] = { PLAYER_WALL, PAWN_NONE };
	}

	for (uchar j = 1; j <= BOARD_HEIGHT; j++)
	{
		matrix[j][0] = { PLAYER_WALL, PAWN_NONE };
		matrix[j][BOARD_WIDTH + 1] = { PLAYER_WALL, PAWN_NONE };
	}

	turn = PLAYER_FIRST;
	enemy = PLAYER_SECOND;
}

void ShogiBoard::Init(const std::string &str)
{
	//'h', 'H', 'y', 'Y', 'e', 'E', 'g', 'G', 'k', 'u', 'U', 'r', 'R', 'o';

	std::vector<std::string> strs = split(str, '\n');

	for (int i = 0; i < PLAYER_MAX; i++)
	{
		gyokux[i] = 0;
		gyokuy[i] = 0;
	}

	for (int i = 0; i<(uchar)CAPTURE_MAX; i++)
	{
		char first[3] = { strs[BOARD_HEIGHT + 1][i * 4 + 1], strs[BOARD_HEIGHT + 1][i * 4 + 2], '\0' };
		char second[3] = { strs[0][i * 4 + 1], strs[0][i * 4 + 2], '\0' };
		captured[PLAYER_FIRST][i] = atoi(first);
		captured[PLAYER_SECOND][i] = atoi(second);
	}

	for (int j = 2; j <= BOARD_HEIGHT + 1; j++)
	{
		for (int i = 1; i <= BOARD_WIDTH; i++)
		{
			char c[2] = { strs[j - 1][(i - 1) * 2], strs[j - 1][(i - 1) * 2 + 1] };
			if (c[0] == ' ')
			{
				matrix[j - 1][i].player = PLAYER_NONE;
				matrix[j - 1][i].pawn = PAWN_NONE;
			}
			else
			{
				char type;
				PLAYER player = PLAYER_NONE;
				if (c[0] == '^')
				{
					player = PLAYER_FIRST;
					type = c[1];
				}
				else
				{
					player = PLAYER_SECOND;
					type = c[0];
				}
				matrix[j - 1][i].player = player;
				matrix[j - 1][i].pawn = charToPawn[type];

				if (type == 'o')
				{
					gyokux[player] = i;
					gyokuy[player] = j - 1;
				}
			}
		}
	}

	if (strs[BOARD_HEIGHT + 2] == "first")
	{
		turn = PLAYER_FIRST;
		enemy = PLAYER_SECOND;
	}
	else if (strs[BOARD_HEIGHT + 2] == "second")
	{
		turn = PLAYER_SECOND;
		enemy = PLAYER_FIRST;
	}
}

std::string ShogiBoard::BoardToString() const
{
	std::ostringstream sout;

	for (int i = 0; i<(uchar)CAPTURE_MAX; i++)
	{
		sout << PAWN_CHAR[i];
		sout << std::setfill('0') << std::setw(2) << (int)(captured[PLAYER_SECOND][i]);
		sout << ' ';
	}
	sout << '\n';

	for (int j = 2; j <= BOARD_HEIGHT + 1; j++)
	{
		for (int i = 1; i <= BOARD_WIDTH; i++)
		{
			if (matrix[j - 1][i].player == PLAYER_FIRST)
			{
				sout << '^' << PAWN_CHAR[matrix[j - 1][i].pawn];
			}
			else if (matrix[j - 1][i].player == PLAYER_SECOND)
			{
				sout << PAWN_CHAR[matrix[j - 1][i].pawn] << '_';
			}
			else
			{
				sout << " .";
			}
		}
		sout << '\n';
	}

	for (int i = 0; i<(uchar)CAPTURE_MAX; i++)
	{
		sout << PAWN_CHAR[i];
		sout << std::setfill('0') << std::setw(2) << (int)captured[PLAYER_FIRST][i];
		sout << ' ';
	}
	sout << '\n';

	if (turn == PLAYER_FIRST)
	{
		sout << "first";
	}
	else
	{
		sout << "second";
	}

	return sout.str();
}

MoveList ShogiBoard::GetMoveList()
{
	MoveList moveList;

	uchar lineMax, lineMin, lineTop, lineMid;

	if (turn == PLAYER_FIRST)
	{
		lineMax = 3;
		lineMin = 1;
		lineTop = 1;
		lineMid = 2;
	}
	else
	{
		lineMax = BOARD_HEIGHT;
		lineMin = BOARD_HEIGHT - 2;
		lineTop = BOARD_HEIGHT;
		lineMid = BOARD_HEIGHT - 1;
	}
	int forward = -1;
	if (turn == PLAYER_SECOND)
	{
		forward = +1;
	}
	for (char j = 1; j <= BOARD_HEIGHT; j++)
	{
		for (char i = 1; i <= BOARD_WIDTH; i++)
		{
			CELL cell = matrix[(uchar)j][(uchar)i];
			if (cell.player == enemy)
			{
				continue;
			}
			PAWN pawn = cell.pawn;
			int x, y;
			switch (pawn)
			{
			case PAWN_HU:
				x = i;
				y = j + forward;
				if (y != lineTop)
				{
					AddMove(PAWN_NONE, i, j, x, y, false, moveList);
				}
				if (lineMin <= y && y <= lineMax)
				{
					AddMove(PAWN_NONE, i, j, x, y, true, moveList);
				}
				break;
			case PAWN_KYOH:
				y = j + forward;
				for (bool ret = true; 0 < y && y <= BOARD_HEIGHT && ret; y += forward)
				{
					if (y != lineTop)
					{
						ret &= AddMove(PAWN_NONE, i, j, i, y, false, moveList);
					}
					if (lineMin <= y && y <= lineMax)
					{
						ret &= AddMove(PAWN_NONE, i, j, i, y, true, moveList);
					}
				}
				break;
			case PAWN_KEI:
				x = i - 1;
				y = j - forward - forward;
				if (y != lineTop && y != lineMid)
				{
					AddMove(PAWN_NONE, i, j, x, y, false, moveList);
				}
				if (lineMin <= y && y <= lineMax)
				{
					AddMove(PAWN_NONE, i, j, x, y, true, moveList);
				}
				break;
			case PAWN_GIN:
				AddMove(PAWN_NONE, i, j, i - 1, j + forward, false, moveList);
				AddMove(PAWN_NONE, i, j, i, j + forward, false, moveList);
				AddMove(PAWN_NONE, i, j, i + 1, j + forward, false, moveList);
				AddMove(PAWN_NONE, i, j, i - 1, j - forward, false, moveList);
				AddMove(PAWN_NONE, i, j, i + 1, j - forward, false, moveList);
				if ((lineMin <= (j + forward) && (j + forward) <= lineMax) || (lineMin <= j && j <= lineMax))
				{
					AddMove(PAWN_NONE, i, j, i - 1, j + forward, true, moveList);
					AddMove(PAWN_NONE, i, j, i, j + forward, true, moveList);
					AddMove(PAWN_NONE, i, j, i + 1, j + forward, true, moveList);
					AddMove(PAWN_NONE, i, j, i - 1, j - forward, true, moveList);
					AddMove(PAWN_NONE, i, j, i + 1, j - forward, true, moveList);
				}
				break;
			case PAWN_KIN:
			case PAWN_HUN:
			case PAWN_KYOHN:
			case PAWN_KEIN:
			case PAWN_GINN:
				AddMove(PAWN_NONE, i, j, i - 1, j + forward, false, moveList);
				AddMove(PAWN_NONE, i, j, i, j + forward, false, moveList);
				AddMove(PAWN_NONE, i, j, i + 1, j + forward, false, moveList);
				AddMove(PAWN_NONE, i, j, i - 1, j, false, moveList);
				AddMove(PAWN_NONE, i, j, i + 1, j, false, moveList);
				AddMove(PAWN_NONE, i, j, i, j - forward, false, moveList);
				break;
			case PAWN_KAKU:
				x = i + 1; y = j + 1;
				while (true)
				{
					if (AddMove(PAWN_NONE, i, j, x, y, false, moveList) == false)
					{
						break;
					}
					if (lineMin <= y && y <= lineMax)
					{
						if (AddMove(PAWN_NONE, i, j, x, y, true, moveList) == false)
						{
							break;
						}
					}
					x++;
					y++;
				}
				x = i + 1; y = j - 1;
				while (true)
				{
					if (AddMove(PAWN_NONE, i, j, x, y, false, moveList) == false)
					{
						break;
					}
					if (lineMin <= y && y <= lineMax)
					{
						if (AddMove(PAWN_NONE, i, j, x, y, true, moveList) == false)
						{
							break;
						}
					}
					x++;
					y -= 1;
				}
				x = i - 1; y = j + 1;
				while (true)
				{
					if (AddMove(PAWN_NONE, i, j, x, y, false, moveList) == false)
					{
						break;
					}
					if (lineMin <= y && y <= lineMax)
					{
						if (AddMove(PAWN_NONE, i, j, x, y, true, moveList) == false)
						{
							break;
						}
					}
					x -= 1;
					y++;
				}
				x = i - 1; y = j - 1;
				while (true)
				{
					if (AddMove(PAWN_NONE, i, j, x, y, false, moveList) == false)
					{
						break;
					}
					if (lineMin <= y && y <= lineMax)
					{
						if (AddMove(PAWN_NONE, i, j, x, y, true, moveList) == false)
						{
							break;
						}
					}
					x -= 1;
					y -= 1;
				}
				break;
			case PAWN_UMA:
				x = i + 1; y = j + 1;
				while (true)
				{
					if (AddMove(PAWN_NONE, i, j, x, y, false, moveList) == false)
					{
						break;
					}
					x++;
					y++;
				}
				x = i + 1; y = j - 1;
				while (true)
				{
					if (AddMove(PAWN_NONE, i, j, x, y, false, moveList) == false)
					{
						break;
					}
					x++;
					y -= 1;
				}
				x = i - 1; y = j + 1;
				while (true)
				{
					if (AddMove(PAWN_NONE, i, j, x, y, false, moveList) == false)
					{
						break;
					}
					x -= 1;
					y++;
				}
				x = i - 1; y = j - 1;
				while (true)
				{
					if (AddMove(PAWN_NONE, i, j, x, y, false, moveList) == false)
					{
						break;
					}
					x -= 1;
					y -= 1;
				}
				AddMove(PAWN_NONE, i, j, i + 1, j, false, moveList);
				AddMove(PAWN_NONE, i, j, i - 1, j, false, moveList);
				AddMove(PAWN_NONE, i, j, i, j + 1, false, moveList);
				AddMove(PAWN_NONE, i, j, i, j - 1, false, moveList);
				break;
			case PAWN_HI:
				x = i + 1; y = j;
				while (true)
				{
					if (AddMove(PAWN_NONE, i, j, x, y, false, moveList) == false)
					{
						break;
					}
					if (lineMin <= y && y <= lineMax)
					{
						if (AddMove(PAWN_NONE, i, j, x, y, true, moveList) == false)
						{
							break;
						}
					}
					x++;
				}
				x = i - 1; y = j;
				while (true)
				{
					if (AddMove(PAWN_NONE, i, j, x, y, false, moveList) == false)
					{
						break;
					}
					if (lineMin <= y && y <= lineMax)
					{
						if (AddMove(PAWN_NONE, i, j, x, y, true, moveList) == false)
						{
							break;
						}
					}
					x -= 1;
				}
				x = i; y = j + 1;
				while (true)
				{
					if (AddMove(PAWN_NONE, i, j, x, y, false, moveList) == false)
					{
						break;
					}
					if (lineMin <= y && y <= lineMax)
					{
						if (AddMove(PAWN_NONE, i, j, x, y, true, moveList) == false)
						{
							break;
						}
					}
					y++;
				}
				x = i; y = j - 1;
				while (true)
				{
					if (AddMove(PAWN_NONE, i, j, x, y, false, moveList) == false)
					{
						break;
					}
					if (lineMin <= y && y <= lineMax)
					{
						if (AddMove(PAWN_NONE, i, j, x, y, true, moveList) == false)
						{
							break;
						}
					}
					y -= 1;
				}
				break;
			case PAWN_RYU:
				x = i + 1; y = j;
				while (true)
				{
					if (AddMove(PAWN_NONE, i, j, x, y, false, moveList) == false)
					{
						break;
					}
					x++;
				}
				x = i - 1; y = j;
				while (true)
				{
					if (AddMove(PAWN_NONE, i, j, x, y, false, moveList) == false)
					{
						break;
					}
					x -= 1;
				}
				x = i; y = j + 1;
				while (true)
				{
					if (AddMove(PAWN_NONE, i, j, x, y, false, moveList) == false)
					{
						break;
					}
					y++;
				}
				x = i; y = j - 1;
				while (true)
				{
					if (AddMove(PAWN_NONE, i, j, x, y, false, moveList) == false)
					{
						break;
					}
					y -= 1;
				}
				AddMove(PAWN_NONE, i, j, i + 1, j + 1, false, moveList);
				AddMove(PAWN_NONE, i, j, i - 1, j + 1, false, moveList);
				AddMove(PAWN_NONE, i, j, i + 1, j - 1, false, moveList);
				AddMove(PAWN_NONE, i, j, i - 1, j - 1, false, moveList);
				break;
			case PAWN_GYOKU:
				AddMove(PAWN_NONE, i, j, i - 1, j - 1, false, moveList);
				AddMove(PAWN_NONE, i, j, i, j - 1, false, moveList);
				AddMove(PAWN_NONE, i, j, i + 1, j - 1, false, moveList);
				AddMove(PAWN_NONE, i, j, i - 1, j, false, moveList);
				AddMove(PAWN_NONE, i, j, i + 1, j, false, moveList);
				AddMove(PAWN_NONE, i, j, i - 1, j + 1, false, moveList);
				AddMove(PAWN_NONE, i, j, i, j + 1, false, moveList);
				AddMove(PAWN_NONE, i, j, i + 1, j + 1, false, moveList);
				break;
			default:
				// ‹ó‚«‚¾‚Á‚½‚ç‘Å‚¿
				for (uchar roll = 0; roll < CAPTURE_MAX; roll++)
				{
					if (captured[turn][roll] == 0)
					{
						continue;
					}
					switch (roll)
					{
					case PAWN_HU:
						if (j != lineTop)
						{
							uchar k;
							for (k = 1; k <= BOARD_HEIGHT; k++)
							{
								if (matrix[k][i].player == turn && matrix[k][i].pawn == PAWN_HU)
								{
									break;
								}
							}
							if (BOARD_HEIGHT < k)
							{
								AddMove(roll, 0, 0, i, j, false, moveList);
							}
						}
						break;
					case PAWN_KYOH:
						if (j != lineTop)
						{
							AddMove(roll, 0, 0, i, j, false, moveList);
						}
						break;
					case PAWN_KEI:
						if (j != lineTop && j != lineMid)
						{
							AddMove(roll, 0, 0, i, j, false, moveList);
						}
						break;
					case PAWN_GIN:
					case PAWN_KIN:
					case PAWN_KAKU:
					case PAWN_HI:
						AddMove(roll, 0, 0, i, j, false, moveList);
						break;
					default:
						break;
					}
					break;
				}

				break;
			}
		}
	}

#ifdef USE_SHOGI_PRIORITY
	moveList.sort();
#endif
	return moveList;
}

bool ShogiBoard::AddMove(PAWN roll, uchar fromx, uchar fromy, uchar tox, uchar toy, bool upgrade, MoveList &moveList)
{
	if (matrix[toy][tox].player == turn || matrix[toy][tox].player == PLAYER_WALL)
	{
		return false;
	}

	PAWN_MOVE move(roll, fromx, fromy, tox, toy, matrix[fromy][fromx].pawn, matrix[toy][tox].pawn, upgrade, 0);
	if (roll != PAWN_NONE)
	{
		move.from.x = 0;
		move.from.y = 0;
		move.from.pawn = PAWN_NONE;
	}

	Move(move);
	// •‰‚¯‚éŽè‚ÍŽw‚³‚È‚¢
	if (IsEnd() == false)
	{
		move.priority = GetPriority(move);
		moveList.push(move);
	}
	Back(move);

	return matrix[toy][tox].pawn == PAWN_NONE;
}

int ShogiBoard::GetEvaluate(const MoveList &moveList)
{
	return (int)(moveList.size());
}

int ShogiBoard::GetPriority(const PAWN_MOVE &move)
{
	int priority = 0;
#ifdef USE_SHOGI_PRIORITY
	// ‰¤Žè‚ª‚©‚©‚Á‚Ä‚é‚©H
	SwitchTurn();
	if (IsEnd())
	{
		priority += 1000;
	}
	SwitchTurn();
	// ‹î‚ðŽæ‚é‚©H
	if (move.to.pawn != PAWN_NONE)
	{
		priority += (int)(move.to.pawn) + (int)PAWN_MAX;
	}
	// ¬‚é‚©H
	if (move.upgrade)
	{
		priority += (int)(move.from.pawn);
	}
#endif
	return priority;
}

bool ShogiBoard::IsEnd() const
{
	// ‹Ê‚ÌˆÊ’u‚ð‹‚ß‚é
	char gyokux = this->gyokux[enemy];
	char gyokuy = this->gyokuy[enemy];
	if (gyokux == 0)
	{
		return false;
	}
	CELL cell;
	if (turn == PLAYER_FIRST)
	{
		// ‹Ê‚ÌŽüˆÍ
		if (GetCell(gyokux - 1, gyokuy, cell))
		{
			if (IsGyokuKinUpgrade(cell.pawn))
			{
				return true;
			}
		}
		if (GetCell(gyokux + 1, gyokuy, cell))
		{
			if (IsGyokuKinUpgrade(cell.pawn))
			{
				return true;
			}
		}
		if (GetCell(gyokux, gyokuy - 1, cell))
		{
			if (IsGyokuKinUpgrade(cell.pawn))
			{
				return true;
			}
		}
		if (GetCell(gyokux, gyokuy + 1, cell))
		{
			if (IsGyokuKinUpgrade(cell.pawn) || cell.pawn == PAWN_HU || cell.pawn == PAWN_GIN)
			{
				return true;
			}
		}
		if (GetCell(gyokux - 1, gyokuy - 1, cell))
		{
			if (cell.pawn == PAWN_GYOKU || cell.pawn == PAWN_GIN || cell.pawn == PAWN_RYU)
			{
				return true;
			}
		}
		if (GetCell(gyokux + 1, gyokuy - 1, cell))
		{
			if (cell.pawn == PAWN_GYOKU || cell.pawn == PAWN_GIN || cell.pawn == PAWN_RYU)
			{
				return true;
			}
		}
		if (GetCell(gyokux + 1, gyokuy + 1, cell))
		{
			if (IsGyokuKinUpgrade(cell.pawn) || cell.pawn == PAWN_GIN)
			{
				return true;
			}
		}
		if (GetCell(gyokux - 1, gyokuy + 1, cell))
		{
			if (IsGyokuKinUpgrade(cell.pawn) || cell.pawn == PAWN_GIN)
			{
				return true;
			}
		}

		// Œj
		if (gyokuy < BOARD_HEIGHT)
		{
			if (GetCell(gyokux - 1, gyokuy + 2, cell))
			{
				if (cell.pawn == PAWN_KEI)
				{
					return true;
				}
			}
			if (GetCell(gyokux + 1, gyokuy + 2, cell))
			{
				if (cell.pawn == PAWN_KEI)
				{
					return true;
				}
			}
		}

		// 
		for (char j = gyokuy + 1; j <= BOARD_HEIGHT; j++)
		{
			if (matrix[j][gyokux].player == PLAYER_NONE)
			{
				continue;
			}
			if (matrix[j][gyokux].player == enemy)
			{
				break;
			}
			if (matrix[j][gyokux].pawn == PAWN_KYOH)
			{
				return true;
			}
			break;
		}
	}
	else
	{
		// ‹Ê‚ÌŽüˆÍ
		if (GetCell(gyokux - 1, gyokuy, cell))
		{
			if (IsGyokuKinUpgrade(cell.pawn))
			{
				return true;
			}
		}
		if (GetCell(gyokux + 1, gyokuy, cell))
		{
			if (IsGyokuKinUpgrade(cell.pawn))
			{
				return true;
			}
		}
		if (GetCell(gyokux, gyokuy - 1, cell))
		{
			if (IsGyokuKinUpgrade(cell.pawn) || cell.pawn == PAWN_HU || cell.pawn == PAWN_GIN)
			{
				return true;
			}
		}
		if (GetCell(gyokux, gyokuy + 1, cell))
		{
			if (IsGyokuKinUpgrade(cell.pawn))
			{
				return true;
			}
		}
		if (GetCell(gyokux - 1, gyokuy - 1, cell))
		{
			if (IsGyokuKinUpgrade(cell.pawn) || cell.pawn == PAWN_GIN)
			{
				return true;
			}
		}
		if (GetCell(gyokux + 1, gyokuy - 1, cell))
		{
			if (IsGyokuKinUpgrade(cell.pawn) || cell.pawn == PAWN_GIN)
			{
				return true;
			}
		}
		if (GetCell(gyokux + 1, gyokuy + 1, cell))
		{
			if (cell.pawn == PAWN_GYOKU || cell.pawn == PAWN_GIN || cell.pawn == PAWN_RYU)
			{
				return true;
			}
		}
		if (GetCell(gyokux - 1, gyokuy + 1, cell))
		{
			if (cell.pawn == PAWN_GYOKU || cell.pawn == PAWN_GIN || cell.pawn == PAWN_RYU)
			{
				return true;
			}
		}

		// Œj
		if (2 <= gyokuy)
		{
			if (GetCell(gyokux - 1, gyokuy - 2, cell))
			{
				if (cell.pawn == PAWN_KEI)
				{
					return true;
				}
			}
			if (GetCell(gyokux + 1, gyokuy - 2, cell))
			{
				if (cell.pawn == PAWN_KEI)
				{
					return true;
				}
			}
		}

		// 
		for (char j = gyokuy - 1; 0 < j; j--)
		{
			if (matrix[j][gyokux].player == PLAYER_NONE)
			{
				continue;
			}
			if (matrix[j][gyokux].player == enemy)
			{
				break;
			}
			if (matrix[j][gyokux].pawn == PAWN_KYOH)
			{
				return true;
			}
			break;
		}
	}

	int i, j;
	// ”ò—´
	i = gyokux + 1;
	while (true)
	{
		if (matrix[gyokuy][i].player == turn && Down(matrix[gyokuy][i].pawn) == PAWN_HI)
		{
			return true;
		}
		if (matrix[gyokuy][i].player != PLAYER_NONE)
		{
			break;
		}
		i++;
	}
	i = gyokux - 1;
	while (true)
	{
		if (matrix[gyokuy][i].player == turn && Down(matrix[gyokuy][i].pawn) == PAWN_HI)
		{
			return true;
		}
		if (matrix[gyokuy][i].player != PLAYER_NONE)
		{
			break;
		}
		i--;
	}
	j = gyokuy + 1;
	while (true)
	{
		if (matrix[j][gyokux].player == turn && Down(matrix[j][gyokux].pawn) == PAWN_HI)
		{
			return true;
		}
		if (matrix[j][gyokux].player != PLAYER_NONE)
		{
			break;
		}
		j++;
	}
	j = gyokuy - 1;
	while (true)
	{
		if (matrix[j][gyokux].player == turn && Down(matrix[j][gyokux].pawn) == PAWN_HI)
		{
			return true;
		}
		if (matrix[j][gyokux].player != PLAYER_NONE)
		{
			break;
		}
		j--;
	}

	// Šp”n
	i = gyokux + 1;
	j = gyokuy + 1;
	while (true)
	{
		if (matrix[j][i].player == turn && Down(matrix[j][i].pawn) == PAWN_KAKU)
		{
			return true;
		}
		if (matrix[j][i].player != PLAYER_NONE)
		{
			break;
		}
		i++;
		j++;
	}
	i = gyokux - 1;
	j = gyokuy + 1;
	while (true)
	{
		if (matrix[j][i].player == turn && Down(matrix[j][i].pawn) == PAWN_KAKU)
		{
			return true;
		}
		if (matrix[j][i].player != PLAYER_NONE)
		{
			break;
		}
		i--;
		j++;
	}
	i = gyokux - 1;
	j = gyokuy - 1;
	while (true)
	{
		if (matrix[j][i].player == turn && Down(matrix[j][i].pawn) == PAWN_KAKU)
		{
			return true;
		}
		if (matrix[j][i].player != PLAYER_NONE)
		{
			break;
		}
		i--;
		j--;
	}
	i = gyokux + 1;
	j = gyokuy - 1;
	while (true)
	{
		if (matrix[j][i].player == turn && Down(matrix[j][i].pawn) == PAWN_KAKU)
		{
			return true;
		}
		if (matrix[j][i].player != PLAYER_NONE)
		{
			break;
		}
		i++;
		j--;
	}

	return false;
}

void ShogiBoard::Move(const PAWN_MOVE &move)
{
	assert(move != PAWN_MOVE_ZERO);

	if (move.reserve != PAWN_NONE)
	{
		captured[turn][move.reserve]--;
		matrix[move.to.y][move.to.x].player = turn;
		matrix[move.to.y][move.to.x].pawn = move.reserve;

		SwitchTurn();
		return;
	}

	PAWN pawn = move.from.pawn;
	if (move.upgrade)
	{
		Upgrade(pawn);
	}
	matrix[move.to.y][move.to.x].player = turn;
	matrix[move.to.y][move.to.x].pawn = pawn;
	matrix[move.from.y][move.from.x].player = PLAYER_NONE;
	matrix[move.from.y][move.from.x].pawn = PAWN_NONE;

	if (move.to.pawn != PAWN_NONE)
	{
		PAWN roll = move.to.pawn;
		captured[turn][roll]++;
	}
	if (move.from.pawn == PAWN_GYOKU)
	{
		gyokux[turn] = move.to.x;
		gyokuy[turn] = move.to.y;
	}

	SwitchTurn();
}

void ShogiBoard::Back(const PAWN_MOVE &move)
{
	assert(move != PAWN_MOVE_ZERO);

	if (move.reserve != PAWN_NONE)
	{
		captured[enemy][move.reserve]++;

		matrix[move.to.y][move.to.x].player = PLAYER_NONE;
		matrix[move.to.y][move.to.x].pawn = PAWN_NONE;

		SwitchTurn();
		return;
	}

	PAWN pawn = move.from.pawn;
	if (move.upgrade)
	{
		Downgrade(pawn);
	}
	matrix[move.from.y][move.from.x].player = enemy;
	matrix[move.from.y][move.from.x].pawn = pawn;

	if (move.to.pawn != PAWN_NONE)
	{
		captured[enemy][move.to.pawn]--;

		matrix[move.to.y][move.to.x].player = turn;
		matrix[move.to.y][move.to.x].pawn = move.to.pawn;
	}
	else
	{
		matrix[move.to.y][move.to.x].player = PLAYER_NONE;
		matrix[move.to.y][move.to.x].pawn = PAWN_NONE;
	}

	if (move.from.pawn == PAWN_GYOKU)
	{
		gyokux[enemy] = move.from.x;
		gyokuy[enemy] = move.from.y;
	}

	SwitchTurn();
}

void ShogiBoard::PrintBoard() const
{
	for (uchar i = 0; i<(uchar)CAPTURE_MAX; i++)
	{
		std::cout << PAWN_KANJI[i] << ":" << (unsigned int)captured[PLAYER_SECOND][i] << " ";
	}
	std::cout << std::endl;
	std::cout << "‚X‚W‚V‚U‚T‚S‚R‚Q‚P" << std::endl;
	for (uchar j = 1; j <= BOARD_HEIGHT; j++)
	{
		for (uchar i = 1; i <= BOARD_WIDTH; i++)
		{
			switch (matrix[j][i].player)
			{
			case PLAYER_FIRST:
				std::cout << "^" << PAWN_CHAR[matrix[j][i].pawn];
				break;
			case PLAYER_SECOND:
				std::cout << PAWN_CHAR[matrix[j][i].pawn] << "_";
				break;
			case PLAYER_NONE:
				std::cout << " .";
				break;
			default:
				break;
			}
		}
		std::cout << " " << numberToKanji[j - 1] << std::endl;
	}
	for (uchar i = 0; i<(uchar)CAPTURE_MAX; i++)
	{
		std::cout << PAWN_KANJI[i] << ":" << (unsigned int)captured[PLAYER_FIRST][i] << " ";
	}
	std::cout << std::endl;
	//std::cout << PLAYER_STRING[(int)turn] << std::endl;
	if (turn == PLAYER_FIRST)
	{
		std::cout << "æŽè”Ô" << std::endl;
	}
	else
	{
		std::cout << "ŒãŽè”Ô" << std::endl;
	}
}