#include <iostream>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <iterator>
#include <vector>
#include <map>
#include <unordered_map>
#include "definitions.h"
#include "board.h"

Board::Board()
{
  for( uchar i=0; i<(uchar)PLAYER::MAX; i++ )
  {
    for( uchar j=0; j<(uchar)PAWN_ROLL::CAPTURE_MAX; j++ )
    {
      captured[i][j] = 0;
    }
  }
  
  for( uchar j=0; j<BOARD_HEIGHT; j++ )
  {
    for( uchar i=0; i<BOARD_WIDTH; i++ )
    {
      matrix[j][i] = { PLAYER::NONE, PAWN_TYPE::NONE };
    }
  }
	
	turn = PLAYER::FIRST;
}

void Board::Init(const std::string str)
{
	std::vector<std::string> strs = split(str, '\n');
	
	for( int i=0; i<(uchar)PAWN_ROLL::CAPTURE_MAX; i++ )
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
				matrix[j-1][i].pawn = PAWN_TYPE::NONE;
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
				//'h', 'H', 'y', 'Y', 'e', 'E', 'g', 'G', 'k', 'u', 'U', 'r', 'R', 'o';
				switch( type )
				{
					case 'h':
						matrix[j-1][i].pawn = PAWN_TYPE::HU;
						break;
					case 'H':
						matrix[j-1][i].pawn = PAWN_TYPE::HUN;
						break;
					case 'y':
						matrix[j-1][i].pawn = PAWN_TYPE::KYOH;
						break;
					case 'Y':
						matrix[j-1][i].pawn = PAWN_TYPE::KYOHN;
						break;
					case 'e':
						matrix[j-1][i].pawn = PAWN_TYPE::KEI;
						break;
					case 'E':
						matrix[j-1][i].pawn = PAWN_TYPE::KEIN;
						break;
					case 'g':
						matrix[j-1][i].pawn = PAWN_TYPE::GIN;
						break;
					case 'G':
						matrix[j-1][i].pawn = PAWN_TYPE::GINN;
						break;
					case 'k':
						matrix[j-1][i].pawn = PAWN_TYPE::KIN;
						break;
					case 'u':
						matrix[j-1][i].pawn = PAWN_TYPE::KAKU;
						break;
					case 'U':
						matrix[j-1][i].pawn = PAWN_TYPE::UMA;
						break;
					case 'r':
						matrix[j-1][i].pawn = PAWN_TYPE::HI;
						break;
					case 'R':
						matrix[j-1][i].pawn = PAWN_TYPE::RYU;
						break;
					case 'o':
						matrix[j-1][i].pawn = PAWN_TYPE::GYOKU;
						break;
					default:
						break;
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
	
	for( int i=0; i<(uchar)PAWN_ROLL::CAPTURE_MAX; i++ )
	{
		sout << (char)(PAWN_ROLL)i;
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
				sout << '^' << (char)(matrix[j-1][i].pawn);
			}
			else if( matrix[j-1][i].player == PLAYER::SECOND )
			{
				sout << (char)(matrix[j-1][i].pawn) << '_';
			}
			else
			{
				sout << " .";
			}
		}
		sout << '\n';
	}
	
	for( int i=0; i<(uchar)PAWN_ROLL::CAPTURE_MAX; i++ )
	{
		sout << (char)(PAWN_ROLL)i;
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

bool Board::AddMove(PAWN_ROLL roll, uchar fromx, uchar fromy, char tox, char toy, bool upgrade, std::list<Board::PAWN_MOVE> &moveList)
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
	PAWN_TYPE capture = matrix[utoy][utox].pawn;
	PAWN_MOVE move{ roll, fromx, fromy, utox, utoy, matrix[fromy][fromx].pawn, matrix[utoy][utox].pawn, upgrade };

	Move(move);
	// 負ける手は指さない
	if (IsEnd() == false)
	{
		moveList.push_back(move);
	}
	Back(move);

	return capture == PAWN_TYPE::NONE;
}

