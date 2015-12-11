#pragma once
#include "Scenegraph/GameObject.h"

class Maze : public GameObject
{
public:
	Maze();
	virtual ~Maze();

	virtual void Initialize(const GameContext& gameContext);
	virtual void Update(const GameContext& gameContext);

private:	

	// -------------------------
	// Disabling default copy constructor and default 
	// assignment operator.
	// -------------------------
	Maze(const Maze& yRef);
	Maze& operator=(const Maze& yRef);
};
