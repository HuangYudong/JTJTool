#include "jtjtool.h"

JTJTool::JTJTool(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	mQtimer = new QTimer(this);										//定时器初始化
	connect(mQtimer, SIGNAL(timeout()), this, SLOT(on_CircRun()));	//绑定槽函数

	ScreenshotCmd = "adb shell /system/bin/screencap -p /sdcard/screenshot.png";
	PullToPcCmd = "adb pull /sdcard/screenshot.png C:/TTT/screenshot.png";
	
	///***********************获取屏幕分辨率并自适应*************************************
	system(ScreenshotCmd.toLatin1());								//获取截屏
	system(PullToPcCmd.toLatin1());									//将图像传到电脑

	IplImage* TempImg = cvLoadImage("C://TTT//screenshot.png");		
	if (TempImg == NULL)
	{
		QMessageBox::warning(this, "TTT/Img is Null", "TTT/img is Null");
	}
	Hg = TempImg->height;
	Wg = TempImg->width;	///自适应分辨率
	XY2XY(CentX, CentY);
	XY2XY(Tempg, HUpD);
	XY2XY(Tempg, GetNextXYScanBegin);
	XY2XY(Tempg, ScanceButLinY);
	XY2XY(ReStartX, ReStartY);

}

void JTJTool::on_RunBtn_clicked()
{
	if (ui.RunBtn->text() == QString::fromLocal8Bit("执行"))
	{
		mQtimer->start(800);										//执行定时器，1200ms
		ui.RunBtn->setText(QString::fromLocal8Bit("停止"));
	}
	else
	{
		mQtimer->stop();
		ui.RunBtn->setText(QString::fromLocal8Bit("执行"));

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
	int R		= 0;										//R通道值
	int G		= 0;										//G通道值
	int B		= 0;										//B通道值
	int HButY	= 0;										//棋子底部Y坐标
	int HXSum	= 0;										//棋子X轴求和
	int HXCount = 0;										//棋子像素总数
	int HX		= 0;										//棋子位置X值
	int BackR	= 0;										//背景R通道值
	int BackG	= 0;										//背景G通道值
	int BackB	= 0;										//背景B通道值
	int flag	= 0;										//标志位
	int YTemp	= 0;										//临时值
	int BXSum	= 0;										//箱子定点X轴求和
	int BXCount = 0;										//箱子定点像素总数
	CvScalar s;												//获取像素点

	system(ScreenshotCmd.toLatin1());								//获取截屏
	system(PullToPcCmd.toLatin1());									//将图像传到电脑
	cvWaitKey(30);													//延迟30ms
	PressCmd = "adb shell input swipe 900 250 900 300 ";			//点击重来
	IplImage* ipl= cvLoadImage("C://TTT//screenshot.png");
	///缩小窗口（为了UI在adb Cmd指令执行前刷新）
	cvNamedWindow("ttt", CV_WINDOW_NORMAL);
	cvResizeWindow("ttt", 0, 0);


	///获取棋子位置
	for (int y = 0; y <ScanceButLinY; y++) {
		for (int x = 0; x < ipl->width; x++) {
			s = cvGet2D(ipl, y, x);
			R = s.val[0];
			G = s.val[1];
			B = s.val[2];
			//识别棋子
			if (G > 50 && G < 60 && B > 53 && B < 63 && R > 95 && R < 110)
			{
				HXSum += x;
				HXCount++;
				//获取棋子底部Y值
				HButY = y > HButY ? y : HButY;
			}
		}
	}
	///判断是否找到棋子(如果识别出的像素点数为0则为未找到)
	if (HXCount !=0)
		HX = HXSum / HXCount;


	///获取下一个箱子位置
	flag = 0;
	for (int y = GetNextXYScanBegin; y< HButY - HUpD; y++) {
		for (int x = 0; x < ipl->width; x++) {
			s = cvGet2D(ipl, y, x);
	///获取背景值
			if (y == GetNextXYScanBegin)	
			{
				BackR = s.val[0];
				BackG = s.val[1];
				BackB = s.val[2];
			}
	///获取当前点值
			R = s.val[0];
			G = s.val[1];
			B = s.val[2];
	///判断是否在棋子上方，是否背景值（如果不是背景色则为下一个箱子的最上方位置）
			if (abs(x - HX)>48)
				if (abs(R - BackR)>14 || abs(G - BackG)>14 || abs(B - BackB)>14)
				{
	///记录该行并只检查这一行
					if (flag == 0)
					{
						YTemp = y;		
						NextY = y;
						flag = 1;
					}
	///扫描这一并记录，用于求平均值
					if (YTemp == y)		
					{
						BXSum += x;
						BXCount++;
					}
				}

		}
	}	
	///计算平均值并做非零判断
	if(BXCount!=0)						
		NextX = BXSum / BXCount;
	
	//IplImage *ipl1 = cvLoadImage("E:\\TTT\\Back2.bmp");
	if (NextY>400&& HXCount >20)
	{


		
		///*********************************计算距离长度和路径*************************
		///跳的路径会进过中心点，中心点固定不变，利用斜率和前面求出的下一个箱子中心X坐
		///标和棋子坐标求得需要跳的距离
		double Dist = (((HButY - HUpD) - CentY)*1.000 / abs(HX - CentX))*abs(NextX - HX);
		//NextY = HButY - HUpD - (int)Dist;//函数计算
		int distLong = sqrt(Dist*Dist + (NextX - HX)*(NextX - HX));
		TimeL = KKK*distLong;


		//标记关键点和路径
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
		///备份照片用于显示
		path = "C:\\TTT\\Back.bmp";
		cvSaveImage(path, ipl);
		//	cvShowImage("ttt", ipl);
		
		///生成日志文件并输出
		SXLog = QString::number(HX, 10);
		SYLog = QString::number(HButY - HUpD, 10);
		EXLog = QString::number(NextX, 10);
		EYLog = QString::number(NextY, 10);
		PrsTimeLog = QString::number(TimeL, 10);

		Log = QString::fromLocal8Bit("棋子坐标(") + SXLog + QString::fromLocal8Bit(",") + SYLog + QString::fromLocal8Bit(")");
		ui.TextB->append(Log);
		Log = QString::fromLocal8Bit("下一个坐标(") + EXLog + QString::fromLocal8Bit(",") + EYLog + QString::fromLocal8Bit(")");
		ui.TextB->append(Log);
		Log = QString::fromLocal8Bit("按键时长：") + PrsTimeLog;
		ui.TextB->append(Log);

		///*************************读取带有计算结果的图像并显示************************
		QImage img;
		img.load("C:/TTT/Back.bmp");
		QImage result = img.scaled(800, 600).scaled(ui.ShowImg->width(), ui.ShowImg->height(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);//放缩图片，以固定大小显示  
		ui.ShowImg->setPixmap(QPixmap::fromImage(result));
		ui.ShowImg->show();
		update();
		cvWaitKey(20);
		///************************根据计算距离计算出按键时间长短***********************
		TimeL = TimeL + 20;
		if (Wg != 1440)//(2k分辨率)
		{
			TimeL =(int)(1.35*(double)TimeL);
		}
		else			//(1080P分辨率)
		{
			TimeL = TimeL+10;
		} 
		///将计算结果转换成指令
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
	///用sleep使线程休眠，如果用定时器的话可能存在该方法未运行完就进行第二次
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
