/********************************************************************
-- STARDICT Project --
������ ������� ����⠭��
(c) I.Vladimirsky 1997
********************************************************************/

#if !defined _DEFINES_H
#define _DEFINES_H

typedef unsigned char boolean;
// ����⠭�� ��饣� ���������
#define YES      1
#define NO       0
#define OK       0
#define NOK     -1
#define ON               1
#define OFF              0
/*
// ������ ( ᪠� ���� )
#define SC_ENTER         0x1C0D
#define SC_ESC           0x11B
#define SC_DOWN          0x5000
#define SC_UP            0x4800
#define SC_LEFT          19200
#define SC_RIGHT         19712
#define SC_PGUP          18688
#define SC_PGDN          20736
#define SC_HOME          18176
#define SC_END           20224
#define SC_F10           17408
  */

// ��� ����஢�� ⥪�⮢
#define OFTEN_CODE       0xE
#define SELDOM_CODE      0xF

#define MAX_BODY_SIZE 50000

// �訡��
#define WRONG_INDEXDAT  -2
#define WRONG_DATADAT   -3
#define WRONG_TH        -4
#define NO_MEMORY       -5
#define NOT_ENOUPH_BUF  -6

// ���-�� ������������ � "���ਨ" ��⥩
#define _HISTORY
#define MAX_HISTORY     20

#endif
