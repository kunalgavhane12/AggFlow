#include "MainWindow.h"
#include <QStringListModel>
#include <QFileDialog>
#include <QHBoxLayout>
#include <QMimeData>
#include <QDrag>
#include <QMenuBar>
#include <QLabel>
#include <QXmlStreamWriter>
#include <QMessageBox>
#include <QStatusBar>
#include <QToolBar>
#include <QGroupBox>
#include <QPushButton>
#include <QColorDialog>
#include <QInputDialog>
#include <QTableView>
#include <QPrinter>
#include <QPrintDialog>
#include "userpreferences.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , centralWidget(new QWidget(this))
    , tabPlant(new QTabWidget(this))
    , tabPage(new QTabWidget(this))
    , listView(new QListView(this))
    , delegate(new CustomDelegate(40, this))
    , graphicsView(new CustomGraphicsView(this))
    , oldData(new QLabel(this))
    , newData(new QLabel(this))
    , UndoData(new QLabel(this))
    , RedoData(new QLabel(this))
    , status(new QLabel(this))
    , currentFile("saveTest.scene")
    , zoomFactor(1.5)
{
    SetupUI();
    createActions();
    createMenus();
    createToolbar();
    connectUI();
    graphicsView->setFixedSizeAndScene(QSize(800, 600));

    runAction = new QAction("Run", this);
    menuBar()->addAction(runAction);
    connect(runAction, &QAction::triggered, graphicsView, &CustomGraphicsView::onResult);

    exportAction = new QAction("Export", this);
    menuBar()->addAction(exportAction);
    connect(exportAction, &QAction::triggered, this, &MainWindow::onSave);

    status->setText("<span style='font-size: 16px; font-weight: bold;'>Result : 0</span>");
    statusBar()->addPermanentWidget(status);


}

void MainWindow::onClear()
{
    graphicsView->ClearScene();
    CustomPixmapItem::GlobalItemId = 0;
    status->setText("Result : 0");
    statusBar()->showMessage(tr("Scene cleared"), 2000);
}

void MainWindow::newFile()
{
    graphicsView->ClearScene();
    setCurrentFile(QString());
}

void MainWindow::openFile()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), "", tr("XML Files (*.xml);;Scene Files(*.scene)"));
    if (fileName.isEmpty())
        return;

    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly))
    {
        QMessageBox::warning(this, tr("Open File"), tr("Cannot open file %1:\n%2.").arg(fileName).arg(file.errorString()));
        return;
    }
    setCurrentFile(fileName);
    onLoad();
}

void MainWindow::SetupUI()
{
    QStringList menulabels = {"Start point", "In line Equipment", "Transport Equipment", "Spiltters and flop_gates",
                              "Crushing Equipment", "Screening Equipment", "Mobile Equipment", "Wash Equipment",
                              "Inventory (temporary storage)", "End Products", "Clean Water Equipment", "Measurement Equipment", "Power Sources and Auxiliary Equipment",
                              "Notes Coloring and Drawing"};
    QList<QIcon> menuIcons;
    menuIcons << QIcon(":/icons/images/start_point.png")
              << QIcon(":/icons/images/in_line_Equipment.png")
              << QIcon(":/icons/images/transport_Equipment.png")
              << QIcon(":/icons/images/spiltters_and_flop_gates.png")
              << QIcon(":/icons/images/crushing_Equipment.png")
              << QIcon(":/icons/images/screening_Equipment.png")
              << QIcon(":/icons/images/mobile_Equipment.png")
              << QIcon(":/icons/images/wash_Equipment.png")
              << QIcon(":/icons/images/inventory_(temporary storage).png")
              << QIcon(":/icons/images/end_Products.png")
              << QIcon(":/icons/images/clean_Water_Equipment.png")
              << QIcon(":/icons/images/measurement_Equipment.png")
              << QIcon(":/icons/images/power_Sources_and_Auxiliary_Equipment.png")
              << QIcon(":/icons/images/notes_Coloring_and_Drawing.png");

    setCentralWidget(centralWidget);
    QGroupBox *groupBox = new QGroupBox();
    QVBoxLayout *groupBoxLayout = new QVBoxLayout(groupBox);

    for (int i = 0; i < menulabels.size(); i++) {
        QPushButton *button = new QPushButton( this);
        button->setIcon(menuIcons[i]);
        button->setIconSize(QSize(35, 35));
        button->setFixedSize(40, 40);
        button->setToolTip(menulabels[i]);
        connect(button, &QPushButton::clicked, [this, i]() { onItemClicked(i); });
        groupBoxLayout->addWidget(button, i, 0);
    }

    groupBoxLayout->addStretch(1);

    IconListModel *model = new IconListModel(this);
    QStringList labels = {"start_points_loader", "start_points_dump_truck", "start_points_excavator",
                          "start_points_bull_dozer", "start_points_dredge", "start_points_generic_material_source",
                          "start_points_start_sugar_pile"};
    QList<QIcon> icons;
    icons = {QIcon(":/icons/dragIcon/start_points_loader.png"),
             QIcon(":/icons/dragIcon/start_points_dump_truck.png"),
             QIcon(":/icons/dragIcon/start_points_excavator.png"),
             QIcon(":/icons/dragIcon/start_points_bull_dozer.png"),
             QIcon(":/icons/dragIcon/start_points_dredge.png"),
             QIcon(":/icons/dragIcon/start_points_generic_material_source.png"),
             QIcon(":/icons/dragIcon/start_points_start_suger_pile.png")};

    model->setData(labels, icons);
    listView->setModel(model);
    listView->setFixedWidth(50);
    listView->setIconSize(QSize(40, 40));
    listView->setItemDelegate(delegate);
    listView->setDragEnabled(true);

    createTabs();

    QHBoxLayout *hlayout = new QHBoxLayout(centralWidget);

    groupBox->setFixedWidth(50);
    groupBoxLayout->setMargin(0);
    hlayout->setMargin(2);
    hlayout->addWidget(groupBox);
    hlayout->addWidget(listView);
    hlayout->addWidget(tabPlant);
    setMinimumSize(800, 700);
    statusBar();
}

