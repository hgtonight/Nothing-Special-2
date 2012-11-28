#ifndef BLOCKYENTITY_H
#define BLOCKYENTITY_H

#include <allegro5\allegro_primitives.h>
#include <allegro5\allegro_color.h>

class BlockyEntity {
protected:
public:
	int PositionX, PositionY, Width, Height;
	ALLEGRO_COLOR Color;
	bool Live;

	BlockyEntity();
	BlockyEntity(int x, int y, int w, int h, ALLEGRO_COLOR c);
	~BlockyEntity();
	// allow subclasses to override the default Render and Update functions
	virtual void Render();
	virtual void Update(float DeltaT);
	int LineIntersectLine(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4);
	bool Collides(int x, int y);
	bool Collides(int x, int y, int r);
	bool Collides(int x1, int y1, int x2, int y2);
	bool IsAlive();
	void SetPosition(int x, int y);
	void SetDimensions(int w, int h);
};
#endif