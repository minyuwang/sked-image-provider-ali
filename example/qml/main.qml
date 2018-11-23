import QtQuick 2.7
import Qt.labs.folderlistmodel 2.1

Rectangle {
  id: root
  width: 1280
  height: 720
  color: "#000000"

  property string provider: "image://sked/"
  //property string provider: "file://"

  Item {
    anchors.fill: parent
    Image {
      id: cur_img
      anchors.fill: parent
      sourceSize.width: parent.width;
      sourceSize.height: parent.height;
      fillMode: Image.PreserveAspectFit;
      asynchronous: false;
      cache: false;
    }
    Text {
      anchors.top: parent.top
      anchors.left: parent.left
      anchors.right: parent.right
      anchors.margins: 40
      horizontalAlignment: Text.AlignHCenter
      verticalAlignment: Text.AlignVCenter
      color: "#FFFFFF"
      font.pointSize: 26
      text: {
        var source = cur_img.source.toString()
        if (source.startsWith("image://sked/")) return source.slice(13)
        else if (source.startsWith("file://")) return source.slice(7)
        return source;
      }
    }
  }

  FolderListModel {
    id: folder
    folder: "file:///media/sda1/photos"
    showDirs: false
    sortField: FolderListModel.Name
    nameFilters: [ "*.png", "*.PNG", "*.jpg", "*.JPG", "*.jpeg", "*.JPEG", "*.gif", "*.GIF" ]
  }

  Rectangle {
    anchors.bottom: parent.bottom
    anchors.left: parent.left
    anchors.right: parent.right
    anchors.bottomMargin: 40
    height: 110
    opacity: 0.5
    color: "#606060"

    ListView {
      id: preview
      orientation: Qt.Horizontal
      anchors.fill: parent
      anchors.margins: 10
      spacing: 10
      clip: true
      focus: true
      model: folder
      delegate: Component {
        Image {
          source: root.provider + filePath
          width: 160
          height: 90
          sourceSize.width: 160
          sourceSize.height: 90
          fillMode: Image.PreserveAspectFit
          asynchronous: false;
          cache: false;
          //smooth: true
        }
      }
      highlight: Item {
        width: preview.currentItem == null ? 0 : preview.currentItem.width
        visible: preview.count != 0
        Rectangle {
          anchors.fill: parent
          color: "transparent"
          border.width: 3
          border.color: "yellow"
        }
      }
      highlightMoveVelocity: 2000
      onCurrentIndexChanged: {
        var startTime = new Date().getTime()
        cur_img.source = root.provider + model.get(currentIndex, "filePath")
        console.log("Load " + cur_img.source.toString() + " took " + (new Date().getTime() - startTime) + "ms")
      }
    }
    Text { text: "                 " } /* workaround crash */
  }

  /* for auto run
  Timer {
    id: timer
    interval: 2500
    triggeredOnStart: false
    repeat: true
    running: false
    onTriggered: {
      preview.currentIndex = (preview.currentIndex + 1) % folder.count
    }
  }

  Component.onCompleted: {
    timer.start();
  }
  */
}
