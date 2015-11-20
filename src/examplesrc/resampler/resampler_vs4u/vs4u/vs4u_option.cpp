
//--------------------------------------------------------------
// VS4U�I�v�V�����ݒ�
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
// �I�v�V�����ݒ�
//
// ����
//  int		argc		�����̐�
//  char	*argv[]		����
//  VS4UOPTION *option	�I�v�V����
//
// �߂�l
//  0:���� 1:���s
//
// �T�v
//  �R�}���h���C��������ǂݏo���A�I�v�V������ݒ肷��B
//--------------------------------------------------------------
int Vs4uGetOption( int argc, char *argv[], VS4UOPTION *option )
{
#if 0
	printf( "argc = %d\n", argc);
	for ( int i=1; i<argc; i++ ){
		printf( "%d = %s\n", i, argv[i]);
	}
#endif

	// �I�v�V����������
	memset( option, 0, sizeof(VS4UOPTION));
	option->peekCmp = 86;
	option->breathiness = 50;

	// 1.���̓t�@�C��
	strcpy_s( option->inputWaveFile, VS4U_MAX_PATH, argv[1]);

	// 2.�o�̓t�@�C��
	strcpy_s( option->outputWaveFile, VS4U_MAX_PATH, argv[2]);

	// 3.���K��
	option->noteNum = Vs4uCnvNoteNum( argv[3]);

	// 4.����������̕ω���(�q�����x)
	option->fixSpeed = LimitDoubleValue( atof( argv[4] ), 0.0, 400.0);

	// 5.�t���O
	Vs4uSetFlgOption( argv[5], option);

	// 6.�I�t�Z�b�g
	option->offset = atof( argv[6]);

	// 7.�v����
	option->reqLength = atof( argv[7]);

	// 8.�q����
	option->fixedLength = atof( argv[8]);

	// 9.�u�����N
	option->endBlank = atof( argv[9]);

	// 10.�{�����[��
	option->volume = LimitDoubleValue( atof( argv[10] ), 0.0, 200.0);

	// 11.���W�����[�V����
	option->modulation = LimitDoubleValue( atof( argv[11] ), -400.0, 400.0);

	// 12�`13.�s�b�`�x���h
	if( argc >= 13 ){
		Vs4uSetPitchBendOption( argv, option);
	}

	// �Ǝ����g���\�t�@�C�����ݒ�
	strcpy_s( option->vsfrqFile, VS4U_MAX_PATH, option->inputWaveFile);
	AddExtStr( option->vsfrqFile, VS4U_MAX_PATH, ".vs4ufrq");

	// �W�����g���\�t�@�C�����ݒ�
	strcpy_s( option->frqFile, VS4U_MAX_PATH, option->inputWaveFile);
	AddExtStr( option->frqFile, VS4U_MAX_PATH, ".frq");

	// �v���W�F�N�g�t�@�C�����ݒ�
	strcpy_s( option->vspFile, VS4U_MAX_PATH, option->outputWaveFile);
	AddExtStr( option->vspFile, VS4U_MAX_PATH, ".vshp");
	
	return 0;
}


//--------------------------------------------------------------
// ���K����������m�[�g�i���o�[�ɕϊ�
//
// ����
//  char *nnStr		���K��������(C0�`B9)
//
// �߂�l
//  �m�[�g�i���o�[(C4=60�Ƃ��锼���P��)
//
// �T�v
//  ���K����������m�[�g�i���o�[�ɕϊ�����B
//--------------------------------------------------------------
int Vs4uCnvNoteNum( char *nnStr )
{
	int noteNum = 0;		// �m�[�g�i���o�[
	int octStrOffset = 1;	// �I�N�^�[�u�����̃C���f�b�N�X

	// ���K�������񒷂��`�F�b�N
	int length = strlen( nnStr);
	if( length < 2 ){
		return 60;
	}

	// ���K�����m�[�g�i���o�[�ɕϊ�
	switch( nnStr[0] ){
	case 'C':
		noteNum = 0;
		break;
	case 'D':
		noteNum = 2;
		break;
	case 'E':
		noteNum = 4;
		break;
	case 'F':
		noteNum = 5;
		break;
	case 'G':
		noteNum = 7;
		break;
	case 'A':
		noteNum = 9;
		break;
	case 'B':
		noteNum = 11;
		break;
	default:
		noteNum = 0;
		break;
	}

	// #������Δ����グ��
	if( nnStr[1] == '#' ){
		noteNum++;
		octStrOffset = 2;
	}

	// �I�N�^�[�u��ϊ�
	if( length > octStrOffset ){
		int oct = nnStr[octStrOffset]-'0';
		if( oct < 0 ){
			oct = 0;
		}
		else if( oct > 10 ){
			oct = 10;
		}
		noteNum += (oct+1)*12;
	}

	return noteNum;
}


