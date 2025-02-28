#pragma once
#include "Core.h"
#include <Windows.h>
#include "ApplicationWindow.h"

namespace CPPSnake
{
	class GfxDevice : public IApplicationWindowListener
	{
		friend class Application;
	public:

		GfxDevice() { };
		~GfxDevice();

		Bool initialize();

	public:

		Void clearColor(UInt32 color);
		Void present();

		Void DrawSquare(const Coord2I32& topLeft, UInt32 size, UInt32 color);
		Void drawTransparentQuad(const Coord2I32& topLeft, UInt32 width, UInt32 height, UInt32 color);

		UInt32 getBufferWidth() const { return _bufferWidth; }
		UInt32 getBufferHeight() const { return _bufferHeight; }
		UInt32* getColorBuffer() { return _colorBuffer; }
		HDC getBackbufferDC() const { return _backbufferDC; }

	private:

		Bool createBackBuffer();
		Void onAppWindowResized(const Size2UI32& newClientSize, Bool isMinimized);

	private:

		HBITMAP _backbuffer{};
		HBITMAP _backbufferOut{};
		HDC _backbufferDC{};
		HDC _wndClientDC{};

		UInt32* _colorBuffer{};
		UInt32 _bufferWidth{};
		UInt32 _bufferHeight{};
	};

	extern GfxDevice* _gfxDevice;
}

