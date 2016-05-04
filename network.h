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
	~Network(){

	}

signals:
	void receivedData(QJsonDocument);
public slots:
	bool connectTo(QString ip, int port){
		this->socket->connectToHost(QHostAddress(ip), port);
		return this->socket->waitForConnected(30000);
	}

	void send(QJsonDocument doc){
		socket->write(doc.toBinaryData());
	}

private slots:
	void connected(){
		//qDebug() << "[INFO] [Network] Conneeted.";
	}

	void disconnected(){
		//qDebug() << "[INFO] [Network] Disconnected.";
	}

	void receiveData(){
		qint64 size = 0;
		do{
			size = this->socket->bytesAvailable();
			QThread::currentThread()->msleep(10);
		}while(size != this->socket->bytesAvailable());
		QJsonDocument doc = QJsonDocument::fromBinaryData(this->socket->readAll());
		if(doc.isNull() || doc.isEmpty()){
			//no receive.
			return;
		}
		emit receivedData(doc);
	}
};

#endif // NETWORK_H
