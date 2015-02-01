#include "outputarea.h"
#include "QDrag"

#include <QDragEnterEvent>
#include <QMimeData>
#include <QProcess>
#include <QDir>
#include <QFileInfo>

OutputArea::OutputArea(QWidget *parent)
    : QLabel(parent)
{
    setFrameStyle(QFrame::Sunken | QFrame::StyledPanel);
    setAlignment(Qt::AlignCenter);
    setAutoFillBackground(true);
    command = new QString();
    finalCommand = new QString();
    flags = new QString();
    preview = new QPixmap();
    tempPath = new QString();
    finalOutputPath = new QString();
    fileName = new QString("untitled");
    outFormat = new QString();
    setBackgroundRole(QPalette::Background);
}

void OutputArea::setCommand(QString command = Q_NULLPTR, QString flags = Q_NULLPTR)
{
    if(!command.isNull())
    {
        *this->command = command;
    }
    if(!flags.isNull())
    {
        *this->flags = flags;
    }
}

void OutputArea::setFinalCommand(QString command = Q_NULLPTR, QString format = Q_NULLPTR)
{
    if(!command.isNull())
    {
        *this->finalCommand = command;
    }
    if(!format.isNull())
    {
        *this->outFormat = format.toLower();
    }
}

QString OutputArea::getFinalCommand()
{
    QString finalOutFileName = *fileName + "." + *outFormat;
    QString finalOutFilePath = QDir::tempPath() + "\\channel_shuffle\\" + finalOutFileName;
    return QString(*finalCommand + " \"" + finalOutFilePath + "\" " + *flags);
}

void OutputArea::updatePreview(QString fileName)
{
    *tempPath = QDir::tempPath() + "\\channel_shuffle\\" + fileName;
    QFile::remove(*tempPath + ".tga");
    QString program = *command + " \"" + *tempPath+ ".tga\" " + *flags;

    QProcess *process = new QProcess(this);
    process->start(program);
    connect(process, SIGNAL(finished(int)), this, SLOT(previewCompleteTGA()));
}

void OutputArea::runCommand(QString fileName, bool final = false)
{
    *finalOutputPath = QDir::tempPath() + "\\channel_shuffle\\" + fileName;
    QFile::remove(*finalOutputPath);
    QProcess *process = new QProcess(this);
    QString commandToRun;
    commandToRun = final ? *finalCommand : *command;
    QString program = commandToRun + " \"" + *finalOutputPath + "\" " + *flags;
    process->start(program);
    connect(process, SIGNAL(finished()), this, SLOT(previewCompleteBMP()));
}

void OutputArea::previewCompleteTGA()
{
    QFile::remove(*tempPath + ".bmp");
    QString program = "channel_shuffle \"" + *tempPath + ".tga\" \"" + *tempPath + ".bmp\" -r 1r -g 1g -b 1b";
    QProcess *process = new QProcess(this);
    process->start(program);
    connect(process, SIGNAL(finished(int)), this, SLOT(previewCompleteBMP()));

    // CREATE R, G, B & A PREVIEWS
    QProcess *processR = new QProcess(this);
    processR->start("channel_shuffle \"" + *tempPath + ".tga\" \"" + *tempPath + "_r.bmp\" -r 1r -g 1r -b 1r");
    QProcess *processG = new QProcess(this);
    processG->start("channel_shuffle \"" + *tempPath + ".tga\" \"" + *tempPath + "_g.bmp\" -r 1g -g 1g -b 1g");
    QProcess *processB = new QProcess(this);
    processB->start("channel_shuffle \"" + *tempPath + ".tga\" \"" + *tempPath + "_b.bmp\" -r 1b -g 1b -b 1b");
    QProcess *processA = new QProcess(this);
    processA->start("channel_shuffle \"" + *tempPath + ".tga\" \"" + *tempPath + "_a.bmp\" -r 1a -g 1a -b 1a");
}

void OutputArea::previewCompleteBMP()
{
    *preview = QPixmap::fromImage(QImage(*tempPath + ".bmp"));
    setPixmap(*preview);
}

void OutputArea::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton
            && !pixmap()->isNull())
    {
        QString finalOutFileName = *fileName + "." + *outFormat;
        QString finalOutPath = QDir::tempPath() + "\\channel_shuffle\\" + finalOutFileName;
        runCommand(finalOutFileName, true);

        QDrag *drag = new QDrag(this);
        QMimeData *mimeData = new QMimeData;
        QList<QUrl> url = QList<QUrl>();
        url.append(QUrl("file:///" + finalOutPath));
        mimeData->setUrls((const QList<QUrl>)url);
        drag->setMimeData(mimeData);
        drag->exec();
    }
}

void OutputArea::mouseMoveEvent(QMouseEvent *ev)
{
    int fourth = this->height()/4;
    if(ev->pos().y() < fourth)
    {
        setPixmap(QPixmap::fromImage(QImage(*tempPath + "_r.bmp")));
    }
    else if (ev->pos().y() < fourth*2)
    {
        setPixmap(QPixmap::fromImage(QImage(*tempPath + "_g.bmp")));
    }
    else if (ev->pos().y() < fourth*3)
    {
        setPixmap(QPixmap::fromImage(QImage(*tempPath + "_b.bmp")));
    }
    else
    {
        setPixmap(QPixmap::fromImage(QImage(*tempPath + "_a.bmp")));
    }
}

void OutputArea::leaveEvent(QEvent*)
{
    previewCompleteBMP();
}
