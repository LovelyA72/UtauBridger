
//--------------------------------------------------------------
// VS4U���C��
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
// ���C��
//--------------------------------------------------------------
int mainproc( int argc, char *argv[] )
{
	// �T�v�\��
	if( argc <= 1 ){
		printf( "----------------------------------------\n");
		printf( "VS4U ver.1.32\n");
		printf( "Copyright (C) 2013-2014 �������[\n");
		printf( "http://ackiesound.ifdef.jp/\n");
		printf( "----------------------------------------\n");
		return 0;
	}

#ifdef PRINT_TIME
	// �J�n���Ԏ擾
	int time_start = GetTickCount();
#endif

	// �I�v�V�����ݒ�
	VS4UOPTION option;
	if( Vs4uGetOption( argc, argv, &option ) != 0 ){
		return 0;
	}

	// �I�v�V�����\��
#ifdef PRINT_OPTION
	Vs4uPrintOption( &option);
#endif

	// �s�b�`�A�t�H���}���g�A�^�C���X�g���b�`
	if( Vs4uStep1( &option ) != 0 ){
		return 0;
	}

	// �t�B���^�A�{�����[��
	if( Vs4uStep2( &option ) != 0 ){
		return 0;
	}

	return 0;
}

