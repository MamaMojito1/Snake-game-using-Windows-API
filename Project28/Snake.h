#pragma once

#include "Core.h"

namespace CPPSnake
{
	struct SnakeSettings
	{
		UInt32 headColor = 0x0000FF00;
		UInt32 tailColor = 0x00008800;
		Float moveTimeStep = 0.12f;
		Float minMoveTimeStep = 0.033f;
	};

	class Snake
	{
		friend class Application;
	private:

		Snake() {}
		~Snake() {}

		Bool initialize(const SnakeSettings& settings);
		Void update();
		Void draw();

	public:

		Void SetMoveDirection(Direction2D moveDirection);

		UInt32 getNumFoodEaten() const { return _numFoodEaten; }
		Bool bitItsOwnTail() const { return _bitItsOwnTail; }

	private:

		SnakeSettings _settings{};
		Float _moveTimeStep{};
		Float _timeSinceLastMove{};
		Bool _bitItsOwnTail{};
		Direction2D _moveDirection;
		UInt32 _length{};
		UInt32 _numFoodEaten{};
		MallocBuffer<Coord2I32> _body{};
	};

	extern Snake* _snake;
}
