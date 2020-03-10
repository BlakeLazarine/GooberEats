#include "provided.h"
#include <vector>
#include <string>
#include <cmath>
using namespace std;

class DeliveryPlannerImpl
{
public:
    DeliveryPlannerImpl(const StreetMap* sm);
    ~DeliveryPlannerImpl();
    DeliveryResult generateDeliveryPlan(
        const GeoCoord& depot,
        const vector<DeliveryRequest>& deliveries,
        vector<DeliveryCommand>& commands,
        double& totalDistanceTravelled) const;
private:
	const StreetMap* m_map;
	void getStepsBtwnCoords(vector<DeliveryCommand>& commands, list<StreetSegment>& segments) const;
};

DeliveryPlannerImpl::DeliveryPlannerImpl(const StreetMap* sm)
{
	m_map = sm;
}

DeliveryPlannerImpl::~DeliveryPlannerImpl()
{
}


void DeliveryPlannerImpl::getStepsBtwnCoords(vector<DeliveryCommand>& commands, list<StreetSegment>& segments) const {
	list<StreetSegment>::iterator it = segments.begin();
	string prevName = "";
	double currentRunDist = 0;
	string currentDirection;
	StreetSegment* currentSeg = nullptr;
	while (it != segments.end()) {

		if (it->name != prevName) {

			double angle = angleOfLine(*it);

			if (currentSeg) {
				DeliveryCommand pro;
				pro.initAsProceedCommand(currentDirection, prevName, currentRunDist);
				commands.push_back(pro);


				double angIntersection = angleBetween2Lines(*currentSeg, *it);
				DeliveryCommand com;
				if (angIntersection >= 1 && angIntersection < 180) {
					com.initAsTurnCommand("left", it->name);
					commands.push_back(com);
				}

				else if (angIntersection >= 180 && angIntersection <= 359) {
					com.initAsTurnCommand("right", it->name);
					commands.push_back(com);
				}
			}

			angle += 180;
			if (angle > 360)
				angle -= 360;
			if (0 <= angle && angle < 22.5)
				currentDirection = "east";
			else if (angle < 67.5)
				currentDirection = "northeast";
			else if (angle < 112.5)
				currentDirection = "north";
			else if (angle < 157.5)
				currentDirection = "northwest";
			else if (angle < 202.5)
				currentDirection = "west";
			else if (angle < 247.5)
				currentDirection = "southwest";
			else if (angle < 292.5)
				currentDirection = "south";
			else if (angle < 337.5)
				currentDirection = "southeast";
			else
				currentDirection = "east";

			currentRunDist = 0;


		}
		currentRunDist += distanceEarthMiles(it->start, it->end);
		prevName = it->name;
		currentSeg = &(*it);
		it++;
	}
	DeliveryCommand pro;
	pro.initAsProceedCommand(currentDirection, prevName, currentRunDist);
	commands.push_back(pro);

}

DeliveryResult DeliveryPlannerImpl::generateDeliveryPlan(
	const GeoCoord& depot,
    const vector<DeliveryRequest>& deliveries,
    vector<DeliveryCommand>& commands,
    double& totalDistanceTravelled) const
{
	
	DeliveryOptimizer op(m_map);
	vector<DeliveryRequest> newDeliveries = deliveries;
	/*for (int i = 0; i < deliveries.size(); i++) {
		newDeliveries.push_back(deliveries[i]);
		cerr << i << " " << newDeliveries.size() << fendl;
	}*/
		
	
	double oldDist, newDist;
	
	op.optimizeDeliveryOrder(depot, newDeliveries, oldDist, newDist);
	totalDistanceTravelled = 0;
	
	GeoCoord start = depot;
	commands.clear();
	

	PointToPointRouter router(m_map);
	
	for (int i = 0; i < newDeliveries.size(); i++) {
		
		list<StreetSegment> segments;
		double dist;
		DeliveryResult res = router.generatePointToPointRoute(start, newDeliveries[i].location, segments, dist);
		
		if (res == NO_ROUTE)
			return NO_ROUTE;
		if (res == BAD_COORD)
			return BAD_COORD;
		totalDistanceTravelled += dist;
		
		getStepsBtwnCoords(commands, segments);

		DeliveryCommand del;
		del.initAsDeliverCommand(newDeliveries[i].item);
		commands.push_back(del);

		start = newDeliveries[i].location;
	}

	list<StreetSegment> segments;
	double dist;
	DeliveryResult res = router.generatePointToPointRoute(start, depot, segments, dist);
	if (res == NO_ROUTE)
		return NO_ROUTE;
	if (res == BAD_COORD)
		return BAD_COORD;
	totalDistanceTravelled += dist;
	getStepsBtwnCoords(commands, segments);

    return DELIVERY_SUCCESS;
}

//******************** DeliveryPlanner functions ******************************

// These functions simply delegate to DeliveryPlannerImpl's functions.
// You probably don't want to change any of this code.

DeliveryPlanner::DeliveryPlanner(const StreetMap* sm)
{
    m_impl = new DeliveryPlannerImpl(sm);
}

DeliveryPlanner::~DeliveryPlanner()
{
    delete m_impl;
}

DeliveryResult DeliveryPlanner::generateDeliveryPlan(
    const GeoCoord& depot,
    const vector<DeliveryRequest>& deliveries,
    vector<DeliveryCommand>& commands,
    double& totalDistanceTravelled) const
{
    return m_impl->generateDeliveryPlan(depot, deliveries, commands, totalDistanceTravelled);

}
