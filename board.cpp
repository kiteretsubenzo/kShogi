#include <iostream>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <vector>
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
	std::vector<std::string> strs;
	
	std::stringstream ss{str};
	std::string buf;
	while (std::getline(ss, buf)) {
		strs.push_back(buf);
	}
	
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
		sout << ROLL_CHAR[i];
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
				sout << '^' << PAWN_CHAR[(uchar)matrix[j-1][i].pawn];
			}
			else if( matrix[j-1][i].player == PLAYER::SECOND )
			{
				sout << PAWN_CHAR[(uchar)matrix[j-1][i].pawn] << '_';
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
		sout << ROLL_CHAR[i];
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

void Board::GetMoveList(std::vector<PAWN_MOVE> &moveList)
{
	moveList.clear();
	uchar lineMax = 0;
	uchar lineMid = 1;
	uchar lineMin = 2;
	if(turn == PLAYER::SECOND)
	{
		lineMax = BOARD_HEIGHT-1;
		lineMid = BOARD_HEIGHT-2;
		lineMin = BOARD_HEIGHT-3;
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
			switch( pawn )
			{
				case PAWN_TYPE::HU:
					x = i;
					y = j+forward;
					if( y != lineMax )
					{
						AddMove( i, j, x, y, false, moveList );
					}
					if( lineMin <= y && y <= lineMax )
					{
						AddMove( i, j, x, y, true, moveList );
					}
					break;
				case PAWN_TYPE::KYOH:
					y=j+forward;
					for( bool ret = true; 0 <= (int)y && (int)y < BOARD_HEIGHT && ret; y += forward )
					{
						if( y != lineMax )
						{
							ret &= AddMove( i, j, i, y, false, moveList );
						}
						if( lineMin <= y && y <= lineMax )
						{
							ret &= AddMove( i, j, i, y, true, moveList );
						}
					}
					break;
				case PAWN_TYPE::KEI:
					x = i-1;
					y = j-forward-forward;
					if( y != lineMax && y != lineMid )
					{
						AddMove( i, j, x, y, false, moveList );
					}
					if( lineMin <= y && y <= lineMax )
					{
						AddMove( i, j, x, y, true, moveList );
					}
					break;
				case PAWN_TYPE::GIN:
					AddMove( i, j, i-1, j+forward, false, moveList );
					AddMove( i, j, i, j+forward, false, moveList );
					AddMove( i, j, i+1, j+forward, false, moveList );
					AddMove( i, j, i-1, j-forward, false, moveList );
					AddMove( i, j, i+1, j-forward, false, moveList );
					if( ( lineMin <= (j+forward) && (j+forward) <= lineMax ) || (lineMin <= j && j <= lineMax) )
					{
						AddMove( i, j, i-1, j+forward, true, moveList );
						AddMove( i, j, i, j+forward, true, moveList );
						AddMove( i, j, i+1, j+forward, true, moveList );
						AddMove( i, j, i-1, j-forward, true, moveList );
						AddMove( i, j, i+1, j-forward, true, moveList );
					}
					break;
				case PAWN_TYPE::KIN:
				case PAWN_TYPE::HUN:
				case PAWN_TYPE::KYOHN:
				case PAWN_TYPE::KEIN:
				case PAWN_TYPE::GINN:
					AddMove( i, j, i-1, j+forward, false, moveList );
					AddMove( i, j, i, j+forward, false, moveList );
					AddMove( i, j, i+1, j+forward, false, moveList );
					AddMove( i, j, i-1, j, false, moveList );
					AddMove( i, j, i+1, j, false, moveList );
					AddMove( i, j, i, j-forward, false, moveList );
					break;
				case PAWN_TYPE::KAKU:
					x = i+1; y = j+1;
					for( bool ret = true; x < BOARD_WIDTH && y < BOARD_HEIGHT && ret; x++, y++ )
					{
						ret &= AddMove( i, j, x, y, false, moveList );
						if( lineMin <= y && y <= lineMax )
						{
							ret &= AddMove( i, j, x, y, true, moveList );
						}
					}
					x = i+1; y = j-1;
					for( bool ret = true; x < BOARD_WIDTH && 0 <= y && ret; x++, y -= 1 )
					{
						ret &= AddMove( i, j, x, y, false, moveList );
						if( lineMin <= y && y <= lineMax )
						{
							ret &= AddMove( i, j, x, y, true, moveList );
						}
					}
					x = i-1; y = j+1;
					for( bool ret = true; 0 <= x && y < BOARD_HEIGHT && ret; x -= 1, y++ )
					{
						ret &= AddMove( i, j, x, y, false, moveList );
						if( lineMin <= y && y <= lineMax )
						{
							ret &= AddMove( i, j, x, y, true, moveList );
						}
					}
					x = i-1; y = j-1;
					for( bool ret = true; 0 <= x && 0 <= y && ret; x -= 1, y -= 1 )
					{
						ret &= AddMove( i, j, x, y, false, moveList );
						if( lineMin <= y && y <= lineMax )
						{
							ret &= AddMove( i, j, x, y, true, moveList );
						}
					}
					break;
				case PAWN_TYPE::UMA:
					x = i+1; y = j+1;
					for( bool ret = true; x < BOARD_WIDTH && y < BOARD_HEIGHT && ret; x++, y++ )
					{
						ret &= AddMove( i, j, x, y, false, moveList );
					}
					x = i+1; y = j-1;
					for( bool ret = true; x < BOARD_WIDTH && 0 <= y && ret; x++, y -= 1 )
					{
						ret &= AddMove( i, j, x, y, false, moveList );
					}
					x = i-1; y = j+1;
					for( bool ret = true; 0 <= x && y < BOARD_HEIGHT && ret; x -= 1, y++ )
					{
						ret &= AddMove( i, j, x, y, false, moveList );
					}
					x = i-1; y = j-1;
					for( bool ret = true; 0 <= x && 0 <= y && ret; x -= 1, y -= 1 )
					{
						ret &= AddMove( i, j, x, y, false, moveList );
					}
					AddMove( i, j, i+1, j, false, moveList );
					AddMove( i, j, i-1, j, false, moveList );
					AddMove( i, j, i, j+1, false, moveList );
					AddMove( i, j, i, j-1, false, moveList );
					break;
				case PAWN_TYPE::HI:
					x = i+1; y = j;
					for( bool ret = true; x < BOARD_WIDTH && ret; x++ )
					{
						ret &= AddMove( i, j, x, y, false, moveList );
						if( lineMin <= y && y <= lineMax )
						{
							ret &= AddMove( i, j, x, y, true, moveList );
						}
					}
					x = i-1; y = j;
					for( bool ret = true; 0 <= x && ret; x -= 1 )
					{
						ret &= AddMove( i, j, x, y, false, moveList );
						if( lineMin <= y && y <= lineMax )
						{
							ret &= AddMove( i, j, x, y, true, moveList );
						}
					}
					x = i; y = j+1;
					for( bool ret = true; y < BOARD_HEIGHT && ret; y++ )
					{
						ret &= AddMove( i, j, x, y, false, moveList );
						if( lineMin <= y && y <= lineMax )
						{
							ret &= AddMove( i, j, x, y, true, moveList );
						}
					}
					x = i; y = j-1;
					for( bool ret = true; 0 <= y && ret; y -= 1 )
					{
						ret &= AddMove( i, j, x, y, false, moveList );
						if( lineMin <= y && y <= lineMax )
						{
							ret &= AddMove( i, j, x, y, true, moveList );
						}
					}
					break;
				case PAWN_TYPE::RYU:
					x = i+1; y = j;
					for( bool ret = true; x < BOARD_WIDTH && ret; x++ )
					{
						ret &= AddMove( i, j, x, y, false, moveList );
					}
					x = i-1; y = j;
					for( bool ret = true; 0 <= x && ret; x -= 1 )
					{
						ret &= AddMove( i, j, x, y, false, moveList );
					}
					x = i; y = j+1;
					for( bool ret = true; y < BOARD_HEIGHT && ret; y++ )
					{
						ret &= AddMove( i, j, x, y, false, moveList );
					}
					x = i; y = j-1;
					for( bool ret = true; 0 <= y && ret; y -= 1 )
					{
						ret &= AddMove( i, j, x, y, false, moveList );
					}
					AddMove( i, j, i+1, j+1, false, moveList );
					AddMove( i, j, i-1, j+1, false, moveList );
					AddMove( i, j, i+1, j-1, false, moveList );
					AddMove( i, j, i-1, j-1, false, moveList );
					break;
				case PAWN_TYPE::GYOKU:
					AddMove( i, j, i-1, j-1, false, moveList );
					AddMove( i, j, i, j-1, false, moveList );
					AddMove( i, j, i+1, j-1, false, moveList );
					AddMove( i, j, i-1, j, false, moveList );
					AddMove( i, j, i+1, j, false, moveList );
					AddMove( i, j, i-1, j+1, false, moveList );
					AddMove( i, j, i, j+1, false, moveList );
					AddMove( i, j, i+1, j+1, false, moveList );
					break;
				default:
					break;
			}
    }
  }
	
	PAWN_MOVE move = PAWN_MOVE_ZERO;
	
	for( uchar roll=0; roll<(uchar)PAWN_ROLL::CAPTURE_MAX; roll++ )
	{
		if( captured[(uchar)turn][roll] == 0 )
		{
			continue;
		}
		move.reserve = (PAWN_ROLL)roll;
		for( uchar j=0; j<BOARD_HEIGHT; j++ )
		{
			for( uchar i=0; i<BOARD_WIDTH; i++ )
			{
				if( matrix[j][i].player != PLAYER::NONE || matrix[j][i].pawn != PAWN_TYPE::NONE )
				{
					continue;
				}

				switch( (PAWN_ROLL)roll )
				{
					case PAWN_ROLL::HU:
						if( j != lineMax )
						{
							uchar k;
							for( k=0; k<BOARD_HEIGHT; k++ )
							{
								if( matrix[k][i].player == turn && matrix[k][i].pawn == PAWN_TYPE::HU )
								{
									break;
								}
							}
							if( BOARD_HEIGHT <= k )
							{
								move.tox = i; move.toy = j;
								Move(move);
								if( IsEnd() == false )
								{
									moveList.push_back( move );
								}
								Back(move);
							}
						}
						break;
					case PAWN_ROLL::KYOH:
						if( j != lineMax )
						{
							move.tox = i; move.toy = j;
							Move(move);
							if( IsEnd() == false )
							{
								moveList.push_back( move );
							}
							Back(move);
						}
						break;
					case PAWN_ROLL::KEI:
						if( j != lineMax && j != lineMid )
						{
							move.tox = i; move.toy = j;
							Move(move);
							if( IsEnd() == false )
							{
								moveList.push_back( move );
							}
							Back(move);
						}
						break;
						break;
					case PAWN_ROLL::GIN:
					case PAWN_ROLL::KIN:
					case PAWN_ROLL::KAKU:
					case PAWN_ROLL::HI:
						move.tox = i; move.toy = j;
						Move(move);
						if( IsEnd() == false )
						{
							moveList.push_back( move );
						}
						Back(move);
						break;
					default:
						break;
				}
			}
		}
	}
}

