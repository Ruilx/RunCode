#ifndef NETWORK_H
#define NETWORK_H

#include <QObject>
#include <QTcpSocket>
#include <QFile>
#include <QTextStream>
#include <QJsonDocument>
#include <QThread>
#include <QHostAddress>

class Network : public QObject
{
	Q_OBJECT
	QTcpSocket *socket;
public:
	explicit Network(QObject *parent = 0);
	~Network();
	QString errorString();
signals:
	void connected();
	void disconnected();
	void receivedData(QJsonDocument);
public slots:
	bool connectTo(QString ip, int port);
	void disconnect();
	void send(QJsonDocument doc);
private slots:
	void connectedSlot();
	void disconnectedSlot();
	void receiveData();
	void bytesWritten(qint64 size);
};

#endif // NETWORK_H
