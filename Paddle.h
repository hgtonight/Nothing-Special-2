#ifndef PADDLE_H
#define PADDLE_H

#include "BlockyEntity.h"

class Paddle : public BlockyEntity {
private:
	bool Rotated;
	int FixedCoord;
public:
	Paddle(int position);
	Paddle(int position, bool vertical);
	~Paddle();
	bool IsVertical();
	bool IsHorizontal();
};
#endif