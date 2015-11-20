
//--------------------------------------------------------------
// VS4U �s�b�`�A�t�H���}���g�A�^�C���X�g���b�`
//--------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <math.h>
#include <windows.h>

#include "vslib.h"
#include "vs4u.h"


//--------------------------------------------------------------
// �s�b�`�V�t�g�A�t�H���}���g�V�t�g�A�^�C���X�g���b�`
//
// ����
//  VS4UOPTION *option	�I�v�V����
//
// �߂�l
//  0:���� 1:���s
//
// �T�v
//  �s�b�`�V�t�g�A�t�H���}���g�V�t�g�A�^�C���X�g���b�`�������s���B
//  (inputWaveFile��tmpWaveData)
//--------------------------------------------------------------
int Vs4uStep1( VS4UOPTION *option )
{
	VSPRJINFO vsPrjInfo;
	VSITEMINFO vsItemInfo;

	// �v���W�F�N�g�쐬
	HVSPRJ hVsprj = Vs4uLoadProject( option);
	if( hVsprj == NULL )
	{
		return 1;
	}

	// �^�C���X�g���b�`�ݒ�
	Vs4uSetTimeStretchPrm( hVsprj, option);

	// �s�b�`�ݒ�
	Vs4uSetPitPrm( hVsprj, option);

	// �������������ݒ�
	if( option->disablePitchShift == -2 )
	{
		// �a�����[�h�ɐݒ�
		VslibGetItemInfo( hVsprj, 0, &vsItemInfo);
		vsItemInfo.synthMode = SYNTHMODE_P;
		VslibSetItemInfo( hVsprj, 0, &vsItemInfo);
	}
	else
	{
		if( option->noFormantFlg != 0 )
		{
			// �t�H���}���g�����Ȃ��Ȃ�P�����[�h�ɐݒ�
			VslibGetItemInfo( hVsprj, 0, &vsItemInfo);
			vsItemInfo.synthMode = SYNTHMODE_M;
			VslibSetItemInfo( hVsprj, 0, &vsItemInfo);
		}
		else
		{
			// �t�H���}���g�␳����Ȃ�P���t�H���}���g�␳���[�h�ɐݒ�
			VslibGetItemInfo( hVsprj, 0, &vsItemInfo);
			vsItemInfo.synthMode = SYNTHMODE_MF;
			VslibSetItemInfo( hVsprj, 0, &vsItemInfo);
		}
	}

	// EQ�ݒ�
	int eq1[15] = { 0, 0, 0, 0, 0, 0, 0, 20, 60, 100, 120, 120, 80,   0,   0 };
	int eq2[15] = { 0, 0, 0, 0, 0, 0, 0,  0,  0,   0,   0,   0, 40, 120, 120 };
	VslibSetEQGain( hVsprj, 0, 0, eq1);
	VslibSetEQGain( hVsprj, 0, 1, eq2);

	// ����_���擾
	VslibGetItemInfo( hVsprj, 0, &vsItemInfo);
	int nCtrlPnt = vsItemInfo.ctrlPntNum;
	
	// ����_�p�����[�^�ݒ�
	for( int i=0; i<nCtrlPnt; i++ ){
		// ����_���擾
		VSCPINFOEX vsCpInfo;
		VslibGetCtrlPntInfoEx( hVsprj, 0, i, &vsCpInfo);
		
		// �W�F���_�[�t�@�N�^�[�ݒ�
		vsCpInfo.formant = -20*option->genderFactor;

		// �u���V�l�X�ݒ�
		vsCpInfo.breathiness = (option->breathiness-50)*200;

		// �u���C�g�l�X�ݒ�
		vsCpInfo.eq1 = option->brightness*100;

		// �N���A�l�X�ݒ�
		vsCpInfo.eq2 = option->clearness*100;

		// ����_���ݒ�
		VslibSetCtrlPntInfoEx( hVsprj, 0, i, &vsCpInfo);
	}

	// �T���v�����O���g���A�T���v�����擾
	VslibGetProjectInfo( hVsprj, &vsPrjInfo);
	option->tmpSmpFreq = vsPrjInfo.sampFreq;
	VslibGetMixSample( hVsprj, &option->tmpSample);

	// WAVE�f�[�^�擾
	option->tmpWaveData = (short *)calloc( option->tmpSample, sizeof(short));
	VslibGetMixData( hVsprj, option->tmpWaveData, 16, 1, 0, option->tmpSample);

	// �v���W�F�N�g�t�@�C���o��
	if( option->outputVshpFlg == 12345 ){
		VslibSaveProject( hVsprj, option->vspFile);
	}

	// �v���W�F�N�g�폜
	VslibDeleteProject( hVsprj);

	return 0;
}


