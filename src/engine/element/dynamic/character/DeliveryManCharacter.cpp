#include "DeliveryManCharacter.hpp"

#include <cassert>

#include "src/engine/element/dynamic/CharacterManagerInterface.hpp"
#include "src/engine/element/static/building/AbstractProcessableBuilding.hpp"
#include "src/engine/element/static/building/StorageBuilding.hpp"
#include "src/engine/map/path/PathGenerator.hpp"
#include "src/engine/map/MapSearchEngine.hpp"



DeliveryManCharacter::DeliveryManCharacter(
    QObject* parent,
    CharacterManagerInterface& characterManager,
    const PathGeneratorInterface& pathGenerator,
    const MapSearchEngine& searchEngine,
    const CharacterInformation& conf,
    AbstractProcessableBuilding& issuer,
    const ItemInformation& transportedItemConf,
    const int transportedQuantity
) :
    Character(parent, characterManager, pathGenerator, conf, issuer),
    searchEngine(searchEngine),
    target(),
    transportedItemConf(transportedItemConf),
    transportedQuantity(transportedQuantity),
    goingHome(false)
{

}



const ItemInformation& DeliveryManCharacter::getTransportedItemConf() const
{
    return transportedItemConf;
}



bool DeliveryManCharacter::isEmpty() const
{
    return transportedQuantity == 0;
}



int DeliveryManCharacter::getTransportedQuantity() const
{
    return transportedQuantity;
}



void DeliveryManCharacter::unload(const int quantity)
{
    assert(quantity <= transportedQuantity);

    transportedQuantity -= quantity;
}



void DeliveryManCharacter::goHome()
{
    if (issuer.isValid()) {
        goingHome = true;
        motionHandler.takePath(pathGenerator.generateShortestRoadPathTo(
            motionHandler.getCurrentLocation(),
            issuer.get().getEntryPoint()
        ));
    }
}



void DeliveryManCharacter::process(const CycleDate& date)
{
    if (target.isNull()) {
        auto storage(searchEngine.getStorageThatCanStore(transportedItemConf));
        if (storage) {
            target.reassign(*storage);
            motionHandler.takePath(pathGenerator.generateShortestRoadPathTo(motionHandler.getCurrentLocation(), storage->getEntryPoint()));
        }
    }

    Character::process(date);

    if (motionHandler.isPathCompleted()) {
        if (goingHome) {
            if (issuer.isValid()) {
                issuer.get().processInteraction(date, *this);
            }
            if (transportedQuantity == 0) {
                characterManager.clearCharacter(*this);
            }
            else {
                // TODO: Fetch a new target?
            }
        }
        else {
            if (target.isValid()) {
                target.get().processInteraction(date, *this);
                notifyViewDataChange();
                if (isEmpty()) {
                    goHome();
                }
                else {
                    // TODO: Fetch a new target?
                }
            }
        }
    }
}
