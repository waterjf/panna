# Microsoft Developer Studio Project File - Name="pnEditor" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=pnEditor - Win32 DLL Universal Release
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "pnEditor.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "pnEditor.mak" CFG="pnEditor - Win32 DLL Universal Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "pnEditor - Win32 DLL Universal Release" (based on "Win32 (x86) Application")
!MESSAGE "pnEditor - Win32 DLL Universal Debug" (based on "Win32 (x86) Application")
!MESSAGE "pnEditor - Win32 Universal Release" (based on "Win32 (x86) Application")
!MESSAGE "pnEditor - Win32 Universal Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "pnEditor - Win32 DLL Universal Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "vc_mswunivudll"
# PROP BASE Intermediate_Dir "vc_mswunivudll\pnEditor"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "vc_mswunivudll"
# PROP Intermediate_Dir "vc_mswunivudll\pnEditor"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W4 /GR /Zi /O2 /I ".\..\..\lib\vc_dll\mswunivu" /I ".\..\..\include" /I "." /I ".\..\..\samples" /D "WIN32" /D "__WXMSW__" /D "__WXUNIVERSAL__" /D "NDEBUG" /D "_UNICODE" /D "WXUSINGDLL" /D "_WINDOWS" /D "NOPCH" /Fd"vc_mswunivudll\pnEditor.pdb" /FD /opt:ref /opt:icf /EHsc /c
# ADD CPP /nologo /MD /W4 /GR /Zi /O2 /I ".\..\..\lib\vc_dll\mswunivu" /I ".\..\..\include" /I "." /I ".\..\..\samples" /D "WIN32" /D "__WXMSW__" /D "__WXUNIVERSAL__" /D "NDEBUG" /D "_UNICODE" /D "WXUSINGDLL" /D "_WINDOWS" /D "NOPCH" /Fd"vc_mswunivudll\pnEditor.pdb" /FD /opt:ref /opt:icf /EHsc /c
# ADD BASE MTL /nologo /D "WIN32" /D "__WXMSW__" /D "__WXUNIVERSAL__" /D "NDEBUG" /D "_UNICODE" /D "WXUSINGDLL" /D "_WINDOWS" /D "NOPCH" /mktyplib203 /win32
# ADD MTL /nologo /D "WIN32" /D "__WXMSW__" /D "__WXUNIVERSAL__" /D "NDEBUG" /D "_UNICODE" /D "WXUSINGDLL" /D "_WINDOWS" /D "NOPCH" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /i ".\..\..\lib\vc_dll\mswunivu" /i ".\..\..\include" /i "." /i ".\..\..\samples" /d "__WXMSW__" /d "__WXUNIVERSAL__" /d "NDEBUG" /d "_UNICODE" /d "WXUSINGDLL" /d "_WINDOWS" /d "NOPCH"
# ADD RSC /l 0x409 /i ".\..\..\lib\vc_dll\mswunivu" /i ".\..\..\include" /i "." /i ".\..\..\samples" /d "__WXMSW__" /d "__WXUNIVERSAL__" /d "NDEBUG" /d "_UNICODE" /d "WXUSINGDLL" /d "_WINDOWS" /d "NOPCH"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 wxmswuniv29u_aui.lib wxmswuniv29u_html.lib wxmswuniv29u_adv.lib wxmswuniv29u_core.lib wxbase29u_xml.lib wxbase29u.lib wxtiff.lib wxjpeg.lib wxpng.lib wxzlib.lib wxregexu.lib wxexpat.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib wininet.lib /nologo /subsystem:windows /debug /machine:I386 /libpath:".\..\..\lib\vc_dll"
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 wxmswuniv29u_aui.lib wxmswuniv29u_html.lib wxmswuniv29u_adv.lib wxmswuniv29u_core.lib wxbase29u_xml.lib wxbase29u.lib wxtiff.lib wxjpeg.lib wxpng.lib wxzlib.lib wxregexu.lib wxexpat.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib wininet.lib /nologo /subsystem:windows /debug /machine:I386 /libpath:".\..\..\lib\vc_dll"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "pnEditor - Win32 DLL Universal Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "vc_mswunivuddll"
# PROP BASE Intermediate_Dir "vc_mswunivuddll\pnEditor"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "vc_mswunivuddll"
# PROP Intermediate_Dir "vc_mswunivuddll\pnEditor"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W4 /Gm /GR /Zi /Od /I ".\..\..\lib\vc_dll\mswunivud" /I ".\..\..\include" /I "." /I ".\..\..\samples" /D "WIN32" /D "_DEBUG" /D "__WXMSW__" /D "__WXUNIVERSAL__" /D "_UNICODE" /D "WXUSINGDLL" /D "_WINDOWS" /D "NOPCH" /Fd"vc_mswunivuddll\pnEditor.pdb" /FD /EHsc /c
# ADD CPP /nologo /MDd /W4 /Gm /GR /Zi /Od /I ".\..\util\wxWidgets\lib\vc_dll\mswunivud" /I ".\..\util\wxWidgets\include" /I "." /D "WIN32" /D "_DEBUG" /D "__WXMSW__" /D "__WXUNIVERSAL__" /D "_UNICODE" /D "WXUSINGDLL" /D "_WINDOWS" /D "NOPCH" /Yu"precomp.h" /Fd"vc_mswunivuddll\pnEditor.pdb" /FD /EHsc /c
# ADD BASE MTL /nologo /D "WIN32" /D "_DEBUG" /D "__WXMSW__" /D "__WXUNIVERSAL__" /D "_UNICODE" /D "WXUSINGDLL" /D "_WINDOWS" /D "NOPCH" /mktyplib203 /win32
# ADD MTL /nologo /D "WIN32" /D "_DEBUG" /D "__WXMSW__" /D "__WXUNIVERSAL__" /D "_UNICODE" /D "WXUSINGDLL" /D "_WINDOWS" /D "NOPCH" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /i ".\..\..\lib\vc_dll\mswunivud" /i ".\..\..\include" /i "." /i ".\..\..\samples" /d "_DEBUG" /d "__WXMSW__" /d "__WXUNIVERSAL__" /d "_UNICODE" /d "WXUSINGDLL" /d "_WINDOWS" /d "NOPCH"
# ADD RSC /l 0x409 /i ".\..\..\lib\vc_dll\mswunivud" /i ".\..\..\include" /i "." /i ".\..\..\samples" /d "_DEBUG" /d "__WXMSW__" /d "__WXUNIVERSAL__" /d "_UNICODE" /d "WXUSINGDLL" /d "_WINDOWS" /d "NOPCH"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 wxmswuniv29ud_aui.lib wxmswuniv29ud_html.lib wxmswuniv29ud_adv.lib wxmswuniv29ud_core.lib wxbase29ud_xml.lib wxbase29ud.lib wxtiffd.lib wxjpegd.lib wxpngd.lib wxzlibd.lib wxregexud.lib wxexpatd.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib wininet.lib /nologo /subsystem:windows /debug /machine:I386 /libpath:".\..\..\lib\vc_dll"
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 wxmswuniv29ud_aui.lib wxmswuniv29ud_html.lib wxmswuniv29ud_adv.lib wxmswuniv29ud_core.lib wxbase29ud_xml.lib wxbase29ud.lib wxtiffd.lib wxjpegd.lib wxpngd.lib wxzlibd.lib wxregexud.lib wxexpatd.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib wininet.lib /nologo /subsystem:windows /debug /machine:I386 /libpath:".\..\util\wxWidgets\lib\vc_dll"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "pnEditor - Win32 Universal Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "vc_mswunivu"
# PROP BASE Intermediate_Dir "vc_mswunivu\pnEditor"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "vc_mswunivu"
# PROP Intermediate_Dir "vc_mswunivu\pnEditor"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W4 /GR /Zi /O2 /I ".\..\..\lib\vc_lib\mswunivu" /I ".\..\..\include" /I "." /I ".\..\..\samples" /D "WIN32" /D "__WXMSW__" /D "__WXUNIVERSAL__" /D "NDEBUG" /D "_UNICODE" /D "_WINDOWS" /D "NOPCH" /Fd"vc_mswunivu\pnEditor.pdb" /FD /opt:ref /opt:icf /EHsc /c
# ADD CPP /nologo /MD /W4 /GR /Zi /O2 /I ".\..\..\lib\vc_lib\mswunivu" /I ".\..\..\include" /I "." /I ".\..\..\samples" /D "WIN32" /D "__WXMSW__" /D "__WXUNIVERSAL__" /D "NDEBUG" /D "_UNICODE" /D "_WINDOWS" /D "NOPCH" /Fd"vc_mswunivu\pnEditor.pdb" /FD /opt:ref /opt:icf /EHsc /c
# ADD BASE MTL /nologo /D "WIN32" /D "__WXMSW__" /D "__WXUNIVERSAL__" /D "NDEBUG" /D "_UNICODE" /D "_WINDOWS" /D "NOPCH" /mktyplib203 /win32
# ADD MTL /nologo /D "WIN32" /D "__WXMSW__" /D "__WXUNIVERSAL__" /D "NDEBUG" /D "_UNICODE" /D "_WINDOWS" /D "NOPCH" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /i ".\..\..\lib\vc_lib\mswunivu" /i ".\..\..\include" /i "." /i ".\..\..\samples" /d "__WXMSW__" /d "__WXUNIVERSAL__" /d "NDEBUG" /d "_UNICODE" /d "_WINDOWS" /d "NOPCH"
# ADD RSC /l 0x409 /i ".\..\..\lib\vc_lib\mswunivu" /i ".\..\..\include" /i "." /i ".\..\..\samples" /d "__WXMSW__" /d "__WXUNIVERSAL__" /d "NDEBUG" /d "_UNICODE" /d "_WINDOWS" /d "NOPCH"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 wxmswuniv29u_aui.lib wxmswuniv29u_html.lib wxmswuniv29u_adv.lib wxmswuniv29u_core.lib wxbase29u_xml.lib wxbase29u.lib wxtiff.lib wxjpeg.lib wxpng.lib wxzlib.lib wxregexu.lib wxexpat.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib wininet.lib /nologo /subsystem:windows /debug /machine:I386 /libpath:".\..\..\lib\vc_lib"
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 wxmswuniv29u_aui.lib wxmswuniv29u_html.lib wxmswuniv29u_adv.lib wxmswuniv29u_core.lib wxbase29u_xml.lib wxbase29u.lib wxtiff.lib wxjpeg.lib wxpng.lib wxzlib.lib wxregexu.lib wxexpat.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib wininet.lib /nologo /subsystem:windows /debug /machine:I386 /libpath:".\..\..\lib\vc_lib"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "pnEditor - Win32 Universal Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "vc_mswunivud"
# PROP BASE Intermediate_Dir "vc_mswunivud\pnEditor"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "vc_mswunivud"
# PROP Intermediate_Dir "vc_mswunivud\pnEditor"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W4 /Gm /GR /Zi /Od /I ".\..\..\lib\vc_lib\mswunivud" /I ".\..\..\include" /I "." /I ".\..\..\samples" /D "WIN32" /D "_DEBUG" /D "__WXMSW__" /D "__WXUNIVERSAL__" /D "_UNICODE" /D "_WINDOWS" /D "NOPCH" /Fd"vc_mswunivud\pnEditor.pdb" /FD /EHsc /c
# ADD CPP /nologo /MDd /W4 /Gm /GR /Zi /Od /I ".\..\..\lib\vc_lib\mswunivud" /I ".\..\..\include" /I "." /I ".\..\..\samples" /D "WIN32" /D "_DEBUG" /D "__WXMSW__" /D "__WXUNIVERSAL__" /D "_UNICODE" /D "_WINDOWS" /D "NOPCH" /Fd"vc_mswunivud\pnEditor.pdb" /FD /EHsc /c
# ADD BASE MTL /nologo /D "WIN32" /D "_DEBUG" /D "__WXMSW__" /D "__WXUNIVERSAL__" /D "_UNICODE" /D "_WINDOWS" /D "NOPCH" /mktyplib203 /win32
# ADD MTL /nologo /D "WIN32" /D "_DEBUG" /D "__WXMSW__" /D "__WXUNIVERSAL__" /D "_UNICODE" /D "_WINDOWS" /D "NOPCH" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /i ".\..\..\lib\vc_lib\mswunivud" /i ".\..\..\include" /i "." /i ".\..\..\samples" /d "_DEBUG" /d "__WXMSW__" /d "__WXUNIVERSAL__" /d "_UNICODE" /d "_WINDOWS" /d "NOPCH"
# ADD RSC /l 0x409 /i ".\..\..\lib\vc_lib\mswunivud" /i ".\..\..\include" /i "." /i ".\..\..\samples" /d "_DEBUG" /d "__WXMSW__" /d "__WXUNIVERSAL__" /d "_UNICODE" /d "_WINDOWS" /d "NOPCH"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 wxmswuniv29ud_aui.lib wxmswuniv29ud_html.lib wxmswuniv29ud_adv.lib wxmswuniv29ud_core.lib wxbase29ud_xml.lib wxbase29ud.lib wxtiffd.lib wxjpegd.lib wxpngd.lib wxzlibd.lib wxregexud.lib wxexpatd.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib wininet.lib /nologo /subsystem:windows /debug /machine:I386 /libpath:".\..\..\lib\vc_lib"
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 wxmswuniv29ud_aui.lib wxmswuniv29ud_html.lib wxmswuniv29ud_adv.lib wxmswuniv29ud_core.lib wxbase29ud_xml.lib wxbase29ud.lib wxtiffd.lib wxjpegd.lib wxpngd.lib wxzlibd.lib wxregexud.lib wxexpatd.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib wininet.lib /nologo /subsystem:windows /debug /machine:I386 /libpath:".\..\..\lib\vc_lib"
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "pnEditor - Win32 DLL Universal Release"
# Name "pnEditor - Win32 DLL Universal Debug"
# Name "pnEditor - Win32 Universal Release"
# Name "pnEditor - Win32 Universal Debug"
# Begin Group "framework cpp"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\pnEditor.cpp
# End Source File
# Begin Source File

