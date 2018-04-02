// ControlDemo.cpp : Defines the entry point for the application.
//
#include "stdafx.h"
#include "windows.h"
#include "commdlg.h"
#include "DUILib.h"
#include "DUIPopupWindow.h"
#include "DUIDebug.h"
#include "DUIControls.h"
#include "DUIResLoader.h"
#include "DUISkinMgr.h"
#include "DUIDibProcess.h"
#include "Python.h"
#include <tchar.h>
#include <stdio.h> 
#include <strsafe.h>
#include <stdlib.h>

#include <sstream>
#include<Shlobj.h>  
#pragma comment(lib,"Shell32.lib")  
using namespace std;

DUI_USING_NAMESPACE

#define BUFSIZE 4096 

HANDLE g_hChildStd_IN_Rd = NULL;
HANDLE g_hChildStd_IN_Wr = NULL;
HANDLE g_hChildStd_OUT_Rd = NULL;
HANDLE g_hChildStd_OUT_Wr = NULL;

HANDLE g_hInputFile = NULL;

void CreateChildProcess(void);
void WriteToPipe(void);
void ReadFromPipe(void);
void ErrorExit(PTSTR);

static OPENFILENAME ofn, ofn2;
PROCESS_INFORMATION piProcInfo;
CRefPtr<CImageList> Image[15];
DWORD ID[15];
double rate[15];
int mode,times;
CDUIString choose, title, choose_path, save;
BOOL start = false, have = false, havepath = false;
void PopFileInitialize(HWND hwnd)
{
	static TCHAR szFilter[] = TEXT("所有图片文件\0*.bmp;*.dib;*.jpg;*.jpeg;*.jpe;*.jfif;*.gif;*.tif;*.tiff;*.png;*.ico\0")  \
		TEXT("位图文件(*.bmp;*.dib)\0*.bmp;*.dib\0") \
		TEXT("JPEG(*.jpg;*.jpeg;*jpe;*.jfif)\0*.jpg;*.jpeg;*.jpe;*.jfif\0") \
		TEXT("GIF(*.gif)\0*.gif\0") \
		TEXT("TIFF(*.tif;*.tiff)\0*.tif;*.tiff\0") \
		TEXT("PNG(*.png)\0*.png\0") \
		TEXT("ICO(*.ico)\0*.ico\0") \
		TEXT("所有文件\0*.*\0");

	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = hwnd;
	ofn.hInstance = NULL;
	ofn.lpstrFilter = szFilter;
	ofn.lpstrCustomFilter = NULL;
	ofn.nMaxCustFilter = 0;
	ofn.nFilterIndex = 0;
	ofn.lpstrFile = NULL;          // Set in Open and Close functions
	ofn.nMaxFile = MAX_PATH;
	ofn.lpstrFileTitle = NULL;          // Set in Open and Close functions
	ofn.nMaxFileTitle = MAX_PATH;
	ofn.lpstrInitialDir = NULL;
	ofn.lpstrTitle = NULL;
	ofn.Flags = 0;             // Set in Open and Close functions
	ofn.nFileOffset = 0;
	ofn.nFileExtension = 0;
	ofn.lpstrDefExt = TEXT("*.bmp;*.dib;*.jpg;*.jpeg;*.jpe;*.jfif;*.gif;*.tif;*.tiff;*.png;*.ico");
	ofn.lCustData = 0L;
	ofn.lpfnHook = NULL;
	ofn.lpTemplateName = NULL;

	static TCHAR szFilter2[] = TEXT("XML文件(*.xml)\0*.xml\0")  \
		TEXT("所有文件\0*.*\0");

	ofn2.lStructSize = sizeof(OPENFILENAME);
	ofn2.hwndOwner = hwnd;
	ofn2.hInstance = NULL;
	ofn2.lpstrFilter = szFilter2;
	ofn2.lpstrCustomFilter = NULL;
	ofn2.nMaxCustFilter = 0;
	ofn2.nFilterIndex = 0;
	ofn2.lpstrFile = NULL;          // Set in Open and Close functions
	ofn2.nMaxFile = MAX_PATH;
	ofn2.lpstrFileTitle = NULL;          // Set in Open and Close functions
	ofn2.nMaxFileTitle = MAX_PATH;
	ofn2.lpstrInitialDir = NULL;
	ofn2.lpstrTitle = NULL;
	ofn2.Flags = 0;             // Set in Open and Close functions
	ofn2.nFileOffset = 0;
	ofn2.nFileExtension = 0;
	ofn2.lpstrDefExt = TEXT("*.bmp;*.dib;*.jpg;*.jpeg;*.jpe;*.jfif;*.gif;*.tif;*.tiff;*.png;*.ico");
	ofn2.lCustData = 0L;
	ofn2.lpfnHook = NULL;
	ofn2.lpTemplateName = NULL;
}
BOOL PopFileOpenDlg(HWND hwnd, PTSTR pstrFileName, PTSTR pstrTitleName)
{
	ofn.hwndOwner = hwnd;
	ofn.lpstrFile = pstrFileName;
	ofn.lpstrFileTitle = pstrTitleName;
	ofn.Flags = OFN_HIDEREADONLY | OFN_CREATEPROMPT | OFN_EXPLORER | OFN_FILEMUSTEXIST;

	return GetOpenFileName(&ofn);
}
BOOL PopFileSaveDlg(HWND hwnd, PTSTR pstrFileName, PTSTR pstrTitleName)
{
	ofn2.hwndOwner = hwnd;
	ofn2.lpstrFile = pstrFileName;
	ofn2.lpstrFileTitle = pstrTitleName;
	ofn2.Flags = OFN_HIDEREADONLY | OFN_CREATEPROMPT | OFN_EXPLORER;

	return GetSaveFileName(&ofn2);
}
BOOL PopFolderOpenDlg(PTSTR pstrFolderName)
{
	BROWSEINFO bi = { 0 };
	bi.hwndOwner = NULL;//拥有着窗口句柄，为NULL表示对话框是非模态的，实际应用中一般都要有这个句柄  
	bi.pszDisplayName = pstrFolderName;//接收文件夹的缓冲区  
	bi.lpszTitle = TEXT("选择一个文件夹");//标题  
	bi.ulFlags = BIF_NEWDIALOGSTYLE;
	LPITEMIDLIST idl = SHBrowseForFolder(&bi);
	return SHGetPathFromIDList(idl, pstrFolderName);
}

