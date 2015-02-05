#include "droparea.h"

#include <QDragEnterEvent>
#include <QMimeData>
#include <QProcess>
#include <QDir>
#include <QFileInfo>

DropArea::DropArea(QWidget *parent)
    : QLabel(parent)
{
    setFrameStyle(QFrame::Sunken | QFrame::StyledPanel);
    setAlignment(Qt::AlignCenter);
    setAcceptDrops(true);
    setAutoFillBackground(true);
    outPath = new QString("no_image");
    originalFile = new QString("no_image_preview.tga");
    inputPath = new QString();
    activePreview = new int;
    *activePreview = 0;
    clearText = new QString("Drop an image here");
    clear();
}

void DropArea::dragEnterEvent(QDragEnterEvent *event)
{
    if(pixmap() != Q_NULLPTR)
    {
        setBackgroundRole(QPalette::Highlight);
    }

    event->acceptProposedAction();
}

void DropArea::dragMoveEvent(QDragMoveEvent *event)
{
    event->acceptProposedAction();
}

void DropArea::dropEvent(QDropEvent *event)
{
    const QMimeData *mimeData = event->mimeData();

    if (mimeData->hasUrls()) {
        QList<QUrl> urlList = mimeData->urls();
        for (int i = 0; i < urlList.size() && i < 32; ++i) {
            QString url = urlList.at(i).path();
            *inputPath = url;
            while(inputPath->at(0) == '\\' || inputPath->at(0) == '/') *inputPath = inputPath->remove(0, 1);
        }
        *clearText = "Loading image...";
        clear();
        QString fileName = this->objectName();
        *outPath = QDir::tempPath() + "\\channel_shuffle\\" + fileName;
        QString tempPath = QFileInfo(*outPath).path();
        QDir().mkpath(QDir().filePath(tempPath));

        // MAKE PREVIEW
        QFile::remove(*outPath + "_preview.tga");
        QProcess *process = new QProcess(this);
        QString program = "channel_shuffle \"" + *inputPath + "\" \"" + *outPath + "_preview.tga\" -width 256 -height 256 -r 1r -g 1g -b 1b -a 1a";
        process->start(program);
        connect(process, SIGNAL(finished(int)), this, SLOT(previewComplete(int)));

        activateWindow();
    }

    setBackgroundRole(QPalette::Dark);
    event->acceptProposedAction();
}

void DropArea::previewComplete(int exitStatus)
{
    emit(changed());
    if(exitStatus == 0)
    {
        // STORE ORIGINAL
        QFile *inputFile = new QFile(*inputPath);
        QString suffix = QFileInfo(*inputFile).suffix();
        QString originalFilePath = *outPath + "_original." + suffix;
        QFile::remove(originalFilePath);
        if(QFile::copy(*inputPath, originalFilePath))
        {
            *originalFile = originalFilePath;
        }
        else
        {
            setText("ERROR: Unable to\ncopy original file\nto temp directory");
        }

        // CREATE TGA THUMBNAIL
        QString program = "channel_shuffle \"" + *outPath + "_preview.tga\" \"" + *outPath + "_thumbnail.tga\" -width 128 -height 128 -r 1r -g 1g -b 1b -a 1a";
        QProcess *process = new QProcess(this);
        process->start(program);
        connect(process, SIGNAL(finished(int)), this, SLOT(tgaThumbnailComplete()));
    }
    else
    {
        *clearText = "Unsupported format";
        *outPath = "no_image";
        *originalFile = "no_image_preview.tga";
        setPixmap(*new QPixmap());
        clear();

        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setText("Unsupported file format");
        msgBox.setInformativeText("Supported types: bmp, dds, hdr, jpg, png, psd & tga.\n\nPress \"Show details...\" below for more specific format compatibility.");
        msgBox.setDetailedText(
"Readable Image Formats:\n\n\
BMP - non-1bpp, non-RLE (from stb_image documentation)\n\n\
DDS - DXT1/2/3/4/5, uncompressed, cubemaps (can't read 3D DDS files yet)\n\n\
HDR - converted to LDR, unless loaded with *HDR* functions (RGBE or RGBdivA or RGBdivA2)\n\n\
JPG - JPEG baseline (from stb_image documentation)\n\n\
PNG - non-interlaced (from stb_image documentation)\n\n\
PSD - (from stb_image documentation)\n\n\
TGA - greyscale or RGB or RGBA or indexed, uncompressed or RLE");
        msgBox.exec();
    }
}

void DropArea::tgaThumbnailComplete()
{
    // CREATE THUMBNAIL
    QString program = "channel_shuffle \"" + *outPath + "_thumbnail.tga\" \"" + *outPath + "_thumbnail.bmp\" -r 1r -g 1g -b 1b";
    QProcess *process = new QProcess(this);
    process->start(program);
    connect(process, SIGNAL(finished(int)), this, SLOT(thumbnailComplete()));

    // CREATE R, G, B & A THUMBNAILS
    QProcess *processR = new QProcess(this);
    processR->start("channel_shuffle \"" + *outPath + "_thumbnail.tga\" \"" + *outPath + "_thumbnail_r.bmp\" -r 1r -g 1r -b 1r");
    QProcess *processG = new QProcess(this);
    processG->start("channel_shuffle \"" + *outPath + "_thumbnail.tga\" \"" + *outPath + "_thumbnail_g.bmp\" -r 1g -g 1g -b 1g");
    QProcess *processB = new QProcess(this);
    processB->start("channel_shuffle \"" + *outPath + "_thumbnail.tga\" \"" + *outPath + "_thumbnail_b.bmp\" -r 1b -g 1b -b 1b");
    QProcess *processA = new QProcess(this);
    processA->start("channel_shuffle \"" + *outPath + "_thumbnail.tga\" \"" + *outPath + "_thumbnail_a.bmp\" -r 1a -g 1a -b 1a");
}

void DropArea::thumbnailComplete()
{
    setPixmap(QPixmap::fromImage(QImage(*outPath + "_thumbnail.bmp")));
    clear();
    emit changed();
}

void DropArea::dragLeaveEvent(QDragLeaveEvent *event)
{
    event->accept();
}

void DropArea::clear()
{
    if(pixmap() != Q_NULLPTR && !pixmap()->isNull())
    {
        setPixmap(QPixmap::fromImage(QImage(*outPath + "_thumbnail.bmp")));
    }
    else
    {
        setText(*clearText);
        setBackgroundRole(QPalette::Dark);
    }
}

void DropArea::mouseMoveEvent(QMouseEvent *ev)
{
    if(pixmap() != Q_NULLPTR && !pixmap()->isNull())
    {
        int fourth = this->height()/4;
        if(ev->pos().y() < fourth)
        {
            setPixmap(QPixmap::fromImage(QImage(*outPath + "_thumbnail_r.bmp")));
            *activePreview = 1;
        }
        else if (ev->pos().y() < fourth*2)
        {
            setPixmap(QPixmap::fromImage(QImage(*outPath + "_thumbnail_g.bmp")));
            *activePreview = 2;
        }
        else if (ev->pos().y() < fourth*3)
        {
            setPixmap(QPixmap::fromImage(QImage(*outPath + "_thumbnail_b.bmp")));
            *activePreview = 3;
        }
        else
        {
            setPixmap(QPixmap::fromImage(QImage(*outPath + "_thumbnail_a.bmp")));
            *activePreview = 4;
        }
    }
    else
    {
        clear();
    }
}

void DropArea::leaveEvent(QEvent*)
{
    *activePreview = 0;
    clear();
}
