#include "emojiselector.h"
#include "ui_emojiselector.h"

#include <QStandardItemModel>
#include <QTableView>
#include <QHeaderView>
#include <QVBoxLayout>
#include <QPushButton>
#include <QPainter>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QCoreApplication>
#include <QDebug>
#include <QDir>
#include <QTextEdit>


class Emoji {
public:
    // 构造函数
    Emoji() : qcid(0), emojiType(0), aniStickerPackId(0), aniStickerId(0), isHide(false), startTime(0), endTime(0), animationWidth(0), animationHeight(0) {}

    // 从 JSON 对象初始化
    void fromJson(const QJsonObject &json) {
        emojiId = json["emojiId"].toString();
        describe = json["describe"].toString();
        qzoneCode = json["qzoneCode"].toString();
        qcid = json["qcid"].toInt();
        emojiType = json["emojiType"].toInt();
        aniStickerPackId = json["aniStickerPackId"].toInt();
        aniStickerId = json["aniStickerId"].toInt();
        QJsonArray wordsArray = json["associateWords"].toArray();
        for (const QJsonValue &value : wordsArray) {
            associateWords.append(value.toString());
        }
        isHide = json["isHide"].toBool();
        startTime = json["startTime"].toString().toInt();
        endTime = json["endTime"].toString().toInt();
        animationWidth = json["animationWidth"].toInt();
        animationHeight = json["animationHeigh"].toInt(); // 这里用的是 animationHeigh
    }

    // 打印 Emoji 信息（可选）
    void print() const {
        qDebug() << "Emoji ID:" << emojiId;
        qDebug() << "Description:" << describe;
        qDebug() << "Qzone Code:" << qzoneCode;
        qDebug() << "QCID:" << qcid;
        qDebug() << "Emoji Type:" << emojiType;
        qDebug() << "Ani Sticker Pack ID:" << aniStickerPackId;
        qDebug() << "Ani Sticker ID:" << aniStickerId;
        qDebug() << "Associate Words:" << associateWords;
        qDebug() << "Is Hidden:" << isHide;
        qDebug() << "Start Time:" << startTime;
        qDebug() << "End Time:" << endTime;
        qDebug() << "Animation Width:" << animationWidth;
        qDebug() << "Animation Height:" << animationHeight;
    }

public:
    QString emojiId;
    QString describe;
    QString qzoneCode;
    int qcid;
    int emojiType;
    int aniStickerPackId;
    int aniStickerId;
    QStringList associateWords;
    bool isHide;
    int startTime;
    int endTime;
    int animationWidth;
    int animationHeight;
};

EmojiSelector::EmojiSelector(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EmojiSelector)
{
    ui->setupUi(this);
    setWindowTitle("选择表情");
    resize(420, 300);
    // 创建表情模型
    QStandardItemModel *model = new QStandardItemModel(4, 4, this); // 4x4 网格
    setEmojis(model);
    // 创建表格视图
    QTableView *tableView = new QTableView(this);
    tableView->setModel(model);
    tableView->setFixedSize(400, 200);
    tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    tableView->setSelectionBehavior(QAbstractItemView::SelectItems);
    tableView->horizontalHeader()->setVisible(false);
    tableView->verticalHeader()->setVisible(false);

    // 设置图标大小
    tableView->setIconSize(QSize(32, 32));

    tableView->resizeColumnsToContents(); // 根据内容调整列宽
    // 调整每列宽度
    for (int i = 0; i < model->columnCount(); ++i) {
        int currentWidth = tableView->columnWidth(i);
        tableView->setColumnWidth(i, currentWidth - 7); // 调整为更窄的宽度
    }
    // 创建布局
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(tableView);


    connect(tableView, &QTableView::clicked, this, [this, model](const QModelIndex &index) {
        if (index.isValid()) {

            QString emoji = model->item(index.row(), index.column())->data().toString();
            qDebug() << index << emoji;
            // 这里可以处理选择的表情，例如发送信号或保存选择
            QString emojiStr = QString("#0xa") + emoji;
            emit emojiSelected(emoji);
            QDialog::accept();
        }
    });
}

EmojiSelector::~EmojiSelector()
{
    delete ui;
}

int EmojiSelector::explain(QTextEdit *plain, QString context)
{
    QString appPath = QCoreApplication::applicationDirPath();
    QDir appDir(appPath);
    QString file = appDir.absolutePath() + "/default-emojis/" + context + ".png";
    qDebug() << "emojiFile : " << file;
    QTextCursor cursor = plain->textCursor();
    QTextImageFormat format;
    format.setName(file);
    format.setWidth(30);
    format.setHeight(30);

    if (mEmojiPalinMap.count(context)) {
        Emoji *emo = mEmojiPalinMap[context];
        format.setToolTip(context + emo->describe);
    }
    cursor.insertImage(format);
    plain->toPlainText();
    qDebug() << plain->toPlainText();
    //TODO: 这里plain是没法解出的，需要再去考虑
    for (int i = 0; i < 00; i++) {

    }
    return 1;
}


