#include "BlockyEntity.h"

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
void BlockyEntity::Update(float DeltaT) {
	// Insert default update logic here
	// do nothing
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