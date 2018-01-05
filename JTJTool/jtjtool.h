#ifndef JTJTOOL_H
#define JTJTOOL_H
#include <QtWidgets/QMainWindow>
#include "ui_jtjtool.h"
#include "cv.h"
#include "qdebug.h"
#include "highgui.h" 
#include <qtimer.h>
#include <qdatetime.h>
#include <qfile.h>
#include<qscreen.h>
#include <qmessagebox.h>

class JTJTool : public QMainWindow
{
	Q_OBJECT
public:
	JTJTool(QWidget *parent = 0);
	int getInC = 0;
	~JTJTool();
	IplImage* bw = NULL;
	IplImage* color = NULL;
	IplImage* jh = NULL;
	IplImage* sm = NULL;
public slots:
	void on_RunBtn_clicked();
	void on_CircRun();
private:
	QString ScreenshotCmd;					//����ָ��
	QString PullToPcCmd;					//��ͼָ��
	QString PressCmd;						//ģ����ָ��
	int Tempg = 0;
	int AllCount = 0;
	Ui::JTJToolClass ui;
	QTimer *mQtimer;						//��ʱ��
	int HUpD = 32;
	int GetNextXYScanBegin = 550;
	int ScanceButLinY = 1814;
	CvPoint StarP;
	CvPoint EndP;
	CvPoint CentP;
	int Hg;									//ȫ�ֱ������
	int	Wg;									//ȫ����Ť�߶�	
	int NextX = 0;
	int NextY = 0;
	int CentX = 751;
	int CentY = 1307;
	int TimeL = 0;
	double KKK = 1;
	int ReStartX = 672;
	int ReStartY = 2126;
	QString Log;
	QString SXLog;
	QString SYLog;
	QString EXLog;
	QString EYLog;
	QString PrsTimeLog;
	void SaveLog();
	void XY2XY(int &x,int &y);				//�ֱ���ת������
	void ReXY2XY(int &x, int &y);			//�ֱ��ʷ�ת����
};

#endif // JTJTOOL_H
