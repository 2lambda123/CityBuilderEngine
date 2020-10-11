#ifndef BUILDINGVIEW_HPP
#define BUILDINGVIEW_HPP

#include <QtCore/QSharedPointer>
#include <QtCore/QSizeF>
#include <QtCore/QWeakPointer>

class Building;
class BuildingImage;
class ImageLibrary;
class StaticElement;
class Tile;

class BuildingView
{
    private:
        const QSizeF& baseTileSize;
        QWeakPointer<const Building> engineData;
        Tile& tile;
        BuildingImage& image;
        StaticElement* graphicElement;
        int currentViewVersion;
        int animationIndex;

    public:
        BuildingView(
            const ImageLibrary& imageLibrary,
            const QSizeF& baseTileSize,
            const QSharedPointer<const Building>& engineData,
            Tile& tile
        );

        ~BuildingView();

        void updateFromEngineData();

        bool hasBeenDestroyed() const;

        void advanceAnimation();

    private:
        void setDestroyed();
};

#endif // BUILDINGVIEW_HPP
