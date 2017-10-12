/*
 *  ProcessingThread.cpp
 *  Spatula
 *
 *  Created by Omar Rodriguez-Arenas on 11/01/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */


#include <dae.h>
#include <dom/domCOLLADA.h>
#include <dom/domProfile_COMMON.h>
#include <dom/domConstants.h>

#include <QTextStream>

#include <iostream>

#include "ProcessingThread.h"
#include "ProcessedScene.h"
#include "integrationHelper.h"

ProcessingThread::ProcessingThread()
{
	connect( & __surfaceMesher, SIGNAL( taskProgress(int) ), this, SLOT( taskProgress(int) ) );
	connect( & __exporterXYZ, SIGNAL( taskProgress(int) ), this, SLOT( taskProgress(int) ) );
	__meshDocument.addNewMesh("canvas");
}

ProcessingThread::ProcessingThread(QFileInfoList &fileList)
{
	setFileList(fileList);
}


ProcessingThread::~ProcessingThread()
{

}

void ProcessingThread::setFileList(QFileInfoList &fileList)
{
	__fileList = fileList;
	
}

void ProcessingThread::run()
{
	ProcessedScene scene;
	domVisual_scene* domScene;
	
	QFileInfoList::iterator fileIterator;
	
	domCOLLADA* root;
	__totalProgress = 0;
	__fileProgress = 0;
	
	QTime totalTime;
	QTime fileTime;
	QTime proxyTime;
	
	totalTime.start();
	fileTime.start();
	proxyTime.setHMS(0, 0, 0);

	QString dirName = __directory.dirName() + QString("Processed");
	__directory.cdUp();
	__directory.mkdir( dirName );
	__directory.cd( dirName );

	
	
	for (fileIterator = __fileList.begin(); fileIterator != __fileList.end(); fileIterator++) {
		if ( fileIterator->isFile() ) {
			
			fileTime.restart();
			emit ( fileProgress( 0 ) );
			emit ( actionProgress( 0 )  );
			emit ( updateLog( fileIterator->fileName() ) );
			emit ( updateLog( "Reading" ) );
			
			emit ( actionProgress( 50 )  );
			DAE daeObject;
			root = daeObject.open( fileIterator->absoluteFilePath().toStdString() );
			emit ( actionProgress( 100 )  );
			
			emit ( updateLog( "Importing" ) );
			emit ( actionProgress( 0 )  );

			if (root != NULL) {
			
				domScene = daeSafeCast<domVisual_scene>(root->getDescendant("visual_scene"));
			}
			else {
				emit ( updateLog( "Something wrong with file root \n" ) );
				break;
			}
			

			emit ( actionProgress( 50 )  );
			if (domScene != NULL) {
				scene = lookup<ProcessedScene, domVisual_scene>(*domScene);

			}
			else {
				emit ( updateLog( "Something wrong with file scene\n" ) );
				break;
			}

			scene.fileName = fileIterator->absoluteFilePath();

			emit ( actionProgress( 100 )  );
			emit ( fileProgress( 40 ) );
			#if 1
		
			switch( __type ) {
			// generate mesh
				case ProcessingThread::daeFiles :
					// closign daeObject inside
					processDAE(scene, fileIterator, proxyTime, fileTime, daeObject);
					break;
				case ProcessingThread::xyzFiles :
					processXYZ(scene, fileIterator, proxyTime, fileTime);
					break;
			} 	
			#endif
			freeConversionObjects<ProcessedNode, domNode>(daeObject);	
			freeConversionObjects<ProcessedScene, domVisual_scene>(daeObject);	
		}
	}
	emit ( updateLog( QString("Total time elapsed %1").arg( (proxyTime.addMSecs(totalTime.elapsed()) ).toString("hh:mm:ss.zzz")  ) ) );
	
	return;
}

void ProcessingThread::processXYZ(ProcessedScene &scene,
								  QFileInfoList::iterator fileIterator,
								  QTime &proxyTime,
								  QTime &fileTime)
{
	QString result = __exporterXYZ.getData(scene);
	
	//QString saveLocation = __directory.absolutePath();
	//saveLocation += QString("/P_") + fileIterator->fileName();
	
	
	QFile file;
	QDir::setCurrent(__directory.absolutePath());
	file.setFileName(fileIterator->fileName() + QString(".xyz") );

	file.open(QIODevice::WriteOnly);
	QTextStream out(&file);
	out << result ;
	//file.write(result);
    	file.close();
	//std::cout << result.toStdString() << "\n";
	
}



void ProcessingThread::processDAE(ProcessedScene &scene,
								  QFileInfoList::iterator fileIterator,
								  QTime &proxyTime,
								  QTime &fileTime,
								  DAE &daeObject)
{
	
	emit ( updateLog( "Meshing" ) );
	__surfaceMesher.getMesh( scene, __meshDocument );
	emit ( fileProgress( 60 ) );			
	
	// color mesh
	
	
	// save mesh in collada 
	#if 1
	QString saveLocation = __directory.absolutePath();
	saveLocation += QString("/P_") + fileIterator->fileName();
	
	
	DAE newDae;
	
	newDae.add(saveLocation.toStdString().c_str());
	
	// reconstruct will create a new dae file from scratch
	
	scene.fileName = saveLocation;
	
	__meshDocument.busy = true;
	__reconstructor.reconstruct(scene, newDae, __meshDocument);
	__meshDocument.busy = false;
	// aqui fue el pasado
	// save new collada file
	
	
	//saveLocation += QString("/P") + scene.fileName;
	
	//newDae.saveAs( saveLocation.toStdString().c_str(), scene.fileName.toStdString().c_str() );
	newDae.writeAll();
	
	daeObject.close( fileIterator->absoluteFilePath().toStdString() );
	
	
	__reconstructor.releaseMemory();
	
	
	emit ( updateLog( QString("Fileprocessed in %1").arg( (proxyTime.addMSecs(fileTime.elapsed()) ).toString("hh:mm:ss.zzz") ) ) );
	emit ( fileProgress( 100 ) );
	emit ( finishedFile( ++__totalProgress ) );
	#endif
}

void ProcessingThread::taskProgress(int progress)
{
	
	emit( actionProgress( progress )  );
	
}

void ProcessingThread::setDirectory(QDir &dir)
{
	__directory = dir;
}

QString ProcessingThread::getWorkingPath(QString daeFileName)
{
	QString fileName = daeFileName.section('/', -1);
	QString path = daeFileName;
	path.remove( fileName );
	path += "/";
	
	QString identificator = fileName;
	identificator.remove(".dae");
	path += identificator + QString("_animation") + QString("/"); 
	
	return path;
	
}

void ProcessingThread::setType(Type type)
{
	
	__type = type;
}


