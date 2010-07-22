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
		big = new SnakeWindow(pElem->GetText(), threshold, delta, l);
		pElem = hRoot.FirstChild( "Medium" ).Element();
		medium = new SnakeWindow(pElem->GetText(), threshold, delta, l);
		pElem = hRoot.FirstChild( "Small" ).Element();
		small = new SnakeWindow(pElem->GetText(), threshold, delta, l);
	}
}

SnakeDetector::~SnakeDetector(void)
{
	if (big != 0)
		delete big;
	if (medium != 0)
		delete medium;
	if (small != 0)
		delete small;
}
