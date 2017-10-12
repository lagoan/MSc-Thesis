

#include <QDir>
#include <QFileInfo>
#include <QVector>

#include "Spatula.h"

Spatula2::Spatula2() 
{
}


Spatula2::~Spatula2() 
{
	 
}




void Spatula2::updateLog(QString logEntry)
{
}

void Spatula2::setFileProgress(int progress)
{
}

void Spatula2::setActionProgress(int progress)
{
}


void Spatula2::processWorkingDirectory() 
{
}

void Spatula2::processWorkingDirectory(QString directory, 
									  ProcessingThread::Type type) 
{
	
	
	
}

void Spatula2::run() {
	
}

void Spatula2::processFile(QString directory, 
						  QString fileName,
						  ProcessingThread::Type type)
{
	QDir workingDirectory(directory);
	
	QStringList filters;
	filters << fileName;
	
	workingDirectory.setNameFilters(filters);
	
	QFileInfoList fileList = workingDirectory.entryInfoList();
	
	
	__processingThread.setDirectory(workingDirectory);
	__processingThread.setFileList( fileList );
	__processingThread.setType(type);
	__processingThread.run();
}

void Spatula2::setWorkingDirectory()
{
	
	
}

QString Spatula2::getDirectoryPath()
{
		
}

void Spatula2::updateProgress(int progress)
{
}

