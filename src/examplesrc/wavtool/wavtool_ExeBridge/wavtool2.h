////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ��������
#pragma comment(linker, "/EXPORT:exec=_Exec@60")
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//WAVTOOL����ִ�к���������wavtool����
extern "C" int WINAPI Exec(char* outputWaveFile, char* beAppendWaveFile, char* StartPoint, char* LengthAtTempo, char* Env_P1, char* Env_P2, char* Env_P3, char* Env_V1, char* Env_V2, char* Env_V3, char* Env_V4, char* OverLap, char* Env_P4, char* Env_P5, char* Env_V5);


