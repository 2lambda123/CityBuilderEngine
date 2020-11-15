#include "StaticElementHandler.hpp"

#include <QtCore/QtAlgorithms>

#include "src/engine/element/static/building/FarmBuilding.hpp"
#include "src/engine/element/static/building/HouseBuilding.hpp"
#include "src/engine/element/static/building/LaboratoryBuilding.hpp"
#include "src/engine/element/static/building/ProducerBuilding.hpp"
#include "src/engine/element/static/building/Road.hpp"
#include "src/engine/element/static/building/SanityBuilding.hpp"
#include "src/engine/element/static/building/SchoolBuilding.hpp"
#include "src/engine/element/static/building/StorageBuilding.hpp"
#include "src/engine/processing/TimeCycleProcessor.hpp"
#include "src/global/conf/BuildingInformation.hpp"



StaticElementHandler::StaticElementHandler(
    CharacterFactoryInterface& characterFactory,
    ImmigrantGeneratorInterface& immigrantGenerator,
    TimeCycleProcessor& processor
) :
    characterFactory(characterFactory),
    immigrantGenerator(immigrantGenerator),
    processor(processor),
    buildingSearchEngine(),
    natureElementSearchEngine(),
    currentState(),
    detailsCache()
{

}



StaticElementHandler::~StaticElementHandler()
{
    qDeleteAll(currentState.buildings);
    qDeleteAll(currentState.natureElements);
}



FarmBuilding& StaticElementHandler::generateFarm(const BuildingInformation& conf, const MapArea& area)
{
    auto building(new FarmBuilding(nullptr, characterFactory, conf, area, getBestBuildingEntryPoint(area)));
    currentState.buildings.push_back(building);
    processor.registerBuilding(*building);
    registerBuildingInDetailsCache(area);

    return *building;
}



HouseBuilding& StaticElementHandler::generateHouse(const BuildingInformation& conf, const MapArea& area)
{
    auto building(new HouseBuilding(nullptr, immigrantGenerator, conf, area, getBestBuildingEntryPoint(area)));
    currentState.buildings.push_back(building);
    processor.registerBuilding(*building);
    registerBuildingInDetailsCache(area);
    // TODO: Register a population handler.

    return *building;
}



LaboratoryBuilding& StaticElementHandler::generateLaboratory(const BuildingInformation& conf, const MapArea& area)
{
    auto building(new LaboratoryBuilding(nullptr, characterFactory, conf, area, getBestBuildingEntryPoint(area)));
    currentState.buildings.push_back(building);
    processor.registerBuilding(*building);
    registerBuildingInDetailsCache(area);
    buildingSearchEngine.registerBuilding(*building);

    return *building;
}



ProducerBuilding& StaticElementHandler::generateProducer(const BuildingInformation& conf, const MapArea& area)
{
    auto building(new ProducerBuilding(
        nullptr,
        natureElementSearchEngine,
        characterFactory,
        conf,
        area,
        getBestBuildingEntryPoint(area)
    ));
    currentState.buildings.push_back(building);
    processor.registerBuilding(*building);
    registerBuildingInDetailsCache(area);

    return *building;
}



SanityBuilding& StaticElementHandler::generateSanity(const BuildingInformation& conf, const MapArea& area)
{
    auto building(new SanityBuilding(nullptr, characterFactory, conf, area, getBestBuildingEntryPoint(area)));
    currentState.buildings.push_back(building);
    processor.registerBuilding(*building);
    registerBuildingInDetailsCache(area);

    return *building;
}



SchoolBuilding& StaticElementHandler::generateSchool(const BuildingInformation& conf, const MapArea& area)
{
    auto building(new SchoolBuilding(
        nullptr,
        buildingSearchEngine,
        characterFactory,
        conf,
        area,
        getBestBuildingEntryPoint(area)
    ));
    currentState.buildings.push_back(building);
    processor.registerBuilding(*building);
    registerBuildingInDetailsCache(area);

    return *building;
}



StorageBuilding& StaticElementHandler::generateStorage(const BuildingInformation& conf, const MapArea& area)
{
    auto building(new StorageBuilding(nullptr, conf, area, getBestBuildingEntryPoint(area)));
    currentState.buildings.push_back(building);
    processor.registerBuilding(*building);
    registerBuildingInDetailsCache(area);
    buildingSearchEngine.registerStorageBuilding(*building);

    return *building;
}



Road& StaticElementHandler::generateRoad(const BuildingInformation& conf, const MapCoordinates& location)
{
    auto road(new Road(nullptr, conf, location));
    currentState.buildings.push_back(road);
    registerRoadInDetailsCache(location);

    return *road;
}



MapCoordinates StaticElementHandler::getBestBuildingEntryPoint(const MapArea& area) const
{
    // Fetch a location around the area, starting at the coordinates at north of left point, and turning clockwise
    // around the area.

    auto left(area.getLeft());
    auto right(area.getRight());
    int moveX(1);
    int moveY(0);

    MapCoordinates coordinates(left.getNorth());
    while (!detailsCache.roadCoordinates.contains(hashCoordinates(coordinates))) {
        coordinates.setX(coordinates.getX() + moveX);
        coordinates.setY(coordinates.getY() + moveY);

        if (moveX == 1 && coordinates.getX() > right.getX()) {
            // Overstep top corner.
            moveX = 0;
            moveY = 1;
            coordinates.setY(coordinates.getY() + moveY);
        }
        else if (moveY == 1 && coordinates.getY() > right.getY()) {
            // Overstep right corner.
            moveX = -1;
            moveY = 0;
            coordinates.setX(coordinates.getX() + moveX);
        }
        else if (moveX == -1 && coordinates.getX() < left.getX()) {
            // Overstep bottom corner.
            moveX = 0;
            moveY = -1;
            coordinates.setY(coordinates.getY() + moveY);
        }
        else if (moveY == -1 && coordinates.getY() < left.getY()) {
            // Overstep left corner. No road nodefound.
            return MapCoordinates();
        }
    }

    return coordinates;
}



QString StaticElementHandler::hashCoordinates(const MapCoordinates& coordinates) const
{
    return QString::number(coordinates.getX()) + ';' + QString::number(coordinates.getY());
}



void StaticElementHandler::registerBuildingInDetailsCache(const MapArea& area)
{
    for (auto coordinates : area) {
        auto hash(hashCoordinates(coordinates));
        detailsCache.nonTraversableCoordinates << hash;
        detailsCache.nonConstructibleCoordinates << hash;
    }
}



void StaticElementHandler::registerRoadInDetailsCache(const MapCoordinates& location)
{
    auto hash(hashCoordinates(location));
    detailsCache.roadCoordinates << hash;
    detailsCache.nonConstructibleCoordinates << hash;
}
