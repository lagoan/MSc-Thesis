

#ifndef SPATULA_H
#define SPATULA_H

//#include "ui_spatula.h"

//#include <QtGui>
#include <QString>

#include "ProcessingThread.h"
#include <QString>
#include <QFileInfo>
#include <QDir>
#include <QTime>

#include "SurfaceMesher.h"
#include "ColladaReconstructor.h"
#include "meshlab/meshmodel.h"

class Spatula2 : public QObject
{
    Q_OBJECT
	
public:
 
	
	Spatula2(); 
	~Spatula2(); 
	
	//void setUpUI();
	QString getDirectoryPath();
	void processWorkingDirectory(QString, ProcessingThread::Type);
	void processFile(QString directory, 
				     QString fileName,
					 ProcessingThread::Type type);
	void processFile(QString);
	void run();

	
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
