@echo off
set dr=d:\upload
d:
cd \projects\stardict

echo.
echo �������� ��⠫��� �� ��᪥ E:
echo.
if not exist %dr% md %dr%
if not exist %dr%\_common md %dr%\_common

echo.
echo ����஢���� 䠩���.
echo.
xcopy _common\*.* %dr%\_common\
copy *.c   %dr% /y
copy *.h   %dr% /y
copy *.rh  %dr% /y
copy *.rc  %dr% /y
copy *.def %dr% /y
copy stardict.exe %dr% /y
copy stardict.ide %dr% /y
copy *.dll %dr% /y
copy *.bmp %dr% /y
copy *.cur %dr% /y
copy *.ico %dr% /y
copy *.bat %dr% /y
copy *.ini %dr% /y

echo.
echo �������� �⫠��筮� ���ଠ樨.
echo.
d:
cd %dr%
call tdstrip.exe stardict.exe
call tdstrip.exe stdrus.dll
call tdstrip.exe stdeng.dll

echo.
echo �������� � 䠩��.
echo.
del std.rar
del std.r0?
call rar m -r -y -std -v80k -m5 -av -$d std.rar *.*
