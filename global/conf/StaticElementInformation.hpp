#ifndef STATICELEMENTINFORMATION_HPP
#define STATICELEMENTINFORMATION_HPP

#include <QList>
#include <QPixmap>
#include <QString>
#include <yaml-cpp/yaml.h>

#include "global/conf/StaticElementAreaPartDescription.hpp"

class StaticElementInformation : public QObject
{
        Q_OBJECT

    public:
        enum class Type {
            None = 0,
            CityEntryPoint,
            HousingBuilding,
            ServiceBuilding,
            Road,
        };

    private:
        Type type;
        QString key;
        QString title;
        MapSize size;
        int price;
        int employees;
        int fireRiskIncrement;
        int damageRiskIncrement;
        QList<StaticElementAreaPartDescription*> areaDescription;
        QPixmap image;

    public:
        /**
         * @brief Hold the information about a static element.
         *
         * @param configurationYamlNode The YAML node corresponding to a static element configuration.
         */
        StaticElementInformation(QObject* parent, const QString& key, const YAML::Node& model);

        Type getType() const;

        const QString& getTitle() const;

        const MapSize& getSize() const;

        const QPixmap& getImage() const;

        /**
         * @brief Check if the model for a static element is valid.
         *
         * If the model is invalid, it throws an exception.
         *
         * @param key
         * @param model
         * @throws BadConfigurationException
         */
        static void checkModel(const QString& key, const YAML::Node& model);

    private:
        static Type resolveType(const QString& type);
};



// qHash function for using StaticElementInformation::Type as key.
inline uint qHash(StaticElementInformation::Type key, uint seed)
{
    return qHash(static_cast<uint>(key), seed);
}

#endif // STATICELEMENTINFORMATION_HPP
