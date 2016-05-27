#ifndef MEMORYMONITOR_H
#define MEMORYMONITOR_H

#include <QtCore>
#include <QObject>
#include "utils/logsystem.h"
#if defined(Q_OS_WIN)
//#	include <winbase.h>
#	include <windows.h>
#	include "include/MemoryInfo.h"
#endif

class MemoryMonitor : public QObject
{
	Q_OBJECT
	int pid;
	int interval;
	int timerId;
	uint limitMemory;
	uint usedMemory;
#if defined(Q_OS_LINUX)
	QFile *statm;
#endif
	QList<uint> memUsageHistory;

	void timerEvent(QTimerEvent *);
public:
	explicit MemoryMonitor(int pid, uint limitMemory, QObject *parent = 0);
	~MemoryMonitor();

	uint getProgramUsedMemory();
	void setInterval(uint interval);
	QList<uint> getProgramMemoryUsageHistory();

signals:
	void memoryOverflowSig();
public slots:
	void startMonitor();
	void stopMonitor();
};

#endif // MEMORYMONITOR_H
