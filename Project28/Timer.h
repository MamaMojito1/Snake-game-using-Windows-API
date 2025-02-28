#pragma once
#include "Core.h"
#include <Windows.h>

namespace CPPSnake
{
	class Timer
	{
		friend class Application;

	private:

		Timer() {}
		~Timer() {}

		Bool initialize();

	public:

		Float getCurrentTime() const;

	private:

		Float _toSeconds{};
	};

	extern Timer* _timer;
}