void ErrorExit(PTSTR lpszFunction)

// Format a readable error message, display a message box, 
// and exit from the application.
{
	LPVOID lpMsgBuf;
	LPVOID lpDisplayBuf;
	DWORD dw = GetLastError();

	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM |
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		dw,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf,
		0, NULL);

	lpDisplayBuf = (LPVOID)LocalAlloc(LMEM_ZEROINIT,
		(lstrlen((LPCTSTR)lpMsgBuf) + lstrlen((LPCTSTR)lpszFunction) + 40) * sizeof(TCHAR));
	StringCchPrintf((LPTSTR)lpDisplayBuf,
		LocalSize(lpDisplayBuf) / sizeof(TCHAR),
		TEXT("%s failed with error %d: %s"),
		lpszFunction, dw, lpMsgBuf);
	MessageBox(NULL, (LPCTSTR)lpDisplayBuf, TEXT("Error"), MB_OK);

	LocalFree(lpMsgBuf);
	LocalFree(lpDisplayBuf);
	ExitProcess(1);
}


class CMainWnd : public CDUIPopupWindow
{
	typedef CDUIPopupWindow theBase;
	BEGIN_MSG_MAP(CMainWnd)
		MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
		MESSAGE_HANDLER(WM_CREATE, OnCreate)
		MESSAGE_HANDLER(WM_TIMER, OnTimer)
		CHAIN_MSG_MAP(theBase)
	END_MSG_MAP()

	CDUIPopupWindow m_winTest;

