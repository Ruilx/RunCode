#include "About.h"

About::About()
{

}

QString About::helpString() {
	QString appName = qApp->applicationName();
	QString a;
	a.append("Usage:\n")
			.append(appName)
			.append(" [OPTIONS]\n")
			.append("OPTIONS:\n")
			.append("  -i                 Open an session to input commands, when use it with -r, it can open session after code exit.\n")
			.append("* -r execfile        Run an specified execfile, save its stdout, memory usage, time info [MUST].\n")
			.append("  -f inputfile       Open an input file write in stdin when code is running.\n")
			.append("  -p                 Print program status to display after ran code.\n")
			.append("  -s                 Print program stdout to display while running code.\n")
			.append("* -n ip address:port Connect to host code handle [MUST].\n")
			.append("  -t time limit      Set program time limit (unit: ms)(default: 5000).\n")
			.append("  -m memory limit    Set program memory limit (unit: kb)(default: 65535).\n")
			.append("  -h                 Show this help infomation.\n")
			.append("* -N name            Give a name to runcode result to attach which code it is [MUST!].\n")
			.append("  -o output limit    Limit the program's output size(unit: byte)(default: inf).\n")
			.append("\n")
			.append("Made by Ruilx<ruilxalxa@hotmail.com> @ GT-Soft Studio 2016\n")
			.append("Based on Qt5.6.0 GCC-64bit\n")
			.append("For report bugs: please visit www.ruilx.top\n")
			.append("RunCode v0.1.0-build 0335 GT-Soft Studio.\n");
	return a;
}

QString About::argErrorHandle(QString wrongArg) {
	return QString("Error parsing option on the command line: '")
			.append(wrongArg)
			.append("'\n");
}

QString About::argParameterError(QString arg, int paraCount) {
	return QString(arg)
			.append(" require ")
			.append(QString::number(paraCount))
			.append(" parameter")
			.append(paraCount>1?"s\n":"\n");
}

QString About::ipError(QString wrongIp) {
	return QString("Error IP address: '")
			.append(wrongIp)
			.append("'\n");
}

QString About::numError(QString numStr) {
	return QString("Wrong number: '")
			.append(numStr)
			.append("'\n");
}

QString About::numRangeError(QString numStr) {
	return QString("Number cannot assign to this: '")
			.append(numStr)
			.append("'\n");
}

bool About::checkNum(QString numberStr, int *dest){
	bool ok = false;
	int num = numberStr.toInt(&ok);
	if(!ok){
		return false;
	}
	*dest = num;
	return true;
}

bool About::ipSolution(const QString &host, QString *ipAddress, int *portAddress){
	QStringList ipHostList = host.split(":");
	if(ipHostList.length() > 2){
		return false;
	}
	QString ip = ipHostList.at(0);
	QString port = ipHostList.at(1);
	QStringList ipList = ip.split(".");
	if(ipList.length() != 4){
		return false;
	}
	foreach(QString ipSeg, ipList){
		bool ok = false;
		int ipSegInt = ipSeg.toInt(&ok);
		if(ok == false){
			return false;
		}
		if(ipSegInt < 0 || ipSegInt > 255){
			return false;
		}
	}
	bool ok = false;
	int portInt = port.toInt(&ok);
	if(ok == false){
		return false;
	}
	if(portInt < 0 || portInt > 65535){
		return false;
	}
	*ipAddress = ip;
	*portAddress = portInt;
	return true;
}

bool About::argProcessor(About::ArgSettings &settings, QString &message){
	QStringList args = qApp->arguments();
	if(args.length() < 2){
		message = QString(About::helpString());
		return false;
	}
	for(int i=1; i<args.length(); i++){
		QString arg = args.at(i);
		if(arg.startsWith("-")){
			arg.remove(0, 1);
			if(arg.startsWith("-")){
				arg.remove(0, 1);
				if(0 /* arg.startWith('something') */){
					//no "--" start arguments
				}else{
					message = QString(About::argErrorHandle(QString("--").append(arg)));
					return false;
				}
				continue;
			}
			if(arg == "h"){
				message = QString(About::helpString());
				return false;
			}
			if(arg == "i"){ // -i (open input handle)
				settings.input = true;
				continue;
			}
			if(arg == "r"){
				if(args.length() <= i+1 && args.at(i+1).startsWith("-")){
					message = QString(argParameterError(QString("-").append(arg), 1));
					return false;
				}
				i++;
				settings.runPath = args.at(i);
				continue;
			}
			if(arg == "p"){
				settings.printStatus = true;
				continue;
			}
			if(arg == "s"){
				settings.printOutput = true;
				continue;
			}
			if(arg == "n"){
				if(args.length() <= i+1 && args.at(i+1).startsWith("-")){
					message = QString(argParameterError(QString("-").append(arg), 1));
					return false;
				}
				i++;
				if(!About::ipSolution(args.at(i), &settings.ip, &settings.port)){
					message = QString(ipError(args.at(i)));
					return false;
				}
				continue;
			}
			if(arg == "t"){
				if(args.length() <= i+1 && args.at(i+1).startsWith("-")){
					message = QString(argParameterError(QString('-').append(arg), 1));
					return false;
				}
				i++;
				int timeLimit;
				if(!checkNum(args.at(i), &timeLimit)){
					message = QString(numError(args.at(i)));
					return false;
				}
				if(timeLimit < 1){
					message = QString(numRangeError(args.at(i)));
					return false;
				}
				settings.timeLimit = (uint)timeLimit;
				continue;
			}
			if(arg == "m"){
				if(args.length() <= i+1 && args.at(i+1).startsWith("-")){
					message = QString(argParameterError(QString('-').append(arg), 1));
					return false;
				}
				i++;
				int memLimit;
				if(!checkNum(args.at(i), &memLimit)){
					message = QString(numError(args.at(i)));
					return false;
				}
				if(memLimit < 1){
					message = QString(numRangeError(args.at(i)));
					return false;
				}
				settings.memLimit = (uint)memLimit;
				continue;
			}
			if(arg == "f"){
				if(args.length() <= i+1 && args.at(i+1).startsWith("-")){
					message = QString(argParameterError(QString('-').append(arg), 1));
					return false;
				}
				i++;
				settings.inputPath = args.at(i);
				continue;

			}
			if(arg == "o"){
				if(args.length() <= i+1 && args.at(i+1).startsWith("-")){
					message = QString(argParameterError(QString('-').append(arg), 1));
					return false;
				}
				i++;
				int outputLimit;
				if(!checkNum(args.at(i), &outputLimit)){
					message = QString(numError(args.at(i)));
					return false;
				}
				if(outputLimit < 1){
					message = QString(numRangeError(args.at(i)));
					return false;
				}
				settings.outputLimit = (uint)outputLimit;
				continue;
			}
			if(arg == "N"){
				if(args.length() <= i+1 && args.at(i+1).startsWith("-")){
					message = QString(argParameterError(QString('-').append(arg), 1));
					return false;
				}
				i++;
				settings.name = args.at(i);
			}
			else{
				message = QString(About::argErrorHandle(arg));
				return false;
			}
		}else{
			message = QString(About::argErrorHandle(arg));
			return false;
		}
	}
	message = QString();
	return true;
}