SOURCE=.\pnEditorFrame.cpp
# End Source File
# Begin Source File

SOURCE=.\SettingsPanel.cpp
# End Source File
# Begin Source File

SOURCE=.\wxSizeReportCtrl.cpp
# End Source File
# End Group
# Begin Group "framework header"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\pnEditorFrame.h
# End Source File
# Begin Source File

SOURCE=.\SettingsPanel.h
# End Source File
# Begin Source File

SOURCE=.\wxSizeReportCtrl.h
# End Source File
# End Group
# Begin Group "precompile"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\precomp.cpp

!IF  "$(CFG)" == "pnEditor - Win32 DLL Universal Release"

!ELSEIF  "$(CFG)" == "pnEditor - Win32 DLL Universal Debug"

# ADD CPP /Yc"precomp.h"

!ELSEIF  "$(CFG)" == "pnEditor - Win32 Universal Release"

!ELSEIF  "$(CFG)" == "pnEditor - Win32 Universal Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\precomp.h
# End Source File
# End Group
# Begin Group "shape cpp"

# PROP Default_Filter "px*.cpp"
# Begin Source File

SOURCE=.\pxCanvas.cpp
# End Source File
# Begin Source File

SOURCE=.\pxCanvasWx2D.cpp
# End Source File
# Begin Source File

