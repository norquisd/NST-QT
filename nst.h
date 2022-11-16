#ifndef NST_H
#define NST_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui
{
class NST;
}
QT_END_NAMESPACE

class NST : public QMainWindow
{
        Q_OBJECT

    public:
        NST(QWidget *parent = nullptr);
        ~NST();

    private:
        int current_index;
        std::string content_image;
        std::string style_image;
        std::string imsize;
        std::string content_layers;
        std::string style_layers;
        std::string num_steps;
        std::string content_weight;
        std::string style_weight;
        Ui::NST *ui;

//    protected:
//        void resizeEvent(QResizeEvent *event) override;

    private slots:
        void UploadButtonPressed();
        void NextButtonPressed();
        void BackButtonPressed();
        void CallPython();
        void ParseSettings();
};

#endif // NST_H
