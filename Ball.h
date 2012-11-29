#ifndef BALL_H
#define BALL_H
#include <cmath>
#include <vector>
#include "BlockyEntity.h"

class Ball : public BlockyEntity {
public:
	static int BallCount;
	float Speed, Theta;
	int CollisionDamage;
	std::vector<BlockyEntity> CollidesList;
	Ball();
	~Ball();
	Path GetPath(float DeltaT);
	void Render();
	void ReflectX();
	void ReflectY();
};
#endif