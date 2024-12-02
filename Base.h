#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <algorithm>

#ifdef _WIN64 || _WIN32
	#define PLATFORM_WINDOWS
#else
	#define PLATFORM_UNIX
#endif

#ifdef PLATFORM_WINDOWS
#define NOMINMAX

#include <Windows.h>
#include <wrl/client.h>
#include <ShlObj.h>
#include <comdef.h>
#endif

const char* APPLICATION_NAME = "Task Manager";
const char* APP_VERSION = "v0.1.0";

const std::string ALL_TASKS_FILE_NAME = "UserTasks.txt";

#ifdef PLATFORM_WINDOWS
void showNotification(const std::wstring& title, const std::wstring& message) {
    HWND hwnd = CreateWindow(L"STATIC", L"Notification", WS_OVERLAPPEDWINDOW, 0, 0, 0, 0, NULL, NULL, NULL, NULL);
    NOTIFYICONDATA nid = { 0 };
    nid.cbSize = sizeof(nid);
    nid.uFlags = NIF_ICON | NIF_INFO; 
    nid.dwInfoFlags = NIIF_INFO; 
    wcscpy_s(nid.szTip, L"Notification");
    wcscpy_s(nid.szInfoTitle, title.c_str());
    wcscpy_s(nid.szInfo, message.c_str());
    nid.hWnd = hwnd;
    nid.uCallbackMessage = WM_USER + 1;

    nid.hIcon = LoadIcon(NULL, IDI_INFORMATION); 

    if (!Shell_NotifyIcon(NIM_ADD, &nid)) {
        DWORD dwError = GetLastError();
        std::wcerr << L"Error: Failed to show notification! Error code: " << dwError << std::endl;
    }
    else {
        std::wcout << L"Notification sent successfully!" << std::endl;
    }

    Sleep(5000);

    Shell_NotifyIcon(NIM_DELETE, &nid); 
}
#else
void showNotification(const std::string& title, const std::string& message) {
    // TODO ....
}
#endif