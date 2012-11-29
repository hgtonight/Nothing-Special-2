#include "BlockyEntity.h"

BlockyEntity::BlockyEntity() {
	Position.X = 1;
	Position.Y = 1;
	Width = 40;
	Height = 10;
	Color = al_color_name("white");
	Live = true;
	Hit = false;
}
BlockyEntity::BlockyEntity(float x, float y, float w, float h, ALLEGRO_COLOR c) {
	Position.X = x;
	Position.Y = y;
	Width = w;
	Height = h;
	Color = c;
	Live = true;
	Hit = false;
}
BlockyEntity::~BlockyEntity() {
	// end of life tasks
}
void BlockyEntity::Render() {
	al_draw_filled_rectangle(Position.X, Position.Y, Position.X + Width, Position.Y + Height, Color);
}
Path BlockyEntity::GetPath(float DeltaT) {
	Path Result;
	Result.X1 = Position.X;
	Result.X2 = Position.X;
	Result.Y1 = Position.Y;
	Result.Y2 = Position.Y;
	// Reset hit status
	Hit = false;

	return Result;
}

// determines if two lines intersect, and if so, where
// adapted from: http://paulbourke.net/geometry/pointlineplane/
Intersection BlockyEntity::LineIntersectLine(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4) {
	Intersection Result;
	float Denominator, NumeratorA, NumeratorB;

	Denominator = ( (y4 - y3) * (x2 - x1) ) - ( (x4 - x3) * (y2 - y1) );
	NumeratorA = ( (x4 - x3) * (y1 - y3) ) - ( (y4 - y3) * (x1 - x3) );
	NumeratorB = ( (x2 - x1) * (y1 - y3) ) - ( (y2 - y1) * (x1 - x3) );
	if(Denominator == 0.0f) {
		if(NumeratorA == 0.0f && NumeratorB == 0.0f) {
			// line segments are on top of each other
			Result.Type = Coincident;
		}
		// line segments are parallel
		Result.Type = Parallel;
	}
	else {
		float UnknownA = NumeratorA / Denominator;
		float UnknownB = NumeratorB / Denominator;

		// we only care if the line segments are intersecting
		if(UnknownA >= 0.0f && UnknownA <= 1.0f && UnknownB >= 0.0f && UnknownB <= 1.0f) {
			Result.Type = Intersecting;
			// line segments intersect at 
			Result.X = x1 + UnknownA * (x2 - x1);
			Result.Y = y1 + UnknownA * (y2 - y1);
		}
		// line segments do not intersect
		Result.Type = NotIntersecting;
	}
	return Result;
}
Intersection BlockyEntity::PathIntersect(Path SentPath) {
	Intersection Result;
	
	if(Position.DeltaY > 0.0f) {
		Result = LineIntersectLine(SentPath.X1, SentPath.Y1, SentPath.X2, SentPath.Y2, Position.X, Position.Y, Position.X + Width, Position.Y);
		Result.From = Top;
	}
	else {
		Result = LineIntersectLine(SentPath.X1, SentPath.Y1, SentPath.X2, SentPath.Y2, Position.X, Position.Y + Height, Position.X + Width, Position.Y + Height);
		Result.From = Bottom;
	}
	if(Position.DeltaX > 0.0f) {
		Result = LineIntersectLine(SentPath.X1, SentPath.Y1, SentPath.X2, SentPath.Y2, Position.X, Position.Y, Position.X, Position.Y + Height);
		Result.From = Left;
	}
	else {
		Result = LineIntersectLine(SentPath.X1, SentPath.Y1, SentPath.X2, SentPath.Y2, Position.X + Width, Position.Y, Position.X + Width, Position.Y + Height);
		Result.From = Right;
	}
	
	return Result;
}
bool BlockyEntity::IsAlive() {
	return Live;
}
bool BlockyEntity::HasBeenHit() {
	return Hit;
}
bool BlockyEntity::HasCollisionBox() {
	return Collides;
}
void BlockyEntity::SetPosition(float x, float y) {
	Position.X = x;
	Position.Y = y;
}
void BlockyEntity::SetDimensions(float w, float h) {
	Width = w;
	Height = h;
}