#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSharedMemory>
#include <QMap>
#include<QDomElement>
#include "PixmapObject.h"
#include "TextObject.h"
class QTimer;
class QLabel;
#define N_GAME_OBJECTS 8192 // have to match tcAllianceSensorMap::MAX_TRACKS and ::MAX_SMTRACKS
#define C_RADTODEG 57.295779513f
#define MTYPE_OBJECT  0
#define MTYPE_SURFACE 1
#define MTYPE_CARRIER 2
#define MTYPE_AIR 3
#define MTYPE_FIXEDWING 4
#define MTYPE_MISSILE 5
#define MTYPE_HELO 6
#define MTYPE_SUBSURFACE 7
#define MTYPE_SUBMARINE 8
#define MTYPE_TORPEDO 9
#define MTYPE_FIXED 10
#define MTYPE_PLATFORM 11
#define MTYPE_FIXEDWINGX 12  // model with more realism
#define MTYPE_AIRFIELD 13
#define MTYPE_BALLISTIC 14
#define MTYPE_SONOBUOY 15
#define MTYPE_AIRCM 16 // air countermeasure model
#define MTYPE_GROUNDVEHICLE 17 // e.g. ground mobile SAM
#define MTYPE_FUELTANK 20
#define MTYPE_LASERGUIDEDBOMB 21
#define MTYPE_WATERCM 22 // water countermeasure model
#define MTYPE_BALLISTICMISSILE 23
#define MTYPE_ROCKET 24

#define MAX_COMMAND_STRING_LEN (1024*100)
namespace Ui {
class MainWindow;
}
class MapGraphicsObject;
class MapGraphicsScene;
class MapGraphicsView ;

class MainWindow : public QMainWindow
{
    Q_OBJECT

    struct UnitInfo{
        long mnID;
        int alliance;
        unsigned int  mnModelType;              ///< class MTYPE_ identifier
        char mzUnit[128];
        char mzClass[128];
        double mfLon_rad;              ///< longitude [rad]
        double mfLat_rad;              ///< latitude [rad]
        float mfAlt_m;                 ///< altitude, negative is subsurface depth [m]
        float mfHeading_rad;           ///< relative to north [rad]
        float mfClimbAngle_rad;        ///< climb angle defines vertical motion vector [rad]
        float mfYaw_rad;               ///< orientation in azimuthal plane
        float mfPitch_rad;             ///< orientation in elevation plane
        float mfRoll_rad;			   ///< orienation about roll axis
        float mfSpeed_kts;             ///< [kts]
    };
    struct SimTime
    {
        double mfSimTime;
        uint64_t dateTime;
    };
    struct SharedSimData
    {
        SimTime simTime;
        unsigned long count;
        UnitInfo unitInfos[N_GAME_OBJECTS];
    };
    struct CommandString
    {
        unsigned int length;
        char str[MAX_COMMAND_STRING_LEN+1];
    };

    struct EnityObject
    {
         PixmapObject *pixmapObject;
         TextObject *textObject;
    };
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    bool ReadSharedMemoryData();
private slots:
    void on_actionExit_triggered();
    void on_actionStart_triggered();
    void on_pushButtonUpdate_clicked();
    void on_pushButtonSend_clicked();
private:
    Ui::MainWindow *ui;
    QSharedMemory sharedSimDataMemory;
    static SharedSimData sharedSimData;
    QSharedMemory commandStringMemory;
    static CommandString commandString;
    QMap<long,EnityObject> graphicsObjects;
    MapGraphicsScene * scene ;
    MapGraphicsView * view ;
    QTimer *timer;
    QPixmap GeneratePixmapFromSvg(const QString& path,QString color,QSize size=QSize(0,0));

    void SetAttrRecur( QDomElement &elem, QString strtagname, QString strattr, QString strattrval);
    QMap<int,QString> allianceColorString;
    QMap<int,QColor> allianceColor;
    QMap<unsigned int,QString> mnModelTypeSymbology;//类型对应图标
    QLabel *simTimeLabel;
    QLabel *datatimeLabel;
    QLabel *positionLabel;

};

#endif // MAINWINDOW_H
