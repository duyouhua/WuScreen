// WuScreen.cpp : ����Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "WuScreen.h"
#include "CMainWnd.h"

HHOOK mouse_Hook=NULL;
tagPOINT ps;
tagPOINT pe;
BOOL is_up = false;

BOOL LB_DOWN_STATUS = false;

void PrintDebug(const char* strOutputString, ...)
{
	char strBuffer[4096] = { 0 };
	va_list vlArgs;
	va_start(vlArgs, strOutputString);
	_vsnprintf_s(strBuffer, sizeof(strBuffer) - 1, strOutputString, vlArgs);
	//vsprintf(strBuffer,strOutputString,vlArgs);
	va_end(vlArgs);
	//OutputDebugString(CA2W(strBuffer));
	OutputDebugString(strBuffer);
}

BOOL UninstallHook()
{

	BOOL jud = FALSE;
	if (mouse_Hook) {
		jud = UnhookWindowsHookEx(mouse_Hook);
		mouse_Hook = NULL;  //set NULL  
	}

	return jud;
}

void showRectInDesktop(RECT & rect)
{
	// ���DC
	HDC hDC = ::GetDC(0);
	// ����ʵ�ߣ����Ϊ1����ɫ�ı�
	HPEN hPen = ::CreatePen(PS_SOLID, 4, RGB(255, 0, 0));
	// ����ѡ��DC
	HPEN hOldPen = (HPEN)::SelectObject(hDC, hPen);
	// ����һ��͸����ˢ
	HBRUSH hBrush = (HBRUSH)::GetStockObject(NULL_BRUSH);
	// ����ˢѡ��DC
	HBRUSH hOldBrush = (HBRUSH)::SelectObject(hDC, hBrush);
	// ���ƾ���
	::Rectangle(hDC, rect.left, rect.top, rect.right, rect.bottom);
	// �ָ���ͼ����
	::SelectObject(hDC, hOldPen);
	::SelectObject(hDC, hOldBrush);
	// ɾ����ͼ����
	::DeleteObject(hPen);
	::DeleteObject(hBrush);
	// �ͷ�DC
	::ReleaseDC(0, hDC);
}

LRESULT CALLBACK LowLevelMouseProc(INT nCode, WPARAM wParam, LPARAM lParam)
{
	MSLLHOOKSTRUCT *pkbhs = (MSLLHOOKSTRUCT *)lParam;
	switch (nCode)
	{

	case HC_ACTION:
	{ 
	//	if (wParam == WM_LBUTTONDOWN || wParam == WM_RBUTTONDOWN || wParam == WM_LBUTTONUP || wParam == WM_RBUTTONUP) {
			//��ȡ����λ�ã������б�Ҫ���ж�
			//����Ҽ�����
	//		PrintDebug("cliss==%s \n", "test");
	//	}
		if (LB_DOWN_STATUS && wParam == WM_MOUSEMOVE) {
			GetCursorPos(&pe);
			PrintDebug("x==%d  y==%d \n", pe.x, pe.y);
			//Rect := Rect(20, 30, 50, 90);
		}
		else {

		}
		
		//����������
		if (wParam == WM_LBUTTONDOWN) {
			LB_DOWN_STATUS = true;
			GetCursorPos(&ps);
			PrintDebug("x==%d  y==%d \n", ps.x, ps.y);
		}
		//�������ɿ�
		if (wParam == WM_LBUTTONUP) {
			LB_DOWN_STATUS = false;
		}
		//����Ҽ�����
		if (wParam == WM_RBUTTONDOWN) {

		}
		//����Ҽ��ɿ�
		if (wParam == WM_RBUTTONUP) {

		}
	}
	default:   break;
	}

	return CallNextHookEx(NULL, nCode, wParam, lParam);
}

BOOL InstallKbHook(HINSTANCE hInstance)
{

	if (mouse_Hook)   UninstallHook();
	mouse_Hook = SetWindowsHookEx(WH_MOUSE_LL,
		(HOOKPROC)(LowLevelMouseProc), hInstance, NULL);

	return(mouse_Hook != NULL);
}





int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	::CoInitialize(NULL);
	CPaintManagerUI::SetInstance(hInstance);
	InstallKbHook(hInstance);
	CMainWnd *pFrame = new CMainWnd(_T("duilib.xml"));
	pFrame->Create(NULL, _T("DUIWnd"), UI_WNDSTYLE_FRAME, WS_EX_WINDOWEDGE);
	pFrame->ShowModal();

	//���ڹص�֮��ɱ��¼��
	string cmd = "taskkill /f /t /im ffmpeg.exe";
	WinExec(cmd.c_str(), SW_HIDE);

	UninstallHook();
	delete pFrame;
	::CoUninitialize();
	return 0;
}


