#include <QApplication>
#include <QtQuick>
#include <QtPlugin>
#include "../lib/skedimageprovider.h"
#include <QtCore/QDebug>
extern "C" {
#include <aui_osd.h>
}

#ifdef QT_STATIC
Q_IMPORT_PLUGIN(QtQuick2Plugin)
Q_IMPORT_PLUGIN(QmlFolderListModelPlugin)
#endif

int main(int argc, char *argv[])
{
  aui_gfx_init(NULL, NULL);
  QApplication app(argc, argv);

  QQuickView view;
  QQmlEngine *engine = view.engine();
  engine->addImageProvider(QLatin1String("sked"), new SkedImageProvider);
  view.setSource(QUrl("qrc:/qml/main.qml"));
  view.setColor(QColor(Qt::transparent));
  view.show();
  view.requestActivate();

  return app.exec();
}
