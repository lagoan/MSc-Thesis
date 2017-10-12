/****************************************************************************
* MeshLab                                                           o o     *
* A versatile mesh processing toolbox                             o     o   *
*                                                                _   O  _   *
* Copyright(C) 2005-2008                                           \/)\/    *
* Visual Computing Lab                                            /\/|      *
* ISTI - Italian National Research Council                           |      *
*                                                                    \      *
* All rights reserved.                                                      *
*                                                                           *
* This program is free software; you can redistribute it and/or modify      *   
* it under the terms of the GNU General Public License as published by      *
* the Free Software Foundation; either version 2 of the License, or         *
* (at your option) any later version.                                       *
*                                                                           *
* This program is distributed in the hope that it will be useful,           *
* but WITHOUT ANY WARRANTY; without even the implied warranty of            *
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the             *
* GNU General Public License (http://www.gnu.org/licenses/gpl.txt)          *
* for more details.                                                         *
*                                                                           *
****************************************************************************/

#ifndef MESHLAB_INTERFACES_H
#define MESHLAB_INTERFACES_H
#include <QtCore>
#include <QApplication>
#include <QAction>
#include <QMessageBox>
#include <QTabletEvent>
#include "filterparameter.h"



class QWidget;
class QGLWidget;
class QString;
class QVariant;
class QMouseEvent;
class MeshModel;
class RenderMode;
class GLArea;



class MeshFilterInterface
{
public:
  typedef int FilterIDType;
	
	// The FilterClass enum represents the set of keywords that must be used to categorize a filter.
	// Each filter can belong to one or more filtering class, or-ed togheter. 
	// The filter class 
	
	enum FilterClass 
	{ 
			Generic          =0x00000, // Should be avoided if possible
			Selection        =0x00001, // select or de-select something, basic operation on selections (like deleting)
			Cleaning         =0x00002, // Filters that can be used to clean meshes (duplicated vertices etc)
			Remeshing        =0x00004, // Simplification, Refinement, Reconstruction and mesh optimization
			FaceColoring     =0x00008, 
			VertexColoring   =0x00010, 
			MeshCreation     =0x00020, 
			Smoothing        =0x00040, // Stuff that does not change the topology, but just the vertex positions
			Quality          =0x00080, 
			Layer            =0x00100, // Layers, attributes
			Normal           =0x00200, // Normal, Curvature, orientation (rotations and transformations fall here)
		  Sampling         =0x00400,
		  Texture          =0x00800,
		  RangeMap         =0x01000, // filters specific for range map processing
      PointSet         =0x02000,
      Measure          =0x04000 // Filters that compute measures and information on meshes.
	};
	
	
	// The FilterPrecondition enum is used to build the prerequisite bitmask that each filter reports. 
	// This mask is used to explicitate what data a filter really needs in order to start. 
	// For example algorithms that compute per face quality have as precondition the existence of faces 
	// (but quality per face is not a precondition, because quality per face is created by these algorithms)
	// on the other hand an algorithm that deletes faces according to the stored quality has both FaceQuality
	// and Face as precondition.
	// These conditions do NOT include computed properties like borderFlags, manifoldness or watertightness. 
	// They are also used to grayout menus un-appliable entries.
	
	enum FilterPrecondition
	{
			FP_Generic          =0x00000, // Should be avoided if possible
			FP_Face             =0x00001, //  
			FP_VertexColor      =0x00002, //  
			FP_VertexQuality    =0x00004, //  
			FP_VertexRadius     =0x00008, //  
			FP_WedgeTexCoord    =0x00010, //
			FP_FaceColor        =0x00020 //  
	};

	virtual ~MeshFilterInterface() {}

	// The longer string describing each filtering action. 
	// This string is printed in the top of the parameter window 
	// so it should be at least one or two paragraphs long.
	// you can use simple html formatting tags (like <br> <b> and <i>) to improve readability.
	// This string is used in the About plugin dialog and by meshlabserver to create the filter list page.
	virtual const QString filterInfo(FilterIDType filter) const =0;
	
