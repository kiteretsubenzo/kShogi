#include <iostream>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <iterator>
#include <vector>
#include <map>
#include <unordered_map>
#include <set>
#include "definitions.h"
#include "board.h"

Board::Board()
{
  for( uchar i=0; i<(uchar)PLAYER::MAX; i++ )
  {
    for( uchar j=0; j<(uchar)CAPTURE_MAX; j++ )
    {
      captured[i][j] = 0;
    }
  }
  
  for( uchar j=0; j<BOARD_HEIGHT; j++ )
  {
    for( uchar i=0; i<BOARD_WIDTH; i++ )
    {
      matrix[j][i] = { PLAYER::NONE, PAWN_NONE };
    }
  }
	
	turn = PLAYER::FIRST;
}

void Board::Init(const std::string str)
{
	
	//'h', 'H', 'y', 'Y', 'e', 'E', 'g', 'G', 'k', 'u', 'U', 'r', 'R', 'o';

	std::vector<std::string> strs = split(str, '\n');
	
	for (int i = 0; i < (int)PLAYER::MAX; i++)
	{
		gyokux[i] = -1;
		gyokuy[i] = -1;
	}

	for( int i=0; i<(uchar)CAPTURE_MAX; i++ )
	{
		char first[3] = { strs[BOARD_HEIGHT+1][i*4+1], strs[BOARD_HEIGHT+1][i*4+2], '\0' };
		char second[3] = { strs[0][i*4+1], strs[0][i*4+2], '\0' };
		captured[(uchar)PLAYER::FIRST][i] = atoi(first);
		captured[(uchar)PLAYER::SECOND][i] = atoi(second);
	}

	for( int j=1; j < BOARD_HEIGHT+1; j++ )
	{
		for( int i=0; i<BOARD_WIDTH; i++ )
		{
			char c[2] = { strs[j][i*2], strs[j][i*2+1] };
			if( c[0] == ' ' )
			{
				matrix[j-1][i].player = PLAYER::NONE;
				matrix[j-1][i].pawn = PAWN_NONE;
			}
			else
			{
				char type;
				if( c[0] == '^' )
				{
					matrix[j-1][i].player = PLAYER::FIRST;
					type = c[1];
				}
				else
				{
					matrix[j-1][i].player = PLAYER::SECOND;
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
		turn = PLAYER::FIRST;
	}
	else if( strs[BOARD_HEIGHT+2] == "second" )
	{
		turn = PLAYER::SECOND;
	}
}

std::string Board::BoardToString() const
{
	std::ostringstream sout;
	
	for( int i=0; i<(uchar)CAPTURE_MAX; i++ )
	{
		sout << PAWN_CHAR[i];
		//std::cout << "::" << (int)captured[(uchar)PLAYER::SECOND][i] << std::endl;
		sout << std::setfill('0') << std::setw(2) << (int)(captured[(uchar)PLAYER::SECOND][i]);
		sout << ' ';
	}
	sout << '\n';
	
	for( int j=1; j < BOARD_HEIGHT+1; j++ )
	{
		for( int i=0; i<BOARD_WIDTH; i++ )
		{
			if( matrix[j-1][i].player == PLAYER::FIRST )
			{
				sout << '^' << PAWN_CHAR[matrix[j-1][i].pawn];
			}
			else if( matrix[j-1][i].player == PLAYER::SECOND )
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
		sout << std::setfill('0') << std::setw(2) << (int)captured[(uchar)PLAYER::FIRST][i];
		sout << ' ';
	}
	sout << '\n';
	
	if( turn == PLAYER::FIRST )
	{
		sout << "first";
	}
	else
	{
		sout << "second";
	}
	
	return sout.str();
}

#ifdef USE_PRIORITY_MULTISET
std::multiset<Board::PAWN_MOVE> Board::GetMoveList()
#else
std::list<Board::PAWN_MOVE> Board::GetMoveList()
#endif
{
#ifdef USE_PRIORITY_MULTISET
	std::multiset<Board::PAWN_MOVE> moveList;
#else
	std::list<Board::PAWN_MOVE> moveList;
#endif
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
				for (bool ret = true; 0 <= (int)y && (int)y < BOARD_HEIGHT && ret; y += forward)
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
				for (bool ret = true; x < BOARD_WIDTH && y < BOARD_HEIGHT && ret; x++, y++)
				{
					ret &= AddMove(PAWN_NONE, i, j, x, y, false, moveList);
					if (lineMin <= y && y <= lineMax)
					{
						ret &= AddMove(PAWN_NONE, i, j, x, y, true, moveList);
					}
				}
				x = i + 1; y = j - 1;
				for (bool ret = true; x < BOARD_WIDTH && 0 <= y && ret; x++, y -= 1)
				{
					ret &= AddMove(PAWN_NONE, i, j, x, y, false, moveList);
					if (lineMin <= y && y <= lineMax)
					{
						ret &= AddMove(PAWN_NONE, i, j, x, y, true, moveList);
					}
				}
				x = i - 1; y = j + 1;
				for (bool ret = true; 0 <= x && y < BOARD_HEIGHT && ret; x -= 1, y++)
				{
					ret &= AddMove(PAWN_NONE, i, j, x, y, false, moveList);
					if (lineMin <= y && y <= lineMax)
					{
						ret &= AddMove(PAWN_NONE, i, j, x, y, true, moveList);
					}
				}
				x = i - 1; y = j - 1;
				for (bool ret = true; 0 <= x && 0 <= y && ret; x -= 1, y -= 1)
				{
					ret &= AddMove(PAWN_NONE, i, j, x, y, false, moveList);
					if (lineMin <= y && y <= lineMax)
					{
						ret &= AddMove(PAWN_NONE, i, j, x, y, true, moveList);
					}
				}
				break;
			case PAWN_UMA:
				x = i + 1; y = j + 1;
				for (bool ret = true; x < BOARD_WIDTH && y < BOARD_HEIGHT && ret; x++, y++)
				{
					ret &= AddMove(PAWN_NONE, i, j, x, y, false, moveList);
				}
				x = i + 1; y = j - 1;
				for (bool ret = true; x < BOARD_WIDTH && 0 <= y && ret; x++, y -= 1)
				{
					ret &= AddMove(PAWN_NONE, i, j, x, y, false, moveList);
				}
				x = i - 1; y = j + 1;
				for (bool ret = true; 0 <= x && y < BOARD_HEIGHT && ret; x -= 1, y++)
				{
					ret &= AddMove(PAWN_NONE, i, j, x, y, false, moveList);
				}
				x = i - 1; y = j - 1;
				for (bool ret = true; 0 <= x && 0 <= y && ret; x -= 1, y -= 1)
				{
					ret &= AddMove(PAWN_NONE, i, j, x, y, false, moveList);
				}
				AddMove(PAWN_NONE, i, j, i + 1, j, false, moveList);
				AddMove(PAWN_NONE, i, j, i - 1, j, false, moveList);
				AddMove(PAWN_NONE, i, j, i, j + 1, false, moveList);
				AddMove(PAWN_NONE, i, j, i, j - 1, false, moveList);
				break;
			case PAWN_HI:
				x = i + 1; y = j;
				for (bool ret = true; x < BOARD_WIDTH && ret; x++)
				{
					ret &= AddMove(PAWN_NONE, i, j, x, y, false, moveList);
					if (lineMin <= y && y <= lineMax)
					{
						ret &= AddMove(PAWN_NONE, i, j, x, y, true, moveList);
					}
				}
				x = i - 1; y = j;
				for (bool ret = true; 0 <= x && ret; x -= 1)
				{
					ret &= AddMove(PAWN_NONE, i, j, x, y, false, moveList);
					if (lineMin <= y && y <= lineMax)
					{
						ret &= AddMove(PAWN_NONE, i, j, x, y, true, moveList);
					}
				}
				x = i; y = j + 1;
				for (bool ret = true; y < BOARD_HEIGHT && ret; y++)
				{
					ret &= AddMove(PAWN_NONE, i, j, x, y, false, moveList);
					if (lineMin <= y && y <= lineMax)
					{
						ret &= AddMove(PAWN_NONE, i, j, x, y, true, moveList);
					}
				}
				x = i; y = j - 1;
				for (bool ret = true; 0 <= y && ret; y -= 1)
				{
					ret &= AddMove(PAWN_NONE, i, j, x, y, false, moveList);
					if (lineMin <= y && y <= lineMax)
					{
						ret &= AddMove(PAWN_NONE, i, j, x, y, true, moveList);
					}
				}
				break;
			case PAWN_RYU:
				x = i + 1; y = j;
				for (bool ret = true; x < BOARD_WIDTH && ret; x++)
				{
					ret &= AddMove(PAWN_NONE, i, j, x, y, false, moveList);
				}
				x = i - 1; y = j;
				for (bool ret = true; 0 <= x && ret; x -= 1)
				{
					ret &= AddMove(PAWN_NONE, i, j, x, y, false, moveList);
				}
				x = i; y = j + 1;
				for (bool ret = true; y < BOARD_HEIGHT && ret; y++)
				{
					ret &= AddMove(PAWN_NONE, i, j, x, y, false, moveList);
				}
				x = i; y = j - 1;
				for (bool ret = true; 0 <= y && ret; y -= 1)
				{
					ret &= AddMove(PAWN_NONE, i, j, x, y, false, moveList);
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
				break;
			}
		}
	}

	PAWN_MOVE move = PAWN_MOVE_ZERO;

	for (uchar roll = 0; roll<(uchar)CAPTURE_MAX; roll++)
	{
		if (captured[(uchar)turn][roll] == 0)
		{
			continue;
		}
		move.reserve = roll;
		for (uchar j = 0; j<BOARD_HEIGHT; j++)
		{
			for (uchar i = 0; i<BOARD_WIDTH; i++)
			{
				if (matrix[j][i].player != PLAYER::NONE || matrix[j][i].pawn != PAWN_NONE)
				{
					continue;
				}

				switch (roll)
				{
				case PAWN_HU:
					if (j != lineTop)
					{
						uchar k;
						for (k = 0; k<BOARD_HEIGHT; k++)
						{
							if (matrix[k][i].player == turn && matrix[k][i].pawn == PAWN_HU)
							{
								break;
							}
						}
						if (BOARD_HEIGHT <= k)
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
			}
		}
	}

#ifdef USE_PRIORITY_LIST
	moveList.sort();
#endif
	return moveList;
}

#ifdef USE_PRIORITY_MULTISET
bool Board::AddMove(PAWN roll, uchar fromx, uchar fromy, char tox, char toy, bool upgrade, std::multiset<Board::PAWN_MOVE> &moveList)
#else
bool Board::AddMove(PAWN roll, uchar fromx, uchar fromy, char tox, char toy, bool upgrade, std::list<Board::PAWN_MOVE> &moveList)
#endif
{
	if (tox < 0 || BOARD_WIDTH <= tox)
	{
		return false;
	}
	if (toy < 0 || BOARD_HEIGHT <= toy)
	{
		return false;
	}
	uchar utox = (uchar)tox;
	uchar utoy = (uchar)toy;
	if (matrix[utoy][utox].player == turn)
	{
		return false;
	}
	PAWN capture = matrix[utoy][utox].pawn;

	PAWN_MOVE move( roll, fromx, fromy, utox, utoy, matrix[fromy][fromx].pawn, matrix[utoy][utox].pawn, upgrade, 0 );

	Move(move);
	// 負ける手は指さない
	if (IsEnd() == false)
	{
		move.priority = GetPriority(move);
#ifdef USE_PRIORITY_MULTISET
		moveList.insert(move);
#else
		moveList.push_back(move);
#endif
	}
	Back(move);

	return capture == PAWN_NONE;
}

#ifdef USE_PRIORITY_MULTISET
int Board::GetEvaluate(const std::multiset<Board::PAWN_MOVE> &moveList)
{
	return moveList.size();
}
#else
int Board::GetEvaluate(const std::list<Board::PAWN_MOVE> &moveList)
{
	return moveList.size();
}
#endif

int Board::GetPriority(const Board::PAWN_MOVE &move)
{
	int priority = 0;
#ifdef USE_PRIORITY_MULTISET
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
#ifdef USE_PRIORITY_LIST
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
	PLAYER enemy;
	if( turn == PLAYER::FIRST )
	{
		enemy = PLAYER::SECOND;
	}
	else
	{
		enemy = PLAYER::FIRST;
	}

	// 玉の位置を求める
	char gyokux = this->gyokux[(int)enemy];
	char gyokuy = this->gyokuy[(int)enemy];
	if (gyokux == -1)
	{
		return false;
	}
	CELL cell;
	if (turn == PLAYER::FIRST)
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

		// 香
		for (char j = gyokuy + 1; j < BOARD_HEIGHT; j++)
		{
			if (matrix[j][gyokux].player == PLAYER::NONE)
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

		// 香
		for (char j = gyokuy - 1; j >= 0; j--)
		{
			if (matrix[j][gyokux].player == PLAYER::NONE)
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

	// 飛龍
	for (char i = gyokux + 1; i < BOARD_WIDTH; i++)
	{
		if (matrix[gyokuy][i].player == PLAYER::NONE)
		{
			continue;
		}
		if (matrix[gyokuy][i].player == enemy)
		{
			break;
		}
		if (matrix[gyokuy][i].pawn == PAWN_HI || matrix[gyokuy][i].pawn == PAWN_RYU)
		{
			return true;
		}
		break;
	}
	for (char i = gyokux - 1; i >= 0; i--)
	{
		if (matrix[gyokuy][i].player == PLAYER::NONE)
		{
			continue;
		}
		if (matrix[gyokuy][i].player == enemy)
		{
			break;
		}
		if (matrix[gyokuy][i].pawn == PAWN_HI || matrix[gyokuy][i].pawn == PAWN_RYU)
		{
			return true;
		}
		break;
	}
	for (char j = gyokuy + 1; j < BOARD_HEIGHT; j++)
	{
		if (matrix[j][gyokux].player == PLAYER::NONE)
		{
			continue;
		}
		if (matrix[j][gyokux].player == enemy)
		{
			break;
		}
		if (matrix[j][gyokux].pawn == PAWN_HI || matrix[j][gyokux].pawn == PAWN_RYU)
		{
			return true;
		}
		break;
	}
	for (char j = gyokuy - 1; j >= 0; j--)
	{
		if (matrix[j][gyokux].player == PLAYER::NONE)
		{
			continue;
		}
		if (matrix[j][gyokux].player == enemy)
		{
			break;
		}
		if (matrix[j][gyokux].pawn == PAWN_HI || matrix[j][gyokux].pawn == PAWN_RYU)
		{
			return true;
		}
		break;
	}

	// 角馬
	for (char i = gyokux + 1, j = gyokuy + 1; i < BOARD_WIDTH && j < BOARD_HEIGHT; i++, j++)
	{
		if (matrix[j][i].player == PLAYER::NONE)
		{
			continue;
		}
		if (matrix[j][i].player == enemy)
		{
			break;
		}
		if (matrix[j][i].pawn == PAWN_KAKU || matrix[j][i].pawn == PAWN_UMA)
		{
			return true;
		}
		break;
	}
	for (char i = gyokux - 1, j = gyokuy + 1; i >= 0 && j < BOARD_HEIGHT; i--, j++)
	{
		if (matrix[j][i].player == PLAYER::NONE)
		{
			continue;
		}
		if (matrix[j][i].player == enemy)
		{
			break;
		}
		if (matrix[j][i].pawn == PAWN_KAKU || matrix[j][i].pawn == PAWN_UMA)
		{
			return true;
		}
		break;
	}
	for (char i = gyokux - 1, j = gyokuy - 1; i >= 0 && j >= 0; i--, j--)
	{
		if (matrix[j][i].player == PLAYER::NONE)
		{
			continue;
		}
		if (matrix[j][i].player == enemy)
		{
			break;
		}
		if (matrix[j][i].pawn == PAWN_KAKU || matrix[j][i].pawn == PAWN_UMA)
		{
			return true;
		}
		break;
	}
	for (char i = gyokux + 1, j = gyokuy - 1; i < BOARD_WIDTH && j >= 0; i++, j--)
	{
		if (matrix[j][i].player == PLAYER::NONE)
		{
			continue;
		}
		if (matrix[j][i].player == enemy)
		{
			break;
		}
		if (matrix[j][i].pawn == PAWN_KAKU || matrix[j][i].pawn == PAWN_UMA)
		{
			return true;
		}
		break;
	}

	return false;
}

bool Board::GetCell(char tox, char toy, CELL &cell) const
{
	if (tox < 0 || BOARD_WIDTH <= tox)
	{
		return false;
	}
	if (toy < 0 || BOARD_HEIGHT <= toy)
	{
		return false;
	}
	if (matrix[toy][tox].player != turn)
	{
		return false;
	}

	cell = matrix[toy][tox];

	return true;
}

void Board::Move(const PAWN_MOVE &move)
{
	PLAYER nextTurn;
	if( turn == PLAYER::FIRST )
	{
		nextTurn = PLAYER::SECOND;
	}
	else
	{
		nextTurn = PLAYER::FIRST;
	}
	
	if( move.reserve != PAWN_NONE )
	{
		captured[(int)turn][(int)move.reserve]--;
		matrix[move.to.y][move.to.x].player = turn;
		matrix[move.to.y][move.to.x].pawn = move.reserve;

		turn = nextTurn;
		return;
	}
	
	PAWN pawn = move.from.pawn;
	if (move.upgrade)
	{
		Upgrade(pawn);
	}
	matrix[move.to.y][move.to.x].player = turn;
	matrix[move.to.y][move.to.x].pawn = pawn;
	matrix[move.from.y][move.from.x].player = PLAYER::NONE;
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

	turn = nextTurn;
}

void Board::Back(const PAWN_MOVE &move)
{
	PLAYER prevTurn;
	if( turn == PLAYER::FIRST )
	{
		prevTurn = PLAYER::SECOND;
	}
	else
	{
		prevTurn = PLAYER::FIRST;
	}
	
	if( move.reserve != PAWN_NONE )
	{
		captured[(uchar)prevTurn][(int)move.reserve]++;

		matrix[move.to.y][move.to.x].player = PLAYER::NONE;
		matrix[move.to.y][move.to.x].pawn = PAWN_NONE;

		turn = prevTurn;
		return;
	}
	
	PAWN pawn = move.from.pawn;
	if (move.upgrade)
	{
		Downgrade(pawn);
	}
	matrix[move.from.y][move.from.x].player = prevTurn;
	matrix[move.from.y][move.from.x].pawn = pawn;

	if (move.to.pawn != PAWN_NONE)
	{
		captured[(uchar)prevTurn][move.to.pawn]--;

		matrix[move.to.y][move.to.x].player = turn;
		matrix[move.to.y][move.to.x].pawn = move.to.pawn;
	}
	else
	{
		matrix[move.to.y][move.to.x].player = PLAYER::NONE;
		matrix[move.to.y][move.to.x].pawn = PAWN_NONE;
	}

	if (move.from.pawn == PAWN_GYOKU)
	{
		gyokux[(int)prevTurn] = move.from.x;
		gyokuy[(int)prevTurn] = move.from.y;
	}

	turn = prevTurn;
}

void Board::SwitchTurn()
{
	if (turn == PLAYER::FIRST)
	{
		turn = PLAYER::SECOND;
	}
	else
	{
		turn = PLAYER::FIRST;
	}
}

void Board::PrintBoard() const
{
	for( uchar i=0; i<(uchar)CAPTURE_MAX; i++)
	{
		std::cout << PAWN_KANJI[i] << ":" << (unsigned int)captured[(uchar)PLAYER::SECOND][i] << " ";
	}
	std::cout << std::endl;
	std::cout << "９８７６５４３２１" << std::endl;
	for( uchar j=0; j<BOARD_HEIGHT; j++ )
	{
		for( uchar i=0; i<BOARD_WIDTH; i++ )
		{
			switch(matrix[j][i].player)
			{
			case PLAYER::FIRST:
				std::cout << "^" << PAWN_CHAR[matrix[j][i].pawn];
				break;
			case PLAYER::SECOND:
				std::cout << PAWN_CHAR[matrix[j][i].pawn]  << "_";
				break;
			case PLAYER::NONE:
				std::cout << " .";
				break;
			default:
				break;
			}
		}
		std::cout << " " << numberToKanji[j] << std::endl;
	}
	for( uchar i=0; i<(uchar)CAPTURE_MAX; i++)
	{
		std::cout << PAWN_KANJI[i] << ":" << (unsigned int)captured[(uchar)PLAYER::FIRST][i] << " ";
	}
	std::cout << std::endl;
	std::cout << PLAYER_STRING[(uchar)turn] << std::endl;
}