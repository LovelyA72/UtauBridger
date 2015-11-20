
//--------------------------------------------------------------
// VS4U
//--------------------------------------------------------------
#include "vslib.h"

#ifndef _VS4U_H_
#define _VS4U_H_

//--------------------------------------------------------------
// �ޥ����x
//--------------------------------------------------------------
#define M_PI			(3.14159265358979323846)

#define VS4U_MAX_PATH	(260)	// �ѥ����L��

#define VS4U_PBSIZE		(65536)	// �ԥå��٥�ɥǩ`���åХåե�������(120BPM�Έ���341���)

//#define PRINT_TIME			// �I��r�g��ʾ
//#define PRINT_OPTION			// ���ץ�����ʾ


//--------------------------------------------------------------
// �����嶨�x
//--------------------------------------------------------------

// ���ץ����ʤɱ�Ҫ�ʥǩ`����VS4UOPTION�˶��x����
typedef struct{
	// �ե�������
	char inputWaveFile[VS4U_MAX_PATH];	// ����WAVE�ե�����
	char outputWaveFile[VS4U_MAX_PATH];	// ����WAVE�ե�����
	char frqFile[VS4U_MAX_PATH];		// �˜��ܲ�����ե�����
	char vsfrqFile[VS4U_MAX_PATH];		// �����ܲ�����ե�����
	char vspFile[VS4U_MAX_PATH];		// �ץ������ȥե�����(�ǥХå���)

	// ���ץ����
	int noteNum;				// �������A[�Ω`�ȥʥ�Щ`(C4=60)]
	double offset;				// ���ե��å�[ms]
	double reqLength;			// Ҫ���L[ms]
	double fixedLength;			// ������[ms]
	double endBlank;			// �֥��[ms](ؓ�Έ��ϡ����ե��åȤ����������)
	double fixSpeed;			// �����ٶ�(0��2��,100��1��,200��0.5��)

	double volume;				// �ܥ��`��[%](0��200)
	double modulation;			// �⥸���`�����[%](-200��200)
	double tempo;				// �ƥ��[BPM](10��512)
	int nPitchBend;				// �ԥå��٥�ɥǩ`����	
	int pitchBend[VS4U_PBSIZE];	// �ԥå��٥�ɥǩ`��[cent](-2048��2047)
								// ��(���ٻ��ĩ`���ʹ�äƌg�Ф����H���h���ˤ�äƤτӵĴ_����
								//    ʧ�����뤳�Ȥ����뤿�᥹���å�����_����ԭ��ϲ���)

	int noFormantFlg;			// �ե���ޥ���{���ʤ��ե饰	("N",���ڂ�OFF)
	int genderFactor;			// ��������`�ե������`[-20cent]("g",-100��100,���ڂ�0)
	int pitchShift;				// �ԥå�΢�{��[10cent]			("t",-240��240,���ڂ�0)
	int pitchShift2;			// �ԥå�΢�{��[1cent]			("T",-2400��2400,���ڂ�0)
	int peekCmp;				// �ԩ`������ץ�å�			("P",0��100,���ڂ�86)
	int lowPassfilter;			// ��`�ѥ��ե��륿				("H",0��99,���ڂ�0)
	int breathiness;			// �֥쥷�ͥ�					("B",0��100,���ڂ�50)
	int brightness;				// �֥饤�ȥͥ�					("O",-100��100,���ڂ�0)
	int clearness;				// ���ꥢ�ͥ�					("S",-100��100,���ڂ�0)
	int outputVshpFlg;			// VSHP�����ե饰				("Z",���ڂ�OFF)
	int reGenerateFrqFlg;		// �ܲ����������ɥե饰			("V",���ڂ�OFF)
	int disablePitchShift;		// �ԥå����եȟo�����ե饰		("W",������0)
								//	-2:������`��,PS�o�� -1:�g����`��,PS�o�� 0:PS�Є�

	// �I����
	short *tmpWaveData;			// һ�r�ǩ`��
	int tmpSample;				// һ�r�ǩ`������ץ���
	int tmpSmpFreq;				// һ�r�ǩ`���Υ���ץ���ܲ���
} VS4UOPTION;

// �ܲ����ǩ`��
typedef struct{
	double freq;
	double dyn;
} FREQDATA;


//--------------------------------------------------------------
// �ץ�ȥ���������
//--------------------------------------------------------------

// ��ͨ
double LimitDoubleValue( double value, double min, double max );
int LimitIntValue( int value, int min, int max );
void AddExtStr( char *dstStr, int dstSize, char *extStr );
int CheckFileExist( char *fileName );

// ���ץ�����O��
int Vs4uGetOption( int argc, char *argv[], VS4UOPTION *option );
int Vs4uCnvNoteNum( char *nnStr );
int Vs4uSetFlgOption( char *flgStr, VS4UOPTION *option );
int Vs4uDecodeBase64( char *str );
int Vs4uSetPitchBendOption( char *argv[], VS4UOPTION *option );
void Vs4uPrintOption( VS4UOPTION *option );

// �ԥå����ե���ޥ�ȡ������ߥ�
int Vs4uStep1( VS4UOPTION *option );
HVSPRJ Vs4uLoadProject( VS4UOPTION *option );
int Vs4uSetTimeStretchPrm( HVSPRJ hVsprj, VS4UOPTION *option );
int Vs4uLoadFrqFile( HVSPRJ hVsprj, VS4UOPTION *option );
int Vs4uCalcAveragePit( HVSPRJ hVsprj, VS4UOPTION *option );
int Vs4uSetPitPrm( HVSPRJ hVsprj, VS4UOPTION *option );

// ��`�ѥ��ե��륿���ܥ��`��
int Vs4uStep2( VS4UOPTION *option );
int Vs4uLowPassFilter( VS4UOPTION *option );
int Vs4uVolume( VS4UOPTION *option );
int Vs4uOutputWaveFile( VS4UOPTION *option );

#endif /* _VS4U_H_ */

