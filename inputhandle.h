#ifndef INPUTHANDLE_H
#define INPUTHANDLE_H

#include <QObject>
#include <QtCore>
#include "utils/logsystem.h"

class InputHandle : public QObject
{
	Q_OBJECT

	QFile *fin;
	QFile *fout;
	QTextStream sin;
	QTextStream sout;

public:
	explicit InputHandle(QObject *parent = nullptr);
	~InputHandle();

	void putString(QString str);

signals:
	void requestToExit();
public slots:
	void start();
	void puts(const QString &string);
};

#endif // INPUTHANDLE_H
