#include "inputhandle.h"



InputHandle::InputHandle(QObject *parent): QObject(parent){
	//qDebug() << "INPUT HANDLE CREATED.";
	this->fout = new QFile(this);
	if(!fout->open(stdout, QIODevice::WriteOnly)){

		emit this->requestToExit();
		QThread::currentThread()->quit();
		return;
	}
	sout.setDevice(fout);
	this->fin = new QFile(this);
	if(!fin->open(stdin, QIODevice::ReadOnly)){
		sout << LogSystem::getLogString(LogSystem::Fatal, "System", "Cannot bind current input stream, starting command mode failed.");

		emit this->requestToExit();
		QThread::currentThread()->quit();
		return;
	}
	sin.setDevice(fin);
	//sout << LogSystem::getLogString(LogSystem::Note, "System", "Welcome to RunCode Command Mode!");
}

InputHandle::~InputHandle(){
	//qDebug() << "INPUTHANDLE DESTROYED.";
	//		if(this->fin->isOpen()){
	//			this->fin->close();
	//		}
	//		if(this->fout->isOpen()){
	//			this->fout->close();
	//		}
}

void InputHandle::putString(QString str){
	sout.flush();
	sout << str;
}

void InputHandle::start(){
	sout << LogSystem::getLogString(LogSystem::Note, "System", "Welcome to RunCode Command Mode!");
loop:
	sout << ">";
	this->sout.flush();
	QString line = sin.readLine();
	QStringList inputLine(line.split(QRegExp("\\s"), QString::SkipEmptyParts));
	if(inputLine.size() == 0){
		//sout << LogSystem::getLogString(LogSystem::Error, "System", "No command found.");
		goto loop;
	}
	QString cmd = inputLine.at(0);
	if(cmd == "quit" || cmd == "exit"){
		sout << LogSystem::getLogString(LogSystem::Info, "System", "Quitting...");

		//emit this->requestToExit();
		return;
	}else if(cmd == "help"){
		sout << LogSystem::getLogString(LogSystem::Info, "System", "Usage:");
		sout << LogSystem::getLogString(LogSystem::Info, "System", "Can Use Command:");
		sout << LogSystem::getLogString(LogSystem::Info, "System", "exit quit say");
		goto loop;
	}else if(cmd == "say"){
		for(auto i = inputLine.begin() + 1; i != inputLine.end(); i++){
			sout << *i << " ";
		}
		sout << "\n";
		goto loop;
	}else if(cmd == "output"){
		//TODO: 之后完善, 组件全部移入MainController, 这个只做功能设置
	}else{
		sout << LogSystem::getLogString(LogSystem::Error, "System", QString("Unknown Command: %1").arg(cmd));
		goto loop;
	}
	goto loop;
}

void InputHandle::puts(const QString &string){
	this->putString(string);
}
