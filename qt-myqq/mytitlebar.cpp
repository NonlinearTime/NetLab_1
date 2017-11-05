#include "mytitlebar.h"
#include <QHBoxLayout>
#include <QPainter>
#include <QFile>
#include <QMouseEvent>
#include <QDebug>

#define BUTTON_HEIGHT 30
#define BUTTON_WIDTH 30
#define TITLE_HEIGHT 30

MyTitleBar::MyTitleBar(QWidget *parent) :
    QWidget(parent) ,
    m_colorR(153) ,
    m_colorG(153) ,
    m_colorB(153) ,
    m_isPressed(false) ,
    m_buttonType(MIN_MAX_BUTTON) ,
    m_windowBorderWidth(0) ,
    m_isTransparent(false)
{
    initControl();
    initConnections();
    //loadStyleSheet("MyTitle");
}

MyTitleBar::~MyTitleBar()
{

}

void MyTitleBar::initControl()
{
    m_pIcon = new QLabel;
    m_pTitleContent = new QLabel;

    m_pButtonClose = new QPushButton;
    m_pButtonMax = new QPushButton;
    m_pButtonMin = new QPushButton;
    m_pButtonRestore = new QPushButton;

    m_pButtonMin->setFixedSize(QSize(BUTTON_WIDTH,BUTTON_HEIGHT));
    m_pButtonMax->setFixedSize(QSize(BUTTON_WIDTH,BUTTON_HEIGHT));
    m_pButtonClose->setFixedSize(QSize(BUTTON_WIDTH,BUTTON_HEIGHT));
    m_pButtonRestore->setFixedSize(QSize(BUTTON_WIDTH,BUTTON_HEIGHT));

    //m_pButtonMax->setIcon(QIcon(":/Resources/MyTitle/max.png"));
    //m_pButtonClose->setIcon(QIcon(":/Resources/MyTitle/close.png"));
    //m_pButtonClose->setIconSize(QSize(m_pButtonClose->width()*3,m_pButtonClose->height()));
    m_pButtonMax->setStyleSheet("QPushButton:hover{ border-image:url(:/Resources/MyTitle/max.png) 0 54 0 27 ;}"
                                "QPushButton#ButtonMax{border-image:url(:/Resources/MyTitle/max.png) 0 81 0 0 ;}"
                                "QPushButton:pressed#ButtonMax{border-image:url(:/Resources/MyTitle/max.png) 0 27 0 54 ;}");

    m_pButtonClose->setStyleSheet("QPushButton#ButtonClose{border-image:url(:/Resources/MyTitle/close.png) 0 60 0 0 ;border-top-right-radius:3 ;}"
                                  "QPushButton#ButtonClose:hover{border-image:url(:/Resources/MyTitle/close.png) 0 30 0 30 ;border-top-right-radius:3 ;}"
                                  "QPushButton#ButtonClose:pressed{border-image:url(:/Resources/MyTitle/close.png) 0 0 0 60 ;border-top-right-radius:3 ;}");

    m_pButtonRestore->setStyleSheet("QPushButton#ButtonRestore{border-image:url(:/Resources/MyTitle/restore.png) 0 81 0 0 ;}"
                                    "QPushButton#ButtonRestore:hover{border-image:url(:/Resources/MyTitle/restore.png) 0 54 0 27 ;}"
                                    "QPushButton#ButtonRestore:pressed{border-image:url(:/Resources/MyTitle/restore.png) 0 27 0 54 ;}");

    m_pButtonMin->setStyleSheet("QPushButton#ButtonMin{border-image:url(:/Resources/MyTitle/min.png) 0 60 0 0 ;}"
                                "QPushButton#ButtonMin:hover{border-image:url(:/Resources/MyTitle/min.png) 0 30 0 30 ;}"
                                "QPushButton#ButtonMin:pressed{border-image:url(:/Resources/MyTitle/min.png) 0 0 0 60 ;}");

    m_pTitleContent->setObjectName("TitleContent");
    m_pButtonMin->setObjectName("ButtonMin");
    m_pButtonMax->setObjectName("ButtonMax");
    m_pButtonClose->setObjectName("ButtonClose");
    m_pButtonRestore->setObjectName("ButtonRestore");

    QHBoxLayout *mylayout = new QHBoxLayout(this);
    mylayout->addWidget(m_pIcon);
    mylayout->addWidget(m_pTitleContent);

    mylayout->addWidget(m_pButtonMin);
    mylayout->addWidget(m_pButtonRestore);
    mylayout->addWidget(m_pButtonMax);
    mylayout->addWidget(m_pButtonClose);

    mylayout->setContentsMargins(5,0,0,0);
    mylayout->setSpacing(0);

    m_pTitleContent->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);
    this->setFixedHeight(TITLE_HEIGHT);
    this->setWindowFlags(Qt::FramelessWindowHint);

}

void MyTitleBar::initConnections()
{
    connect(m_pButtonMin,SIGNAL(clicked()),this,SLOT(onButtonMinClicked()));
    connect(m_pButtonRestore,SIGNAL(clicked()),this,SLOT(onButtonRestoreClicked()));
    connect(m_pButtonMax,SIGNAL(clicked()),this,SLOT(onButtonMaxClicked()));
    connect(m_pButtonClose,SIGNAL(clicked()),this,SLOT(onButtonCloseClicked()));
}

