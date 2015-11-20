
//--------------------------------------------------------------
// VocalShifter - VSLIB
//--------------------------------------------------------------

#ifndef _VSLIB_H_
#define _VSLIB_H_

#pragma comment( lib, "lib\\vslib.lib" )

#ifdef __cplusplus
#define VSLIB_IMPORT extern "C" __declspec(dllimport)
#else
#define VSLIB_IMPORT __declspec(dllimport)
#endif

//--------------------------------------------------------------
// �ޥ����x
//--------------------------------------------------------------

#define VSLIB_MAX_PATH		(256)
#define VSLIB_MAX_TRACK		(64)	// �ȥ�å������
#define VSLIB_MAX_ITEM		(1024)	// �����ƥ������

// ����`���`��
#define VSERR_NOERR			(0)		// ����`�ʤ�
#define VSERR_PRM			(1)		// �ѥ��`������
#define VSERR_PRJOPEN		(2)		// PRJ�ե����륪�`�ץ��ʧ��
#define VSERR_PRJFORMAT		(3)		// PRJ�ե�����ե��`�ޥåȲ���
#define VSERR_WAVEOPEN		(4)		// WAVE�ե����륪�`�ץ��ʧ��
#define VSERR_WAVEFORMAT	(5)		// WAVE�ե�����ե��`�ޥåȲ���
#define VSERR_FREQ			(6)		// ����ץ���ܲ�������
#define VSERR_MAX			(7)		// ���������
#define VSERR_NOMEM			(8)		// ���겻��

// �����ϳɷ�ʽ
#define SYNTHMODE_M			(0)		// �g��
#define SYNTHMODE_MF		(1)		// �g���ե���ޥ���a��
#define SYNTHMODE_P			(2)		// ����

// �ϥ�ɥ붨�x
typedef struct HVSPRJ__ { int unused; } *HVSPRJ;

// ASAnalyzer �����ͥ�
#define AS_CH_LPR			(0)
#define AS_CH_LMR			(1)
#define AS_CH_L				(2)
#define AS_CH_R				(3)

// ASAnalyzer ����`���`��
#define AS_ERR_NOERR		(0)
#define AS_ERR_NOMEM		(1)
#define AS_ERR_BADPRM		(2)
#define AS_ERR_OPEN			(3)
#define AS_ERR_FROMAT		(4)


//--------------------------------------------------------------
// �����嶨�x
//--------------------------------------------------------------

// �ץ����������
typedef struct{
	double masterVolume;	// (R/W) �ޥ����`�ܥ��`��[��]
	int sampFreq;			// (R/W) ����ץ���ܲ���[Hz]
} VSPRJINFO;

// �ȥ�å����
typedef struct{
	double volume;			// (R/W) �ܥ��`��[��]
	double pan;				// (R/W) �ѥ�(-1.0��1.0)
	int invPhaseFlg;		// (R/W) ����ե饰
	int soloFlg;			// (R/W) ����ե饰
	int muteFlg;			// (R/W) �ߥ�`�ȥե饰
} VSTRACKINFO;

// �����ƥ����
typedef struct{
	char fileName[VSLIB_MAX_PATH];	// (R/-) �ե�������
	int sampFreq;			// (R/-) ����ץ���ܲ���[Hz]
	int channel;			// (R/-) �����ͥ���
	int sampleOrg;			// (R/-) ���ꥸ�ʥ�������ե�����Υ���ץ���
	int sampleEdit;			// (R/-) ������Υ���ץ���
	int ctrlPntPs;			// (R/-) 1�뤢�������������
	int ctrlPntNum;			// (R/-) ȫ��������
	int synthMode;			// (R/W) �����ϳɷ�ʽ
	int trackNum;			// (R/W) �ȥ�å�����
	int offset;				// (R/W) ���ե��å�[sample]
} VSITEMINFO;

// �����ƥ����������
typedef struct{
	double dynOrg;			// (R/-) ����ǰ�����ʥߥ���[��]
	double dynEdit;			// (R/W) ����������ʥߥ���[��]
	double volume;			// (R/W) �ܥ��`��[��]
	double pan;				// (R/W) �ѥ�(-1.0��1.0)
	double spcDyn;			// (R/-) ���ڥ��ȥ�����ʥߥ���
	int pitAna;				// (R/-) �ԥå�������[cent]
	int pitOrg;				// (R/W) ����ǰ�ԥå�[cent]
	int pitEdit;			// (R/W) ������ԥå�[cent]
	int formant;			// (R/W) �ե���ޥ��[cent]
	int pitFlgOrg;			// (R/-) ����ǰ�ԥå��Пo�ե饰
	int pitFlgEdit;			// (R/W) ������ԥå��Пo�ե饰
	int breathiness;		// (R/W) �֥쥷�ͥ�[-10000��10000]
	int eq1;				// (R/W) EQ1[-10000��10000]
	int eq2;				// (R/W) EQ2[-10000��10000]
} VSCPINFOEX;