void MainWindow::connectUI()
{
    connect(newAction, &QAction::triggered, this, &MainWindow::newFile);
    connect(openAction, &QAction::triggered, this, &MainWindow::openFile);
    connect(saveAction, &QAction::triggered, this, &MainWindow::onSave);
    connect(saveAsAction, &QAction::triggered, this, &MainWindow::onSaveAs);
    connect(loadAction, &QAction::triggered, this, &MainWindow::onLoad);
    connect(clearAction, &QAction::triggered, this, &MainWindow::onClear);
    connect(closeAction, &QAction::triggered, this, &MainWindow::close);
    connect(undoAction, &QAction::triggered, graphicsView, &CustomGraphicsView::UndoTriggered);
    connect(redoAction, &QAction::triggered, graphicsView, &CustomGraphicsView::RedoTriggered);
    connect(zoomInAction, &QAction::triggered, this, &MainWindow::zoomIn);
    connect(zoomOutAction, &QAction::triggered, this, &MainWindow::zoomOut);
    connect(zoomToNormalAction, &QAction::triggered, this, &MainWindow::zoomToNormal);
    connect(zoomToFitAction, &QAction::triggered, this, &MainWindow::zoomToFit);
    connect(delegate, &CustomDelegate::sizeHintChanged, listView, &QListView::doItemsLayout);
    connect(graphicsView, &CustomGraphicsView::PublishOldData, this, &MainWindow::onOldPos);
    connect(graphicsView, &CustomGraphicsView::PublishNewData, this, &MainWindow::onNewPos);
    connect(graphicsView, &CustomGraphicsView::PublishUndoData, this, &MainWindow::onUndoPos);
    connect(graphicsView, &CustomGraphicsView::PublishRedoData, this, &MainWindow::onRedoPos);
    connect(graphicsView, &CustomGraphicsView::resultUpdated, this, &MainWindow::updateResult);
    connect(tabPage, &QTabWidget::currentChanged, this, &MainWindow::addNewPageTab);
    connect(tabPlant, &QTabWidget::currentChanged, this, &MainWindow::addNewPlantTab);
    connect(tabPlant, &QTabWidget::tabCloseRequested, this, &MainWindow::closePlantTab);
    connect(tabPage, &QTabWidget::tabCloseRequested, this, &MainWindow::closePageTab);

    // print
    connect(printAction, &QAction::triggered, this, &MainWindow::printCurrentContent);
    connect(printAllAction, &QAction::triggered, this, &MainWindow::printAllContent);
    connect(setUserPreference, &QAction::triggered, this, &MainWindow::setUserPreferences);
    //export pdf
    connect(pageToPdf, &QAction::triggered, this, &MainWindow::exportPageToPdf);
    connect(pageWithResultToPDF, &QAction::triggered, this, &MainWindow::exportPageWithResultToPDF);
    connect(pageToEPS, &QAction::triggered, this, &MainWindow::exportPageToEPS);
    connect(pageWithResultToEPS, &QAction::triggered, this, &MainWindow::exportPageWithResultToEPS);
    connect(pageToJPEG, &QAction::triggered, this, &MainWindow::exportPageToJPEG);
    connect(pageToTIFFColor, &QAction::triggered, this, &MainWindow::exportPageToTIFFColor);
    connect(pageToTIFFBW, &QAction::triggered, this, &MainWindow::exportPageToTIFFBW);
    connect(plantToPdf, &QAction::triggered, this, &MainWindow::exportPlantToPdf);
    connect(plantWithResultToPDF, &QAction::triggered, this, &MainWindow::exportPlantWithResultToPDF);
    connect(plantToEPS, &QAction::triggered, this, &MainWindow::exportPlantToEPS);
    connect(plantWithResultToEPS, &QAction::triggered, this, &MainWindow::exportPlantWithResultToEPS);

    //Alignment
    connect(topAction, &QAction::triggered, graphicsView, &CustomGraphicsView::onActionTopTriggered);
    connect(bottomAction, &QAction::triggered, graphicsView, &CustomGraphicsView::onActionBottomTriggered);
    connect(leftAction, &QAction::triggered, graphicsView, &CustomGraphicsView::onActionLeftTriggered);
    connect(rightAction, &QAction::triggered, graphicsView , &CustomGraphicsView::onActionRightTriggered);

    //reportConnection triggered
    connect(createReportForSelectedItems, &QAction::triggered, this, &MainWindow::createReportSelectedItems);
    connect(createReportForAllItemsOnWorksheet, &QAction::triggered, this, &MainWindow::createReportSelectedItems);
    connect(createEmissionReportForSelectedItems, &QAction::triggered, this, &MainWindow::createEmissionReportSelectedItems);
    connect(createEmissionReportForAllItemsOnWorksheet, &QAction::triggered, this, &MainWindow::createEmissionReportSelectedItems);
    connect(setTitleAndPrintOptions, &QAction::triggered, this, &MainWindow::openFile);
}

void MainWindow::createTabs()
{
    QWidget *tab1 = new QWidget(this);
    QVBoxLayout *tab1Layout = new QVBoxLayout(tab1);
    tab1Layout->addWidget(tabPage);
    tab1->setLayout(tab1Layout);
    tabPlant->addTab(tab1, "Plant Stage #1");

    QWidget *tab2 = new QWidget(this);
    tabPlant->addTab(tab2, "+ Stage");
    tabPage->addTab(graphicsView, "Page #1");
    tabPage->addTab(new QWidget(this), "+ Page");

    tabPlant->setTabsClosable(true);
    tabPage->setTabsClosable(true);
}

