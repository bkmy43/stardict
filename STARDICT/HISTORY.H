struct TViewerInfo
{
    public:
	int number;           // ����� ��ꥪ�
	int fline;
	int ActiveRefCode;  // ��।����� ���  ��⨢��� ��뫪�
};

class TViewerHistory
{
     public:
	TViewerInfo history[MAX_HISTORY];  // ���ଠ�� � �।���� ������
	int hnum; // ���-��
	int curnum; // �ᯮ����⥫쭠�
	int count;    // ���ଠ��
	TViewerHistory();
	virtual ~TViewerHistory();
	void AppendLast( int n, int fl, int arc); //�������� ���. � ���� ( ⥬� )
	void Clear();
};
