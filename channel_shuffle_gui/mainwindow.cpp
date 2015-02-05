#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    QString tempDir = QDir::tempPath() + "\\channel_shuffle\\";

    // CLEAN TEMP FOLDER
    QDir dir = QDir(tempDir);
    dir.setNameFilters(QStringList() << "*.*");
    dir.setFilter(QDir::Files);
    foreach(QString dirFile, dir.entryList())
    {
        dir.remove(dirFile);
    }

    ui->setupUi(this);

    connect(ui->actionCopy_command_to_clipboard, SIGNAL(triggered()), this, SLOT(copyCommandToClipboard()));

    // SET DEFAULT IMAGES
    QMimeData* mimeData = new QMimeData();
    QList<QUrl>* urlList = new QList<QUrl>();
    urlList->append(QUrl("file:///" + QDir("image_1.tga").absolutePath()));
    mimeData->setUrls(*urlList);
    QDropEvent* input1Event = new QDropEvent(QPointF(), Qt::DropAction::MoveAction, mimeData, Qt::MouseButton::LeftButton, Q_NULLPTR);
    ui->input_1->dropEvent(input1Event);

    urlList->clear();
    urlList->append(QUrl("file:///" + QDir("image_2.tga").absolutePath()));
    mimeData->setUrls(*urlList);
    input1Event = new QDropEvent(QPointF(), Qt::DropAction::MoveAction, mimeData, Qt::MouseButton::LeftButton, Q_NULLPTR);
    ui->input_2->dropEvent(input1Event);

    // HIDE STUFF THAT ISN'T IMPLEMENTED
    ui->editFileType->hide();
    ui->line->hide();
    ui->toolButton->hide();
    ui->outWidth->hide();
    ui->outHeight->hide();
    ui->label->hide();

    // HOTKEYS
    actionRed = new QAction(this);
    QList<QKeySequence>* list = new QList<QKeySequence>();
    list->append(Qt::Key_1);
    list->append(Qt::Key_R);
    actionRed->setShortcuts(*list);
    list->clear();
    connect(actionRed, SIGNAL(triggered()), this, SLOT(hotkeyRed()));
    addAction(actionRed);

    actionGreen = new QAction(this);
    list->append(Qt::Key_2);
    list->append(Qt::Key_G);
    actionGreen->setShortcuts(*list);
    list->clear();
    connect(actionGreen, SIGNAL(triggered()), this, SLOT(hotkeyGreen()));
    addAction(actionGreen);

    actionBlue = new QAction(this);
    list->append(Qt::Key_3);
    list->append(Qt::Key_B);
    actionBlue->setShortcuts(*list);
    list->clear();
    connect(actionBlue, SIGNAL(triggered()), this, SLOT(hotkeyBlue()));
    addAction(actionBlue);

    actionAlpha = new QAction(this);
    list->append(Qt::Key_4);
    list->append(Qt::Key_A);
    actionAlpha->setShortcuts(*list);
    delete list;
    connect(actionAlpha, SIGNAL(triggered()), this, SLOT(hotkeyAlpha()));
    addAction(actionAlpha);

    connect(ui->actionAbout, SIGNAL(triggered()), this, SLOT(openAboutWindow()));
    connect(ui->input_1, SIGNAL(changed()), this, SLOT(updateOutput()));
    connect(ui->input_2, SIGNAL(changed()), this, SLOT(updateOutput()));

    connect(ui->invertR, SIGNAL(clicked()), this, SLOT(updateOutput()));
    connect(ui->invertG, SIGNAL(clicked()), this, SLOT(updateOutput()));
    connect(ui->invertB, SIGNAL(clicked()), this, SLOT(updateOutput()));
    connect(ui->invertA, SIGNAL(clicked()), this, SLOT(updateOutput()));


    connect(ui->fileType, SIGNAL(currentIndexChanged(int)), this, SLOT(updateFileType()));


    this->setStyleSheet("QRadioButton::indicator::unchecked {image: url(:/about/radio_off.png);}"
                        "QRadioButton::indicator::checked {image: url(:/about/radio_on.png);}");

    connect(ui->r1r, SIGNAL(clicked()), this, SLOT(updateOutput()));
    connect(ui->r1g, SIGNAL(clicked()), this, SLOT(updateOutput()));
    connect(ui->r1b, SIGNAL(clicked()), this, SLOT(updateOutput()));
    connect(ui->r1a, SIGNAL(clicked()), this, SLOT(updateOutput()));

    connect(ui->g1r, SIGNAL(clicked()), this, SLOT(updateOutput()));
    connect(ui->g1g, SIGNAL(clicked()), this, SLOT(updateOutput()));
    connect(ui->g1b, SIGNAL(clicked()), this, SLOT(updateOutput()));
    connect(ui->g1a, SIGNAL(clicked()), this, SLOT(updateOutput()));

    connect(ui->b1r, SIGNAL(clicked()), this, SLOT(updateOutput()));
    connect(ui->b1g, SIGNAL(clicked()), this, SLOT(updateOutput()));
    connect(ui->b1b, SIGNAL(clicked()), this, SLOT(updateOutput()));
    connect(ui->b1a, SIGNAL(clicked()), this, SLOT(updateOutput()));

    connect(ui->a1r, SIGNAL(clicked()), this, SLOT(updateOutput()));
    connect(ui->a1g, SIGNAL(clicked()), this, SLOT(updateOutput()));
    connect(ui->a1b, SIGNAL(clicked()), this, SLOT(updateOutput()));
    connect(ui->a1a, SIGNAL(clicked()), this, SLOT(updateOutput()));


    connect(ui->r2r, SIGNAL(clicked()), this, SLOT(updateOutput()));
    connect(ui->r2g, SIGNAL(clicked()), this, SLOT(updateOutput()));
    connect(ui->r2b, SIGNAL(clicked()), this, SLOT(updateOutput()));
    connect(ui->r2a, SIGNAL(clicked()), this, SLOT(updateOutput()));

    connect(ui->g2r, SIGNAL(clicked()), this, SLOT(updateOutput()));
    connect(ui->g2g, SIGNAL(clicked()), this, SLOT(updateOutput()));
    connect(ui->g2b, SIGNAL(clicked()), this, SLOT(updateOutput()));
    connect(ui->g2a, SIGNAL(clicked()), this, SLOT(updateOutput()));

    connect(ui->b2r, SIGNAL(clicked()), this, SLOT(updateOutput()));
    connect(ui->b2g, SIGNAL(clicked()), this, SLOT(updateOutput()));
    connect(ui->b2b, SIGNAL(clicked()), this, SLOT(updateOutput()));
    connect(ui->b2a, SIGNAL(clicked()), this, SLOT(updateOutput()));

    connect(ui->a2r, SIGNAL(clicked()), this, SLOT(updateOutput()));
    connect(ui->a2g, SIGNAL(clicked()), this, SLOT(updateOutput()));
    connect(ui->a2b, SIGNAL(clicked()), this, SLOT(updateOutput()));
    connect(ui->a2a, SIGNAL(clicked()), this, SLOT(updateOutput()));
}

