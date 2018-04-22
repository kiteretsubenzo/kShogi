#include <iostream>
#include <sstream>
#include <iomanip>
#include <unordered_map>
#include <set>
#include <vector>
#include <algorithm>
#include <cassert>
#include "definitions.h"
#include "board.h"

Board::Board()
{
	for( uchar i=0; i<PLAYER_MAX; i++ )
	{
		for( uchar j=0; j<(uchar)CAPTURE_MAX; j++ )
		{
			captured[i][j] = 0;
		}
	}
  
	for( uchar j=1; j<=BOARD_HEIGHT; j++ )
	{
		for( uchar i=1; i<=BOARD_WIDTH; i++ )
		{
			matrix[j][i] = { PLAYER_NONE, PAWN_NONE };
		}
	}

	for (uchar i = 0; i < BOARD_WIDTH+2; i++)
	{
		matrix[0][i] = { PLAYER_WALL, PAWN_NONE };
		matrix[BOARD_HEIGHT+1][i] = { PLAYER_WALL, PAWN_NONE };
	}

	for (uchar j = 1; j <= BOARD_HEIGHT; j++)
	{
		matrix[j][0] = { PLAYER_WALL, PAWN_NONE };
		matrix[j][BOARD_WIDTH+1] = { PLAYER_WALL, PAWN_NONE };
	}
	
	turn = PLAYER_FIRST;
	enemy = PLAYER_SECOND;
}

void Board::Init(const std::string &str)
{
	//'h', 'H', 'y', 'Y', 'e', 'E', 'g', 'G', 'k', 'u', 'U', 'r', 'R', 'o';

	std::vector<std::string> strs = split(str, '\n');
	
	for (int i = 0; i < PLAYER_MAX; i++)
	{
		gyokux[i] = -1;
		gyokuy[i] = -1;
	}

	for( int i=0; i<(uchar)CAPTURE_MAX; i++ )
	{
		char first[3] = { strs[BOARD_HEIGHT+1][i*4+1], strs[BOARD_HEIGHT+1][i*4+2], '\0' };
		char second[3] = { strs[0][i*4+1], strs[0][i*4+2], '\0' };
		captured[PLAYER_FIRST][i] = atoi(first);
		captured[PLAYER_SECOND][i] = atoi(second);
	}

	for( int j=2; j <= BOARD_HEIGHT+1; j++ )
	{
		for( int i=1; i <= BOARD_WIDTH; i++ )
		{
			char c[2] = { strs[j-1][(i-1)*2], strs[j-1][(i-1)*2+1] };
			if( c[0] == ' ' )
			{
				matrix[j-1][i].player = PLAYER_NONE;
				matrix[j-1][i].pawn = PAWN_NONE;
			}
			else
			{
				char type;
				if( c[0] == '^' )
				{
					matrix[j-1][i].player = PLAYER_FIRST;
					type = c[1];
				}
				else
				{
					matrix[j-1][i].player = PLAYER_SECOND;
					type = c[0];
				}

				matrix[j - 1][i].pawn = charToPawn[type];
				if (type == 'o')
				{
					gyokux[(int)(matrix[j - 1][i].player)] = i;
					gyokuy[(int)(matrix[j - 1][i].player)] = j - 1;
				}
			}
		}
	}

	if( strs[BOARD_HEIGHT+2] == "first" )
	{
		turn = PLAYER_FIRST;
		enemy = PLAYER_SECOND;
	}
	else if( strs[BOARD_HEIGHT+2] == "second" )
	{
		turn = PLAYER_SECOND;
		enemy = PLAYER_SECOND;
	}
}

std::string Board::BoardToString() const
{
	std::ostringstream sout;
	
	for( int i=0; i<(uchar)CAPTURE_MAX; i++ )
	{
		sout << PAWN_CHAR[i];
		sout << std::setfill('0') << std::setw(2) << (int)(captured[PLAYER_SECOND][i]);
		sout << ' ';
	}
	sout << '\n';
	
	for( int j=2; j <= BOARD_HEIGHT+1; j++ )
	{
		for( int i=1; i <= BOARD_WIDTH; i++ )
		{
			if( matrix[j-1][i].player == PLAYER_FIRST )
			{
				sout << '^' << PAWN_CHAR[matrix[j-1][i].pawn];
			}
			else if( matrix[j-1][i].player == PLAYER_SECOND )
			{
				sout << PAWN_CHAR[matrix[j-1][i].pawn] << '_';
			}
			else
			{
				sout << " .";
			}
		}
		sout << '\n';
	}
	
	for( int i=0; i<(uchar)CAPTURE_MAX; i++ )
	{
		sout << PAWN_CHAR[i];
		sout << std::setfill('0') << std::setw(2) << (int)captured[PLAYER_FIRST][i];
		sout << ' ';
	}
	sout << '\n';
	
	if( turn == PLAYER_FIRST )
	{
		sout << "first";
	}
	else
	{
		sout << "second";
	}
	
	return sout.str();
}

