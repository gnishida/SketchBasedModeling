#include "MainWindow.h"
#include <QFileDialog>
#include <QDate>
#include <time.h>

MainWindow::MainWindow(QWidget *parent, Qt::WFlags flags) : QMainWindow(parent, flags) {
	ui.setupUi(this);

	// メニューハンドラ
	connect(ui.actionNewSketch, SIGNAL(triggered()), this, SLOT(onNewSketch()));
	connect(ui.actionSaveImage, SIGNAL(triggered()), this, SLOT(onSaveImage()));
	connect(ui.actionLoadCamera, SIGNAL(triggered()), this, SLOT(onLoadCamera()));
	connect(ui.actionSaveCamera, SIGNAL(triggered()), this, SLOT(onSaveCamera()));
	connect(ui.actionResetCamera, SIGNAL(triggered()), this, SLOT(onResetCamera()));
	connect(ui.actionExit, SIGNAL(triggered()), this, SLOT(close()));

	glWidget = new GLWidget3D(this);
	setCentralWidget(glWidget);
}

void MainWindow::keyPressEvent(QKeyEvent* e) {
	glWidget->keyPressEvent(e);
}

void MainWindow::keyReleaseEvent(QKeyEvent* e) {
	glWidget->keyReleaseEvent(e);
}

void MainWindow::onNewSketch() {
	glWidget->clear();
	glWidget->update();
}

void MainWindow::onSaveImage() {
	QString	filename = QFileDialog::getSaveFileName(this, tr("Save image..."), "", tr("Image Files (*.png)"));
	if (filename.isEmpty()) return;

	printf("Save %s\n",filename.toUtf8().constData());
}

void MainWindow::onLoadCamera() {
	QString filename = QFileDialog::getOpenFileName(this, tr("Open camera file..."), "", tr("Camera Files (*.cam)"));
	if (filename.isEmpty()) return;

	glWidget->camera.loadCameraPose(filename.toUtf8().data());
	glWidget->update();
}

void MainWindow::onSaveCamera() {
	QString filename = QFileDialog::getSaveFileName(this, tr("Save camera file..."), "", tr("Camera Files (*.cam)"));
	if (filename.isEmpty()) return;

	glWidget->camera.saveCameraPose(filename.toUtf8().data());
}

void MainWindow::onResetCamera() {
	glWidget->camera.resetCamera();
	glWidget->update();
}