void MainWindow::onItemClicked(int index)
{
    QStringList labels;
    IconListModel *menuModel = new IconListModel(this);
    QList<QIcon> menuIcons;

    switch (index) {
    case 0:
        labels = QStringList {"start_points_loader", "start_points_dump_truck", "start_points_excavator",
                "start_points_bull_dozer", "start_points_dredge", "start_points_generic_material_source",
                "start_points_start_sugar_pile"
    };
        menuIcons = {QIcon(":/icons/dragIcon/start_points_loader.png"),
                     QIcon(":/icons/dragIcon/start_points_dump_truck.png"),
                     QIcon(":/icons/dragIcon/start_points_excavator.png"),
                     QIcon(":/icons/dragIcon/start_points_bull_dozer.png"),
                     QIcon(":/icons/dragIcon/start_points_dredge.png"),
                     QIcon(":/icons/dragIcon/start_points_generic_material_source.png"),
                     QIcon(":/icons/dragIcon/start_points_start_suger_pile.png")};
        break;
    case 1:
        labels = QStringList {"apron_feeder", "feeder", "belt_feeder", "suger_bin","pan_feeder",
                "suger_bin_with_feeder", "dust_collector" };

        menuIcons = {QIcon(":/icons/dragIcon/apron_feeder.png"),
                     QIcon(":/icons/dragIcon/feeder.png"),
                     QIcon(":/icons/dragIcon/belt_feeder.png"),
                     QIcon(":/icons/dragIcon/suger_bin.png"),
                     QIcon(":/icons/dragIcon/pan_feeder.png"),
                     QIcon(":/icons/dragIcon/suger_bin_with_feeder.png"),
                     QIcon(":/icons/dragIcon/dust_collector.png")};
        break;
    case 2:
        labels = QStringList {"place_a_conveyor_in_the_flow", "place_a_reversible_conveyor_in_the_flow",
                "place_a_haul_truck_in_the_flow", "place_a_front_end_loader_in_the_flow",
                "place_a_front_end_loader_in_the_flow","place_a_surge_bin_in_the_flow",
                "bucket_elevator", "screw_conveyor" };

        menuIcons = {QIcon(":/icons/dragIcon/place_a_conveyor_in_the_flow.png"),
                     QIcon(":/icons/dragIcon/place_a_reversible_conveyor_in_the_flow.png"),
                     QIcon(":/icons/dragIcon/place_a_haul_truck_in_the_flow.png"),
                     QIcon(":/icons/dragIcon/place_a_front_end_loader_in_the_flow.png"),
                     QIcon(":/icons/dragIcon/place_a_stock_pile_in_the_flow.png"),
                     QIcon(":/icons/dragIcon/place_a_surge_bin_in_the_flow.png"),
                     QIcon(":/icons/dragIcon/bucket_elevator.png"),
                     QIcon(":/icons/dragIcon/screw_conveyor.png")};
        connect(listView, &QListView::clicked, [this](const QModelIndex &index) {
            onTransportEquipment(index);
        });
        break;
    case 3:
        labels = QStringList {"place_a_splitter_in_the_flow", "place_a_three_way_splitter_in_the_flow",
                "place_a_flop_gate_in_the_flow","place_an_overflow_box_in_the_flow",
                "place_a_finger_gate_in_the_flow"};

        menuIcons = {QIcon(":/icons/dragIcon/place_a_splitter_in_the_flow.png"),
                     QIcon(":/icons/dragIcon/place_a_three_way_splitter_in_the_flow.png"),
                     QIcon(":/icons/dragIcon/place_a_flop_gate_in_the_flow.png"),
                     QIcon(":/icons/dragIcon/place_an_overflow_box_in_the_flow.png"),
                     QIcon(":/icons/dragIcon/place_a_finger_gate_in_the_flow.png")};
        break;
    case 4:
        labels = QStringList {"place_a_jaw_crusher_in_the_flow", "place_a_cone_crusher_in_the_flow",
                "place_an_hsi_crusher_in_the_flow","place_a_vsi_crusher_in_the_flow",
                "place_a_roll_crusher_in_the_flow", "place_a_mill_crusher_in_the_flow" };

        menuIcons = {QIcon(":/icons/dragIcon/place_a_jaw_crusher_in_the_flow.png"),
                     QIcon(":/icons/dragIcon/place_a_cone_crusher_in_the_flow.png"),
                     QIcon(":/icons/dragIcon/place_an_hsi_crusher_in_the_flow.png"),
                     QIcon(":/icons/dragIcon/place_a_vsi_crusher_in_the_flow.png"),
                     QIcon(":/icons/dragIcon/place_a_roll_crusher_in_the_flow.png"),
                     QIcon(":/icons/dragIcon/place_a_mill_crusher_in_the_flow.png")};
        break;
    case 5:
        labels = QStringList {"grizzly_feeder_vibrating_scalper", "1deck", "2deck", "3deck","4deck","5deck",
                "place_a_custom_screen_in_the_flow_trommel_or_banana",
                "place_a_custom_multi_screen_or_split_deck_screen_in_the_flow","air_seperator" };

        menuIcons = {QIcon(":/icons/dragIcon/grizzly_feeder_vibrating_scalper.png"),
                     QIcon(":/icons/dragIcon/1deck.png"),
                     QIcon(":/icons/dragIcon/2deck.png"),
                     QIcon(":/icons/dragIcon/3deck.png"),
                     QIcon(":/icons/dragIcon/4deck.png"),
                     QIcon(":/icons/dragIcon/5deck.png"),
                     QIcon(":/icons/dragIcon/place_a_custom_screen_in_the_flow_trommel_or_banana.png"),
                     QIcon(":/icons/dragIcon/place_a_custom_multi_screen_or_split_deck_screen_in_the_flow.png"),
                     QIcon(":/icons/dragIcon/air_seperator.png")};
        break;
    case 6:
        labels = QStringList {"place_a_mobile_jaw_on_the_worksheet", "place_a_mobile_cone_on_the_worksheet",
                "place_a_mobile_hsi_on_the_worksheet","place_a_mobile_vsi_on_the_worksheet",
                "place_a_mobile_screen_on_the_worksheet", "place_a_mobile_wash_unit_on_the_worksheet",
                "place_a_mobile_conveyor_on_the_worksheet"};
        menuIcons = {QIcon(":/icons/dragIcon/place_a_mobile_jaw_on_the_worksheet.png"),
                     QIcon(":/icons/dragIcon/place_a_mobile_cone_on_the_worksheet.png"),
                     QIcon(":/icons/dragIcon/place_a_mobile_hsi_on_the_worksheet.png"),
                     QIcon(":/icons/dragIcon/place_a_mobile_vsi_on_the_worksheet.png"),
                     QIcon(":/icons/dragIcon/place_a_mobile_screen_on_the_worksheet.png"),
                     QIcon(":/icons/dragIcon/place_a_mobile_wash_unit_on_the_worksheet.png"),
                     QIcon(":/icons/dragIcon/place_a_mobile_conveyor_on_the_worksheet.png")};
        break;
    case 7:
        labels = QStringList {"scrubbing_and_attrition_equipment", "classification_equipment",
                "sand_washing_dewatering","place_an_overflow_box_in_the_flow","mixing_box",
                "place_a_slurry_box_in_the_flow", "slurry_pump", "slurry_valve",
                "water_treatment_or_recycling_recover_water" };
        menuIcons = {QIcon(":/icons/dragIcon/scrubbing_and_attrition_equipment.png"),
                     QIcon(":/icons/dragIcon/classification_equipment.png"),
                     QIcon(":/icons/dragIcon/sand_washing_dewatering.png"),
                     QIcon(":/icons/dragIcon/place_an_overflow_box_in_the_flow.png"),
                     QIcon(":/icons/dragIcon/mixing_box.png"),
                     QIcon(":/icons/dragIcon/place_a_slurry_box_in_the_flow.png"),
                     QIcon(":/icons/dragIcon/slurry_pump.png"),
                     QIcon(":/icons/dragIcon/slurry_valve.png"),
                     QIcon(":/icons/dragIcon/water_treatment_or_recycling_recover_water.png")};
        break;
    case 8:
        labels = QStringList {"inventory_suger_pile_with_feeders", "inventory_suger_bin_with_feeders"};
        menuIcons = {QIcon(":/icons/dragIcon/inventory_suger_pile_with_feeders.png"),
                     QIcon(":/icons/dragIcon/inventory_suger_bin_with_feeders.png")};
        break;
    case 9:
        labels = QStringList {"end_point_product_pile", "end_point_haul_truck",
                "end_point_haul_railway_transport","end_point_haul_water_transport",
                "end_point_product_bin", "end_point_blending_station" };
        menuIcons = {QIcon(":/icons/dragIcon/end_point_product_pile.png"),
                     QIcon(":/icons/dragIcon/end_point_haul_truck.png"),
                     QIcon(":/icons/dragIcon/end_point_haul_railway_transport.png"),
                     QIcon(":/icons/dragIcon/end_point_haul_water_transport.png"),
                     QIcon(":/icons/dragIcon/end_point_product_bin.png"),
                     QIcon(":/icons/dragIcon/end_point_blending_station.png")};
        break;
    case 10:
        labels = QStringList {"clean_water_source", "clean_Watersource", "water_pump","water_splitter",
                "waterSplitter", "water_tank", "water_valve", "water_spray_nozzles" };
        menuIcons = {QIcon(":/icons/dragIcon/clean_water_source.png"),
                     QIcon(":/icons/dragIcon/clean_Watersource.png"),
                     QIcon(":/icons/dragIcon/water_pump.png"),
                     QIcon(":/icons/dragIcon/water_splitter.png"),
                     QIcon(":/icons/dragIcon/waterSplitter.png"),
                     QIcon(":/icons/dragIcon/water_tank.png"),
                     QIcon(":/icons/dragIcon/water_valve.png"),
                     QIcon(":/icons/dragIcon/water_spray_nozzles.png")};
        break;
    case 11:
        labels = QStringList {"sample_bucket" };
        menuIcons = {QIcon(":/icons/dragIcon/sample_bucket.png")};
        break;
    case 12:
        labels = QStringList {"external_power_source", "internal_power_source", "fuel_tank","personnel"};

        menuIcons = {QIcon(":/icons/dragIcon/external_power_source.png"),
                     QIcon(":/icons/dragIcon/internal_power_source.png"),
                     QIcon(":/icons/dragIcon/fuel_tank.png"),
                     QIcon(":/icons/dragIcon/personnel.png")};
        break;
    case 13:
        labels = QStringList {"reset_to_pointer_default_mouse_function", "paint_brush",
                "select_color_for_paint_brush","erase_color", "multi_line_notes",
                "adjustable_text", "arrow", "line", "polyline", "ellipse", "rectangle",
                "symbols_open_to_select_model"};
        menuIcons = {QIcon(":/icons/dragIcon/reset_to_pointer_default_mouse_function.png"),
                     QIcon(":/icons/dragIcon/paint_brush.png"),
                     QIcon(":/icons/dragIcon/select_color_for_paint_brush.png"),
                     QIcon(":/icons/dragIcon/erase_color.png"),
                     QIcon(":/icons/dragIcon/multi_line_notes.png"),
                     QIcon(":/icons/dragIcon/adjustable_text.png"),
                     QIcon(":/icons/dragIcon/arrow.png"),
                     QIcon(":/icons/dragIcon/line.png"),
                     QIcon(":/icons/dragIcon/polyline.png"),
                     QIcon(":/icons/dragIcon/ellipse.png"),
                     QIcon(":/icons/dragIcon/rectangle.png"),
                     QIcon(":/icons/dragIcon/symbols_open_to_select_model.png")};
        connect(listView, &QListView::clicked, [this](const QModelIndex &index) {
            onDrawingModeSelected(index.row());
        });
        break;
    default:
        menuIcons = {QIcon()};
        break;
    }

//    for (int i = 0 ; i<menuIcons.size(); i++ )
//    {
//        labels.append("Item " + QString::number(i));
//    }

    menuModel->setData(labels, menuIcons);
    listView->setModel(menuModel);
    listView->setIconSize(QSize(40, 40));
    listView->setItemDelegate(delegate);
    listView->setDragEnabled(true);
    listView->setFixedWidth(50);
}

