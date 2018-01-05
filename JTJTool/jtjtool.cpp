#include "jtjtool.h"

JTJTool::JTJTool(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	mQtimer = new QTimer(this);										//��ʱ����ʼ��
	connect(mQtimer, SIGNAL(timeout()), this, SLOT(on_CircRun()));	//�󶨲ۺ���

	ScreenshotCmd = "adb shell /system/bin/screencap -p /sdcard/screenshot.png";
	PullToPcCmd = "adb pull /sdcard/screenshot.png C:/TTT/screenshot.png";
	
	///***********************��ȡ��Ļ�ֱ��ʲ�����Ӧ*************************************
	system(ScreenshotCmd.toLatin1());								//��ȡ����
	system(PullToPcCmd.toLatin1());									//��ͼ�񴫵�����

	IplImage* TempImg = cvLoadImage("C://TTT//screenshot.png");		
	if (TempImg == NULL)
	{
		QMessageBox::warning(this, "TTT/Img is Null", "TTT/img is Null");
	}
	Hg = TempImg->height;
	Wg = TempImg->width;	///����Ӧ�ֱ���
	XY2XY(CentX, CentY);
	XY2XY(Tempg, HUpD);
	XY2XY(Tempg, GetNextXYScanBegin);
	XY2XY(Tempg, ScanceButLinY);
	XY2XY(ReStartX, ReStartY);

}

void JTJTool::on_RunBtn_clicked()
{
	if (ui.RunBtn->text() == QString::fromLocal8Bit("ִ��"))
	{
		mQtimer->start(800);										//ִ�ж�ʱ����1200ms
		ui.RunBtn->setText(QString::fromLocal8Bit("ֹͣ"));
	}
	else
	{
		mQtimer->stop();
		ui.RunBtn->setText(QString::fromLocal8Bit("ִ��"));

	}
}

JTJTool::~JTJTool()
{

}