// HOTKEYS
void MainWindow::hotkeyRed()
{
    switch (*ui->input_1->activePreview)
    {
    case 1:
    {
        ui->r1r->setChecked(true);
        break;
    }
    case 2:
    {
        ui->r1g->setChecked(true);
        break;
    }
    case 3:
    {
        ui->r1b->setChecked(true);
        break;
    }
    case 4:
    {
        ui->r1a->setChecked(true);
        break;
    }
    default:
    {
        break;
    }
    }
    switch (*ui->input_2->activePreview)
    {
    case 1:
    {
        ui->r2r->setChecked(true);
        break;
    }
    case 2:
    {
        ui->r2g->setChecked(true);
        break;
    }
    case 3:
    {
        ui->r2b->setChecked(true);
        break;
    }
    case 4:
    {
        ui->r2a->setChecked(true);
        break;
    }
    default:
    {
        break;
    }
    }
    updateOutput();
}

void MainWindow::hotkeyGreen()
{
    switch (*ui->input_1->activePreview)
    {
    case 1:
    {
        ui->g1r->setChecked(true);
        break;
    }
    case 2:
    {
        ui->g1g->setChecked(true);
        break;
    }
    case 3:
    {
        ui->g1b->setChecked(true);
        break;
    }
    case 4:
    {
        ui->g1a->setChecked(true);
        break;
    }
    default:
    {
        break;
    }
    }
    switch (*ui->input_2->activePreview)
    {
    case 1:
    {
        ui->g2r->setChecked(true);
        break;
    }
    case 2:
    {
        ui->g2g->setChecked(true);
        break;
    }
    case 3:
    {
        ui->g2b->setChecked(true);
        break;
    }
    case 4:
    {
        ui->g2a->setChecked(true);
        break;
    }
    default:
    {
        break;
    }
    }
    updateOutput();
}

void MainWindow::hotkeyBlue()
{
    switch (*ui->input_1->activePreview)
    {
    case 1:
    {
        ui->b1r->setChecked(true);
        break;
    }
    case 2:
    {
        ui->b1g->setChecked(true);
        break;
    }
    case 3:
    {
        ui->b1b->setChecked(true);
        break;
    }
    case 4:
    {
        ui->b1a->setChecked(true);
        break;
    }
    default:
    {
        break;
    }
    }
    switch (*ui->input_2->activePreview)
    {
    case 1:
    {
        ui->b2r->setChecked(true);
        break;
    }
    case 2:
    {
        ui->b2g->setChecked(true);
        break;
    }
    case 3:
    {
        ui->b2b->setChecked(true);
        break;
    }
    case 4:
    {
        ui->b2a->setChecked(true);
        break;
    }
    default:
    {
        break;
    }
    }
    updateOutput();
}

