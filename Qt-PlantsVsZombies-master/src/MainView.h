#ifndef PLANTS_VS_ZOMBIES_MAINVIEW_H
#define PLANTS_VS_ZOMBIES_MAINVIEW_H

#include <QtWidgets>
#include "Timer.h"

class SelectorScene;
class GameScene;
class MainWindow;

class MainView: public QGraphicsView
{
    Q_OBJECT

public:
    MainView(MainWindow *mainWindow);
    ~MainView();

    QString getUsername() const;
    void setUsername(const QString &username);
    MainWindow *getMainWindow() const;
    GameScene *getGameScene() const;

    void switchToMenuScene();
    void switchToGameScene(const QString &eName);
    void pauseGameScene();
    void endGameScene();

protected:
    virtual void resizeEvent(QResizeEvent *event) override;

private:
    const int width, height;
    const QString usernameSettingEntry;

    SelectorScene *selectorScene;
    GameScene *gameScene;
    MainWindow *mainWindow;
};

class MainWindow: public QMainWindow
{
    Q_OBJECT

public:
    MainWindow();

    QAction *getFullScreenAction() const;
private:
    const QString fullScreenSettingEntry;//全屏设置

    QGraphicsView *mainView;//图形指针
    QAction *fullScreenAction;//全屏
    QAction *pauseAction;//暂停
};

extern MainView *gMainView;

#endif //PLANTS_VS_ZOMBIES_MAINVIEW_H
