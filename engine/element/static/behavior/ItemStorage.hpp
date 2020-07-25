#ifndef ITEMSTORAGE_HPP
#define ITEMSTORAGE_HPP

#include <QtCore/QHash>
#include <QtCore/QList>
#include <QtCore/QString>

#include "engine/element/static/behavior/AbstractBehavior.hpp"

class BehaviorInformation;
class Character;
class CycleDate;
class ItemInformation;
class ProcessableBuilding;

class ItemStorage : public AbstractBehavior
{
        Q_OBJECT

    private:
        static const int MAX_CAPACITY = 32;

        enum class Order {
            Accept,
            Get,
            Refuse,
            Empty
        };

        struct ItemStatus {
            Order order;
            int maxQuantity;
            int currentQuantity;

            ItemStatus() :
                order(Order::Accept),
                maxQuantity(MAX_CAPACITY),
                currentQuantity(0)
            {}
        };

    private:
        ProcessableBuilding* issuer;
        const QList<const ItemInformation*>& availableItems;
        QHash<QString, ItemStatus*> storage;

    public:
        ItemStorage(ProcessableBuilding* issuer, const BehaviorInformation* behaviorInformation);

        virtual ~ItemStorage();

        virtual void init(const CycleDate& date) override;

        virtual void process(const CycleDate& date) override;

        virtual bool processInteraction(const CycleDate& date, Character* actor) override;
};

#endif // ITEMSTORAGE_HPP