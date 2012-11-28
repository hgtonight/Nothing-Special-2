#ifndef BALL_H
#define BALL_H
#include <cmath>
#include <vector>
#include "BlockyEntity.h"

class Ball : public BlockyEntity {
public:
	static int BallCount;
	float Speed, Theta;
	std::vector<BlockyEntity> CollidesList;
	Ball();
	~Ball();
	void Update(float DeltaT);
	void Render();
	void ReflectX();
	void ReflectY();
};
#endif