//TODO:要写一些把Json全部读出来，把表情放Tableview里，点击后，把图片放入输入框里，
//在输入框点发送后，将textEdit里读出来，然后编码为QString，或者Qt有提供其他机制。
//发送，然后在把内容拷贝到textEdit2里，注意解码为表情，进行渲染。发送到服务器后就是QString,
//到客户端就要进行解码渲染。
void EmojiSelector::jsonGet() {
    // 获取应用程序的完整路径
    QString appPath = QCoreApplication::applicationDirPath();
    QDir appDir(appPath);
    QString proFileDir = appDir.absolutePath();
    QFile file(proFileDir + "/default-emojis/default_config.json");
    qDebug() << "exe File Directory:" << proFileDir;
    // 打开文件
    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << "Could not open file:" << file.errorString();
        return;
    }

    // 读取文件内容
    QByteArray jsonData = file.readAll();
    file.close();

    // 解析 JSON 数据
    QJsonDocument jsonDoc(QJsonDocument::fromJson(jsonData));
    if (jsonDoc.isNull()) {
        qDebug() << "Failed to create JSON doc.";
        return;
    }

    // 检查 JSON 是否是对象
    if (!jsonDoc.isObject()) {
        qDebug() << "JSON is not an object.";
        return;
    }

    // 获取 JSON 对象
    QJsonObject jsonObj = jsonDoc.object();
    QJsonObject normalPanelResult = jsonObj["normalPanelResult"].toObject();
    {
        QJsonArray SysEmojiGroupList = normalPanelResult["SysEmojiGroupList"].toArray();
        for (int i = 0; i < SysEmojiGroupList.size(); i++) {
            QJsonObject obj1 = SysEmojiGroupList[i].toObject();
            QString arrName = obj1["groupName"].toString();
            QJsonArray arr = obj1["SysEmojiList"].toArray();
            //{"emojiId":"5","describe":"/流泪","qzoneCode":"105","qcid":0,"emojiType":1,"aniStickerPackId":1,"aniStickerId":16,"associateWords":[],"isHide":false,"startTime":"0","endTime":"0","animationWidth":0,"animationHeigh":0}

            // 遍历 JSON 数组
            for (const QJsonValue &value : arr) {
                QJsonObject emojiObj = value.toObject();
                Emoji &emo = *(new Emoji);
                emo.fromJson(emojiObj);
                qDebug() << "arrName:" << arrName << ", emojiId:" << emo.emojiId << ", describe:" << emo.describe;
                mEmojiStructureMap[arrName][emo.emojiId] = &emo;
            }
        }
    }

    {
        QJsonObject redHeartPanelResult = jsonObj["redHeartPanelResult"].toObject();
        QJsonArray SysEmojiGroupList = normalPanelResult["SysEmojiGroupList"].toArray();
        for (int i = 0; i < SysEmojiGroupList.size(); i++) {
            QJsonObject obj1 = SysEmojiGroupList[i].toObject();
            QString arrName = obj1["groupName"].toString();
            QJsonArray arr = obj1["SysEmojiList"].toArray();
            //{"emojiId":"5","describe":"/流泪","qzoneCode":"105","qcid":0,"emojiType":1,"aniStickerPackId":1,"aniStickerId":16,"associateWords":[],"isHide":false,"startTime":"0","endTime":"0","animationWidth":0,"animationHeigh":0}
            // 遍历 JSON 数组
            for (const QJsonValue &value : arr) {
                QJsonObject emojiObj = value.toObject();
                Emoji &emo = *(new Emoji);
                emo.fromJson(emojiObj);
                qDebug() << "arrName:" << arrName << ", emojiId:" << emo.emojiId << ", describe:" << emo.describe;
                mEmojiStructureMap[arrName][emo.emojiId] = &emo;
                mEmojiPalinMap[emo.emojiId] = &emo;
            }
        }
    }
}

void EmojiSelector::setEmojis(QStandardItemModel *model)
{
    QStringList emojis = {
        ":smile:", ":laughing:", ":wink:", ":heart:", ":thumbsup:",
        ":cry:", ":angry:", ":sweat_smile:", ":star_struck:", ":thinking:"
    };
    jsonGet();
    int k = 0;
    for (auto &mapt : mEmojiStructureMap) {
        for (auto &emo : mapt.second) {
            Emoji& emoji = *emo.second;
            QString emojiId = emo.first;
            int id = emojiId.toInt();
            if (id > 0) {
                QString path = ":/emoji/debug/default-emojis/" + QString::number(id) + ".png";
                QPixmap pixmap(path); // 每个图标大小
                QPixmap scaledPixmap = pixmap.scaled(30, 30, Qt::KeepAspectRatio, Qt::SmoothTransformation);
        //        pixmap.fill(Qt::transparent);
        //        QPainter painter(&pixmap);
        //        painter.drawText(pixmap.rect(), Qt::AlignCenter, emojis[i]);
                QStandardItem *item = new QStandardItem();
                item->setIcon(QIcon(pixmap));
                item->setData(emoji.emojiId);
                item->setToolTip(emoji.describe);
                model->setItem(k / 10, k % 10, item); // 设置图标到模型
                k++;
            }
            else {

            }
        }
    }
}
