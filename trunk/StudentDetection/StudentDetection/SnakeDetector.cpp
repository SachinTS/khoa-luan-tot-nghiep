#include "stdafx.h"
#include "SnakeDetector.h"

SnakeDetector::SnakeDetector(void)
{
}

SnakeDetector::SnakeDetector(const char *config) {
	TiXmlDocument doc(config);
	if (!doc.LoadFile()) return;

	TiXmlHandle hDoc(&doc);
	TiXmlElement* pElem;
	TiXmlHandle hRoot(0);

	// block: name
	{
		pElem = hDoc.FirstChildElement().Element();
		// should always have a valid root but handle gracefully if it does
		if (!pElem) return;
		// save this for later
		hRoot = TiXmlHandle(pElem);
	}

	// block: string table
	{
		pElem = hRoot.FirstChild( "threshold" ).Element();
		this->threshold = atof(pElem->GetText());
		pElem = hRoot.FirstChild( "delta" ).Element();
		this->delta = atoi(pElem->GetText());
		pElem = hRoot.FirstChild( "length" ).Element();
		this->l = atoi(pElem->GetText());
		pElem = hRoot.FirstChild( "Big" ).Element();
		bigw = new SnakeWindow(pElem->GetText(), threshold, delta, l);
		pElem = hRoot.FirstChild( "Medium" ).Element();
		mediumw = new SnakeWindow(pElem->GetText(), threshold, delta, l);
		pElem = hRoot.FirstChild( "Small" ).Element();
		smallw = new SnakeWindow(pElem->GetText(), threshold, delta, l);
	}
}

SnakeDetector::~SnakeDetector(void)
{
	if (bigw != NULL)
		delete bigw;
	if (mediumw != NULL)
		delete mediumw;
	if (smallw != NULL)
		delete smallw;
}

Snake *SnakeDetector::GetSnake(IplImage *gray, int distance, CvPoint location, CvRect rect) {
	switch (distance) {
		case 1:
			return bigw->GetSnake(gray, location, rect);
		case 2:
			return mediumw->GetSnake(gray, location, rect);
		case 3:
			return smallw->GetSnake(gray, location, rect);
	}
}