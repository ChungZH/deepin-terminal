#include "tabrenamewidget.h"
#include <QDebug>
#include <QKeyEvent>

#include <DFontSizeManager>
/*******************************************************************************
 1. @函数:    LineEdit
 2. @作者:    ut000442 赵公强
 3. @日期:    2020-10-30
 4. @说明:    输入条构造函数，为了解决键盘交互问题需要重写keyPressEvent
*******************************************************************************/
LineEdit::LineEdit(DLineEdit *parent)
    : DLineEdit(parent)
{

}

/*******************************************************************************
 1. @函数:    keyPressEvent
 2. @作者:    ut000442 赵公强
 3. @日期:    2020-10-30
 4. @说明:    键盘事件，当按下enter键不执行键盘按下事件，否则键盘交互会出现问题
*******************************************************************************/
void LineEdit::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter) {
        return;
    }

    DLineEdit::keyPressEvent(event);
}

/*******************************************************************************
 1. @函数:    TabRenameWidget
 2. @作者:    ut000442 赵公强
 3. @日期:    2020-10-30
 4. @说明:    标签重命名控件构造函数，由输入条与一个内置菜单的按钮组成，isRemote表示是否为远程
              ,isSetting表示是否在设置中调用
*******************************************************************************/
TabRenameWidget::TabRenameWidget(bool isRemote, bool isSetting, QWidget *parent)
    : QWidget(parent)
    , m_isRemote(isRemote)
    , m_isSetting(isSetting)
{
    initChoseMenu();
    initUi();
    initConnections();
}

/*******************************************************************************
 1. @函数:    initUi
 2. @作者:    ut000442 赵公强
 3. @日期:    2020-10-30
 4. @说明:    初始化标签重命名控件的ui
*******************************************************************************/
void TabRenameWidget::initUi()
{
//    setMinimumSize(300, 36);

    m_layout = new QHBoxLayout(this);
    m_layout->setContentsMargins(0, 0, 0, 0);

    m_inputedit = new LineEdit;
    m_inputedit->setText("%n:%d");
    m_inputedit->lineEdit()->setSelection(0, m_inputedit->text().size());
    m_inputedit->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    m_inputedit->setClearButtonEnabled(false);
    DFontSizeManager::instance()->bind(m_inputedit, DFontSizeManager::T6);

    m_choseButton = new DPushButton(tr("Insert"));
    m_choseButton->setMaximumSize(82, 45);
    m_choseButton->setMenu(m_choseMenu);
    m_choseButton->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    DFontSizeManager::instance()->bind(m_choseButton, DFontSizeManager::T6);

    if (!m_isSetting) {
        initLabel();
    }

    m_layout->addWidget(m_inputedit);
    m_layout->addSpacing(10);
    m_layout->addWidget(m_choseButton);
}

/*******************************************************************************
 1. @函数:    initChoseMenu
 2. @作者:    ut000442 赵公强
 3. @日期:    2020-10-30
 4. @说明:    初始化按钮内置的菜单
*******************************************************************************/
void TabRenameWidget::initChoseMenu()
{
    m_choseMenu = new DMenu(this);
    DFontSizeManager::instance()->bind(m_choseMenu, DFontSizeManager::T6);
    if (m_isRemote) {
        initRemoteChoseMenu();
    } else {
        initNormalChoseMenu();
    }
}

/*******************************************************************************
 1. @函数:    initRemoteChoseMenu
 2. @作者:    ut000442 赵公强
 3. @日期:    2020-10-30
 4. @说明:    初始化远程标签按钮内置的菜单
*******************************************************************************/
void TabRenameWidget::initRemoteChoseMenu()
{
    QStringList list;
    list << tr("username: %u") << tr("username@: %U") << tr("remote host: %h")
         << tr("session number: %#") << tr("title set by shell: %w");

    foreach (auto it, list) {
        QAction *ac = new QAction(it, m_choseMenu);
        m_choseMenu->addAction(ac);
    }
}

/*******************************************************************************
 1. @函数:    initNormalChoseMenu
 2. @作者:    ut000442 赵公强
 3. @日期:    2020-10-30
 4. @说明:    初始化普通标签按钮内置的菜单
*******************************************************************************/
void TabRenameWidget::initNormalChoseMenu()
{
    QStringList list;
    list << tr("program name: %n") << tr("current directory (short): %d")
         << tr("current directory (long): %D") << tr("session number: %#")
         << tr("username: %u") << tr("local host: %h")
         << tr("title set by shell: %w");

    foreach (auto it, list) {
        QAction *ac = new QAction(it, m_choseMenu);
        m_choseMenu->addAction(ac);
    }
}

/*******************************************************************************
 1. @函数:    initConnections
 2. @作者:    ut000442 赵公强
 3. @日期:    2020-10-30
 4. @说明:    初始化链接
*******************************************************************************/
void TabRenameWidget::initConnections()
{
    connect(m_choseMenu, &DMenu::triggered, this, [ = ](QAction * ac) {
        QStringList spiltlist = ac->text().split("%");
        m_inputedit->lineEdit()->insert("%" + spiltlist.at(1));
    });
}

/*******************************************************************************
 1. @函数:    initLabel
 2. @作者:    ut000442 赵公强
 3. @日期:    2020-10-30
 4. @说明:    初始化小标题
*******************************************************************************/
void TabRenameWidget::initLabel()
{
    m_Label = new QLabel;
//    m_Label->setMaximumSize(126, 20);
    if (!m_isRemote) {
        m_Label->setText(QObject::tr("Tab title format"));
    } else {
        m_Label->setText(QObject::tr("Remote tab title format"));
    }

    DFontSizeManager::instance()->bind(m_Label, DFontSizeManager::T6);

    m_Label->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    m_layout->addWidget(m_Label);
    m_layout->addSpacing(10);
}

/*******************************************************************************
 1. @函数:    inputedit
 2. @作者:    ut000442 赵公强
 3. @日期:    2020-10-30
 4. @说明:    获取输入条接口
*******************************************************************************/
LineEdit *TabRenameWidget::getInputedit() const
{
    return m_inputedit;
}


