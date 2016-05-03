#include "network.h"

Network::Network(QObject *parent) : QObject(parent)
{
	this->socket = new QTcpSocket(this);
	this->connect(this->socket, SIGNAL(connected()), this, SLOT(connected()));
	this->connect(this->socket, SIGNAL(disconnected()), this, SLOT(disconnected()));
	this->connect(this->socket, SIGNAL(readyRead()), this, SLOT(receiveData()));
	//qDebug() << "NETWORK CREATED.";
}



