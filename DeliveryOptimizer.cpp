#include "provided.h"
#include <vector>
#include <list>
using namespace std;

class DeliveryOptimizerImpl
{
public:
    DeliveryOptimizerImpl(const StreetMap* sm);
    ~DeliveryOptimizerImpl();
    void optimizeDeliveryOrder(
        const GeoCoord& depot,
        vector<DeliveryRequest>& deliveries,
        double& oldCrowDistance,
        double& newCrowDistance) const;
};

DeliveryOptimizerImpl::DeliveryOptimizerImpl(const StreetMap* sm)
{
}

DeliveryOptimizerImpl::~DeliveryOptimizerImpl()
{
}

void DeliveryOptimizerImpl::optimizeDeliveryOrder(
    const GeoCoord& depot,
    vector<DeliveryRequest>& deliveries,
    double& oldCrowDistance,
    double& newCrowDistance) const
{
	oldCrowDistance = 0;  // Delete these lines and implement this function correctly
	vector<DeliveryRequest*> toDeliver;

	int numDeliveries = deliveries.size();
	for (int i = 0; i < numDeliveries; i++) {
		oldCrowDistance += 2 * distanceEarthMiles(depot, deliveries[i].location);
		toDeliver.push_back(&deliveries[i]);
	}
	vector<DeliveryRequest> orderedDeliveries;
	GeoCoord lastLoc = depot;
	while (orderedDeliveries.size() < numDeliveries) {
		int minIdx = 0;
		for (int i = 0; i < toDeliver.size(); i++) {
			if (!toDeliver[minIdx] || (toDeliver[i] && distanceEarthMiles(lastLoc, toDeliver[i]->location) < 
				distanceEarthMiles(lastLoc, toDeliver[minIdx]->location))) 
				minIdx = i;
		}
		orderedDeliveries.push_back(*toDeliver[minIdx]);
		
		lastLoc = toDeliver[minIdx]->location;
		toDeliver[minIdx] = nullptr;
	}

	deliveries = orderedDeliveries;
	
	newCrowDistance = 0;
	lastLoc = depot;
	for (int i = 0; i < deliveries.size(); i++) {
		newCrowDistance += distanceEarthMiles(deliveries[i].location, lastLoc);
		lastLoc = deliveries[i].location;
	}
	newCrowDistance += distanceEarthMiles(depot, lastLoc);
}

//******************** DeliveryOptimizer functions ****************************

// These functions simply delegate to DeliveryOptimizerImpl's functions.
// You probably don't want to change any of this code.

DeliveryOptimizer::DeliveryOptimizer(const StreetMap* sm)
{
    m_impl = new DeliveryOptimizerImpl(sm);
}

DeliveryOptimizer::~DeliveryOptimizer()
{
    delete m_impl;
}

void DeliveryOptimizer::optimizeDeliveryOrder(
        const GeoCoord& depot,
        vector<DeliveryRequest>& deliveries,
        double& oldCrowDistance,
        double& newCrowDistance) const
{
    return m_impl->optimizeDeliveryOrder(depot, deliveries, oldCrowDistance, newCrowDistance);
}
