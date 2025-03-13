#ifndef CIRCULARPROGRESSBAR_H
#define CIRCULARPROGRESSBAR_H

#include <QProgressBar>
#include <QPropertyAnimation>
#include <QPainter>
#include <QPushButton>
#include <QThread>
#include <QEasingCurve>
#include <QResizeEvent>
#include <QGraphicsDropShadowEffect>
#include <QTimer>

class CircularProgressBar : public QProgressBar {
    Q_OBJECT
    Q_PROPERTY(int startAngle READ angle WRITE setAngle NOTIFY startAngleChanged)
    Q_PROPERTY(double chunkLength READ chunkLength WRITE setChunkLength NOTIFY chunkLengthChanged)

public:
    CircularProgressBar(QWidget *parent = nullptr);
    ~CircularProgressBar();

    // Setters
    void setCircularDegree(const int &value = 270);
    void setSquare(const bool &enable = false);
    void setGradient(const bool &enable = false);
    void setGradientValues(const QMap<qreal, QColor> &map);
    void setMargin(const int &x = 0, const int &y = 0);
    void setTextAlignment(const Qt::Alignment &alignment = Qt::AlignCenter);
    void setProgressAlignment(const Qt::Alignment &alignment = Qt::AlignCenter);
    void setShadow(const bool &enable = true);
    void setWidth(const int &width = 200);
    void setHeight(const int &height = 200);
    void setProgressWidth(const int &width = 10);
    void setProgressRoundedCap(const bool &enable = true);
    void setEnableBg(const bool &enable = true);
    void setBgColor(const QColor &color = QColor(68, 71, 90));
    void setChunkColor(const QColor &color = QColor(73, 139, 209));
    void setEnableText(const bool &enable = true);
    void setSuffix(const QString &suffix = "%");
    void setInfiniteLoop(bool loop);
    void setTextColor(const QColor &color = QColor(73, 139, 209));
    void setEasingcurve(QEasingCurve::Type);
    void setChunkLength(double length);
    void setDuration(short duration);

    // Getters
    double chunkLength() { return m_chunkLength; }
    int getCircularDegree() const { return this->circularDegree; }
    int getMarginX() const { return this->marginX; }
    int getMarginY() const { return this->marginY; }
    int getWidth() const { return this->width; }
    bool getSquared() const { return this->square; }
    bool getGradient() const { return this->gradient; }
    QMap<qreal, QColor> getGradientValues() const { return this->gradient_colors; }
    int getHeight() const { return this->height; }
    int getProgressWidth() const { return this->progress_width; }
    Qt::Alignment getTextAlignment() const { return this->textAlignment; }
    Qt::Alignment getProgressAlignment() const { return this->progressAlignment; }
    bool getShadow() const { return this->shadow; }
    bool getProgressRoundedCap() const { return this->progress_rounded_cap; }
    bool getEnableBg() const { return this->enable_bg; }
    QColor getBgColor() const { return this->bg_color; }
    QColor chunkColor() const { return this->chunk_color; }
    bool getEnableText() const { return this->enable_text; }
    QString getSuffix() const { return this->suffix; }
    QColor getTextColor() const { return this->text_color; }
    void stop();
    bool isStop() const;
    QPushButton* button() const;

    void updateChunkPosition();

signals:
    void SI_circularDegreeChanged(int value);
    void SI_shadowChanged(bool enable);
    void SI_squareChanged(bool enable);
    void SI_marginChanged(int x, int y);
    void SI_widthChanged(int width);
    void SI_gradientChanged(bool enable);
    void SI_gradientValuesChanged(QMap<qreal, QColor> map);
    void SI_heightChanged(int height);
    void SI_textAlignmentChanged(Qt::Alignment alignment);
    void SI_progressAlignmentChanged(Qt::Alignment alignment);
    void SI_progressWidthChanged(int width);
    void SI_progressCapChanged(bool enable);
    void SI_backgroundChanged(bool enable);
    void SI_backgroundColorChanged(QColor color);
    void SI_chunkColorChanged(QColor color);
    void SI_textEnableChanged(bool enable);
    void SI_suffixChanged(QString suffix);
    void SI_textColorChanged(QColor color);

    void chunkLengthChanged(double);
    void startAngleChanged(int);
    void stopEmitted(bool);

protected:
    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

private:
    QPainter *paint = nullptr;
    QPropertyAnimation *animation = nullptr;
    QTimer* m_timer = nullptr;

    bool square = true;
    int circularDegree = 360;
    int width = 100;
    int marginX = 0;
    int marginY = 0;
    int height = 100;
    int progress_width = 10;
    bool shadow = false;
    bool progress_rounded_cap = true;
    bool enable_bg = true;
    bool gradient = false;
    Qt::Alignment textAlignment = Qt::AlignCenter;
    Qt::Alignment progressAlignment = Qt::AlignCenter;
    QColor bg_color = QColor(20, 20, 20, 255);
    QColor chunk_color = QColor(73, 139, 209);
    bool enable_text = true;

    bool infiniteloop = false;
    int startAngle = 0;
    short m_duration;

    QString suffix = "%";
    QColor text_color = QColor(73, 139, 209);
    QMap<qreal, QColor> gradient_colors;

    double m_chunkLength = 180;
    QEasingCurve::Type m_curve = QEasingCurve::OutQuart;
    QPushButton *stopButton = nullptr;
    bool wasIncreasing = false;

private slots:
    int angle() { return startAngle; }
    void setAngle(int angle) {
        startAngle = angle;
        repaint();
    }
    void start();
};

#endif // CIRCULARPROGRESSBAR_H
