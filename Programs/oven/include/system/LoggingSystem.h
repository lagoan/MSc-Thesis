/*
 *  LoggingSystem.h
 *  oven
 *
 *  Created by Omar Rodriguez-Arenas on 06/08/07.
 *  Copyright 2007 __MyCompanyName__. All rights reserved.
 *
 */

/*
 * LogginSystem provides a log mechanism available to the whole application in 
 * the form on a singleton. The singleton is thread safe, though the tab 
 * information is not guaranteed to work as intended if more that one thread
 * is changing it
 */

#ifndef LOGGINGSYSTEM_H
#define LOGGINGSYSTEM_H

#include <QtGui>
#include <QString>

#include "system/types.h"

#define LOGS LoggingSystem::getInstance() 

enum EventType {
	Normal,
	Error,
	Important,
	Critical
};
 
class LoggingSystem 
{

public:

	static LoggingSystem* getInstance();
	
	void setLogTextEdit(QTextEdit* t);
	void logEvent(QString s, EventType t=Normal);

protected:

	LoggingSystem();
	~LoggingSystem(){};
	void logEventColor(QString& s, QColor color);
	
private:
	
	QTextEdit* _location;
	
	QMutex _locationMutex;
	
	LoggingSystem(const LoggingSystem &s);
	LoggingSystem &operator = (const LoggingSystem &s);
	
};

#endif