#include "runprocess.h"

bool RunProcess::killProcess(){
	if(this->process->state() != QProcess::NotRunning){
		//LogSystem::writeDebugLog(LogSystem::Warning, STR(process), "Process is running, attempt to terminate.");
		this->process->terminate();
		this->initiativeStopped = true;
		bool reply = this->process->waitForFinished(this->waitingTime);
		if(reply == false){
			if(this->process->state() != QProcess::NotRunning){
				//LogSystem::writeDebugLog(LogSystem::Warning, STR(process), "Process is still running, force kill.");
				this->process->kill();
			}
		}
	}
	if(this->process->state() != QProcess::NotRunning){
		initiativeStopped = false;
		return false;
	}else{
		return true;
	}
}

uint RunProcess::getProcessUsedMemory(){
	return this->mm->getProgramUsedMemory();
}

RunProcess::ProcessStatus RunProcess::getProcessStatus(){
	return this->processStatus;
}

int RunProcess::getExitCode(){
	return this->exitCode;
}

void RunProcess::setTimeLimit(uint timeLimit){
	this->timeLimit = timeLimit;
}

void RunProcess::setMemLimit(uint memLimit){
	this->memLimit = memLimit;
}

void RunProcess::timerEvent(QTimerEvent *event){
	killTimer(event->timerId());
	//LogSystem::writeDebugLog(LogSystem::Warning, "Timer", "Time's Up!");
	this->stopProgramBecauseOfTime();
}

void RunProcess::startCurrentProgram(){
	uint timeLimit = this->timeLimit;
	uint memLimit = this->memLimit;
	//qDebug() << "[INFOR][program]: starting Program with limit: time =" << timeLimit << "mem =" << memLimit;

	if(this->timerId == -1){
//		this->mmThread = new QThread(this);
//		this->process->start();
//		LogSystem::writeDebugLog(LogSystem::Info, "process", QString("PID: %1\n").arg(this->process->processId()));
//		//emit this->sendStdOutput(LogSystem::getLogString(LogSystem::Info, "process", QString("PID: %1\n").arg(this->process->processId())));
//		this->mm = new MemoryMonitor(this->process->processId(), memLimit);
//		this->connect(this->mm, &MemoryMonitor::memoryOverflowSig, this, &RunProcess::stopProgramBecauseOfMemory);
//		this->connect(this, &RunProcess::requestMMtoStop, this->mm, &MemoryMonitor::stopMonitor);
//		this->connect(this->mmThread, &QThread::started, this->mm, &MemoryMonitor::startMonitor);
//		this->connect(this->mmThread, &QThread::finished, this->mm, &QObject::deleteLater);
//		this->connect(this, &RunProcess::analysisFinished, this->mmThread, &QThread::quit);
//		mm->moveToThread(this->mmThread);
//		this->mmThread->start();
//		this->timerId = startTimer(timeLimit, Qt::CoarseTimer);

		this->mmThread = new QThread(this);
		this->process->start();
		this->pid = this->process->processId();
		this->mm = new MemoryMonitor(this->pid, memLimit);
		this->mm->moveToThread(this->mmThread);
		connect(this->mmThread, SIGNAL(finished()), this->mm, SLOT(deleteLater()));
		connect(this->mm, SIGNAL(memoryOverflowSig()), this, SLOT(stopProgramBecauseOfMemory()));
		connect(this->mmThread, SIGNAL(started()), this->mm, SLOT(startMonitor()));
		this->mmThread->start();
		if(this->inputFile.isOpen()){
			this->process->write(this->inputFile.readAll());
			this->process->write('\n');
		}
		this->timerId = startTimer(timeLimit, Qt::CoarseTimer);

	}else{
		//LogSystem::writeDebugLog(LogSystem::Warning, "processTimer", "Timer is already running, cannot start program.");
	}
}