void MainWindow::createMenus()
{
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(saveAction);
    fileMenu->addAction(saveAsAction);
    fileMenu->addAction(newAction);
    fileMenu->addSeparator();
    fileMenu->addAction(openAction);
    fileMenu->addSeparator();
    fileMenu->addAction(loadAction);
    fileMenu->addAction(clearAction);
    fileMenu->addSeparator();
    fileMenu->addAction(openControlPanelAction);
    fileMenu->addAction(manageUserAction);
    fileMenu->addSeparator();
    fileMenu->addAction(printAction);
    fileMenu->addAction(printAllAction);
    QMenu *exportMenu = fileMenu->addMenu("Export to PDF, JPEG or TIff");
    exportMenu->setIcon(QIcon(":/icons/images/exports.png"));
    exportMenu->addAction(pageToPdf);
    exportMenu->addAction(pageWithResultToPDF);
    exportMenu->addAction(pageToEPS);
    exportMenu->addAction(pageWithResultToEPS);
    exportMenu->addAction(pageToJPEG);
    exportMenu->addAction(pageToTIFFColor);
    exportMenu->addAction(pageToTIFFBW);
    exportMenu->addSeparator();
    exportMenu->addAction(plantToPdf);
    exportMenu->addAction(plantWithResultToPDF);
    exportMenu->addAction(plantToEPS);
    exportMenu->addAction(plantWithResultToEPS);
    fileMenu->addSeparator();
    fileMenu->addAction(setTitlePrint);
    fileMenu->addAction(setUserPreference);
    fileMenu->addSeparator();
    fileMenu->addAction(closeAction);

    editMenu = menuBar()->addMenu(tr("&Edit"));
    editMenu->addAction(undoAction);
    editMenu->addAction(redoAction);
    editMenu->addSeparator();
    editMenu->addAction(copyAction);
    editMenu->addAction(cutAction);
    editMenu->addAction(pasteAction);
    editMenu->addAction(deleteAction);
    editMenu->addSeparator();
    editMenu->addAction(selectAllAction);
    editMenu->addSeparator();

    alignMenu = editMenu->addMenu(tr("Align"));
    alignMenu->setIcon(QIcon(":/icons/images/algin.png"));
    alignMenu->addAction(topAction);
    alignMenu->addAction(bottomAction);
    alignMenu->addAction(leftAction);
    alignMenu->addAction(rightAction);

    distributeMenu = editMenu->addMenu(tr("Distribute"));
    distributeMenu->setIcon(QIcon(":/icons/images/distribute.png"));
    distributeMenu->addAction(verticalByCenter);
    distributeMenu->addAction(horizontalByCenter);

    viewMenu = menuBar()->addMenu(tr("&View"));
    viewMenu->addAction(refreshAction);
    viewMenu->addSeparator();
    viewMenu->addAction(zoomInAction);
    viewMenu->addAction(zoomOutAction);
    viewMenu->addAction(zoomToNormalAction);
    viewMenu->addAction(zoomToFitAction);
    viewMenu->addSeparator();

    unitsMenu = viewMenu->addMenu(tr("Units"));
    unitsMenu->setIcon(QIcon(":/icons/images/units.png"));
    unitsMenu->addAction(metricAction);
    unitsMenu->addAction(imperialAction);
    viewMenu->addSeparator();
    viewMenu->addAction(displayWaterAction);
    viewMenu->addAction(displayToolbarAction);
    viewMenu->addSeparator();
    viewMenu->addAction(displayToolTipsAction);

    runMenu = menuBar()->addMenu(tr("&Run"));
    runMenu->addAction(runStage);
    runMenu->addSeparator();
    runMenu->addAction(runPlant);
    runMenu->addSeparator();
    runMenu->addAction(turnOffAllRedFlags);
    runMenu->addAction(openConfigurationAdvisor);

    operatingModes = menuBar()->addMenu(tr("Operating Modes"));
    operatingModes->addAction(addNewOperatingMode);
    operatingModes->addSeparator();
    operatingModes->addAction(manageOperatingMode);
    operatingModes->addSeparator();
    operatingModes->addAction(ModeReport);
    operatingModes->addSeparator();
    operatingModes->addAction(ModeDifferencesReport);
    operatingModes->addSeparator();
    operatingModes->addAction(deleteAllOperatingMode);
    operatingModes->addSeparator();
    operatingModes->addAction(whatAreOperatingModes);

    databaseMenu = menuBar()->addMenu(tr("&Database"));
    databaseMenu->addAction(viewAllCrushers);
    databaseMenu->addAction(viewEditSpecifications);
    databaseMenu->addAction(viewEditEmissionTables);
    databaseMenu->addSeparator();
    databaseMenu->addAction(viewScreenSizeConversionTable);
    databaseMenu->addAction(importDataFromPreviousVersion);
    databaseMenu->addSeparator();
    databaseMenu->addAction(manageAttachments);

    reportsMenu = menuBar()->addMenu(tr("Re&ports"));
    reportsMenu->addAction(createReportForSelectedItems);
    reportsMenu->addAction(createReportForAllItemsOnWorksheet);
    reportsMenu->addSeparator();
    reportsMenu->addAction(createEmissionReportForSelectedItems);
    reportsMenu->addAction(createEmissionReportForAllItemsOnWorksheet);
    reportsMenu->addSeparator();
    reportsMenu->addAction(setTitleAndPrintOptions);

    windowMenu = menuBar()->addMenu(tr("&Window"));
    windowMenu->addAction(refreshAll);
    windowMenu->addAction(collapseAll);
    windowMenu->addAction(restoreAll);

    helpMenu = menuBar()->addMenu(tr("&Help"));
    helpMenu->addAction(openHelpSystem);
    helpMenu->addSeparator();
    helpMenu->addAction(getAggFlowUpdates);
    helpMenu->addSeparator();
    helpMenu->addAction(requstSupport);
    helpMenu->addSeparator();
    helpMenu->addAction(sendSuggestion);
    helpMenu->addSeparator();
    helpMenu->addAction(aggFlowDisclaimer);
    helpMenu->addAction(aggFlowLicense);
    helpMenu->addSeparator();
    helpMenu->addAction(about);
}

