

#ifndef SPATULA_H
#define SPATULA_H

#include "ui_spatula.h"

#include <QtGui>
#include <QString>

#include "ProcessingThread.h"


class Spatula : public QMainWindow, private Ui::Spatula
{
    Q_OBJECT
	
public:
    
	Spatula(); 
	~Spatula(); 
	
	void setUpUI();
	QString getDirectoryPath();
	void processWorkingDirectory(QString, ProcessingThread::Type);
	void processFile(QString);
	
public slots:
	
	void setWorkingDirectory();
	void processWorkingDirectory();
	void updateProgress(int);
	void setFileProgress(int);
	void setActionProgress(int);
	void updateLog(QString);

private:
	ProcessingThread __processingThread;
	
	
};


#endif