bool Board::AddMove(PAWN_ROLL roll, uchar fromx, uchar fromy, char tox, char toy, bool upgrade, std::vector<Board::PAWN_MOVE> &moveList)
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
	PAWN_TYPE capture = matrix[utoy][utox].pawn;
	PAWN_MOVE move{ roll, fromx, fromy, utox, utoy, matrix[fromy][fromx].pawn, matrix[utoy][utox].pawn, upgrade };

	Move(move);
	// 負ける手は指さない
	if (IsEnd() == false)
	{
		moveList.push_back(move);
	}
	Back(move);

	return capture == PAWN_TYPE::NONE;
}

bool Board::AddMove( PAWN_ROLL roll, uchar fromx, uchar fromy, char tox, char toy, bool upgrade, std::multimap<int, PAWN_MOVE> &moveMap )
{
	if( tox < 0 || BOARD_WIDTH <= tox )
	{
		return false;
	}
	if( toy < 0 || BOARD_HEIGHT <= toy )
	{
		return false;
	}
	uchar utox = (uchar)tox;
	uchar utoy = (uchar)toy;
	if( matrix[utoy][utox].player == turn)
	{
		return false;
	}
	PAWN_TYPE capture = matrix[utoy][utox].pawn;
	PAWN_MOVE move{ roll, fromx, fromy, utox, utoy, matrix[fromy][fromx].pawn, matrix[utoy][utox].pawn, upgrade };
	
	Move(move);
	// 負ける手は指さない
	if( IsEnd() == false )
	{
		int priority = 0;
		// 王手がかかってるか？
		SwitchTurn();
		if (IsEnd())
		{
			priority += 1000;
		}
		SwitchTurn();
		// 駒を取るか？
		if (move.toPawn != PAWN_TYPE::NONE)
		{
			priority += (int)(move.toPawn) + (int)PAWN_TYPE::MAX;
		}
		// 成るか？
		if (move.upgrade)
		{
			priority += (int)(move.fromPawn);
		}
		moveMap.insert(std::make_pair(-priority, move));
	}
	Back(move);
	
	return capture == PAWN_TYPE::NONE;
}

