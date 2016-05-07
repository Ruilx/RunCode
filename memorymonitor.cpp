#include "memorymonitor.h"
#include <QLoggingCategory>

void MemoryMonitor::timerEvent(QTimerEvent *){
#if defined(Q_OS_LINUX)
	if(!this->statm->open(QIODevice::ReadOnly)){
		//LogSystem::writeDebugLog(LogSystem::Debug, "MMdev", "open statm failed.");
	}else{
		QByteArray memInfoBytea = this->statm->readAll();
		uint mem = (memInfoBytea.split(' ')[5].toInt()) * 4;
		if(mem > this->limitMemory){
			//Memory Overflow
			this->statm->close();
			this->stopMonitor();
			memUsageHistory << mem;
			emit memoryOverflowSig();
			return;
		}else{
			usedMemory = qMax(usedMemory, mem);
		}
		memUsageHistory << mem;
	}
	this->statm->close();
#elif defined(Q_OS_WIN)
	DWORD submitSize = 0;

	submitSize = getProcessMemory(this->pid);
	if(submitSize == 0){
		//LogSystem::writeDebugLog(LogSystem::Debug, "MMdev", "open process failed.");
	}
	submitSize /= 1024;
	if(submitSize > this->limitMemory){
		//Memory overflow
		this->stopMonitor();
		memUsageHistory << submitSize;
		emit memoryOverflowSig();
		return;
	}else{
		usedMemory = qMax(usedMemory, (uint)submitSize);
	}
	memUsageHistory << submitSize;
#endif
}

MemoryMonitor::MemoryMonitor(int pid, uint limitMemory, QObject *parent) : QObject(parent)
{
	//qDebug() << "MEMORYMONITOR CREATED.";
	this->usedMemory = 0;
	this->timerId = -1;
	this->pid = pid;
	this->limitMemory = limitMemory;
#if defined(Q_OS_LINUX)
	this->statm = new QFile(QString("/proc/%1/statm").arg(pid));
#endif

	this->interval = 10; //10ms to monitor memoryinfo
}

void MemoryMonitor::startMonitor(){
	if(this->timerId == -1){
		//qDebug() << "[DEBUG][startTimer]: Timer start.";
		this->timerId = this->startTimer(this->interval, Qt::CoarseTimer);
		//qDebug() << "[DEBUG][Timer]: timerId:" << this->timerId;
	}
	this->timerEvent(0);
}

void MemoryMonitor::stopMonitor(){
	if(this->timerId != -1){
		//qDebug() << "[DEBUG][stopTimer]: Timer Stop.";
		this->killTimer(this->timerId);
		this->timerId = -1;
	}
}
