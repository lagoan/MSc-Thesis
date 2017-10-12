/*
 *  ProcessingThread.h
 *  Spatula
 *
 *  Created by Omar Rodriguez-Arenas on 11/01/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef PROCESSINGTHREAD_H
#define PROCESSINGTHREAD_H

#include <QThread>
#include <QString>
#include <QFileInfo>
#include <QDir>
#include <QTime>

#include "SurfaceMesher.h"
#include "ExporterXYZ.h"
#include "ColladaReconstructor.h"
#include "meshlab/meshmodel.h"

class ProcessingThread : public QThread
{
	Q_OBJECT
	
public:
	
	enum Type {
		daeFiles,
		xyzFiles
	};
	
	ProcessingThread();
	ProcessingThread(QFileInfoList &fileList);
	~ProcessingThread();
	
	void setFileList(QFileInfoList &fileList);
	void setDirectory(QDir &dir);
	void setType(Type type);
	
public slots:
	void taskProgress(int);
	
protected:
	void run();

signals:
	void updateLog(QString);
	void finishedFile(int);
	void fileProgress(int);
	void actionProgress(int);
	
private:
	QDir __directory;
	QFileInfoList  __fileList;
	MeshDocument __meshDocument; 
	SurfaceMesher __surfaceMesher;
	ColladaReconstructor __reconstructor;
	ExporterXYZ __exporterXYZ;
	int __totalProgress;
	int __fileProgress;
	Type __type;
	
	QString getWorkingPath(QString daeFileName);
	void processDAE(ProcessedScene &scene,
					QFileInfoList::iterator fileIterator,
					QTime &proxyTime,
					QTime &fileTime,
					DAE &daeObject);

	void processXYZ(ProcessedScene &scene,
				    QFileInfoList::iterator fileIterator,
					QTime &proxyTime,
					QTime &fileTime);
};



#endif