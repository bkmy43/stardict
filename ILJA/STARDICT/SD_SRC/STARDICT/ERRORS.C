/* *************************************************************************
                         ERRORS.C
    �������� ���� ������ �� �������������� �������.
    (c) I.Vladimirsky
************************************************************************* */

case EC_WRONG_INDEXDAT:   e = "Error in index.dat file.";
                          r = "������ � ����� index.dat.";
                          break;
case EC_WRONG_DATADAT:    e = "Error in data.dat file.";
                          r = "������ � ����� data.dat.";
                          break;

case EC_WRONG_THEMESDAT:  e = "Error in themes.dat file.";
                          r = "������ � ����� themes.dat.";
                          break;

case EC_WRONG_KEYWORDDAT: e = "Error in keyword.dat file.";
                          r = "������ � ����� keyword.dat.";
                          break;

case EC_WRONG_CRC:        e = "Wrong CRC.";
                          r = "�������� ����������� �����.";
                          break;

case EC_CANTOPEN_INDEXDAT: e = "An error occured while opening index.dat file.\n"
                               "Please make sure, that this file is placed\n"
                               "in Stardict work directory.";

                           r = "��������� ������ ��� �������� ����� index.dat.\n"
                               "���������� ��������� � ���, ��� ��������� ����\n"
                               "��������� � ������� �������� Stardict.";
                           break;

case EC_CANTOPEN_DATADAT:  e = "An error occured while opening data.dat file.\n"
                               "Please make sure, that this file is placed\n" 
                               "in Stardict work directory.";

                           r = "��������� ������ ��� �������� ����� data.dat.\n"
                               "���������� ��������� � ���, ��� ��������� ����\n"
                               "��������� � ������� �������� Stardict.";
                           break;

case EC_CANTOPEN_THEMESDAT: e ="An error occured while opening themes.dat file.\n"
                               "Please make sure, that this file is placed\n"
                               "in Stardict work directory.";

                            r ="��������� ������ ��� �������� ����� themes.dat.\n"
                               "���������� ��������� � ���, ��� ��������� ����\n"
                               "��������� � ������� �������� Stardict.";
                            break;

case EC_CANTOPEN_KEYWORDDAT: e="An error occured while opening keyword.dat file.\n"
                               "Please make sure, that this file is placed\n"
                               "in Stardict work directory.";

                             r="��������� ������ ��� �������� ����� keyword.dat.\n"
                               "���������� ��������� � ���, ��� ��������� ����\n"
                               "��������� � ������� �������� Stardict";
                             break;

case EC_WRONG_DATA_VERSION :  e="Wrong data-file version.";
                              r="�������� ������ ������-������.";
                              break;

case EC_WRONG_FORMAT_VERSION: e="Wrong data-file format.";
                              r="�������� ������ ������-������.";
                              break;

case EC_NO_MEMORY:            e="Not enough memory.";
                              r="������������ ������ ��� ���������� ��������.";
                              break;

case EC_NOT_ENOUPH_BUF:       e="Buffer too small.";
                              r="������������� ������ ������.";
                              break;

