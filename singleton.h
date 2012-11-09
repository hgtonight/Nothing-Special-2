#include <allegro5\allegro_color.h>
#include <allegro5\allegro_primitives.h>

class BlockyEntity {
protected:
	int PositionX, PositionY, Width, Height;
	ALLEGRO_COLOR Color;
	bool Live;

	BlockyEntity();
	BlockyEntity(int x, int y, int w, int h, ALLEGRO_COLOR c);
	~BlockyEntity();
	// allow subclasses to override the default Render function
	virtual void Render();
	bool Collides(int x, int y);
	bool Collides(int x, int y, int r);
	bool Collides(int x1, int y1, int x2, int y2);
	bool IsAlive();
};

class Brick : public BlockyEntity {
private:
	static int BrickCount;
	int Strength;
public:
	Brick();
	Brick(int x, int y);
	Brick(int x, int y, int s);
	Brick(int x, int y, int s, int w, int h);
	~Brick();
	void Harm();
	void Harm(int Damage);
};

class Ball : public BlockyEntity {
private:
	static int BallCount;
	float Speed, Angle;
public:
	Ball();
	~Ball();
	void Render();
};

class Paddle : public BlockyEntity {
private:
	bool Rotated;
	int FixedCoord;
public:
	Paddle(int position);
	Paddle(int position, bool vertical);
	~Paddle();
	bool IsVertical();
	bool IsHorizontal();
};