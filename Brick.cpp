#include "Brick.h"

int Brick::BrickCount = 0;
Brick::Brick() {
	BrickCount++;
	Strength = 1;
	Live = true;
	Collides = true;
	Hit = false;
	UpdateColor();
}
Brick::Brick(int x, int y) {
	BrickCount++;
	Position.X = x;
	Position.Y = y;
	Strength = 1;
	Live = true;
	Collides = true;
	Hit = false;
	UpdateColor();
}
Brick::Brick(int x, int y, int s) {
	BrickCount++;
	Position.X = x;
	Position.Y = y;
	Strength = s;
	Live = true;
	Collides = true;
	Hit = false;
	UpdateColor();
}
Brick::Brick(int x, int y, int s, int w, int h) {
	BrickCount++;
	Position.X = x;
	Position.Y = y;
	Width = w;
	Height = h;
	Strength = s;
	Live = true;
	Collides = true;
	Hit = false;
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