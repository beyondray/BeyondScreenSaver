
/*---------------------------------------------------------------------------------
��Cpp�ļ���:ScreenMain.cpp          Created by Beyond Ray,2014��4��
����������ʹ��������CG��ī���͡������ɼ򵥵�����
-----------------------------------------------------------------------------------*/


//------------------------��ͷ�ļ���--------------------------------
#include <windows.h>
#include <tchar.h>								//ʹ��swprintf_s���������ͷ�ļ�
#include "resource.h"							//��Դ�ļ���ֻ����ͼ�꣩

//------------------------��ȫ�ֱ�����------------------------------
INT			g_nOrigin_X = -1, g_nOrigin_Y = -1;	//���ָ����ʼλ��
TCHAR		g_buf[256];							//���ա�ī���͡�������Ϣ�Ļ�����

//------------------------��ȫ�ֺ���������--------------------------
LRESULT CALLBACK	WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);			//�����ڹ��̺���
LRESULT CALLBACK SmallWndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);		//Ԥ�����ڹ��̺���


//-----------------------------------��WinMain( )������-----------------------------------------
//	Desc��WindowsӦ�ó���������
//----------------------------------------------------------------------------------------------
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	HWND hwndFrameWnd = 0;
	HWND hwnd = 0;
	RECT rect;

	//---------------------------�����һ�������ࡿ------------------------------
	WNDCLASSEX wndClass = { 0 };					//��WINDCLASSEX����һ��������,����ʼ��
	wndClass.cbSize = sizeof(WNDCLASSEX);			//���ýṹ����ֽ�����С
	wndClass.style = CS_HREDRAW | CS_VREDRAW;		//���ô��ڵ���ʽ
	wndClass.lpfnWndProc = WndProc;					//����ָ�򴰿ڹ��̺�����ָ��
	wndClass.cbClsExtra = 0;						//������ĸ����ڴ棬ȡ0�Ϳ�����
	wndClass.cbWndExtra = 0;						//���ڵĸ����ڴ棬��Ȼȡ0������
	wndClass.hInstance = hInstance;					//ָ���������ڹ��̵ĳ����ʵ�������
	wndClass.hIcon = (HICON)LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));    //���ؼ����Զ���icoͼ��
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW); //ָ��������Ĺ������
	wndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);  //ָ��һ����ɫ��ˢ���	
	wndClass.lpszMenuName = NULL;					//��һ���Կ���ֹ���ַ�����ָ���˵���Դ�����֡�
	wndClass.lpszClassName = L"BeyondScrnSaver";	//��һ���Կ���ֹ���ַ�����ָ������������֡�

	if (__argc > 1)
	{
		if (strstr(__argv[1], "/p"))	//Ԥ��С����
		{
			if (__argc == 3)
			{
				hwndFrameWnd = (HWND)atoi(__argv[2]);

				GetClientRect(hwndFrameWnd, &rect);

				wndClass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);  //Ԥ�����ڵĺ�ɫ�����պ�Э��
				wndClass.lpfnWndProc = SmallWndProc;						   //Ϊ����������ָ�����ں���
				//----------------------------��ע��һ�������ࡿ------------------------------
				if (!RegisterClassEx(&wndClass))
					return -1;

				hwnd = CreateWindow(L"BeyondScrnSaver", NULL,
					WS_CHILD | WS_VISIBLE,
					rect.left, rect.top,
					rect.right, rect.bottom,
					hwndFrameWnd, NULL,
					hInstance, NULL);

				//----------------------------�����š�ī���͡���Ƶ��--------------------------
				//��ī������Ƶ
				wchar_t open1[100];
				swprintf_s(open1, L"open F:/������CG/ī����.avi type MPEGVideo Alias movie parent %u Style %u notify", hwnd, WS_CHILD);
				mciSendString(open1, g_buf, sizeof(g_buf), NULL);
				//���ô��ڴ�С������
				swprintf_s(open1, L"put movie window at 0 0 %d %d", rect.right - rect.left, rect.bottom - rect.top);
				mciSendString(open1, g_buf, sizeof(g_buf), NULL);
				mciSendString(L"play movie repeat", g_buf, sizeof(g_buf), NULL);

				//------------------------------����Ϣѭ�����̡�-------------------------------
				MSG msg = { 0 };							//���岢��ʼ��msg
				while (GetMessage(&msg, NULL, 0, 0))		//ʹ��whileѭ���������Ϣ����WM_QUIT��Ϣ���ͼ���ѭ��
				{
					TranslateMessage(&msg);			//���������Ϣת��Ϊ�ַ���Ϣ
					DispatchMessage(&msg);			//�ַ�һ����Ϣ�����ڳ���
				}
				return 0;
			}
		}
		else if (strchr(__argv[1], 'c')) //����ѡ�
		{
			MessageBox(NULL, L"This screen saver doesn't have configuration", L"Blank Screen Saver", MB_OK);
			return 0;
		}
	}

	//----------------------------��ע��һ�������ࡿ------------------------------
	if (!RegisterClassEx(&wndClass))
		return -1;

	//-----------------------------������һ�����ڡ�-------------------------------
	hwnd = CreateWindow(L"BeyondScrnSaver", NULL,					//ע�������ʼ��HWND_DESKTOP,�����������СΪ0��
			WS_POPUP | WS_VISIBLE, CW_USEDEFAULT, CW_USEDEFAULT, 0, //�Է�ֹ��Ƶ��û����ǰ��������
			0, HWND_DESKTOP, NULL, hInstance, NULL);

	//���š�ī���͡���Ƶ(�ҽ���Ƶ������ F:/������CG/ī����.avi·�����У�ע�����)
	wchar_t open1[100];
	swprintf_s(open1, L"open F:/������CG/ī����.avi type MPEGVideo Alias movie parent %u Style %u notify", hwnd, WS_CHILD);
	mciSendString(open1, g_buf, sizeof(g_buf), NULL);
	mciSendString(TEXT("play movie fullscreen repeat"), g_buf, sizeof(g_buf), NULL);
	
	//��Ƶ���ų����ˣ����԰Ѵ�����ʾ������
	MoveWindow(hwnd, 0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN), true);
	ShowWindow(hwnd, nShowCmd);				//����ShowWindow��������ʾ����
	UpdateWindow(hwnd);						//�Դ��ڽ��и���

	//------------------------------����Ϣѭ�����̡�-------------------------------
	MSG msg = { 0 };							//���岢��ʼ��msg
	while (GetMessage(&msg, NULL, 0, 0))		//ʹ��whileѭ���������Ϣ����WM_QUIT��Ϣ���ͼ���ѭ��
	{
		TranslateMessage(&msg);			//���������Ϣת��Ϊ�ַ���Ϣ
		DispatchMessage(&msg);			//�ַ�һ����Ϣ�����ڳ���

		if (msg.message == WM_MOUSEMOVE || WM_SYSKEYDOWN || WM_KEYDOWN ||
			WM_LBUTTONDOWN || WM_RBUTTONDOWN || WM_MBUTTONDOWN)	//����ϵͳĬ�Ͻ���Ϣ���ݸ������ϲ����Ƶ��
		{														//�����ڽ��ܲ�����Ϣ�����������ֶ����д��͡�
			SendMessage(hwnd, msg.message, msg.wParam, msg.lParam);
		}
	}


	//-------------------------------����������ע����-------------------------------
	UnregisterClass(L"BeyondScrnSaver", wndClass.hInstance);  //����׼��������ע��������
	return 0;
}

