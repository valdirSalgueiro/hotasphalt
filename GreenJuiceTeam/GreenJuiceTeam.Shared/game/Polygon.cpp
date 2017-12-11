#include "Polygon.h"
#include "Block.h"
#include "Line.h"
#include "Quad.h"
#include "core/gl2d.h"

Polygon::Polygon() {
}


Polygon::Polygon(std::vector< std::vector<int> >* points_){
	setPoints(points_);
}

void Polygon::setPoints(std::vector< std::vector<int> >*  points_){
	//points.clear();
	//for(int i = 0; i < points_->size(); i++){
	//	points.push_back(points_[i]);
	//}
	points=points_;
}

void Polygon::paint(int color, bool fill, bool drawStartLine, bool drawBorder){
	paintPrimitives(color, drawStartLine, drawBorder);
}


void Polygon::paintPrimitives(int color, bool drawStartLine,	bool drawBorder) {
	float r,g,b,a;
	switch(color){
	case Block::COLOR_1:
		r=4.0f/255.0f;
		g=9.0f/255.0f;
		b=9.0f/255.0f;
		a=1.0f;
		//glColor4f(17.0f/255.0f,34.0f/255.0f,34.0f/255.0f, 1.0f);
		break;
	case Block::COLOR_2:
		r=9.0f/255.0f;
		g=12.0f/255.0f;
		b=17.0f/255.0f;
		a=1.0f;
		//glColor4f(34.0f/255.0f,51.0f/255.0f,68.0f/255.0f, 1.0f);
		break;
	case Block::LINE_COLOR:
		r=64.0f/255.0f;
		g=52.0f/255.0f;
		b=17.0f/255.0f;
		a=1.0f;
		//glColor4f(255.0f/255.0f,206.0f/255.0f,66.0f/255.0f, 1.0f);
		break;
	case Block::BORDER_COLOR:
		r=255.0f/255.0f;
		g=247.0f/255.0f;
		b=231.0f/255.0f;
		a=1.0f;
		break;
	case Block::BG_COLOR_1:
		r=107.0f/255.0f;
		g=99.0f/255.0f;
		b=82.0f/255.0f;
		a=1.0f;
		break;
	case Block::BG_COLOR_2:
		r=123.0f/255.0f;
		g=123.0f/255.0f;
		b=82.0f/255.0f;
		a=1.0f;
		break;
	}

	std::vector<int> topLeft = (*points)[0];
	std::vector<int> bottomLeft = (*points)[2];

	std::vector<int> topRight = (*points)[1];
	std::vector<int> bottomRight = (*points)[3];

	//spriteBatchAddQuad(topLeft[0], topLeft[1], topRight[0], topRight[1], bottomLeft[0],bottomLeft[1],bottomRight[0],bottomRight[1],r,g,b,a,0);


	if(drawStartLine){
		//spriteBatchAddQuad(topLeft[0], topLeft[1], topRight[0], topLeft[1] , topLeft[0]   ,   topLeft[1]+Block::BLOCK_LENGTH/4,  topRight[0]    ,topRight[1]+Block::BLOCK_LENGTH/4,255.0f/255.0f,206.0f/255.0f,66.0f/255.0f,1.0f,0);
	}



	if(drawBorder){
		//Line::draw(topLeft[0], topLeft[1], bottomLeft[0], bottomLeft[1]);
		//Line::draw(topRight[0], topRight[1], bottomRight[0], bottomRight[1]);
	}

}
