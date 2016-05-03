#ifndef ABOUT_H
#define ABOUT_H

#include <QtCore>

class About
{
public:
	typedef struct{
		bool input;
		QString runPath;
		int timeLimit;
		int memLimit;
		QString ip;
		int port;
		bool printStatus;
		bool printOutput;
	}ArgSettings;

	About();
	static QString helpString() {
		QString appName = qApp->applicationName();
		QString a;
		a.append("Usage:\n")
				.append(appName)
				.append(" [OPTIONS]\n")
				.append("OPTIONS:\n")
				.append("  -i                 Open an session to input commands, when use it with -r, it can open session after code exit.\n")
				.append("  -r execfile        Run an specified execfile, save its stdout, memory usage, time info.\n")
				.append("  -p                 Print program status to display after ran code.\n")
				.append("  -s                 Print program stdout to display while running code.\n")
				.append("  -n ip address:port Connect to host code handle.\n")
				.append("  -t time limit      Set program time limit (unit: ms)(default: 5000).\n")
				.append("  -m memory limit    Set program memory limit (unit: kb)(default: 65535).\n")
				.append("\n")
				.append("Made by Ruilx<ruilxalxa@hotmail.com> @ GT-Soft Studio 2016\n")
				.append("Based on Qt5.6.0 GCC-64bit\n")
				.append("For report bugs: please visit www.ruilx.top\n")
				.append("RunCode v0.1.0-build 0335 GT-Soft Studio.\n");
		return a;
	}

	static QString argErrorHandle(QString wrongArg) {
		return QString("Error parsing option on the command line: '")
				.append(wrongArg)
				.append("'\n");
	}

	static QString argParameterError(QString arg, int paraCount) {
		return QString(arg)
				.append(" require ")
				.append(QString::number(paraCount))
				.append(" parameter")
				.append(paraCount>1?"s\n":"\n");
	}

	static QString ipError(QString wrongIp) {
		return QString("Error IP address: '")
				.append(wrongIp)
				.append("'\n");
	}

	static QString numError(QString numStr) {
		return QString("Wrong number: '")
				.append(numStr)
				.append("'\n");
	}

	static QString numRangeError(QString numStr) {
		return QString("Number cannot assign to this: '")
				.append(numStr)
				.append("'\n");
	}

	static bool checkNum(QString numberStr, int *dest){
		bool ok = false;
		int num = numberStr.toInt(&ok);
		if(!ok){
			return false;
		}
		*dest = num;
		return true;
	}

	static bool ipSolution(const QString &host, QString *ipAddress, int *portAddress){
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

	static bool argProcessor(ArgSettings &settings, QString &message){
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
				if(arg == "i"){ // -i (open input handle)
					settings.input = true;
					continue;
				}
				if(arg == "r"){
					if(args.length() <= i+1){
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
					if(args.length() <= i+1){
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
					if(args.length() <= i+1){
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
					if(args.length() <= i+1){
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
};

#endif // ABOUT_H