void MainWindow::createActions()
{
    saveAction = new QAction(tr("&Save Revision"), this);
    saveAction->setShortcuts(QKeySequence::Save);
    saveAction->setStatusTip(tr("Ctrl+S"));
    saveAction->setIcon(QIcon(":/icons/images/save_revision.png"));

    saveAsAction = new QAction(tr("&Save as Master"), this);
    saveAsAction->setShortcuts(QKeySequence::SaveAs);
    saveAsAction->setShortcut(tr("Ctrl+Shift+S"));
    saveAsAction->setIcon(QIcon(":/icons/images/save_as_master.png"));

    newAction = new QAction(tr("&New"), this);
    newAction->setShortcuts(QKeySequence::New);
    newAction->setStatusTip(tr("Ctrl+N"));
    newAction->setIcon(QIcon(":/icons/images/add_file.png"));

    openAction = new QAction(tr("&Open Revision From File"), this);
    openAction->setShortcuts(QKeySequence::Open);
    openAction->setStatusTip(tr("Ctrl+O"));
    openAction->setIcon(QIcon(":/icons/images/open_revision_file.png"));

    openControlPanelAction = new QAction(tr("Open Control Panel"), this);
    openControlPanelAction->setIcon(QIcon(":/icons/images/open_control_panel.png"));

    manageUserAction = new QAction(tr("Manage User Access To This Project"));
    manageUserAction->setIcon(QIcon(":/icons/images/manage_user_access.png"));

    printAction = new QAction(tr("Print this Page"), this);
    printAction->setIcon(QIcon(":/icons/images/print_page.png"));

    printAllAction = new QAction(tr("Print All Page..."), this);
    printAllAction->setIcon(QIcon(":/icons/images/print_all_page.png"));
    printAllAction->setIcon(QIcon(":/icons/images/print_all_page.png"));

    pageToPdf = new QAction(tr("Page to PDF"), this);
    pageWithResultToPDF = new QAction(tr("Page with Result to PDF"), this);
    pageToEPS = new QAction(tr("Page to EPS"), this);
    pageWithResultToEPS = new QAction(tr("Page with Result to EPS"), this);
    pageToJPEG = new QAction(tr("Page to JPEG"), this);
    pageToTIFFColor = new QAction(tr("Page to TIFF Color(big)"), this);
    pageToTIFFBW = new QAction(tr("Page to TIFF B/W (small)"), this);
    plantToPdf = new QAction(tr("Plant to PDF"), this);
    plantWithResultToPDF = new QAction(tr("Plant with Result to PDF"), this);
    plantToEPS = new QAction(tr("Plant to EPS"), this);
    plantWithResultToEPS = new QAction(tr("Plant with Result to EPS"), this);

    setTitlePrint = new QAction(tr("Set Title and Print Options"), this);
    setTitlePrint->setIcon(QIcon(":/icons/images/setTitle_print_option.png"));

    setUserPreference = new QAction(tr("Set User Preferences..."), this);
    setUserPreference->setIcon(QIcon(":/icons/images/setuser_preference.png"));

    closeAction = new QAction(tr("&Close This Project"), this);
    closeAction->setShortcuts(QKeySequence::Quit);
    closeAction->setStatusTip(tr("Quit"));
    closeAction->setIcon(QIcon(":/icons/images/close.png"));

    loadAction = new QAction(tr("&Load"), this);
    loadAction->setShortcuts(QKeySequence::Open);
    loadAction->setStatusTip(tr("Ctrl+O"));

    clearAction = new QAction(tr("&Clear"), this);
    clearAction->setStatusTip(tr("Clear"));
    clearAction->setShortcut(tr("Ctrl+L"));

    undoAction = new QAction(tr("&Graphical Undo"), this);
    undoAction->setShortcut(tr("Ctrl+Z"));
    undoAction->setStatusTip(tr("Undo last operation"));
    undoAction->setIcon(QIcon(":/icons/images/graphicalundo.png"));

    redoAction = new QAction(tr("&Graphical Redo"), this);
    redoAction->setShortcut(tr("Ctrl+Shift+Z"));
    redoAction->setStatusTip(tr("Redo last operation"));
    redoAction->setIcon(QIcon(":/icons/images/graphicalredo.png"));

    copyAction = new QAction(tr("&Copy"), this);
    copyAction->setShortcut(tr("Ctrl+C"));
    copyAction->setIcon(QIcon(":/icons/images/copy.png"));

    cutAction = new QAction(tr("&Cut"), this);
    cutAction->setShortcut(tr("Ctrl+X"));
    cutAction->setIcon(QIcon(":/icons/images/cut.png"));

    pasteAction = new QAction(tr("&Paste"), this);
    pasteAction->setShortcut(tr("Ctrl+V"));
    pasteAction->setIcon(QIcon(":/icons/images/paste.png"));

    deleteAction = new QAction(tr("&Delete"), this);
    deleteAction->setShortcut(tr("Del"));
    deleteAction->setIcon(QIcon(":/icons/images/delete.png"));

    selectAllAction = new QAction(tr("&Select All"), this);
    selectAllAction->setShortcut(tr("Ctrl+A"));
    selectAllAction->setIcon(QIcon(":/icons/images/selectall.png"));

    topAction = new QAction(tr("&Top"), this);
    topAction->setIcon(QIcon(":/icons/images/top.png"));
    bottomAction = new QAction(tr("&Bottom"), this);
    bottomAction->setIcon(QIcon(":/icons/images/bottom.png"));
    leftAction = new QAction(tr("&Left"), this);
    leftAction->setIcon(QIcon(":/icons/images/left.png"));
    rightAction = new QAction(tr("&Right"), this);
    rightAction->setIcon(QIcon(":/icons/images/right.png"));

    verticalByCenter = new QAction(tr("&Vertical by Center"), this);
    verticalByCenter->setIcon(QIcon(":/icons/images/verticalbycenter.png"));
    horizontalByCenter = new QAction(tr("&Horizontal by Center"), this);
    horizontalByCenter->setIcon(QIcon(":/icons/images/horizontalbycenter.png"));

    refreshAction = new QAction(tr("&Refresh"), this);
    refreshAction->setIcon(QIcon(":/icons/images/refresh.png"));
    zoomInAction = new QAction(tr("Zoom &In"), this);
    zoomInAction->setStatusTip(tr("Zoom In"));
    zoomInAction->setIcon(QIcon(":/icons/images/zoomIn.png"));

    zoomOutAction = new QAction(tr("Zoom &Out"), this);
    zoomOutAction->setStatusTip(tr("Zoom Out"));
    zoomOutAction->setIcon(QIcon(":/icons/images/zoomout.png"));

    zoomToNormalAction = new QAction(tr("Zoom to &Normal"), this);
    zoomToNormalAction->setStatusTip(tr("Zoom to Fit"));
    zoomToNormalAction->setIcon(QIcon(":/icons/images/zoomnormal.png"));

    zoomToFitAction = new QAction(tr("Zoom to &Fit"), this);
    zoomToFitAction->setStatusTip(tr("Zoom to Fit"));
    zoomToFitAction->setIcon(QIcon(":/icons/images/zoom_to_fit.PNG"));

    drawOrthogonalAction = new QAction(tr("Draw O&rthogonal Flow Streams"), this);
    drawOrthogonalAction->setShortcut(tr("F3"));
    drawOrthogonalAction->setIcon(QIcon(":/icons/images/draw_orthogonal.png"));

    metricAction = new QAction(tr("Metric"), this);
    imperialAction = new QAction(tr("Imperial"), this);

    displayWaterAction = new QAction(tr("Display Clean Water Equipment"), this);
    displayWaterAction->setIcon(QIcon(":/icons/images/displayCleanWater.png"));

    displayToolbarAction = new QAction(tr("Display Toolbar"), this);
    displayToolbarAction->setIcon(QIcon(":/icons/images/displayToolbar.png"));

    displayToolTipsAction = new QAction(tr("Display Tool Tips"), this);
    displayToolTipsAction->setIcon(QIcon(":/icons/images/displayToolbarTip.png"));

    runStage = new QAction(tr("Run Stage"), this);
    runStage->setShortcut(tr("Ctrl+F5"));
    runStage->setIcon(QIcon(":/icons/images/runStage.png"));

    runPlant = new QAction(tr("&Run Plant"), this);
    runPlant->setShortcut(tr("F5"));
    runPlant->setIcon(QIcon(":/icons/images/runPlant.png"));

    runSave = new QAction(tr("Re-Run and Save All Operating Modes"));
    runSave->setIcon(QIcon(":/icons/images/run_save.png"));

    turnOffAllRedFlags = new QAction(tr("Turn Off All Red Flags"), this);
    turnOffAllRedFlags->setIcon(QIcon(":/icons/images/turnoffallredflag.png"));

    openConfigurationAdvisor = new QAction(tr("&Open Configuration Advisor"), this);
    openConfigurationAdvisor->setIcon(QIcon(":/icons/images/openConfigurationAdvisor.png"));

    addNewOperatingMode = new QAction(tr("Add New Operating Mode"), this);
    manageOperatingMode = new QAction(tr("Manage Operating Mode"), this);
    ModeReport = new QAction(tr("Mode Report"), this);
    ModeDifferencesReport = new QAction(tr("Mode Differences Report"), this);
    deleteAllOperatingMode = new QAction(tr("Delete All Operating Mode"), this);
    whatAreOperatingModes = new QAction(tr("What are Operating Modes"), this);

    viewAllCrushers = new QAction(tr("View All Crushers"), this);
    viewAllCrushers->setIcon(QIcon(":/icons/images/viewallcrusher.png"));
    viewEditSpecifications = new QAction(tr("View/Edit Specifications..."), this);
    viewEditSpecifications->setIcon(QIcon(":/icons/images/view_edit_specifications.png"));
    viewEditEmissionTables = new QAction(tr("View/Edit Emission Tables..."), this);
    viewEditEmissionTables->setIcon(QIcon(":/icons/images/view_edit_emission_table.png"));
    viewScreenSizeConversionTable = new QAction(tr("View Screen Size Conversion &Table..."), this);
    viewScreenSizeConversionTable->setIcon(QIcon(":/icons/images/view_screen_size.png"));
    importDataFromPreviousVersion = new QAction(tr("Import Data From Previous Version"), this);
    importDataFromPreviousVersion->setIcon(QIcon(":/icons/images/import_data_previous.png"));
    manageAttachments = new QAction(tr("Manage Attachments"), this);
    manageAttachments->setIcon(QIcon(":/icons/images/manage_attchements.png"));

    createReportForSelectedItems = new QAction(tr("Create Report for Selected Item(s)"), this);
    createReportForSelectedItems->setIcon(QIcon(":/icons/images/reportSelectItem.png"));
    createReportForAllItemsOnWorksheet = new QAction(tr("Create Report For &All Items on Worksheet"), this);
    createReportForAllItemsOnWorksheet->setIcon(QIcon(":/icons/images/reportallItem.png"));
    createEmissionReportForSelectedItems = new QAction(tr("Create &Emission Report for Selected Item(s)"), this);
    createEmissionReportForSelectedItems->setIcon(QIcon(":/icons/images/reportEmission.png"));
    createEmissionReportForAllItemsOnWorksheet = new QAction(tr("Create Emission Report for All Items on the Worksheet"), this);
    createEmissionReportForAllItemsOnWorksheet->setIcon(QIcon(":/icons/images/reportEmissionall.png"));
    setTitleAndPrintOptions = new QAction(tr("Set Title and Print Options"), this);
    setTitleAndPrintOptions->setIcon(QIcon(":/icons/images/setTitle_print_option.png"));

    refreshAll = new QAction(tr("Re&fresh All"), this);
    collapseAll = new QAction(tr("&Collapse All"), this);
    restoreAll = new QAction(tr("&Restore All"), this);

    openHelpSystem = new QAction(tr("Open Help System"), this);
    openHelpSystem->setIcon(QIcon(":/icons/images/openHelp.png"));
    getAggFlowUpdates = new QAction(tr("Get AggFlow Updates"), this);
    getAggFlowUpdates->setIcon(QIcon(":/icons/images/getUpdate.png"));
    requstSupport = new QAction(tr("Request Support"), this);
    requstSupport->setIcon(QIcon(":/icons/images/requestSupport.png"));
    sendSuggestion = new QAction(tr("Send &Suggestions To AggFlow"), this);
    sendSuggestion->setIcon(QIcon(":/icons/images/sendSuggestion.png"));
    aggFlowDisclaimer = new QAction(tr("AggFlow Disclaimer"), this);
    aggFlowDisclaimer->setIcon(QIcon(":/icons/images/disclaimer.png"));
    aggFlowLicense = new QAction(tr("AggFlow License Agreement"), this);
    aggFlowLicense->setIcon(QIcon(":/icons/images/liencseAggrement.png"));
    about = new QAction(tr("&About"), this);
    about->setIcon(QIcon(":/icons/images/about.png"));
}

