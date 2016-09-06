/*------------------------------------------------------------
   HELLOWIN.C -- Displays "Hello, Windows 98!" in client area
   (c) Charles Petzold, 1998
   ------------------------------------------------------------*/

#include <windows.h>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
//因为在WinMain函数中就要用到，所以要先声明

int WINAPI WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	PSTR szCmdLine,
	int iCmdShow)//决定了最开始是正常显示还是最小化或最大化显示
{
	static TCHAR szAppName[] = TEXT("HelloWin");

	HWND         hwnd;
	MSG          msg;

	WNDCLASS     wndclass;//定义一个WNDCLASS窗口类

	//接下来初始化该窗口类
	wndclass.style = CS_HREDRAW | CS_VREDRAW;//窗口风格
	//现在这样表示水平或垂直改变时，窗口重绘
	wndclass.lpfnWndProc = WndProc;//基于该窗口类的所有窗口的窗口过程的地址
	wndclass.cbClsExtra = 0;//这个和下面这个用于预留额外空间
	wndclass.cbWndExtra = 0;
	wndclass.hInstance = hInstance;//应用程序的实例句柄(WinMain的一个参数)
	wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);//给所有基于该
	//窗口类的窗口设定一个图标,若要加载自定义图标，则第一个参数为应用程序实例句柄hInstance
	//而第二个参数始终为一个前缀为"IDI_"的图标标识符
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);//类似，载入鼠标
	wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);//背景色
	wndclass.lpszMenuName = NULL;//菜单项
	wndclass.lpszClassName = szAppName;//窗口类名称

	//完成WNDCLASS结构的初始化后，调用RegisterClass函数来进行窗口类注册
	//注意，要对类取地址！(下面是标准的处理方法，注意，注册失败要及时退出)
	if (!RegisterClass(&wndclass))
	{
		MessageBox(NULL, TEXT("This program requires Windows NT!"),
			szAppName, MB_ICONERROR);
		return 0;
	}

	//上面仅仅只是注册窗口类，下面才是真正创建窗口
	//返回一个窗口的句柄，返回值保存在HWND类型的变量中
	hwnd = CreateWindow(szAppName,                  // window class name
		TEXT("The Hello Program"), // window 标题
		WS_OVERLAPPEDWINDOW,        // 窗口风格，这里是普通层叠窗口
		CW_USEDEFAULT,              // initial x position
		CW_USEDEFAULT,              // initial y position
		CW_USEDEFAULT,              // initial x size
		CW_USEDEFAULT,              // initial y size
		NULL,                       // 父窗口 handle
		NULL,                       // window menu handle
		hInstance,                  // program instance handle
		NULL);                     // 创建参数

	////下面才是把窗口真正显示出来
	ShowWindow(hwnd, iCmdShow);//iCmdShow(WinMain参数)
	//决定了最开始是正常显示还是最小化或最大化显示
	UpdateWindow(hwnd);//这个函数向窗口过程函数(WndProc)发送一个WM_PAINT消息

	//Windows为每个程序维护了一个消息队列，当事件发生后，会自动把事件转化为消息
	//并放置在程序的消息队列中，程序通过下面的消息循环来从消息队列中获取消息
	while (GetMessage(&msg, NULL, 0, 0))//GetMessage()从消息队列中检索消息
	{					//后三个参数为NULL/0表示得到该程序创建的所有窗口的消息
		//若得到的消息为WM_QUIT，则GetMessage()返回0，否则返回非0

		TranslateMessage(&msg);//返回给Windows以进行某些键盘消息的转换

		DispatchMessage(&msg);//再次将消息返回给Windows
	}
	return msg.wParam;
}


//程序真正有意义的在窗口过程中：(窗口过程函数名可任意)
//一个程序可以包含多个窗口过程，单一个窗口过程总与一个调用RegisterClass的窗口类关联
//窗口过程函数只用来处理窗口消息！（但是这个消息包括了CREATE和PAINT消息）
LRESULT CALLBACK WndProc(HWND hwnd,   //窗口句柄
	UINT message,  //消息标志
	WPARAM wParam, //后两个参数用于提供消息更丰富的信息
	LPARAM lParam)
{
	//这三个参数下面处理消息要用
	HDC         hdc;
	PAINTSTRUCT ps;
	RECT        rect;

	switch (message)
	{
	case WM_CREATE:
		PlaySound(TEXT("hellowin.wav"), NULL, SND_FILENAME | SND_ASYNC);
		return 0;//注意，消息处理完就return 0，而不是break

	case WM_PAINT://当由于最初初始化/更改尺寸/最小化...窗口无效时，重绘窗口
		hdc = BeginPaint(hwnd, &ps);//返回设备环境句柄

		GetClientRect(hwnd, &rect);//得到窗口的(legt,top,right,bottom)信息，
									//并传入rect结构体
		
		//根据rect结构体中的信息，绘制文本
		DrawText(hdc,		//设备环境句柄
				TEXT("Hello, Windows 98!"), //文本信息
				-1, //表示该文本字符串以0结尾
				&rect,
			DT_SINGLELINE | DT_CENTER | DT_VCENTER);//显示样式：单行，居中，水平

		EndPaint(hwnd, &ps);
		return 0;

	case WM_DESTROY://点击了关闭(销毁窗口)
		PostQuitMessage(0);
		return 0;
	}

	//一般用下面形式返回
	return DefWindowProc(hwnd, message, wParam, lParam);
}