// ASAnalyzer AWDINFO������
typedef struct{
	int wavdatasize;	// �ǩ`���Υ�����[����ץ�]
	int wavsampleps;	// ����ץ���ܲ���[Hz]
	int wavbit;			// �ӥå���(8or16)
	int wavchannel;		// �����ͥ���(1or2)
	int nnoffset;		// �������������[�Ω`�ȥʥ�Щ`]
	int nnrange;		// �����������A�ι���[����]
	int blockpn;		// ����������Υ֥�å���(1-20)
	int dynsize;		// �����ʳ����g[����ץ�]
	int targetch;		// ����Υ����ͥ�
} AWDINFO;


//--------------------------------------------------------------
// �ץ�ȥ���������
//--------------------------------------------------------------

// �饤�֥��
VSLIB_IMPORT int __stdcall VslibGetVersion( void );

// �ץ�������
VSLIB_IMPORT int __stdcall VslibCreateProject( HVSPRJ *hVsprj );
VSLIB_IMPORT int __stdcall VslibOpenProject( HVSPRJ *hVsprj, char *fileName );
VSLIB_IMPORT int __stdcall VslibSaveProject( HVSPRJ hVsprj, char *fileName );
VSLIB_IMPORT int __stdcall VslibDeleteProject( HVSPRJ hVsprj );
VSLIB_IMPORT int __stdcall VslibGetProjectInfo( HVSPRJ hVsprj, VSPRJINFO *vsPrjInfo );
VSLIB_IMPORT int __stdcall VslibSetProjectInfo( HVSPRJ hVsprj, VSPRJINFO *vsPrjInfo );

// �ȥ�å�
VSLIB_IMPORT int __stdcall VslibGetTrackMaxNum( HVSPRJ hVsprj, int *trackMaxNum );
VSLIB_IMPORT int __stdcall VslibAddTrack( HVSPRJ hVsprj );
VSLIB_IMPORT int __stdcall VslibCopyTrack( HVSPRJ hVsprjDst, int *trackNumDst, HVSPRJ hVsprjSrc, int trackNumsrc );
VSLIB_IMPORT int __stdcall VslibUpTrack( HVSPRJ hVsprj, int trackNum );
VSLIB_IMPORT int __stdcall VslibDownTrack( HVSPRJ hVsprj, int trackNum );
VSLIB_IMPORT int __stdcall VslibDeleteTrack( HVSPRJ hVsprj, int trackNum );
VSLIB_IMPORT int __stdcall VslibGetTrackInfo( HVSPRJ hVsprj, int trackNum, VSTRACKINFO *vsTrackInfo );
VSLIB_IMPORT int __stdcall VslibSetTrackInfo( HVSPRJ hVsprj, int trackNum, VSTRACKINFO *vsTrackInfo );

// �����ƥ�
VSLIB_IMPORT int __stdcall VslibGetItemMaxNum( HVSPRJ hVsprj, int *itemMaxNum );
VSLIB_IMPORT int __stdcall VslibAddItem( HVSPRJ hVsprj, char *fileName, int *itemNum );
VSLIB_IMPORT int __stdcall VslibAddItemEx( HVSPRJ hVsprj, char *fileName, int *itemNum,
									int nnOffset, int nnRange, int skipPitFlg );
VSLIB_IMPORT int __stdcall VslibCopyItem( HVSPRJ hVsprjDst, int *itemNumDst, HVSPRJ hVsprjSrc, int itemNumSrc );
VSLIB_IMPORT int __stdcall VslibDeleteItem( HVSPRJ hVsprj, int itemNum );
VSLIB_IMPORT int __stdcall VslibGetItemInfo( HVSPRJ hVsprj, int itemNum, VSITEMINFO *vsItemInfo );
VSLIB_IMPORT int __stdcall VslibSetItemInfo( HVSPRJ hVsprj, int itemNum, VSITEMINFO *vsItemInfo );
VSLIB_IMPORT int __stdcall VslibGetCtrlPntInfoEx( HVSPRJ hVsprj, int itemNum, int pntNum, VSCPINFOEX *vsCpInfo );
VSLIB_IMPORT int __stdcall VslibSetCtrlPntInfoEx( HVSPRJ hVsprj, int itemNum, int pntNum, VSCPINFOEX *vsCpInfo );

