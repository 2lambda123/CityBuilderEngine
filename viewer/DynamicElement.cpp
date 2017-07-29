#include "DynamicElement.hpp"



const qreal BASE_DYNAMIC_ELEMENT_IMAGE_PADDING(5.0);



DynamicElement::DynamicElement(const QSizeF& baseTileSize, const QWeakPointer<AbstractDynamicMapElement>& element, const QPixmap& elementImage) :
    QGraphicsPixmapItem(elementImage),
    baseTileSize(baseTileSize),
    element(element)
{
    setAcceptedMouseButtons(Qt::RightButton);

    refresh();
    setZValue(1.0);
}



bool DynamicElement::stillExists()
{
    return !element.toStrongRef().isNull();
}



MapCoordinates DynamicElement::getCoordinates() const
{
    auto elementAccess(element.toStrongRef());
    if (elementAccess) {
        return elementAccess->getCurrentLocation();
    }

    return MapCoordinates();
}



void DynamicElement::refresh()
{
    auto elementAccess(element.toStrongRef());
    if (elementAccess) {
        const MapCoordinates& location(elementAccess->getCurrentLocation());
        MapCoordinates roundedLocation(location.getRounded());

        qreal xBase(baseTileSize.width() / 2.0 - pixmap().width() / 2.0);
        qreal yBase(-pixmap().height() + baseTileSize.height() / 2.0 + BASE_DYNAMIC_ELEMENT_IMAGE_PADDING);

        qreal xDiff(location.getX() - roundedLocation.getX());
        qreal yDiff(location.getY() - roundedLocation.getY());

        qreal xNew(xBase + xDiff * baseTileSize.width() / 2.0 + yDiff * baseTileSize.width() / 2.0);
        qreal yNew(yBase - xDiff * baseTileSize.height() / 2.0 + yDiff * baseTileSize.height() / 2.0);

        setPos(xNew, yNew);
    }
}
