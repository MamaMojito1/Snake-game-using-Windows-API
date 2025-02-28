#include "pch.h"
#include "GfxDevice.h"
#include "Application.h"
#include "ApplicationWindow.h"

namespace CPPSnake
{
	GfxDevice* _gfxDevice;

	GfxDevice::~GfxDevice()
	{
		_appWindow->removeListener(this);

		if (_backbuffer)
		{
			SelectObject(_backbufferDC, _backbufferOut);
			DeleteObject(_backbuffer);
		}

		if (_backbufferDC) DeleteDC(_backbufferDC);
	}

	Bool GfxDevice::initialize()
	{
		_bufferWidth = _appWindow->getClientWidth();
		_bufferHeight = _appWindow->getClientHeight();
		if (!createBackBuffer()) return false;

		_wndClientDC = _appWindow->getClientDC();
		_appWindow->addListener(this);

		return true;
	}


	Void GfxDevice::clearColor(UInt32 color)
	{
		if (!_colorBuffer)
		{
			MessageBox(NULL, "colorBuffer is Invalid", "Error", MB_OK);
		}
		__stosd((PDWORD)_colorBuffer, color, _bufferWidth * _bufferHeight);
	}

	Void GfxDevice::present()
	{
		BitBlt(_wndClientDC, 0, 0, _bufferWidth, _bufferHeight, _backbufferDC, 0, 0, SRCCOPY);
	}

	Void GfxDevice::DrawSquare(const Coord2I32 & topLeft, UInt32 size, UInt32 color)
	{
		Int32 left = topLeft.x;
		if (left >= (Int32)_bufferWidth) return;
		Int32 right = left + size - 1;
		if (right < 0) return;
		if (left < 0) left = 0;
		if (right >= (Int32)_bufferWidth) right = _bufferWidth - 1;
		Int32 top = topLeft.y;
		if (top >= (Int32)_bufferHeight) return;
		Int32 bottom = top + size - 1;
		if (bottom < 0) return;
		if (top < 0) top = 0;
		if (bottom >= (Int32)_bufferHeight) bottom = _bufferHeight;

		size = right - left + 1;
		for (Int32 y = top; y <= bottom; ++y)
		{
			__stosd((PDWORD)&_colorBuffer[left + y * _bufferWidth], color, size);
		}
	}

	Void GfxDevice::drawTransparentQuad(const Coord2I32 & topLeft, UInt32 width, UInt32 height, UInt32 color)
	{
		Int32 left = topLeft.x;
		if (left >= (Int32)_bufferWidth) return;
		Int32 right = left + width - 1;
		if (right < 0) return;
		if (left < 0) left = 0;
		if (right >= (Int32)_bufferWidth) right = _bufferWidth - 1;
		Int32 top = topLeft.y;
		if (top >= (Int32)_bufferHeight) return;
		Int32 bottom = top + height - 1;
		if (bottom < 0) return;
		if (top < 0) top = 0;
		if (bottom >= (Int32)_bufferHeight) bottom = _bufferHeight;

		BGRA32 srcColor = color;
		BGRA32 destColor{};

		Float t = 1.0f - (Byte)(color >> 24) / 255.0f;
		for (Int32 y = top; y <= bottom; y++)
		{
			for (Int32 x = left; x <= right; x++)
			{
				destColor = _colorBuffer[x + y * _bufferWidth];
				_colorBuffer[x + y * _bufferWidth] = BGRA32::lerp(srcColor, destColor, t);
			}
		}
	}

	Bool GfxDevice::createBackBuffer()
	{
		if (_backbuffer)
		{
			SelectObject(_backbufferDC, _backbufferOut);
			DeleteObject(_backbuffer);
		}

		if (!_backbufferDC)
		{
			_backbufferDC = CreateCompatibleDC(_wndClientDC);
			SetBkMode(_backbufferDC, TRANSPARENT);
		}
		if (!_backbufferDC) return false;

		BITMAPINFO bmpInfo{};
		bmpInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
		bmpInfo.bmiHeader.biWidth = (LONG)_bufferWidth;
		bmpInfo.bmiHeader.biHeight = -(LONG)_bufferHeight;
		bmpInfo.bmiHeader.biPlanes = 1;
		bmpInfo.bmiHeader.biBitCount = 32;
		_backbuffer = CreateDIBSection(_backbufferDC, &bmpInfo, DIB_RGB_COLORS, (void**)&_colorBuffer, NULL, 0);
		if (!_backbuffer || !_colorBuffer)
		{
			MessageBox(NULL, "Failed to create backbuffer!", "Error", MB_OK);
			return false;
		}

		//std::cout << "Buffer Width: " << _bufferWidth << ", Buffer Height: " << _bufferHeight << std::endl;


		_backbufferOut = (HBITMAP)SelectObject(_backbufferDC, _backbuffer);
		return true;

	}

	Void GfxDevice::onAppWindowResized(const Size2UI32& newClientSize, Bool isMinimized)
	{
		if (!isMinimized)
		{
			_bufferWidth = newClientSize.width;
			_bufferHeight = newClientSize.height;
			if (!createBackBuffer())
			{
				_app->quit();
				return;
			}
		}
		else
		{
			_bufferWidth = 0;
			_bufferHeight = 0;
		}
	}

}