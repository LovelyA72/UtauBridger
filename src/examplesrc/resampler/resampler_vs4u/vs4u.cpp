#include <Windows.h>
#include "resampler.h"
#include "vs4u_main.h"

//Resampler�ϳɹ��̣���������ͨEXE����˳����ͬ���14����������Լ��
extern "C" int WINAPI Exec(char* inputWaveFile, char* outputWaveFile, char* NoteStr, char* Velocity, char* Flags, char* Offset, char* RequiredLength, char* FixedLength, char* EndBlank, char* Volume, char* Modulation, char* Tempo, char* PitchString, int aSign)
{
	char* argv[14]={"",inputWaveFile,outputWaveFile,NoteStr,Velocity,Flags,Offset,RequiredLength,FixedLength,EndBlank,Volume,Modulation,Tempo,PitchString};//��������һ����������һ��������EXE��ڵ�EXE�ļ�����·��
	int ret=mainproc(14,argv);
	return ret;
}

//�жϷ����Ƿ���ڣ���һ������ΪĬ�Ϸ������ڶ�����������ǣ���Ϊ0��Ŀǰ�ò�����ϳ���ֱ�ӹ�����ֱ�ӷ���1���ɡ�
extern "C" int WINAPI IsExists(char* DefaultLyric,int isSign)
{
	return 1;
}