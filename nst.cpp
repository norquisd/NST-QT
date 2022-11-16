#include <Python.h>
#include "nst.h"
#include "ui_nst.h"
#include <QFileDialog>
#include <QErrorMessage>
#include <QTextStream>
#include <iostream>
#include <windows.h>


NST::NST(QWidget *parent)
    : QMainWindow(parent)
    , current_index(0)
    , content_image("")
    , style_image("")
    , imsize("")
    , content_layers("")
    , style_layers("")
    , num_steps("")
    , content_weight("")
    , style_weight("")
    , ui(new Ui::NST)
{
    ui->setupUi(this);
    ui->stackedWidget->setCurrentIndex(current_index);

    connect(ui->UploadContentButton, SIGNAL(released()), this, SLOT(UploadButtonPressed()));
    connect(ui->UploadStyleButton, SIGNAL(released()), this, SLOT(UploadButtonPressed()));
    connect(ui->NextButton_0, SIGNAL(released()), this, SLOT(NextButtonPressed()));
    connect(ui->NextButton_1, SIGNAL(released()), this, SLOT(NextButtonPressed()));
}

NST::~NST()
{
    delete ui;
}

void NST::UploadButtonPressed()
{
    QPushButton *button = (QPushButton *)sender();

    QString filename = QFileDialog::getOpenFileName(this, "Upload Image", "", "Image Files (*.png *.jpg *.bmp)");

    if (filename.isNull()) return;

    QImage image;
    bool valid = image.load(filename);

    if (valid)
    {
        image = image.scaledToWidth(ui->ContentLabel->width());
    }
    else
    {
        QErrorMessage *errorLoadingImage = new QErrorMessage(this);
        errorLoadingImage->showMessage("Error loading image.");
        return;
    }


    // check if content or style
    if (button->text() == ui->UploadContentButton->text())
    {
        try
        {
            ui->ContentLabel->setPixmap(QPixmap::fromImage(image));
            ui->ContentLabel->show();
            content_image = filename.toStdString();
        }
        catch (std::exception &exc)
        {
            QErrorMessage *errorShowingContentImage = new QErrorMessage(this);
            std::string message = "Error Displaying image: ";
            message += exc.what();
            errorShowingContentImage->showMessage(QString::fromStdString(message));
            content_image = "";
            return;
        }

    }
    else
    {

        try
        {
            ui->StyleLabel->setPixmap(QPixmap::fromImage(image));
            ui->StyleLabel->show();
            style_image = filename.toStdString();
        }
        catch (std::exception &exc)
        {
            QErrorMessage *errorShowingStyleImage = new QErrorMessage(this);
            std::string message = "Error Displaying image: ";
            message += exc.what();
            errorShowingStyleImage->showMessage(QString::fromStdString(message));
            style_image = "";
            return;
        }
    }

    if (!content_image.empty() && !style_image.empty())
    {
        ui->NextButton_0->setEnabled(true);
    }
    else
    {
        ui->NextButton_0->setEnabled(false);
    }
}

void NST::NextButtonPressed()
{
    if (ui->stackedWidget->currentIndex() == 1)
    {
        CallPython();
        Sleep(5);
        QImage image;
        bool valid = image.load("D:\\Users\\quinn\\Documents\\Projects\\NST-qt\\temp.png");

        if (valid)
        {
            image = image.scaledToWidth(ui->ResultLabel->width(), Qt::SmoothTransformation);
        }
        else
        {
            QErrorMessage *errorLoadingImage = new QErrorMessage(this);
            errorLoadingImage->showMessage("Error loading image.");
            return;
        }


        try
        {
            ui->ResultLabel->setPixmap(QPixmap::fromImage(image));
            ui->ResultLabel->show();
        }
        catch (std::exception &exc)
        {
            QErrorMessage *errorShowingContentImage = new QErrorMessage(this);
            std::string message = "Error Displaying image: ";
            message += exc.what();
            errorShowingContentImage->showMessage(QString::fromStdString(message));
        }

    }

    ui->stackedWidget->setCurrentIndex(++current_index);
}

void NST::BackButtonPressed()
{
    ui->stackedWidget->setCurrentIndex(--current_index);
}

void NST::CallPython()
{
    ParseSettings();
    std::string args[17] = {"nst.py", "--content", content_image, "--style", style_image, "--imsize", imsize, "--content_layers", content_layers, "--style_layers", style_layers, "--num_steps", num_steps, "--style_weight", style_weight, "--content_weight", content_weight};
    wchar_t* _argv[17];

    for(int i=0; i<17; i++)
    {
        wchar_t *arg = Py_DecodeLocale(args[i].c_str(), NULL);
        _argv[i] = arg;
    }

    try
    {
        Py_Initialize();
        PySys_SetArgv(17, _argv);
        FILE* file = fopen("D:\\Users\\quinn\\Documents\\Projects\\NST-qt\\nst.py", "r");

        if (file == NULL)
        {
            qInfo("file is null");
        }
        else
        {
            PyRun_SimpleFile(file, "D:\\Users\\quinn\\Documents\\Projects\\NST-qt\\nst.py");
        }

        Py_Finalize();
    }
    catch (std::exception &exc)
    {
        qInfo(exc.what());
    }

    return;
}

void NST::ParseSettings()
{
    imsize = std::to_string(ui->imageSizeSpinBox->value());
    QString l_content_layers = "";
    QString l_style_layers = "";

    for (int i = 1; i <=5; i++)
    {
        QString content_layer = "content_conv_" + QString::number(i);
        QString style_layer = "style_conv_" + QString::number(i);

        if (NST::findChild<QCheckBox *>(content_layer)->isChecked())
        {
            l_content_layers += QString::number(i);
        }

        if (NST::findChild<QCheckBox *>(style_layer)->isChecked())
        {
            l_style_layers += QString::number(i);
        }
    }

    content_layers = l_content_layers.toStdString();
    style_layers = l_style_layers.toStdString();

    num_steps = std::to_string(ui->numberOfIterationsSpinBox->value());
    content_weight = std::to_string(ui->contentWeightSpinBox->value());
    style_weight = std::to_string(ui->styleWeightSpinBox->value());



//    QTextStream out(stdout);
//    out << "imsize: " << imsize << "\n";
//    out << "content_layers: " << content_layers << "\n";
//    out << "style_layers: " << style_layers << "\n";
//    out << "num_steps: " << iterations << "\n";
//    out << "content_weight: " << content_weight << "\n";
//    out << "style_weight: " << style_weight << "\n";
}