void MainWindow::createToolbar()
{
    QToolBar *editToolBar;
    editToolBar = addToolBar(tr("Edit"));
    editToolBar->addAction(saveAction);
    editToolBar->addSeparator();
    editToolBar->addAction(openControlPanelAction);
    editToolBar->addSeparator();
    editToolBar->addAction(runStage);
    editToolBar->addAction(runPlant);
    editToolBar->addAction(runSave);
    editToolBar->addSeparator();
    editToolBar->addAction(turnOffAllRedFlags);
    editToolBar->addSeparator();
    editToolBar->addAction(undoAction);
    editToolBar->addSeparator();
    editToolBar->addAction(redoAction);
    editToolBar->addSeparator();
    editToolBar->addAction(zoomInAction);
    editToolBar->addAction(zoomOutAction);
    editToolBar->addAction(zoomToNormalAction);
    editToolBar->addSeparator();
    editToolBar->addAction(zoomToFitAction);
    editToolBar->addAction(drawOrthogonalAction);
    editToolBar->addSeparator();
    editToolBar->addAction(displayWaterAction);
    editToolBar->addSeparator();
    editToolBar->addAction(openHelpSystem);
    removeToolBar(editToolBar);
    addToolBar(Qt::TopToolBarArea, editToolBar);
    editToolBar->setMovable(false);
    editToolBar->show();
}

void MainWindow::setCurrentFile(const QString &fileName)
{
    currentFile = fileName;
    setWindowModified(false);
    setWindowFilePath(currentFile.isEmpty() ? tr("untitled.xml") : currentFile);
}