//--------------------------------------------------------------
// �v���W�F�N�g�쐬
//
// ����
//  VS4UOPTION *option	�I�v�V����
//
// �߂�l
//  �v���W�F�N�g�n���h��
//
// �T�v
//  �Ǝ����g���\��ǂݍ��݁A�v���W�F�N�g�n���h����Ԃ��B
//  �Ǝ����g���\���Ȃ��ꍇ�͍쐬����B
//  ���s�����ꍇ��NULL��Ԃ��B
//--------------------------------------------------------------
HVSPRJ Vs4uLoadProject( VS4UOPTION *option )
{
	int sta;

	// WAVE�t�@�C�����݃`�F�b�N
	if( CheckFileExist( option->inputWaveFile ) == 0 ){
		printf( "VS4U:���̓t�@�C����������܂���.\n");
		return NULL;
	}

	// �W�����g���\���݃`�F�b�N
	int existFrqFlg = CheckFileExist( option->frqFile);

	// �Ǝ����g���\���݃`�F�b�N
	int existVsfrqFlg = CheckFileExist( option->vsfrqFile);

	// �v���W�F�N�g�쐬 ----------------------------------------
	HVSPRJ hVsprj = NULL;

	if( option->reGenerateFrqFlg != 0 ){
		existVsfrqFlg = 0;
	}
	else{
		if( existVsfrqFlg != 0 ){
			// �Ǝ����g���\��ǂݍ���
			sta = VslibOpenProject( &hVsprj, option->vsfrqFile);
			if( sta != VSERR_NOERR ){
				printf( "VS4U:���g���\�ǂݍ��݃G���[.\n");
				existVsfrqFlg = 0;
			}
		}
	}

	if( existVsfrqFlg == 0 ){
		// �Ǝ����g���\(vsp)���쐬����
		printf( "VS4U:���g���\���쐬���܂�.\n");

		// �v���W�F�N�g�쐬
		sta = VslibCreateProject( &hVsprj);
		if( sta != VSERR_NOERR ){
			printf( "VS4U:�������G���[.\n");
			return NULL;
		}
		
		// �A�C�e���ǉ�
		int itemNum;
		sta = VslibAddItemEx( hVsprj, option->inputWaveFile, &itemNum,  36, 48, 0);
		if( sta != VSERR_NOERR ){
			printf( "VS4U:�t�@�C�����̓G���[.\n");
			return NULL;
		}

		// �W�����g���\�𗘗p���Ď��g���ݒ�
		if( existFrqFlg != 0 ){
			Vs4uLoadFrqFile( hVsprj, option);
		}

		// �Ǝ����g���\�ۑ�
		sta = VslibSaveProject( hVsprj, option->vsfrqFile);
		if( sta != VSERR_NOERR ){
			printf( "VS4U:���g���\�̕ۑ��Ɏ��s���܂���.\n");
		}
		else{
			printf( "VS4U:���g���\���쐬���܂���.\n");
		}
	}

	return hVsprj;
}


