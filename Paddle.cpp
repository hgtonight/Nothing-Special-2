#include "Paddle.h"

Paddle::Paddle(int position) {
	Rotated = false;
	Width = 64;
	Height = 8;

	Position.X = 320;
	Position.Y = position;
}
Paddle::Paddle(int position, bool vertical) {
	Rotated = vertical;
	Position.X = 320;
	Position.Y = 320;
	
	if(vertical) {
		Width = 8;
		Height = 64;
		Position.X = position;
	}
	else {
		Width = 64;
		Height = 8;
		Position.Y = position;
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