Board::PAWN_MOVE Board::GetNextMove(PAWN_MOVE &move)
{
	uchar lineMax = 0;
	uchar lineMid = 1;
	uchar lineMin = 2;
	if(turn == PLAYER::SECOND)
	{
		lineMax = BOARD_HEIGHT-1;
		lineMid = BOARD_HEIGHT-2;
		lineMin = BOARD_HEIGHT-3;
	}
	int forward = -1;
	if( turn == PLAYER::SECOND )
	{
		forward = +1;
	}
	
	bool first = (move == PAWN_MOVE_ZERO);
	
	std::vector<PAWN_MOVE> moveList;
	
	if( move.reserve == PAWN_ROLL::NONE )
	{
		char j = 0;
		char i = 0;
		if( first == false )
		{
			j = move.fromy;
			i = move.fromx;
		}
		
		for( ; j<BOARD_HEIGHT; j++ )
		{
			for( ; i<BOARD_WIDTH; i++ )
			{
				//std::cout << (int)i << " " << (int)j << std::endl;
				CELL cell = matrix[(uchar)j][(uchar)i];
				if(cell.player != turn)
				{
					continue;
				}
				moveList.clear();
				PAWN_TYPE pawn = cell.pawn;
				int x, y;
				switch( pawn )
				{
					case PAWN_TYPE::HU:
						x = i;
						y = j+forward;
						if( lineMin <= y && y <= lineMax )
						{
							AddMove( i, j, x, y, true, moveList );
						}
            else
            {
              AddMove( i, j, x, y, false, moveList );
            }
						break;
					case PAWN_TYPE::KYOH:
						y=j+forward;
						for( bool ret = true; 0 <= (int)y && (int)y < BOARD_HEIGHT && ret; y += forward )
						{
							if( y != lineMax )
							{
								ret &= AddMove( i, j, i, y, false, moveList );
							}
							if( lineMin <= y && y <= lineMax )
							{
								ret &= AddMove( i, j, i, y, true, moveList );
							}
						}
						break;
					case PAWN_TYPE::KEI:
						x = i-1;
						y = j-forward-forward;
						if( y != lineMax && y != lineMid )
						{
							AddMove( i, j, x, y, false, moveList );
						}
						if( lineMin <= y && y <= lineMax )
						{
							AddMove( i, j, x, y, true, moveList );
						}
						break;
					case PAWN_TYPE::GIN:
						AddMove( i, j, i-1, j+forward, false, moveList );
						AddMove( i, j, i, j+forward, false, moveList );
						AddMove( i, j, i+1, j+forward, false, moveList );
						AddMove( i, j, i-1, j-forward, false, moveList );
						AddMove( i, j, i+1, j-forward, false, moveList );
						if( ( lineMin <= (j+forward) && (j+forward) <= lineMax ) || (lineMin <= j && j <= lineMax) )
						{
							AddMove( i, j, i-1, j+forward, true, moveList );
							AddMove( i, j, i, j+forward, true, moveList );
							AddMove( i, j, i+1, j+forward, true, moveList );
							AddMove( i, j, i-1, j-forward, true, moveList );
							AddMove( i, j, i+1, j-forward, true, moveList );
						}
						break;
					case PAWN_TYPE::KIN:
					case PAWN_TYPE::HUN:
					case PAWN_TYPE::KYOHN:
					case PAWN_TYPE::KEIN:
					case PAWN_TYPE::GINN:
						AddMove( i, j, i-1, j+forward, false, moveList );
						AddMove( i, j, i, j+forward, false, moveList );
						AddMove( i, j, i+1, j+forward, false, moveList );
						AddMove( i, j, i-1, j, false, moveList );
						AddMove( i, j, i+1, j, false, moveList );
						AddMove( i, j, i, j-forward, false, moveList );
						break;
					case PAWN_TYPE::KAKU:
						x = i+1; y = j+1;
						for( bool ret = true; x < BOARD_WIDTH && y < BOARD_HEIGHT && ret; x++, y++ )
						{
							if( lineMin <= y && y <= lineMax )
							{
								ret &= AddMove( i, j, x, y, true, moveList );
							}
							else
							{
								ret &= AddMove( i, j, x, y, false, moveList );
							}
						}
						x = i+1; y = j-1;
						for( bool ret = true; x < BOARD_WIDTH && 0 <= y && ret; x++, y -= 1 )
						{
							if( lineMin <= y && y <= lineMax )
							{
								ret &= AddMove( i, j, x, y, true, moveList );
							}
							else
							{
								ret &= AddMove( i, j, x, y, false, moveList );
							}
						}
						x = i-1; y = j+1;
						for( bool ret = true; 0 <= x && y < BOARD_HEIGHT && ret; x -= 1, y++ )
						{
							if( lineMin <= y && y <= lineMax )
							{
								ret &= AddMove( i, j, x, y, true, moveList );
							}
							else
							{
								ret &= AddMove( i, j, x, y, false, moveList );
							}
						}
						x = i-1; y = j-1;
						for( bool ret = true; 0 <= x && 0 <= y && ret; x -= 1, y -= 1 )
						{
							if( lineMin <= y && y <= lineMax )
							{
								ret &= AddMove( i, j, x, y, true, moveList );
							}
							else
							{
								ret &= AddMove( i, j, x, y, false, moveList );
							}
						}
						break;
					case PAWN_TYPE::UMA:
						x = i+1; y = j+1;
						for( bool ret = true; x < BOARD_WIDTH && y < BOARD_HEIGHT && ret; x++, y++ )
						{
							ret &= AddMove( i, j, x, y, false, moveList );
						}
						x = i+1; y = j-1;
						for( bool ret = true; x < BOARD_WIDTH && 0 <= y && ret; x++, y -= 1 )
						{
							ret &= AddMove( i, j, x, y, false, moveList );
						}
						x = i-1; y = j+1;
						for( bool ret = true; 0 <= x && y < BOARD_HEIGHT && ret; x -= 1, y++ )
						{
							ret &= AddMove( i, j, x, y, false, moveList );
						}
						x = i-1; y = j-1;
						for( bool ret = true; 0 <= x && 0 <= y && ret; x -= 1, y -= 1 )
						{
							ret &= AddMove( i, j, x, y, false, moveList );
						}
						AddMove( i, j, i+1, j, false, moveList );
						AddMove( i, j, i-1, j, false, moveList );
						AddMove( i, j, i, j+1, false, moveList );
						AddMove( i, j, i, j-1, false, moveList );
						break;
					case PAWN_TYPE::HI:
						x = i+1; y = j;
						for( bool ret = true; x < BOARD_WIDTH && ret; x++ )
						{
							if( lineMin <= y && y <= lineMax )
							{
								ret &= AddMove( i, j, x, y, true, moveList );
							}
							else
							{
								ret &= AddMove( i, j, x, y, false, moveList );
							}
						}
						x = i-1; y = j;
						for( bool ret = true; 0 <= x && ret; x -= 1 )
						{
							if( lineMin <= y && y <= lineMax )
							{
								ret &= AddMove( i, j, x, y, true, moveList );
							}
							else
							{
								ret &= AddMove( i, j, x, y, false, moveList );
							}
						}
						x = i; y = j+1;
						for( bool ret = true; y < BOARD_HEIGHT && ret; y++ )
						{
							if( lineMin <= y && y <= lineMax )
							{
								ret &= AddMove( i, j, x, y, true, moveList );
							}
							else
							{
								ret &= AddMove( i, j, x, y, false, moveList );
							}
						}
						x = i; y = j-1;
						for( bool ret = true; 0 <= y && ret; y -= 1 )
						{
							if( lineMin <= y && y <= lineMax )
							{
								ret &= AddMove( i, j, x, y, true, moveList );
							}
							else
							{
								ret &= AddMove( i, j, x, y, false, moveList );
							}
						}
						break;
					case PAWN_TYPE::RYU:
						x = i+1; y = j;
						for( bool ret = true; x < BOARD_WIDTH && ret; x++ )
						{
							ret &= AddMove( i, j, x, y, false, moveList );
						}
						x = i-1; y = j;
						for( bool ret = true; 0 <= x && ret; x -= 1 )
						{
							ret &= AddMove( i, j, x, y, false, moveList );
						}
						x = i; y = j+1;
						for( bool ret = true; y < BOARD_HEIGHT && ret; y++ )
						{
							ret &= AddMove( i, j, x, y, false, moveList );
						}
						x = i; y = j-1;
						for( bool ret = true; 0 <= y && ret; y -= 1 )
						{
							ret &= AddMove( i, j, x, y, false, moveList );
						}
						AddMove( i, j, i+1, j+1, false, moveList );
						AddMove( i, j, i-1, j+1, false, moveList );
						AddMove( i, j, i+1, j-1, false, moveList );
						AddMove( i, j, i-1, j-1, false, moveList );
						break;
					case PAWN_TYPE::GYOKU:
						AddMove( i, j, i-1, j-1, false, moveList );
						AddMove( i, j, i, j-1, false, moveList );
						AddMove( i, j, i+1, j-1, false, moveList );
						AddMove( i, j, i-1, j, false, moveList );
						AddMove( i, j, i+1, j, false, moveList );
						AddMove( i, j, i-1, j+1, false, moveList );
						AddMove( i, j, i, j+1, false, moveList );
						AddMove( i, j, i+1, j+1, false, moveList );
						break;
					default:
						break;
				}
				if( moveList.size() == 0 )
				{
					continue;
				}
				if( first )
				{
					//std::cout << "first " << moveList.size() << std::endl;
					return moveList.front();
				}

				//std::cout << "found" << std::endl;
				std::vector<PAWN_MOVE>::iterator ite = std::find( moveList.begin(), moveList.end(), move );
				if( ite != moveList.end() )
				{
					++ite;
					if( ite != moveList.end() )
					{
						//std::cout << "return" << std::endl;
						return *ite;
					}
				}
				//std::cout << "continue" << std::endl;
				first = true;
			}
			i = 0;
		}
	}
	
	PAWN_MOVE next = PAWN_MOVE_ZERO;
	
	uchar roll = 0;
	uchar j = 0;
	uchar i = 0;
	if( move.reserve != PAWN_ROLL::NONE )
	{
		roll = (uchar)move.reserve;
		j = move.toy;
		i = move.tox + 1;
	}
	
	moveList.clear();
	
	for( ; roll<(uchar)PAWN_ROLL::CAPTURE_MAX; roll++ )
	{
		if( captured[(uchar)turn][roll] == 0 )
		{
			continue;
		}
		next.reserve = (PAWN_ROLL)roll;
		for( ; j<BOARD_HEIGHT; j++ )
		{
			for( ; i<BOARD_WIDTH; i++ )
			{
				if( matrix[j][i].player != PLAYER::NONE || matrix[j][i].pawn != PAWN_TYPE::NONE )
				{
					continue;
				}

				switch( (PAWN_ROLL)roll )
				{
					case PAWN_ROLL::HU:
						if( j != lineMax )
						{
							uchar k;
							for( k=0; k<BOARD_HEIGHT; k++ )
							{
								if( matrix[k][i].player == turn && matrix[k][i].pawn == PAWN_TYPE::HU )
								{
									break;
								}
							}
							if( BOARD_HEIGHT <= k )
							{
								next.tox = i; next.toy = j;
								if( IsNextEnd(next) == false )
								{
									return next;
								}
							}
						}
						break;
					case PAWN_ROLL::KYOH:
						if( j != lineMax )
						{
							next.tox = i; next.toy = j;
							if( IsNextEnd(next) == false )
							{
								return next;
							}
						}
						break;
					case PAWN_ROLL::KEI:
						if( j != lineMax && j != lineMid )
						{
							next.tox = i; next.toy = j;
							if( IsNextEnd(next) == false )
							{
								return next;
							}
						}
						break;
					case PAWN_ROLL::GIN:
					case PAWN_ROLL::KIN:
					case PAWN_ROLL::KAKU:
					case PAWN_ROLL::HI:
						next.tox = i; next.toy = j;
						if( IsNextEnd(next) == false )
						{
							return next;
						}
						break;
					default:
						break;
				}
			}
			i = 0;
		}
	}
	
	return PAWN_MOVE_ZERO;
}

