

#include <QDir>
#include <QFileInfo>
#include <QVector>

#include "Spatula.h"

Spatula::Spatula() 
{
	setUpUI();
}


Spatula::~Spatula() 
{
	 
	__processingThread.wait();
}


void Spatula::setUpUI()
{
	setupUi(this);  
	
	totalProgressBar->reset();
	fileProgressBar->reset();
	fileProgressBar->setRange(0, 100);
	actionProgressBar->reset();
	actionProgressBar->setRange(0, 100);
	
	connect( dirButton, SIGNAL( clicked() ), this, SLOT( setWorkingDirectory() ) );
	connect( goButton, SIGNAL( clicked() ), this, SLOT( processWorkingDirectory() ) );
	connect( xyzButton, SIGNAL( clicked() ), this, SLOT( processWorkingDirectory() ) );
	connect( & __processingThread, SIGNAL( updateLog(QString) ), this, SLOT( updateLog(QString) ) );
	connect( & __processingThread, SIGNAL( finishedFile(int) ), this, SLOT( updateProgress(int) ) );
	connect( & __processingThread, SIGNAL( fileProgress(int) ), this, SLOT( setFileProgress(int) ) );
	connect( & __processingThread, SIGNAL( actionProgress(int) ), this, SLOT( setActionProgress(int) ) );
	
}

void Spatula::updateLog(QString logEntry)
{
	progressPlainTextEdit->insertPlainText(logEntry + "\n");
	progressPlainTextEdit->ensureCursorVisible();
}

void Spatula::setFileProgress(int progress)
{
	fileProgressBar->setValue(progress);
}

void Spatula::setActionProgress(int progress)
{
	actionProgressBar->setValue(progress);
}


void Spatula::processWorkingDirectory() {
	totalProgressBar->reset();
	fileProgressBar->reset();
	progressPlainTextEdit->setPlainText("");
	
	QPushButton *button = (QPushButton*) QObject::sender();
	
	if (button == goButton) {
		processWorkingDirectory( dirLineEdit->text(), ProcessingThread::daeFiles );
	} else if (button == xyzButton) {
		processWorkingDirectory( dirLineEdit->text(), ProcessingThread::xyzFiles );
	}
}

void Spatula::processWorkingDirectory(QString directory, 
									  ProcessingThread::Type type) 
{
	
	QDir workingDirectory(directory);
	
	// get only dae files
	
	QStringList filters;
	filters << "*.dae";
	
	workingDirectory.setNameFilters(filters);
	
	QFileInfoList fileList = workingDirectory.entryInfoList();
	
	fileProgressBar->setValue(0);
	actionProgressBar->setValue(0);
	
	totalProgressBar->setRange(0, fileList.size());
	totalProgressBar->setValue(0);
		
	__processingThread.setDirectory(workingDirectory);
	__processingThread.setFileList( fileList );
	__processingThread.setType(type);
	__processingThread.start();
	
}

void Spatula::setWorkingDirectory()
{
	
	dirLineEdit->setText( getDirectoryPath() );
	
}

QString Spatula::getDirectoryPath()
{
	
	
	return QFileDialog::getExistingDirectory(this,
											tr("Choose a working directory"),
											QString(),
											QFileDialog::DontResolveSymlinks
											| QFileDialog::ShowDirsOnly);
	
     
	
}

void Spatula::updateProgress(int progress)
{
	totalProgressBar->setValue(progress);
	progressPlainTextEdit->insertPlainText("Done!\n");
	progressPlainTextEdit->ensureCursorVisible();
	
}

