#ifndef BOARD_H
#define BOARD_H

#define PRIORITY_NONE		0
#define PRIORITY_LIST		1
#define PRIORITY_MULTISET	2
#define PRIORITY_BUFFER		3

#define USE_PRIORITY PRIORITY_BUFFER

struct CELL
{
	Player player;
	Pawn pawn;

	CELL() {}
	CELL(const Player &playerValue, const Pawn &pawnValue) : player(playerValue), pawn(pawnValue) {}
};

#if USE_PRIORITY == PRIORITY_MULTISET
class MoveList
{
public:
	bool empty() const
	{
		return list.empty();
	}
	size_t size() const
	{
		return list.size();
	}
	void clear()
	{
		list.clear();
	}
	void push(const Move &move)
	{
		list.insert(move);
	}
	Move front() const
	{
		return *(list.begin());
	}
	void pop_front()
	{
		list.erase(list.begin());
	}
	void sort() {}
private:
	std::multiset<Move> list;
};
#elif USE_PRIORITY == PRIORITY_BUFFER
class MoveList
{
public:
	bool empty() const
	{
		return first == last;
	}
	unsigned int size() const
	{
		return last - first;
	}
	void clear()
	{
		first = 0;
		last = 0;
	}
	void push(const Move &move)
	{
		list[last] = move;
		last++;
	}
	Move front() const
	{
		return list[first];
	}
	void pop_front()
	{
		first++;
	}
	void sort()
	{
		//list.sort();
	}
private:
	Move list[1024];
	int first = 0;
	int last = 0;
};
#else
class MoveList
{
public:
	bool empty() const
	{
		return list.empty();
	}
	size_t size() const
	{
		return list.size();
	}
	void clear()
	{
		list.clear();
	}
	void push(const Move &move)
	{
		list.push_back(move);
	}
	Move front() const
	{
		return *(list.begin());
	}
	void pop_front()
	{
		list.erase(list.begin());
	}
	void sort()
	{
		list.sort();
	}
private:
	std::list<Move> list;
};
#endif

class Board
{
public:
	Board()
	{
		for (uchar i = 0; i < PlayerDef::MAX; i++)
		{
			for (uchar j = 0; j < PawnDef::CAPTURE_MAX; j++)
			{
				captured[i][j] = 0;
			}
		}

		for (uchar j = 1; j <= BoardDef::HEIGHT; j++)
		{
			for (uchar i = 1; i <= BoardDef::WIDTH; i++)
			{
				matrix[j][i] = CELL(PlayerDef::NONE, PawnDef::NONE);
			}
		}

		for (uchar i = 0; i < BoardDef::WIDTH + 2; i++)
		{
			matrix[0][i] = CELL(PlayerDef::WALL, PawnDef::NONE);
			matrix[BoardDef::HEIGHT + 1][i] = CELL(PlayerDef::WALL, PawnDef::NONE);
		}

		for (uchar j = 1; j <= BoardDef::HEIGHT; j++)
		{
			matrix[j][0] = CELL(PlayerDef::WALL, PawnDef::NONE);
			matrix[j][BoardDef::WIDTH + 1] = CELL(PlayerDef::WALL, PawnDef::NONE);
		}

		turn = PlayerDef::FIRST;
		enemy = PlayerDef::SECOND;
	}

