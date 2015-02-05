#ifndef DROPAREA_H
#define DROPAREA_H

#include <QLabel>
#include <QFile>

QT_BEGIN_NAMESPACE
class QMimeData;
QT_END_NAMESPACE

class DropArea : public QLabel
{
    Q_OBJECT

public:
    DropArea(QWidget *parent = 0);
    QString *outPath;
    QString *originalFile;
    QString *inputPath;
    int *activePreview;
    void dropEvent(QDropEvent *event) Q_DECL_OVERRIDE;

private slots:
    void clear();
    void previewComplete(int exitStatus);
    void thumbnailComplete();
    void tgaThumbnailComplete();

signals:
    void changed(const QMimeData *mimeData = 0);

protected:
    void dragEnterEvent(QDragEnterEvent *event) Q_DECL_OVERRIDE;
    void dragMoveEvent(QDragMoveEvent *event) Q_DECL_OVERRIDE;
    void dragLeaveEvent(QDragLeaveEvent *event) Q_DECL_OVERRIDE;
    void mouseMoveEvent(QMouseEvent*);
    void leaveEvent(QEvent*);

private:
    QString *clearText;
};

#endif // DROPAREA_H
