#include "singleton.h"

BlockyEntity::BlockyEntity() {
	PositionX = 1;
	PositionY = 1;
	Width = 40;
	Height = 10;
	Color = al_color_hsl(1, 0, 1);
	Live = true;
}
BlockyEntity::BlockyEntity(int x, int y, int w, int h, ALLEGRO_COLOR c) {
	PositionX = x;
	PositionY = y;
	Width = w;
	Height = h;
	Color = c;
	Live = true;
}
BlockyEntity::~BlockyEntity() {
	// end of life tasks
}
void BlockyEntity::Render() {
	al_draw_filled_rectangle(PositionX, PositionY, PositionX + Width, PositionY + Height, Color);
}
// determines if the point x, y is inside the entity
bool BlockyEntity::Collides(int x, int y) {
	// same as a box collision with zero width
	return Collides(x, y, x, y);	
}
// determines if a circle collides with this entity
bool BlockyEntity::Collides(int x, int y, int r) {
	// TODO: put a real circle-rectangle detection system in place	
	// lazy for right now
	return Collides(x - r, y - r, x + r, y + r);
	return false;
}
// determines if a rectangle collides with this entity
bool BlockyEntity::Collides(int x1, int y1, int x2, int y2) {
	// check if completely on the left or right
	if(PositionX + Width < x1 || PositionX > x2) {
		return false;
	}
	// check if completely above or below
	if (PositionY + Height < y1 || PositionY > y2) {
		return false;
	}
	// we know there is a collision!
	return true;
}
bool BlockyEntity::IsAlive() {
	return Live;
}

int Brick::BrickCount = 0;
Brick::Brick() {
	BrickCount++;
	Strength = 1;
}
Brick::Brick(int x, int y) {
	BrickCount++;
	PositionX = x;
	PositionY = y;
	Strength = 1;
}
Brick::Brick(int x, int y, int s) {
	BrickCount++;
	PositionX = x;
	PositionY = y;
	Strength = s;
}
Brick::Brick(int x, int y, int s, int w, int h) {
	BrickCount++;
	PositionX = x;
	PositionY = y;
	Width = w;
	Height = h;
	Strength = s;
}
Brick::~Brick() {
	BrickCount--;
}
void Brick::Harm() {
	Harm(1);
}
void Brick::Harm(int Damage) {
	if(Strength > 0)
	{
		Strength -= Damage;
		if(Strength < 1) {
			BrickCount--;
			Live = false;
		}
	}
}

int Ball::BallCount = 0;
Ball::Ball() {
	BallCount++;
	Speed = 0;
	Angle = 45.0;
	PositionX = 40;
	PositionY = 448 - Height;
}
Ball::~Ball() {
	BallCount--;
}
void Ball::Render() {
	al_draw_filled_circle(PositionX + (Width / 2.0), PositionY + (Height / 2.0), Width / 2.0, Color);
}

Paddle::Paddle(int position) {
	FixedCoord = position;
	Rotated = false;
	Width = 64;
	Height = 8;
}
Paddle::Paddle(int position, bool vertical) {
	FixedCoord = position;
	Rotated = vertical;
	if(vertical) {
		Width = 8;
		Height = 64;
	}
	else {
		Width = 64;
		Height = 8;
	}
	
}
Paddle::~Paddle() {
	// Need to do something when a paddle leaves?
	// Put it here!
}
bool Paddle::IsVertical() {
	return Rotated;
}
bool Paddle::IsHorizontal() {
	return !Rotated;
}