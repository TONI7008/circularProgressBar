#include "circularprogressbar.h"
#include <QPainterPath>
#include <QDebug>
#include <QResizeEvent>

CircularProgressBar::CircularProgressBar(QWidget *parent) : QProgressBar(parent) {
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    setMinimumSize(50, 50);
    resize(width, height);

    paint = new QPainter();
    m_timer = new QTimer(this);
    animation = new QPropertyAnimation(this, "animationProgress");
    animation->setEasingCurve(m_curve);
    animation->setDuration(200);

    stopButton = new QPushButton(this);
    stopButton->setVisible(false);
    stopButton->setCheckable(true);
    stopButton->setChecked(false);
    stopButton->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    stopButton->setStyleSheet("background:rgba(193, 195, 196,0.6); border-radius: 6px; border: none;");
    stopButton->setIconSize(QSize(24, 24));

    connect(m_timer, &QTimer::timeout, this, &CircularProgressBar::updateChunkPosition);
    connect(animation, &QPropertyAnimation::valueChanged, this, [this](){ update(); });
    connect(this, &QProgressBar::valueChanged, this, [this](){
        if(!infiniteloop) updateProgressAnimation();
    });
    connect(stopButton, &QPushButton::clicked, this, &CircularProgressBar::stopEmitted);

    setupAnimations();
}

CircularProgressBar::~CircularProgressBar() {
    delete paint;
    delete animation;
    if (m_timer->isActive()) m_timer->stop();
    delete m_timer;
}

void CircularProgressBar::setCircularDegree(int value)
{
    circularDegree=value;

    emit SI_circularDegreeChanged(value);
}

void CircularProgressBar::setupAnimations() {
    m_timer->start(m_duration); // ~60fps
    QTimer::singleShot(0, this, [this]() {
        if (maximum() == minimum()) {
            setInfiniteLoop(true);
        }
    });
}

void CircularProgressBar::setAnimationProgress(float progress) {
    if (m_animationProgress != progress) {
        m_animationProgress = progress;
        emit animationProgressChanged(progress);
    }
}

void CircularProgressBar::setAngle(int angle)
{
    startAngle=angle;
}

void CircularProgressBar::updateProgressAnimation() {
    float range = maximum() - minimum();
    float target = (range > 0) ? (value() - minimum()) / range : 0.0f;
    float delta = std::abs(target - m_animationProgress);

    if (value() >= maximum() || delta >= m_threshold) {
        animation->stop();
        animation->setStartValue(m_animationProgress);
        animation->setEndValue(target);
        animation->setDuration(qBound(100, static_cast<int>(delta * 1000), 1000));
        animation->start();
    }
}

void CircularProgressBar::setInfiniteLoop(bool loop) {
    if (infiniteloop == loop) return;

    infiniteloop = loop;
    if (infiniteloop) {
        animation->stop();
        m_timer->start(m_duration);
    } else {
        m_timer->stop();
        updateProgressAnimation();
        repaint();
    }
    emit modeChanged(infiniteloop);
}

void CircularProgressBar::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing);

    int pnwidth = width - progress_width;
    int pnheight = height - progress_width;
    int margin = progress_width / 2;
    QRect rect(marginX + margin, marginY + margin, pnwidth, pnheight);

    // Draw background
    painter.setPen(Qt::NoPen);
    if (enable_bg) {
        QPen bgPen;
        bgPen.setColor(bg_color);
        bgPen.setWidth(progress_width);
        bgPen.setCosmetic(true);
        if (progress_rounded_cap) bgPen.setCapStyle(Qt::RoundCap);
        painter.setPen(bgPen);
        painter.drawArc(rect, 90 * 16, -circularDegree * 16);
    }

    // Calculate progress proportion
    double proportion = infiniteloop ? 0.0 : m_animationProgress;

    // Set progress pen style
    QPen pen;
    pen.setWidth(progress_width);
    pen.setCosmetic(true);
    if (progress_rounded_cap) pen.setCapStyle(Qt::RoundCap);

    // Set progress color
    if (proportion > 0.85) {
        pen.setColor(QColor(71, 158, 245));
    } else {
        pen.setColor(chunk_color);
    }

    // Apply gradient if enabled
    // Apply gradient if enabled
    if (gradient && proportion <= 0.85) {
        QLinearGradient linearGrad(rect.topLeft(), rect.bottomRight());
        for (auto it = gradient_colors.begin(); it != gradient_colors.end(); ++it) {
            linearGrad.setColorAt(it.key(), it.value());
        }
        pen.setBrush(linearGrad);
    }


    // Draw progress arc
    painter.setPen(pen);
    int spanAngle = -(infiniteloop ? m_chunkLength * 16 : proportion * circularDegree * 16);
    painter.drawArc(rect, 90 * 16 + (infiniteloop ? startAngle * 16 : 0), spanAngle);

    // Draw text or stop button
    if (enable_text && !infiniteloop) {
        pen.setColor(text_color);
        painter.setPen(pen);
        QString text = QString::number(static_cast<int>(proportion * 100)) + suffix;
        QFontMetrics metrics = painter.fontMetrics();
        QRect textRect = metrics.boundingRect(text);
        QRect centeredRect(
            rect.center().x() - textRect.width() / 2,
            rect.center().y() - textRect.height() / 2,
            textRect.width(), textRect.height()
            );
        painter.drawText(centeredRect, textAlignment, text);
        stopButton->setVisible(false);
    } else if (!enable_text && !infiniteloop) {
        int btnSize = qMin(width, height) / 2;
        stopButton->setGeometry(
            rect.center().x() - btnSize / 2,
            rect.center().y() - btnSize / 2,
            btnSize, btnSize
            );
        stopButton->setVisible(true);
    }
}