//--------------------------------------------------------------
// �t���O������ŃI�v�V�����ݒ�
//
// ����
//  char *flgStr	�t���O������
//
// �߂�l
//  0:���� 1:���s
//
// �T�v
//  �t���O������ŃI�v�V������ݒ肷��B
//--------------------------------------------------------------
int Vs4uSetFlgOption( char *flgStr, VS4UOPTION *option )
{
	int length = strlen( flgStr);

	// �ݒ�
	int offset = 0;	// �ǂݏo���ʒu
	for(;;){
		// �I������
		if( offset == length ){
			break;
		}

		// �t���O��ʓǂݏo��
		char flg = flgStr[offset];
		offset++;

		// �t���O�l�̕����񒷂��Z�o
		int valueLength = 0;
		for(;;){
			// �I�[�ɓ��B
			if( offset+valueLength == length ){
				break;
			}
			// ���̃t���O(�A���t�@�x�b�g)�ɓ��B
			if( isalpha( flgStr[offset+valueLength] ) ){
				break;
			}
			valueLength++;
		}

		// �t���O�l�𐔒l�ɕϊ�
		int valueData = 0;
		if( ( valueLength > 0 ) && ( valueLength < 32 ) ){
			char tmpStr[32];
			memcpy_s( tmpStr, 32, flgStr+offset, valueLength);
			tmpStr[valueLength] = 0;
			valueData = atoi( tmpStr);
			offset += valueLength;
		}

		// �t���O�ݒ�
		switch( flg ){
		case 'N':
			option->noFormantFlg = 1;
			break;
		case 'g':
			option->genderFactor = LimitIntValue( valueData, -100, 100);
			break;
		case 't':
			option->pitchShift = LimitIntValue( valueData, -240, 240);
			break;
		case 'T':
			option->pitchShift2 = LimitIntValue( valueData, -2400, 2400);
			break;
		case 'P':
			option->peekCmp = LimitIntValue( valueData, 0, 100);
			break;
		case 'H':
			option->lowPassfilter = LimitIntValue( valueData, 0, 99);
			break;
		case 'B':
			option->breathiness = LimitIntValue( valueData, 0, 100);
			break;
		case 'O':
			option->brightness = LimitIntValue( valueData, -100, 100);
			break;
		case 'S':
			option->clearness = LimitIntValue( valueData, -100, 100);
			break;
		case 'Z':
			option->outputVshpFlg = valueData;
			break;
#if 0
		case 'V':
			option->reGenerateFrqFlg = 1;
			break;
#endif
		case 'W':
			option->disablePitchShift = valueData;
			break;
		}
	}

	return 0;
}


//--------------------------------------------------------------
// Base64�f�R�[�h
//
// ����
//  char *str	������
//
// �߂�l
//  �f�R�[�h�l
//
// �T�v
//  2����BASE64��������f�R�[�h���A-2048�`2047�̒l��Ԃ��B
//--------------------------------------------------------------
int Vs4uDecodeBase64( char *str )
{
	int value = 0;

	for( int i=0; i<2; i++ ){
		int c = str[i];
		int v = 0;

		if( ( c >= 'A' ) && ( c <= 'Z' ) ){
			v = c-'A';
		}
		else if( ( c >= 'a' ) && ( c <= 'z' ) ){
			v = c-'a'+26;
		}
		else if( ( c >= '0' ) && ( c <= '9' ) ){
			v = c-'0'+52;
		}
		else if( c == '+' ){
			v = 62;
		}
		else if( c == '/' ){
			v = 63;
		}

		value = value*64+v;
	}

	if( value >= 2048 ){
		value -= 4096;
	}

	return value;
}


