#ifndef SKEDIMAGEPROVIDER_H
#define SKEDIMAGEPROVIDER_H
#include <QQuickImageProvider>
#include <QtCore>

class SkedImageProvider : public QQuickImageProvider
{
public:
    SkedImageProvider()
        : QQuickImageProvider(QQuickImageProvider::Image)
    {
    }

    QImage requestImage(const QString &id, QSize *size, const QSize &requestedSize) override;

private:
    QSize getSize(const QString &path);
};
#endif // SKEDIMAGEPROVIDER_H
