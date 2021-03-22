#include <QtCore>
#include <QtWidgets>
#include "MainView.h"
#include "ImageManager.h"

int main(int argc, char * *argv)
{
    QApplication app(argc, argv);//初始应用程序

    QCoreApplication::setOrganizationName("Junko");//已经默认构造了QSetting
    QCoreApplication::setOrganizationDomain("Coat Corporation");
    QCoreApplication::setApplicationName("Plants vs DSM");

    QTranslator appTranslator;//翻译

    appTranslator.load(QString(":/translations/main.%1.qm").arg(QLocale::system().name()));
    app.installTranslator(&appTranslator);

    InitImageManager();//图片加载器部署

    qsrand((uint) QTime::currentTime().msec());//播种

    MainWindow mainWindow;
    gMainView->switchToMenuScene();//初始界面
    mainWindow.show();
    int res = app.exec();//进入事件循环
    DestoryImageManager();//析构
    return res;
}


