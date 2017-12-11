#include <vector>


class Polygon {
public:
	 
	std::vector< std::vector< int > >* points;

	Polygon();


	Polygon(std::vector< std::vector<int> >*  points);

	void setPoints(std::vector< std::vector<int> >*  points);

	void paint(int color, bool fill, bool drawStartLine, bool drawBorder);


	void paintPrimitives(int color, bool drawStartLine,bool drawBorder);

};
