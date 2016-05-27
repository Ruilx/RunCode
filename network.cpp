#include "network.h"

Network::Network(QObject *parent) : QObject(parent)
{
	this->socket = new QTcpSocket(this);
	this->connect(this->socket, SIGNAL(connected()), this, SLOT(connectedSlot()));
	this->connect(this->socket, SIGNAL(disconnected()), this, SLOT(disconnectedSlot()));
	this->connect(this->socket, SIGNAL(readyRead()), this, SLOT(receiveData()));
	//this->connect(this->socket, SIGNAL(bytesWritten(qint64)), this, SLOT(bytesWritten(qint64)));
	//qDebug() << "NETWORK CREATED.";
}

Network::~Network(){

}

QString Network::errorString(){
	return this->socket->errorString();
}

bool Network::connectTo(QString ip, int port){
	this->socket->connectToHost(/*QHostAddress(*/ip/*)*/, port);
	return this->socket->waitForConnected(10000);
}

void Network::disconnect(){
	this->socket->disconnectFromHost();
}

void Network::send(QJsonDocument doc){
	if(this->socket->isOpen()){
		socket->write(doc.toBinaryData());
		qDebug() << "[DEBUG][network]Sent json:" << doc;
	}else{
		qDebug() << "[ERROR][network.send]: socket is not open.";
	}
}

void Network::connectedSlot(){
	//qDebug() << "[INFO] [Network] Conneeted.";
	emit this->connected();
}

void Network::disconnectedSlot(){
	//qDebug() << "[INFO] [Network] Disconnected.";
	emit this->disconnected();

}

void Network::receiveData(){
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

void Network::bytesWritten(qint64 size){
	qDebug() << "Socket bytes written. size:" << size;
}



