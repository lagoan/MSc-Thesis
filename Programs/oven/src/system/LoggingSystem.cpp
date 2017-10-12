/*
 *  LoggingSystem.cpp
 *  oven
 *
 *  Created by Omar Rodriguez-Arenas on 06/08/07.
 *  Copyright 2007 __MyCompanyName__. All rights reserved.
 *
 */

#include "LoggingSystem.h"

LoggingSystem::LoggingSystem()
{
	_location = NULL;
}

LoggingSystem* LoggingSystem::getInstance()
{
	static LoggingSystem* loggingSystem = new LoggingSystem();
	return loggingSystem;

}

void LoggingSystem::setLogTextEdit(QTextEdit* t)
{
	_locationMutex.lock();
	_location = t;
	_location->ensureCursorVisible();
	_locationMutex.unlock();

}


void LoggingSystem::logEvent(QString s, EventType t)
{
	
	QString _output;
	
	_output = s;
	
	switch(t) {
		case Normal:
			logEventColor(_output, Qt::darkGray);
			break;
		case Error:
			logEventColor(_output, Qt::red);
			break;
		case Important:
			logEventColor(_output, Qt::black);
			break;
		case Critical:
			_output = "!! " + _output;
			logEventColor(_output, Qt::red);
			break;
		default:
			logEventColor(_output, Qt::cyan);
			break;
	};
	
}


void LoggingSystem::logEventColor(QString& s, QColor color)
{
	QString _output;
	
	_locationMutex.lock();
	
	_output = "";	
	
	_output +=  "<font color=" + color.name() +"> " + s + "<\font><br>";
	
	

	if (_location != NULL) {
		_location->textCursor().insertHtml(_output); 
		_location->ensureCursorVisible();
	}
	
	_locationMutex.unlock();

}