	LRESULT OnDestroy(UINT, WPARAM, LPARAM, BOOL& bHandled)
	{
		bHandled = FALSE;

		if (m_winTest.IsWindow())
		{
			m_winTest.UnregisterNotifyHandler(this);
			m_winTest.DestroyWindow();
		}

		PostQuitMessage(0);

		return S_OK;
	}
	LRESULT OnCreate(UINT, WPARAM, LPARAM, BOOL& bHandled)
	{
		bHandled = FALSE;

		ResetSkin();

		memset(Image, 0, sizeof(Image));

		return S_OK;
	}
	LRESULT OnTimer(UINT, WPARAM, LPARAM, BOOL& bHandled)
	{
		DWORD  ExitCode;
		ReadFromPipe();

		if (start) {
			if (GetExitCodeProcess(piProcInfo.hProcess, &ExitCode))
			{
				DUI::CTrace::TraceInfo(_T("Exit code = %d/n"), ExitCode);
			}
			else
			{
				DUI::CTrace::TraceInfo(_T("GetExitCodeProcess() failed: %ld/n"), GetLastError());
			}
			times++;
			if (ExitCode != STILL_ACTIVE) {
				//OutputEvent("Consumed ");
				if (mode == 1) {
					CDUIString temp = _T("result_JPG\\");
					temp.append(title.substr(0, title.find(_T("."))));
					CResLoader mResLoad;
					mResLoad.ToAbsolutePath(temp);
					int nLen = temp.length();
					char buf[1024];
					memset(buf, 0, sizeof(buf));
					WideCharToMultiByte(CP_ACP, 0, (LPCWSTR)temp.c_str(), temp.length(), (LPSTR)buf, 1024, NULL, NULL);
					string str(buf);
					if (access(str.c_str(), 0) == 0) {
						//清空列表
						IDUIControl* pControl = GetControlByName(_T("tile_layouQQ"));
						if (pControl != NULL)
						{
							IDUITileLayout* pTilteLayout = (IDUITileLayout*)pControl->GetInterface(ITileLayout);
							pControl = GetControlByName(_T("choose_list"));
							IDUIListBox* pListBox = (IDUIListBox*)pControl->GetInterface(IListBox);
							if (pListBox->GetControlCount() > 1) {
								for (int i = 0; i < 14; i++) {
									pListBox->RemoveControlAt(1);
									pTilteLayout->RemoveControlAt(1);
								}
							}
							pListBox->SetCurSel(-1);

							if (pTilteLayout != NULL && pTilteLayout->GetControlCount() > 0 && pListBox != NULL && pListBox->GetControlCount() > 0)
							{
								CResLoader mResLoad;
								long   hFile = 0;
								struct _finddata_t fileinfo;
								string p;//字符串，存放路径
								string newpath = p.assign(str).append("\\*.*").c_str();
								wstring resfile[15];
								int now = 0;
								BOOL first = true, first2 = true;
								if ((hFile = _findfirst(p.assign(str).append("\\*.*").c_str(), &fileinfo)) != -1)//若查找成功，则进入
								{
									do
									{
										//如果是目录,迭代之（即文件夹内还有文件夹）  
										if ((fileinfo.attrib &  _A_SUBDIR))
										{

										}
										else {
											newpath = p.assign(str).append("\\").append(fileinfo.name);
											int nLen = (int)newpath.length();
											wchar_t buf[1024];
											memset(buf, 0, sizeof(buf));
											int nResult = MultiByteToWideChar(CP_ACP, 0, (LPCSTR)newpath.c_str(), nLen, (LPWSTR)buf, 1024);
											wstring wstr(buf);
											DUI::CTrace::TraceInfo(_T("the file is %s\n"), wstr.c_str());
											char *ptr;
											resfile[(int)strtod(fileinfo.name, &ptr) - 1] = wstr;
										}
									} while (_findnext(hFile, &fileinfo) == 0);
									//_findclose函数结束查找
									_findclose(hFile);
								}
								IDUIRichEdit* pEventEdit = NULL;
								IDUIControl* pControl = GetControlByName(_T("control_event_edit"));
								if (pControl != NULL) pEventEdit = (IDUIRichEdit*)pControl->GetInterface(IRichEdit);
								wstring data = pEventEdit->GetText();

								for (int i = 0; i < 15; i++)
								{
									Image[i] = mResLoad.LoadImageFromFile(resfile[i]);
									CRefPtr<IDUIControl> pNewControl;
									IDUIPictureBox* pPictureBox;
									if (!first) {
										pNewControl = pTilteLayout->GetControlByIndex(0)->Clone();
										pPictureBox = (IDUIPictureBox*)pNewControl->GetInterface(IPictureBox);
										pTilteLayout->AddControl(pNewControl);
									}
									else {
										pNewControl = pTilteLayout->GetControlByIndex(0);
										pPictureBox = (IDUIPictureBox*)pNewControl->GetInterface(IPictureBox);
										first = false;
									}
									pPictureBox->SetImage(Image[i]);
									pPictureBox->SetDrawMode(PICTURE_BOX_STRETCH_IMAGE);
									pTilteLayout->SetColumnCount(15);
									DeleteFile(resfile[i].c_str());

									IDUIIconTextItem* pIconText;
									if (!first2) {
										pNewControl = pListBox->GetControlByIndex(0)->Clone();
										pIconText = (IDUIIconTextItem*)pNewControl->GetInterface(IListIconTextItem);
										pListBox->AddControl(pNewControl);
									}
									else {
										pNewControl = pListBox->GetControlByIndex(0);
										pIconText = (IDUIIconTextItem*)pNewControl->GetInterface(IListIconTextItem);
										first2 = false;
									}
									pIconText->SetIcon(Image[i]);
									wchar_t buf[5];
									_itow(i + 1, buf, 10);
									pIconText->SetText(wstring(_T("候选图片")).append(wstring(buf)).c_str());

									wstring now = data.substr(data.find(wstring(_T("Top ")).append(buf)), 42);
									wchar_t *end;
									ID[i] = wcstod(now.substr(now.find(wstring(_T("# ID: "))) + 6, 6).c_str(), &end);
									rate[i] = wcstof(now.substr(now.find(wstring(_T("matching rate: "))) + 15, 7).c_str(), &end);
								}
								int ret = RemoveDirectory(temp.c_str());
								start = false;
							}
						}
					}
				}
				else if (mode == 2) {
					CDUIString temp = _T("result_XML\\");
					CResLoader mResLoad;
					mResLoad.ToAbsolutePath(temp);
					int nLen = temp.length();
					char buf[1024];
					memset(buf, 0, sizeof(buf));
					WideCharToMultiByte(CP_ACP, 0, (LPCWSTR)temp.c_str(), temp.length(), (LPSTR)buf, 1024, NULL, NULL);
					string str(buf);
					if (access(str.c_str(), 0) == 0) {
						//清空列表
						CResLoader mResLoad;
						long   hFile = 0;
						struct _finddata_t fileinfo;
						string p;//字符串，存放路径
						string newpath = p.assign(str).append("\\*.*").c_str();
						wstring resfile[15];
						int now = 0;
						BOOL first = true, first2 = true;
						if ((hFile = _findfirst(p.assign(str).append("\\*.*").c_str(), &fileinfo)) != -1)//若查找成功，则进入
						{
							do
							{
								//如果是目录,迭代之（即文件夹内还有文件夹）  
								if ((fileinfo.attrib &  _A_SUBDIR))
								{

								}
								else if (string(fileinfo.name).compare("result.xml") == 0) {
									start = false;
									newpath = p.assign(str).append("\\").append(fileinfo.name);
									int nLen = (int)newpath.length();
									wchar_t buf[1024];
									memset(buf, 0, sizeof(buf));
									int nResult = MultiByteToWideChar(CP_ACP, 0, (LPCSTR)newpath.c_str(), nLen, (LPWSTR)buf, 1024);
									wstring wstr(buf);
									DUI::CTrace::TraceInfo(_T("the file is %s\n"), wstr.c_str());

									HANDLE hInputFile = CreateFile(
										wstr.c_str(),
										GENERIC_READ,
										0,
										NULL,
										OPEN_EXISTING,
										FILE_ATTRIBUTE_READONLY,
										NULL);

									if (hInputFile == INVALID_HANDLE_VALUE)
										ErrorExit(TEXT("CreateFile"));

									DWORD dwRead, dwWritten, dwTotalBytesAvail;
									CHAR chBuf[BUFSIZE];
									BOOL bSuccess = FALSE;
									memset(chBuf, 0, sizeof(chBuf));
									while (ReadFile(hInputFile, chBuf, BUFSIZE, &dwRead, NULL))
									{
										if (dwRead <= 0)
											break;

										wchar_t buf[BUFSIZE];
										memset(buf, 0, sizeof(buf));
										int nResult = MultiByteToWideChar(CP_ACP, 0, (LPCSTR)chBuf, dwRead, (LPWSTR)buf, BUFSIZE);
										wstring wstr(buf);
										IDUIRichEdit* pEventEdit = NULL;
										IDUIControl* pControl = GetControlByName(_T("export_edit"));
										if (pControl != NULL) pEventEdit = (IDUIRichEdit*)pControl->GetInterface(IRichEdit);

										pEventEdit->AppendText(wstr.c_str());
										if (str.length() == 0) break;
										memset(chBuf, 0, sizeof(chBuf));
									}
									CloseHandle(hInputFile);
									DeleteFile(wstr.c_str());
								}
							} while (_findnext(hFile, &fileinfo) == 0);
							//_findclose函数结束查找
							_findclose(hFile);
						}
					}
				}
			}
		}
		return S_OK;
	}

