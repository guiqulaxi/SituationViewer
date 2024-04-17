#include "MainWindow.h"
#include "ui_MainWindow.h"

#include "MapGraphicsView.h"
#include "MapGraphicsScene.h"
#include "tileSources/GridTileSource.h"
#include "tileSources/OSMTileSource.h"
#include "tileSources/CompositeTileSource.h"
#include "guts/CompositeTileSourceConfigurationWidget.h"
#include "PixmapObject.h"
#include "TextObject.h"
#include "PolygonObject.h"
#include "CircleObject.h"
#include <QSharedPointer>
#include <QtDebug>
#include <QThread>
#include <QImage>
#include <QTimer>
#include <QtSvg>
#include <QDomDocument>
MainWindow::SharedSimData MainWindow::sharedSimData;
MainWindow::CommandString MainWindow::commandString;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //Setup the MapGraphics scene and view
    scene = new MapGraphicsScene(this);
    view = new MapGraphicsView(scene,this);

    //The view will be our central widget
    this->setCentralWidget(view);

    //Setup some tile sources
    QSharedPointer<OSMTileSource> osmTiles(new OSMTileSource(OSMTileSource::OSMTiles), &QObject::deleteLater);
    QSharedPointer<GridTileSource> gridTiles(new GridTileSource(), &QObject::deleteLater);
    QSharedPointer<CompositeTileSource> composite(new CompositeTileSource(), &QObject::deleteLater);
    composite->addSourceBottom(osmTiles);
    composite->addSourceTop(gridTiles);
    view->setTileSource(composite);

    //Create a widget in the dock that lets us configure tile source layers
    CompositeTileSourceConfigurationWidget * tileConfigWidget = new CompositeTileSourceConfigurationWidget(composite.toWeakRef(),
                                                                                                           this->ui->dockWidget);
    this->ui->dockWidget->setWidget(tileConfigWidget);
    delete this->ui->dockWidgetContents;

    this->ui->menuWindow->addAction(this->ui->dockWidget->toggleViewAction());
    this->ui->dockWidget->toggleViewAction()->setText("&Layers");

    allianceColorString={{1,"rgb(255, 0, 0)"},
                         {2,"rgb(0, 0, 255)"},
                         {3,"rgb(0, 255, 0)"} };
    allianceColor={{1,QColor(255, 0, 0)},
                   {2,QColor(0, 0, 255)},
                   {3,QColor(0, 255, 0)} };
    mnModelTypeSymbology={
        { MTYPE_OBJECT,"changguidaodan.svg"},
        { MTYPE_SURFACE,"A-huweijian.svg"},
        { MTYPE_CARRIER,"A-hangkongmujian.svg"},
        { MTYPE_AIR,"A-jianjiji.svg"},
        { MTYPE_FIXEDWING,"A-jianjiji.svg"},
        { MTYPE_MISSILE,"changguidaodan.svg"},
        { MTYPE_HELO,"A-zhishengji.svg"},
        { MTYPE_SUBSURFACE,"A-qianting.svg"},
        { MTYPE_SUBMARINE,"A-qianting.svg"},
        { MTYPE_TORPEDO,"yulei-hei-loukong.svg"},
        { MTYPE_FIXED,"sheshi.svg"},
        { MTYPE_PLATFORM,"zhongxingtanke.svg"},
        { MTYPE_FIXEDWINGX,"A-jianjiji.svg"},
        { MTYPE_AIRFIELD,"jichang.svg"},
        { MTYPE_BALLISTIC,"changguidaodan.svg"},
        { MTYPE_SONOBUOY,"fantankedaodanfasheche.svg"},
        { MTYPE_AIRCM,"A-jianjiji.svg"},
        { MTYPE_GROUNDVEHICLE,"zhongxingtanke.svg"},
        { MTYPE_FUELTANK,"zhongxingtanke.svg"},
        { MTYPE_LASERGUIDEDBOMB,"changguidaodan.svg"},
        { MTYPE_WATERCM,"changguidaodan.svg"},
        { MTYPE_BALLISTICMISSILE,"changguidaodan.svg"},
        { MTYPE_ROCKET,"changguidaodan.svg"}
    };

    view->setZoomLevel(6);
    view->centerOn(-3, 66);

    TextObject * textObject = new TextObject("123456",QColor(255,0,0),true);

    scene->addObject(textObject);
    textObject->setLongitude(-3);
    textObject->setLatitude(66);


    QPixmap pix=GeneratePixmapFromSvg("D:/1.learn/3.c++/SituationViewer/SituationViewer/images/A-huweijian.svg",
                                      "red",
                                      QSize(100,100));
    PixmapObject * pixmapObject = new PixmapObject(pix,true);

    scene->addObject(pixmapObject);
    pixmapObject->setLongitude(-3);
    pixmapObject->setLatitude(66);
    pixmapObject->setRotation(180);

    MapGraphicsObject * circle = new CircleObject(5, true, QColor(255, 0, 0, 100));
    scene->addObject(circle);
    circle->setLongitude(-3);
    circle->setLatitude(66);
    //        MapGraphicsObject * circle = new CircleObject(5000, false, QColor(255, 0, 0, 100));
    //        circle->setLatitude(34+0.2*i);
    //        circle->setLongitude(122+0.2*j);
    //        scene->addObject(circle);

    // Create a circle on the map to demonstrate MapGraphicsObject a bit
    // The circle can be clicked/dragged around and should be ~5km in radius
    //    for (int i=0;i<100;i++) {
    //        for (int j=0;j<100;j++) {
    //        MapGraphicsObject * circle = new CircleObject(5000, false, QColor(255, 0, 0, 100));
    //        circle->setLatitude(34+0.2*i);
    //        circle->setLongitude(122+0.2*j);
    //        scene->addObject(circle);
    //        }
    //    }
    sharedSimDataMemory.setKey("SharedSimData");//设置标志一定要与共享内存的标志一样
    commandStringMemory.setKey("CommandString");



    timer=new QTimer(this);
    connect(timer,&QTimer::timeout,[=](){
        if(!ReadSharedMemoryData())return;
        //更新已有的
        for (size_t i=0;i<sharedSimData.count;i++)
        {
            auto mnID=sharedSimData.unitInfos[i].mnID;
            auto alliance=sharedSimData.unitInfos[i].alliance;
            auto mnModelType=sharedSimData.unitInfos[i].mnModelType;
            auto mzUnit=sharedSimData.unitInfos[i].mzUnit;
            //不存在就创建
            if(!graphicsObjects.contains(mnID))
            {
                QPixmap pix=GeneratePixmapFromSvg("D:/1.learn/3.c++/SituationViewer/SituationViewer/images/"+mnModelTypeSymbology[mnModelType],
                                                  allianceColorString[alliance],
                                                  QSize(100,100));
                PixmapObject * pixmapObject = new PixmapObject(pix,true);
                TextObject * textObject = new TextObject(QString(mzUnit),allianceColor[alliance],true);
                //             MapGraphicsObject * circle =new CircleObject(50000, false, QColor(255, 0, 0, 100));
                graphicsObjects[mnID].pixmapObject=pixmapObject;
                graphicsObjects[mnID].textObject=textObject;
                scene->addObject(pixmapObject);
                scene->addObject(textObject);
            }
            auto heading=sharedSimData.unitInfos[i].mfHeading_rad*C_RADTODEG;

            graphicsObjects[mnID].pixmapObject->setRotation(heading);
            graphicsObjects[mnID].pixmapObject->setLatitude(sharedSimData.unitInfos[i].mfLat_rad*C_RADTODEG);
            graphicsObjects[mnID].pixmapObject->setLongitude(sharedSimData.unitInfos[i].mfLon_rad*C_RADTODEG);
            graphicsObjects[mnID].textObject->setLatitude(sharedSimData.unitInfos[i].mfLat_rad*C_RADTODEG);
            graphicsObjects[mnID].textObject->setLongitude(sharedSimData.unitInfos[i].mfLon_rad*C_RADTODEG);
            int hh=sharedSimData.simTime.mfSimTime/3600;
            int min=(sharedSimData.simTime.mfSimTime-hh*3600)/60;
            double sec=(sharedSimData.simTime.mfSimTime-hh*3600-min*60);

            QString simTime=QString("仿真时间：%1:%2:%3").arg(hh,4, 10, QLatin1Char('0')).arg(min,2, 10, QLatin1Char('0')).arg(QString::number(sec, 'g', 4));
            simTimeLabel->setText(simTime);
            auto dateTime=QDateTime::fromMSecsSinceEpoch(sharedSimData.simTime.dateTime);
            datatimeLabel->setText("仿真日期："+dateTime.toString("yyyy-MM-dd hh:mm:ss.zzz"));

        }
        //删除不存在的
        QSet<long> currentmnID;
        for (size_t i=0;i<sharedSimData.count;i++)
        {
            auto mnID=sharedSimData.unitInfos[i].mnID;
            currentmnID.insert(mnID);
        }
         QSet<long> idToDelete;
        for (auto  key:graphicsObjects.keys())
        {
            if(!currentmnID.contains(key))
            {
                idToDelete.insert(key);
            }
        }
        for (auto  key:idToDelete)
        {
            scene->removeObject(graphicsObjects[key].textObject);
             scene->removeObject(graphicsObjects[key].pixmapObject);
            graphicsObjects.remove(key);

        }

    });

    simTimeLabel=new QLabel;
    this->statusBar()->addWidget(simTimeLabel);

    datatimeLabel=new QLabel;
    this->statusBar()->addWidget(datatimeLabel);
    positionLabel=new QLabel;
    this->statusBar()->addWidget(positionLabel);
}

