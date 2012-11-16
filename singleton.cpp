#include "singleton.h"

BlockyEntity::BlockyEntity() {
	PositionX = 1;
	PositionY = 1;
	Width = 40;
	Height = 10;
	Color = al_color_name("white");
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
	// check if completely above or below
	if (PositionY + Height < y1 || PositionY > y2) {
		return false;
	}
	// check if completely on the left or right
	if(PositionX + Width < x1 || PositionX > x2) {
		return false;
	}
	
	// we know there is a collision!
	return true;
}
bool BlockyEntity::IsAlive() {
	return Live;
}
void BlockyEntity::SetPosition(int x, int y) {
	PositionX = x;
	PositionY = y;
}
void BlockyEntity::SetDimensions(int w, int h) {
	Width = w;
	Height = h;
}

int Brick::BrickCount = 0;
Brick::Brick() {
	BrickCount++;
	Strength = 1;
	UpdateColor();
}
Brick::Brick(int x, int y) {
	BrickCount++;
	PositionX = x;
	PositionY = y;
	Strength = 1;
	UpdateColor();
}
Brick::Brick(int x, int y, int s) {
	BrickCount++;
	PositionX = x;
	PositionY = y;
	Strength = s;
	UpdateColor();
}
Brick::Brick(int x, int y, int s, int w, int h) {
	BrickCount++;
	PositionX = x;
	PositionY = y;
	Width = w;
	Height = h;
	Strength = s;
	UpdateColor();
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
		UpdateColor();
	}
}
void Brick::UpdateColor() {
	switch(Strength) {
	default:
	case 0:
		Color = al_color_name("darkgray");
		break;
	case 1:
		Color = al_color_name("red");
		break;
	case 2:
		Color = al_color_name("orange");
		break;
	case 3:
		Color = al_color_name("yellow");
		break;
	case 4:
		Color = al_color_name("green");
		break;
	case 5:
		Color = al_color_name("blue");
		break;
	case 6:
		Color = al_color_name("indigo");
		break;
	case 7:
		Color = al_color_name("violet");
		break;
	}
}

int Ball::BallCount = 0;
Ball::Ball() {
	BallCount++;
	Speed = 10;
	Theta = -0.7;
	PositionX = 320;
	PositionY = 240;
	Width = 6;
	Height = 6;
}
Ball::~Ball() {
	BallCount--;
}
void Ball::Render() {
	al_draw_filled_circle(PositionX + (Width / 2.0), PositionY + (Height / 2.0), Width / 2.0, Color);
}
void Ball::ReflectX() {
	Theta = -Theta;
}
void Ball::ReflectY() {
	if(Theta > 0) {
		Theta = 3.14159265358979323846 - Theta;
	}
	else {
		Theta = -3.14159265358979323846 - Theta;
	}
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