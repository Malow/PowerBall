#pragma once

/*
Wrapper-class for the graphics engine. Contains the interface that user use from the outside.
Singleton.
*/

#include "stdafx.h"
#include "Mesh.h"
#include "Image.h"

class GraphicsEngine
{
private:

public:
	GraphicsEngine();
	virtual ~GraphicsEngine();
	
	/*! Creates a Mesh and starts rendering it every frame. Return is a pointer to the Mesh created. To stop rendering it call DeleteMesh on the engine with the Mesh as parameter. */
	Mesh* CreateMesh();
	/*! Stops rendering the Mesh and internally deletes it and the pointer will become NULL. Return depends on if the Mesh was sucessfully removed. */
	bool DeleteMesh(Mesh* delMesh);

	/*! Create an Image and starts rendering it every frame. Return is a pointer to the image created. To stop rendering it call DeleteImage on the engine with the image as parameter. */
	Image* CreateImage();
	/*! Stops rendering the Image and internally deletes it and the pointer will become NULL. Return depends on if the Image was sucessfully removed. */
	bool DeleteImage(Image* delImage);
};