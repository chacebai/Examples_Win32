#include <Windows.h>
#include <strsafe.h>
#include <commctrl.h>

#pragma comment(linker,"\"/manifestdependency:type='win32' \
name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

// 链接库
#pragma comment(lib, "comctl32.lib")

BOOL CenterWindow(HWND hwndWindow)
{
    HWND hwndParent;
    RECT rectWindow, rectParent;

    // make the window relative to its parent
    if ((hwndParent = GetParent(hwndWindow)) != NULL)
    {
        GetWindowRect(hwndWindow, &rectWindow);
        GetWindowRect(hwndParent, &rectParent);

        int nWidth = rectWindow.right - rectWindow.left;
        int nHeight = rectWindow.bottom - rectWindow.top;

        int nX = ((rectParent.right - rectParent.left) - nWidth) / 2 + rectParent.left;
        int nY = ((rectParent.bottom - rectParent.top) - nHeight) / 2 + rectParent.top;

        int nScreenWidth = GetSystemMetrics(SM_CXSCREEN);
        int nScreenHeight = GetSystemMetrics(SM_CYSCREEN);

        // make sure that the dialog box never moves outside of the screen
        if (nX < 0) nX = 0;
        if (nY < 0) nY = 0;
        if (nX + nWidth > nScreenWidth) nX = nScreenWidth - nWidth;
        if (nY + nHeight > nScreenHeight) nY = nScreenHeight - nHeight;

        MoveWindow(hwndWindow, nX, nY, nWidth, nHeight, FALSE);

        return TRUE;
    }

    return FALSE;
}

HWND CreateSimpleToolbar(HWND hWndParent)
{
    // Declare and initialize local constants.
    const int ImageListID = 0;
    const int numButtons = 6;
    const int bitmapSize = 16;

    const DWORD buttonStyles = BTNS_AUTOSIZE;

    // Create the toolbar.
    HWND hWndToolbar = CreateWindowEx(0, TOOLBARCLASSNAME, NULL,
        WS_CHILD | TBSTYLE_WRAPABLE, 0, 0, 0, 0,
        hWndParent, NULL, GetModuleHandle(NULL), NULL);

    if (hWndToolbar == NULL)
        return NULL;

    // Create the image list.
    HIMAGELIST hImageList = ImageList_Create(bitmapSize, bitmapSize,   // Dimensions of individual bitmaps.
        ILC_COLOR16 | ILC_MASK,   // Ensures transparent background.
        numButtons, 0);

    // 加载ICO图标
    HICON hIcon = (HICON)LoadImage(NULL, TEXT("../icon/giftbox.ico"), IMAGE_ICON, bitmapSize, bitmapSize, LR_LOADFROMFILE);
    HICON hIcon2 = (HICON)LoadImage(NULL, TEXT("../icon/delete.ico"), IMAGE_ICON, bitmapSize, bitmapSize, LR_LOADFROMFILE);
    HICON hIcon3 = (HICON)LoadImage(NULL, TEXT("../icon/check.ico"), IMAGE_ICON, bitmapSize, bitmapSize, LR_LOADFROMFILE);
    ImageList_AddIcon(hImageList, hIcon);
    ImageList_AddIcon(hImageList, hIcon2);
    ImageList_AddIcon(hImageList, hIcon3);

    // Set the image list.
    SendMessage(hWndToolbar, TB_SETIMAGELIST,
        (WPARAM)ImageListID,
        (LPARAM)hImageList);

    // Load the button images.
    SendMessage(hWndToolbar, TB_LOADIMAGES,
        (WPARAM)IDB_STD_SMALL_COLOR,
        (LPARAM)HINST_COMMCTRL);

    // Initialize button info.
    // IDM_NEW, IDM_OPEN, and IDM_SAVE are application-defined command constants.
    const int IDM_NEW = 101;
    const int IDM_OPEN = 102;
    const int IDM_SAVE = 103;

    TBBUTTON tbButtons[numButtons] =
    {
        { MAKELONG(STD_FILENEW,  ImageListID), IDM_NEW,  TBSTATE_ENABLED, buttonStyles, {0}, 0, (INT_PTR)TEXT("New") },
        { MAKELONG(STD_FILEOPEN, ImageListID), IDM_OPEN, TBSTATE_ENABLED, buttonStyles, {0}, 0, (INT_PTR)TEXT("Open")},
        { MAKELONG(STD_FILESAVE, ImageListID), IDM_SAVE, 0,               buttonStyles, {0}, 0, (INT_PTR)TEXT("Save")},
        { MAKELONG(0, ImageListID), IDM_SAVE, TBSTATE_ENABLED, buttonStyles, {0}, 0, (INT_PTR)TEXT("Button")},
        { MAKELONG(1, ImageListID), IDM_SAVE, TBSTATE_ENABLED, buttonStyles, {0}, 0, (INT_PTR)TEXT("Button")},
        { MAKELONG(2, ImageListID), IDM_SAVE, TBSTATE_ENABLED, buttonStyles, {0}, 0, (INT_PTR)TEXT("Button")},
    };
    // 工具栏按钮信息
    //tbButtons[3].iBitmap = 0;
    //tbButtons[3].idCommand = 1;
    //tbButtons[3].fsState = TBSTATE_ENABLED;
    //tbButtons[3].fsStyle = BTNS_BUTTON;
    //tbButtons[3].dwData = 0;
    //tbButtons[3].iString = (INT_PTR)"Button";

    // Add buttons.
    SendMessage(hWndToolbar, TB_BUTTONSTRUCTSIZE, (WPARAM)sizeof(TBBUTTON), 0);
    SendMessage(hWndToolbar, TB_ADDBUTTONS, (WPARAM)numButtons, (LPARAM)&tbButtons);

    // Resize the toolbar, and then show it.
    SendMessage(hWndToolbar, TB_AUTOSIZE, 0, 0);
    ShowWindow(hWndToolbar, TRUE);

    return hWndToolbar;
}

LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_CREATE:
    {
        // 创建菜单
        HMENU hMenu, hSubMenu;

        hMenu = CreateMenu();

        hSubMenu = CreatePopupMenu();
        AppendMenu(hSubMenu, MF_STRING, 1, TEXT("文件(&F)"));
        AppendMenu(hSubMenu, MF_STRING, 2, TEXT("退出(&X)"));
        AppendMenu(hMenu, MF_STRING | MF_POPUP, (UINT)hSubMenu, TEXT("文件(&F)"));

        SetMenu(hWnd, hMenu);
        // 添加工具栏
        CreateSimpleToolbar(hWnd);
        break;
    }
    case WM_PAINT:
    {
        break;
    }
    case WM_SIZE:
    {
        break;
    }
    case WM_COMMAND:
    {
        break;
    }
    case WM_LBUTTONDOWN:
    {
        PostMessage(hWnd, WM_NCLBUTTONDOWN, HTCAPTION, lParam);
        break;
    }
    case WM_RBUTTONDOWN:
    {
        int xPos = LOWORD(lParam);
        int yPos = HIWORD(lParam);
        TCHAR buffer[256];
        StringCchPrintf(buffer, 256, TEXT("xPos: %d, yPos: %d"), xPos, yPos);
        MessageBox(hWnd, buffer, TEXT("WM_LBUTTONDOWN"), MB_OK);
        break;
    }
    case WM_KEYDOWN:
    {
        break;
    }
    case WM_CLOSE:
    {
        DestroyWindow(hWnd);
        break;
    }
    case WM_DESTROY:
    {
        PostQuitMessage(0);
        break;
    }
    default:
        break;
    }
    LPCTSTR lpWindowplace = TEXT("./WinConfig.ini");
    if (uMsg == WM_CREATE) {
        // Window place
        WINDOWPLACEMENT wp;
        GetPrivateProfileStruct(TEXT("MainFrame"), TEXT("WindowPlace"), &wp, sizeof(wp), lpWindowplace);
        // 如果窗口最小化了，可以将它还原
        if (wp.showCmd == SW_SHOWMINIMIZED) {
            wp.showCmd = SW_SHOWNORMAL;
        }
        // 检查窗口是否在屏幕外部 将其移到屏幕内部
        RECT screenRect;
        SystemParametersInfo(SPI_GETWORKAREA, 0, &screenRect, 0);
        // 靠近屏幕边缘 快看不见了 把窗口居中
        screenRect.left += 100;
        screenRect.top += 100;
        screenRect.right -= 100;
        screenRect.bottom -= 100;
        if (!IntersectRect(&screenRect, &wp.rcNormalPosition, &screenRect)) {
            //wp.rcNormalPosition.left = screenRect.left;
            //wp.rcNormalPosition.top = screenRect.top;
            //wp.rcNormalPosition.right = screenRect.left + (wp.rcNormalPosition.right - wp.rcNormalPosition.left);
            //wp.rcNormalPosition.bottom = screenRect.top + (wp.rcNormalPosition.bottom - wp.rcNormalPosition.top);
            CenterWindow(hWnd);
        }
        else
            SetWindowPlacement(hWnd, &wp);
    }
    else if (uMsg == WM_DESTROY) {
        // Window place
        WINDOWPLACEMENT wp;
        GetWindowPlacement(hWnd, &wp);
        WritePrivateProfileStruct(TEXT("MainFrame"), TEXT("WindowPlace"), &wp, sizeof(wp), lpWindowplace);
    }
    return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

