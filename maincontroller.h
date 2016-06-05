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
		RestrictedFunction,
		Running,
		Hidden,
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
	void buildJsonResult();
	void sendResult(QJsonDocument &doc);
	void sendInfo();
public:
	explicit MainController(Network *n, QObject *parent = 0);
	~MainController();
signals:
	void startInputHandleThread();
	void startRunProcessThread();
	void exit();

	void networkSend(QJsonDocument doc);
	void networkConnect(QString ip, int port);
private slots:
	void getProcessStatus();
	void appendOutput(const QString &str);
	void doExit();
	void handleReceive(QJsonDocument doc);
public slots:
	void start();
	void startWorking();
	void finish();
};

#endif // MAINCONTROLLER_H
