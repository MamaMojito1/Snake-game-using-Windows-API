#include "Pch.h"
#include "Grid.h"
#include "ApplicationWindow.h"
#include "Snake.h"

namespace CPPSnake
{
	Grid* _grid{};

	Bool Grid::initialize(const GridSettings& settings)
	{
		_settings = settings;
		if (_settings.cellSize < 8) _settings.cellSize = 8;

		_numCellsX = (_appWindow->getClientWidth() - 1) / _settings.cellSize;
		_numCellsY = (_appWindow->getClientHeight() - 1) / _settings.cellSize;

		if (!_settings.maxNumCellsX) _settings.maxNumCellsX = 100;
		if (!_settings.maxNumCellsY) _settings.maxNumCellsY = 100;

		if (_numCellsX > _settings.maxNumCellsX) _numCellsX = _settings.maxNumCellsX;
		if (_numCellsY > _settings.maxNumCellsY) _numCellsY = _settings.maxNumCellsY;

		UInt32 remSpace = _appWindow->getClientWidth() - _numCellsX * _settings.cellSize;
		_topLeft.x = (UInt32)(remSpace * 0.5f);
		remSpace = _appWindow->getClientHeight() - _numCellsY * _settings.cellSize;
		_topLeft.y = (UInt32)(remSpace * 0.5f);

		_cells.numItems = getTotalNumCells();
		if (_cells.capacity < _cells.numItems)
			_cells.grow(_cells.numItems - _cells.capacity);

		for (UInt32 i = 0; i < _cells.numItems; ++i)
			_cells[i].flags = 0;

		generateFood();

		return true;
	}

	Void Grid::draw()
	{
		UInt32* colorBuffer = _gfxDevice->getColorBuffer();
		UInt32 bufferWidth = _gfxDevice->getBufferWidth();

		Coord2I32 foodTopLeft = calcCellTopLeft((UInt32)_foodCoords.x, (UInt32)_foodCoords.y);
		_gfxDevice->DrawSquare(foodTopLeft, _settings.cellSize, _settings.foodColor);

		UInt32 numLines = _numCellsX + 1;
		UInt32 lineLength = _numCellsY * _settings.cellSize;
		for (UInt32 lineIndex = 0; lineIndex < numLines; ++lineIndex)
		{
			UInt32 x = lineIndex * _settings.cellSize + _topLeft.x;
			for (UInt32 y = 0; y < lineLength; ++y)
				colorBuffer[(y + _topLeft.y) * bufferWidth + x] = _settings.lineColor;
		}

		numLines = _numCellsY + 1;
		lineLength = _numCellsX * _settings.cellSize;
		for (UInt32 lineIndex = 0; lineIndex < numLines; ++lineIndex)
			__stosd((PDWORD)&colorBuffer[(_topLeft.y + lineIndex * _settings.cellSize) * bufferWidth + _topLeft.x], _settings.lineColor, lineLength);
	}


	Void Grid::generateFood()
	{
		_cells[_foodCoords.x + _foodCoords.y * _numCellsX].flags &= ~(UInt32)CellFlags::HasFood;
		_foodCoords.x = rand() % _numCellsX;
		_foodCoords.y = rand() % _numCellsY;
		_cells[_foodCoords.x + _foodCoords.y * _numCellsX].flags |= (UInt32)CellFlags::HasFood;
		
		if (hasSnake(_foodCoords.x, _foodCoords.y))
		{
			const UInt32 maxNumIters = 5;
			UInt32 iterIndex{};
			while(iterIndex < maxNumIters && hasSnake(_foodCoords.x, _foodCoords.y))
			{
				_cells[_foodCoords.x + _foodCoords.y * _numCellsX].flags &= ~(UInt32)CellFlags::HasFood;
				_foodCoords.x = rand() % _numCellsX;
				_foodCoords.y = rand() % _numCellsY;
				_cells[_foodCoords.x + _foodCoords.y * _numCellsX].flags |= (UInt32)CellFlags::HasFood;


				++iterIndex;
			}
		}
	}
}