	void CreateChildProcess(int choice)
		// Create a child process that uses the previously created pipes for STDIN and STDOUT.
	{
		CDUIString szCmdline = _T("python ");
		STARTUPINFO siStartInfo;
		BOOL bSuccess = FALSE;

		CResLoader mResLoad;
		CDUIString path, Current;
		CDUIString temp = _T("");
		mResLoad.ToAbsolutePath(temp);
		Current = temp;
		if (choice == 1)
		{
			temp = _T("test_single.py");
			//mResLoad.ToAbsolutePath(temp);
			path = szCmdline.append(temp);
			path = path.append(_T(" --pic_path \""));
			path.append(choose);
			path.append(_T("\""));
			mode = 1;
		}
		else if (choice == 2)
		{
			temp = _T("test_multiple.py ");
			//mResLoad.ToAbsolutePath(temp);
			path = szCmdline.append(temp);
			path = path.append(_T(" --query_dir \""));
			path.append(choose_path);
			path.append(_T("\""));
			mode = 2;
		}


		// Set up members of the PROCESS_INFORMATION structure. 

		ZeroMemory(&piProcInfo, sizeof(PROCESS_INFORMATION));

		// Set up members of the STARTUPINFO structure. 
		// This structure specifies the STDIN and STDOUT handles for redirection.

		ZeroMemory(&siStartInfo, sizeof(STARTUPINFO));
		siStartInfo.cb = sizeof(STARTUPINFO);
		siStartInfo.hStdError = g_hChildStd_OUT_Wr;
		siStartInfo.hStdOutput = g_hChildStd_OUT_Wr;
		siStartInfo.hStdInput = g_hChildStd_IN_Rd;
		siStartInfo.dwFlags |= STARTF_USESTDHANDLES;

		// Create the child process. 

		bSuccess = CreateProcess(NULL,
			(LPWSTR)path.c_str(),     // command line 
			NULL,          // process security attributes 
			NULL,          // primary thread security attributes 
			TRUE,          // handles are inherited 
			CREATE_NO_WINDOW,             // creation flags 
			NULL,          // use parent's environment 
			Current.c_str(),          // use parent's current directory 
			&siStartInfo,  // STARTUPINFO pointer 
			&piProcInfo);  // receives PROCESS_INFORMATION 

						   // If an error occurs, exit the application. 
		if (!bSuccess)
			ErrorExit(TEXT("CreateProcess"));
		else
		{
			// Close handles to the child process and its primary thread.
			// Some applications might keep these handles to monitor the status
			// of the child process, for example. 

			//CloseHandle(piProcInfo.hProcess);
			//CloseHandle(piProcInfo.hThread);
		}
	}

	void WriteToPipe(void)

		// Read from a file and write its contents to the pipe for the child's STDIN.
		// Stop when there is no more data. 
	{
		DWORD dwRead, dwWritten;
		CHAR chBuf[BUFSIZE];
		BOOL bSuccess = FALSE;

		for (;;)
		{
			bSuccess = ReadFile(g_hInputFile, chBuf, BUFSIZE, &dwRead, NULL);
			if (!bSuccess || dwRead == 0) break;

			bSuccess = WriteFile(g_hChildStd_IN_Wr, chBuf, dwRead, &dwWritten, NULL);
			if (!bSuccess) break;
		}

		// Close the pipe handle so the child process stops reading. 

		if (!CloseHandle(g_hChildStd_IN_Wr))
			ErrorExit(TEXT("StdInWr CloseHandle"));
	}

	void ReadFromPipe(void)

