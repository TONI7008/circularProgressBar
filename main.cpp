#include <QApplication>
#include <QWidget>
#include <QGridLayout>
#include <QPushButton>
#include <QSlider>
#include <QLabel>
#include <QCheckBox>
#include <QGroupBox>
#include <QRandomGenerator>
#include <QComboBox>
#include "circularprogressbar.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    // Create main window
    QWidget window;
    window.setWindowTitle("CircularProgressBar Test Suite");
    window.resize(1000, 800);

    // Create layout
    QGridLayout *layout = new QGridLayout(&window);

    // Create progress bar
    CircularProgressBar *progressBar = new CircularProgressBar();
    progressBar->setFixedSize(400, 400);
    layout->addWidget(progressBar, 0, 0, 1, 3);

    // Create control panel
    QWidget *controlPanel = new QWidget();
    QGridLayout *controlLayout = new QGridLayout(controlPanel);
    layout->addWidget(controlPanel, 1, 0, 1, 3);

    // Gradient Controls
    QGroupBox *gradientGroup = new QGroupBox("Gradient Controls");
    QGridLayout *gradientLayout = new QGridLayout(gradientGroup);

    QPushButton *btnToggleGradient = new QPushButton("Toggle Gradient");
    QPushButton *btnNextGradient = new QPushButton("Next Gradient");
    QSlider *gradientPosSlider = new QSlider(Qt::Horizontal);
    gradientPosSlider->setRange(0, 100);
    gradientPosSlider->setValue(50);

    gradientLayout->addWidget(btnToggleGradient, 0, 0);
    gradientLayout->addWidget(btnNextGradient, 0, 1);
    gradientLayout->addWidget(new QLabel("Gradient Position:"), 1, 0);
    gradientLayout->addWidget(gradientPosSlider, 1, 1, 1, 2);

    // Infinite Loop Controls
    QGroupBox *loopGroup = new QGroupBox("Infinite Loop Controls");
    QGridLayout *loopLayout = new QGridLayout(loopGroup);

    QPushButton *btnToggleLoop = new QPushButton("Toggle Infinite Loop");
    QSlider *loopSpeedSlider = new QSlider(Qt::Horizontal);
    loopSpeedSlider->setRange(1, 100);
    loopSpeedSlider->setValue(18);
    QLabel *loopSpeedLabel = new QLabel("Loop Speed: 18ms");

    loopLayout->addWidget(btnToggleLoop, 0, 0, 1, 2);
    loopLayout->addWidget(new QLabel("Loop Speed:"), 1, 0);
    loopLayout->addWidget(loopSpeedSlider, 1, 1);
    loopLayout->addWidget(loopSpeedLabel, 1, 2);

    // Progress Controls
    QGroupBox *progressGroup = new QGroupBox("Progress Controls");
    QGridLayout *progressLayout = new QGridLayout(progressGroup);

    QSlider *progressSlider = new QSlider(Qt::Horizontal);
    progressSlider->setRange(0, 100);
    progressSlider->setValue(50);
    QPushButton *btnRandomValue = new QPushButton("Random Value");

    progressLayout->addWidget(new QLabel("Progress Value:"), 0, 0);
    progressLayout->addWidget(progressSlider, 0, 1, 1, 2);
    progressLayout->addWidget(btnRandomValue, 1, 0, 1, 3);

    // Style Controls
    QGroupBox *styleGroup = new QGroupBox("Style Controls");
    QGridLayout *styleLayout = new QGridLayout(styleGroup);

    QCheckBox *chkRoundedCap = new QCheckBox("Rounded Cap");
    chkRoundedCap->setChecked(true);
    QCheckBox *chkShowText = new QCheckBox("Show Text");
    chkShowText->setChecked(true);
    QCheckBox *chkShowBg = new QCheckBox("Show Background");
    chkShowBg->setChecked(true);

    QComboBox *degreeCombo = new QComboBox();
    degreeCombo->addItems({"90°", "180°", "270°", "360°"});
    degreeCombo->setCurrentIndex(2);

    styleLayout->addWidget(chkRoundedCap, 0, 0);
    styleLayout->addWidget(chkShowText, 0, 1);
    styleLayout->addWidget(chkShowBg, 0, 2);
    styleLayout->addWidget(new QLabel("Arc Degree:"), 1, 0);
    styleLayout->addWidget(degreeCombo, 1, 1, 1, 2);

    // Add groups to control panel
    controlLayout->addWidget(gradientGroup, 0, 0);
    controlLayout->addWidget(loopGroup, 0, 1);
    controlLayout->addWidget(progressGroup, 1, 0);
    controlLayout->addWidget(styleGroup, 1, 1);

    // Create gradient presets
    QList<QMap<qreal, QColor>> gradients;

    // Gradient 1: Red-Yellow
    QMap<qreal, QColor> gradient1;
    gradient1[0.0] = Qt::red;
    gradient1[1.0] = Qt::yellow;
    gradients.append(gradient1);

    // Gradient 2: Rainbow
    QMap<qreal, QColor> gradient2;
    gradient2[0.0] = QColor(255, 0, 0);
    gradient2[0.2] = QColor(255, 127, 0);
    gradient2[0.4] = QColor(255, 255, 0);
    gradient2[0.6] = QColor(0, 255, 0);
    gradient2[0.8] = QColor(0, 0, 255);
    gradient2[1.0] = QColor(139, 0, 255);
    gradients.append(gradient2);

    // Gradient 3: Pastel
    QMap<qreal, QColor> gradient3;
    gradient3[0.0] = QColor(255, 204, 204);
    gradient3[0.5] = QColor(204, 255, 204);
    gradient3[1.0] = QColor(204, 204, 255);
    gradients.append(gradient3);

    int currentGradient = 0;

    // Connect signals and slots
    // Gradient controls
    QObject::connect(btnToggleGradient, &QPushButton::clicked, [progressBar]() {
        progressBar->setGradient(!progressBar->hasGradient());
    });

    QObject::connect(btnNextGradient, &QPushButton::clicked,
                     [progressBar, &gradients, &currentGradient]() {
                         currentGradient = (currentGradient + 1) % gradients.size();
                         progressBar->setGradientValues(gradients[currentGradient]);
                     });

    QObject::connect(gradientPosSlider, &QSlider::valueChanged, progressBar, &CircularProgressBar::setValue);

    // Infinite loop controls
    QObject::connect(btnToggleLoop, &QPushButton::clicked, [progressBar]() {
        progressBar->setInfiniteLoop(!progressBar->isInfiniteLoop());
    });

    QObject::connect(loopSpeedSlider, &QSlider::valueChanged,
                     [progressBar, loopSpeedLabel](int value) {
                         progressBar->setDuration(value);
                         loopSpeedLabel->setText(QString("Loop Speed: %1ms").arg(value));
                     });

    // Progress controls
    QObject::connect(progressSlider, &QSlider::valueChanged, progressBar, &CircularProgressBar::setValue);

    QObject::connect(btnRandomValue, &QPushButton::clicked,
                     [progressSlider]() {
                         int randomValue = QRandomGenerator::global()->bounded(101);
                         progressSlider->setValue(randomValue);
                     });

    // Style controls
    QObject::connect(chkRoundedCap, &QCheckBox::toggled, progressBar, &CircularProgressBar::setProgressRoundedCap);
    QObject::connect(chkShowText, &QCheckBox::toggled, progressBar, &CircularProgressBar::setEnableText);
    QObject::connect(chkShowBg, &QCheckBox::toggled, progressBar, &CircularProgressBar::setEnableBg);

    QObject::connect(degreeCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
                     [progressBar](int index) {
                         int degrees[] = {90, 180, 270, 360};
                         progressBar->setCircularDegree(degrees[index]);
                     });

    // Initialize with first gradient
    progressBar->setGradientValues(gradients.first());
    progressBar->setGradient(true);

    window.show();
    return app.exec();
}
