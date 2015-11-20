
//--------------------------------------------------------------
// VS4U ���[�p�X�t�B���^�A�{�����[��
//--------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <math.h>
#include <windows.h>

#include "vs4u.h"


//--------------------------------------------------------------
// ���[�p�X�t�B���^�A�{�����[��
//
// ����
//  VS4UOPTION *option	�I�v�V����
//
// �߂�l
//  0:����, 1:���s
//
// �T�v
//  �t�B���^�����A�{�����[���ύX���s��Wave�t�@�C�����o�͂���B
//  (tmpWaveData��outputWaveFile)
//--------------------------------------------------------------
int Vs4uStep2( VS4UOPTION *option )
{
	// ���[�p�X�t�B���^
	Vs4uLowPassFilter( option);

	// �{�����[������
	Vs4uVolume( option);
	
	// Wave�t�@�C���o��
	int retVal = Vs4uOutputWaveFile( option);

	// �ꎞ�f�[�^���
	free( option->tmpWaveData);
	option->tmpWaveData = NULL;

	return retVal;
}


//--------------------------------------------------------------
// ���[�p�X�t�B���^����
//
// ����
//  VS4UOPTION *option	�I�v�V����
//
// �߂�l
//  0:����
//
// �T�v
//  tmpWaveData�Ƀ��[�p�X�t�B���^��������B
//  (tmpWaveData��tmpWaveData)
//--------------------------------------------------------------
int Vs4uLowPassFilter( VS4UOPTION *option )
{
	short *waveData = option->tmpWaveData;
	int nSample = option->tmpSample;
	int sampFreq = option->tmpSmpFreq;

	// ���g���v�Z
	// H�t���O�Ǝ��g���̑Ή��F
	// H : 100  50  20  10
	// f :  2k  4k 10k 20k
	double freq = 16000.0;
	if( option->lowPassfilter > 0 ){
		freq = 200000.0/option->lowPassfilter;
	}
	if( freq > 16000.0 ){
		freq = 16000.0;
	}
	if( freq > sampFreq*0.40 ){
		freq = sampFreq*0.40;
	}
	
	// �t�B���^�W���v�Z
	double q = 1.0/sqrt(2.0);
	double c1 = tan(M_PI*freq/sampFreq);
	double c2 = c1*c1;

	double b0 = c2/(1.0+c1/q+c2);
	double b1 = c2/(1.0+c1/q+c2)*2.0;
	double b2 = c2/(1.0+c1/q+c2);
	double a1 = 2.0*(c2-1.0)/(1.0+c1/q+c2);
	double a2 = (1.0-c1/q+c2)/(1.0+c1/q+c2);

	double x0 = 0.0, x1 = 0.0, x2 = 0.0;
	double y0 = 0.0, y1 = 0.0, y2 = 0.0;

	// �t�B���^��������
	for(int i=0; i<nSample; i++){
		x2 = x1;
		x1 = x0;
		y2 = y1;
		y1 = y0;
		x0 = waveData[i];
		y0 = b0*x0+b1*x1+b2*x2-a1*y1-a2*y2;

		if( y0 >= 0 ){
			int v = (int)(y0+0.5);
			if( v > 32767 ){
				v = 32767;
			}
			waveData[i] = (short)v;
		}
		else{
			int v = (int)(y0-0.5);
			if( v < -32768 ){
				v = -32768;
			}
			waveData[i] = (short)v;
		}
	}

	return 0;
}


//--------------------------------------------------------------
// �{�����[���ύX
//
// ����
//  VS4UOPTION *option	�I�v�V����
//
// �߂�l
//  0:����
//
// �T�v
//  tmpWaveData�̃{�����[����ύX����B
//  (tmpWaveData��tmpWaveData)
//--------------------------------------------------------------
int Vs4uVolume( VS4UOPTION *option )
{
	short *waveData = option->tmpWaveData;
	int nSample = option->tmpSample;

	// �ő剹�ʌv��
	int dynMax = 0;
	for(int i=0; i<nSample; i++){
		int dyn = abs( waveData[i]);
		if( dyn > dynMax ){
			dynMax = dyn;
		}
	}

	// �{�����[������
	if( dynMax != 0 ){
		double volume = option->volume/100.0;
		double peekCmp = option->peekCmp/100.0;
		double v1 = volume;							// PCMP�������̃{�����[��
		double v2 = volume*16384/dynMax;			// PCMP�L�����̃{�����[��
		double v3 = v2*peekCmp+v1*(1.0-peekCmp);	// �{�����[�����

		for(int i=0; i<nSample; i++){
			double w = waveData[i]*v3;
			if( w >= 0 ){
				int v = (int)(w+0.5);
				if( v > 32767 ){
					v = 32767;
				}
				waveData[i] = (short)v;
			}
			else{
				int v = (int)(w-0.5);
				if( v < -32768 ){
					v = -32768;
				}
				waveData[i] = (short)v;
			}
		}
	}

	return 0;
}


//--------------------------------------------------------------
// Wave�t�@�C���o��
//
// ����
//  VS4UOPTION *option	�I�v�V����
//
// �߂�l
//  0:����, 1:���s
//
// �T�v
//  tmpWaveData��Wave�t�@�C���ɏo�͂���B
//  (tmpWaveData��outputWaveFile)
//--------------------------------------------------------------
int Vs4uOutputWaveFile( VS4UOPTION *option )
{
	int retVal = 0;

	short *waveData = option->tmpWaveData;
	int nSample = option->tmpSample;
	int sampFreq = option->tmpSmpFreq;
	int bit = 16;
	int channel = 1;
	int datasize = nSample*(bit/8)*channel;

	FILE *fpOut;
	fopen_s( &fpOut, option->outputWaveFile, "wb");

	if( fpOut != NULL ){
		// �w�b�_�쐬
		char header[44];
		*(int *)(header+ 0) = 0x46464952;	// "RIFF"
		*(int *)(header+ 4) = 36+datasize;	// �t�@�C���T�C�Y-8
		*(int *)(header+ 8) = 0x45564157;	// "WAVE"
		*(int *)(header+12) = 0x20746d66;	// "fmt "
		*(int *)(header+16) = 16;			// �`�����N�T�C�Y
		*(short *)(header+20) = (short)WAVE_FORMAT_PCM;
		*(short *)(header+22) = (short)channel;
		*(int *)(header+24) = sampFreq;
		*(int *)(header+28) = sampFreq*channel*(bit/8);
		*(short *)(header+32) = (short)(channel*(bit/8));
		*(short *)(header+34) = (short)bit;
		*(int *)(header+36) = 0x61746164;	// "data"
		*(int *)(header+40) = datasize;		// datasize

		// �w�b�_��������
		fseek( fpOut, 0, SEEK_SET);
		fwrite( header, sizeof(char), 44, fpOut);

		// WAVE�f�[�^��������
		fseek( fpOut, 44, SEEK_SET);
		fwrite( waveData, sizeof(short), nSample, fpOut);

		// �o�̓t�@�C�������
		fclose( fpOut);
	}
	else{
		printf( "VS4U:�t�@�C���o�̓G���[.\n");
		retVal = 1;
	}

	return retVal;
}

