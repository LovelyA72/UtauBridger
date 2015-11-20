////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ͷ�ļ�
#include <Windows.h>
#include <stdio.h>
#include <io.h>
#include "AutoTranslater.h"
#include "resampler.h"
#include <locale.h>

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


//Resampler�ϳɹ��̣���������ͨEXE����˳����ͬ���14����������Լ��
typedef int( __stdcall *DLL_FUNC_Exec)(char* inputWaveFile, char* outputWaveFile, char* NoteStr, char* Velocity, char* Flags, char* Offset, char* RequiredLength, char* FixedLength, char* EndBlank, char* Volume, char* Modulation, char* Tempo, char* PitchString, int aSign);
HINSTANCE hdll=NULL;
DLL_FUNC_Exec D_Exec;

int AutoTranslater=1;
wchar_t* inifile;
wchar_t* currentEngine;
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ��ں���
BOOL WINAPI DllMain(HMODULE hModule, DWORD dwReason, PVOID pvReserved)
{
	if (dwReason == DLL_PROCESS_ATTACH)
	{
		//����Ϊ��ʼ��
		wchar_t* file=(wchar_t*)malloc(256);
		currentEngine=(wchar_t*)malloc(256);
		int fsize=GetModuleFileNameW(hModule,file,256);
		file[fsize-1]='i';
		file[fsize-2]='n';
		file[fsize-3]='i';
		inifile=new wchar_t[sizeof(wchar_t) * (wcslen(file)+1)];
		wcscpy(inifile,file);
		AutoTranslater=GetPrivateProfileIntW(L"Engine2",L"AutoCodePage",1,inifile);
		fsize=GetPrivateProfileStringW(L"Engine2",L"Resampler",L"",file,256,inifile);
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
	} else if(dwReason == DLL_PROCESS_DETACH){		
		if(hdll!=NULL)FreeLibrary(hdll);
	}
	return TRUE;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void UpdateEngineTable()
{
	wchar_t* file=(wchar_t*)malloc(256);
	int str=GetPrivateProfileStringW(L"Engine2",L"Resampler",L"",file,256,inifile);	
	int diff=wcsicmp(file,currentEngine);
	if(diff!=0)
	{
		D_Exec=0;
		if(hdll!=NULL)FreeLibrary(hdll);
		wcscpy(currentEngine,file);
		hdll=LoadLibraryW(file);
		D_Exec=(DLL_FUNC_Exec)GetProcAddress(hdll,"exec");
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ��������

extern "C" int WINAPI Exec(char* inputWaveFile, char* outputWaveFile, char* NoteStr, char* Velocity, char* Flags, char* Offset, char* RequiredLength, char* FixedLength, char* EndBlank, char* Volume, char* Modulation, char* Tempo, char* PitchString, int aSign)
{
	//˵����Exec���������ش���������ΪResampler�Ĳ�����ӁExe�汾��ͬ�������ط�0��ԭ��Ϊ-1��ʱ��澺ϳɳɹ�������0Ϊʧ�ܡ�
	//����Ϊ������resampler�������
	int r=0;

	UpdateEngineTable();
	if(hdll != NULL && ((int)D_Exec)!=0)
	{
		//��ʼ����ַ�أ����Խ��е�ַת��
		char* newInputPath=(char*)malloc(sizeof(char)*MAX_PATH);
		strcpy(newInputPath,inputWaveFile);
		if(AutoTranslater!=0 && access(newInputPath,0)!=0)
		{			
			int IniACP=GetPrivateProfileIntW(L"Engine2",L"SystemLangID",-1,inifile);
			if(IniACP==-1 || !IsValidCodePage(IniACP))
			{
				IniACP=GetACP();
				if(IniACP == 932)
				{
					char* ww=setlocale(LC_ALL,"");
					char* cp=strrchr(ww, '.')+1;
					int cpi=atoi(cp);
					if(IsValidCodePage(cpi))IniACP=cpi;
				}
			}
			TranslateWAVPath(newInputPath,IniACP);
		}
		r=D_Exec(newInputPath,outputWaveFile,NoteStr,Velocity,Flags,Offset,RequiredLength,FixedLength,EndBlank,Volume,Modulation,Tempo,PitchString,aSign);
		return r;
	}else
	{
		return 0;
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ��������

extern "C" int WINAPI IsExists(char* DefaultLyric,int isSign)
{
	//˵����DefaultLyricһ��Ϊ����a��isSignΪ0���������utau��Դ�Ƿ����á�ֱ�ӷ���1
	return 1;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

