#include <QCoreApplication>
#include "maincontroller.h"
#include "network.h"

int main(int argc, char *argv[])
{
	QCoreApplication a(argc, argv);

	Network *n = new Network;

	//QThread *mainThread = new QThread;
	MainController *mainController = new MainController(n);
	//mainController->moveToThread(mainThread);
	//QObject::connect(mainThread, &QThread::started, mainController, &MainController::start);
	//QObject::connect(mainThread, &QThread::finished, mainController, &MainController::finish);
	//QObject::connect(&mainThread, &QThread::finished, &mainController, &MainController::deleteLater);
	//mainThread->start();
	mainController->start();

	int ret = a.exec();

	//mainThread->quit();
	//mainThread->wait();

	return ret;
}