//--------------------------------------------------------------
// �W�����g���\�𗘗p���Ď��g���ݒ�
//
// ����
//  HVSPRJ hVsprj	�v���W�F�N�g
//  VS4UOPTION *option	�I�v�V����
//
// �߂�l
//  0:���� 1:���s
//
// �T�v
//  �W�����g���\�𗘗p���ăs�b�`��͒l��ݒ肷��B
//--------------------------------------------------------------
int Vs4uLoadFrqFile( HVSPRJ hVsprj, VS4UOPTION *option )
{
	VSPRJINFO vsPrjInfo;
	VSITEMINFO vsItemInfo;
	int retVal = 0;

	FILE *fpFrq;
	fopen_s( &fpFrq, option->frqFile, "rb");

	if ( fpFrq != NULL ){
		// �f�[�^�Ԋu[�T���v��]
		int itvSmp = 256;
		fseek( fpFrq, 8, SEEK_SET);
		fread( &itvSmp, sizeof(int), 1, fpFrq);
		
		// �L�[���g��
		double keyFreq = 440.0;
		fseek( fpFrq, 12, SEEK_SET);
		fread( &keyFreq, sizeof(double), 1, fpFrq);

		// �f�[�^��
		int nData = 0;
		fseek( fpFrq, 36, SEEK_SET);
		fread( &nData, sizeof(int), 1, fpFrq);

		// ���g���f�[�^
		FREQDATA *freqData = (FREQDATA *)calloc( nData, sizeof(FREQDATA));
		fseek( fpFrq, 40, SEEK_SET);
		fread( freqData, sizeof(FREQDATA), nData, fpFrq);

#if 0
		// �\��(�f�o�b�O�p)
		printf( "itvSmp=%d\n", itvSmp);
		printf( "keyFreq=%f\n", keyFreq);
		printf( "nData=%d\n", nData);
		for( int i=0; i<nData; i++ ){
			printf( "%f,%f; ", freqData[i].freq, freqData[i].dyn);
		}
		getchar();
#endif

		// ����_���擾
		VslibGetItemInfo( hVsprj, 0, &vsItemInfo);
		int nCtrlPnt = vsItemInfo.ctrlPntNum;

		// �T���v�����O���g���擾
		VslibGetProjectInfo( hVsprj, &vsPrjInfo);
		int sampFreq = vsPrjInfo.sampFreq;
		double cpSec = 1.0/vsItemInfo.ctrlPntPs;	// ����_������̕b��

		// ���g���ݒ�
		for( int i=0; i<nCtrlPnt; i++ ){
			// �W�����g���\��ł̃|�C���g�Z�o
			double smp = i*cpSec*sampFreq;
			int pnt = (int)(smp/itvSmp);

			// ���g���ݒ�
			double freq = 0.0;
			if( ( pnt >= 0 ) && ( pnt < nData ) ){
				freq = freqData[pnt].freq;
			}

			if( freq > 55.0 ){	// �s�b�`����
				// �ҏW�O�s�b�`�ƃs�b�`�L���t���O��ݒ�
				VSCPINFOEX vsCpInfo;
				VslibGetCtrlPntInfoEx( hVsprj, 0, i, &vsCpInfo);
				vsCpInfo.pitOrg = (int)(log(freq/440.0)/log(2.0)*1200.0)+6900;
				vsCpInfo.pitFlgEdit = 1;
				VslibSetCtrlPntInfoEx( hVsprj, 0, i, &vsCpInfo);
			}
			else{				// �s�b�`�Ȃ�
				// �ҏW�O�s�b�`�ƃs�b�`�L���t���O��ݒ�
				VSCPINFOEX vsCpInfo;
				VslibGetCtrlPntInfoEx( hVsprj, 0, i, &vsCpInfo);
				vsCpInfo.pitOrg = (int)(log(keyFreq/440.0)/log(2.0)*1200.0)+6900;
				vsCpInfo.pitFlgEdit = 0;
				VslibSetCtrlPntInfoEx( hVsprj, 0, i, &vsCpInfo);
			}
		}
		
		// ���g���f�[�^���
		free( freqData);
		freqData = NULL;

		// �t�@�C�������
		fclose( fpFrq);
	}
	else{
		retVal = 1;
	}

	return retVal;
}


//--------------------------------------------------------------
// �^�C���X�g���b�`�ݒ�
//
// ����
//  HVSPRJ hVsprj	�v���W�F�N�g
//  VS4UOPTION *option	�I�v�V����
//
// �߂�l
//  0:����
//
// �T�v
//  �^�C���X�g���b�`�p�����[�^��ݒ肷��B
//--------------------------------------------------------------
int Vs4uSetTimeStretchPrm( HVSPRJ hVsprj, VS4UOPTION *option )
{
	VSPRJINFO vsPrjInfo;
	VSITEMINFO vsItemInfo;

	// �T���v�����O���g���擾
	VslibGetProjectInfo( hVsprj, &vsPrjInfo);
	int sampFreq = vsPrjInfo.sampFreq;

	// �A�C�e�����擾
	VslibGetItemInfo( hVsprj, 0, &vsItemInfo);
	double orgSmp = vsItemInfo.sampleOrg;					// ���̃T���v����
	double offsetSmp = option->offset*0.001*sampFreq;		// �I�t�Z�b�g�̃T���v����
	double fixedSmp = option->fixedLength*0.001*sampFreq;	// �q�����̃T���v����
	double endBlankSmp = option->endBlank*0.001*sampFreq;	// �u�����N�̃T���v����
	double reqSmp = option->reqLength*0.001*sampFreq;		// �v�����̃T���v����

	// �u�����N����(�I�t�Z�b�g����̑��Βl)�Ȃ�I�[����̃u�����N�ɕϊ�����
	if( endBlankSmp < 0 ){
		endBlankSmp = orgSmp-offsetSmp+endBlankSmp;
	}

	// �q�����̃X�g���b�`�{��
	double fixStretch = pow( 2.0, 1.0-option->fixSpeed/100.0);

	// �^�C�~���O����_�ǉ�
	if( reqSmp >= fixedSmp*fixStretch ){
		// �v�������q�����ȏ�
		VslibSetTimeCtrlPnt( hVsprj, 0, 1, (int)orgSmp, (int)reqSmp);
		VslibAddTimeCtrlPnt( hVsprj, 0, (int)(orgSmp-endBlankSmp), (int)reqSmp);
		VslibAddTimeCtrlPnt( hVsprj, 0, (int)(offsetSmp+fixedSmp), (int)(fixedSmp*fixStretch));
		VslibAddTimeCtrlPnt( hVsprj, 0, (int)offsetSmp, 0);
	}
	else{
		// �v�������q��������
		VslibSetTimeCtrlPnt( hVsprj, 0, 1, (int)orgSmp, (int)reqSmp);
		VslibAddTimeCtrlPnt( hVsprj, 0, (int)(offsetSmp+reqSmp/fixStretch), (int)reqSmp);
		VslibAddTimeCtrlPnt( hVsprj, 0, (int)offsetSmp, 0);
	}

	return 0;
}