bool Board::AddMove( uchar fromx, uchar fromy, char tox, char toy, bool upgrade, std::vector<PAWN_MOVE> &moveList )
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
	PAWN_MOVE move{ PAWN_ROLL::NONE, fromx, fromy, utox, utoy, capture, upgrade };
	
	if( IsNextEnd(move) == false )
	{
		moveList.push_back( move );
	}
	
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
			switch( pawn )
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
					IsCapture( i+1, j+1, enemy, isCapture );
					IsCapture( i-1, j+1, enemy, isCapture );
					IsCapture( i+1, j-1, enemy, isCapture );
					IsCapture( i-1, j-1, enemy, isCapture );
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

bool Board::IsNextEnd(const PAWN_MOVE &move)
{
	Move(move);
	bool isEnd = IsEnd();
	Back(move);
	return isEnd;
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
	
	//std::cout << (int)tox << ", " << (int)toy;

	uchar utox = (uchar)tox;
	uchar utoy = (uchar)toy;
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
		captured[(uchar)turn][(uchar)move.reserve]--;
		matrix[move.toy][move.tox].player = turn;
		matrix[move.toy][move.tox].pawn = rollToType[(uchar)move.reserve];
		turn = nextTurn;
		return;
	}
	
	PAWN_TYPE pawn = matrix[move.fromy][move.fromx].pawn;
	if( move.upgrade )
	{
		pawn = toUpgrade[(uchar)pawn];
	}
	matrix[move.toy][move.tox].player = turn;
	matrix[move.toy][move.tox].pawn = pawn;
	matrix[move.fromy][move.fromx].player = PLAYER::NONE;
	matrix[move.fromy][move.fromx].pawn = PAWN_TYPE::NONE;
	if( move.capture != PAWN_TYPE::NONE )
	{
		PAWN_ROLL roll = typeToRoll[(uchar)move.capture];
		captured[(uchar)turn][(uchar)roll]++;
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
		captured[(uchar)prevTurn][(uchar)move.reserve]++;
		matrix[move.toy][move.tox].player = PLAYER::NONE;
		matrix[move.toy][move.tox].pawn = PAWN_TYPE::NONE;
		turn = prevTurn;
		return;
	}
	
	PAWN_TYPE pawn = matrix[move.toy][move.tox].pawn;
	if( move.upgrade )
	{
		pawn = toDowngrade[(uchar)pawn];
	}
	matrix[move.fromy][move.fromx].player = prevTurn;
	matrix[move.fromy][move.fromx].pawn = pawn;
	if( move.capture != PAWN_TYPE::NONE )
	{
		captured[(uchar)prevTurn][(uchar)typeToRoll[(uchar)move.capture]]--;
		matrix[move.toy][move.tox].player = turn;
		matrix[move.toy][move.tox].pawn = move.capture;
	}
	else
	{
		matrix[move.toy][move.tox].player = PLAYER::NONE;
		matrix[move.toy][move.tox].pawn = PAWN_TYPE::NONE;
	}
	turn = prevTurn;
}