void CircularProgressBar::setValue(int value) {
    value = qBound(minimum(), value, maximum());
    if (value == this->value()) return;
    QProgressBar::setValue(value);
}

void CircularProgressBar::setGradient(bool enable)
{
    gradient=enable;

    repaint();
}

void CircularProgressBar::setGradientValues(const QMap<qreal, QColor> &map)
{
    gradient_colors=map;

    repaint();
}

void CircularProgressBar::setProgressWidth(int width)
{
    progress_width=width;
    repaint();
}

void CircularProgressBar::setProgressRoundedCap(bool enable)
{
    progress_rounded_cap=enable;
    repaint();
}

void CircularProgressBar::setEnableBg(bool enable)
{
    enable_bg=enable;
    repaint();
}

void CircularProgressBar::setEnableText(bool enable)
{
    enable_text=enable;
    repaint();
}

void CircularProgressBar::setRange(int minValue, int maxValue) {
    if (minValue >= maxValue) {
        qWarning() << "Invalid range";
        return;
    }

    //bool wasInfinite = !infiniteloop && (maximum() == minimum());
    QProgressBar::setRange(minValue, maxValue);

    if (infiniteloop && (maxValue > minValue)) {
        setInfiniteLoop(false);
    } else if (maxValue == minValue) {
        setInfiniteLoop(true);
    }
}

void CircularProgressBar::setChunkLength(double length)
{
    m_chunkLength=length;
    repaint();
}

void CircularProgressBar::setDuration(short duration)
{
    if(infiniteloop) {
        setInfiniteLoop(false);
        m_duration=duration;
        setInfiniteLoop(true);
    }

    m_duration=duration;
}

void CircularProgressBar::updateChunkPosition() {
    startAngle -= 6;
    if (startAngle < 0) startAngle += 360;
    update();
}

void CircularProgressBar::stop() {
    m_stop = true;
    setInfiniteLoop(false);
    animation->stop();
}

void CircularProgressBar::resizeEvent(QResizeEvent *event) {
    QSize size = event->size();

    if (square) {
        width = height = qMin(size.width(), size.height());
        if (progressAlignment & Qt::AlignCenter) {
            marginX = (size.width() - width) / 2;
            marginY = (size.height() - height) / 2;
        } else {
            marginX = (progressAlignment & Qt::AlignRight) ? (size.width() - width) :
                          (progressAlignment & Qt::AlignHCenter) ? (size.width() - width) / 2 : 0;
            marginY = (progressAlignment & Qt::AlignBottom) ? (size.height() - height) :
                          (progressAlignment & Qt::AlignVCenter) ? (size.height() - height) / 2 : 0;
        }
    } else {
        width = size.width();
        height = size.height();
        marginX = marginY = 0;
    }

    if (stopButton->isVisible()) {
        int btnSize = qMin(width, height) / 2;
        stopButton->setGeometry(
            marginX + (width - btnSize) / 2,
            marginY + (height - btnSize) / 2,
            btnSize, btnSize
            );
    }

    update();
}
