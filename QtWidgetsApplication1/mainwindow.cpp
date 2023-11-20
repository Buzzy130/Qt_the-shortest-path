#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTimer>
#include <QMessageBox>
#include <QTextStream>
MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    mPix = QPixmap(1920, 1080);
    mPix.fill(Qt::black);
    mousePressed = false;
    init = { 50, 50 };
    goal = { 600, 300 };
    i = 0;
    r = 10;
    drawObs = false;
    showingPath = false;
    moveInit = false;
    lock = false;
    drawPath = false;
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(animate()));
    timer->start(15);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_newScene_clicked()
{
    init = { 50, 50 };
    goal = { 600, 300 };
    segmentsVertices.clear();
    path.clear();
    showingPath = false;
    lock = false;
    ui->label->setText("");
}


void MainWindow::on_save_clicked()
{
    QString fileName = QFileDialog::getSaveFileName(this,
        tr("Save scene"), "../",
        tr("*.txt;;All Files (*)"));
    if (fileName.isEmpty())
        return;
    else {
        QFile file(fileName);
        if (!file.open(QIODevice::WriteOnly)) {
            QMessageBox::information(this, tr("Unable to open file"),
                file.errorString());
            return;
        }
        QTextStream out(&file);
        out << QString::number(init.x) << " " << QString::number(init.y) << " " << QString::number(r) << "\n";
        out << QString::number(goal.x) << " " << QString::number(goal.y) << "\n";
        for (size_t i = 0; i + 3 < segmentsVertices.size(); i += 4) {
            out << QString::number(segmentsVertices[i]) << " " << QString::number(segmentsVertices[i + 1]) << " "
                << QString::number(segmentsVertices[i + 2]) << " " << QString::number(segmentsVertices[i + 3]) << "\n";
        }
        file.close();
    }

}


void MainWindow::on_load_clicked()
{
    QString loadfile = QFileDialog::getOpenFileName(this, "Open save file", "../", "*.txt");
    if (loadfile.isEmpty())
        QMessageBox::information(this, tr("File error"), tr("File does not exist"));
    else {
        QFile file(loadfile);
        if (!file.open(QIODevice::ReadOnly)) {
            QMessageBox::information(this, tr("File error"), file.errorString());
        }
        else {
            segmentsVertices.clear();
            path.clear();
            showingPath = false;
            lock = false;
            QString initX, initY, rStr, goalX, goalY;
            QTextStream in(&file);
            QString text = in.readLine();

            QStringList line = text.split(' ');
            init.x = line[0].toInt();
            init.y = line[1].toInt();
            r = line[2].toInt();

            text = in.readLine();

            line = text.split(' ');
            goal.x = line[0].toInt();
            goal.y = line[1].toInt();


            while (!in.atEnd()) {
                text = in.readLine();
                line = text.split(' ');
                segmentsVertices.push_back(line[0].toInt());
                segmentsVertices.push_back(line[1].toInt());
                segmentsVertices.push_back(line[2].toInt());
                segmentsVertices.push_back(line[3].toInt());
            }
        }
        file.close();
    }
    ui->label->setText("");

}

void MainWindow::on_start_clicked()
{
    lock = true;
    C_obs obs;
    for (size_t i = 0; i + 3 < segmentsVertices.size(); i += 4) {
        int x1 = segmentsVertices[i];
        int y1 = segmentsVertices[i + 1];
        int x2 = segmentsVertices[i + 2];
        int y2 = segmentsVertices[i + 3];

        segment s(x1, y1, x2, y2);
        obs.add_obstacle(s, r);
    }

    path = shortest_path(init, goal, obs, r);

    if (path.size() == 0) {
        ui->label->setText("the path is missing");
    }

    else {
        i = 0;
        showingPath = true;
        drawPath = true;
    }
}


void MainWindow::on_lineEdit_textChanged(const QString& arg1)
{
    if (!lock)
        r = arg1.toInt();
}
void MainWindow::animate() {
    if (showingPath) {
        if (i + 1 < path.size()) {
            double normalX = path[i + 1].x - init.x;
            double normalY = path[i + 1].y - init.y;
            double distance = sqrt(normalX * normalX + normalY * normalY);
            normalX = normalX * 5.0 / distance;
            normalY = normalY * 5.0 / distance;

            if ((init.x < path[i + 1].x - normalX || init.x > path[i + 1].x + normalX)
                || (init.y < path[i + 1].y - normalY || init.y > path[i + 1].y + normalY)) {
                init.x = (double)init.x + normalX;
                init.y = (double)init.y + normalY;

            }
            else {
                init.x = path[i + 1].x;
                init.y = path[i + 1].y;
                i++;
            }
        }
        //update();
    }

}




