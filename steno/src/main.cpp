#include "model/media_player/media_player.h"
#include "model/recorder/recorder.h"
#include "model/settings/settings.h"
#include "model/stream/audio/audio_config.h"
#include "model/stream/stream.h"
#include "model/stream/utils/math/angle_calculations.h"
#include "model/stream/video/output/default_virtual_camera_output.h"
#include "view/mainwindow.h"

#include <QApplication>
#include <QFile>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QFile file(":/stylesheets/globalStylesheet.qss");
    file.open(QFile::ReadOnly);
    a.setStyleSheet(QLatin1String(file.readAll()));

    // TODO : move these configurations in a file

    float inRadius = 400.f;
    float outRadius = 1400.f;
    float angleSpan = Model::math::deg2rad(90.f);
    float topDistorsionFactor = 0.08f;
    float bottomDistorsionFactor = 0.f;
    float fisheyeAngle = Model::math::deg2rad(220.f);
    Model::DewarpingConfig dewarpingConfig(inRadius, outRadius, angleSpan, topDistorsionFactor, bottomDistorsionFactor,
                                           fisheyeAngle);

    int fpsTarget = 20;

    int inWidth = 2880;
    int inHeight = 2160;
    Model::VideoConfig videoInputConfig(inWidth, inHeight, fpsTarget, "/dev/video0", Model::ImageFormat::UYVY_FMT);

    int outWidth = 800;
    int outHeight = 600;
    Model::VideoConfig videoOutputConfig(outWidth, outHeight, fpsTarget, "/dev/video1", Model::ImageFormat::UYVY_FMT);

    std::string inDeviceName = "odas";
    int inChannels = 4;
    int inRate = 44100;
    int inFormatBytes = 2;
    bool inIsLittleEndian = true;
    int inPacketAudioSize = 4096;
    int inPacketHeaderSize = 0;
    Model::AudioConfig audioInputConfig(inDeviceName, inChannels, inRate, inFormatBytes, inIsLittleEndian,
                                        inPacketAudioSize, inPacketHeaderSize);

    std::string outDeviceName = "";
    int outChannels = 4;
    int outRate = 44100;
    int outFormatBytes = 2;
    bool outIsLittleEndian = true;
    int outPacketAudioSize = 4096;
    int outPacketHeaderSize = 0;
    Model::AudioConfig audioOutputConfig(outDeviceName, outChannels, outRate, outFormatBytes, outIsLittleEndian,
                                         outPacketAudioSize, outPacketHeaderSize);

    std::shared_ptr<Model::ISettings> settings = std::make_shared<Model::Settings>();

    std::shared_ptr<Model::IMediaPlayer> mediaPlayer = std::make_shared<Model::MediaPlayer>();

    std::shared_ptr<Model::IStream> stream = std::make_shared<Model::Stream>(
        videoInputConfig, videoOutputConfig, audioInputConfig, audioOutputConfig, dewarpingConfig);

    std::shared_ptr<Model::IRecorder> recorder = std::make_shared<Model::Recorder>(settings);

    View::MainWindow w(settings, mediaPlayer, stream, recorder);
    w.show();

    Model::DefaultVirtualCameraOutput::writeDefaultImage();

    return QApplication::exec();
}