	// The very short string (a few words) describing each filtering action 
	// This string is used also to define the menu entry
	virtual const QString filterName(FilterIDType filter) const =0;
	
	// This function is called by the framework, for each action at the loading of the plugins.
	// it allows to add a list of global persistent parameters that can be changed from the meshlab itself.
	// If your plugins/action has no GlobalParameter, do nothing.
	virtual void initGlobalParameterSet(QAction *, RichParameterSet & /*globalparam*/) {}

	// The FilterClass describes in which generic class of filters it fits. 
	// This choice affect the submenu in which each filter will be placed 
	// For example filters that perform an action only on the selection will be placed in the Selection Class
	virtual const FilterClass getClass(QAction *) { return MeshFilterInterface::Generic; }
	
	// The filters can have some additional requirements on the mesh capabiliteis. 
	// For example if a filters requires Face-Face Adjacency you shoud re-implement 
	// this function making it returns MeshModel::MM_FACEFACETOPO. 
	// The framework will ensure that the mesh has the requirements satisfied before invoking the applyFilter function
	virtual const int getRequirements(QAction *){return MeshModel::MM_NONE;}
	
	// The FilterPrecondition mask is used to explicitate what a filter really needs to be applied. 
	// For example algorithms that compute per face quality have as precondition the existence of faces 
	// (but quality per face is not a precondition, because quality per face is created by these algorithms)
	// on the other hand an algorithm that deletes faces according to the stored quality has both FaceQuality
	// and Face as precondition.
	virtual int getPreConditions(QAction *) const {return FP_Generic;}


	// Function used by the framework to get info about the mesh properties changed by the filter.
	// It is widely used by the meshlab's preview system.
	//TO BE REPLACED WITH = 0
	virtual int postCondition( QAction* ) const {return MeshModel::MM_UNKNOWN;};

	// The main function that applies the selected filter with the already stabilished parameters
	// This function is called by the framework after getting the user params 
	// NO GUI interaction should be done here. No dialog asking, no messagebox errors. 
	// this function will also be called by the commandline framework.
	// If you want report errors, use the errorMsg() string. It will displayed in case of filters returning false.
	
	virtual bool applyFilter(QAction * /*filter*/, MeshModel &/*m*/, RichParameterSet & /* par */, vcg::CallBackPos * /*cb*/) = 0;
 	virtual bool applyFilter(QAction *   filter, MeshDocument &md,   RichParameterSet & par,       vcg::CallBackPos *cb) 
	{ 
		return applyFilter(filter,*(md.mm()),par,cb); 
	}

	// Function used by the framework to test if a filter is applicable to a mesh.
	// For istance a colorize by quality filter cannot be applied to a mesh without per-vertex-quality.
    // if return false it fills a list of strings describing the missing items.
	bool isFilterApplicable(QAction *act, const MeshModel& m, QStringList &MissingItems) const
	{	
		int preMask = getPreConditions(act);
		MissingItems.clear();
		
		if (preMask == MeshFilterInterface::FP_Generic) // no precondition specified. 
			return true;
		
		if (preMask & MeshFilterInterface::FP_VertexColor && !m.hasDataMask(MeshModel::MM_VERTCOLOR))
				MissingItems.push_back("Vertex Color");

		if (preMask & MeshFilterInterface::FP_FaceColor && !m.hasDataMask(MeshModel::MM_FACECOLOR))
				MissingItems.push_back("Face Color");
				
		if (preMask & MeshFilterInterface::FP_VertexQuality && !m.hasDataMask(MeshModel::MM_VERTQUALITY))
				MissingItems.push_back("Vertex Quality");

		if (preMask & MeshFilterInterface::FP_WedgeTexCoord && !m.hasDataMask(MeshModel::MM_WEDGTEXCOORD))
				MissingItems.push_back("Per Wedge Texture Coords");

		if (preMask & MeshFilterInterface::FP_VertexRadius && !m.hasDataMask(MeshModel::MM_VERTRADIUS))
				MissingItems.push_back("Vertex Radius");

		if (preMask & MeshFilterInterface::FP_Face && (m.cm.fn==0))
				MissingItems.push_back("Non empty Face Set");

		return MissingItems.isEmpty();
	}

