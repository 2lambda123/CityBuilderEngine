#ifndef MAPSCENE_HPP
#define MAPSCENE_HPP

#include <QtWidgets/QGraphicsScene>

class Building;
class BuildingInformation;
class Character;
class DynamicElement;
class Map;
class MapArea;
class MapCoordinates;
class MapSize;
class SelectionElement;
class Tile;

class MapScene : public QGraphicsScene
{
        Q_OBJECT

    private:
        const Map& map;
        QList<Tile*> tileList;
        QList<DynamicElement*> dynamicElementList;
        SelectionElement* selectionElement;

    public:
        MapScene(const Map& map);

        /**
         * @brief Request the positioning of a building (trigger selection element).
         *
         * @param type The type of building the user wants to create.
         */
        void requestBuildingPositioning(const BuildingInformation* elementConf);

        /**
         * @brief Request a building creation.
         *
         * @param buildingKey The type of building.
         * @param area        The area of construction.
         */
        void requestBuildingCreation(const BuildingInformation* elementConf, const MapArea& area);

    public slots:
        void registerNewStaticElement(const Building* element);

        void registerNewDynamicElement(const Character* element);

        /**
         * @brief Refresh the map.
         */
        void refresh();

    private:
        Tile* getTileAt(const MapCoordinates& location);

        void addStaticElementBuilding(Tile* tile, const MapSize& elementSize, const QPixmap& elementImage);

        /**
         * @brief Refresh the selection element.
         *
         * @note Please call only if element is visible.
         */
        void refreshSelectionElement();

    private slots:
        void currentTileChanged(Tile* currentTile);

    signals:
        /**
         * @brief Indicate the user requested a building creation.
         *
         * @param buildingKey The type of building.
         * @param area        The area of construction.
         */
        void buildingCreationRequested(const BuildingInformation* elementConf, const MapArea& area);
};

#endif // MAPSCENE_HPP
