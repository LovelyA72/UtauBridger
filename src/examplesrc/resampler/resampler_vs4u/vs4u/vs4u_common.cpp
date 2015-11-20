
//--------------------------------------------------------------
// VS4U���ʏ���
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
// �����l�͈͐���
//
// ����
//  int value	�����l
//  int min		�ŏ��l
//  int max		�ő�l
//
// �߂�l
//  �͈͐�����̎����l
//
// �T�v
//  �ŏ��l�ȉ��Ȃ�ŏ��l�ɁA�ő�l�ȏ�Ȃ�ő�l�ɕϊ�����B
//--------------------------------------------------------------
double LimitDoubleValue( double value, double min, double max )
{
	if( value < min ){
		value = min;
	}
	else if( value > max ){
		value = max;
	}
	return value;
}

//--------------------------------------------------------------
// �����l�͈͐���
// 
// ����
//  int value	�����Ώۂ̒l
//	int min		�ŏ��l
//	int max		�ő�l
// 
// �߂�l
//  �͈͐�����̐����l
// 
// �T�v
//  �ŏ��l�ȉ��Ȃ�ŏ��l�ɁA�ő�l�ȏ�Ȃ�ő�l�ɕϊ�����B
//--------------------------------------------------------------
int LimitIntValue( int value, int min, int max )
{
	if( value < min ){
		value = min;
	}
	else if( value > max ){
		value = max;
	}
	return value;
}


//--------------------------------------------------------------
// �g���q�t��
//
// ����
//  char *dstStr	�t�@�C����
//  int dstSize		�t�@�C�����o�b�t�@�T�C�Y
//  char *extStr	�g���q
//
// �߂�l
//  �Ȃ�
//
// �T�v
//  �t�@�C�����Ɋg���q��t������B
//--------------------------------------------------------------
void AddExtStr( char *dstStr, int dstSize, char *extStr )
{
	int len = strlen( dstStr);

	// ���̃t�@�C�����̊g���q�́u.�v���u_�v�ɕϊ�
	for( int i=len-1; i>=0; i-- ){
		if( dstStr[i] == '\\' ){
			break;
		}
		if( dstStr[i] == '.' ){
			dstStr[i] = '_';
			break;
		}
	}
	
	// �g���q�t��
	strcat_s( dstStr, dstSize, extStr);
}


//--------------------------------------------------------------
// �t�@�C�����݊m�F
//
// ����
//  char *fileName	�t�@�C����
//
// �߂�l
//  �t�@�C��������:1,�t�@�C�����Ȃ�:0
//
// �T�v
//  �w�肵���t�@�C�������݂����1���A�t�@�C�������݂��Ȃ����0��Ԃ��B
//--------------------------------------------------------------
int CheckFileExist( char *fileName )
{
	int existFlg = 0;
	FILE *fp;

	fopen_s( &fp, fileName, "rb");
	if ( fp != NULL ){
		fclose( fp);
		existFlg = 1;
	}

	return existFlg;
}

