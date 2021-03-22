#include "MainView.h"
#include "GameLevelData.h"
#include "SelectorScene.h"
#include "GameScene.h"
#include "AspectRatioLayout.h"

MainView *gMainView;

MainView::MainView(MainWindow *mainWindow)
        : width(900), height(600),
          usernameSettingEntry("Global/Username"),
          selectorScene(nullptr), gameScene(nullptr),
          mainWindow(mainWindow)
{
    gMainView = this;

    setMouseTracking(true);//鼠标跟踪生效

    setRenderHint(QPainter::Antialiasing, true);
    setRenderHint(QPainter::TextAntialiasing, true);
    setRenderHint(QPainter::SmoothPixmapTransform, true);//画笔设置

    setFrameStyle(0);//边框风格
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);//无老滚:>
    setMinimumSize(width, height);

    if (getUsername().isEmpty()) {//设置用户名
        QString username = qgetenv("USER"); // Linux 或 Mac
        if (username.isEmpty())
            username = qgetenv("USERNAME"); // Windows
        if (username.isEmpty())
            username = tr("Guest");
        setUsername(username);
    }
}

MainView::~MainView()
{
    if (selectorScene)
        selectorScene->deleteLater();
    if (gameScene)
        gameScene->deleteLater();
}

QString MainView::getUsername() const
{
    return QSettings().value(usernameSettingEntry, "").toString();
}

void MainView::setUsername(const QString &username)
{
    return QSettings().setValue(usernameSettingEntry, username);
}

MainWindow *MainView::getMainWindow() const
{
    return mainWindow;
}

GameScene *MainView::getGameScene() const
{
    return gameScene;
}

void MainView::switchToGameScene(const QString &eName)//进入游戏画面
{    
    //if (gameScene)
        //gameScene->deleteLater();
    GameScene *newGameScene = new GameScene(GameLevelDataFactory(eName));//实例化游戏界面，同时通过游戏关卡数据工厂载入数据
    setScene(newGameScene);
    gameScene = newGameScene;
    gameScene->loadReady();
}

void MainView::switchToMenuScene()
{
    SelectorScene *newSelectorScene = new SelectorScene;
    setScene(newSelectorScene);
    if (selectorScene)
        selectorScene->deleteLater();
    selectorScene = newSelectorScene;
    selectorScene->loadReady();
}

void MainView::pauseGameScene(){
    gameScene->pauseGame();
}

void MainView::endGameScene(){
    gameScene->exitGame();
}

void MainView::resizeEvent(QResizeEvent *event)
{
    // `fitInView` has a bug causing extra margin.
    // see "https://bugreports.qt.io/browse/QTBUG-11945"
    QRectF viewRect = frameRect();
    QTransform trans;
    trans.scale(viewRect.width() / width, viewRect.height() / height);
    setTransform(trans);
}

MainWindow::MainWindow()
    : fullScreenSettingEntry("UI/FullScreen"),//注册表中全屏设置
      mainView(new MainView(this)),
      fullScreenAction(new QAction),
      pauseAction(new QAction)
{
    // 给mainwindow布局
    QWidget *centralWidget = new QWidget;
    QLayout *layout = new AspectRatioLayout;
    layout->addWidget(mainView);
    centralWidget->setLayout(layout);
    setCentralWidget(centralWidget);
    // Pause
    pauseAction->setCheckable(true);
    pauseAction->setShortcut(Qt::Key_Escape);
    addAction(pauseAction);
    connect(pauseAction, &QAction::toggled, [this] (bool checked) {
        if (gMainView->getGameScene()!=nullptr)
            qobject_cast<MainView *>(mainView)->pauseGameScene();
        else
            close();
    });
    // Full Screen action triggered with "F11"
    fullScreenAction->setCheckable(true);
    fullScreenAction->setShortcut(Qt::Key_F11);
    addAction(fullScreenAction);
    connect(fullScreenAction, &QAction::toggled, [this] (bool checked) {
        if (checked)
            setWindowState(Qt::WindowFullScreen);
        else
            setWindowState(Qt::WindowNoState);
        QSettings().setValue(fullScreenSettingEntry, checked);
    });
    fullScreenAction->setChecked(QSettings().value(fullScreenSettingEntry, false).toBool());
    // Set background color to black
    setPalette(Qt::black);
    setAutoFillBackground(true);

    adjustSize();
}

QAction *MainWindow::getFullScreenAction() const
{
    return fullScreenAction;
}
