#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListView>
#include "CustomGraphicsView.h"
#include <customdelegate.h>
#include <QPushButton>
#include <QDebug>
#include "customshapeitem.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);

private slots:
    void onClear();
    void newFile();
    void openFile();
    void onSave();
    void onSaveAs();
    void onLoad();
    void onOldPos(QString data);
    void onNewPos(QString data);
    void onUndoPos(QString data);
    void onRedoPos(QString data);
    void updateResult(const QString &result);
    void zoomIn();
    void zoomOut();
    void zoomToNormal();
    void zoomToFit();
    void addNewPlantTab(int index);
    void addNewPageTab(int index);
    void closePlantTab(int index);
    void closePageTab(int index);
    void createReportSelectedItems();
    void createEmissionReportSelectedItems();
    void exportPageToPdf();
    void exportPageWithResultToPDF();
    void exportPageToEPS();
    void exportPageWithResultToEPS();
    void exportPageToJPEG();
    void exportPageToTIFFColor();
    void exportPageToTIFFBW();
    void exportPlantToPdf();
    void exportPlantWithResultToPDF();
    void exportPlantToEPS();
    void exportPlantWithResultToEPS();
    void printCurrentContent();
    void printAllContent();
    void setUserPreferences();

private:
    void SetupUI();
    void connectUI();
    void createTabs();
    void onItemClicked(int index);
    void createMenus();
    void createActions();
    void createToolbar();
    void setCurrentFile(const QString &fileName);
    void onDrawingModeSelected(int mode);
    void onTransportEquipment(const QModelIndex &index);

    QWidget *centralWidget;
    QTabWidget *tabPlant;
    QTabWidget *tabPage;
    QListView *listView;
    CustomDelegate *delegate;
    CustomGraphicsView *graphicsView;
    QLabel* oldData;
    QLabel* newData;
    QLabel* UndoData;
    QLabel* RedoData;
    QLabel* status;
    QMenu *fileMenu, *editMenu, *alignMenu, *distributeMenu, *viewMenu, *unitsMenu, *runMenu, *operatingModes;
    QMenu *databaseMenu, *reportsMenu, *windowMenu, *helpMenu;
    QAction *newAction;
    QAction *saveAction;
    QAction *saveAsAction;
    QAction *openAction;
    QAction *openControlPanelAction;
    QAction *manageUserAction;
    QAction *printAction;
    QAction *printAllAction;
    QAction *exportAction;
    QAction *pageToPdf;
    QAction *pageWithResultToPDF;
    QAction *pageToEPS;
    QAction *pageWithResultToEPS;
    QAction *pageToJPEG;
    QAction *pageToTIFFColor;
    QAction *pageToTIFFBW;
    QAction *plantToPdf;
    QAction *plantWithResultToPDF;
    QAction *plantToEPS;
    QAction *plantWithResultToEPS;
    QAction *setTitlePrint;
    QAction *setUserPreference;
    QAction *closeAction;
    QAction *loadAction;
    QAction *clearAction;
    QAction *undoAction;
    QAction *redoAction;
    QAction *copyAction;
    QAction *cutAction;
    QAction *pasteAction;
    QAction *deleteAction;
    QAction *selectAllAction;
    QAction *topAction;
    QAction *bottomAction;
    QAction *leftAction;
    QAction *rightAction;
    QAction *verticalByCenter;
    QAction *horizontalByCenter;
    QAction *refreshAction;
    QAction *zoomInAction;
    QAction *zoomToNormalAction;
    QAction *zoomOutAction;
    QAction *zoomToFitAction;
    QAction *drawOrthogonalAction;
    QAction *metricAction;
    QAction *imperialAction;
    QAction *displayWaterAction;
    QAction *displayToolbarAction;
    QAction *displayToolTipsAction;
    QAction *runAction;
    QAction *runStage;
    QAction *runPlant;
    QAction *runSave;
    QAction *turnOffAllRedFlags;
    QAction *openConfigurationAdvisor;
    QAction *addNewOperatingMode;
    QAction *manageOperatingMode;
    QAction *ModeReport;
    QAction *ModeDifferencesReport;
    QAction *deleteAllOperatingMode;
    QAction *whatAreOperatingModes;
    QAction *viewAllCrushers;
    QAction *viewEditSpecifications;
    QAction *viewEditEmissionTables;
    QAction *viewScreenSizeConversionTable;
    QAction *importDataFromPreviousVersion;
    QAction *manageAttachments;
    QAction *createReportForSelectedItems;
    QAction *createReportForAllItemsOnWorksheet;
    QAction *createEmissionReportForSelectedItems;
    QAction *createEmissionReportForAllItemsOnWorksheet;
    QAction *setTitleAndPrintOptions;
    QAction *refreshAll;
    QAction *collapseAll;
    QAction *restoreAll;
    QAction *openHelpSystem;
    QAction *getAggFlowUpdates;
    QAction *requstSupport;
    QAction *sendSuggestion;
    QAction *aggFlowDisclaimer;
    QAction *aggFlowLicense;
    QAction *about;
    QString currentFile;
    qreal zoomFactor;
};

#endif // MAINWINDOW_H
