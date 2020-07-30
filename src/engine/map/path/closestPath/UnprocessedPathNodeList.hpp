#ifndef UNPROCESSEDPATHNODELIST_HPP
#define UNPROCESSEDPATHNODELIST_HPP

#include <QtCore/QLinkedList>

#include "src/defines.hpp"

class MapCoordinates;
class PathNode;

/**
 * @brief An list for keeping nodes to be processed by the path algorithm.
 */
class UnprocessedPathNodeList
{
    private:
        QLinkedList<owner<PathNode*>> list;

    public:
        UnprocessedPathNodeList();

        ~UnprocessedPathNodeList();

        bool hasNodeToProcess() const;

        void insertNodeToProcess(owner<PathNode*> node);

        bool hasNodeAtLocation(const MapCoordinates& location) const;

        owner<PathNode*> takeClosestToDestination();
};

#endif // UNPROCESSEDPATHNODELIST_HPP
