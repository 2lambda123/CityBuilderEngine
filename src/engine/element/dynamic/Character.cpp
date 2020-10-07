#include "Character.hpp"

#include "src/engine/element/dynamic/MotionHandler.hpp"
#include "src/engine/element/static/ProcessableBuilding.hpp"
#include "src/global/conf/CharacterInformation.hpp"



Character::Character(
    QObject* parent,
    CharacterManagerInterface& characterManager,
    const PathGeneratorInterface& pathGenerator,
    const CharacterInformation& conf,
    ProcessableBuilding& issuer
) :
    QObject(parent),
    AbstractProcessable(),
    characterManager(characterManager),
    pathGenerator(pathGenerator),
    conf(conf),
    motionHandler(conf.getSpeed(), issuer.getEntryPoint()),
    issuer(&issuer)
{

}



bool Character::isOfType(const CharacterInformation& conf) const
{
    return &conf == &this->conf;
}



const CharacterInformation& Character::getConf() const
{
    return conf;
}



const MapCoordinates& Character::getCurrentLocation() const
{
    return motionHandler.getCurrentLocation();
}



optional<ProcessableBuilding*> Character::getIssuer() const
{
    return issuer;
}



void Character::process(const CycleDate& /*date*/)
{
    motionHandler.move();
}
