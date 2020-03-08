#include "provided.h"
#include <string>
#include <vector>
#include <functional>
#include <iostream>
#include <fstream>
#include "ExpandableHashMap.h"

using namespace std;

unsigned int hasher(const GeoCoord& g)
{
    return hash<string>()(g.latitudeText + g.longitudeText);
}

class StreetMapImpl
{
public:
    StreetMapImpl();
    ~StreetMapImpl();
    bool load(string mapFile);
    bool getSegmentsThatStartWith(const GeoCoord& gc, vector<StreetSegment>& segs) const;
private:
	ExpandableHashMap<GeoCoord, vector<StreetSegment>> segments;
};

StreetMapImpl::StreetMapImpl()
{
}

StreetMapImpl::~StreetMapImpl()
{
}

bool StreetMapImpl::load(string mapFile)
{
	ifstream parser(mapFile);
	if (!parser)
		return false;
	string s;
	while (getline(parser, s)) {
		string streetName = s;
		int numSegments;
		parser >> numSegments;
		parser.ignore(10000, '\n');
		for (int i = 0; i < numSegments; i++) {
			string startLat, startLon, endLat, endLon;
			parser >> startLat;
			parser >> startLon;
			parser >> endLat;
			parser >> endLon;
			GeoCoord startCoord(startLat, startLon);
			GeoCoord endCoord(endLat, endLon);
			parser.ignore(10000, '\n');

			StreetSegment forwards(startCoord, endCoord, streetName);
			StreetSegment backwards(endCoord, startCoord, streetName);
			vector<StreetSegment> segs;
			segments.associate(endCoord, segs);
		}

	}
	cerr << "size of this chungus map: " << segments.size() << endl;
	return true;  
}

bool StreetMapImpl::getSegmentsThatStartWith(const GeoCoord& gc, vector<StreetSegment>& segs) const
{
	if (segments.find(gc)) {
		segs = *(segments.find(gc));
		return true;
	}

	return false;  // Delete this line and implement this function correctly
}

//******************** StreetMap functions ************************************

// These functions simply delegate to StreetMapImpl's functions.
// You probably don't want to change any of this code.

StreetMap::StreetMap()
{
    m_impl = new StreetMapImpl;
}

StreetMap::~StreetMap()
{
    delete m_impl;
}

bool StreetMap::load(string mapFile)
{
    return m_impl->load(mapFile);
}

bool StreetMap::getSegmentsThatStartWith(const GeoCoord& gc, vector<StreetSegment>& segs) const
{
   return m_impl->getSegmentsThatStartWith(gc, segs);
}