RunProcess::RunProcess(const QString &filename, QString &inputFilePath, QObject *parent): QObject(parent)
{
	//qDebug() << "RUN PROCESS CREATED.";
	this->waitingTime = 5000;
	this->processStatus = Unknown;
	this->initiativeStopped = false;
	this->process = new QProcess(this);
	this->timerId = -1;
	this->timeLimit = 5000;
	this->memLimit = 65535;
	this->exitCode = -1;

	this->process->setProgram(filename);
#if QT_VERSION >= 0x050600 //if QT Version is 5.6.0 or later, QProcess::error signal called errorOccurred
	this->connect(this->process, &QProcess::errorOccurred, this, &RunProcess::errorOccurred);
#else
	//this->connect(this->process, &QProcess::error, this, &RunProcess::errorOccurred);
	this->connect(this->process, SIGNAL(error(QProcess::ProcessError)), this, SLOT(errorOccurred(QProcess::ProcessError)));
#endif
	//this->connect(this->process, &QProcess::finished, this, &RunProcess::finished);
	this->connect(this->process, SIGNAL(finished(int,QProcess::ExitStatus)), this, SLOT(finished(int,QProcess::ExitStatus)));
	this->connect(this->process, &QProcess::readyReadStandardError, this, &RunProcess::readStandardError);
	this->connect(this->process, &QProcess::readyReadStandardOutput, this, &RunProcess::readStandardOutput);
	this->connect(this->process, &QProcess::stateChanged, this, &RunProcess::stateChanged);

	if(inputFilePath.isEmpty()){
		return;
	}
	if(!QFile::exists(inputFilePath)){
		emit inputFileNotFound();
		return;
	}
	if(!this->inputFile.open(inputFile)){
		emit inputFileOpenFailed();
		return;
	}
}

void RunProcess::errorOccurred(QProcess::ProcessError error){
	switch(error){
		case QProcess::FailedToStart:
			/* The Process failed to start.
			 * Either the invoked program is missing
			 * or you may have insufficient
			 * permission to invoke the program.
			 */
			this->killProcess();
			this->processStatus = RuntimeErrorExit;
			//emit this->sendStdOutput(LogSystem::getLogString(LogSystem::Error, "process", "Process Failed to start."));
			//emit this->sendStdOutput(LogSystem::getLogString(LogSystem::Error, "process", this->process->errorString()));
			break;
		case QProcess::Crashed:
			/* The process crashed some time after
			 * started.
			 */
			this->killProcess();
			this->processStatus = RuntimeErrorExit;
			if(this->initiativeStopped){
				//LogSystem::writeDebugLog(LogSystem::Info, "User", "The program had been stoped by event.");
				//emit this->sendStdOutput(LogSystem::getLogString(LogSystem::Info, "user", "Program has been stopped."));
			}else{
				//emit this->sendStdOutput(LogSystem::getLogString(LogSystem::Error, "process", "Process has crashed. Runtime error."));
			}
			break;
		case QProcess::Timedout:
			/* The last waitfor...() function
			 * timed out. the state of QProcess
			 * is unchanged, and you can try calling
			 * waitFor...() again
			 */
			break;
		case QProcess::WriteError:
			/* Error occurred when attempting to
			 * write to the progress.
			 * process closed its input channel
			 */
			//LogSystem::writeDebugLog(LogSystem::Error, "write", "An error occurred while attempting to write");
			break;
		case QProcess::ReadError:
			/* Error occurred when attempting to
			 * read from the process.
			 * process closed or may not ran yet.
			 */
			//LogSystem::writeDebugLog(LogSystem::Error, "read", "An error occurred while attempting to read");
			break;
		case QProcess::UnknownError:
			/* An unknown error occurred
			 * OR IT IS DEFAULT VALUE CALLING error()
			 */
			break;
	}
}

