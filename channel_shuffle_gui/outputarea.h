#ifndef OUTPUTAREA_H
#define OUTPUTAREA_H

#include <QLabel>

QT_BEGIN_NAMESPACE
class QMimeData;
QT_END_NAMESPACE

class OutputArea : public QLabel
{
    Q_OBJECT

public:
    OutputArea(QWidget *parent = 0);
    void updatePreview(QString fileName);
    void setCommand(QString command, QString flags);
    void setFinalCommand(QString command, QString format);
    void runCommand(QString fileName, bool final);
    QString getFinalCommand();

private slots:
    void previewCompleteTGA();
    void previewCompleteBMP();

signals:

protected:

private:
    QLabel *label;
    QPixmap *preview;
    QString *fileName;
    QString *tempPath;
    QString *finalOutputPath;
    QString *command;
    QString *finalCommand;
    QString *flags;
    QString *outFormat;
    void mousePressEvent(QMouseEvent*);
    void mouseMoveEvent(QMouseEvent*);
    void leaveEvent(QEvent*);
};

#endif // OUTPUTAREA_H