SOURCE=.\pxShapeArc.cpp
# End Source File
# Begin Source File

SOURCE=.\pxShapeBase.cpp
# End Source File
# Begin Source File

SOURCE=.\pxShapeEllipse.cpp
# End Source File
# Begin Source File

SOURCE=.\pxShapeIOStream.cpp
# End Source File
# Begin Source File

SOURCE=.\pxShapeLine.cpp
# End Source File
# Begin Source File

SOURCE=.\pxShapePolygon.cpp
# End Source File
# Begin Source File

SOURCE=.\pxShapeText.cpp
# End Source File
# End Group
# Begin Group "shape header"

# PROP Default_Filter "px*.h"
# Begin Source File

SOURCE=.\pxCanvas.h
# End Source File
# Begin Source File

SOURCE=.\pxCanvasWx2D.h
# End Source File
# Begin Source File

SOURCE=.\pxDef.h
# End Source File
# Begin Source File

SOURCE=.\pxShapeArc.h
# End Source File
# Begin Source File

SOURCE=.\pxShapeBase.h
# End Source File
# Begin Source File

SOURCE=.\pxShapeEllipse.h
# End Source File
# Begin Source File

SOURCE=.\pxShapeIOStream.h
# End Source File
# Begin Source File

SOURCE=.\pxShapeLine.h
# End Source File
# Begin Source File

SOURCE=.\pxShapePolygon.h
# End Source File
# Begin Source File

SOURCE=.\pxShapeText.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\pxShapeAnimator.cpp
# End Source File
# Begin Source File

SOURCE=.\pxShapeAnimator.h
# End Source File
# Begin Source File

SOURCE=.\pxShapeImg.cpp
# End Source File
# Begin Source File

SOURCE=.\pxShapeImg.h
# End Source File
# End Target
# End Project