bool Board::IsEnd()
{
	bool isCapture = false;
	PLAYER enemy;
	if( turn == PLAYER::FIRST )
	{
		enemy = PLAYER::SECOND;
	}
	else
	{
		enemy = PLAYER::FIRST;
	}
	int forward = -1;
	if( turn == PLAYER::SECOND )
	{
		forward = +1;
	}
	for( char j=0; j<BOARD_HEIGHT; j++ )
  {
    for( char i=0; i<BOARD_WIDTH; i++ )
    {
			CELL cell = matrix[(uchar)j][(uchar)i];
			if(cell.player != turn)
			{
				continue;
			}
			PAWN_TYPE pawn = cell.pawn;
			int x, y;
			switch( (PAWN_TYPE::TYPE)pawn )
			{
				case PAWN_TYPE::HU:
					x = i;
					y = j+forward;
					IsCapture( x, y, enemy, isCapture );
					break;
				case PAWN_TYPE::KYOH:
					y=j+forward;
					for( bool ret = true; 0 <= (int)y && (int)y < BOARD_HEIGHT && ret; y += forward )
					{
						ret &= IsCapture( i, y, enemy, isCapture );
					}
					break;
				case PAWN_TYPE::KEI:
					x = i-1;
					y = j-forward-forward;
					IsCapture( x, y, enemy, isCapture );
					break;
				case PAWN_TYPE::GIN:
					IsCapture( i-1, j+forward, enemy, isCapture );
					IsCapture( i, j+forward, enemy, isCapture );
					IsCapture( i+1, j+forward, enemy, isCapture );
					IsCapture( i-1, j-forward, enemy, isCapture );
					IsCapture( i+1, j-forward, enemy, isCapture );
					break;
				case PAWN_TYPE::KIN:
				case PAWN_TYPE::HUN:
				case PAWN_TYPE::KYOHN:
				case PAWN_TYPE::KEIN:
				case PAWN_TYPE::GINN:
					IsCapture( i-1, j+forward, enemy, isCapture );
					IsCapture( i, j+forward, enemy, isCapture );
					IsCapture( i+1, j+forward, enemy, isCapture );
					IsCapture( i-1, j, enemy, isCapture );
					IsCapture( i+1, j, enemy, isCapture );
					IsCapture( i, j-forward, enemy, isCapture );
					break;
				case PAWN_TYPE::KAKU:
					x = i+1; y = j+1;
					for( bool ret = true; x < BOARD_WIDTH && y < BOARD_HEIGHT && ret; x++, y++ )
					{
						ret &= IsCapture( x, y, enemy, isCapture );
					}
					x = i+1; y = j-1;
					for( bool ret = true; x < BOARD_WIDTH && 0 <= y && ret; x++, y -= 1 )
					{
						ret &= IsCapture( x, y, enemy, isCapture );
					}
					x = i-1; y = j+1;
					for( bool ret = true; 0 <= x && y < BOARD_HEIGHT && ret; x -= 1, y++ )
					{
						ret &= IsCapture( x, y, enemy, isCapture );
					}
					x = i-1; y = j-1;
					for( bool ret = true; 0 <= x && 0 <= y && ret; x -= 1, y -= 1 )
					{
						ret &= IsCapture( x, y, enemy, isCapture );
					}
					break;
				case PAWN_TYPE::UMA:
					x = i+1; y = j+1;
					for( bool ret = true; x < BOARD_WIDTH && y < BOARD_HEIGHT && ret; x++, y++ )
					{
						ret &= IsCapture( x, y, enemy, isCapture );
					}
					x = i+1; y = j-1;
					for( bool ret = true; x < BOARD_WIDTH && 0 <= y && ret; x++, y -= 1 )
					{
						ret &= IsCapture( x, y, enemy, isCapture );
					}
					x = i-1; y = j+1;
					for( bool ret = true; 0 <= x && y < BOARD_HEIGHT && ret; x -= 1, y++ )
					{
						ret &= IsCapture( x, y, enemy, isCapture );
					}
					x = i-1; y = j-1;
					for( bool ret = true; 0 <= x && 0 <= y && ret; x -= 1, y -= 1 )
					{
						ret &= IsCapture( x, y, enemy, isCapture );
					}
					IsCapture( i+1, j, enemy, isCapture );
					IsCapture( i-1, j, enemy, isCapture );
					IsCapture( i, j+1, enemy, isCapture );
					IsCapture( i, j-1, enemy, isCapture );
					break;
				case PAWN_TYPE::HI:
					x = i+1; y = j;
					for( bool ret = true; x < BOARD_WIDTH && ret; x++ )
					{
						ret &= IsCapture( x, y, enemy, isCapture );
					}
					x = i-1; y = j;
					for( bool ret = true; 0 <= x && ret; x -= 1 )
					{
						ret &= IsCapture( x, y, enemy, isCapture );
					}
					x = i; y = j+1;
					for( bool ret = true; y < BOARD_HEIGHT && ret; y++ )
					{
						ret &= IsCapture( x, y, enemy, isCapture );
					}
					x = i; y = j-1;
					for( bool ret = true; 0 <= y && ret; y -= 1 )
					{
						ret &= IsCapture( x, y, enemy, isCapture );
					}
					break;
				case PAWN_TYPE::RYU:
					x = i+1; y = j;
					for( bool ret = true; x < BOARD_WIDTH && ret; x++ )
					{
						ret &= IsCapture( x, y, enemy, isCapture );
					}
					x = i-1; y = j;
					for( bool ret = true; 0 <= x && ret; x -= 1 )
					{
						ret &= IsCapture( x, y, enemy, isCapture );
					}
					x = i; y = j+1;
					for( bool ret = true; y < BOARD_HEIGHT && ret; y++ )
					{
						ret &= IsCapture( x, y, enemy, isCapture );
					}
					x = i; y = j-1;
					for( bool ret = true; 0 <= y && ret; y -= 1 )
					{
						ret &= IsCapture( x, y, enemy, isCapture );
					}
					IsCapture( i+1, j+1, enemy, isCapture );
					IsCapture( i-1, j+1, enemy, isCapture );
					IsCapture( i+1, j-1, enemy, isCapture );
					IsCapture( i-1, j-1, enemy, isCapture );
					break;
				case PAWN_TYPE::GYOKU:
					IsCapture( i-1, j-1, enemy, isCapture );
					IsCapture( i, j-1, enemy, isCapture );
					IsCapture( i+1, j-1, enemy, isCapture );
					IsCapture( i-1, j, enemy, isCapture );
					IsCapture( i+1, j, enemy, isCapture );
					IsCapture( i-1, j+1, enemy, isCapture );
					IsCapture( i, j+1, enemy, isCapture );
					IsCapture( i+1, j+1, enemy, isCapture );
					break;
				default:
					break;
			}
			if( isCapture )
			{
				return true;
			}
    }
  }
	return isCapture;
}