		// Read output from the child process's pipe for STDOUT
		// and write to the parent process's pipe for STDOUT. 
		// Stop when there is no more data. 
	{
		DWORD dwRead, dwWritten, dwTotalBytesAvail, dwRealRead;
		CHAR chBuf[BUFSIZE];
		BOOL bSuccess = FALSE;
		HANDLE hParentStdOut = GetStdHandle(STD_OUTPUT_HANDLE);

		Sleep(200);

		while (PeekNamedPipe(g_hChildStd_OUT_Rd, chBuf, BUFSIZE, &dwRealRead, &dwTotalBytesAvail, NULL))
		{
			if (dwRealRead <= 0)
				break;

			memset(chBuf, 0, sizeof(chBuf));
			bSuccess = ReadFile(g_hChildStd_OUT_Rd, chBuf, BUFSIZE, &dwRead, NULL);
			if (!bSuccess || dwRead == 0) break;

			/*bSuccess = WriteFile(hParentStdOut, chBuf,
			dwRead, &dwWritten, NULL);
			if (!bSuccess) break;*/
			string str(chBuf);
			std::wstring wstr(str.length(), L' ');
			std::copy(str.begin(), str.end(), wstr.begin());
			OutputEvent(wstr);
			if (str.length() == 0) break;
		}
	}