MainWindow::~MainWindow()
{
    delete ui;
}

bool MainWindow::ReadSharedMemoryData()
{
    if(!sharedSimDataMemory.attach(QSharedMemory::ReadOnly))//将shareMemory与该进程绑定使之可以访问shareMemory里的内容
    {
        qDebug()<<tr("can't attach share memory");
        qDebug()<<sharedSimDataMemory.errorString();
        return  false;
    }

    sharedSimDataMemory.lock();//给shareMemory枷锁
    const char *from = (char*)sharedSimDataMemory.data();

    char *to = (char*)&sharedSimData;
    //时间
    memcpy(to,from,sizeof (SimTime));
    //Enity
    from+=sizeof (SimTime);
    to+=sizeof (SimTime);
    memcpy(to,from,sizeof(unsigned long));//数据从该进程中拷贝到共享数据内存中
    from+=sizeof (unsigned long);
    to+=sizeof (unsigned long);

    memcpy(to,from,sharedSimData.count*sizeof(UnitInfo));//数据从该进程中拷贝到共享数据内存中
    sharedSimDataMemory.unlock();
    sharedSimDataMemory.detach();//将shareMemeory与该进程分离
    return true;
}

//private slot
void MainWindow::on_actionExit_triggered()
{
    this->close();

}
void MainWindow::on_actionStart_triggered()
{
    timer->stop();
    scene->clearObject();
    graphicsObjects.clear();
    timer->start(1000);
}
void MainWindow::on_pushButtonUpdate_clicked()
{

}