void JTJTool::on_CircRun()
{
	//if (getInC > 28)
	//	return;
	//getInC++;
	int R		= 0;										//Rͨ��ֵ
	int G		= 0;										//Gͨ��ֵ
	int B		= 0;										//Bͨ��ֵ
	int HButY	= 0;										//���ӵײ�Y����
	int HXSum	= 0;										//����X�����
	int HXCount = 0;										//������������
	int HX		= 0;										//����λ��Xֵ
	int BackR	= 0;										//����Rͨ��ֵ
	int BackG	= 0;										//����Gͨ��ֵ
	int BackB	= 0;										//����Bͨ��ֵ
	int flag	= 0;										//��־λ
	int YTemp	= 0;										//��ʱֵ
	int BXSum	= 0;										//���Ӷ���X�����
	int BXCount = 0;										//���Ӷ�����������
	CvScalar s;												//��ȡ���ص�

	system(ScreenshotCmd.toLatin1());								//��ȡ����
	system(PullToPcCmd.toLatin1());									//��ͼ�񴫵�����
	cvWaitKey(30);													//�ӳ�30ms
	PressCmd = "adb shell input swipe 900 250 900 300 ";			//�������
	IplImage* ipl= cvLoadImage("C://TTT//screenshot.png");
	///��С���ڣ�Ϊ��UI��adb Cmdָ��ִ��ǰˢ�£�
	cvNamedWindow("ttt", CV_WINDOW_NORMAL);
	cvResizeWindow("ttt", 0, 0);


	///��ȡ����λ��
	for (int y = 0; y <ScanceButLinY; y++) {
		for (int x = 0; x < ipl->width; x++) {
			s = cvGet2D(ipl, y, x);
			R = s.val[0];
			G = s.val[1];
			B = s.val[2];
			//ʶ������
			if (G > 50 && G < 60 && B > 53 && B < 63 && R > 95 && R < 110)
			{
				HXSum += x;
				HXCount++;
				//��ȡ���ӵײ�Yֵ
				HButY = y > HButY ? y : HButY;
			}
		}
	}
	///�ж��Ƿ��ҵ�����(���ʶ��������ص���Ϊ0��Ϊδ�ҵ�)
	if (HXCount !=0)
		HX = HXSum / HXCount;


	///��ȡ��һ������λ��
	flag = 0;
	for (int y = GetNextXYScanBegin; y< HButY - HUpD; y++) {
		for (int x = 0; x < ipl->width; x++) {
			s = cvGet2D(ipl, y, x);
	///��ȡ����ֵ
			if (y == GetNextXYScanBegin)	
			{
				BackR = s.val[0];
				BackG = s.val[1];
				BackB = s.val[2];
			}
	///��ȡ��ǰ��ֵ
			R = s.val[0];
			G = s.val[1];
			B = s.val[2];
	///�ж��Ƿ��������Ϸ����Ƿ񱳾�ֵ��������Ǳ���ɫ��Ϊ��һ�����ӵ����Ϸ�λ�ã�
			if (abs(x - HX)>48)
				if (abs(R - BackR)>14 || abs(G - BackG)>14 || abs(B - BackB)>14)
				{
	///��¼���в�ֻ�����һ��
					if (flag == 0)
					{
						YTemp = y;		
						NextY = y;
						flag = 1;
					}
	///ɨ����һ����¼��������ƽ��ֵ
					if (YTemp == y)		
					{
						BXSum += x;
						BXCount++;
					}
				}

		}
	}	
	///����ƽ��ֵ���������ж�
	if(BXCount!=0)						
		NextX = BXSum / BXCount;
	
	//IplImage *ipl1 = cvLoadImage("E:\\TTT\\Back2.bmp");
	if (NextY>400&& HXCount >20)
	{


		
		///*********************************������볤�Ⱥ�·��*************************
		///����·����������ĵ㣬���ĵ�̶����䣬����б�ʺ�ǰ���������һ����������X��
		///����������������Ҫ���ľ���
		double Dist = (((HButY - HUpD) - CentY)*1.000 / abs(HX - CentX))*abs(NextX - HX);
		//NextY = HButY - HUpD - (int)Dist;//��������
		int distLong = sqrt(Dist*Dist + (NextX - HX)*(NextX - HX));
		TimeL = KKK*distLong;


		//��ǹؼ����·��
		StarP.x = HX;
		StarP.y = HButY - HUpD;

		EndP.x = NextX;
		EndP.y = StarP.y -Dist;

		cvCircle(ipl, StarP, 8, CV_RGB(0, 255, 0), 1, 8, 3);
		cvCircle(ipl, EndP, 8, CV_RGB(0, 0, 255), 1, 8, 3);
		cvLine(ipl, StarP, EndP, cvScalar(0, 33, 133), 2);



		const char* path;
		//AllCount++;
		////cvSaveImage("E:\\TTT\\Back1.bmp", ipl1);
		//if (AllCount >=1)
		//{
		//	path = "E:\\TTT\\Back2.bmp";
		//	cvSaveImage(path, ipl);
		//	AllCount = 1;
		//}
		///������Ƭ������ʾ
		path = "C:\\TTT\\Back.bmp";
		cvSaveImage(path, ipl);
		//	cvShowImage("ttt", ipl);
		
		///������־�ļ������
		SXLog = QString::number(HX, 10);
		SYLog = QString::number(HButY - HUpD, 10);
		EXLog = QString::number(NextX, 10);
		EYLog = QString::number(NextY, 10);
		PrsTimeLog = QString::number(TimeL, 10);

		Log = QString::fromLocal8Bit("��������(") + SXLog + QString::fromLocal8Bit(",") + SYLog + QString::fromLocal8Bit(")");
		ui.TextB->append(Log);
		Log = QString::fromLocal8Bit("��һ������(") + EXLog + QString::fromLocal8Bit(",") + EYLog + QString::fromLocal8Bit(")");
		ui.TextB->append(Log);
		Log = QString::fromLocal8Bit("����ʱ����") + PrsTimeLog;
		ui.TextB->append(Log);

		///*************************��ȡ���м�������ͼ����ʾ************************
		QImage img;
		img.load("C:/TTT/Back.bmp");
		QImage result = img.scaled(800, 600).scaled(ui.ShowImg->width(), ui.ShowImg->height(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);//����ͼƬ���Թ̶���С��ʾ  
		ui.ShowImg->setPixmap(QPixmap::fromImage(result));
		ui.ShowImg->show();
		update();
		cvWaitKey(20);
		///************************���ݼ��������������ʱ�䳤��***********************
		TimeL = TimeL + 20;
		if (Wg != 1440)//(2k�ֱ���)
		{
			TimeL =(int)(1.35*(double)TimeL);
		}
		else			//(1080P�ֱ���)
		{
			TimeL = TimeL+10;
		} 
		///��������ת����ָ��
		QString sx = QString::number(TimeL, 10);
		PressCmd = PressCmd + sx;
		system(PressCmd.toLatin1());
	}
	else
	{
		SaveLog();
		QString CmdReS= "adb shell input swipe ";
		QString sx = QString::number(ReStartX, 10);
		CmdReS = CmdReS + sx + " ";
		 sx = QString::number(ReStartY, 10);
		CmdReS = CmdReS + sx + " ";
		 sx = QString::number(ReStartX+10, 10);
		CmdReS = CmdReS + sx + " ";
		 sx = QString::number(ReStartY+10, 10);
		CmdReS = CmdReS + sx + " ";
		CmdReS = CmdReS + "1000";
	 //	qDebug() << CmdReS;
		system(CmdReS.toLatin1());

	}
	cvReleaseImage(&ipl);
	//cvReleaseImage(&ipl1);
	//qDebug() << "VVV";
	//qDebug() << NextY;
	//CvPoint p1=cvPoint(NextX, NextY);
	//CvPoint p2=cvPoint(ipl->width, NextY);
	//	cvLine(ipl, cvPoint(CentX, CentY), cvPoint(ipl->width, CentY), cvScalar(33, 33, 133), 2);
	//cvLine(ipl, p1, p2, cvScalar(33, 33, 133), 2);
	///��sleepʹ�߳����ߣ�����ö�ʱ���Ļ����ܴ��ڸ÷���δ������ͽ��еڶ���
	_sleep(1000);
}


void JTJTool::SaveLog()
{
	//QDateTime current_date_time = QDateTime::currentDateTime();
	//QString current_date = current_date_time.toString("yyyyMMddhhmmsszzzddd");
	//QString ImgName = "E:/TTT/Log/img/" + current_date + ".bmp";
	//QImage img;
	//img.load("E:/TTT/Back1.bmp");
	//qDebug() << ImgName;
	//img.save(ImgName);
	//QString fileName = "E:\\TTT\\Log\\TLog\\" + current_date+".txt";
	//QFile file(fileName);
	//if (!file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append))
	//{
	//}
	//QTextStream in(&file);
	//in << ui.TextB->toPlainText()<< "\n";
	//file.close();
}

void JTJTool::XY2XY(int &x, int &y)
{

	double kw = (double)Wg/ 1440.0000;
	double kh = (double)Hg/ 2560.0000;
	double Tx = (double)x;
	double Ty = (double)y;

	Tx = Tx*kw;
	Ty = Ty*kh;

	x = (int)Tx;
	y = (int)Ty;

}

void JTJTool::ReXY2XY(int &x, int &y)
{
	double kw = 1440.0000 / (double)Wg;
	double kh = 2560.0000 / (double)Hg;
	double Tx = (double)x;
	double Ty = (double)y;/*
	qDebug() << kw;
	qDebug() << kh;*/
	Tx = Tx*kw;
	Ty = Ty*kh;

	x = (int)Tx;
	y = (int)Ty;

}