	VOID OnDUINotify(const DUINotify& info, BOOL& bHandled)
	{
		CheckSystemNotification(info, bHandled);

		//DebugEvent(info);

		if (m_winTest.IsWindow()
			&& info.hWndManager == m_winTest.m_hWnd)
		{
			if (info.dwMsgType == WM_NOTIFY_BUTTON_CLICKED)
			{
				CDUIString strName = info.pSender->GetName();
				if (strName.compare(_T("change_skin")) == 0)
				{
					ResetSkin();
				}
			}

			return;
		}

		if (info.dwMsgType == WM_NOTIFY_BUTTON_CLICKED)
		{
			CDUIString strName = info.pSender->GetName();
			if (strName.compare(_T("open")) == 0)
			{
				wchar_t FileName[128], TitleName[50];
				memset(FileName, 0, sizeof(FileName));
				memset(TitleName, 0, sizeof(TitleName));
				if (PopFileOpenDlg(0, FileName, TitleName)) {
					choose = FileName;
					title = TitleName;
					CResLoader mResLoad;
					CRefPtr<CImageList> Image = mResLoad.LoadImageFromFile(FileName);
					CRefPtr<IDUIControl> pControl = GetControlByName(_T("select"));
					IDUIPictureBox* pPictureBox = (IDUIPictureBox*)pControl->GetInterface(IPictureBox);
					pPictureBox->SetImage(Image);
					pPictureBox->SetDrawMode(PICTURE_BOX_STRETCH_IMAGE);
					have = true;
				}
			}
			else if (strName.compare(_T("search")) == 0) {
				if (start) return;
				if (!have) return;

				IDUIRichEdit* pEventEdit = NULL;
				IDUIControl* pControl = GetControlByName(_T("control_event_edit"));
				if (pControl != NULL) pEventEdit = (IDUIRichEdit*)pControl->GetInterface(IRichEdit);
				pEventEdit->SetText(_T(""));

				SECURITY_ATTRIBUTES saAttr;

				printf("\n->Start of parent execution.\n");

				saAttr.nLength = sizeof(SECURITY_ATTRIBUTES);
				saAttr.bInheritHandle = TRUE;
				saAttr.lpSecurityDescriptor = NULL;

				if (!CreatePipe(&g_hChildStd_OUT_Rd, &g_hChildStd_OUT_Wr, &saAttr, 0))
					ErrorExit(TEXT("StdoutRd CreatePipe"));
				if (!SetHandleInformation(g_hChildStd_OUT_Rd, HANDLE_FLAG_INHERIT, 0))
					ErrorExit(TEXT("Stdout SetHandleInformation"));
				if (!CreatePipe(&g_hChildStd_IN_Rd, &g_hChildStd_IN_Wr, &saAttr, 0))
					ErrorExit(TEXT("Stdin CreatePipe"));
				if (!SetHandleInformation(g_hChildStd_IN_Wr, HANDLE_FLAG_INHERIT, 0))
					ErrorExit(TEXT("Stdin SetHandleInformation"));
				CreateChildProcess(1);

				CResLoader mResLoad;
				CDUIString temp = _T("inout.txt");
				mResLoad.ToAbsolutePath(temp);

				g_hInputFile = CreateFile(
					temp.c_str(),
					GENERIC_READ,
					0,
					NULL,
					OPEN_EXISTING,
					FILE_ATTRIBUTE_READONLY,
					NULL);

				if (g_hInputFile == INVALID_HANDLE_VALUE)
					ErrorExit(TEXT("CreateFile"));

				WriteToPipe();

				OutputEvent(_T("\n->Searching for the similiar picture...\n\n"));
				ReadFromPipe();

				CloseHandle(g_hInputFile);

				start = true; times = 0;
			}
			else if (strName.compare(_T("open_path")) == 0)
			{
				wchar_t PathName[256];
				memset(PathName, 0, sizeof(PathName));
				if (PopFolderOpenDlg(PathName)) {
					choose_path = PathName;
					CResLoader mResLoad;
					CRefPtr<IDUIControl> pControl = GetControlByName(_T("path"));
					IDUILabel* pLabel = (IDUILabel*)pControl->GetInterface(ILabel);
					pLabel->SetText(wstring(_T("路径：")).append(choose_path));
					havepath = true;
				}
			}
			else if (strName.compare(_T("export")) == 0)
			{
				if (start) return;
				if (!havepath) return;
				SECURITY_ATTRIBUTES saAttr;

				printf("\n->Start of parent execution.\n");

				saAttr.nLength = sizeof(SECURITY_ATTRIBUTES);
				saAttr.bInheritHandle = TRUE;
				saAttr.lpSecurityDescriptor = NULL;

				if (!CreatePipe(&g_hChildStd_OUT_Rd, &g_hChildStd_OUT_Wr, &saAttr, 0))
					ErrorExit(TEXT("StdoutRd CreatePipe"));
				if (!SetHandleInformation(g_hChildStd_OUT_Rd, HANDLE_FLAG_INHERIT, 0))
					ErrorExit(TEXT("Stdout SetHandleInformation"));
				if (!CreatePipe(&g_hChildStd_IN_Rd, &g_hChildStd_IN_Wr, &saAttr, 0))
					ErrorExit(TEXT("Stdin CreatePipe"));
				if (!SetHandleInformation(g_hChildStd_IN_Wr, HANDLE_FLAG_INHERIT, 0))
					ErrorExit(TEXT("Stdin SetHandleInformation"));
				CreateChildProcess(2);

				CResLoader mResLoad;
				CDUIString temp = _T("inout.txt");
				mResLoad.ToAbsolutePath(temp);

				g_hInputFile = CreateFile(
					temp.c_str(),
					GENERIC_READ,
					0,
					NULL,
					OPEN_EXISTING,
					FILE_ATTRIBUTE_READONLY,
					NULL);

				if (g_hInputFile == INVALID_HANDLE_VALUE)
					ErrorExit(TEXT("CreateFile"));

				WriteToPipe();

				OutputEvent(_T("\n->Searching for the similiar picture...\n\n"));
				ReadFromPipe();

				CloseHandle(g_hInputFile);

				start = true; times = 0;
			}
			else if (strName.compare(_T("save")) == 0)
			{
				wchar_t FileName[128], TitleName[50];
				memset(FileName, 0, sizeof(FileName));
				memset(TitleName, 0, sizeof(TitleName));
				if (PopFileSaveDlg(0, FileName, TitleName)) {
					save = FileName;
					have = true;
					HANDLE hOutputFile = CreateFile(
						save.c_str(),
						GENERIC_WRITE,
						0,
						NULL,
						OPEN_ALWAYS,
						FILE_ATTRIBUTE_NORMAL,
						NULL);

					if (hOutputFile == INVALID_HANDLE_VALUE)
						ErrorExit(TEXT("CreateFile"));
					IDUIRichEdit* pEventEdit = NULL;
					IDUIControl* pControl = GetControlByName(_T("export_edit"));
					DWORD dwWritten;
					BOOL bSuccess;
					if (pControl != NULL) pEventEdit = (IDUIRichEdit*)pControl->GetInterface(IRichEdit);
					wstring data = pEventEdit->GetText();
					bSuccess = WriteFile(hOutputFile, data.c_str(), data.length(), &dwWritten, NULL);
					CloseHandle(hOutputFile);
				}
			}
			else if (strName.compare(_T("start")) == 0)
			{
				StartProgress();
			}
			else if (strName.compare(_T("add_listitem")) == 0)
			{
				AddListItem();
			}
			else if (strName.compare(_T("file_button")) == 0)
			{
				ShowMenu(info.pSender, _T("menu_file"));
			}
			else if (strName.compare(_T("click_me_button")) == 0)
			{
				ShowMenu(info.pSender, _T("menu_click_me"));
			}
			else if (strName.compare(_T("radio1")) == 0
				|| strName.compare(_T("radio2")) == 0)
			{
				ClickRadioButton(info.pSender);
			}
			else if (strName.compare(_T("test_popup")) == 0)
			{
				ShowTestWindow();
			}
			else if (strName.compare(_T("add_treeitem")) == 0)
			{

			}
		}
		else if (info.dwMsgType == WM_NOTIFY_TRACK_POS_CHANGED)
		{
			SetTrackBarTooltip(info);
		}
		else if (info.dwMsgType == WM_NOTIFY_LISTBOX_NOTIFY)
		{
			IDUIControl* pControl = GetControlByName(_T("selected"));
			if (pControl == NULL) return;
			IDUIControl* pControl2 = GetControlByName(_T("choose_list"));
			if (pControl2 == NULL) return;
			IDUIControl* pControl3 = GetControlByName(_T("rate"));
			if (pControl3 == NULL) return;
			IDUIControl* pControl4 = GetControlByName(_T("number"));
			if (pControl4 == NULL) return;

			IDUIPictureBox* pPicBox = (IDUIPictureBox*)pControl->GetInterface(IPictureBox);
			if (pPicBox == NULL) return;
			IDUIListBox* pListBox = (IDUIListBox*)pControl2->GetInterface(IListBox);
			if (pListBox == NULL) return;
			IDUILabel* pLabel = (IDUILabel*)pControl3->GetInterface(ILabel);
			if (pLabel == NULL) return;
			IDUILabel* pLabel2 = (IDUILabel*)pControl4->GetInterface(ILabel);
			if (pLabel2 == NULL) return;

			INT i = pListBox->GetCurSel();
			if (i >= 0 && !Image[i].IsNull()) {
				pPicBox->SetImage(Image[i]);
				pPicBox->SetDrawMode(PICTURE_BOX_STRETCH_IMAGE);
				pPicBox->UpdateLayout();
				wchar_t buf[25];
				swprintf(buf, _T("%.2f%%"), rate[i]);
				pLabel->SetText(wstring(buf));
				swprintf(buf, _T("编号：%d"), ID[i]);
				pLabel2->SetText(wstring(buf));
			}
		}

		if (!bHandled)
		{
			theBase::OnDUINotify(info, bHandled);
		}
	}

