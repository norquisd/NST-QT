#include "qscalinglabel.h"
#include <QResizeEvent>

QScalingLabel::QScalingLabel(QWidget *parent) : QLabel(parent) {
// allow to shrink to 1 pixel
    setMinimumSize(1,1);
}

void QScalingLabel::resizeEvent(QResizeEvent *pQEvent)
{
  QLabel::resizeEvent(pQEvent);
  if (!_qPixmap.isNull()){
  setPixmap(_qPixmap, this->width());
  }

}

void QScalingLabel::setPixmap(const QPixmap &qPixmap, const int &width)
{
  _qPixmap = qPixmap;
  _qPixmapScaled = _qPixmap.scaledToWidth(width, Qt::SmoothTransformation);
  QLabel::setPixmap(_qPixmapScaled);
}
