
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// �������������º���������UTAUϵͳ����
#pragma comment(linker, "/EXPORT:exec=_Exec@56")
#pragma comment(linker, "/EXPORT:isexists=_IsExists@8")
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//Resampler�ϳɹ��̣���������ͨEXE����˳����ͬ���14����������Լ��
extern "C" int WINAPI Exec(char* inputWaveFile, char* outputWaveFile, char* NoteStr, char* Velocity, char* Flags, char* Offset, char* RequiredLength, char* FixedLength, char* EndBlank, char* Volume, char* Modulation, char* Tempo, char* PitchString, int aSign);
//�жϷ����Ƿ���ڣ���һ������ΪĬ�Ϸ������ڶ�����������ǣ���Ϊ0��Ŀǰ�ò�����ϳ���ֱ�ӹ�����ֱ�ӷ���1���ɡ�
extern "C" int WINAPI IsExists(char* DefaultLyric,int isSign);