	VOID CheckSystemNotification(const DUINotify& info, BOOL& bHandled)
	{
		IDUIControl* pControl = info.pSender;

		switch (info.dwMsgType)
		{
		case WM_NOTIFY_BUTTON_CLICKED:
			if (pControl != NULL)
			{
				const CDUIString& strName = pControl->GetName();
				if (strName.compare(DUI_SYS_MIN_BTN) == 0)
				{
					if (GetStyle()&WS_SYSMENU)
					{
						PostMessage(WM_SYSCOMMAND, SC_MINIMIZE, 0);
					}
					else
					{
						ShowWindow(SW_MINIMIZE);
					}
					bHandled = TRUE;
				}
				else if (strName.compare(DUI_SYS_MAX_BTN) == 0)
				{
					if (GetStyle()&WS_SYSMENU)
						PostMessage(WM_SYSCOMMAND, SC_MAXIMIZE, 0);
					else
						ShowWindow(SW_MAXIMIZE);
					bHandled = TRUE;
				}
				else if (strName.compare(DUI_SYS_RESTORE_BTN) == 0)
				{
					if (GetStyle()&WS_SYSMENU)
						PostMessage(WM_SYSCOMMAND, SC_RESTORE, 0);
					else
						ShowWindow(SW_RESTORE);
					bHandled = TRUE;
				}
				else if (strName.compare(DUI_SYS_CLOSE_BTN) == 0)
				{
					if (GetStyle()&WS_SYSMENU)
						PostMessage(WM_SYSCOMMAND, SC_CLOSE, 0);
					else
						PostMessage(WM_CLOSE, 0, 0);
					bHandled = TRUE;
				}
			}
			break;

		default:
			break;
		}
	}

	VOID OutputEvent(const CDUIString& strText)
	{
		IDUIRichEdit* pEventEdit = NULL;
		IDUIControl* pControl = GetControlByName(_T("control_event_edit"));
		if (pControl != NULL) pEventEdit = (IDUIRichEdit*)pControl->GetInterface(IRichEdit);
		if (pEventEdit == NULL) return;

		pEventEdit->AppendText(strText.c_str());
	}

	VOID ResetSkin()
	{
		IDUIApp* pDUIApp = DUIGetAppInstance();
		IDUISkinMgr* pSkinMgr = pDUIApp->GetSkinMgr();
		static BOOL bRet = TRUE;
		CRefPtr<CDUIFrameBK> pFrameBK = NULL;
		if (bRet)
		{
			pFrameBK = pSkinMgr->GetFrameBK(_T("skin2_bk"));
		}
		else
		{
			pFrameBK = pSkinMgr->GetFrameBK(_T("skin1_bk"));
		}
		bRet = !bRet;

		pSkinMgr->SetSkinUIData(pFrameBK);
	}

	virtual VOID OnBeforeFlipDIB(CDibSection* pDIB)
	{
		if (!IsNcActivate())
		{
			CGrayScaleImageFilter::Process(pDIB);
		}
	}

	VOID StartProgress()
	{
		IDUIControl* pControl1 = GetControlByName(_T("progressbar1"));
		IDUIControl* pControl2 = GetControlByName(_T("progressbar2"));
		if (pControl1 == NULL || pControl2 == NULL) return;

		IDUIProgressBar* pProgressbar1 = (IDUIProgressBar*)pControl1->GetInterface(IProgressBar);
		if (pProgressbar1 == NULL) return;
		IDUIProgressBar* pProgressbar2 = (IDUIProgressBar*)pControl2->GetInterface(IProgressBar);
		if (pProgressbar2 == NULL) return;

		INT nMin1, nMax1;
		pProgressbar1->GetRange(nMin1, nMax1);
		pProgressbar1->SetPos(nMin1);

		INT nMin2, nMax2;
		pProgressbar2->GetRange(nMin2, nMax2);
		pProgressbar2->SetPos(nMin2);

		while (pProgressbar1->GetPos() < nMax1
			|| pProgressbar2->GetPos() < nMax2)
		{
			pProgressbar1->OffsetPos(50);
			pProgressbar2->OffsetPos(100);
			UpdateWindow();

			Sleep(200);
		}
	}

	VOID ClickRadioButton(IDUIControl* pControl)
	{
		IDUIControl* pRadio1 = GetControlByName(_T("radio1"));
		IDUIControl* pRadio2 = GetControlByName(_T("radio2"));
		IDUIControl* pUnCheck = NULL;
		if (pControl == pRadio1)
		{
			pUnCheck = pRadio2;
		}
		else if (pControl == pRadio2)
		{
			pUnCheck = pRadio1;
		}

		//only one radio button can be checked at the same time
		if (pUnCheck != NULL)
		{
			IDUIButton* pRadioUnCheck = (IDUIButton*)pUnCheck->GetInterface(IButton);
			if (pRadioUnCheck != NULL)
			{
				pRadioUnCheck->SetCheck(FALSE);
			}
		}
	}