void RunProcess::finished(int exitCode, QProcess::ExitStatus exitStatus){
	QList<uint> memHis;
	switch(exitStatus){
		case QProcess::NormalExit:
			/* The process exitted normally */
			emit requestMMtoStop();
			this->stopProgramSuccessfully();
			//emit this->sendStdOutput(LogSystem::getLogString(LogSystem::Info, "process", QString("Program Exitting Normally, Code: %1").arg(exitCode)));
			//LogSystem::writeDebugLog(LogSystem::Info, "process", QString("Program Exitting Normally, Code: %1").arg(exitCode));
			//emit this->sendStdOutput(LogSystem::getLogString(LogSystem::Info, "program", QString("Program used %1k memory while running.").arg(this->mm->getProgramUsedMemory())));
			memHis = this->mm->getProgramMemoryUsageHistory();
			//for(int i=0; i<memHis.length(); i++){
				//emit this->sendStdOutput(LogSystem::getLogString(LogSystem::Debug, "MemHis", QString("[%1]: %2").arg(i+1).arg(memHis.at(i))));
			//}
			this->exitCode = exitCode;
			break;
		case QProcess::CrashExit:
			/* The process crashed */
			emit requestMMtoStop();
			//this->mm->stopMonitor();
			this->stopProgramBecauseCrashed();
			//emit this->sendStdOutput(LogSystem::getLogString(LogSystem::Info, "process", QString("Program Crashed, Code: %1").arg(exitCode)));
			//LogSystem::writeDebugLog(LogSystem::Warning, "process", QString("Program Crashed, Code: %1").arg(exitCode));
			//emit this->sendStdOutput(LogSystem::getLogString(LogSystem::Info, "program", QString("Program used %1k memory while running.").arg(this->mm->getProgramUsedMemory())));
			memHis = this->mm->getProgramMemoryUsageHistory();
			//for(int i=0; i<memHis.length(); i++){
				//emit this->sendStdOutput(LogSystem::getLogString(LogSystem::Debug, "MemHis", QString("[%1]: %2").arg(i+1).arg(memHis.at(i))));
			//}
			this->exitCode = exitCode;
			break;
	}
	emit this->analysisFinished();
}

void RunProcess::readStandardOutput(){
	qint64 size;
	this->process->setReadChannel(QProcess::StandardOutput);
	do{
		size = this->process->bytesAvailable();
		QThread::currentThread()->msleep(5);
	}while(size != this->process->bytesAvailable());
	QByteArray str = this->process->readAllStandardOutput();
	if(str.isEmpty()){
		// if reads none? no output
		return;
	}
	emit this->sendStdOutput(str);
}

void RunProcess::readStandardError(){
	qint64 size;
	this->process->setReadChannel(QProcess::StandardError);
	do{
		size = this->process->bytesAvailable();
		QThread::currentThread()->msleep(10);
	}while(size != this->process->bytesAvailable());
	QByteArray str = this->process->readAllStandardError();
	if(str.isEmpty()){
		// if reads none? no output
		return;
	}
	//qDebug() << "[Debug][stderr]:" << str;
	//LogSystem::writeDebugLog(LogSystem::Debug, "stderr", str);
}

void RunProcess::stateChanged(QProcess::ProcessState newState){
	switch(newState){
		case QProcess::NotRunning:
			/* The process turned to notrunning */
			emit stoppedSig();
			break;
		case QProcess::Starting:
			/* The process attempted to start */
			break;
		case QProcess::Running:
			/* The process is running */
			emit runningSig();
			break;
	}
}

void RunProcess::stopCurrentProgram(){
	//qDebug() << "[DEBUG] [processTimer]: detected timerId:" << this->timerId;
	if(this->timerId != -1){
		killTimer(this->timerId);
		this->timerId = -1;
		//this->mm->stopMonitor();
		emit requestMMtoStop();
		//LogSystem::writeDebugLog(LogSystem::Info, "program", "Program Stopped.");
		this->initiativeStopped = true;
		this->killProcess();
	}else{
		//LogSystem::writeDebugLog(LogSystem::Debug, "processTimer", "timer Stop failure. there has no timer is running!");
		this->killProcess();
	}
}

void RunProcess::stopProgramBecauseOfMemory(){
	stopCurrentProgram();
	this->processStatus = MemoryOutExit;
}

void RunProcess::stopProgramBecauseOfTime(){
	stopCurrentProgram();
	this->processStatus = TimeOutExit;
}

void RunProcess::stopProgramSuccessfully(){
	stopCurrentProgram();
	this->processStatus = NormalExit;
}

void RunProcess::stopProgramBecauseCrashed(){
	stopCurrentProgram();
	this->processStatus = RuntimeErrorExit;
}

void RunProcess::startProgram(){
	this->startCurrentProgram();
}

void RunProcess::stopProgram(){
	stopCurrentProgram();
	this->processStatus = Unknown;
}
