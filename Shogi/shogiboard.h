#ifndef SHOGI_BOARD_H
#define SHOGI_BOARD_H

class ShogiBoard : public Board
{
public:
	ShogiBoard();

	void Init(const std::string &str);
	std::string BoardToString() const;

	MoveList GetMoveList();

	void Move(const PAWN_MOVE &move);
	void Back(const PAWN_MOVE &move);

	int GetEvaluate(const MoveList &moveList);
	int GetPriority(const PAWN_MOVE &move);

	void PrintBoard() const;

	bool operator==(const ShogiBoard& rhs) const
	{
		return (
			captured == rhs.captured &&
			matrix == rhs.matrix &&
			turn == rhs.turn
			);
	}
	bool operator!=(const ShogiBoard& rhs) const
	{
		return (
			captured != rhs.captured ||
			matrix != rhs.matrix ||
			turn != rhs.turn
			);
	}

protected:
	bool AddMove(PAWN roll, uchar fromx, uchar fromy, uchar tox, uchar toy, bool upgrade, MoveList &moveList);
	bool IsEnd() const;

private:
	uchar captured[PLAYER_MAX][(uchar)CAPTURE_MAX];

	uchar gyokux[PLAYER_MAX];
	uchar gyokuy[PLAYER_MAX];
};

#endif // SHOGI_BOARD_H