void Board::PrintBoard() const
{
  for( uchar i=0; i<(uchar)PAWN_ROLL::CAPTURE_MAX; i++)
  {
    std::cout << ROLL_KANJI[i] << ":" << (unsigned int)captured[(uchar)PLAYER::SECOND][i] << " ";
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
          std::cout << "^" << PAWN_CHAR[(uchar)matrix[j][i].pawn];
          break;
        case PLAYER::SECOND:
          std::cout << PAWN_CHAR[(uchar)matrix[j][i].pawn]  << "_";
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
    std::cout << ROLL_KANJI[i] << ":" << (unsigned int)captured[(uchar)PLAYER::FIRST][i] << " ";
  }
	std::cout << std::endl;
	std::cout << PLAYER_STRING[(uchar)turn] << std::endl;
}

void Board::PrintKihu(const PAWN_MOVE &move)
{
	//uchar tox, toy;
	std::cout << numberToZenkaku[(uchar)move.tox] << numberToKanji[(uchar)move.toy];

	if( move.reserve != PAWN_ROLL::NONE )
	{
		//PAWN_ROLL reserve;
		std::cout << " " << ROLL_KANJI[(uchar)move.reserve] << " 打ち";
	}
	else
	{
		CELL cell = GetCell(move.fromx, move.fromy);
		std::cout << " " << PAWN_KANJI[(uchar)cell.pawn];

		std::cout << "( " << numberToZenkaku[(uchar)move.fromx] << ", " << numberToKanji[(uchar)move.fromy] << " )";

		//bool upgrade;
		if( move.upgrade )
		{
			std::cout << " 成り";
		}
	}
	std::cout << std::endl;
}