//--------------------------------------------------------------
// �ꉹ�����̃s�b�`���ϒl�v�Z
//
// ����
//  HVSPRJ hVsprj	�v���W�F�N�g
//  VS4UOPTION *option	�I�v�V����
//
// �߂�l
//  0:����
//
// �T�v
//  �ꉹ�����̃s�b�`���ϒl�v�Z���v�Z����B
//--------------------------------------------------------------
int Vs4uCalcAveragePit( HVSPRJ hVsprj, VS4UOPTION *option )
{
	VSITEMINFO vsItemInfo;

	// ����_���擾
	VslibGetItemInfo( hVsprj, 0, &vsItemInfo);
	int nCtrlPnt = vsItemInfo.ctrlPntNum;

	// ����_������̕b�����Z�o
	double ctrpntSec = 1.0/vsItemInfo.ctrlPntPs;

	// �ꉹ��Ԃ̐���_�ԍ����Z�o
	int startPnt = (int)((option->offset+option->fixedLength)*0.001/ctrpntSec);
	int endPnt = (int)(nCtrlPnt-option->endBlank*0.001/ctrpntSec);
	startPnt = LimitIntValue( startPnt, 0, nCtrlPnt);
	endPnt = LimitIntValue( endPnt, 0, nCtrlPnt);

	// �ꉹ�̃s�b�`���ϒl�Z�o
	int pitSum = 0;
	int count = 0;
	for( int i=startPnt; i<endPnt; i++ ){
		VSCPINFOEX vsCpInfo;
		VslibGetCtrlPntInfoEx( hVsprj, 0, i, &vsCpInfo);
		if( vsCpInfo.pitFlgEdit == 1 ){	// �s�b�`�����镔���̂݃J�E���g
			pitSum += vsCpInfo.pitOrg;
			count++;
		}
	}
	int pitAverage = 6000;
	if( count != 0 ){
		pitAverage = pitSum/count;
	}

	// �ꉹ�̃s�b�`���ϒl�Z�o(2���)
	pitSum = 0;
	count = 0;
	for( int i=startPnt; i<endPnt; i++ ){
		VSCPINFOEX vsCpInfo;
		VslibGetCtrlPntInfoEx( hVsprj, 0, i, &vsCpInfo);
		if( vsCpInfo.pitFlgEdit == 1 ){	// �s�b�`�����镔���̂݃J�E���g
			if( abs(vsCpInfo.pitOrg-pitAverage) < 200 ){
				pitSum += vsCpInfo.pitOrg;
				count++;
			}
		}
	}
	pitAverage = 6000;
	if( count != 0 ){
		pitAverage = pitSum/count;
	}

	return pitAverage;
}


