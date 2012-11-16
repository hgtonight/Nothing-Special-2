#include <allegro5\allegro_color.h>
#include <allegro5\allegro_primitives.h>

class BlockyEntity {
protected:
public:
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
	void SetPosition(int x, int y);
	void SetDimensions(int w, int h);
};

class Brick : public BlockyEntity {
private:
	int Strength;
public:
	static int BrickCount;
	Brick();
	Brick(int x, int y);
	Brick(int x, int y, int s);
	Brick(int x, int y, int s, int w, int h);
	~Brick();
	void Harm();
	void Harm(int Damage);
	void UpdateColor();
};

class Ball : public BlockyEntity {
public:
	static int BallCount;
	float Speed, Theta;
	Ball();
	~Ball();
	void Render();
	void ReflectX();
	void ReflectY();
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