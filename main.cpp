#include <QCoreApplication>
#include "maincontroller.h"

int main(int argc, char *argv[])
{
	QCoreApplication a(argc, argv);
//	QThread inputHandleThread;
//	InputHandle inputHandle;
//	inputHandle.moveToThread(&inputHandleThread);
//	QObject::connect(&inputHandleThread, &QThread::started, &inputHandle, &InputHandle::start);
//	QObject::connect(&inputHandleThread, &QThread::finished, &inputHandle, &InputHandle::finish);
//	inputHandleThread.start();

	QThread mainThread;
	MainController mainController;
	mainController.moveToThread(&mainThread);
	QObject::connect(&mainThread, &QThread::started, &mainController, &MainController::start);
	QObject::connect(&mainThread, &QThread::finished, &mainController, &MainController::finish);
	//QObject::connect(&mainThread, &QThread::finished, &mainController, &MainController::deleteLater);
	//QObject::connect(&mainThread, SIGNAL(finished()), qApp, SLOT(quit()));
	//QObject::connect(&mainController, SIGNAL(exit()), &mainThread, SLOT(quit()));
	mainThread.start();

	int ret = a.exec();

	mainThread.quit();
	mainThread.wait();

	return ret;
}
