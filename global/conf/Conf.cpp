#include "Conf.hpp"

#include <yaml-cpp/yaml.h>

#include "global/conf/BuildingInformation.hpp"
#include "global/conf/CharacterInformation.hpp"
#include "global/conf/ControlPanelElementInformation.hpp"
#include "global/conf/ItemInformation.hpp"
#include "global/yamlLibraryEnhancement.hpp"



Conf::Conf(QObject* parent, const QString& filePath) :
    QObject(parent),
    items(),
    buildings(),
    characters(),
    controlPanelElements()
{
    // Load file.
    YAML::Node configurationRoot(YAML::LoadFile(filePath.toStdString()));

    // Load item's configuration.
    for (auto node : configurationRoot["items"]) {
        QString key(node.first.as<QString>());
        ItemInformation::checkModel(key, node.second);
        items.insert(key, new ItemInformation(this, key, node.second));
    }

    // Load characters' configuration.
    for (auto node : configurationRoot["characters"]) {
        QString key(node.first.as<QString>());
        characters.insert(key, new CharacterInformation(this, key, node.second));
    }

    // Load buildings' configuration.
    for (auto node : configurationRoot["buildings"]) {
        QString key(node.first.as<QString>());
        BuildingInformation::checkModel(key, node.second);
        buildings.insert(key, new BuildingInformation(this, this, key, node.second));
    }

    // Resolve dependencies.
    for (auto element : buildings) {
        element->resolveDependencies(this);
    }

    // Load control panel items.
    for (auto node : configurationRoot["controlPanel"]["content"]) {
        ControlPanelElementInformation::checkModel(node);
        controlPanelElements.append(new ControlPanelElementInformation(this, this, node));
    }
}



const ItemInformation* Conf::getItemConf(const QString& key) const
{
    return items.value(key);
}



const CharacterInformation* Conf::getCharacterConf(const QString& key) const
{
    return characters.value(key);
}



const BuildingInformation* Conf::getBuildingConf(const QString& key) const
{
    return buildings.value(key);
}



const QList<ControlPanelElementInformation*> Conf::getControlPanelElements() const
{
    return controlPanelElements;
}
