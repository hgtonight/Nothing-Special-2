#include "Ball.h"

int Ball::BallCount = 0;
Ball::Ball() {
	BallCount++;
	Speed = 500;
	Theta = -0.7;
	Position.X = 320;
	Position.Y = 240;
	Width = 6;
	Height = 6;
	Collides = true;
	CollisionDamage = 1;
}
Ball::~Ball() {
	BallCount--;
}
void Ball::Render() {
	al_draw_filled_circle(Position.X + (Width / 2.0), Position.Y + (Height / 2.0), Width / 2.0, Color);
}
// defaults to 1 / 60 of a second
Path Ball::GetPath(float DeltaT) {
	Path Result;
	// don't need to do anything if the ball is not moving	
	if (!Speed) {
		Result.X1 = Position.X;
		Result.X2 = Position.X;
		Result.Y1 = Position.Y;
		Result.Y2 = Position.Y;
		return Result;
	}
	// Update member variables
	Position.DeltaX = cos(Theta) * Speed;
	Position.DeltaY = sin(Theta) * Speed;

	Result.X1 = Position.X;
	Result.X2 = Position.X + (Position.DeltaX * DeltaT);
	Result.Y1 = Position.Y;
	Result.Y2 = Position.Y + (Position.DeltaY * DeltaT);
	
	// Reset hit status
	Hit = false;

	return Result;
	
}
void Ball::ReflectX() {
	Theta = -Theta;
}
void Ball::ReflectY() {
	if(Theta > 0) {
		Theta = ALLEGRO_PI - Theta;
	}
	else {
		Theta = -ALLEGRO_PI - Theta;
	}
}