	void Init(const std::string &str)
	{
		std::vector<std::string> strs = Json::split(str, '\n');

		for (int i = 0; i < PlayerDef::MAX; i++)
		{
			gyokux[i] = 0;
			gyokuy[i] = 0;
		}

		for (int i = 0; i < PawnDef::CAPTURE_MAX; i++)
		{
			std::string first = strs[BoardDef::HEIGHT + 1].substr(i * 4 + 1, 2);
			std::string second = strs[0].substr(i * 4 + 1, 2);
			captured[PlayerDef::FIRST][i] = stoi(first);
			captured[PlayerDef::SECOND][i] = stoi(second);
		}

		for (int j = 2; j <= BoardDef::HEIGHT + 1; j++)
		{
			for (int i = 1; i <= BoardDef::WIDTH; i++)
			{
				char c[2] = { strs[j - 1][(i - 1) * 2], strs[j - 1][(i - 1) * 2 + 1] };
				if (c[0] == ' ')
				{
					matrix[j - 1][i].player = PlayerDef::NONE;
					matrix[j - 1][i].pawn = PawnDef::NONE;
				}
				else
				{
					char type;
					Player player = PlayerDef::NONE;
					if (c[0] == '^')
					{
						player = PlayerDef::FIRST;
						type = c[1];
					}
					else
					{
						player = PlayerDef::SECOND;
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

		if (strs[BoardDef::HEIGHT + 2] == "first")
		{
			turn = PlayerDef::FIRST;
			enemy = PlayerDef::SECOND;
		}
		else if (strs[BoardDef::HEIGHT + 2] == "second")
		{
			turn = PlayerDef::SECOND;
			enemy = PlayerDef::FIRST;
		}
	}

	// TODO
	std::string BoardToString() const
	{
		std::string sout = "";

		for (int i = 0; i < PawnDef::CAPTURE_MAX; i++)
		{
			sout += Pawn_CHAR[i];
			if (captured[PlayerDef::SECOND][i] < 10)
			{
				sout += '0';
			}
			sout += std::to_string(captured[PlayerDef::SECOND][i]);
			sout += ' ';
		}
		sout += '\n';

		for (int j = 2; j <= BoardDef::HEIGHT + 1; j++)
		{
			for (int i = 1; i <= BoardDef::WIDTH; i++)
			{
				if (matrix[j - 1][i].player == PlayerDef::FIRST)
				{
					sout += '^';
					sout += Pawn_CHAR[matrix[j - 1][i].pawn];
				}
				else if (matrix[j - 1][i].player == PlayerDef::SECOND)
				{
					sout += Pawn_CHAR[matrix[j - 1][i].pawn];
					sout += '_';
				}
				else
				{
					sout += " .";
				}
			}
			sout += '\n';
		}

		for (int i = 0; i < PawnDef::CAPTURE_MAX; i++)
		{
			sout += Pawn_CHAR[i];
			if (captured[PlayerDef::FIRST][i] < 10)
			{
				sout += '0';
			}
			sout += std::to_string(captured[PlayerDef::FIRST][i]);
			sout += ' ';
		}
		sout += '\n';

		if (turn == PlayerDef::FIRST)
		{
			sout += "first";
		}
		else
		{
			sout += "second";
		}

		return sout;
	}
	
	MoveList GetMoveList()
	{
		MoveList moveList;

		uchar lineMax, lineMin, lineTop, lineMid;

		if (turn == PlayerDef::FIRST)
		{
			lineMax = 3;
			lineMin = 1;
			lineTop = 1;
			lineMid = 2;
		}
		else
		{
			lineMax = BoardDef::HEIGHT;
			lineMin = BoardDef::HEIGHT - 2;
			lineTop = BoardDef::HEIGHT;
			lineMid = BoardDef::HEIGHT - 1;
		}
		int forward = -1;
		if (turn == PlayerDef::SECOND)
		{
			forward = +1;
		}
		for (char j = 1; j <= BoardDef::HEIGHT; j++)
		{
			for (char i = 1; i <= BoardDef::WIDTH; i++)
			{
				CELL cell = matrix[(uchar)j][(uchar)i];
				if (cell.player == enemy)
				{
					continue;
				}
				Pawn pawn = cell.pawn;
				int x, y;
				switch (pawn)
				{
				case PawnDef::HU:
					x = i;
					y = j + forward;
					if (y != lineTop)
					{
						AddMove(PawnDef::NONE, i, j, x, y, false, moveList);
					}
					if (lineMin <= y && y <= lineMax)
					{
						AddMove(PawnDef::NONE, i, j, x, y, true, moveList);
					}
					break;
				case PawnDef::KYOH:
					y = j + forward;
					for (bool ret = true; 0 < y && y <= BoardDef::HEIGHT && ret; y += forward)
					{
						if (y != lineTop)
						{
							ret &= AddMove(PawnDef::NONE, i, j, i, y, false, moveList);
						}
						if (lineMin <= y && y <= lineMax)
						{
							ret &= AddMove(PawnDef::NONE, i, j, i, y, true, moveList);
						}
					}
					break;
				case PawnDef::KEI:
					x = i - 1;
					y = j - forward - forward;
					if (y != lineTop && y != lineMid)
					{
						AddMove(PawnDef::NONE, i, j, x, y, false, moveList);
					}
					if (lineMin <= y && y <= lineMax)
					{
						AddMove(PawnDef::NONE, i, j, x, y, true, moveList);
					}
					break;
				case PawnDef::GIN:
					AddMove(PawnDef::NONE, i, j, i - 1, j + forward, false, moveList);
					AddMove(PawnDef::NONE, i, j, i, j + forward, false, moveList);
					AddMove(PawnDef::NONE, i, j, i + 1, j + forward, false, moveList);
					AddMove(PawnDef::NONE, i, j, i - 1, j - forward, false, moveList);
					AddMove(PawnDef::NONE, i, j, i + 1, j - forward, false, moveList);
					if ((lineMin <= (j + forward) && (j + forward) <= lineMax) || (lineMin <= j && j <= lineMax))
					{
						AddMove(PawnDef::NONE, i, j, i - 1, j + forward, true, moveList);
						AddMove(PawnDef::NONE, i, j, i, j + forward, true, moveList);
						AddMove(PawnDef::NONE, i, j, i + 1, j + forward, true, moveList);
						AddMove(PawnDef::NONE, i, j, i - 1, j - forward, true, moveList);
						AddMove(PawnDef::NONE, i, j, i + 1, j - forward, true, moveList);
					}
					break;
				case PawnDef::KIN:
				case PawnDef::HUN:
				case PawnDef::KYOHN:
				case PawnDef::KEIN:
				case PawnDef::GINN:
					AddMove(PawnDef::NONE, i, j, i - 1, j + forward, false, moveList);
					AddMove(PawnDef::NONE, i, j, i, j + forward, false, moveList);
					AddMove(PawnDef::NONE, i, j, i + 1, j + forward, false, moveList);
					AddMove(PawnDef::NONE, i, j, i - 1, j, false, moveList);
					AddMove(PawnDef::NONE, i, j, i + 1, j, false, moveList);
					AddMove(PawnDef::NONE, i, j, i, j - forward, false, moveList);
					break;
				case PawnDef::KAKU:
					x = i + 1; y = j + 1;
					while (true)
					{
						if (AddMove(PawnDef::NONE, i, j, x, y, false, moveList) == false)
						{
							break;
						}
						if (lineMin <= y && y <= lineMax)
						{
							if (AddMove(PawnDef::NONE, i, j, x, y, true, moveList) == false)
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
						if (AddMove(PawnDef::NONE, i, j, x, y, false, moveList) == false)
						{
							break;
						}
						if (lineMin <= y && y <= lineMax)
						{
							if (AddMove(PawnDef::NONE, i, j, x, y, true, moveList) == false)
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
						if (AddMove(PawnDef::NONE, i, j, x, y, false, moveList) == false)
						{
							break;
						}
						if (lineMin <= y && y <= lineMax)
						{
							if (AddMove(PawnDef::NONE, i, j, x, y, true, moveList) == false)
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
						if (AddMove(PawnDef::NONE, i, j, x, y, false, moveList) == false)
						{
							break;
						}
						if (lineMin <= y && y <= lineMax)
						{
							if (AddMove(PawnDef::NONE, i, j, x, y, true, moveList) == false)
							{
								break;
							}
						}
						x -= 1;
						y -= 1;
					}
					break;
				case PawnDef::UMA:
					x = i + 1; y = j + 1;
					while (true)
					{
						if (AddMove(PawnDef::NONE, i, j, x, y, false, moveList) == false)
						{
							break;
						}
						x++;
						y++;
					}
					x = i + 1; y = j - 1;
					while (true)
					{
						if (AddMove(PawnDef::NONE, i, j, x, y, false, moveList) == false)
						{
							break;
						}
						x++;
						y -= 1;
					}
					x = i - 1; y = j + 1;
					while (true)
					{
						if (AddMove(PawnDef::NONE, i, j, x, y, false, moveList) == false)
						{
							break;
						}
						x -= 1;
						y++;
					}
					x = i - 1; y = j - 1;
					while (true)
					{
						if (AddMove(PawnDef::NONE, i, j, x, y, false, moveList) == false)
						{
							break;
						}
						x -= 1;
						y -= 1;
					}
					AddMove(PawnDef::NONE, i, j, i + 1, j, false, moveList);
					AddMove(PawnDef::NONE, i, j, i - 1, j, false, moveList);
					AddMove(PawnDef::NONE, i, j, i, j + 1, false, moveList);
					AddMove(PawnDef::NONE, i, j, i, j - 1, false, moveList);
					break;
				case PawnDef::HI:
					x = i + 1; y = j;
					while (true)
					{
						if (AddMove(PawnDef::NONE, i, j, x, y, false, moveList) == false)
						{
							break;
						}
						if (lineMin <= y && y <= lineMax)
						{
							if (AddMove(PawnDef::NONE, i, j, x, y, true, moveList) == false)
							{
								break;
							}
						}
						x++;
					}
					x = i - 1; y = j;
					while (true)
					{
						if (AddMove(PawnDef::NONE, i, j, x, y, false, moveList) == false)
						{
							break;
						}
						if (lineMin <= y && y <= lineMax)
						{
							if (AddMove(PawnDef::NONE, i, j, x, y, true, moveList) == false)
							{
								break;
							}
						}
						x -= 1;
					}
					x = i; y = j + 1;
					while (true)
					{
						if (AddMove(PawnDef::NONE, i, j, x, y, false, moveList) == false)
						{
							break;
						}
						if (lineMin <= y && y <= lineMax)
						{
							if (AddMove(PawnDef::NONE, i, j, x, y, true, moveList) == false)
							{
								break;
							}
						}
						y++;
					}
					x = i; y = j - 1;
					while (true)
					{
						if (AddMove(PawnDef::NONE, i, j, x, y, false, moveList) == false)
						{
							break;
						}
						if (lineMin <= y && y <= lineMax)
						{
							if (AddMove(PawnDef::NONE, i, j, x, y, true, moveList) == false)
							{
								break;
							}
						}
						y -= 1;
					}
					break;
				case PawnDef::RYU:
					x = i + 1; y = j;
					while (true)
					{
						if (AddMove(PawnDef::NONE, i, j, x, y, false, moveList) == false)
						{
							break;
						}
						x++;
					}
					x = i - 1; y = j;
					while (true)
					{
						if (AddMove(PawnDef::NONE, i, j, x, y, false, moveList) == false)
						{
							break;
						}
						x -= 1;
					}
					x = i; y = j + 1;
					while (true)
					{
						if (AddMove(PawnDef::NONE, i, j, x, y, false, moveList) == false)
						{
							break;
						}
						y++;
					}
					x = i; y = j - 1;
					while (true)
					{
						if (AddMove(PawnDef::NONE, i, j, x, y, false, moveList) == false)
						{
							break;
						}
						y -= 1;
					}
					AddMove(PawnDef::NONE, i, j, i + 1, j + 1, false, moveList);
					AddMove(PawnDef::NONE, i, j, i - 1, j + 1, false, moveList);
					AddMove(PawnDef::NONE, i, j, i + 1, j - 1, false, moveList);
					AddMove(PawnDef::NONE, i, j, i - 1, j - 1, false, moveList);
					break;
				case PawnDef::GYOKU:
					AddMove(PawnDef::NONE, i, j, i - 1, j - 1, false, moveList);
					AddMove(PawnDef::NONE, i, j, i, j - 1, false, moveList);
					AddMove(PawnDef::NONE, i, j, i + 1, j - 1, false, moveList);
					AddMove(PawnDef::NONE, i, j, i - 1, j, false, moveList);
					AddMove(PawnDef::NONE, i, j, i + 1, j, false, moveList);
					AddMove(PawnDef::NONE, i, j, i - 1, j + 1, false, moveList);
					AddMove(PawnDef::NONE, i, j, i, j + 1, false, moveList);
					AddMove(PawnDef::NONE, i, j, i + 1, j + 1, false, moveList);
					break;
				default:
					// 空きだったら打ち
					for (uchar roll = 0; roll < PawnDef::CAPTURE_MAX; roll++)
					{
						if (captured[turn][roll] == 0)
						{
							continue;
						}
						switch (roll)
						{
						case PawnDef::HU:
							if (j != lineTop)
							{
								uchar k;
								for (k = 1; k <= BoardDef::HEIGHT; k++)
								{
									if (matrix[k][i].player == turn && matrix[k][i].pawn == PawnDef::HU)
									{
										break;
									}
								}
								if (BoardDef::HEIGHT < k)
								{
									AddMove(roll, 0, 0, i, j, false, moveList);
								}
							}
							break;
						case PawnDef::KYOH:
							if (j != lineTop)
							{
								AddMove(roll, 0, 0, i, j, false, moveList);
							}
							break;
						case PawnDef::KEI:
							if (j != lineTop && j != lineMid)
							{
								AddMove(roll, 0, 0, i, j, false, moveList);
							}
							break;
						case PawnDef::GIN:
						case PawnDef::KIN:
						case PawnDef::KAKU:
						case PawnDef::HI:
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

#if USE_PRIORITY != PRIORITY_NONE
		moveList.sort();
#endif
		return moveList;
	}

	void Forward(const Move &move)
	{
		if (move.reserve != PawnDef::NONE)
		{
			captured[turn][move.reserve]--;
			matrix[move.to.y][move.to.x].player = turn;
			matrix[move.to.y][move.to.x].pawn = move.reserve;

			SwitchTurn();
			return;
		}

		Pawn pawn = move.from.pawn;
		if (move.upgrade)
		{
			Upgrade(pawn);
		}
		matrix[move.to.y][move.to.x].player = turn;
		matrix[move.to.y][move.to.x].pawn = pawn;
		matrix[move.from.y][move.from.x].player = PlayerDef::NONE;
		matrix[move.from.y][move.from.x].pawn = PawnDef::NONE;

		if (move.to.pawn != PawnDef::NONE)
		{
			captured[turn][Down(move.to.pawn)]++;
		}
		if (move.from.pawn == PawnDef::GYOKU)
		{
			gyokux[turn] = move.to.x;
			gyokuy[turn] = move.to.y;
		}

		SwitchTurn();
	}

	void Back(const Move &move)
	{
		if (move.reserve != PawnDef::NONE)
		{
			captured[enemy][move.reserve]++;

			matrix[move.to.y][move.to.x].player = PlayerDef::NONE;
			matrix[move.to.y][move.to.x].pawn = PawnDef::NONE;

			SwitchTurn();
			return;
		}

		Pawn pawn = move.from.pawn;
		if (move.upgrade)
		{
			Downgrade(pawn);
		}
		matrix[move.from.y][move.from.x].player = enemy;
		matrix[move.from.y][move.from.x].pawn = pawn;

		if (move.to.pawn != PawnDef::NONE)
		{
			captured[enemy][Down(move.to.pawn)]--;

			matrix[move.to.y][move.to.x].player = turn;
			matrix[move.to.y][move.to.x].pawn = move.to.pawn;
		}
		else
		{
			matrix[move.to.y][move.to.x].player = PlayerDef::NONE;
			matrix[move.to.y][move.to.x].pawn = PawnDef::NONE;
		}

		if (move.from.pawn == PawnDef::GYOKU)
		{
			gyokux[enemy] = move.from.x;
			gyokuy[enemy] = move.from.y;
		}

		SwitchTurn();
	}

	int GetEvaluate(const MoveList &moveList)
	{
		return (int)(moveList.size());
	}
	int GetPriority(const Move &move)
	{
		int priority = 0;
#if USE_PRIORITY != PRIORITY_NONE && USE_PRIORITY != PRIORITY_BUFFER
		// 王手がかかってるか？
		SwitchTurn();
		if (IsEnd())
		{
			priority += 1000;
		}
		SwitchTurn();
		// 駒を取るか？
		if (move.to.pawn != Pawn_NONE)
		{
			priority += (int)(move.to.pawn) + (int)Pawn_MAX;
		}
		// 成るか？
		if (move.upgrade)
		{
			priority += (int)(move.from.pawn);
		}
#endif
		return priority;
	}

	void PrintBoard() const
	{
		for (uchar i = 0; i < PawnDef::CAPTURE_MAX; i++)
		{
			std::cout << Pawn_KANJI[i] << ":" << (unsigned int)captured[PlayerDef::SECOND][i] << " ";
		}
		std::cout << std::endl;
		std::cout << "９８７６５４３２１" << std::endl;
		for (uchar j = 1; j <= BoardDef::HEIGHT; j++)
		{
			for (uchar i = 1; i <= BoardDef::WIDTH; i++)
			{
				switch (matrix[j][i].player)
				{
				case PlayerDef::FIRST:
					std::cout << "^" << Pawn_CHAR[matrix[j][i].pawn];
					break;
				case PlayerDef::SECOND:
					std::cout << Pawn_CHAR[matrix[j][i].pawn] << "_";
					break;
				case PlayerDef::NONE:
					std::cout << " .";
					break;
				default:
					break;
				}
			}
			std::cout << " " << numberToKanji[j - 1] << std::endl;
		}
		for (uchar i = 0; i < PawnDef::CAPTURE_MAX; i++)
		{
			std::cout << Pawn_KANJI[i] << ":" << (unsigned int)captured[PlayerDef::FIRST][i] << " ";
		}
		std::cout << std::endl;
		//std::cout << Player_STRING[(int)turn] << std::endl;
		if (turn == PlayerDef::FIRST)
		{
			std::cout << "先手番" << std::endl;
		}
		else
		{
			std::cout << "後手番" << std::endl;
		}
	}

	bool operator==(const Board& rhs) const
	{
		return (
			matrix == rhs.matrix &&
			turn == rhs.turn
		);
	}
	bool operator!=(const Board& rhs) const
	{
		return (
			matrix != rhs.matrix ||
			turn != rhs.turn
		);
	}

protected:
	bool AddMove(Pawn roll, int fromx, int fromy, int tox, int toy, bool upgrade, MoveList &moveList)
	{
		if (matrix[toy][tox].player == turn || matrix[toy][tox].player == PlayerDef::WALL)
		{
			return false;
		}

		Move move(roll, fromx, fromy, tox, toy, matrix[fromy][fromx].pawn, matrix[toy][tox].pawn, upgrade, 0);
		if (roll != PawnDef::NONE)
		{
			move.from.x = 0;
			move.from.y = 0;
			move.from.pawn = PawnDef::NONE;
		}

		Forward(move);
		// 負ける手は指さない
		if (IsEnd() == false)
		{
			move.priority = GetPriority(move);
			moveList.push(move);
		}
		Back(move);

		return matrix[toy][tox].pawn == PawnDef::NONE;
	}

	bool IsEnd() const
	{
		// 玉の位置を求める
		char gyokux = this->gyokux[enemy];
		char gyokuy = this->gyokuy[enemy];
		if (gyokux == 0)
		{
			return false;
		}
		CELL cell;
		if (turn == PlayerDef::FIRST)
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
				if (IsGyokuKinUpgrade(cell.pawn) || cell.pawn == PawnDef::HU || cell.pawn == PawnDef::GIN)
				{
					return true;
				}
			}
			if (GetCell(gyokux - 1, gyokuy - 1, cell))
			{
				if (cell.pawn == PawnDef::GYOKU || cell.pawn == PawnDef::GIN || cell.pawn == PawnDef::RYU)
				{
					return true;
				}
			}
			if (GetCell(gyokux + 1, gyokuy - 1, cell))
			{
				if (cell.pawn == PawnDef::GYOKU || cell.pawn == PawnDef::GIN || cell.pawn == PawnDef::RYU)
				{
					return true;
				}
			}
			if (GetCell(gyokux + 1, gyokuy + 1, cell))
			{
				if (IsGyokuKinUpgrade(cell.pawn) || cell.pawn == PawnDef::GIN)
				{
					return true;
				}
			}
			if (GetCell(gyokux - 1, gyokuy + 1, cell))
			{
				if (IsGyokuKinUpgrade(cell.pawn) || cell.pawn == PawnDef::GIN)
				{
					return true;
				}
			}

			// 桂
			if (gyokuy < BoardDef::HEIGHT)
			{
				if (GetCell(gyokux - 1, gyokuy + 2, cell))
				{
					if (cell.pawn == PawnDef::KEI)
					{
						return true;
					}
				}
				if (GetCell(gyokux + 1, gyokuy + 2, cell))
				{
					if (cell.pawn == PawnDef::KEI)
					{
						return true;
					}
				}
			}

			// 香
			for (int j = gyokuy + 1; j <= BoardDef::HEIGHT; j++)
			{
				if (matrix[j][gyokux].player == PlayerDef::NONE)
				{
					continue;
				}
				if (matrix[j][gyokux].player == enemy)
				{
					break;
				}
				if (matrix[j][gyokux].pawn == PawnDef::KYOH)
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
				if (IsGyokuKinUpgrade(cell.pawn) || cell.pawn == PawnDef::HU || cell.pawn == PawnDef::GIN)
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
				if (IsGyokuKinUpgrade(cell.pawn) || cell.pawn == PawnDef::GIN)
				{
					return true;
				}
			}
			if (GetCell(gyokux + 1, gyokuy - 1, cell))
			{
				if (IsGyokuKinUpgrade(cell.pawn) || cell.pawn == PawnDef::GIN)
				{
					return true;
				}
			}
			if (GetCell(gyokux + 1, gyokuy + 1, cell))
			{
				if (cell.pawn == PawnDef::GYOKU || cell.pawn == PawnDef::GIN || cell.pawn == PawnDef::RYU)
				{
					return true;
				}
			}
			if (GetCell(gyokux - 1, gyokuy + 1, cell))
			{
				if (cell.pawn == PawnDef::GYOKU || cell.pawn == PawnDef::GIN || cell.pawn == PawnDef::RYU)
				{
					return true;
				}
			}

			// 桂
			if (2 <= gyokuy)
			{
				if (GetCell(gyokux - 1, gyokuy - 2, cell))
				{
					if (cell.pawn == PawnDef::KEI)
					{
						return true;
					}
				}
				if (GetCell(gyokux + 1, gyokuy - 2, cell))
				{
					if (cell.pawn == PawnDef::KEI)
					{
						return true;
					}
				}
			}

			// 香
			for (int j = gyokuy - 1; 0 < j; j--)
			{
				if (matrix[j][gyokux].player == PlayerDef::NONE)
				{
					continue;
				}
				if (matrix[j][gyokux].player == enemy)
				{
					break;
				}
				if (matrix[j][gyokux].pawn == PawnDef::KYOH)
				{
					return true;
				}
				break;
			}
		}

		{
			int i, j;
			// 飛龍
			i = gyokux + 1;
			while (true)
			{
				if (matrix[gyokuy][i].player == turn && Down(matrix[gyokuy][i].pawn) == PawnDef::HI)
				{
					return true;
				}
				if (matrix[gyokuy][i].player != PlayerDef::NONE)
				{
					break;
				}
				i++;
			}
			i = gyokux - 1;
			while (true)
			{
				if (matrix[gyokuy][i].player == turn && Down(matrix[gyokuy][i].pawn) == PawnDef::HI)
				{
					return true;
				}
				if (matrix[gyokuy][i].player != PlayerDef::NONE)
				{
					break;
				}
				i--;
			}
			j = gyokuy + 1;
			while (true)
			{
				if (matrix[j][gyokux].player == turn && Down(matrix[j][gyokux].pawn) == PawnDef::HI)
				{
					return true;
				}
				if (matrix[j][gyokux].player != PlayerDef::NONE)
				{
					break;
				}
				j++;
			}
			j = gyokuy - 1;
			while (true)
			{
				if (matrix[j][gyokux].player == turn && Down(matrix[j][gyokux].pawn) == PawnDef::HI)
				{
					return true;
				}
				if (matrix[j][gyokux].player != PlayerDef::NONE)
				{
					break;
				}
				j--;
			}

			// 角馬
			i = gyokux + 1;
			j = gyokuy + 1;
			while (true)
			{
				if (matrix[j][i].player == turn && Down(matrix[j][i].pawn) == PawnDef::KAKU)
				{
					return true;
				}
				if (matrix[j][i].player != PlayerDef::NONE)
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
				if (matrix[j][i].player == turn && Down(matrix[j][i].pawn) == PawnDef::KAKU)
				{
					return true;
				}
				if (matrix[j][i].player != PlayerDef::NONE)
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
				if (matrix[j][i].player == turn && Down(matrix[j][i].pawn) == PawnDef::KAKU)
				{
					return true;
				}
				if (matrix[j][i].player != PlayerDef::NONE)
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
				if (matrix[j][i].player == turn && Down(matrix[j][i].pawn) == PawnDef::KAKU)
				{
					return true;
				}
				if (matrix[j][i].player != PlayerDef::NONE)
				{
					break;
				}
				i++;
				j--;
			}
		}

		return false;
	}

	CELL GetCell(int x, int y) { return matrix[y][x]; }
	bool GetCell(int tox, int toy, CELL &cell) const
	{
		if (matrix[toy][tox].player != turn)
		{
			return false;
		}

		cell = matrix[toy][tox];

		return true;
	}

	void SwitchTurn()
	{
		if (turn == PlayerDef::FIRST)
		{
			turn = PlayerDef::SECOND;
			enemy = PlayerDef::FIRST;
		}
		else
		{
			turn = PlayerDef::FIRST;
			enemy = PlayerDef::SECOND;
		}
	}
	
	CELL matrix[BoardDef::HEIGHT+2][BoardDef::WIDTH+2];
	Player turn;
	Player enemy;

private:
	uchar captured[PlayerDef::MAX][PawnDef::CAPTURE_MAX];

	uchar gyokux[PlayerDef::MAX];
	uchar gyokuy[PlayerDef::MAX];

	static void Upgrade(Pawn &type)
	{
		type |= 0x08;
	}
	static void Downgrade(Pawn &type)
	{
		type &= 0x07;
	}
	static Pawn Down(const Pawn type)
	{
		return (type & 0x07);
	}
	static bool IsUpgrade(const Pawn type)
	{
		return ((type & 0x08) != 0);
	}
	static bool IsGyokuKinUpgrade(const Pawn type)
	{
		return (PawnDef::KIN <= type);
	}
};

#endif // BOARD_H