	VOID ShowMenu(IDUIControl* pControl, const CDUIString& strMenuName)
	{
		CRefPtr<IDUIMenu> pMemu = CDUIMenu::CreateFromResource(strMenuName);
		if (!pMemu.IsNull())
		{
			RECT rtClient;
			GetClientRect(&rtClient);
			::ClientToScreen(m_hWnd, (LPPOINT)&rtClient);
			::ClientToScreen(m_hWnd, ((LPPOINT)&rtClient + 1));
			RECT rtControl = pControl->GetControlRect();
			INT nX = rtClient.left + rtControl.left;
			INT nY = rtClient.top + rtControl.bottom;
			CRefPtr<IDUIMenuItem> pMenuItem = pMemu->TrackPopupMenu(pControl, nX, nY);
		}
	}

	VOID SetTrackBarTooltip(const DUINotify& info)
	{
		IDUITrackBar* pTrackBar = (IDUITrackBar*)info.pSender->GetInterface(ITrackBar);
		if (pTrackBar != NULL)
		{
			IDUIButton* pThumbBtn = pTrackBar->GetThumbButton();
			if (pThumbBtn != NULL)
			{
				INT nPos = pTrackBar->GetPos();
				wstringstream ws;
				ws << L"current position:" << nPos;
				pThumbBtn->SetTooltip(ws.str());
			}
		}
	}


	VOID ShowTestWindow()
	{
		if (!m_winTest.IsWindow())
		{
			m_winTest.SetUseSkinBK(TRUE);

			RECT rtParent;
			GetWindowRect(&rtParent);

			RECT rtTest;
			rtTest.left = rtParent.left + 100;
			rtTest.top = rtParent.top + 100;
			rtTest.right = rtTest.left + 400;
			rtTest.bottom = rtTest.top + 400;

			m_winTest.CreateFromResource(_T("popup_win"), NULL, rtTest, NULL
				, WS_VISIBLE | WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN | WS_CLIPSIBLINGS,/* WS_EX_TOPMOST |*/ WS_EX_TOOLWINDOW);

			m_winTest.RegisterNotifyHandler(this);
		}
		else
		{
			::SetActiveWindow(m_winTest.m_hWnd);
		}
	}

	VOID AddListItem()
	{
		IDUIControl* pControl = GetControlByName(_T("test_list"));
		if (pControl == NULL) return;

		IDUIListBox* pList = (IDUIListBox*)pControl->GetInterface(IListBox);
		if (pList == NULL) return;

		INT nCount = pList->GetControlCount();
		IDUIControl* pItemCopy = NULL;
		INT nCopyIndex(0);
		if (nCount > 0)
		{
			for (INT i = 0; i < nCount; ++i)
			{
				IDUIListItem* pItem = (IDUIListItem*)(pList->GetControlByIndex(i)->GetInterface(IListItem));
				if (pItem != NULL
					&& !pItem->GetGroup()
					&& pItem->IsVisible()
					&& !pItem->GetExpand())
				{
					pItemCopy = pItem;
					nCopyIndex = i;
					break;
				}
			}
		}

		if (pItemCopy != NULL)
		{
			LockLayout(TRUE);
			const INT nCopyCount = 1000;
			CDUIString strText = pItemCopy->GetText();

			wstringstream ws;
			for (INT i = 0; i < nCopyCount; ++i)
			{
				CRefPtr<IDUIControl> pControl = pItemCopy->Clone();
				pList->AddControlAt(nCopyIndex, pControl);

				ws.str(L"");
				ws << strText << i;
				pControl->SetText(ws.str());
			}
			LockLayout(FALSE);

			LayoutRootControl();
			Invalidate();
		}
	}
};

int APIENTRY WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR     lpCmdLine,
	int       nCmdShow)
{
	IDUIApp* pDUIApp = DUIGetAppInstance();
	if (pDUIApp == NULL) return -1;

	CResLoader loader;
	CDUIString strXMLEntry;
	loader.LoadXMLFromFile(_T(".\\res\\xml\\dui_control_demo.xml"), strXMLEntry);
	//pDUIApp->Init(strXMLEntry);

	BOOL bRet = pDUIApp->Init(strXMLEntry);//载入XML文件初始化皮肤部分
	DUI_ASSERT(bRet);
	if (!bRet) return -1;

	PopFileInitialize(0); 


	IDUISkinMgr* pSkinMgr = pDUIApp->GetSkinMgr();
	IDUIControlFactory* pControlFactory = pDUIApp->GetControlFactory();

	CDUIString strTitle;
	pSkinMgr->GetStringText(_T("ids_title"), strTitle);//查询XML文件中关键字为ids_title的部分获取窗口标题
	CMainWnd main;
	RECT rtMain = { 100, 100, 700, 700 };
	main.CreateFromResource(_T("main_wnd"), NULL, rtMain, strTitle.c_str()
		, WS_VISIBLE | WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN | WS_CLIPSIBLINGS);

	SetTimer(main, 1, 20, NULL);

	//main.RegisterNotifyHandler(&main);

	pDUIApp->GetMessageLoop()->Run();

	pDUIApp->Term();

	return 0;
}



