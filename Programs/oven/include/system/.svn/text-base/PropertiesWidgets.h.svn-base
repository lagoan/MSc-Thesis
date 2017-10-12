/*
 *  PropertiesWidgets.h
 *  oven
 *
 *  Created by Omar Rodriguez-Arenas on 19/09/07.
 *  Copyright 2007 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef	PROPERTIESWIDGETS_H
#define PROPERTIESWIDGETS_H

#include "SimulationProperties.h"

#include <QtGui>
#include <QItemDelegate>
#include <map>



class PropertiesDelegate : public QItemDelegate
{
	Q_OBJECT

public:
	
	PropertiesDelegate(QObject *parent = 0);
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &,
         const QModelIndex &index) const;
    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model,
         const QModelIndex &index) const;	

};


class PropertiesWidgets
{
public:

	void generate();
	void clear();

	std::map<ScenePropertyType, QTableWidgetItem*>	sceneValues;
	std::map<ScenePropertyType, QTableWidgetItem*>	sceneQTableWidgetItem;
	
	std::map<ObjectPropertyType, QTableWidgetItem*>	objectValues;
	std::map<ObjectPropertyType, QTableWidgetItem*>	objectQTableWidgetItem;

};

#endif