////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ��������
#pragma comment(linker, "/EXPORT:moduleloader=_ModuleCheck")
#pragma comment(linker, "/EXPORT:append_files=_AppendWhdDat@12")
#pragma comment(linker, "/EXPORT:exec=_Exec@60")
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//WAVTOOL����ִ�к���������wavtool����
extern "C" int WINAPI Exec(char* outputWaveFile, char* beAppendWaveFile, char* StartPoint, char* LengthAtTempo, char* Env_P1, char* Env_P2, char* Env_P3, char* Env_V1, char* Env_V2, char* Env_V3, char* Env_V4, char* OverLap, char* Env_P4, char* Env_P5, char* Env_V5);
//WAV�ļ��ϲ����̽����������ϲ�wavtool�������ɵ�wavͷ��temp.whd)�ͱ����ӵ�wav�ļ��壨temp.dat)��Ŀ���ļ�(temp.wav)����־��wavtool�ϳɹ������ս���
extern "C" int WINAPI AppendWhdDat(char* whdFile, char* datFile, char* outputWave);
//ģ���⺯�������ģ���Ƿ������ʹ��ڣ����÷���0;
extern "C" int ModuleCheck();