/*
Board::PAWN_MOVE Board::StringToMove(const std::string &str)
{
	PAWN_MOVE move;
	
	move.reserve = (PAWN_ROLL)std::stoi(str.substr(0, 2));
	move.fromx = std::stoi(str.substr(2, 2));
	move.fromy = std::stoi(str.substr(4, 2));
	move.tox = std::stoi(str.substr(6, 2));
	move.toy = std::stoi(str.substr(8, 2));
	move.capture = (PAWN_TYPE)std::stoi(str.substr(10, 2));
	if( str[11] == 't' )
	{
		move.upgrade = true;
	}
	else
	{
		move.upgrade = false;
	}
	return move;
}
*/
std::ostream& operator<<(std::ostream& stream, const Board::PAWN_MOVE& move) 
{
	stream << std::setfill('0') << std::setw(2) << (int)move.reserve;
	stream << std::setfill('0') << std::setw(2) << (int)move.fromx;
	stream << std::setfill('0') << std::setw(2) << (int)move.fromy;
	stream << std::setfill('0') << std::setw(2) << (int)move.tox;
	stream << std::setfill('0') << std::setw(2) << (int)move.toy;
	stream << std::setfill('0') << std::setw(2) << (int)move.capture;
	if( move.upgrade )
	{
		stream << "t";
	}
	else
	{
		stream << "f";
	}

	return stream;
}