# Microsoft Developer Studio Project File - Name="DUILib" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=DUILib - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "DUILib.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "DUILib.mak" CFG="DUILib - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "DUILib - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "DUILib - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "DUILib - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "../../lib/release"
# PROP Intermediate_Dir "../../obj/release/DUILib"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "DUILIB_EXPORTS" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "../../include/DUILib" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "UNICODE" /D "_UNICODE" /D "_USRDLL" /D "DUILIB_EXPORTS" /D "MARKUP_STL" /FD /c
# SUBTRACT CPP /YX /Yc /Yu
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "NDEBUG"
# ADD RSC /l 0x804 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib Msimg32.lib /nologo /dll /machine:I386 /out:"../../bin/release/DUILib.dll" /libpath:"../../lib/release"

!ELSEIF  "$(CFG)" == "DUILib - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "../../lib/debug"
# PROP Intermediate_Dir "../../obj/debug/DUILib"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "DUILIB_EXPORTS" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "../../include/DUILib" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "UNICODE" /D "_UNICODE" /D "_USRDLL" /D "DUILIB_EXPORTS" /D "MARKUP_STL" /FD /GZ /c
# SUBTRACT CPP /YX /Yc /Yu
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "_DEBUG"
# ADD RSC /l 0x804 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib Msimg32.lib /nologo /dll /debug /machine:I386 /out:"../../bin/debug/DUILib.dll" /pdbtype:sept /libpath:"../../lib/debug"

!ENDIF 

# Begin Target

# Name "DUILib - Win32 Release"
# Name "DUILib - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Group "Control_CPP"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\DUIAnimation.cpp
# End Source File
# Begin Source File

SOURCE=.\DUIButton.cpp
# End Source File
# Begin Source File

SOURCE=.\DUICanvas.cpp
# End Source File
# Begin Source File

SOURCE=.\DUIComboBox.cpp
# End Source File
# Begin Source File

SOURCE=.\DUIControlBase.cpp
# End Source File
# Begin Source File

SOURCE=.\DUIControlContainer.cpp
# End Source File
# Begin Source File

SOURCE=.\DUIDecorate.cpp
# End Source File
# Begin Source File

SOURCE=.\DUILabel.cpp
# End Source File
# Begin Source File

SOURCE=.\DUIListBox.cpp
# End Source File
# Begin Source File

SOURCE=.\DUIMenu.cpp
# End Source File
# Begin Source File

SOURCE=.\DUIPictureBox.cpp
# End Source File
# Begin Source File

SOURCE=.\DUIProgressBar.cpp
# End Source File
# Begin Source File

SOURCE=.\DUIRichEdit.cpp
# End Source File
# Begin Source File

SOURCE=.\DUIScrollbar.cpp
# End Source File
# Begin Source File

SOURCE=.\DUISplit.cpp
# End Source File
# Begin Source File

SOURCE=.\DUIToolBar.cpp
# End Source File
# Begin Source File

SOURCE=.\DUITrackBar.cpp
# End Source File
# Begin Source File

SOURCE=.\DUITreeView.cpp
# End Source File
# Begin Source File

SOURCE=.\DUIWinObject.cpp
# End Source File
# End Group
# Begin Group "Common_CPP"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\CMarkup.cpp
# End Source File
# Begin Source File

SOURCE=.\CMarkup.h
# End Source File
# Begin Source File

SOURCE=.\DUIBase.cpp
# End Source File
# Begin Source File

SOURCE=.\DUIBitBuffer.cpp
# End Source File
# Begin Source File

SOURCE=.\DUIBkgnd.cpp
# End Source File
# Begin Source File

SOURCE=.\DUIColor.cpp
# End Source File
# Begin Source File

SOURCE=.\DUICursor.cpp
# End Source File
# Begin Source File

SOURCE=.\DUIDdb.cpp
# End Source File
# Begin Source File

SOURCE=.\DUIDebug.cpp
# End Source File
# Begin Source File

SOURCE=.\DUIDib.cpp
# End Source File
# Begin Source File

SOURCE=.\DUIDibFilter.cpp
# End Source File
# Begin Source File

SOURCE=.\DUIDibProcess.cpp
# End Source File
# Begin Source File

SOURCE=.\DUIFile.cpp
# End Source File
# Begin Source File

SOURCE=.\DUIFont.cpp
# End Source File
# Begin Source File

SOURCE=.\DUIImage.cpp
# End Source File
# Begin Source File

SOURCE=.\DUIRenderEngine.cpp
# End Source File
# Begin Source File

SOURCE=.\DUIString.cpp
# End Source File
# Begin Source File

SOURCE=.\DUITooltip.cpp
# End Source File
# Begin Source File

SOURCE=.\DUIUtil.cpp
# End Source File
# Begin Source File

SOURCE=.\stb_image.c
# End Source File
# End Group
# Begin Group "Manager_CPP"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\DUIApp.cpp
# End Source File
# Begin Source File

SOURCE=.\DUIChildWindow.cpp
# End Source File
# Begin Source File

SOURCE=.\DUIControlBuilder.cpp
# End Source File
# Begin Source File

SOURCE=.\DUIControlFactory.cpp
# End Source File
# Begin Source File

SOURCE=.\DUIPopupWindow.cpp
# End Source File
# Begin Source File

SOURCE=.\DUIResLoader.cpp
# End Source File
# Begin Source File

SOURCE=.\DUIResMgr.cpp
# End Source File
# Begin Source File

SOURCE=.\DUISkinMgr.cpp
# End Source File
# Begin Source File

SOURCE=.\DUIWindowBase.cpp
# End Source File
# Begin Source File

