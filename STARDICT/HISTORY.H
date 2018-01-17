struct TViewerInfo
{
    public:
	int number;           // номер объекта
	int fline;
	int ActiveRefCode;  // передается код  активной ссылки
};

class TViewerHistory
{
     public:
	TViewerInfo history[MAX_HISTORY];  // информация о предыдущих статьях
	int hnum; // кол-во
	int curnum; // вспомогательная
	int count;    // информация
	TViewerHistory();
	virtual ~TViewerHistory();
	void AppendLast( int n, int fl, int arc); //добавить инф. о статье ( теме )
	void Clear();
};