bool Board::IsCapture( char tox, char toy, PLAYER enemy, bool &isCapture )
{
	if( tox < 0 || BOARD_WIDTH <= tox )
	{
		return false;
	}
	if( toy < 0 || BOARD_HEIGHT <= toy )
	{
		return false;
	}
	
	uchar utox = (uchar)tox;
	uchar utoy = (uchar)toy;
	//std::cout << (int)tox << "," << (int)toy << "," << PAWN_CHAR[(int)matrix[utoy][utox].pawn] << std::endl; 
	if( matrix[utoy][utox].player == enemy && matrix[utoy][utox].pawn == PAWN_TYPE::GYOKU )
	{
		isCapture |= true;
		return false;
	}

	return matrix[utoy][utox].pawn == PAWN_TYPE::NONE;
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
	
	if( move.reserve != PAWN_ROLL::NONE )
	{
		captured[(int)turn][(int)move.reserve]--;
		matrix[move.toy][move.tox].player = turn;
		matrix[move.toy][move.tox].pawn = (PAWN_TYPE)move.reserve;
		turn = nextTurn;
		return;
	}
	
	PAWN_TYPE pawn = matrix[move.fromy][move.fromx].pawn;
	if( move.upgrade )
	{
		pawn.Upgrade();
	}
	matrix[move.toy][move.tox].player = turn;
	matrix[move.toy][move.tox].pawn = pawn;
	matrix[move.fromy][move.fromx].player = PLAYER::NONE;
	matrix[move.fromy][move.fromx].pawn = PAWN_TYPE::NONE;
	if( move.toPawn != PAWN_TYPE::NONE )
	{
		PAWN_ROLL roll = (PAWN_ROLL)(move.toPawn);
		captured[(int)turn][(int)roll]++;
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
	
	if( move.reserve != PAWN_ROLL::NONE )
	{
		captured[(uchar)prevTurn][(int)move.reserve]++;
		matrix[move.toy][move.tox].player = PLAYER::NONE;
		matrix[move.toy][move.tox].pawn = PAWN_TYPE::NONE;
		turn = prevTurn;
		return;
	}
	
	PAWN_TYPE pawn = matrix[move.toy][move.tox].pawn;
	if( move.upgrade )
	{
		pawn.Downgrade();
	}
	matrix[move.fromy][move.fromx].player = prevTurn;
	matrix[move.fromy][move.fromx].pawn = pawn;
	if( move.toPawn != PAWN_TYPE::NONE )
	{
		captured[(uchar)prevTurn][(int)((PAWN_ROLL)move.toPawn)]--;
		matrix[move.toy][move.tox].player = turn;
		matrix[move.toy][move.tox].pawn = move.toPawn;
	}
	else
	{
		matrix[move.toy][move.tox].player = PLAYER::NONE;
		matrix[move.toy][move.tox].pawn = PAWN_TYPE::NONE;
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
  for( uchar i=0; i<(uchar)PAWN_ROLL::CAPTURE_MAX; i++)
  {
    std::cout << (std::string)(PAWN_ROLL)i << ":" << (unsigned int)captured[(uchar)PLAYER::SECOND][i] << " ";
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
          std::cout << "^" << (char)(matrix[j][i].pawn);
          break;
        case PLAYER::SECOND:
          std::cout << (char)(matrix[j][i].pawn)  << "_";
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
	for( uchar i=0; i<(uchar)PAWN_ROLL::CAPTURE_MAX; i++)
  {
    std::cout << (std::string)(PAWN_ROLL)i << ":" << (unsigned int)captured[(uchar)PLAYER::FIRST][i] << " ";
  }
	std::cout << std::endl;
	std::cout << PLAYER_STRING[(uchar)turn] << std::endl;
}