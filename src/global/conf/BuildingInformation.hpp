#ifndef BUILDINGINFORMATION_HPP
#define BUILDINGINFORMATION_HPP

#include <QtCore/QObject>
#include <QtCore/QList>
#include <QtCore/QString>
#include <QtGui/QPixmap>

#include "src/engine/map/MapSize.hpp"

class BehaviorInformation;
class BuildingAreaPartDescription;
class BuildingSearchCriteriaDescription;
class CharacterInformation;
class Conf;
class ItemInformation;
class ModelReader;
class NatureElementInformation;
namespace YAML {
    class Node;
}

class BuildingInformation : public QObject
{
        Q_OBJECT

    public:
        enum class Type {
            None = 0,
            Building,
            CityEntryPoint,
            Road
        };

        struct Common {
            QString title;
            MapSize size;
            int price;
            int employees;
            int fireRiskIncrement;
            int damageRiskIncrement;
            QList<BuildingAreaPartDescription*> areaDescription;

            explicit Common(const ModelReader& model);
        };

        struct Graphics {
            QPixmap image;

            explicit Graphics(const ModelReader& model);
        };

    private:
        QString key;
        Type type;
        Common common;
        Graphics graphics;
        QList<BehaviorInformation*> behaviors;

    public:
        /**
         * @brief Hold the information about a building.
         */
        BuildingInformation(QObject* parent, const Conf* conf, const ModelReader& model);

        void resolveDependencies(const Conf* conf);

        Type getType() const;

        const QString& getTitle() const;

        const MapSize& getSize() const;

        const QList<BehaviorInformation*>& getBehaviors() const;

        const QPixmap& getImage() const;

    private:
        static Type resolveType(const QString& type);
};

#endif // BUILDINGINFORMATION_HPP
