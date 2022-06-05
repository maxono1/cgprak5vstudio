//
//  LineBoxModel.cpp
//  CGXcode
//
//  Created by Philipp Lensing on 10.10.16.
//  Copyright © 2016 Philipp Lensing. All rights reserved.
//

#include "LineBoxModel.h"

LineBoxModel::LineBoxModel(float Width, float Height, float Depth)
{
	VB.begin(); //alles clearen und blank slate
	// TODO: Add your code (Exercise 1)   

	float start = -Height/2;

	//GROUND LEVEL
	VB.addVertex(start, start, start);
	VB.addVertex(Width + start, start, start);

	VB.addVertex(Width + start, start, start);
	VB.addVertex(Width + start, start, Depth + start);

	VB.addVertex(Width + start, start, Depth + start);
	VB.addVertex(start, start, Depth + start);

	VB.addVertex(start, start, Depth + start);
	VB.addVertex(start, start, start);


	//Vertical lines
	VB.addVertex(start, start, start);
	VB.addVertex(start, Height + start, start);

	VB.addVertex(Width + start, start, start);
	VB.addVertex(Width + start, Height + start, start);

	VB.addVertex(Width + start, start, Depth + start);
	VB.addVertex(Width + start, Height + start, Depth + start);

	VB.addVertex(start, start, Depth + start);
	VB.addVertex(start, Height + start, Depth + start);

	//Top layer
	VB.addVertex(start, Height + start, start);
	VB.addVertex(Width + start, Height + start, start);

	VB.addVertex(Width + start, Height + start, start);
	VB.addVertex(Width + start, Height + start, Depth + start);

	VB.addVertex(Width + start, Height + start, Depth + start);
	VB.addVertex(start, Height + start, Depth + start);

	VB.addVertex(start, Height + start, Depth + start);
	VB.addVertex(start, Height + start, start);
	//VB.addVertex(-Width, Height, Depth);


	VB.end();
}

void LineBoxModel::draw(const BaseCamera& Cam)
{
	// TODO: Add your code (Exercise 1)

	BaseModel::draw(Cam);
	VB.activate();

	glDrawArrays(GL_LINES, 0, VB.vertexCount());

	VB.deactivate();
}
