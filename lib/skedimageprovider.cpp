#include "skedimageprovider.h"
#include <QtCore>
#include <QImage>
extern "C" {
#include <aui_osd.h>
}

QImage SkedImageProvider::requestImage(const QString &path, QSize *size, const QSize &requestedSize)
{
  int width = 1280;
  int height = 720;

  qDebug() << "SkedImageProvider::requestImage" << path << requestedSize;

  if (!requestedSize.isEmpty()) {
    width = requestedSize.width();
    height = requestedSize.height();
  }

  /*
  QImage img(width, height, QImage::Format_ARGB32);
  img.fill(QColor("green").rgba());
  if (size) *size = QSize(width, height);
  return img;
  */

  QElapsedTimer timer;
  timer.start();

  QSize srcSize = getSize(path);
  if (srcSize.isEmpty()) {
    qWarning() << "SkedImageProvider::requestImage invalid image size" << srcSize;
    return QImage();
  }

  if (width > (double)srcSize.width() / srcSize.height() * height)
      width = (double)srcSize.width() / srcSize.height() * height;
  else if (height > (double)srcSize.height() / srcSize.width() * width)
      height = (double)srcSize.height() / srcSize.width() * width;

  aui_hdl surface_handle;
  if (aui_gfx_sw_surface_create(AUI_OSD_HD_ARGB8888, width, height, &surface_handle)) {
    qWarning() << "SkedImageProvider aui_gfx_sw_surface_create failed";
    return QImage();
  }

  if (aui_gfx_render_image_to_surface(surface_handle, qPrintable(path))) {
    qWarning() << "SkedImageProvider aui_gfx_render_image_to_surface failed";
    aui_gfx_surface_delete(surface_handle);
    return QImage();
  }

  aui_surface_info surface_info;
  aui_gfx_surface_lock(surface_handle);
  if (aui_gfx_surface_info_get(surface_handle, &surface_info)) {
    qWarning() << "SkedImageProvider aui_gfx_surface_info_get failed";
    aui_gfx_surface_unlock(surface_handle);
    aui_gfx_surface_delete(surface_handle);
    return QImage();
  }

  unsigned char * buf = (unsigned char *)malloc(surface_info.pitch * surface_info.height);
  memcpy(buf, surface_info.p_surface_buf, surface_info.pitch * surface_info.height);
  QImage image(buf, surface_info.width, surface_info.height, surface_info.pitch,
               QImage::Format_ARGB32, free, buf);
  //image.save("sked_image_provider.png", "PNG");

  aui_gfx_surface_unlock(surface_handle);
  if (aui_gfx_surface_delete(surface_handle)) {
    qWarning() << "SkedImageProvider aui_gfx_surface_delete failed";
  }

  qDebug("SkedImageProvider::requestImage %s of size %dx%d to %dx%d took %lld ms",
         qPrintable(path), srcSize.width(), srcSize.height(),
         image.width(), image.height(), timer.elapsed());

  if (size) *size = image.size();

  return image;
}

QSize SkedImageProvider::getSize(const QString &path)
{
  int w, h;

  if (aui_gfx_get_image_info(qPrintable(path), &w, &h)) {
    qWarning() << "SkedImageProvider aui_gfx_get_image_info of" << path << "failed";
    return QSize(-1, -1);
  }

  return QSize(w, h);
}
