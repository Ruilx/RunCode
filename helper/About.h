#ifndef ABOUT_H
#define ABOUT_H

#include <QtCore>

class About
{
public:
	typedef struct{
		bool input;
		QString runPath;
		QString inputPath;
		int timeLimit;
		int memLimit;
		QString ip;
		int port;
		bool printStatus;
		bool printOutput;
		uint outputLimit;
		QString name;
	}ArgSettings;

	About();
	static QString helpString();
	static QString argErrorHandle(QString wrongArg);
	static QString argParameterError(QString arg, int paraCount);
	static QString ipError(QString wrongIp);
	static QString numError(QString numStr);
	static QString numRangeError(QString numStr);
	static bool checkNum(QString numberStr, int *dest);
	static bool ipSolution(const QString &host, QString *ipAddress, int *portAddress);
	static bool argProcessor(ArgSettings &settings, QString &message);
};

#endif // ABOUT_H
