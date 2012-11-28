#include "Ball.h"

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
// defaults to 1 / 60 of a second
void Ball::Update(float DeltaT) {
	// don't need to do anything if the ball is not moving	
	if (!Speed) {
		if(Live) {
			// ball is live but not moving
		}
		else {
			// ball is dead and not moving
		}
		return;
	}

	// calculate the deltas for each axis using current member vars
	float DeltaX = cos(Theta) * Speed;
	float DeltaY = sin(Theta) * Speed;
	// distance traveled in axis directions in delta t
	float NX = DeltaX * DeltaT;
	float NY = DeltaY * DeltaT;
	// new unencombered position
	float NewPositionX = PositionX + NX;
	float NewPositionY = PositionY + NY;

	// look for collisions
	for(int i = 0 ; i < CollidesList.size() ; i++) {
		if (!item.hit) {
			point = Game.Math.ballIntercept(this, item, p2.nx, p2.ny);
			if (point) {
				mCurrent = Game.Math.magnitude(point.x - this.x, point.y - this.y);
				// nab the closest collision so we can handle multiple collisions
				if (mCurrent < mClosest) {
					
					mClosest = mCurrent;
					closest = {item: item, point: point};
				}
			}
		}
	}

	if (closest) {
		if (closest.item.isbrick) {
			this.game.hitBrick(closest.item);
			if (!this.moving) // if hitBrick caused game to end we dont want to continue updating our state
			return;
		}

		if ((closest.item == this.game.paddle) && (closest.point.d == 'top')) {
			p2.dx = this.speed * (closest.point.x - (this.game.paddle.left + this.game.paddle.w/2)) / (this.game.paddle.w/2);
			this.game.playSound('paddle');
		}

		this.setpos(closest.point.x, closest.point.y);

		switch(closest.point.d) {
			case 'left':
			case 'right':
				this.setdir(-p2.dx, p2.dy);
				break;

			case 'top':
			case 'bottom':
				this.setdir(p2.dx, -p2.dy);
				break;
		}

		var udt = dt * (mClosest / Game.Math.magnitude(p2.nx, p2.ny)); // how far along did we get before intercept ?
		return this.update(dt - udt); // so we can update for time remaining
	}

	if ((p2.x < 0) || (p2.y < 0) || (p2.x > this.game.width) || (p2.y > this.game.height)) {
		this.game.loseBall();
	}
	else {
		this.setpos(p2.x, p2.y);
		this.setdir(p2.dx, p2.dy);
	}


	// See if the new position would collide with any other entity

	// paddle collision
											
	// brick collision
	for(int j = 0; j < Bricks.size(); j++) {
		if(Balls[i].Collides(Bricks[j].PositionX, Bricks[j].PositionY, Bricks[j].PositionX + Bricks[j].Width, Bricks[j].PositionY + Bricks[j].Height) ) {
			// reflect and damage block
			Bricks[j].Harm();
		}
	}
	// move that ball
	Balls[i].PositionX += DeltaX;
	Balls[i].PositionY += DeltaY;
						
	// decide if ball is lost
	// ball is lost if it somehow gets off the screen
	if(Balls[i].PositionX >= 0 && Balls[i].PositionX + Balls[i].Width <= SCREEN_W && Balls[i].PositionY >= 0 && Balls[i].PositionY + Balls[i].Height <= SCREEN_W) {
		Balls[i].Live = false;
	}
	else {
		score += multi;
	}
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