void MainWindow::on_pushButtonSend_clicked()
{
    if(!commandStringMemory.attach(QSharedMemory::ReadWrite))//将shareMemory与该进程绑定使之可以访问shareMemory里的内容
    {
        qDebug()<<tr("can't attach share memory");
        qDebug()<<commandStringMemory.errorString();
        return;
    }
    commandStringMemory.lock();//给shareMemory枷锁
    unsigned int length = *((unsigned int*)commandStringMemory.data());
    QString command="\r\n"+ui->plainTextEditCommand->toPlainText();
    auto commandByte= command.toUtf8();
    char *to=((char*)commandStringMemory.data());
    unsigned int newlength=length+commandByte.size();//新长度
    if(newlength<=MAX_COMMAND_STRING_LEN)
    {
        memcpy(to,(char*)&newlength,sizeof(unsigned int));
        to+=sizeof(unsigned int)+length;
        memcpy(to,commandByte.data(),commandByte.size());
    }else
    {
        qDebug()<<tr("command is too large");
    }

    commandStringMemory.unlock();
    commandStringMemory.detach();//将shareMemeory与该进程分离

}
void MainWindow::SetAttrRecur( QDomElement &elem, QString strtagname, QString strattr, QString strattrval)
{
    // if it has the tagname then overwritte desired attribute
    if (elem.tagName().compare(strtagname) == 0)
    {
        elem.setAttribute(strattr, strattrval);
    }
    // loop all children
    for (int i = 0; i < elem.childNodes().count(); i++)
    {
        if (!elem.childNodes().at(i).isElement())
        {
            continue;
        }
        auto el= elem.childNodes().at(i).toElement();
        SetAttrRecur(el, strtagname, strattr, strattrval);
    }
}
QPixmap MainWindow::GeneratePixmapFromSvg(const QString &path, QString color, QSize size)
{
    QFile file(path);
    file.open(QIODevice::ReadOnly);
    QByteArray baData = file.readAll();
    file.close();
    QDomDocument doc;
    doc.setContent(baData);
    // recurivelly change color
    auto el=doc.documentElement();
    SetAttrRecur(el, "path", "fill", color);
    // create svg renderer with edited contents
    QSvgRenderer render(doc.toByteArray());
    // create pixmap target (could be a QImage)


    QSize pixSize = render.defaultSize ();

    if(!size.isNull())
        pixSize=size;
    QPixmap pix(pixSize);        // 可以在这儿直接缩放处理
    pix.fill (Qt::transparent); // 像素清空

    QPainter painter(&pix);
    painter.setRenderHints (QPainter::Antialiasing);
    render.render (&painter);
    return pix;
}
