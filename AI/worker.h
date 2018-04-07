#ifndef WORKER_H
#define WORKER_H

class Ai;

class Worker
{
public:
	Worker(Ai &aiValue);
	
	void Start();
	void Join();
		
private:
	template <typename T> struct NODE
	{
		T moves;
		int score;
		Board::PAWN_MOVE GetFirstMove() const {}
		void InsertBegin(Board::PAWN_MOVE move) {}
	};

	Ai *ai;
	Board board;
	bool state;
	std::thread th;

	bool debug = true;

	std::string container = "list";

	template <typename T> void Search();
};

template<> Board::PAWN_MOVE Worker::NODE<std::vector<Board::PAWN_MOVE>>::GetFirstMove() const
{
	return *(moves.cbegin());
}
template<> Board::PAWN_MOVE Worker::NODE<std::list<Board::PAWN_MOVE>>::GetFirstMove() const
{
	return *(moves.cbegin());
}
template<> Board::PAWN_MOVE Worker::NODE<std::multimap<int, Board::PAWN_MOVE>>::GetFirstMove() const
{
	return moves.cbegin()->second;
}

template<> void Worker::NODE<std::vector<Board::PAWN_MOVE>>::InsertBegin(Board::PAWN_MOVE move)
{
	moves.insert(moves.cbegin(), PAWN_MOVE_ZERO);
}
template<> void Worker::NODE<std::list<Board::PAWN_MOVE>>::InsertBegin(Board::PAWN_MOVE move)
{
	moves.insert(moves.cbegin(), PAWN_MOVE_ZERO);
}
template<> void Worker::NODE<std::multimap<int, Board::PAWN_MOVE>>::InsertBegin(Board::PAWN_MOVE move)
{
	if (moves.empty())
	{
		moves.insert(std::make_pair(0, PAWN_MOVE_ZERO));
	}
	else
	{
		moves.insert(std::make_pair(moves.cbegin()->first - 1, PAWN_MOVE_ZERO));
	}
}

#endif // WORKER_H