void MyTitleBar::setBackgroundColor(int r, int g, int b, bool isTransparent)
{
    m_colorR = r;
    m_colorG = g;
    m_colorB = b;
    m_isTransparent = isTransparent;
    update();
}

void MyTitleBar::setTitleContent(QString titleContent, int titleFontSize)
{
    QFont font = m_pTitleContent->font();
    font.setPointSize(titleFontSize);
    m_pTitleContent->setFont(font);
    m_pTitleContent->setText(titleContent);
    m_pTitleContent->setStyleSheet("font: bold; color: rgba(45, 70, 62,0.8)");
    m_titleContent = titleContent;
}

void MyTitleBar::setTitleWidth(int width)
{
    this->setFixedWidth(width);
}

void MyTitleBar::setButtonType(ButtonType buttontype)
{
    m_buttonType = buttontype;

    switch (buttontype)
    {
    case MIN_BUTTON:{
        m_pButtonRestore->setVisible(false);
        m_pButtonMax->setVisible(false);
    }
        break;
    case MIN_MAX_BUTTON:{
        m_pButtonRestore->setVisible(false);
    }
        break;
    case ONLY_CLOSE_BUTTON:{
        m_pButtonMin->setVisible(false);
        m_pButtonMax->setVisible(false);
        m_pButtonRestore->setVisible(false);
    }
        break;
    default:
        break;
    }
}

void MyTitleBar::setTitleRoll()
{
    connect(&m_titleRollTimer,SIGNAL(timeout()),this,SLOT(onRollTitle()));
    m_titleRollTimer.start(200);
}

void MyTitleBar::setWindowBarderWidth(int borderWidth)
{
    m_windowBorderWidth = borderWidth;
}

void MyTitleBar::saveRestoreInfo(const QPoint point, const QSize size)
{
    m_restorePos = point;
    m_restoreSize = size;
}

void MyTitleBar::getRestoreInfo(QPoint &point, QSize &size)
{
    point = m_restorePos;
    size = m_restoreSize;
}

void MyTitleBar::paintEvent(QPaintEvent *event)
{
    if (!m_isTransparent) {
        QPainter painter(this);
        QPainterPath pathBack;
        pathBack.setFillRule(Qt::WindingFill);
        pathBack.addRoundedRect(QRect(0,0,this->width(),this->height()),3,3);
        painter.fillPath(pathBack,QBrush(QColor(m_colorR,m_colorG,m_colorB)));
    }

    if (this->width() != (this->parentWidget()->width() - m_windowBorderWidth)) {
        this->setFixedWidth(this->parentWidget()->width() - m_windowBorderWidth);
    }
    QWidget::paintEvent(event);
}

void MyTitleBar::mouseDoubleClickEvent(QMouseEvent *event)
{
    if (m_buttonType == MIN_MAX_BUTTON) {
        if (m_pButtonMax->isVisible()) {
            onButtonMaxClicked();
        } else {
            onButtonRestoreClicked();
        }
    }

    return QWidget::mouseDoubleClickEvent(event);
}

void MyTitleBar::mousePressEvent(QMouseEvent *event)
{
    if (m_buttonType == MIN_MAX_BUTTON) {
        if (m_pButtonMax->isVisible()) {
            m_isPressed = true;
            m_startMovePos = event->globalPos();
        }
    } else {
        m_isPressed = true;
        m_startMovePos = event->globalPos();
    }

    return QWidget::mousePressEvent(event);
}

void MyTitleBar::mouseMoveEvent(QMouseEvent *event)
{
    if (m_isPressed) {
        QPoint movePoint = event->globalPos() - m_startMovePos;
        QPoint widgetPos = this->parentWidget()->pos();
        m_startMovePos = event->globalPos();
        this->parentWidget()->move(widgetPos.x() + movePoint.x(), widgetPos.y() + movePoint.y());
    }
    return QWidget::mouseMoveEvent(event);
}

void MyTitleBar::mouseReleaseEvent(QMouseEvent *event)
{
    m_isPressed = false;
    return QWidget::mouseReleaseEvent(event);
}

//从文件中读取样式
void MyTitleBar::loadStyleSheet(const QString &sheetName)
{
    QFile file(":/Rsources/" + sheetName + ".css");
    file.open(QFile::ReadOnly);
    if (file.isOpen()) {
        QString styleSheet = this->styleSheet();
        styleSheet += QLatin1String(file.readAll());
        this->setStyleSheet(styleSheet);
    }
}

void MyTitleBar::onButtonMinClicked()
{
    emit signalButtonMinClicked();
}

void MyTitleBar::onButtonRestoreClicked()
{
    m_pButtonRestore->setVisible(false);
    m_pButtonMax->setVisible(true);
    emit signalButtonRestoreClicked();
}

void MyTitleBar::onButtonMaxClicked()
{
    m_pButtonRestore->setVisible(true);
    m_pButtonMax->setVisible(false);
    emit signalButtonMaxClicked();
}

void MyTitleBar::onButtonCloseClicked()
{
    emit signalButtonCloseClicked();
}

//标题栏滚动
void MyTitleBar::onRollTitle()
{
    static int nPos = 0;
    QString titleContent = m_titleContent;
    if (nPos > titleContent.length()) {
        nPos = 0;
    }
    m_pTitleContent->setText(titleContent.mid(nPos));
    nPos++;
}






















