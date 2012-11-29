#ifndef BLOCKYENTITY_H
#define BLOCKYENTITY_H

#include <allegro5\allegro_primitives.h>
#include <allegro5\allegro_color.h>
#include <allegro5\allegro.h>

enum Direction { Top, Bottom, Left, Right };
enum IntersectType { Parallel, Coincident, NotIntersecting, Intersecting };

struct Path {
	float X1, X2, Y1, Y2;
};

struct PositionType {
	float X, Y, DeltaX, DeltaY;
};

struct Intersection {
	float X, Y;
	Direction From;
	IntersectType Type;
};



class BlockyEntity {
protected:
public:
	float Width, Height;
	PositionType Position;
	ALLEGRO_COLOR Color;
	bool Live, Collides, Hit;

	BlockyEntity();
	BlockyEntity(float x, float y, float w, float h, ALLEGRO_COLOR c);
	~BlockyEntity();
	// allow subclasses to override the default Render and Update functions
	virtual void Render();
	virtual Path GetPath(float DeltaT);
	Intersection LineIntersectLine(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4);
	Intersection PathIntersect(Path SentPath);
	bool IsAlive();
	bool HasBeenHit();
	bool HasCollisionBox();
	void SetPosition(float x, float y);
	void SetDimensions(float w, float h);
};

struct ClosestEntity {
	int EntityIndex;
	Intersection CollisionPoint;
	float Magnitude;
};
#endif