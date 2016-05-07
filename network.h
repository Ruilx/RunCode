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
	QString errorString(){
		return this->socket->errorString();
	}

signals:
	void connected();
	void disconnected();
	void receivedData(QJsonDocument);
public slots:
	bool connectTo(QString ip, int port){
		this->socket->connectToHost(/*QHostAddress(*/ip/*)*/, port);
		return this->socket->waitForConnected(10000);
	}

	void disconnect(){
		this->socket->disconnectFromHost();
	}

	void send(QJsonDocument doc){
		if(this->socket->isOpen()){
			qDebug() << "[DEBUG][network]Send json:" << doc;
			socket->write(doc.toBinaryData());
		}else{
			qDebug() << "[ERROR][network.send]: socket is not open.";
		}
	}

private slots:
	void connectedSlot(){
		//qDebug() << "[INFO] [Network] Conneeted.";
		emit this->connected();
	}

	void disconnectedSlot(){
		//qDebug() << "[INFO] [Network] Disconnected.";
		emit this->disconnected();

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