//-----------------------------------��WndProc( )������-------------------------------------------
// Desc�����ڹ��̺���WndProc,�Դ�����Ϣ���д���
//------------------------------------------------------------------------------------------------
LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{

	switch (message)						//switch��俪ʼ
	{
	case WM_CREATE:
		ShowCursor(false);
		break;

	case WM_LBUTTONDOWN:
	case WM_MBUTTONDOWN:
	case WM_RBUTTONDOWN:
	case WM_KEYDOWN:
	case WM_SYSKEYDOWN:
		MoveWindow(hwnd, 0, 0, 0, 0, true);		//�رմ����ٶȱ���Ƶ�˳�������ʹ�䲻�ɼ����Է�ֹ��������
		PostQuitMessage(0);						// �˳����ڳ���
		mciSendString(L"close movie", g_buf, sizeof(g_buf), NULL);	//�ر�ī������Ƶ
		break; 								

	case WM_MOUSEMOVE:
	{
		int nNew_X = LOWORD(lParam);
		int nNew_Y = HIWORD(lParam);
		if (g_nOrigin_X == -1 && g_nOrigin_Y == -1)		//�������ָ���ʼ������
		{
			g_nOrigin_X = nNew_X;
			g_nOrigin_Y = nNew_Y;
		}
		else if (g_nOrigin_X != nNew_X  &&  g_nOrigin_Y != nNew_Y)	//�ж�����ƶ����
		{
			MoveWindow(hwnd, 0, 0, 0, 0, true);
			mciSendString(L"close movie", g_buf, sizeof(g_buf), NULL);
			PostQuitMessage(0);
		}
	}
		break;

	case WM_DESTROY:			//����������Ϣ
		PostQuitMessage(0);			
		break;									

	default:										
		return DefWindowProc(hwnd, message, wParam, lParam);		//����ȱʡ�Ĵ��ڹ���
	}

	return 0;			//�����˳�
}

//-----------------------------------��SmallWndProc( )������-------------------------------------------
// Desc��Ԥ�����ڹ��̺���,��Ԥ��������Ϣ���д���
//-----------------------------------------------------------------------------------------------------
LRESULT CALLBACK SmallWndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)						//switch��俪ʼ
	{
	case WM_DESTROY:			//����������Ϣ
		PostQuitMessage(0);
		break;

	default:
		return DefWindowProc(hwnd, message, wParam, lParam);		//����ȱʡ�Ĵ��ڹ���
	}

	return 0;			//�����˳�
}