void MainWindow::mousePressEvent(QMouseEvent* e) {

    if (!lock &&
        e->pos().x() < 639 && e->pos().x() > 0 && e->pos().y() < 379 && e->pos().y() > 0) {
        mousePressed = true;
        if (init.x - r <= e->pos().x() && e->pos().x() <= init.x + r &&
            init.y - r <= e->pos().y() && e->pos().y() <= init.y + r) {
            moveInit = true;
        }
        else if (goal.x - 5 <= e->pos().x() && e->pos().x() <= goal.x + 5 &&
            goal.y - 5 <= e->pos().y() && e->pos().y() <= goal.y + 5) {
            moveGoal = true;
        }
        else {
            drawObs = true;
            int x1 = e->pos().x();
            int y1 = e->pos().y();
            rect.setTopLeft(e->pos());
            rect.setBottomRight(e->pos());

            segmentsVertices.push_back(x1);
            segmentsVertices.push_back(y1);
        }
    }
}

void MainWindow::mouseMoveEvent(QMouseEvent* e) {
    if (e->type() == QEvent::MouseMove) {
        int x, y;
        x = e->pos().x();
        y = e->pos().y();

        if (e->pos().x() >= 639)
            x = 639;
        else if (e->pos().x() <= 0)
            x = 0;
        if (e->pos().y() >= 379)
            y = 379;
        else if (e->pos().y() <= 0)
            y = 0;

        if (moveGoal == true) {
            goal.x = x;
            goal.y = y;
        }
        else if (moveInit == true) {
            init.x = x;
            init.y = y;
        }
        else if (drawObs == true)
            rect.setBottomRight(QPoint(x, y));
    }

    //update();
}

void MainWindow::mouseReleaseEvent(QMouseEvent* e) {
    if (drawObs) {
        int x2, y2;
        x2 = e->pos().x();
        y2 = e->pos().y();

        if (e->pos().x() >= 639)
            x2 = 639;
        else if (e->pos().x() <= 0)
            x2 = 0;
        if (e->pos().y() >= 379)
            y2 = 379;
        else if (e->pos().y() <= 0)
            y2 = 0;

        segmentsVertices.push_back(x2);
        segmentsVertices.push_back(y2);
    }

    mousePressed = false;
    moveInit = false;
    moveGoal = false;
    drawObs = false;
    //update();
}

void MainWindow::paintEvent(QPaintEvent*) {
    painter.begin(this);
    QPen pen(Qt::red, 1, Qt::NoPen),
        penBlack(Qt::white, 1, Qt::SolidLine),
        penWay(Qt::green, 3, Qt::SolidLine);
    QBrush robot(Qt::darkGray, Qt::SolidPattern),
        obs(Qt::darkBlue, Qt::SolidPattern),
        goalB(Qt::red, Qt::SolidPattern);

    painter.drawPixmap(0, 0, mPix);
    painter.setRenderHint(QPainter::Antialiasing, true);

    painter.setPen(pen);

    if (mousePressed) {
        if (drawObs) {
            painter.setBrush(robot);
            painter.drawEllipse(init.x - r, init.y - r, 2 * r, 2 * r);
            painter.setBrush(goalB);
            painter.drawEllipse(goal.x - 2, goal.y - 2, 4, 4);
            painter.setBrush(obs);
            painter.drawRect(rect);
        }
    }
    if (drawPath)
    {
        painter.setPen(penWay);
        for (int i = 0; i + 1 < path.size(); i++)
        {
            painter.drawLine(path[i].x, path[i].y, path[i + 1].x, path[i + 1].y);
        }
        painter.setPen(pen);
    }
    for (size_t i = 0; i + 3 < segmentsVertices.size(); i += 4) {
        int x1 = segmentsVertices[i];
        int y1 = segmentsVertices[i + 1];
        int w = segmentsVertices[i + 2] - x1;
        int h = segmentsVertices[i + 3] - y1;
        painter.setPen(pen);
        painter.setBrush(obs);
        painter.drawRect(QRect(x1, y1, w, h));
    }

    painter.setBrush(robot);
    painter.drawEllipse(init.x - r, init.y - r, 2 * r, 2 * r);
    painter.setBrush(goalB);
    painter.drawEllipse(goal.x - 2, goal.y - 2, 4, 4);
    painter.setPen(penBlack);
    painter.setBrush(QBrush(Qt::white, Qt::NoBrush));
    painter.drawRect(0, 0, 639, 379);
    painter.end();
    update();
}
