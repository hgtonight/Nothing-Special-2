#include "Paddle.h"

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