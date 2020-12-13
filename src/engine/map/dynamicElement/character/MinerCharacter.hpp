#ifndef MINERCHARACTER_HPP
#define MINERCHARACTER_HPP

#include <QtCore/QPointer>

#include "src/engine/map/dynamicElement/character/Character.hpp"
#include "src/defines.hpp"

class MinerCharacter : public Character
{
    private:
        bool goingHome;

    public:
        MinerCharacter(
            CharacterDisposerInterface& characterManager,
            const PathGeneratorInterface& pathGenerator,
            const CharacterInformation& conf,
            const QSharedPointer<AbstractProcessableBuilding>& issuer,
            owner<PathInterface*> path
        );

        void goHome();

        virtual void process(const CycleDate& date) override;
};

#endif // MINERCHARACTER_HPP
