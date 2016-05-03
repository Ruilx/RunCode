#ifndef MAINCONTROLLER_H
#define MAINCONTROLLER_H

#include <QObject>
#include <QtCore>
#include <QThread>
#include "helper/About.h"
#include "inputhandle.h"
#include "runprocess.h"

class MainController : public QObject
{
	Q_OBJECT
	About::ArgSettings settings;
	RunProcess *runProcess;
	uint processMemory;
	RunProcess::ProcessStatus processStatus;
	int processExitCode;
	QString outputString;

	QThread *runProcessThread;
	QThread *inputHandleThread;
	QList<QThread*> threadList;

public:
	explicit MainController(QObject *parent = 0): QObject(parent){
		//qDebug() << "MAINCONTROLLER CREATED.";
		this->settings.input = false;
		this->settings.runPath = QString();
		this->settings.ip = QString();
		this->settings.port = -1;
		this->settings.timeLimit = 5000;
		this->settings.memLimit = 65535;
		this->settings.printOutput = false;
		this->settings.printStatus = false;
		this->processMemory = 0;
		this->processStatus = RunProcess::Unknown;
	}
	~MainController(){
		//qDebug() << "MAINCONTROLLER DESTROYED.";
		foreach(QThread* thread, this->threadList){
			thread->quit();
			thread->wait();
		}
	}

signals:
	void startInputHandleThread();
	void startRunProcessThread();

	void exit();
private slots:
	void getProcessStatus(){
		InputHandle inputHandle;
		this->processMemory = this->runProcess->getProcessUsedMemory();
		this->processStatus = this->runProcess->getProcessStatus();
		this->processExitCode = this->runProcess->getExitCode();
		this->runProcessThread->quit();
		if(this->settings.printStatus){
			QString statusStr = "Program exit %1, exit code: %2, used memory: %3k.\n";
			QString str;
			switch(this->processStatus){
				case RunProcess::Unknown:
					str.append("but unknown");
					break;
				case RunProcess::NormalExit:
					str.append("normally");
					break;
				case RunProcess::RuntimeErrorExit:
					str.append("in runtime error");
					break;
				case RunProcess::TimeOutExit:
					str.append("in time out");
					break;
				case RunProcess::MemoryOutExit:
					str.append("in memory out");
					break;
				default:
					str.append("RunCode source issue");
			}
			inputHandle.puts(statusStr.arg(str).arg(this->processExitCode).arg(this->processMemory));
		}
	}

	void appendOutput(const QString &str){
		this->outputString.append(str);
	}

	void doExit(){
		//qDebug() << "doExit Entry";
		qApp->quit();
		//emit this->exit();
	}

public slots:
	void start(){
		QString message;
		if(!About::argProcessor(this->settings, message)){
			InputHandle output(this);
			output.puts(message);
			doExit();
			return;
		}
		InputHandle *output = new InputHandle(this);

		this->inputHandleThread = new QThread(this);
		if(this->settings.input){
			InputHandle *inputHandle = new InputHandle;
			this->threadList.append(this->inputHandleThread);
			inputHandle->moveToThread(this->inputHandleThread);
			connect(inputHandleThread, SIGNAL(finished()), inputHandle, SLOT(deleteLater()));
			connect(this, SIGNAL(startInputHandleThread()), inputHandle, SLOT(start()));
			//connect(inputHandle, SIGNAL(requestToExit()), this, SLOT(doExit()));
			this->inputHandleThread->start();
		}
		this->runProcessThread = new QThread(this);
		if(!this->settings.runPath.isEmpty()){
			this->threadList.append(this->runProcessThread);
			this->runProcess = new RunProcess(this->settings.runPath);
			this->runProcess->setTimeLimit(this->settings.timeLimit);
			this->runProcess->setMemLimit(this->settings.memLimit);
			runProcess->moveToThread(this->runProcessThread);
			connect(runProcessThread, SIGNAL(finished()), runProcess, SLOT(deleteLater()));
			connect(this, SIGNAL(startRunProcessThread()), runProcess, SLOT(startProgram()));
			connect(runProcess, SIGNAL(analysisFinished()), this, SLOT(getProcessStatus()));
			if(this->settings.printOutput){
				this->connect(runProcess, SIGNAL(sendStdOutput(QString)), output, SLOT(puts(QString)));
			}
			this->connect(runProcess, SIGNAL(sendStdOutput(QString)), this, SLOT(appendOutput(QString)));
			this->runProcessThread->start();
		}
		if(this->settings.input && this->runProcessThread->isRunning()){
			connect(runProcessThread, SIGNAL(finished()), output, SLOT(start()));
		}else if(this->settings.input && !this->runProcessThread->isRunning()){
			emit this->startInputHandleThread();
		}
		if((!this->settings.runPath.isEmpty()) && (this->settings.input == false)){
			connect(runProcessThread, SIGNAL(finished()), this, SLOT(finish()));
		}

		if(!this->settings.runPath.isEmpty()){
			emit this->startRunProcessThread();
		}

//		this->runProcessThread = new QThread(this); // for running code
//		connect(this, &MainController::destroyed, this->runProcessThread, &QThread::deleteLater);
//		this->inputHandleThread = new QThread(this); //for input handle
//		connect(this, &MainController::destroyed, this->inputHandleThread, &QThread::deleteLater);
//		InputHandle *inputHandle = new InputHandle;

//		if(!this->settings.runPath.isEmpty()){
//			this->process = new RunProcess(this->settings.runPath);
//			process->setTimeLimit(this->settings.timeLimit);
//			process->setMemLimit(this->settings.memLimit);
//			process->moveToThread(runProcessThread);
//			this->connect(runProcessThread, &QThread::started, process, &RunProcess::startProgram);
//			if(this->settings.printOutput){
//				this->connect(process, &RunProcess::sendStdOutput, inputHandle, &InputHandle::puts);
//				//this->connect(runProcess, &RunProcess::sendStdError, inputHandle, &InputHandle::puts);
//			}
//			this->connect(process, &RunProcess::sendStdOutput, this, &MainController::appendOutput);
//			this->connect(process, &RunProcess::analysisFinished, this, &MainController::getProcessStatus);
//		}
//		if(this->settings.input){
//			inputHandle->moveToThread(inputHandleThread);
//			this->connect(inputHandleThread, &QThread::started, inputHandle, &InputHandle::start);
//			this->connect(inputHandleThread, &QThread::finished, inputHandle, &InputHandle::finish);
//			this->connect(inputHandleThread, &QThread::finished, QThread::currentThread(), &QThread::quit);
//			this->connect(inputHandleThread, &QThread::finished, runProcessThread, &QThread::quit);
//			//this->connect(runProcessThread, &QThread::finished, inputHandleThread, &QThread::start);
//			this->connect(inputHandle, &InputHandle::requestToExit, this, &MainController::deleteLater);
//			if(this->settings.runPath.isEmpty()){
//				inputHandleThread->start();
//			}else{
//				this->connect(runProcessThread, SIGNAL(finished()), inputHandleThread, SLOT(start()));
//			}
//		}
//		if(!this->settings.runPath.isEmpty()){
//			runProcessThread->start();
//		}else{
//			//QThread::currentThread()->quit();
//			runProcessThread->quit();
//		}
	}

	void finish(){
		doExit();
	}
};

#endif // MAINCONTROLLER_H
