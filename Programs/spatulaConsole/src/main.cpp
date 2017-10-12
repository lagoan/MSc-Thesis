/*
 *  main.cpp
 *  Spatula
 *
 *  Created by Omar Rodriguez-Arenas on 10/01/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include <iostream>


#include "Spatula.h"
#include <QApplication>


#include "Spatula.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    //Spatula *window = new Spatula;
	//window->show();	
	
    //return app.exec();

	Spatula2 spatula;

	spatula.processFile(QString(argv[1]),
 						QString(argv[2]), 
 						ProcessingThread::daeFiles);

	std::cout << " i ran\n";

}