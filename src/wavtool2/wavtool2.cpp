////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ͷ�ļ�
#include <Windows.h>
#include <stdio.h>
#include "wavtool2.h"
#include "appender.h"
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

typedef int( __stdcall *DLL_FUNC_Exec)(char* outputWaveFile, char* beAppendWaveFile, char* StartPoint, char* LengthAtTempo, char* Env_P1, char* Env_P2, char* Env_P3, char* Env_V1, char* Env_V2, char* Env_V3, char* Env_V4, char* OverLap, char* Env_P4, char* Env_P5, char* Env_V5);
//typedef int( __stdcall *DLL_FUNC_AppendWhdDat)(char* p1, char* p2, char* p3);

HINSTANCE hdll=NULL;
DLL_FUNC_Exec D_Exec;
//DLL_FUNC_AppendWhdDat D_Append;

wchar_t* inifile;
wchar_t* currentEngine;
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ��ں���
BOOL WINAPI DllMain(HMODULE hModule, DWORD dwReason, PVOID pvReserved)
{
	if (dwReason == DLL_PROCESS_ATTACH)
	{
		wchar_t* file=(wchar_t*)malloc(256);
		currentEngine=(wchar_t*)malloc(256);
		DWORD fsize=GetModuleFileNameW(hModule,file,256);
		//resampler.dll
		//wavtool2.dll
		wchar_t* wfile=L"resampler.ini";
		memcpy((void*)((int)file+sizeof(wchar_t)*(fsize-12)),wfile,sizeof(wchar_t)*14);
		inifile=new wchar_t[wcslen(file)+1];
		wcscpy(inifile,file);
		fsize=GetPrivateProfileStringW(L"Engine2",L"Wavtool2",L"",file,256,inifile);
		wcscpy(currentEngine,file);
		wchar_t* filedir=(wchar_t*)malloc(256);
		wcscpy(filedir,file);
		wchar_t* pfildir=wcsrchr(filedir,'\\');
		if(pfildir!=NULL)
		{
			*pfildir='\0';
			SetDllDirectoryW(filedir);
		}
		hdll=LoadLibraryW(file);
		D_Exec=(DLL_FUNC_Exec)GetProcAddress(hdll,"exec");
		//D_Append=(DLL_FUNC_AppendWhdDat)GetProcAddress(hdll,"append_files");
	} else if(dwReason == DLL_PROCESS_DETACH){		
		FreeLibrary(hdll);
	}
	return TRUE;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void UpdateEngineTable()
{
	wchar_t* file=(wchar_t*)malloc(256);
	int str=GetPrivateProfileStringW(L"Engine2",L"Wavtool2",L"",file,256,inifile);
	int diff=wcsicmp(file,currentEngine);
	if(diff!=0)
	{
		D_Exec=0;
		//D_Append=0;
		if(hdll!=NULL)FreeLibrary(hdll);
		wcscpy(currentEngine,file);
		hdll=LoadLibraryW(file);
		D_Exec=(DLL_FUNC_Exec)GetProcAddress(hdll,"exec");
		//D_Append=(DLL_FUNC_AppendWhdDat)GetProcAddress(hdll,"append_files");
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ��������

extern "C" int WINAPI Exec(char* outputWaveFile, char* beAppendWaveFile, char* StartPoint, char* LengthAtTempo, char* Env_P1, char* Env_P2, char* Env_P3, char* Env_V1, char* Env_V2, char* Env_V3, char* Env_V4, char* OverLap, char* Env_P4, char* Env_P5, char* Env_V5)
{
	//˵����Exec���������ش���������ΪWavtool�Ĳ�������exe�汾��ͬ�������ط�0��ԭ��Ϊ-1��ʱ��ʾ�ϳɳɹ�������0Ϊʧ�ܡ�
	//����Ϊ������Wavtoolϵͳ���Ŵ���
	int r=0;

	UpdateEngineTable();
	if(hdll != NULL && ((int)D_Exec)!=0)
	{
		r=D_Exec(outputWaveFile,beAppendWaveFile,StartPoint,LengthAtTempo,Env_P1,Env_P2,Env_P3,Env_V1,Env_V2,Env_V3,Env_V4,OverLap,Env_P4,Env_P5,Env_V5);
		return r;
	}else
	{
		return 0;
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ��������
extern "C" int ModuleCheck()
{
	//˵������resampler���Ӧ��Ŀ��ͬ���ú���������֤����״̬�ģ�����0����
    return 0;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ��������
extern "C" int WINAPI AppendWhdDat(char* whdFile, char* datFile, char* outputWave)
{
	//��wav�ļ�ͷwhd�ļ������ӵ�wav�ļ���dat���ӣ����ΪoutpuWave�����ô˹��̱�ʾ����exec�����Ѿ����ͺ�ִ����ϡ��ù���ִ����ɺϲ�����Զ�ɾ��whd��dat�ļ�
	//������̱�������UTAUʹ��UTAU�����WAVTOOL2�ļ����еģ�����������wavtool.exe��wavtool4vcv��wavtoolex���ӳ����С�����Ϊ�˷�ֹʹ�ô�ͳ����ʱ��Ⱦ����BRIDGE�����ⲿ�ִ��������ˡ�
	//��������������������ϣ��ȡ���ⲿ�ֳ���ҲӦ������Щ��
	int ret;
	ret=WhdDatAppender(whdFile,datFile,outputWave);
    return ret;
}
/////////////