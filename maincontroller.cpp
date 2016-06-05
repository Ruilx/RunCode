#include "maincontroller.h"


void MainController::buildJsonResult(){

	//		QVariantMap resultMap;
	/* MESSAGE
		 * Name: this vm's name
		 * Status: Status
		 * Exitcode: exit code
		 * TimeUsed: time used
		 * MemoryUsed: memory used
		 * OutputString: result
		 */

	QJsonObject obj;
	obj["cmdId"] = 2;
	obj["name"] = this->settings.name;
	obj["status"] = ProgramStatus::getStatus(this->processStatus);
	obj["statusStr"] = ProgramStatus::getStatus(this->processStatus);
	obj["exitCode"] = this->processExitCode;
	obj["timeUsed"] = (int)this->processDuringTime;
	obj["memoryUsed"] = (int)this->processMemory;
	obj["outputString"] = this->outputString;

	QJsonDocument doc(obj);
	qDebug() << "Will send Doc:" << doc;

	//this->sendResult(doc);
	emit this->networkSend(doc);
	QThread::currentThread()->sleep(2);
	emit this->networkSend(doc);
}

void MainController::sendResult(QJsonDocument &doc){
	this->network->send(doc);
}

void MainController::sendInfo(){
	QJsonObject obj;
	obj["cmdId"] = 1;
	obj["name"] = this->settings.name;

	QJsonDocument doc(obj);

	//this->sendResult(doc);
	emit this->networkSend(doc);
}

MainController::MainController(Network *n, QObject *parent): QObject(parent){
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

	this->network = n;

	connect(this, SIGNAL(networkConnect(QString,int)), this->network, SLOT(connectTo(QString,int)));
	connect(this, SIGNAL(networkSend(QJsonDocument)), this->network, SLOT(send(QJsonDocument)));
}

MainController::~MainController(){
	//qDebug() << "MAINCONTROLLER DESTROYED.";
	foreach(QThread* thread, this->threadList){
		thread->quit();
		thread->wait();
	}
//	runProcess->deleteLater();
//	network->deleteLater();
}

void MainController::getProcessStatus(){
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
	this->buildJsonResult();
	//QThread::currentThread()->sleep(2);
}

void MainController::appendOutput(const QString &str){
	this->outputString.append(str);
}

void MainController::doExit(){
	//qDebug() << "doExit Entry";
	//qApp->quit();
	//emit this->exit();
}

void MainController::handleReceive(QJsonDocument doc){
	Q_UNUSED(doc);
}

void MainController::start(){
	QString message;
	if(!About::argProcessor(this->settings, message)){
		InputHandle output(this);
		output.puts(message);
		doExit();
		return;
	}
	if(this->settings.name.isEmpty()){
		InputHandle output(this);
		output.puts("-N argument is MUST! please get an name to runcode vm.\n");
		doExit();
		return;
	}
	if(this->settings.runPath.isEmpty()){
		InputHandle output(this);
		output.puts("-r argument is MUST! please put an runfile or cmd to runcode vm.\n");
		doExit();
		return;
	}
	if(this->settings.ip.isEmpty() || this->settings.port == 0){
		InputHandle output(this);
		output.puts("-n argument is MUST! please put an host ip:port address to runcode vm.\n");
		doExit();
		return;
	}
	//this->networkThread = new QThread(this);
//	if(!network->connectTo(this->settings.ip, this->settings.port)){
//		InputHandle output(this);
//		output.puts(QString("Network IP:%1:%2 cannot connect:%3").arg(this->settings.ip).arg(this->settings.port).arg(this->network->errorString()));
//		doExit();
//		return;
//	}
	emit this->networkConnect(this->settings.ip, this->settings.port);
	//this->threadList.append(networkThread);
	//this->network->moveToThread(networkThread);
	//connect(networkThread, SIGNAL(finished()), this->network, SLOT(deleteLater()));
	connect(network, SIGNAL(receivedData(QJsonDocument)), this, SLOT(handleReceive(QJsonDocument)));
	//networkThread->start();
	this->sendInfo();

	this->startWorking();
}

void MainController::startWorking(){

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

void MainController::finish(){
	//doExit();
	qApp->exit(0);
}
