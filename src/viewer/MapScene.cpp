#include "MapScene.hpp"

#include <QtWidgets/QGraphicsSceneMouseEvent>

#include "src/engine/state/MapState.hpp"
#include "src/engine/state/State.hpp"
#include "src/engine/processing/TimeCycleProcessor.hpp"
#include "src/engine/state/NatureElementState.hpp"
#include "src/exceptions/OutOfRangeException.hpp"
#include "src/global/conf/BuildingInformation.hpp"
#include "src/global/conf/CharacterInformation.hpp"
#include "src/global/conf/Conf.hpp"
#include "src/global/conf/NatureElementInformation.hpp"
#include "src/viewer/construction/ConstructionCursor.hpp"
#include "src/viewer/element/graphics/DynamicElement.hpp"
#include "src/viewer/element/graphics/StaticElement.hpp"
#include "src/viewer/element/CharacterView.hpp"
#include "src/viewer/element/BuildingView.hpp"
#include "src/viewer/image/BuildingImage.hpp"
#include "src/viewer/image/CharacterImage.hpp"
#include "src/viewer/image/ImageLibrary.hpp"
#include "src/viewer/image/NatureElementImage.hpp"
#include "src/viewer/Tile.hpp"



MapScene::MapScene(
    const Conf& conf,
    const AreaCheckerInterface& areaChecker,
    const RoadPathGeneratorInterface& roadPathGenerator,
    const MapState& mapState,
    const State& initialState
) :
    QGraphicsScene(),
    areaChecker(areaChecker),
    roadPathGenerator(roadPathGenerator),
    imageLibrary(conf),
    positioning(conf.getTileSize()),
    tiles(),
    buildings(),
    characters(),
    selectionElement(nullptr),
    animationClock(),
    currentTileLocation()
{
    setBackgroundBrush(QBrush(Qt::black));

    // Get the grass conf.
    auto& grassConf(conf.getNatureElementConf("grass"));
    auto& grassImage(imageLibrary.getNatureElementImage(grassConf));

    // Create the tiles and their graphics item.
    int line(0);
    int column(0);
    while (line < mapState.size.height()) {
        // NOTE: Because we divide by 2 and casting as integer, we deliberately remove floating precision. However, the
        // adjustment needs to be 1 higher when "mapSize.width() - line" become negative. This is because -0.5 is cast
        // to 0 insted of -1.
        int adjust(line > mapState.size.width() ? 1 : 2);
        while (column < (mapState.size.width() - line + adjust) / 2) {
            MapCoordinates coordinates(column, line + column);
            auto tile(new Tile(
                positioning,
                coordinates,
                *new StaticElement(positioning, MapSize(), grassImage.getImage())
            ));

            addItem(tile);
            tiles.insert(coordinates.getHash(), tile);

            ++column;
        }
        ++line;
        column = -line / 2;
    }

    // Load existing elements.
    for (auto buildingState : initialState.buildings) {
        registerNewBuilding(buildingState);
    }
    for (auto natureElementState : initialState.natureElements) {
        registerNewNatureElement(natureElementState);
    }

    animationClock.start(100, this);
}



MapScene::~MapScene()
{
    qDeleteAll(buildings);
    qDeleteAll(characters);
    if (selectionElement) {
        delete selectionElement;
    }
    qDeleteAll(tiles);
}



Tile& MapScene::getTileAt(const MapCoordinates& location) const
{
    for (auto tile : tiles) {
        if (tile->getCoordinates() == location) {
            return *tile;
        }
    }

    throw OutOfRangeException("Unable to find tile located at " + location.toString());
}



void MapScene::requestBuildingPositioning(const BuildingInformation& elementConf)
{
    if (selectionElement) {
        delete selectionElement;
    }
    selectionElement = new ConstructionCursor(
        positioning,
        areaChecker,
        roadPathGenerator,
        elementConf,
        imageLibrary.getBuildingImage(elementConf)
    );
    addItem(selectionElement);
    connect(selectionElement, &ConstructionCursor::cancel, [this]() {
        delete selectionElement;
        selectionElement = nullptr;
    });
    connect(selectionElement, &ConstructionCursor::construct, [this](const BuildingInformation& buildingConf, QList<MapArea> area) {
        for (auto location : area) {
            emit buildingCreationRequested(buildingConf, location);
        }
    });
}



void MapScene::registerNewBuilding(const BuildingState& buildingState)
{
    buildings.insert(
        buildingState.id,
        new BuildingView(positioning, *this, imageLibrary, buildingState)
    );
    if (selectionElement) {
        selectionElement->refresh();
    }
}



void MapScene::registerNewCharacter(const CharacterState& characterState)
{
    characters.insert(
        characterState.id,
        new CharacterView(positioning, *this, imageLibrary, characterState)
    );
}



void MapScene::registerNewNatureElement(const NatureElementState& natureElementState)
{
    auto& tile(getTileAt(natureElementState.area.getLeft()));
    auto& natureElementImage(imageLibrary.getNatureElementImage(natureElementState.type));

    tile.setStaticElement(new StaticElement(positioning, natureElementState.area.getSize(), natureElementImage.getImage()));
    // TODO: Handle higher size of nature elements by hiding covered tiles (see BuildingView).
}



void MapScene::refresh(const State& state)
{
    auto buildingsToDestroy(buildings.keys());
    for (auto& buildingState : state.buildings) {
        if (buildings.contains(buildingState.id)) {
            // Update existing building.
            buildings.value(buildingState.id)->update(buildingState);
            buildingsToDestroy.removeOne(buildingState.id);
        }
        else {
            // Create new building.
            registerNewBuilding(buildingState);
        }
    }
    for (auto buildingId : buildingsToDestroy) {
        // Delete not found buildings.
        buildings.value(buildingId)->destroy();
        buildings.remove(buildingId);
    }

    auto charactersToDestroy(characters.keys());
    for (auto& characterState : state.characters) {
        if (characters.contains(characterState.id)) {
            // Update existing character.
            characters.value(characterState.id)->update(characterState);
            charactersToDestroy.removeOne(characterState.id);
        }
        else {
            // Create new character.
            registerNewCharacter(characterState);
        }
    }
    for (auto characterId : charactersToDestroy) {
        // Delete not found characters.
        characters.value(characterId)->destroy();
        characters.remove(characterId);
    }
}



void MapScene::timerEvent(QTimerEvent* /*event*/)
{
    for (auto building : buildings) {
        building->advanceAnimation();
    }
}



void MapScene::mouseMoveEvent(QGraphicsSceneMouseEvent* event)
{
    auto newMapCoordinates(positioning.getMapCoordinatesFromMouseCoordinates(event->scenePos()));
    if (newMapCoordinates != currentTileLocation) {
        currentTileLocation = newMapCoordinates;
        if (selectionElement) {
            selectionElement->displayAtLocation(newMapCoordinates);
        }
    }
}
