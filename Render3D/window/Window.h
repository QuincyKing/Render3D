/*!
* @file Window.h
* @date 2017/05/13 18:38
*
* @author QuincyKing
* Contact: j88172365@gmail.com
*
* @brief Win32窗口及图形绘制
*/
#pragma once 

#include <windows.h>
#include <tchar.h>

#ifdef _MSC_VER
#pragma comment(lib, "gdi32.lib")
#pragma comment(lib, "user32.lib")
#endif

namespace Window3D
{
	class Window
	{
	public:
		Window();
		int Init(int w, int h, const TCHAR *title);
		int Close();
		void MsgLoop();
		void Update();

		/*!
		* @function Window::WndProc
		*
		* @brief 消息处理
		*
		* @return LRESULT
		*
		* @param HWND hWnd
		* @param UINT msg
		* @param WPARAM wParam
		* @param LPARAM lParam
		*/
		static LRESULT WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
		{
			switch (msg)
			{
			case WM_CLOSE:
				m_exit = 1; break;
			case WM_KEYDOWN:
				m_keys[wParam & 511] = 1; break;
			case WM_KEYUP:
				m_keys[wParam & 511] = 0; break;
			default:
				return DefWindowProc(hWnd, msg, wParam, lParam);
			}
			return 0;
		}

		unsigned char* GetFrameBuffer() { return m_fb; }
		static bool IsNotExit() { return m_exit != 0; }
		static bool IsKey(size_t _key) { return m_keys[_key] != 0; }

	private:
		int				m_width, m_height;
		HWND			m_handle;			// 主窗口 HWND
		HDC				m_dc;				// 配套的 HDC
		HBITMAP			m_hb;				// DIB
		HBITMAP			m_ob;				// 老的 BITMAP
		unsigned char*	m_fb;				// frame buffer
		static int		m_exit;
		static int		m_keys[512];		// 当前键盘按下状态
	};
}