	//  this function returns true if the filter has parameters that must be filled with an automatically build dialog.
	virtual bool autoDialog(QAction *) {return false;}
	
	// This function is called to initialized the list of parameters. 
	// it is called by the auto dialog framework to know the list of parameters.
	virtual void initParameterSet(QAction *,MeshModel &/*m*/, RichParameterSet & /*par*/) {}
	virtual void initParameterSet(QAction *filter,MeshDocument &md, RichParameterSet &par) 
	{initParameterSet(filter,*(md.mm()),par);}

	//  this function returns true if the filter has a personally customized dialog..
	virtual bool customDialog(QAction *) {return false;}
#if 0
	// This function is invoked for filters with a custom dialog of a filter and when has the params 
	// it notify it to the mainwindow with the collected parameters
	
	virtual bool getCustomParameters(QAction *action, QWidget * /*parent*/, MeshModel &/*m*/, RichParameterSet & params, MainWindowInterface *mw) 
	{
		assert(mw);
		mw->executeFilter(action, params, false);
		return true;
	}	

  /// Standard stuff that usually should not be redefined. 
	void setLog(GLLogStream *log) { this->log = log ; }
#endif
	/// This function is invoked by the framework when the apply filter fails to give some info to the user about the fiter failure
	/// Filters should avoid using QMessageBox for reporting errors. 
	/// Failing filters should put some meaningful information inside the errorMessage string.
	const QString &errorMsg() {return this->errorMessage;}

    virtual const FilterIDType ID(QAction *a) const
  	{
      foreach( FilterIDType tt, types())
        if( a->text() == this->filterName(tt) ) return tt;
			
			
			qDebug("unable to find the id corresponding to action  '%s'",qPrintable(a->text()));
			assert(0);
      return -1;
    }
		
    virtual QAction *AC(FilterIDType filterID)
  	{
			QString idName=this->filterName(filterID);
      foreach( QAction *tt, actionList)
        if( idName  == tt->text() ) return tt;
			
			qDebug("unable to find the action corresponding to action  '%i'",filterID);
			assert(0);
      return 0;
    }
	 
	 virtual const QString filterInfo(QAction *a){return this->filterInfo(ID(a));};
	 virtual const QString filterName(QAction *a) const {return this->filterName(ID(a));};
	 
    virtual QList<QAction *> actions() const { return actionList;}
	  virtual const QList<FilterIDType> &types() const { return typeList;}

protected:
    // Each plugins exposes a set of filtering possibilities. 
		// Each filtering procedure corresponds to a single QAction with a corresponding FilterIDType id. 
		// 
		
    // The list of actions exported by the plugin. Each actions strictly corresponds to 
		QList <QAction *> actionList;
    
		QList <FilterIDType> typeList;
    
		void Log(const char * f, ... ) 
		{
			/*
			if(log)
			{	
				char buf[4096];
				va_list marker;
				va_start( marker, f );     
				vsprintf(buf,f,marker);
				va_end( marker );              
				log->Log(GLLogStream::FILTER,buf);
			}
			 */
		}

		void Log(int Level, const char * f, ... ) 
		{
			/*
			 if(log)
			{	
				char buf[4096];
				va_list marker;
				va_start( marker, f );     
				vsprintf(buf,f,marker);
				va_end( marker );              
				log->Log(Level,buf);
			}
			 */
		}

   // GLLogStream *log;	
		
		// this string is used to pass back to the framework error messages in case of failure of a filter apply.
		QString errorMessage;
};


Q_DECLARE_INTERFACE(MeshFilterInterface,				"vcg.meshlab.MeshFilterInterface/1.0")


#endif