void MainWindow::onDrawingModeSelected(int mode) {
    listView->setDragEnabled(false);
    switch (mode) {
    case 0:
        qDebug() << "default mouse function";
        break;
    case 1:
        //set paint brush color for change item b/w to color in graphicview
        qDebug() << "paint brush";
        break;
    case 2:
        //choose color for brush for change item color in graphicview
        QColorDialog::getColor();
        qDebug() << "brush paint color";
        break;
    case 3:
        //set paint brush B/W for change item color to b/w in graphicview
        qDebug() << "erase color";
        break;
    case 4:
        //open dialog box and take multi line text input in that and paste in graphicview
        qDebug() << "Multiline note";
        break;
    case 5:
        //open dialog box and take signle line text input in that and paste in graphicview
        qDebug() << "adjustable text ";
        break;
    case 6:
        graphicsView->setShapeType(CustomShapeItem::Arrow);
        break;
    case 7:
        graphicsView->setShapeType(CustomShapeItem::Line);
        break;
    case 8:
        graphicsView->setShapeType(CustomShapeItem::PolygonLine);
        break;
    case 9:
        graphicsView->setShapeType(CustomShapeItem::Ellipse);
        break;
    case 10:
        graphicsView->setShapeType(CustomShapeItem::Rectangle);
        break;
    default:
        listView->setDragEnabled(true);
        break;
    }
}

void MainWindow::onTransportEquipment(const QModelIndex &index)
{
    switch (index.row())
    {
    case 0:
        graphicsView->setShapeType(CustomShapeItem::ConvLine);
        break;
    case 1:
        graphicsView->setShapeType(CustomShapeItem::ConvReverseLine);
        break;
    default:
        listView->setDragEnabled(true);
        break;

    }
}

void MainWindow::onSave()
{
    //    QString fileName = "saveTest.scene";//QFileDialog::getSaveFileName(this, tr("Save File"), "", tr("Scene Files (*.scene)"));
    //    if (!fileName.isEmpty()) {
    //        graphicsView->saveToFile(fileName);
    //    }

    QString file = "saveTest.xml";
    if (!file.isEmpty()) {
        graphicsView->saveToXml(file);
    }
}

void MainWindow::onSaveAs()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save As File"), "", tr("XML Files (*.xml);;Scene Files(*.scene)"));
    if (fileName.isEmpty())
        return;
    setCurrentFile(fileName);
    onSave();
}

void MainWindow::onLoad()
{
    //    graphicsView->ClearScene();
    //    QString fileName = "saveTest.scene";//QFileDialog::getOpenFileName(this, tr("Load File"), "", tr("Scene Files (*.scene)"));
    //    if (!fileName.isEmpty()) {
    //        graphicsView->loadFromFile(fileName);
    //    }
    QString file = "saveTest.xml";
    if (!file.isEmpty()) {
        graphicsView->loadFromXml(file);
    }
}

void MainWindow::onOldPos(QString data)
{
    oldData->setText(data);
}

void MainWindow::onNewPos(QString data)
{
    newData->setText(data);
}

void MainWindow::onUndoPos(QString data)
{
    UndoData->setText(data);
}

void MainWindow::onRedoPos(QString data)
{
    RedoData->setText(data);
}

void MainWindow::updateResult(const QString &result)
{
    status->setText("<span style='font-size: 16px; font-weight: bold;'>Result : " + result + "</span>");
}

void MainWindow::zoomIn()
{
    zoomFactor *= 1.5;
    graphicsView->scale(1.5, 1.5);
}

void MainWindow::zoomOut()
{
    zoomFactor /= 1.5;
    graphicsView->scale(1.0 / 1.5, 1.0 / 1.5);
}

void MainWindow::zoomToNormal()
{
    graphicsView->resetTransform();
}

void MainWindow::zoomToFit()
{
    graphicsView->fitInView(graphicsView->sceneRect(), Qt::KeepAspectRatio);
    zoomFactor = 1.5;
}

void MainWindow::addNewPlantTab(int index)
{
    if (index == tabPlant->count() - 1) {
        QWidget *newStage = new QWidget(this);
        QVBoxLayout *layout = new QVBoxLayout(newStage);
        newStage->setLayout(layout);

        QTabWidget *newPageTabWidget = new QTabWidget(this);
        layout->addWidget(newPageTabWidget);

        tabPlant->insertTab(tabPlant->count() - 1, newStage, tr("Plant Stage #%1").arg(tabPlant->count()));
        tabPlant->setCurrentIndex(tabPlant->count() - 2);

        CustomGraphicsView *newGraphicsView = new CustomGraphicsView(this);
        newGraphicsView->setFixedSizeAndScene(QSize(800, 600));  // Set fixed size
        newPageTabWidget->addTab(newGraphicsView, "Page #1");
        newPageTabWidget->addTab(new QWidget(this), "+ Page");
        newPageTabWidget->setTabsClosable(true);
    }
}

void MainWindow::addNewPageTab(int index)
{
    if (index == tabPage->count() - 1) {
        CustomGraphicsView *newGraphicsView = new CustomGraphicsView(this);
        newGraphicsView->setFixedSizeAndScene(QSize(800, 600));  // Set fixed size
        tabPage->insertTab(tabPage->count() - 1, newGraphicsView, tr("Page #%1").arg(tabPage->count()));
        tabPage->setCurrentIndex(tabPage->count() - 2);
    }
}

void MainWindow::closePlantTab(int index)
{
    if (index <= 0 || index >= tabPlant->count() - 1) {
        qDebug() << "Cannot close tab at index" << index;
        return;
    }

    QMessageBox::StandardButton reply = QMessageBox::question(this, "Confirm Close",
                                                              "Do you want to save changes before closing?",
                                                              QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);

    if (reply == QMessageBox::Cancel) {
        return;
    }

    if (reply == QMessageBox::Yes) {
        int newIndex = (index > 0) ? index - 1 : 0;
        tabPlant->setCurrentIndex(newIndex);
    }

    qDebug() << "Closing Plant tab at index" << index;

    QWidget* tabItem = tabPlant->widget(index);
    if (tabItem) {
        tabPlant->removeTab(index);
        delete tabItem;
    } else {
        qDebug() << "No widget found at index" << index;
    }
}

void MainWindow::closePageTab(int index)
{
    if (index <= 0 || index >= tabPage->count() - 1) {
        qDebug() << "Cannot close tab at index" << index;
        return;
    }

    QMessageBox::StandardButton reply = QMessageBox::question(this, "Confirm Close",
                                                              "Do you want to save changes before closing?",
                                                              QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);

    if (reply == QMessageBox::Cancel) {
        return;
    }

    if (reply == QMessageBox::Yes) {
        int newIndex = (index > 0) ? index - 1 : 0;
        tabPage->setCurrentIndex(newIndex);
    }

    qDebug() << "Closing Page tab at index" << index;

    QWidget* tabItem = tabPage->widget(index);
    if (tabItem) {
        tabPage->removeTab(index);
        delete tabItem;
    } else {
        qDebug() << "No widget found at index" << index;
    }
}

