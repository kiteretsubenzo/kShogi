#ifndef DEFINITIONS_H
#define DEFINITIONS_H

#define BOARD_WIDTH 9
#define BOARD_HEIGHT 9

typedef unsigned char uchar;

class PAWN_ROLL;

class PAWN_TYPE
{
public:
	enum TYPE
	{
		HU, HUN, KYOH, KYOHN, KEI, KEIN, GIN, GINN, KIN, KAKU, UMA, HI, RYU, GYOKU,
		MAX, NONE = MAX
	};
	
	PAWN_TYPE()
	{
		type = NONE;
	}
	PAWN_TYPE( const TYPE &t )
	{
		type = t;
	}
	PAWN_TYPE( const PAWN_ROLL &roll );
	
	void Upgrade();
	void Downgrade();
	
	bool operator==( const TYPE& rhs ) const
	{
		return type == rhs;
	}
	bool operator!=( const TYPE& rhs ) const
	{
		return type != rhs;
	}
	
	PAWN_TYPE& operator=( const TYPE& rhs )
	{
		type = rhs;
		return *this;
	}
	
	operator int() const
	{
		return (int)type;
	}
	operator char() const;
	operator std::string() const;
	operator TYPE() const
	{
		return type;
	}
	
private:
	TYPE type;
};

class PAWN_ROLL
{
public:
	enum ROLL
	{
		HU, KYOH, KEI, GIN, KIN, HI, KAKU, GYOKU, MAX,
		NONE = MAX, CAPTURE_MAX = GYOKU
	};
	
	PAWN_ROLL()
	{
		roll = NONE;
	}
	PAWN_ROLL(const ROLL &r)
	{
		roll = r;
	}
	PAWN_ROLL(const PAWN_TYPE &type);
	PAWN_ROLL(int i)
	{
		//roll = INT_ROLL[i];
		roll = (ROLL)i;
	}
	
	bool operator==( const ROLL& rhs ) const
	{
		return roll == rhs;
	}
	bool operator!=( const ROLL& rhs ) const
	{
		return roll != rhs;
	}
	bool operator==(const PAWN_ROLL& rhs) const
	{
		return roll == rhs.roll;
	}
	bool operator!=(const PAWN_ROLL& rhs) const
	{
		return roll != rhs.roll;
	}
	
	PAWN_ROLL& operator=( const ROLL& rhs )
	{
		roll = rhs;
		return *this;
	}
	
	operator int() const
	{
		return (int)roll;
	}
	operator char() const;
	operator std::string () const;
	
private:
	ROLL roll;
};

static int SCORE_NONE = std::numeric_limits<int>::max() - 1;
static int SCORE_WIN = 99999;
static int MOVE_MAX = (BOARD_HEIGHT*BOARD_WIDTH)*(BOARD_HEIGHT*BOARD_WIDTH)*PAWN_ROLL::CAPTURE_MAX;

std::vector<std::string> split(std::string str, char c);
std::unordered_map<std::string, std::string> fromJson(std::string str);

#endif // DEFINITIONS_H