void MainWindow::hotkeyAlpha()
{
    switch (*ui->input_1->activePreview)
    {
    case 1:
    {
        ui->a1r->setChecked(true);
        break;
    }
    case 2:
    {
        ui->a1g->setChecked(true);
        break;
    }
    case 3:
    {
        ui->a1b->setChecked(true);
        break;
    }
    case 4:
    {
        ui->a1a->setChecked(true);
        break;
    }
    default:
    {
        break;
    }
    }
    switch (*ui->input_2->activePreview)
    {
    case 1:
    {
        ui->a2r->setChecked(true);
        break;
    }
    case 2:
    {
        ui->a2g->setChecked(true);
        break;
    }
    case 3:
    {
        ui->a2b->setChecked(true);
        break;
    }
    case 4:
    {
        ui->a2a->setChecked(true);
        break;
    }
    default:
    {
        break;
    }
    }
    updateOutput();
}

void MainWindow::updateOutput()
{
    QString command = "channel_shuffle";
    QString finalCommand = "channel_shuffle";
    if (!ui->input_1->outPath->isEmpty())
    {
        command += " \"" + *ui->input_1->outPath + "_preview.tga\"";
        finalCommand += " \"" + *ui->input_1->originalFile + "\"";
    }
    if (!ui->input_2->outPath->isEmpty())
    {
        command += " \"" + *ui->input_2->outPath + "_preview.tga\"";
        finalCommand += " \"" + *ui->input_2->originalFile + "\"";
    }

    QString flags;
    flags += "-r ";
    if(ui->r1r->isChecked())
    {
        flags += "1r";
    }
    else if(ui->r1g->isChecked())
    {
        flags += "1g";
    }
    else if(ui->r1b->isChecked())
    {
        flags += "1b";
    }
    else if(ui->r1a->isChecked())
    {
        flags += "1a";
    }
    else if(ui->r2r->isChecked())
    {
        flags += "2r";
    }
    else if(ui->r2g->isChecked())
    {
        flags += "2g";
    }
    else if(ui->r2b->isChecked())
    {
        flags += "2b";
    }
    else if(ui->r2a->isChecked())
    {
        flags += "2a";
    }
    if(ui->invertR->isChecked())
    {
        flags += "i";
    }

    flags += " -g ";
    if(ui->g1r->isChecked())
    {
        flags += "1r";
    }
    else if(ui->g1g->isChecked())
    {
        flags += "1g";
    }
    else if(ui->g1b->isChecked())
    {
        flags += "1b";
    }
    else if(ui->g1a->isChecked())
    {
        flags += "1a";
    }
    else if(ui->g2r->isChecked())
    {
        flags += "2r";
    }
    else if(ui->g2g->isChecked())
    {
        flags += "2g";
    }
    else if(ui->g2b->isChecked())
    {
        flags += "2b";
    }
    else if(ui->g2a->isChecked())
    {
        flags += "2a";
    }
    if(ui->invertG->isChecked())
    {
        flags += "i";
    }


    flags += " -b ";
    if(ui->b1r->isChecked())
    {
        flags += "1r";
    }
    else if(ui->b1g->isChecked())
    {
        flags += "1g";
    }
    else if(ui->b1b->isChecked())
    {
        flags += "1b";
    }
    else if(ui->b1a->isChecked())
    {
        flags += "1a";
    }
    else if(ui->b2r->isChecked())
    {
        flags += "2r";
    }
    else if(ui->b2g->isChecked())
    {
        flags += "2g";
    }
    else if(ui->b2b->isChecked())
    {
        flags += "2b";
    }
    else if(ui->b2a->isChecked())
    {
        flags += "2a";
    }
    if(ui->invertB->isChecked())
    {
        flags += "i";
    }

    flags += " -a ";
    if(ui->a1r->isChecked())
    {
        flags += "1r";
    }
    else if(ui->a1g->isChecked())
    {
        flags += "1g";
    }
    else if(ui->a1b->isChecked())
    {
        flags += "1b";
    }
    else if(ui->a1a->isChecked())
    {
        flags += "1a";
    }
    else if(ui->a2r->isChecked())
    {
        flags += "2r";
    }
    else if(ui->a2g->isChecked())
    {
        flags += "2g";
    }
    else if(ui->a2b->isChecked())
    {
        flags += "2b";
    }
    else if(ui->a2a->isChecked())
    {
        flags += "2a";
    }
    if(ui->invertA->isChecked())
    {
        flags += "i";
    }

    ui->output->setCommand(command, flags);
    ui->output->setFinalCommand(finalCommand, ui->fileType->currentText());
    ui->output->updatePreview("output_preview");
}

void MainWindow::updateFileType()
{
    ui->output->setFinalCommand(Q_NULLPTR, ui->fileType->currentText());
}

void MainWindow::openAboutWindow()
{
    aboutWindow = new About();
    aboutWindow->show();
}

void MainWindow::copyCommandToClipboard()
{
    QClipboard *clipboard = QApplication::clipboard();
    clipboard->setText(ui->output->getFinalCommand());
}

MainWindow::~MainWindow()
{
    delete ui;
}
