
/*---------------------------------------------------------------------------------
【Cpp文件】:ScreenMain.cpp          Created by Beyond Ray,2014年4月
（描述）：使用天下叁CG《墨龙纪》制作成简单的屏保
-----------------------------------------------------------------------------------*/


//------------------------【头文件】--------------------------------
#include <windows.h>
#include <tchar.h>								//使用swprintf_s函数所需的头文件
#include "resource.h"							//资源文件（只用于图标）

//------------------------【全局变量】------------------------------
INT			g_nOrigin_X = -1, g_nOrigin_Y = -1;	//鼠标指针起始位置
TCHAR		g_buf[256];							//接收《墨龙纪》返回信息的缓冲区

//------------------------【全局函数声明】--------------------------
LRESULT CALLBACK	WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);			//主窗口过程函数
LRESULT CALLBACK SmallWndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);		//预览窗口过程函数


//-----------------------------------【WinMain( )函数】-----------------------------------------
//	Desc：Windows应用程序的主入口
//----------------------------------------------------------------------------------------------
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	HWND hwndFrameWnd = 0;
	HWND hwnd = 0;
	RECT rect;

	//---------------------------【设计一个窗口类】------------------------------
	WNDCLASSEX wndClass = { 0 };					//用WINDCLASSEX定义一个窗口类,并初始化
	wndClass.cbSize = sizeof(WNDCLASSEX);			//设置结构体的字节数大小
	wndClass.style = CS_HREDRAW | CS_VREDRAW;		//设置窗口的样式
	wndClass.lpfnWndProc = WndProc;					//设置指向窗口过程函数的指针
	wndClass.cbClsExtra = 0;						//窗口类的附加内存，取0就可以了
	wndClass.cbWndExtra = 0;						//窗口的附加内存，依然取0就行了
	wndClass.hInstance = hInstance;					//指定包含窗口过程的程序的实例句柄。
	wndClass.hIcon = (HICON)LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));    //本地加载自定义ico图标
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW); //指定窗口类的光标句柄。
	wndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);  //指定一个白色画刷句柄	
	wndClass.lpszMenuName = NULL;					//用一个以空终止的字符串，指定菜单资源的名字。
	wndClass.lpszClassName = L"BeyondScrnSaver";	//用一个以空终止的字符串，指定窗口类的名字。

	if (__argc > 1)
	{
		if (strstr(__argv[1], "/p"))	//预览小窗口
		{
			if (__argc == 3)
			{
				hwndFrameWnd = (HWND)atoi(__argv[2]);

				GetClientRect(hwndFrameWnd, &rect);

				wndClass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);  //预览窗口的黑色背景刚好协调
				wndClass.lpfnWndProc = SmallWndProc;						   //为窗口类重新指定窗口函数
				//----------------------------【注册一个窗口类】------------------------------
				if (!RegisterClassEx(&wndClass))
					return -1;

				hwnd = CreateWindow(L"BeyondScrnSaver", NULL,
					WS_CHILD | WS_VISIBLE,
					rect.left, rect.top,
					rect.right, rect.bottom,
					hwndFrameWnd, NULL,
					hInstance, NULL);

				//----------------------------【播放《墨龙纪》视频】--------------------------
				//打开墨龙纪视频
				wchar_t open1[100];
				swprintf_s(open1, L"open F:/天下叁CG/墨龙纪.avi type MPEGVideo Alias movie parent %u Style %u notify", hwnd, WS_CHILD);
				mciSendString(open1, g_buf, sizeof(g_buf), NULL);
				//设置窗口大小并播放
				swprintf_s(open1, L"put movie window at 0 0 %d %d", rect.right - rect.left, rect.bottom - rect.top);
				mciSendString(open1, g_buf, sizeof(g_buf), NULL);
				mciSendString(L"play movie repeat", g_buf, sizeof(g_buf), NULL);

				//------------------------------【消息循环过程】-------------------------------
				MSG msg = { 0 };							//定义并初始化msg
				while (GetMessage(&msg, NULL, 0, 0))		//使用while循环，如果消息不是WM_QUIT消息，就继续循环
				{
					TranslateMessage(&msg);			//将虚拟键消息转换为字符消息
					DispatchMessage(&msg);			//分发一个消息给窗口程序。
				}
				return 0;
			}
		}
		else if (strchr(__argv[1], 'c')) //设置选项卡
		{
			MessageBox(NULL, L"This screen saver doesn't have configuration", L"Blank Screen Saver", MB_OK);
			return 0;
		}
	}

	//----------------------------【注册一个窗口类】------------------------------
	if (!RegisterClassEx(&wndClass))
		return -1;

	//-----------------------------【创建一个窗口】-------------------------------
	hwnd = CreateWindow(L"BeyondScrnSaver", NULL,					//注意这里初始化HWND_DESKTOP,并且在最初大小为0，
			WS_POPUP | WS_VISIBLE, CW_USEDEFAULT, CW_USEDEFAULT, 0, //以防止视频还没播放前看到白屏
			0, HWND_DESKTOP, NULL, hInstance, NULL);

	//播放《墨龙纪》视频(我将视频放在了 F:/天下叁CG/墨龙纪.avi路径当中，注意调整)
	wchar_t open1[100];
	swprintf_s(open1, L"open F:/天下叁CG/墨龙纪.avi type MPEGVideo Alias movie parent %u Style %u notify", hwnd, WS_CHILD);
	mciSendString(open1, g_buf, sizeof(g_buf), NULL);
	mciSendString(TEXT("play movie fullscreen repeat"), g_buf, sizeof(g_buf), NULL);
	
	//视频播放出来了，可以把窗口显示出来了
	MoveWindow(hwnd, 0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN), true);
	ShowWindow(hwnd, nShowCmd);				//调用ShowWindow函数来显示窗口
	UpdateWindow(hwnd);						//对窗口进行更新

	//------------------------------【消息循环过程】-------------------------------
	MSG msg = { 0 };							//定义并初始化msg
	while (GetMessage(&msg, NULL, 0, 0))		//使用while循环，如果消息不是WM_QUIT消息，就继续循环
	{
		TranslateMessage(&msg);			//将虚拟键消息转换为字符消息
		DispatchMessage(&msg);			//分发一个消息给窗口程序。

		if (msg.message == WM_MOUSEMOVE || WM_SYSKEYDOWN || WM_KEYDOWN ||
			WM_LBUTTONDOWN || WM_RBUTTONDOWN || WM_MBUTTONDOWN)	//由于系统默认将消息传递给窗口上层的视频，
		{														//而窗口接受不到消息，所以我们手动进行传送。
			SendMessage(hwnd, msg.message, msg.wParam, msg.lParam);
		}
	}


	//-------------------------------【将窗口类注销】-------------------------------
	UnregisterClass(L"BeyondScrnSaver", wndClass.hInstance);  //程序准备结束，注销窗口类
	return 0;
}

