#ifndef MAINCONTROLLER_H
#define MAINCONTROLLER_H

#include <QObject>
#include <QtCore>
#include <QThread>
#include "helper/About.h"
#include "inputhandle.h"
#include "runprocess.h"
#include "network.h"

class ProgramStatus{
public:
	/* Status
	 * 0 Unknown
	 * 1 Accepted
	 * 2 Presentation Error
	 * 3 Wrong Answer
	 * 4 Output Limit Excedded
	 * 5 Validator Error
	 * 6 Memory Limit Excedded
	 * 7 Time Limit Excedded
	 * 8 Runtime Error
	 * 9 Compile Error
	 * 10 System Error
	 * 11 Running
	 * 12 Hidden
	 * 13 Restricted Function
	 */
	enum Status{
		Unknown = 0,
		Accepted,
		PresentationError,
		WrongAnswer,
		OutputLimitExcedded,
		ValidatorError,
		MemoryLimitExcedded,
		TimeLimitExcedded,
		RuntimeError,
		CompileError,
		SystemError,
		Running,
		Hidden,
		RestrictedFunction
	};

	static Status getStatus(RunProcess::ProcessStatus status){
		switch(status){
			case RunProcess::NormalExit:
				return Accepted;
			case RunProcess::RuntimeErrorExit:
				return RuntimeError;
			case RunProcess::TimeOutExit:
				return TimeLimitExcedded;
			case RunProcess::MemoryOutExit:
				return MemoryLimitExcedded;
			case RunProcess::OutputLimit:
				return OutputLimitExcedded;
			case RunProcess::Unknown:
				return Unknown;
		}
		return Unknown;
	}
};

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
	//QThread *networkThread;
	QList<QThread*> threadList;

	Network *network;

	void buildJsonResult(){
		QVariantList result;

		QVariantMap resultMap;
		/* MESSAGE
		 * Name: this vm's name
		 * Status: Status
		 * Exitcode: exit code
		 * TimeUsed: time used
		 * MemoryUsed: memory used
		 * OutputString: result
		 */

		resultMap.insert("Name", this->settings.name);
		resultMap.insert("Status", ProgramStatus::getStatus(this->processStatus));
		resultMap.insert("ExitCode", this->processExitCode);
		resultMap.insert("TimeUsed", this->processDuringTime);
		resultMap.insert("MemoryUsed", this->processMemory);
		resultMap.insert("OutputString", this->outputString);

		result << resultMap;

		QJsonDocument doc = QJsonDocument::fromVariant(result);
		if(doc.isNull()){
			qDebug() << "[ERROR][jsontake]: result JSON is Empty!";
			return;
		}
		qDebug() << "[DEBUG][jsonTake]: JSON:" << doc;

		this->sendResult(doc);

	}

	void sendResult(QJsonDocument &doc){
		this->network->send(doc);
	}

	void sendInfo(){
		QVariantList info;

		QVariantMap infoMap;

		infoMap.insert("Name", this->settings.name);

		info << infoMap;

		QJsonDocument doc = QJsonDocument::fromVariant(info);
		if(doc.isNull()){
			qDebug() << "[ERROR][jsontake]: info JSON is Empty!";
			return;
		}
		qDebug() << "[DEBUG][jsonTake]: JSON:" << doc;

		this->sendResult(doc);
	}


public:
	explicit MainController(Network *n, QObject *parent = 0): QObject(parent){
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
		this->buildJsonResult();
	}

	void appendOutput(const QString &str){
		this->outputString.append(str);
	}

	void doExit(){
		//qDebug() << "doExit Entry";
		qApp->quit();
		//emit this->exit();
	}

	void handleReceive(QJsonDocument doc){
		/* Doc:
		 * "name" = NAME
		 * "message" = MESSAGE
		 */
		/*message
		 * 0: [ErrorCode]
		 * 1: startRunning
		 * 2: stopRunning
		 * 3: showcurrentStatus
		 */
		if(doc.isObject()){
			QVariantMap result = doc.toVariant().toMap();
			QString name = result.value("name", QString()).toString();
			int message = result.value("message", -1).toInt();

			if(name != this->settings.name){
				return;
			}
			switch(message){
				case 0:
					return;
				case 1:
					this->startWorking();
					break;
				case 2:
					this->runProcess->stopProgram();
					break;
				case 3:
//					this->sendCurrentStatus();
					break;
				default:
					return;
			}
		}
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
		if(!network->connectTo(this->settings.ip, this->settings.port)){
			InputHandle output(this);
			output.puts(QString("Network IP:%1:%2 cannot connect:%3").arg(this->settings.ip).arg(this->settings.port).arg(this->network->errorString()));
			doExit();
			return;
		}
		//this->threadList.append(networkThread);
		//this->network->moveToThread(networkThread);
		//connect(networkThread, SIGNAL(finished()), this->network, SLOT(deleteLater()));
		connect(network, SIGNAL(receivedData(QJsonDocument)), this, SLOT(handleReceive(QJsonDocument)));
		//networkThread->start();
		this->sendInfo();
	}

	void startWorking(){

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