SOURCE=.\DUIWindowMgr.cpp
# End Source File
# End Group
# Begin Source File

SOURCE=.\DUIControls.cpp
# End Source File
# Begin Source File

SOURCE=.\DUILib.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Group "Control_H"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\include\DUILib\DUIAnimation.h
# End Source File
# Begin Source File

SOURCE=..\..\include\DUILib\DUIButton.h
# End Source File
# Begin Source File

SOURCE=..\..\include\DUILib\DUICanvas.h
# End Source File
# Begin Source File

SOURCE=..\..\include\DUILib\DUIComboBox.h
# End Source File
# Begin Source File

SOURCE=..\..\include\DUILib\DUIControlBase.h
# End Source File
# Begin Source File

SOURCE=..\..\include\DUILib\DUIControlContainer.h
# End Source File
# Begin Source File

SOURCE=..\..\include\DUILib\DUIDecorate.h
# End Source File
# Begin Source File

SOURCE=..\..\include\DUILib\DUILabel.h
# End Source File
# Begin Source File

SOURCE=..\..\include\DUILib\DUIListBox.h
# End Source File
# Begin Source File

SOURCE=..\..\include\DUILib\DUIMenu.h
# End Source File
# Begin Source File

SOURCE=..\..\include\DUILib\DUIPictureBox.h
# End Source File
# Begin Source File

SOURCE=..\..\include\DUILib\DUIProgressBar.h
# End Source File
# Begin Source File

SOURCE=..\..\include\DUILib\DUIRichEdit.h
# End Source File
# Begin Source File

SOURCE=..\..\include\DUILib\DUIScrollbar.h
# End Source File
# Begin Source File

SOURCE=..\..\include\DUILib\DUISplit.h
# End Source File
# Begin Source File

SOURCE=..\..\include\DUILib\DUIToolBar.h
# End Source File
# Begin Source File

SOURCE=..\..\include\DUILib\DUITrackBar.h
# End Source File
# Begin Source File

SOURCE=..\..\include\DUILib\DUITreeView.h
# End Source File
# Begin Source File

SOURCE=..\..\include\DUILib\DUIWinObject.h
# End Source File
# End Group
# Begin Group "Common_H"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\include\DUILib\DUIAtl.h
# End Source File
# Begin Source File

SOURCE=..\..\include\DUILib\DUIBase.h
# End Source File
# Begin Source File

SOURCE=..\..\include\DUILib\DUIBitBuffer.h
# End Source File
# Begin Source File

SOURCE=..\..\include\DUILib\DUIBkgnd.h
# End Source File
# Begin Source File

SOURCE=..\..\include\DUILib\DUIColor.h
# End Source File
# Begin Source File

SOURCE=..\..\include\DUILib\DUICursor.h
# End Source File
# Begin Source File

SOURCE=.\DUIDdb.h
# End Source File
# Begin Source File

SOURCE=..\..\include\DUILib\DUIDebug.h
# End Source File
# Begin Source File

SOURCE=..\..\include\DUILib\DUIDef.h
# End Source File
# Begin Source File

SOURCE=..\..\include\DUILib\DUIDib.h
# End Source File
# Begin Source File

SOURCE=..\..\include\DUILib\DUIDibFilter.h
# End Source File
# Begin Source File

SOURCE=..\..\include\DUILib\DUIDibProcess.h
# End Source File
# Begin Source File

SOURCE=..\..\include\DUILib\DUIFile.h
# End Source File
# Begin Source File

SOURCE=..\..\include\DUILib\DUIFont.h
# End Source File
# Begin Source File

SOURCE=..\..\include\DUILib\DUIImage.h
# End Source File
# Begin Source File

SOURCE=..\..\include\DUILib\DUIRef.h
# End Source File
# Begin Source File

SOURCE=..\..\include\DUILib\DUIRenderEngine.h
# End Source File
# Begin Source File

SOURCE=..\..\include\DUILib\DUIString.h
# End Source File
# Begin Source File

SOURCE=.\DUITooltip.h
# End Source File
# Begin Source File

SOURCE=.\DUIUtil.h
# End Source File
# Begin Source File

SOURCE=.\DUIWinVer.h
# End Source File
# Begin Source File

SOURCE=.\DUIXml.h
# End Source File
# End Group
# Begin Group "Manager_H"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\include\DUILib\DUIApp.h
# End Source File
# Begin Source File

SOURCE=..\..\include\DUILib\DUIChildWindow.h
# End Source File
# Begin Source File

SOURCE=..\..\include\DUILib\DUIControlBuilder.h
# End Source File
# Begin Source File

SOURCE=..\..\include\DUILib\DUIControlFactory.h
# End Source File
# Begin Source File

SOURCE=..\..\include\DUILib\DUIPopupWindow.h
# End Source File
# Begin Source File

SOURCE=..\..\include\DUILib\DUIResLoader.h
# End Source File
# Begin Source File

SOURCE=..\..\include\DUILib\DUIResMgr.h
# End Source File
# Begin Source File

SOURCE=..\..\include\DUILib\DUISkinMgr.h
# End Source File
# Begin Source File

SOURCE=..\..\include\DUILib\DUIWindowBase.h
# End Source File
# Begin Source File

SOURCE=..\..\include\DUILib\DUIWindowMgr.h
# End Source File
# End Group
# Begin Source File

SOURCE=..\..\include\DUILib\DUIControls.h
# End Source File
# Begin Source File

SOURCE=..\..\include\DUILib\DUILib.h
# End Source File
# Begin Source File

SOURCE=..\..\include\DUILib\DUILibDef.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