//--------------------------------------------------------------
// �s�b�`�x���h�I�v�V�����ݒ�
//
// ����
//  int		argc		�����̐�
//  char	*argv[]		����
//  VS4UOPTION *option	�I�v�V����
//
// �߂�l
//  0:���� 1:���s
//
// �T�v
//  �R�}���h���C��������ǂݏo���A�s�b�`�x���h�I�v�V������ݒ肷��B
//--------------------------------------------------------------
int Vs4uSetPitchBendOption( char *argv[], VS4UOPTION *option )
{
	char *pitchBendStr = argv[12];
	option->tempo = 0.0;
	
	// �e���|�ǂݏo��
	if( pitchBendStr[0] == '!' ){
		option->tempo = atof( pitchBendStr+1);
		pitchBendStr = argv[13];
	}

	// �s�b�`�x���h�f�[�^�ǂݏo��
	int length = strlen( pitchBendStr);	// ������
	int nData = 0;		// �f�[�^��
	int offset = 0;		// �ǂݏo���ʒu
	int lastData = 0;	// �O��̃f�[�^(���������O�X�����p)

	for(;;){
		// �I������
		if( offset >= length ){
			break;
		}

		if( pitchBendStr[offset] == '#' ){
			// ���������O�X�����񒷎Z�o
			int i = offset+1;
			int offset2 = i;
			for(;;){
				if( i == length ){
					break;
				}
				if( pitchBendStr[i] == '#' ){
					offset2 = i;
					break;
				}
				i++;
			}
			int repNumLength = offset2-offset-1;

			// ���������O�X�Z�o,�f�[�^�ݒ�
			int repNum = 0;
			if( repNumLength < 8 ){
				char tmp[8];
				memcpy_s( tmp, 8, pitchBendStr+offset+1, repNumLength);
				tmp[repNumLength] = NULL;
				repNum = atoi( tmp);
				for( int i=0; i<repNum; i++ ){
					if( nData+i < VS4U_PBSIZE ){
						option->pitchBend[nData+i] = lastData;
					}
					else{
						break;
					}
				}
			}

			nData += repNum;
			offset = offset2+1;
		}
		else{
			// ���ڎw��
			if( length >= offset+2 ){
				lastData = Vs4uDecodeBase64( pitchBendStr+offset);
				if( nData < VS4U_PBSIZE ){
					option->pitchBend[nData] = lastData;
				}
				nData++;
				offset += 2;
			}
			else{
				break;
			}
		}
	}

	// �f�[�^���ݒ�
	if( nData > VS4U_PBSIZE ){
		nData = VS4U_PBSIZE;
	}
	option->nPitchBend = nData;

	return 0;
}


#ifdef PRINT_OPTION
//--------------------------------------------------------------
// �I�v�V�����\��(�f�o�b�O�p)
//
// ����
//  VS4UOPTION *option	�I�v�V����
//
// �߂�l
//  �Ȃ�
//
// �T�v
//  �I�v�V������\������B
//--------------------------------------------------------------
void Vs4uPrintOption( VS4UOPTION *option )
{
	printf( "----------------------------------------\n");

	printf( "���̓t�@�C��:%s\n", option->inputWaveFile);
	printf( "�o�̓t�@�C��:%s\n", option->outputWaveFile);
	printf( "�W�����g���\:%s\n", option->frqFile);
	printf( "�Ǝ����g���\:%s\n", option->vsfrqFile);

	printf( "�o�͉��K:%d\n", option->noteNum);
	printf( "�I�t�Z�b�g:%f\n", option->offset);
	printf( "�v����:%f\n", option->reqLength);
	printf( "�q����:%f\n", option->fixedLength);
	printf( "�u�����N:%f\n", option->endBlank);
	printf( "�q�����x:%f\n", option->fixSpeed);
	
	printf( "�{�����[��:%f\n", option->volume);
	printf( "���W�����[�V����:%f\n", option->modulation);

	printf( "�t�H���}���g�����Ȃ��t���O:%d\n", option->noFormantFlg);
	printf( "�W�F���_�[�t�@�N�^�[:%d\n", option->genderFactor);
	printf( "���g���\�č쐬�t���O:%d\n", option->reGenerateFrqFlg);
	printf( "�s�b�`������:%d\n", option->pitchShift);

	printf( "�s�b�`�x���h�f�[�^��:%d\n", option->nPitchBend);
	printf( "�e���|:%f\n", option->tempo);

#if 0
	printf( "�s�b�`�x���h�f�[�^:");
	for( int i=0; i<option->nPitchBend; i++ ){
		printf( "%d,", option->pitchBend[i]);
	}
	printf( "\n");
#endif

	printf( "----------------------------------------\n");
}
#endif // PRINT_OPTION
