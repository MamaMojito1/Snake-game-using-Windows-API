#include "pch.h"
#include "Snake.h"
#include "Grid.h"
#include "Timer.h"

namespace CPPSnake
{
	Snake* _snake{};

	Bool Snake::initialize(const SnakeSettings & settings)
	{
		_settings = settings;
		if (_settings.moveTimeStep < 0.0f) _settings.moveTimeStep = 0.12f;
		if (_settings.minMoveTimeStep < 0.0f || _settings.minMoveTimeStep > _settings.moveTimeStep) _settings.minMoveTimeStep = _settings.moveTimeStep;

		_length = 1;
		_numFoodEaten = 0;

		_moveDirection = (Direction2D)(rand() % 4);
		_body.numItems = 1;
		if (!_body.capacity) _body.grow(20);
		_body[0].x = (Int32)((_grid->getNumCellX() - 1) * 0.5f);
		_body[0].y = (Int32)((_grid->getNumCellY() - 1) * 0.5f);

		_timeSinceLastMove = _timer->getCurrentTime();
		_moveTimeStep = _settings.moveTimeStep;
		_bitItsOwnTail = false;

		return true;
	}

	Void Snake::update()
	{
		Float currentTime = _timer->getCurrentTime();
		if ((currentTime - _timeSinceLastMove) >= _moveTimeStep)
		{
			_timeSinceLastMove = currentTime;

			_grid->setHasSnake(_body[_length - 1].x, _body[_length - 1].y, false);
			for (UInt32 i = _length - 1; i > 0; i--)
			{
				_body[i] = _body[i - 1];
				_grid->setHasSnake(_body[i].x, _body[i].y, true);
			}

			_grid->setHasSnake(_body[0].x, _body[0].y, false);
			switch (_moveDirection)
			{
			case Direction2D::Left:
				_body[0].x -= 1;
				if (_body[0].x < 0) _body[0].x = _grid->getNumCellX() - 1;
				break;

			case Direction2D::Right:
				_body[0].x += 1;
				if (_body[0].x == _grid->getNumCellX()) _body[0].x = 0;
				break;

			case Direction2D::Up:
				_body[0].y -= 1;
				if (_body[0].y < 0) _body[0].y = _grid->getNumCellY() - 1;
				break;

			case Direction2D::Down:
				_body[0].y += 1;
				if (_body[0].y == _grid->getNumCellY()) _body[0].y = 0;
				break;
			}
			if (_grid->hasSnake(_body[0].x, _body[0].y))
			{
				_bitItsOwnTail = true;
				return;
			}
			_grid->setHasSnake(_body[0].x, _body[0].y, true);

			if (_grid->hasFood(_body[0].x, _body[0].y))
			{
				++_length;
				++_numFoodEaten;

				const UInt32 numEatenFoodRef = 80;
				Float t = _numFoodEaten / (Float)numEatenFoodRef;
				if (t > 1.0f) t = 1.0f;

				_moveTimeStep = _settings.moveTimeStep + t * (_settings.minMoveTimeStep - _settings.moveTimeStep);

				if (_body.capacity < _length) _body.grow(_length - _body.capacity);
				_body.add({ _body[_length - 2].x, _body[_length - 2].y });

				if (_length != _grid->getTotalNumCells())
				{
					_grid->generateFood();
				}
			}
		}
	}

	Void Snake::draw()
	{
		Coord2I32 topLeft{};
		topLeft = _grid->calcCellTopLeft(_body[0].x, _body[0].y);
		_gfxDevice->DrawSquare(topLeft, _grid->getCellSize(), _settings.headColor);

		BGRA32 headColor = _settings.headColor;
		BGRA32 tailColor = _settings.tailColor;

		for (UInt32 i = 1; i < _length; i++)
		{
			Float t = (i + 1) / (Float)_length;
			topLeft = _grid->calcCellTopLeft(_body[i].x, _body[i].y);
			_gfxDevice->DrawSquare(topLeft, _grid->getCellSize(), 
				BGRA32::lerp(headColor, tailColor, t));
		}
	}

	Void Snake::SetMoveDirection(Direction2D moveDirection)
	{
		if (_moveDirection == moveDirection) return;

		if(_length > 1)
		{ 
			if (_moveDirection == Direction2D::Left && moveDirection == Direction2D::Right) return;
			if (_moveDirection == Direction2D::Right && moveDirection == Direction2D::Left) return;
			if (_moveDirection == Direction2D::Up && moveDirection == Direction2D::Down) return;
			if (_moveDirection == Direction2D::Down && moveDirection == Direction2D::Up) return;
		}

		_moveDirection = moveDirection;
	}
}