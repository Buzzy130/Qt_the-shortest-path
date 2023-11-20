#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMouseEvent>
#include <QFileDialog>
#include <QMainWindow>
#include <QPainter>
#include <QPaintEvent>
#include <QWidget>
#include <QTextEdit>

#include "shortest_path.h"


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    bool mousePressed = false;
    bool drawStarted = false;
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();
    void mousePressEvent(QMouseEvent* e);
    void mouseMoveEvent(QMouseEvent* e);
    void mouseReleaseEvent(QMouseEvent* e);
    void paintEvent(QPaintEvent* e);
    void showPath();
    void setScene();

private slots:
    void on_start_clicked();
    void on_load_clicked();
    void on_save_clicked();
    void on_lineEdit_textChanged(const QString& arg1);
    void animate();

    void on_newScene_clicked();

private:
    Ui::MainWindow* ui;
    std::vector<vertex> path;
    std::vector<int> segmentsVertices;
    vertex init, goal;
    size_t i;
    int r;
    bool moveInit;
    bool drawObs;
    bool moveGoal;
    bool showingPath;
    bool lock;
    bool drawPath;
    QTimer* timer;
    QPainter painter;
    QPixmap mPix;
    QRect rect;
};
#endif // MAINWINDOW_H
