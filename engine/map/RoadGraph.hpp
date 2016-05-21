#ifndef ROADGRAPH_HPP
#define ROADGRAPH_HPP

#include <QList>

#include "engine/map/MapArea.hpp"
#include "exceptions/UnexpectedException.hpp"

class RoadGraphNode;





/**
 * @brief The road graph is a graph representing the state of the roads on the map.
 *
 * Each road create a new node on the graph. Each node is automatically connected to the eventually existing nodes at
 * direct north, south, west or east. This graph is used to find the shortest path between 2 coordinates on the map (as
 * long as those 2 coordinates are linked together across the graph).
 */
class RoadGraph
{
    private:
        QList<RoadGraphNode*> nodeList;



    public:
        RoadGraph();



        /**
         * @brief Fetch the road graph node located at the given coordinates (or nullptr if none was found).
         */
        RoadGraphNode* fetchNodeAt(const MapCoordinates& coordinates) const;



        /**
         * @brief Fetch the first road graph node arroung the area (or nullptr if none was found).
         *
         * This method is used to find an entry point for a building.
         */
        const RoadGraphNode* fetchNodeArround(const MapArea& area) const;



        /**
         * @brief Get a list of node where a walker can go to continue its walk.
         *
         * The list will contains all the node adjacents to the node at current location. The node at previous location
         * is remove from this list (except if it is the only one in the list).
         *
         * @param previousLocation The preivous location of the walker.
         * @param currentLocation The current location of the walker.
         * @return A list of node.
         */
        QList<const RoadGraphNode*> getNextNodeList(const MapCoordinates& previousLocation, const MapCoordinates& currentLocation) const;



        /**
         * @brief Create a new node at the given coordinates.
         *
         * @throw UnexpectedException A node already exists at those coordinates.
         */
        const RoadGraphNode* createNode(const MapCoordinates& coordinates);



        /**
         * @brief Delete the node at the given coordinates (if the node exists).
         */
        void deleteNode(const MapCoordinates& coordinates);
};

#endif // ROADGRAPH_HPP
