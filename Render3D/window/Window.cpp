/*!
* @file Window.cpp
* @date 2017/05/14 16:23
*
* @author QuincyKing
* Contact: j88172365@gmail.com
*
* @brief ������
*/
#include "Window.h"

namespace Window3D
{
	int Window::m_keys[512] = { 0 };
	int Window::m_exit = 0;

	Window::Window()
	{
		m_handle = NULL; m_hb = NULL; m_dc = NULL; m_ob = NULL; m_fb = NULL;
	}

	/*!
	* @function Window::Init
	*
	* @brief ��ʼ������
	*
	* @return int
	*
	* @param int w
	* @param int h
	* @param const TCHAR * title
	*/
	int Window::Init(int w, int h, const TCHAR *title)
	{
		WNDCLASS wc = { CS_BYTEALIGNCLIENT, (WNDPROC)WndProc, 0, 0, 0,
			NULL, NULL, NULL, NULL, _T("Render") };
		BITMAPINFO bi = { { sizeof(BITMAPINFOHEADER), w, -h, 1, 32, BI_RGB,
			w * h * 4, 0, 0, 0, 0 } };
		RECT rect = { 0, 0, w, h };
		int wx, wy, sx, sy;
		LPVOID ptr;
		HDC hDC;

		Close();

		wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
		wc.hInstance = GetModuleHandle(NULL);
		wc.hCursor = LoadCursor(NULL, IDC_ARROW);
		if (!RegisterClass(&wc)) return -1;

		m_handle = CreateWindow(_T("Render"), title,
			WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
			0, 0, 0, 0, NULL, NULL, wc.hInstance, NULL);
		if (m_handle == NULL) return -2;

		m_exit = 0;
		hDC = GetDC(m_handle);
		m_dc = CreateCompatibleDC(hDC);
		ReleaseDC(m_handle, hDC);

		//����Ӧ�ó������ֱ��д��ġ����豸�޹ص�λͼ��DIB��
		m_hb = CreateDIBSection(m_dc, &bi, DIB_RGB_COLORS, &ptr, 0, 0);
		if (m_hb == NULL) return -3;

		//��һ������(λͼ�����ʡ���ˢ��)ѡ��ָ�����豸�������µĶ������ͬһ���͵��϶���
		m_ob = (HBITMAP)SelectObject(m_dc, m_hb);
		m_fb = (unsigned char*)ptr;
		m_width = w;
		m_height = h;

		//����ͻ����εĴ�С��������Ҫ�Ĵ��ھ��εĴ�С
		AdjustWindowRect(&rect, GetWindowLong(m_handle, GWL_STYLE), 0);
		wx = rect.right - rect.left;
		wy = rect.bottom - rect.top;
		//��ȡ��Ļ���
		sx = (GetSystemMetrics(SM_CXSCREEN) - wx) / 2;
		//��ȡ��Ļ�߶�
		sy = (GetSystemMetrics(SM_CYSCREEN) - wy) / 2;
		if (sy < 0) sy = 0;
		SetWindowPos(m_handle, NULL, sx, sy, wx, wy,
			(SWP_NOCOPYBITS | SWP_NOZORDER | SWP_SHOWWINDOW));
		//������ָ�����ڵ��߳����õ�ǰ̨�����Ҽ���ô���
		SetForegroundWindow(m_handle);

		ShowWindow(m_handle, SW_NORMAL);
		MsgLoop();

		memset(m_fb, 0, w * h * 4);

		return 0;
	}

	/*!
	* @function Window::Close
	*
	* @brief �رմ���
	*
	* @return int
	*
	*/
	int Window::Close()
	{
		if (m_dc)
		{
			if (m_ob)
			{
				SelectObject(m_dc, m_ob);
				m_ob = NULL;
			}
			DeleteDC(m_dc);
			m_dc = NULL;
		}
		if (m_hb)
		{
			DeleteObject(m_hb);
			m_hb = NULL;
		}
		if (m_handle)
		{
			CloseWindow(m_handle);
			m_handle = NULL;
		}
		return 0;
	}

	/*!
	* @function Window::MsgLoop
	*
	* @brief ��Ϣѭ��
	*
	* @return void
	*
	*/
	void Window::MsgLoop()
	{
		MSG msg;
		while (true)
		{
			if (!PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE))
				break;
			if (!GetMessage(&msg, NULL, 0, 0))
				break;
			DispatchMessage(&msg);
		}
	}

	/*!
	* @function Window::Update
	*
	* @brief ���´���
	*
	* @return void
	*
	*/
	void Window::Update()
	{
		HDC hDC = GetDC(m_handle);
		//�ú�����ָ����Դ�豸���������е����ؽ���λ�飨bit_block��ת�����Դ��͵�Ŀ���豸����
		BitBlt(hDC, 0, 0, m_width, m_height, m_dc, 0, 0, SRCCOPY);
		ReleaseDC(m_handle, hDC);
		MsgLoop();
	}
}
