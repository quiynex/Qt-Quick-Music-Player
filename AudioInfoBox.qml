import QtQuick
import QtMultimedia
import com.company.PlayerController
import MusicPlayer

Item {
    id: root

    visible: !!PlayerController.currentSong

    onVisibleChanged: {
        if (visible) {
            albumVideo.play()
        } else {
            albumVideo.seek(0)
            albumVideo.stop()
        }
    }

    Image {
        id: albumImage

        anchors {
            verticalCenter: parent.verticalCenter
            left: parent.left
        }

        width: 150
        height: 150

        mipmap: true

        source: !!PlayerController.currentSong ? PlayerController.currentSong.imageSource : ""
    }

    // more on this later
    Video {
        id: albumVideo

        anchors {
            verticalCenter: parent.verticalCenter
            left: parent.left
        }

        width: 150
        height: 150

        loops: MediaPlayer.Infinite
        volume: 0

        source: !!PlayerController.currentSong ? PlayerController.currentSong.videoSource : ""
        onSourceChanged: {
            if (source != "")
                play()
            else
                stop()
        }
    }

    Text {
        id: titleText

        anchors {
            bottom: parent.verticalCenter
            left: albumImage.right
            right: parent.right
            margins: 20
        }

        color: "white"
        wrapMode: Text.WrapAtWordBoundaryOrAnywhere
        text: !!PlayerController.currentSong ? PlayerController.currentSong.title : ""

        font {
            pixelSize: 20
            bold: true
        }
    }

    Text {
        id: authorText

        anchors {
            top: parent.verticalCenter
            left: titleText.left
            right: parent.right
            topMargin: 5
        }

        color: "gray"
        wrapMode: Text.WrapAtWordBoundaryOrAnywhere
        text: !!PlayerController.currentSong ? PlayerController.currentSong.authorName : ""

        font {
            pixelSize: 16
        }
    }
}
