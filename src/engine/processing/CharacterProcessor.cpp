#include "CharacterProcessor.hpp"

#include "src/engine/element/dynamic/character/Character.hpp"



CharacterProcessor::CharacterProcessor(QObject* parent) :
    QObject(parent),
    AbstractProcessable(),
    processableList(),
    waitingForRegistrationList(),
    waitingForUnregistrationList()
{

}



void CharacterProcessor::registerCharacter(Character* character)
{
    waitingForRegistrationList.append(character);
}



void CharacterProcessor::unregisterCharacter(Character* character)
{
    waitingForUnregistrationList.append(character);
}



void CharacterProcessor::process(const CycleDate& date)
{
    // Process current processable list.
    for (auto processable : processableList) {
        if (processable) {
            processable->process(date);
        } else {
            waitingForUnregistrationList.append(processable);
        }
    }

    // Process unregistration.
    while (!waitingForUnregistrationList.isEmpty()) {
        processableList.remove(waitingForUnregistrationList.takeFirst());
    }

    // Process registration.
    while (!waitingForRegistrationList.isEmpty()) {
        auto processable(waitingForRegistrationList.takeFirst());
        if (processable) {
            processable->init(date);
            processableList.push_back(processable);
        }
    }
}
