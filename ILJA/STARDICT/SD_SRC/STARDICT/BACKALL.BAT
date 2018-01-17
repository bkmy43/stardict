@echo off
cls
d:
cd \projects\stardict\
echo.
echo Сохранение копии проекта в каталог d:\projects\stardict.bak\
echo.
xcopy *.* d:\projects\stardict.bak\ /E /I
echo.
echo ***********************************************************
echo.
echo Сохранение копии проекта в каталог e:\stardict.bak\
echo.
xcopy *.* e:\stardict.bak\ /E /I

goto endfinal

::copy /y *.bmp d:\projects\stardict.bak
::copy *.c   d:\projects\stardict.bak
::copy *.cpp d:\projects\stardict.bak
::copy *.h   d:\projects\stardict.bak
::copy *.hpp d:\projects\stardict.bak
::copy *.rc  d:\projects\stardict.bak
::copy *.rh  d:\projects\stardict.bak
::copy *.cur d:\projects\stardict.bak
::copy *.def d:\projects\stardict.bak
::copy *.ide d:\projects\stardict.bak
::copy *.dat d:\projects\stardict.bak
::copy *.mak d:\projects\stardict.bak
::
::copy *.txt d:\projects\stardict.bak
::copy *.ini d:\projects\stardict.bak
::copy *.ico d:\projects\stardict.bak
::copy *.dsw d:\projects\stardict.bak
::
::echo.
::echo Сохранение файлов Ильи из каталога LOWLEVEL\
::echo.
::xcopy lowlevel d:\projects\stardict.bak\lowlevel\ /s /y
::
::echo ***********************************************************
::echo.
::echo Сохранение копии проекта в каталог e:\stardict.bak\
::echo.
::copy *.bmp e:\stardict.bak
::copy *.c   e:\stardict.bak
::copy *.cpp e:\stardict.bak
::copy *.h   e:\stardict.bak
::copy *.hpp e:\stardict.bak
::copy *.rc  e:\stardict.bak
::copy *.rh  e:\stardict.bak
::copy *.cur e:\stardict.bak
::copy *.def e:\stardict.bak
::copy *.ide e:\stardict.bak
::copy *.dat e:\stardict.bak
::copy *.mak e:\stardict.bak
::
::copy *.txt e:\stardict.bak
::copy *.ini e:\stardict.bak
::copy *.ico e:\stardict.bak
::copy *.dsw e:\stardict.bak
::
::echo.
::echo Сохранение файлов Ильи из каталога LOWLEVEL\
::echo.
::xcopy lowlevel e:\stardict.bak\lowlevel\ /s /y

:endfinal

