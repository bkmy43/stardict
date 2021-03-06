#include <stdio.h>
#include <stdlib.h>
#include <dos.h>
#include <io.h>

#include "rw.h"

int readHalf( BufRead& br, int* res )
//    �-�  ������ �� ��������� ������ � ���������� res ������ ��� ������
// ( � ����������� �� �������� ���. wh ����� ������) ��������  ����� �� �������� ������.
// ���������� ��������� ������ ( 1 � ������ �����  ��� EOF )
 {
   *res=0;

   if( br.ReadByte( res ) != 1 )
       return EOF;


   if( br.wh == 0 ) // ���� ����� ������ �������� �����
       {
	 *res = (*res) >> 4;
	 br.Rew(); // ��������� �� ���� �����
       }
   else
       *res =(*res)&0x0F;

   br.wh=++br.wh%2; // �������  � ��������� ��������

   return 1;
 }
//************************************************************************

int writeHalf( BufWrite& bw, int info )
//    �-�  ���������� � ������ �������� ���������� info � ������ ��� ������
// ( � ����������� �� �������� ���. wh ) ��������  ����� �� �������� ������.
// ���������� ��������� ������ ( 1 � ������ ����� )
 {
   int res, temp=0;

   if( bw.wh== 0 ) // ���� ����� �������� ������ �������� �����
       {
	 info = info << 4;
	 res=bw.WriteByte( info );
	 bw.Rew(); // ��������� �� ���� �����
	 bw.UsedBufSize--; // ���� ���� �� ��������� ��������������
       }
   else
       {
	 temp=bw.FileBuf[ bw.CurPos ]; // ��������� ������
	 temp=temp&0xF0;              // �������� ������ ��������
	 temp=temp|info;            // � �������� ���� ����. buffer
	 info=temp;
	 res=bw.WriteByte( info );
       }

   bw.wh=++bw.wh%2; // �������  � ��������� ��������
   return res;
 }
//*************************************************************************
int readByte( BufRead& brIn, int* ch ) // �-� ������ �� brIn ��� ���������
				// � ���������� �� � ch
 {
    int fHalf,sHalf;
    int res=1;

    if( (res=readHalf( brIn, &fHalf )) != 1 )
	 return res;
    if( (res=readHalf( brIn, &sHalf )) != 1 )
	 return res;

    *ch = (fHalf<<4) | sHalf;
    return res;
 }
//*******************************************************************
int writeByte( BufWrite& bwOut, int ch ) // �-� ���������� � bwOut ���
				     // ��������� �� ch
 {
    int res=1;

    if( (res=writeHalf( bwOut, ch>>4 )) != 1 )
	 return res;
    if( (res=writeHalf( bwOut, ch & 0x0F )) != 1 )
	 return res;
    return res;

 }
