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
	uint processDuringTime;
	QString outputString;

	QThread *runProcessThread;
	QThread *inputHandleThread;
	QList<QThread*> threadList;

public:
	explicit MainController(QObject *parent = 0): QObject(parent){
		//qDebug() << "MAINCONTROLLER CREATED.";
		this->settings.input = false;
		this->settings.runPath = QString();
		this->settings.inputPath = QString();
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
		this->processDuringTime = this->runProcess->getDuringTime();
		this->runProcessThread->quit();
		if(this->settings.printStatus){
			QString statusStr = "Program exit %1, exit code: %2, used memory: %3k, during time: %4ms.\n";
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
				case RunProcess::OutputLimit:
					str.append("in output limit exceeded");
					break;
				default:
					str.append("unfamiliar process status");
			}
			inputHandle.puts(statusStr.arg(str).arg(this->processExitCode).arg(this->processMemory).arg(this->processDuringTime));
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
			this->runProcess = new RunProcess(this->settings.runPath, this->settings.inputPath, this->settings.outputLimit);
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
	}

	void finish(){
		doExit();
	}
};

#endif // MAINCONTROLLER_H