MoveList Board::GetMoveList()
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
	for (char j = 1; j<=BOARD_HEIGHT; j++)
	{
		for (char i = 1; i<=BOARD_WIDTH; i++)
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
				while(true)
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
				// 空きだったら打ち
				for (uchar roll = 0; roll < CAPTURE_MAX; roll++)
				{
					if (captured[(uchar)turn][roll] == 0)
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

#if USE_PRIORITY == PRIORITY_LIST
	moveList.sort();
#endif
	return moveList;
}

bool Board::AddMove(PAWN roll, uchar fromx, uchar fromy, uchar tox, uchar toy, bool upgrade, MoveList &moveList)
{
	if (matrix[toy][tox].player == turn || matrix[toy][tox].player == PLAYER_WALL)
	{
		return false;
	}

	PAWN_MOVE move( roll, fromx, fromy, tox, toy, matrix[fromy][fromx].pawn, matrix[toy][tox].pawn, upgrade, 0 );
	if (roll != PAWN_NONE)
	{
		move.from.x = 0;
		move.from.y = 0;
		move.from.pawn = PAWN_NONE;
	}

	Move(move);
	// 負ける手は指さない
	if (IsEnd() == false)
	{
		move.priority = GetPriority(move);
#if USE_PRIORITY == PRIORITY_MULTISET
		moveList.insert(move);
#else
		moveList.push_back(move);
#endif
	}
	Back(move);

	return matrix[toy][tox].pawn == PAWN_NONE;
}

int Board::GetEvaluate(const MoveList &moveList)
{
	return (int)(moveList.size());
}

int Board::GetPriority(const PAWN_MOVE &move)
{
	int priority = 0;
#if USE_PRIORITY == PRIORITY_LIST || USE_PRIORITY == PRIORITY_MULTISET
	// 王手がかかってるか？
	SwitchTurn();
	if (IsEnd())
	{
		priority += 1000;
	}
	SwitchTurn();
	// 駒を取るか？
	if (move.to.pawn != PAWN_NONE)
	{
		priority += (int)(move.to.pawn) + (int)PAWN_MAX;
	}
	// 成るか？
	if (move.upgrade)
	{
		priority += (int)(move.from.pawn);
	}
#endif
	return priority;
}

bool Board::IsEnd() const
{
	// 玉の位置を求める
	char gyokux = this->gyokux[(int)enemy];
	char gyokuy = this->gyokuy[(int)enemy];
	if (gyokux == -1)
	{
		return false;
	}
	CELL cell;
	if (turn == PLAYER_FIRST)
	{
		// 玉の周囲
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

		// 桂
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

		// 香
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
		// 玉の周囲
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
		if (GetCell( gyokux - 1, gyokuy - 1, cell))
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
		if (GetCell( gyokux - 1, gyokuy + 1, cell))
		{
			if (cell.pawn == PAWN_GYOKU || cell.pawn == PAWN_GIN || cell.pawn == PAWN_RYU)
			{
				return true;
			}
		}

		// 桂
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

		// 香
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
	// 飛龍
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

	// 角馬
	i = gyokux + 1;
	j = gyokuy + 1;
	while(true)
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

void Board::Move(const PAWN_MOVE &move)
{
	assert(move != PAWN_MOVE_ZERO);

	if( move.reserve != PAWN_NONE )
	{
		captured[(int)turn][(int)move.reserve]--;
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

	if( move.to.pawn != PAWN_NONE )
	{
		PAWN roll = move.to.pawn;
		captured[(int)turn][(int)roll]++;
	}
	if (move.from.pawn == PAWN_GYOKU)
	{
		gyokux[(int)turn] = move.to.x;
		gyokuy[(int)turn] = move.to.y;
	}

	SwitchTurn();
}

void Board::Back(const PAWN_MOVE &move)
{
	assert(move != PAWN_MOVE_ZERO);
	
	if( move.reserve != PAWN_NONE )
	{
		captured[(uchar)enemy][(int)move.reserve]++;

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
		captured[(uchar)enemy][move.to.pawn]--;

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
		gyokux[(int)enemy] = move.from.x;
		gyokuy[(int)enemy] = move.from.y;
	}

	SwitchTurn();
}

void Board::PrintBoard() const
{
	for( uchar i=0; i<(uchar)CAPTURE_MAX; i++)
	{
		std::cout << PAWN_KANJI[i] << ":" << (unsigned int)captured[PLAYER_SECOND][i] << " ";
	}
	std::cout << std::endl;
	std::cout << "９８７６５４３２１" << std::endl;
	for( uchar j=1; j<=BOARD_HEIGHT; j++ )
	{
		for( uchar i=1; i<=BOARD_WIDTH; i++ )
		{
			switch(matrix[j][i].player)
			{
			case PLAYER_FIRST:
				std::cout << "^" << PAWN_CHAR[matrix[j][i].pawn];
				break;
			case PLAYER_SECOND:
				std::cout << PAWN_CHAR[matrix[j][i].pawn]  << "_";
				break;
			case PLAYER_NONE:
				std::cout << " .";
				break;
			default:
				break;
			}
		}
		std::cout << " " << numberToKanji[j-1] << std::endl;
	}
	for( uchar i=0; i<(uchar)CAPTURE_MAX; i++)
	{
		std::cout << PAWN_KANJI[i] << ":" << (unsigned int)captured[PLAYER_FIRST][i] << " ";
	}
	std::cout << std::endl;
	std::cout << PLAYER_STRING[(int)turn] << std::endl;
}