VSLIB_IMPORT int __stdcall VslibGetEQGain( HVSPRJ hVsprj, int itemNum, int eqNum, int *gain );
VSLIB_IMPORT int __stdcall VslibSetEQGain( HVSPRJ hVsprj, int itemNum, int eqNum, int *gain );

// �����ߥ�
VSLIB_IMPORT int __stdcall VslibGetTimeCtrlPntNum( HVSPRJ hVsprj, int itemNum, int *timeCtrlPntNum );
VSLIB_IMPORT int __stdcall VslibGetTimeCtrlPnt( HVSPRJ hVsprj, int itemNum, int pntNum, int *time1, int *time2 );
VSLIB_IMPORT int __stdcall VslibSetTimeCtrlPnt( HVSPRJ hVsprj, int itemNum, int pntNum, int time1, int time2 );
VSLIB_IMPORT int __stdcall VslibAddTimeCtrlPnt( HVSPRJ hVsprj, int itemNum, int time1, int time2 );
VSLIB_IMPORT int __stdcall VslibDeleteTimeCtrlPnt( HVSPRJ hVsprj, int itemNum, int pntNum );

VSLIB_IMPORT int __stdcall VslibGetStretchOrgSec( HVSPRJ hVsprj, int itemNum, double time_edt, double *time_org );
VSLIB_IMPORT int __stdcall VslibGetStretchEditSec( HVSPRJ hVsprj, int itemNum, double time_org, double *time_edt );
VSLIB_IMPORT int __stdcall VslibGetStretchOrgSample( HVSPRJ hVsprj, int itemNum, double time_edt, double *time_org );
VSLIB_IMPORT int __stdcall VslibGetStretchEditSample( HVSPRJ hVsprj, int itemNum, double time_org, double *time_edt );

// �ߥ����`
VSLIB_IMPORT int __stdcall VslibGetMixSample( HVSPRJ hVsprj, int *mixSample );
VSLIB_IMPORT int __stdcall VslibGetMixData( HVSPRJ hVsprj, void *waveData, int bit, int channel, int index, int size );
VSLIB_IMPORT int __stdcall VslibExportWaveFile( HVSPRJ hVsprj, char *fileName, int bit, int channel );

// �gλ��Q
VSLIB_IMPORT double __stdcall VslibCent2Freq( int cent );
VSLIB_IMPORT int __stdcall VslibFreq2Cent( double freq );
VSLIB_IMPORT double __stdcall VslibNoteNum2Freq( int noteNum );
VSLIB_IMPORT int __stdcall VslibFreq2NoteNum( double freq );

// ������
VSLIB_IMPORT int __stdcall VslibSetPitchArray( HVSPRJ hVsprj, int itemNum, int *pitData, int nPitData, double interval );

// ASAnalyzer
VSLIB_IMPORT int __stdcall AS_GetDllVer( void );
VSLIB_IMPORT double __stdcall AS_Cent2Freq( int cent, double freqa4 );
VSLIB_IMPORT int __stdcall AS_Freq2Cent( double freq, double freqa4 );
VSLIB_IMPORT double __stdcall AS_Nnum2Freq( int nnum, double freqa4 );
VSLIB_IMPORT int __stdcall AS_Freq2Nnum( double freq, double freqa4 );

VSLIB_IMPORT int __stdcall AS_AnalyzeWaveData( void *wavdata, unsigned short sndspc[], int *pitch, int wavdatasize,
	int wavsampleps, int wavbit, int wavchannel, int nnoffset, int nnrange, int blockpn, int targetch, double freqa4 );
VSLIB_IMPORT int __stdcall AS_AnalyzeWaveDataEX( void *wavdata, unsigned short *sndspc,
	int *pitch, int *dynamics, AWDINFO *awdi, double freqa4 );
VSLIB_IMPORT int __stdcall AS_AnalyzeWaveFile( char *wavfile, char *txtfile, int blockps, int nnoffset, int nnrange );


#endif /* _VSLIB_H_ */

