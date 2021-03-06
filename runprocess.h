#ifndef RUNPROCESS_H
#define RUNPROCESS_H

#define STR(x) #x

#include <QObject>
#include <QtCore>
#include "memorymonitor.h"
#include "utils/logsystem.h"

class RunProcess: public QObject
{
	Q_OBJECT
public:
	enum ProcessStatus{
		Unknown = -1,
		NormalExit = 0,
		RuntimeErrorExit = 1,
		TimeOutExit = 2,
		MemoryOutExit = 4,
		OutputLimit = 8,
	};
private:
	QFile inputFile;
	int waitingTime;
	int timerId;
	bool initiativeStopped;
	uint timeLimit;
	uint memLimit;
	uint outputLimit;
	uint outputLength;
	MemoryMonitor *mm;
	QThread *mmThread;
	QElapsedTimer duringTimer; // this timer will replace by cpumonitor
	uint duringTime;
	int exitCode;

	ProcessStatus processStatus;
	QProcess *process;
	int pid;

	bool killProcess();
	void timerEvent(QTimerEvent *event);

	void stopCurrentProgram();
	void startCurrentProgram();
public:

	RunProcess(const QString &filename, QString inputFilePath = QString(), uint outputLimit = 0, QObject *parent = nullptr);
	~RunProcess();

	uint getProcessUsedMemory();
	ProcessStatus getProcessStatus();
	int getExitCode();
	uint getDuringTime();

	void setTimeLimit(uint timeLimit);
	void setMemLimit(uint memLimit);

signals:
	void stoppedSig();
	void runningSig();

	void requestMMtoStop();
	void analysisFinished();

	void sendStdOutput(QString);
	void sendStdError(QString);
	void inputFileNotFound();
	void inputFileOpenFailed();

	void runFileNotFound();

private slots:
	void errorOccurred(QProcess::ProcessError error);
	void finished(int exitCode, QProcess::ExitStatus exitStatus);
	void readStandardError();
	void readStandardOutput();
	void stateChanged(QProcess::ProcessState newState);
	void stopProgramBecauseOfMemory();
	void stopProgramBecauseOfTime();
	void stopProgramSuccessfully();
	void stopProgramBecauseCrashed();
	void stopProgramBecauseOfOutputLimit();
public slots:
	void startProgram();
	void stopProgram();
};

#endif // RUNPROCESS_H
