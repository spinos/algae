/*
 *  pluginMain.cpp
 *  
 *
 *  Created by zhang on 08-3-31.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#include <maya/MFnPlugin.h>
#include "ExportACacheCmd.h"
#include "ACacheMeshViz.h"
#include "AttribACacheExport.h"
#include "AttribACacheRender.h"
#include "importSceneCmd.h"
#include "cacheTransform.h"
#include "ContextACache.h"
#include "DescACacheContext.h"
#include "EnsembleAShader.h"
#include "AdaptEnsembleAShader.h"
#include "PieceAShader.h"
#include "CheckAShaderCmd.h"
#include "VariableAShader.h"
#include "AdaptVariableAShader.h"
#include "ParseACacheCmd.h"
#include "TranslateAShaderCmd.h"
#include "ParsePCacheCmd.h"
 
MStatus initializePlugin( MObject obj ) 
{
MStatus status;
MFnPlugin plugin( obj, "ZHANG JIAN", "build 4.3.1 Thu April 29 2010", "Any" );


/*
	
*/
		status = plugin.registerCommand( "exportSceneCache", ExportACache::creator, ExportACache::newSyntax);
		if (!status) {
			status.perror("registerCommand");
			return status;
		}
		
		status = plugin.registerNode( "aCacheExportAttrib", AttribACacheExportNode::id, AttribACacheExportNode::creator,
								  AttribACacheExportNode::initialize );
	if (!status) {
		status.perror("registerNode");
		return status;
	}
	
	status = plugin.registerNode( "aCacheRenderAttrib", AttribACacheRenderNode::id, AttribACacheRenderNode::creator,
								  AttribACacheRenderNode::initialize );
	if (!status) {
		status.perror("registerNode");
		return status;
	}
	
	status = plugin.registerData("aCacheContextDesc", DescACacheContext::id,
								 DescACacheContext::creator);
	if (!status) {
		status.perror("registering ctx Desc data.");
		return status;
	}
	
	status = plugin.registerNode( "aCacheContext", ContextACache::id, 
						 &ContextACache::creator, &ContextACache::initialize,
						 MPxNode::kLocatorNode );
	if (!status) {
		status.perror("registerNode");
		return status;
	}
		
	status = plugin.registerNode( "aCacheMesh", ACacheMeshViz::id, 
						 &ACacheMeshViz::creator, &ACacheMeshViz::initialize,
						 MPxNode::kLocatorNode );
	if (!status) {
		status.perror("registerNode");
		return status;
	}
	
	status = plugin.registerCommand( "importSceneCache", XMLSceneCmd::creator, XMLSceneCmd::newSyntax);
		if (!status) {
			status.perror("registerCommand");
			return status;
		}
		
	status = plugin.registerTransform(	"cacheTransform", 
										rockingTransformNode::id, 
						 				&rockingTransformNode::creator, 
										&rockingTransformNode::initialize,
						 				&rockingTransformMatrix::creator,
										rockingTransformMatrix::id);
	if (!status) {
		status.perror("registerNode");
		return status;
	}
	
	status = plugin.registerNode( "aShaderEnsemble", EnsembleAShaderNode::id, EnsembleAShaderNode::creator,
								  EnsembleAShaderNode::initialize );
	if (!status) {
		status.perror("registerNode");
		return status;
	}
	
		status = plugin.registerNode( "aShaderAdaptEnsemble", AdaptEnsembleAShaderNode::id, AdaptEnsembleAShaderNode::creator,
								  AdaptEnsembleAShaderNode::initialize );
	if (!status) {
		status.perror("registerNode");
		return status;
	}
	
	status = plugin.registerNode( "aShaderVariable", VariableAShaderNode::id, VariableAShaderNode::creator,
								  VariableAShaderNode::initialize );
	if (!status) {
		status.perror("registerNode");
		return status;
	}
	
	status = plugin.registerNode( "aShaderAdaptVariable", AdaptVariableAShaderNode::id, AdaptVariableAShaderNode::creator,
								  AdaptVariableAShaderNode::initialize );
	if (!status) {
		status.perror("registerNode");
		return status;
	}
	
	status = plugin.registerNode( "aShaderPiece", PieceAShaderNode::id, PieceAShaderNode::creator,
								  PieceAShaderNode::initialize );
	if (!status) {
		status.perror("registerNode");
		return status;
	}
	
	status = plugin.registerCommand( "aShaderDiagnose", CheckAShader::creator, CheckAShader::newSyntax);
		if (!status) {
			status.perror("registerCommand");
			return status;
	}
	
	status = plugin.registerCommand( "aCacheMeshCacheGeo", ParseACache::creator, ParseACache::newSyntax);
		if (!status) {
			status.perror("registerCommand");
			return status;
		}
		
	status = plugin.registerCommand( "particleVizCacheGeo", ParsePCache::creator, ParsePCache::newSyntax);
		if (!status) {
			status.perror("registerCommand");
			return status;
		}
		
	status = plugin.registerCommand( "translateAShader", TranslateAShader::creator, TranslateAShader::newSyntax);
		if (!status) {
			status.perror("registerCommand");
			return status;
		}
	
	MGlobal::executeCommand ( "source aCacheMenus.mel;anemoneMakeMenus;" );

 return MS::kSuccess;
 }
 
 MStatus uninitializePlugin( MObject obj ) 
 {
MStatus status;
MFnPlugin plugin( obj );

		status = plugin.deregisterCommand( "exportSceneCache" );
		if (!status) {
		      status.perror("deregisterCommand");
		      return status;
		}
		
		status = plugin.deregisterNode( AttribACacheExportNode::id );
	if (!status) {
		status.perror("deregisterNode");
		return status;
	}
	
	status = plugin.deregisterNode( AttribACacheRenderNode::id );
	if (!status) {
		status.perror("deregisterNode");
		return status;
	}
	
	status = plugin.deregisterData(DescACacheContext::id);
	if (!status) {
		status.perror("deregistering ctx desc data.");
		return status;
	}
	
	status = plugin.deregisterNode( ContextACache::id );
	if (!status) {
		      status.perror("deregisterNode");
		      return status;
		}

	status = plugin.deregisterNode( ACacheMeshViz::id );
	if (!status) {
		      status.perror("deregisterNode");
		      return status;
		}

	status = plugin.deregisterCommand( "importSceneCache" );
		if (!status) {
		      status.perror("deregisterCommand");
		      return status;
		}
	
	status = plugin.deregisterNode( rockingTransformNode::id );
	if (!status) {
		status.perror("deregisterNode");
		return status;
	}
	
	status = plugin.deregisterNode( EnsembleAShaderNode::id );
	if (!status) {
		status.perror("deregisterNode");
		return status;
	}
	
	status = plugin.deregisterNode( AdaptEnsembleAShaderNode::id );
	if (!status) {
		status.perror("deregisterNode");
		return status;
	}
	
	status = plugin.deregisterNode( VariableAShaderNode::id );
	if (!status) {
		status.perror("deregisterNode");
		return status;
	}
	
	status = plugin.deregisterNode( AdaptVariableAShaderNode::id );
	if (!status) {
		status.perror("deregisterNode");
		return status;
	}
	
		status = plugin.deregisterNode( PieceAShaderNode::id );
	if (!status) {
		status.perror("deregisterNode");
		return status;
	}
	
	status = plugin.deregisterCommand( "aShaderDiagnose" );
		if (!status) {
		      status.perror("deregisterCommand");
		      return status;
		}
	
	status = plugin.deregisterCommand( "aCacheMeshCacheGeo" );
		if (!status) {
		      status.perror("deregisterCommand");
		      return status;
		}
		
	status = plugin.deregisterCommand( "particleVizCacheGeo" );
		if (!status) {
		      status.perror("deregisterCommand");
		      return status;
		}
		
	status = plugin.deregisterCommand( "translateAShader" );
		if (!status) {
		      status.perror("deregisterCommand");
		      return status;
		}
		
	MGlobal::executeCommand ( "anemoneRemoveMenus;" );

 return MS::kSuccess;
 }