// WINAPI代表__stdcall 参数的传递顺序 从右到左 依次入栈 并且在函数返回前 依次出栈
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    // 启用高DPI支持
    SetProcessDPIAware();  // Windows 8.1及以下版本使用
    // SetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2); // Windows 10使用

    WNDCLASSEX wcex = { 0 };
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.cbClsExtra = 0;                                                                     // 类的额外内存
    wcex.cbWndExtra = 0;                                                                     // 窗口额外内存
    wcex.hbrBackground = GetSysColorBrush(/*COLOR_3DFACE*/ COLOR_3DFACE /*COLOR_3DSHADOW*/); // 背景
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);                                              // 光标 第一个为NULL代表使用系统提供的光标
    wcex.hIcon = LoadIcon(NULL, IDI_APPLICATION);                                            // 图标 第一个为NULL代表使用系统提供的图标
    wcex.hInstance = hInstance;                                                              // 应用程序实例句柄
    wcex.lpfnWndProc = WindowProc;                                                           // 回调函数
    wcex.lpszClassName = TEXT("MainClass");                                                  // 窗口类名称
    wcex.lpszMenuName = NULL;                                                                // 菜单名称
    wcex.style = CS_HREDRAW | CS_VREDRAW;                                                    // 显示风格

    RegisterClassEx(&wcex);

    /*****
     * CreateWindow()
     * lpClassName,lpWindowName,dwStyle,x,y,nWidth,nHeight,hWndParent,hMenu,hInstance,lpParam
     * 类名
     * 标题名
     * WS_OVERLAPPEDWINDOW 混合风格
     * x坐标 CW_USEDEFAULT
     * y坐标
     * 宽
     * 高
     * 父窗口 NULL
     * 菜单 NULL
     * 实例句柄 hInstance
     * 附加值
     */
    HWND hWnd = CreateWindowEx(NULL, wcex.lpszClassName, TEXT("MainFrame"), WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        NULL, NULL, hInstance, NULL);
    if (hWnd == NULL)
    {
        return 0;
    }
    ShowWindow(hWnd, SW_SHOWNORMAL);
    UpdateWindow(hWnd);

    /*****
     * HWND hwnd; 主窗口句柄
     * UINT message; 具体消息名称
     * WPARAM wParam; 附加消息 键盘消息
     * LPARAM lParam; 附加消息 鼠标消息
     * DWORD time; 消息产生时间
     * POINT pt; 附加消息 鼠标消息 x y
     */
    MSG msg;

    /*****
     * LPMSG lpMsg,HWND hWnd,UINT wMsgFilterMin,UINT wMsgFilterMax
     * 消息
     * 捕获窗口 NULL代表捕获所有窗口
     * 最小的过滤消息 0代表捕获所有消息
     * 最大的过滤消息 0代表捕获所有消息
     */
    while (GetMessage(&msg, NULL, 0, 0))
    {
        // 如果按的是组合键 需要翻译
        TranslateMessage(&msg);
        // 单按一个键盘就直接分发消息
        DispatchMessage(&msg);
    }
    return 0;
}