void MainWindow::createReportSelectedItems()
{
    QDialog *dialog = new QDialog(this);
    dialog->setWindowTitle("Results");
    dialog->setFixedWidth(650);
    dialog->setFixedHeight(400);

    // Main Layout
    QVBoxLayout *mainLayout = new QVBoxLayout(dialog);

    // Header Label
    QLabel *headerLabel = new QLabel("AggFlow Results. Select print and export options below:", dialog);
    mainLayout->addWidget(headerLabel);

    // Table Header Labels with Spacing
    QVBoxLayout *headerLayout = new QVBoxLayout();
    headerLayout->setSpacing(20); // Set spacing between labels

    QLabel *labelMachine = new QLabel("Machine", dialog);
    QLabel *labelStream = new QLabel("Stream", dialog);
    QLabel *labelPlaceholder = new QLabel(" ", dialog);
    QLabel *labelTPH = new QLabel("TPH", dialog);
    QLabel *labelPower = new QLabel("Power", dialog);

    // Add labels to vertical layout
    headerLayout->addWidget(labelMachine);
    headerLayout->addWidget(labelStream);
    headerLayout->addWidget(labelPlaceholder);
    headerLayout->addWidget(labelTPH);
    headerLayout->addWidget(labelPower);
    headerLayout->addStretch(1);

    // Create Table View
    QTableView *tableView = new QTableView(dialog);

    // Horizontal Layout for Table and Header Labels
    QHBoxLayout *contentLayout = new QHBoxLayout();
    contentLayout->addLayout(headerLayout);
    contentLayout->addWidget(tableView);

    mainLayout->addLayout(contentLayout);

    // Button Layout
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    QPushButton *button1 = new QPushButton("Export Results", dialog);
    QPushButton *button2 = new QPushButton("Copy Results to Excel", dialog);
    QPushButton *button3 = new QPushButton("Print", dialog);
    QPushButton *button4 = new QPushButton("Export To PDF", dialog);
    QPushButton *button5 = new QPushButton("Close", dialog);

    buttonLayout->addWidget(button1);
    buttonLayout->addStretch(1);
    buttonLayout->addWidget(button2);
    buttonLayout->addStretch(1);
    buttonLayout->addWidget(button3);
    buttonLayout->addStretch(1);
    buttonLayout->addWidget(button4);
    buttonLayout->addStretch(1);
    buttonLayout->addWidget(button5);

    mainLayout->addLayout(buttonLayout);

    dialog->setLayout(mainLayout);
    dialog->exec();
}

void MainWindow::createEmissionReportSelectedItems()
{
    QDialog *dialog = new QDialog(this);
    dialog->setWindowTitle("Emissions");
    dialog->setFixedWidth(650);
    dialog->setFixedHeight(400);

    QLabel *headerLabel = new QLabel("AggFlow Results. Select print and export options below:", dialog);

    QTableView *tableView = new QTableView(dialog);

    // Create a model for the QTableView
    QStandardItemModel *model = new QStandardItemModel(0, 6, dialog); // 0 rows, 6 columns
    model->setHeaderData(0, Qt::Horizontal, "Machine");
    model->setHeaderData(1, Qt::Horizontal, "ID");
    model->setHeaderData(2, Qt::Horizontal, "TPH");
    model->setHeaderData(3, Qt::Horizontal, "Total\nEmission");
    model->setHeaderData(4, Qt::Horizontal, "Factor");
    model->setHeaderData(5, Qt::Horizontal, "Emission Table");

    // Set the model to the table view
    tableView->setModel(model);

    // Layouts
    QVBoxLayout *mainLayout = new QVBoxLayout(dialog);
    QHBoxLayout *mainContentLayout = new QHBoxLayout();
    QHBoxLayout *buttonLayout = new QHBoxLayout();

    // Add widgets to the layouts
    mainContentLayout->addWidget(tableView);

    // Buttons
    QPushButton *Exportbutton = new QPushButton("Export Results", dialog);
    QPushButton *CopyToExcelbutton = new QPushButton("Copy Results to Excel", dialog);
    QPushButton *Printbutton = new QPushButton("Print", dialog);
    QPushButton *ExportToPDFbutton = new QPushButton("Export To PDF", dialog);
    QPushButton *Closebutton = new QPushButton("Close", dialog);

    buttonLayout->addWidget(Exportbutton);
    buttonLayout->addStretch(1);
    buttonLayout->addWidget(CopyToExcelbutton);
    buttonLayout->addStretch(1);
    buttonLayout->addWidget(Printbutton);
    buttonLayout->addStretch(1);
    buttonLayout->addWidget(ExportToPDFbutton);
    buttonLayout->addStretch(1);
    buttonLayout->addWidget(Closebutton);

    // Assemble the main layout
    mainLayout->addWidget(headerLabel);
    mainLayout->addLayout(mainContentLayout);
    mainLayout->addLayout(buttonLayout);

    dialog->setLayout(mainLayout);
    dialog->exec();
}

void MainWindow::exportPageToPdf()
{
    QString fileName = QFileDialog::getSaveFileName(this, "Save PDF", "", "PDF Files (*.pdf)");
    if (!fileName.isEmpty()) {
    }
}

void MainWindow::exportPageWithResultToPDF()
{
    QString fileName = QFileDialog::getSaveFileName(this, "Save PDF", "", "PDF Files (*.pdf)");
    if (!fileName.isEmpty()) {
    }
}

void MainWindow::exportPageToEPS()
{
    QString fileName = QFileDialog::getSaveFileName(this, "Save EPS", "", "EPS Files (*.eps)");
    if (!fileName.isEmpty()) {
    }
}

void MainWindow::exportPageWithResultToEPS()
{
    QString fileName = QFileDialog::getSaveFileName(this, "Save EPS", "", "EPS Files (*.eps)");
    if (!fileName.isEmpty()) {
    }
}

void MainWindow::exportPageToJPEG()
{
    QString fileName = QFileDialog::getSaveFileName(this, "Save JPEG", "", "JPEG Files (*.jpg *.jpeg)");
    if (!fileName.isEmpty()) {
    }
}

void MainWindow::exportPageToTIFFColor()
{
    QString fileName = QFileDialog::getSaveFileName(this, "Save TIFF (Color)", "", "TIFF Files (*.tiff *.tif)");
    if (!fileName.isEmpty()) {
    }
}

void MainWindow::exportPageToTIFFBW()
{
    QString fileName = QFileDialog::getSaveFileName(this, "Save TIFF (Black & White)", "", "TIFF Files (*.tiff *.tif)");
    if (!fileName.isEmpty()) {
    }
}

void MainWindow::exportPlantToPdf()
{
    QString fileName = QFileDialog::getSaveFileName(this, "Save PDF", "", "PDF Files (*.pdf)");
    if (!fileName.isEmpty()) {
    }
}

void MainWindow::exportPlantWithResultToPDF()
{
    QString fileName = QFileDialog::getSaveFileName(this, "Save PDF", "", "PDF Files (*.pdf)");
    if (!fileName.isEmpty()) {
    }
}

void MainWindow::exportPlantToEPS()
{
    QString fileName = QFileDialog::getSaveFileName(this, "Save EPS", "", "EPS Files (*.eps)");
    if (!fileName.isEmpty()) {
    }
}

void MainWindow::exportPlantWithResultToEPS()
{
    QString fileName = QFileDialog::getSaveFileName(this, "Save EPS", "", "EPS Files (*.eps)");
    if (!fileName.isEmpty()) {
    }
}

void MainWindow::printCurrentContent()
{
    QPrinter printer(QPrinter::HighResolution);
    printer.setPrinterName("Printer");
    printer.setOrientation(QPrinter::Portrait);
    printer.setPageSize(QPrinter::A4);

    // Create a QPrintDialog to allow the user to select printer settings
    QPrintDialog printDialog(&printer, this);
    if (printDialog.exec() == QDialog::Accepted) {
        // Create a QPainter to print
        QPainter painter(&printer);
        graphicsView->render(&painter);
        painter.end();
    }
}

void MainWindow::printAllContent()
{
    QPrinter printer(QPrinter::HighResolution);
    printer.setPrinterName("Printer");
    printer.setOrientation(QPrinter::Portrait);
    printer.setPageSize(QPrinter::A4);

    QPrintDialog printDialog(&printer, this);
    if (printDialog.exec() == QDialog::Accepted) {
        QPainter painter(&printer);

        //        foreach (CustomGraphicsView *page, graphicsView) {
        //             page->render(&painter);
        //         }

        painter.end();
    }

}

void MainWindow::setUserPreferences()
{
    UserPreferences *user = new UserPreferences();
    user->show();
}