//--------------------------------------------------------------
// �s�b�`�ݒ�
//
// ����
//  HVSPRJ hVsprj	�v���W�F�N�g
//  VS4UOPTION *option	�I�v�V����
//
// �߂�l
//  0:����
//
// �T�v
//  �s�b�`�p�����[�^��ݒ肷��B
//--------------------------------------------------------------
int Vs4uSetPitPrm( HVSPRJ hVsprj, VS4UOPTION *option )
{
	VSPRJINFO vsPrjInfo;
	VSITEMINFO vsItemInfo;

	// �T���v�����O���g���擾
	VslibGetProjectInfo( hVsprj, &vsPrjInfo);
	int sampFreq = vsPrjInfo.sampFreq;

	// ����_���擾
	VslibGetItemInfo( hVsprj, 0, &vsItemInfo);
	int nCtrlPnt = vsItemInfo.ctrlPntNum;

	// ����_������̕b�����v�Z
	double ctrpntSec = 1.0/vsItemInfo.ctrlPntPs;

	// �s�b�`���ϒl���v�Z
	int pitAverage = Vs4uCalcAveragePit( hVsprj, option);

	// �S�̂ɂ�����s�b�`�V�t�g�ʂ��v�Z
	int globalShift = 10*option->pitchShift+option->pitchShift2;
	
	// �s�b�`�x���h�l�̊Ԋu[�b�P��]���v�Z
	double pbitvSec = 256.0/sampFreq;
	if( option->tempo > 0.0 ){
		pbitvSec = 60.0/option->tempo/96;	// 4��������1/96�̕b��
	}

	if( ( option->disablePitchShift == -1 ) || ( option->disablePitchShift == -2 ) )
	{
		for( int i=0; i<nCtrlPnt; i++ ){
			// �s�b�`�ݒ�
			VSCPINFOEX vsCpInfo;
			VslibGetCtrlPntInfoEx( hVsprj, 0, i, &vsCpInfo);
			vsCpInfo.pitEdit = vsCpInfo.pitOrg;
			VslibSetCtrlPntInfoEx( hVsprj, 0, i, &vsCpInfo);
		}
	}
	else
	{
		// �ҏW��s�b�`�p�����[�^�ݒ� ------------------------------
		for( int i=0; i<nCtrlPnt; i++ ){
			// �ҏW��̈ʒu(�b�P��)�v�Z
			double edtSec;
			VslibGetStretchEditSec( hVsprj, 0, i*ctrpntSec, &edtSec);
		
			// �s�b�`�x���h�l�v�Z
			int bendPnt = (int)(edtSec/pbitvSec);
			int bend = 0;
			if( ( bendPnt >= 0 ) && ( bendPnt < option->nPitchBend ) ){
				bend = option->pitchBend[bendPnt];
			}

			// �s�b�`�ݒ�
			VSCPINFOEX vsCpInfo;
			VslibGetCtrlPntInfoEx( hVsprj, 0, i, &vsCpInfo);
			int modPit = (int)((vsCpInfo.pitOrg-pitAverage)*option->modulation/100.0);
			vsCpInfo.pitEdit = option->noteNum*100+modPit+bend+globalShift;
			VslibSetCtrlPntInfoEx( hVsprj, 0, i, &vsCpInfo);
		}

	
		// �ҏW��s�b�`�p�����[�^(VS4U��p)�ݒ� --------------------
		// �o�̓T���v�����擾
		int mixSample;
		VslibGetMixSample( hVsprj, &mixSample);

		// �s�b�`�x���h�f�[�^�����Z�o
		double mixSec = (double)mixSample/sampFreq;
		int nPitchBend = (int)(mixSec/pbitvSec)+1;

		// �s�b�`�x���h�f�[�^���s�b�`�ύX��[cent]�ɕϊ�
		for( int i=0; i<nPitchBend; i++ ){
			// �s�b�`�x���h�f�[�^�̈ʒu�ɑΉ����鐧��_�ԍ����v�Z
			double orgSec;
			double edtSec = LimitDoubleValue( i*pbitvSec, 0.0, mixSec);
			VslibGetStretchOrgSec( hVsprj, 0, edtSec, &orgSec);
			int pnt = LimitIntValue( (int)(orgSec/ctrpntSec), 0, nCtrlPnt-1);

			// �ҏW�O�s�b�`���擾
			VSCPINFOEX vsCpInfo;
			VslibGetCtrlPntInfoEx( hVsprj, 0, pnt, &vsCpInfo);
			int pitOrg = vsCpInfo.pitOrg;

			// �ҏW��s�b�`���v�Z
			int modPit = (int)((pitOrg-pitAverage)*option->modulation/100.0);
			int pitEdt = option->noteNum*100+modPit+globalShift+option->pitchBend[i];

			// �s�b�`�ύX�ʂ��Z�b�g
			option->pitchBend[i] = pitEdt-pitOrg;
		}

		// �s�b�`�ύX�ʃf�[�^��ݒ�
		VslibSetPitchArray( hVsprj, 0, option->pitchBend, nPitchBend, pbitvSec*sampFreq);
	}

	return 0;
}