//-----------------------------------【WndProc( )函数】-------------------------------------------
// Desc：窗口过程函数WndProc,对窗口消息进行处理
//------------------------------------------------------------------------------------------------
LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{

	switch (message)						//switch语句开始
	{
	case WM_CREATE:
		ShowCursor(false);
		break;

	case WM_LBUTTONDOWN:
	case WM_MBUTTONDOWN:
	case WM_RBUTTONDOWN:
	case WM_KEYDOWN:
	case WM_SYSKEYDOWN:
		MoveWindow(hwnd, 0, 0, 0, 0, true);		//关闭窗口速度比视频退出慢，先使其不可见，以防止看到白屏
		PostQuitMessage(0);						// 退出窗口程序
		mciSendString(L"close movie", g_buf, sizeof(g_buf), NULL);	//关闭墨龙纪视频
		break; 								

	case WM_MOUSEMOVE:
	{
		int nNew_X = LOWORD(lParam);
		int nNew_Y = HIWORD(lParam);
		if (g_nOrigin_X == -1 && g_nOrigin_Y == -1)		//设置鼠标指针初始点坐标
		{
			g_nOrigin_X = nNew_X;
			g_nOrigin_Y = nNew_Y;
		}
		else if (g_nOrigin_X != nNew_X  &&  g_nOrigin_Y != nNew_Y)	//判断鼠标移动检测
		{
			MoveWindow(hwnd, 0, 0, 0, 0, true);
			mciSendString(L"close movie", g_buf, sizeof(g_buf), NULL);
			PostQuitMessage(0);
		}
	}
		break;

	case WM_DESTROY:			//窗口销毁消息
		PostQuitMessage(0);			
		break;									

	default:										
		return DefWindowProc(hwnd, message, wParam, lParam);		//调用缺省的窗口过程
	}

	return 0;			//正常退出
}

//-----------------------------------【SmallWndProc( )函数】-------------------------------------------
// Desc：预览窗口过程函数,对预览窗口消息进行处理
//-----------------------------------------------------------------------------------------------------
LRESULT CALLBACK SmallWndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)						//switch语句开始
	{
	case WM_DESTROY:			//窗口销毁消息
		PostQuitMessage(0);
		break;

	default:
		return DefWindowProc(hwnd, message, wParam, lParam);		//调用缺省的窗口过程
	}

	return 0;			//正常退出
}