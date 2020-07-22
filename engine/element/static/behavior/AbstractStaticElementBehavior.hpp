#ifndef ABSTRACTSTATICELEMENTBEHAVIOR_HPP
#define ABSTRACTSTATICELEMENTBEHAVIOR_HPP

#include "engine/processing/AbstractProcessable.hpp"

class Character;
class CycleDate;

/**
 * @brief The base class for buildings' behaviors.
 */
class AbstractStaticElementBehavior : public AbstractProcessable
{
        Q_OBJECT

    public:
        AbstractStaticElementBehavior(QObject* parent);

        virtual bool processInteraction(const CycleDate& date, Character* actor) = 0;
};

#endif // ABSTRACTSTATICELEMENTBEHAVIOR_HPP
