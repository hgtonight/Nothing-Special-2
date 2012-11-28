#ifndef BRICK_H
#define BRICK_H
#include "BlockyEntity.h"

class Brick : public BlockyEntity {
private:
	int Strength;
public:
	static int BrickCount;
	Brick();
	Brick(int x, int y);
	Brick(int x, int y, int s);
	Brick(int x, int y, int s, int w, int h);
	~Brick();
	void Harm();
	void Harm(int Damage);
	void UpdateColor();
};
#endif