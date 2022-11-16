#ifndef QSCALINGLABEL_H
#define QSCALINGLABEL_H

#include <QLabel>

class QScalingLabel: public QLabel
{
private:
    QPixmap _qPixmap, _qPixmapScaled;
public:
    QScalingLabel(QWidget *parent = nullptr);
    void setPixmap(const QPixmap &pix) { setPixmap(pix, width()); };
protected:
  virtual void resizeEvent(QResizeEvent *evt) override;
private:
    void setPixmap(const QPixmap &pix, const int &width);
};

#endif
