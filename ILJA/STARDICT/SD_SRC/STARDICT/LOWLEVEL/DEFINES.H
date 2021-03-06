/********************************************************************
-- STARDICT Project --
								 defines.h

����������� �������� ��������
(c) I.Vladimirsky 1997
********************************************************************/

#if !defined _DEFINES_H
#define _DEFINES_H

// !!!!!
#define FORMAT_VERSION 20
#define DATA_VERSION 100

typedef unsigned char boolean;
typedef    int  filehand ;

// ��������� ������ ����������
#define YES      1
#define NO       0
#define OK       0
#define NOK     -1
#define ON               1
#define OFF              0

// ��� ���������
#define OFTEN_CODE       0xE
#define SELDOM_CODE      0xF

#define MAX_BODY_SIZE     30000
#define MAX_NAME_LEN      255
//#define MAX_THEMES        200
//#define MAX_ARTS_IN_THEME 200

// !!!!!
//#define MAX_KEYWORDS        200
//#define MAX_ARTS_IN_KEYWORD 200

// for TArtList constructor
#define  BUILD_LIST  1
#define  NOT_BUILD_LIST  0

// �������� � ������ �� ������ �����
#define CRC_POSITION 3

#endif
