/********************************************************************
-- STARDICT Project --

�-�� �����⢫��騥 ������/�⥭��  � 䠩�� ����ﬨ �� ���-����.
�᭮���� �� �ᯮ�짮����� ���ਧ�樨 BufRead � BufWrite.
�ᯮ������� �� ����஢���� ⥪�⮢ � data.dat

(c) I.Vladimirsky 1997
********************************************************************/

#include "rw.h"

int readHalf( BufRead& br, int* res )
//    �-�  �⠥� �� 䠩������ ���� � ��६����� res ����� ��� �����
// ( � ����ᨬ��� �� ���祭�� ���. wh �⮣� ����) ��������  ���� �� ⥪�饬� �����.
// �����頥� १���� �⥭�� ( 1 � ��砥 㤠�  ��� EOF )
 {
   *res=0;

   if( br.ReadByte( res ) != 1 )
       return EOF;


   if( br.wh == 0 ) // �᫨ �㦭� ��ࢠ� �������� ����
       {
         *res = (*res) >> 4;
         br.Rew(); // �������� �� ���� �����
       }
   else
       *res =(*res)&0x0F;

   br.wh=++br.wh%2; // ��३�  � ᫥���饩 ��������

   return 1;
 }
//************************************************************************

int writeHalf( BufWrite& bw, int info )
//    �-�  �����뢠�� � ����� ���祭�� ��६����� info � ����� ��� �����
// ( � ����ᨬ��� �� ���祭�� ���. wh ) ��������  ���� �� ⥪�饬� �����.
// �����頥� १���� ����� ( 1 � ��砥 㤠� )
 {
   int res, temp=0;

   if( bw.wh== 0 ) // �᫨ �㦭� ������� ����� �������� ����
       {
         info = info << 4;
         res=bw.WriteByte( info );
         bw.Rew(); // �������� �� ���� �����
         bw.UsedBufSize--; // ��� ���� �� ��⠥��� �ᯮ�짮�����
       }
   else
       {
         temp=bw.FileBuf[ bw.CurPos ]; // ��᫥���� ������
         temp=temp&0xF0;              // ���㫨�� ����� ��������
         temp=temp|info;            // � ������� �㤠 ����. buffer
         info=temp;
         res=bw.WriteByte( info );
       }

   bw.wh=++bw.wh%2; // ��३�  � ᫥���饩 ��������
   return res;
 }
//*************************************************************************
int readByte( BufRead& brIn, int* ch ) // �-� �⠥� �� brIn ��� ���㡠��
                                // � ��ꥤ���� �� � ch
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
int writeByte( BufWrite& bwOut, int ch ) // �-� �����뢠�� � bwOut ���
                                     // ���㡠�� �� ch
 {
    int res=1;

    if( (res=writeHalf( bwOut, ch>>4 )) != 1 )
         return res;
    if( (res=writeHalf( bwOut, ch & 0x0F )) != 1 )
         return res;
    return res;
 }
