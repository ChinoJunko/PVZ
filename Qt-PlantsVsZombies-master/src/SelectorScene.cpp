#include "SelectorScene.h"
#include "MainView.h"
#include "MouseEventPixmapItem.h"
#include "ImageManager.h"

TextItemWithoutBorder::TextItemWithoutBorder(const QString &text, QGraphicsItem *parent)
        : QGraphicsTextItem(text, parent)
{}

void TextItemWithoutBorder::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QStyleOptionGraphicsItem myOption(*option);
    myOption.state &= ~(QStyle::State_Selected | QStyle::State_HasFocus);
    QGraphicsTextItem::paint(painter, &myOption, widget);
}

SelectorScene::SelectorScene()
        : QGraphicsScene(0, 0, 900, 600),
          background      (new QGraphicsPixmapItem    (gImageCache->load("interface/SelectorBackground.png"))),
          adventureShadow (new QGraphicsPixmapItem    (gImageCache->load("interface/SelectorAdventureShadow.png"))),
          adventureButton (new HoverChangedPixmapItem (gImageCache->load("interface/SelectorAdventureButton.png"))),
          survivalShadow  (new QGraphicsPixmapItem    (gImageCache->load("interface/SelectorSurvivalShadow.png"))),
          survivalButton  (new HoverChangedPixmapItem (gImageCache->load("interface/SelectorSurvivalButton.png"))),
          challengeShadow (new QGraphicsPixmapItem    (gImageCache->load("interface/SelectorChallengeShadow.png"))),
          challengeButton (new HoverChangedPixmapItem (gImageCache->load("interface/SelectorChallengeButton.png"))),
          comeBackButton  (new MouseEventPixmapItem    (gImageCache->load("interface/Button.png"))),
          woodSign1       (new QGraphicsPixmapItem    (gImageCache->load("interface/SelectorWoodSign1.png"))),
          woodSign2       (new QGraphicsPixmapItem    (gImageCache->load("interface/SelectorWoodSign2.png"))),
          woodSign3       (new QGraphicsPixmapItem    (gImageCache->load("interface/SelectorWoodSign3.png"))),
          zombieHand      (new MoviePixmapItem        ("interface/SelectorZombieHand.gif")),
          quitButton      (new MouseEventRectItem     (QRectF(0, 0, 79, 53))),
          optionButton      (new MouseEventRectItem     (QRectF(0, 0, 79, 53))),
          usernameText    (new TextItemWithoutBorder  (gMainView->getUsername())),
          backgroundMusic(new QMediaPlayer(this)),
          buttonBleep(new QMediaPlayer(this))
{
    addItem(background);

    quitButton      ->setPen(Qt::NoPen);
    optionButton      ->setPen(Qt::NoPen);

    adventureButton ->setCursor(Qt::PointingHandCursor);
    survivalButton  ->setCursor(Qt::PointingHandCursor);
    challengeButton ->setCursor(Qt::PointingHandCursor);
    optionButton     ->setCursor(Qt::PointingHandCursor);
    quitButton      ->setCursor(Qt::PointingHandCursor);

    //comeBackButton  ->setCursor(Qt::PointingHandCursor);
    comeBackButton->setVisible(false);
    comeBackButton->setEnabled(false);

    adventureShadow ->setPos(468, 82);  addItem(adventureShadow);
    adventureButton ->setPos(474, 80);  addItem(adventureButton);
    survivalShadow  ->setPos(476, 208); addItem(survivalShadow);
    survivalButton  ->setPos(474, 203); addItem(survivalButton);
    challengeShadow ->setPos(480, 307); addItem(challengeShadow);
    challengeButton ->setPos(478, 303); addItem(challengeButton);
    comeBackButton  ->setPos(450, 400); addItem(comeBackButton);
    optionButton    ->setPos(645, 475); addItem(optionButton);
    quitButton      ->setPos(800, 495); addItem(quitButton);
    woodSign1       ->setPos(20, -8);   addItem(woodSign1);
    woodSign2       ->setPos(23, 126);  addItem(woodSign2);
    woodSign3       ->setPos(34, 179);  addItem(woodSign3);
    zombieHand      ->setPos(262, 264); addItem(zombieHand);



    // Text for username
    usernameText->setParentItem(woodSign1);
    usernameText->setPos(35, 91);
    usernameText->setTextWidth(230);
    usernameText->document()->setDocumentMargin(0);
    usernameText->document()->setDefaultTextOption(QTextOption(Qt::AlignCenter));
    usernameText->setDefaultTextColor(QColor::fromRgb(0xf0c060));
    usernameText->setFont(QFont("Microsoft YaHei", 14, QFont::Bold));

    usernameText->installEventFilter(this);
    usernameText->setTextInteractionFlags(Qt::TextEditorInteraction);

    backgroundMusic->setMedia(QUrl("qrc:/audio/Faster.mp3"));
    buttonBleep->setMedia(QUrl("qrc:/audio/bleep.mp3"));

    connect(backgroundMusic, &QMediaPlayer::stateChanged, [this](QMediaPlayer::State state) {
        if (state == QMediaPlayer::StoppedState)
            backgroundMusic->play();
    });

    connect(adventureButton, &HoverChangedPixmapItem::hoverEntered, [this] { buttonBleep->stop(); buttonBleep->play(); });
    connect(survivalButton, &HoverChangedPixmapItem::hoverEntered, [this] { buttonBleep->stop(); buttonBleep->play(); });
    connect(challengeButton, &HoverChangedPixmapItem::hoverEntered, [this] { buttonBleep->stop(); buttonBleep->play(); });

    QMediaPlayer *loseMusic = new QMediaPlayer(this);
    loseMusic->setMedia(QUrl("qrc:/audio/losemusic.mp3"));

    connect(adventureButton, &HoverChangedPixmapItem::clicked, zombieHand, [this, loseMusic] {
        adventureButton->setCursor(Qt::ArrowCursor);
        survivalButton->setCursor(Qt::ArrowCursor);
        challengeButton->setCursor(Qt::ArrowCursor);
        woodSign3->setCursor(Qt::ArrowCursor);
        adventureButton->setEnabled(false);
        survivalButton->setEnabled(false);
        challengeButton->setEnabled(false);
        woodSign3->setEnabled(false);

        zombieHand->start();
        backgroundMusic->pause();
        loseMusic->play();

    });
    connect(comeBackButton, &MouseEventPixmapItem::clicked, [this] {
        adventureButton->setCursor(Qt::PointingHandCursor);
        survivalButton->setCursor(Qt::PointingHandCursor);
        challengeButton->setCursor(Qt::PointingHandCursor);
        woodSign3->setCursor(Qt::PointingHandCursor);
        quitButton->setCursor(Qt::PointingHandCursor);
        optionButton->setCursor(Qt::PointingHandCursor);
        comeBackButton->setCursor(Qt::ArrowCursor);
        adventureButton->setEnabled(true);
        survivalButton->setEnabled(true);
        challengeButton->setEnabled(true);
        woodSign3->setEnabled(true);
        quitButton->setEnabled(true);
        optionButton->setEnabled(true);
        comeBackButton->setEnabled(false);
        comeBackButton->setVisible(false);

    });
    connect(zombieHand, &MoviePixmapItem::finished, [this, loseMusic] {
        (new Timer(this, 4900, [this, loseMusic](){
            loseMusic->stop();
            gMainView->switchToGameScene("4");//gMainView->switchToGameScene(QSettings().value("Global/NextLevel", "1").toString());
        }))->start();
    });
    connect(quitButton, &MouseEventRectItem::clicked, [] {
        gMainView->getMainWindow()->close();
    });
    connect(optionButton, &MouseEventRectItem::clicked, [this] {
        adventureButton->setCursor(Qt::ArrowCursor);
        survivalButton->setCursor(Qt::ArrowCursor);
        challengeButton->setCursor(Qt::ArrowCursor);
        woodSign3->setCursor(Qt::ArrowCursor);
        quitButton->setCursor(Qt::ArrowCursor);
        optionButton->setCursor(Qt::ArrowCursor);
        comeBackButton->setCursor(Qt::PointingHandCursor);
        adventureButton->setEnabled(false);
        survivalButton->setEnabled(false);
        challengeButton->setEnabled(false);
        woodSign3->setEnabled(false);
        quitButton->setEnabled(false);
        optionButton->setEnabled(false);
        comeBackButton->setEnabled(true);
        comeBackButton->setVisible(true);
    });
}

bool SelectorScene::eventFilter(QObject *watched, QEvent *event)
{
    if (watched == usernameText) {
        if (event->type() == QEvent::KeyPress) {
            QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
            if (keyEvent->key() == Qt::Key_Return || keyEvent->key() == Qt::Key_Enter) {
                // Save the username
                gMainView->setUsername(usernameText->toPlainText());
                setFocusItem(nullptr);
                return true;
            }
            return false;
        }
        return false;
    }
    return QGraphicsScene::eventFilter(watched, event);
}

void SelectorScene::loadReady()
{
    gMainView->getMainWindow()->setWindowTitle(tr("Plants vs. Zombies"));//僵尸手
    backgroundMusic->play();
}
