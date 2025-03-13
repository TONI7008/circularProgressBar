#include "circularprogressbar.h"

CircularProgressBar::CircularProgressBar(QWidget *parent)
    : QProgressBar(parent), infiniteloop(false), startAngle(360), m_duration(18) {

    m_timer = new QTimer(this);
    paint = new QPainter();
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    resize(width, height);
    setMinimumSize(50, 50);

    animation = new QPropertyAnimation(this, "value");
    animation->setEasingCurve(m_curve);
    animation->setDuration(200);

    connect(m_timer, &QTimer::timeout, this, &CircularProgressBar::updateChunkPosition);

    setChunkColor(Qt::white);
    updateGeometry();

    stopButton = new QPushButton(this);
    stopButton->setVisible(false);
    stopButton->setCheckable(true);
    stopButton->setChecked(false);
    stopButton->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    if (stopButton) {
        stopButton->setStyleSheet("background-color: transparent; border-radius: 8px; border: none;");
        stopButton->setIconSize(QSize(24, 24));
    }

    connect(stopButton, &QPushButton::clicked, this, [this](bool checked) {
        emit stopEmitted(checked);
    });
}

CircularProgressBar::~CircularProgressBar() {
    delete this->paint;
    delete animation;
    if (m_timer->isActive()) m_timer->stop();
    delete m_timer;
}

void CircularProgressBar::setInfiniteLoop(bool loop) {
    infiniteloop = loop;
    if (infiniteloop) {
        m_timer->start(m_duration);
    } else {
        m_timer->stop();
    }
}

void CircularProgressBar::resizeEvent(QResizeEvent *event) {
    QSize size;
    if (event->size().width() > width + marginX * 2)
        size.setWidth(qMax(event->size().width(), width + marginX * 2));
    else
        size.setWidth(qMin(event->size().width(), width + marginX * 2));

    if (event->size().height() > height + marginY * 2)
        size.setHeight(qMax(event->size().height(), height + marginY * 2));
    else
        size.setHeight(qMin(event->size().height(), height + marginY * 2));

    if (square) {
        this->width = qMin(size.width(), size.height());
        this->height = qMin(size.width(), size.height());
        if (progressAlignment & Qt::AlignCenter) {
            this->marginX = (size.width() - this->width) / 2;
            this->marginY = (size.height() - this->height) / 2;
        } else {
            if (progressAlignment & Qt::AlignHCenter) {
                this->marginX = (size.width() - this->width) / 2;
            } else if (progressAlignment & Qt::AlignRight) {
                this->marginX = (size.width() - this->width);
            } else if (progressAlignment & Qt::AlignLeft) {
                this->marginX = 0;
            }

            if (progressAlignment & Qt::AlignVCenter) {
                this->marginY = (size.height() - this->height) / 2;
            } else if (progressAlignment & Qt::AlignBottom) {
                this->marginY = (size.height() - this->height);
            } else if (progressAlignment & Qt::AlignTop) {
                this->marginY = 0;
            }
        }
    } else {
        this->width = size.width();
        this->height = size.height();
        this->marginX = 0;
        this->marginY = 0;
    }

    if (stopButton->isVisible()) {
        QRect rect = QRect(marginX, marginY, this->width, this->height);
        int btnWidth = qMin(rect.width(), rect.height()) / 2;
        int btnHeight = btnWidth;

        stopButton->setGeometry(
            rect.center().x() - btnWidth / 2,
            rect.center().y() - btnHeight / 2,
            btnWidth, btnHeight
            );
    }

    update();
}

void CircularProgressBar::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event);

    int pnwidth = width - progress_width;
    int pnheight = height - progress_width;
    int margin = progress_width / 2;

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    QRect rect(marginX + margin, marginY + margin, pnwidth, pnheight);
    painter.setPen(Qt::NoPen);
    painter.drawRect(rect);

    QPen pen;
    pen.setWidth(progress_width);
    pen.setCosmetic(true);
    if (progress_rounded_cap) {
        pen.setCapStyle(Qt::RoundCap);
    }

    if (enable_bg) {
        QPen bgPen;
        bgPen.setColor(bg_color);
        bgPen.setCosmetic(true);
        bgPen.setWidth(progress_width);
        if (progress_rounded_cap) {
            bgPen.setCapStyle(Qt::RoundCap);
        }
        painter.setPen(bgPen);
        painter.drawArc(marginX + margin, marginY + margin, pnwidth, pnheight,
                        90 * 16, -circularDegree * 16);
    }

    double proportion = static_cast<double>(value()) / (maximum() - minimum());

    // Define the border color (e.g., a darker version of the chunk color)
    QColor borderColor = chunk_color.darker(200); // Adjust the darkness as needed

    // Draw the border arc (slightly larger than the main chunk)
    if (true) { // Only draw if there's progress
        QPen borderPen;
        borderPen.setColor(borderColor);
        borderPen.setWidth(progress_width + 3); // Slightly thicker for the border
        borderPen.setCosmetic(true);
        if (progress_rounded_cap) {
            borderPen.setCapStyle(Qt::RoundCap);
        }
        painter.setPen(borderPen);

        int startDrawAngle = 90 * 16;
        int spanAngle = -(infiniteloop ? chunkLength() * 16 : proportion * circularDegree * 16);

        painter.drawArc(marginX + margin, marginY + margin, pnwidth, pnheight,
                        startDrawAngle + (infiniteloop ? startAngle * 16 : 0), spanAngle);
    }

    // Draw the main chunk
    if (proportion > 0.75) {
        if (!enable_text) {
            pen.setColor(QColor(71, 158, 245));
        } else {
            pen.setColor(QColor(75, 181, 67));
        }
    } else {
        pen.setColor(chunk_color);
    }

    if (gradient && proportion <= 0.85) {
        int angle = 90;
        if (progress_rounded_cap) {
            circularDegree == 360 ? angle += 1 : angle -= 1;
        }

        QConicalGradient gradient(QPointF(marginX + width / 2, marginY + height / 2), angle);
        for (auto it = gradient_colors.begin(); it != gradient_colors.end(); ++it) {
            gradient.setColorAt(it.key(), it.value());
        }
        pen.setBrush(gradient);
    }

    painter.setPen(pen);
    int startDrawAngle = 90 * 16;
    int spanAngle = -(infiniteloop ? chunkLength() * 16 : proportion * circularDegree * 16);

    painter.drawArc(marginX + margin, marginY + margin, pnwidth, pnheight,
                    startDrawAngle + (infiniteloop ? startAngle * 16 : 0), spanAngle);

    if (enable_text && !infiniteloop) {
        pen.setColor(text_color);
        painter.setPen(pen);
        double percentage = proportion * 100.0;
        QFontMetrics metrics = painter.fontMetrics();
        QRect textRect = metrics.boundingRect(QString::number(static_cast<int>(percentage)) + suffix);
        QRect centeredTextRect(
            rect.center().x() - textRect.width() / 2,
            rect.center().y() - textRect.height() / 2,
            textRect.width(), textRect.height()
            );
        painter.drawText(centeredTextRect, textAlignment, QString::number(static_cast<int>(percentage)) + suffix);
        stopButton->setVisible(false);
    } else if (!enable_text) {
        QRect rect = QRect(marginX, marginY, this->width, this->height);
        int btnWidth = qMin(rect.width(), rect.height()) / 2;
        int btnHeight = btnWidth;

        stopButton->setGeometry(
            rect.center().x() - btnWidth / 2,
            rect.center().y() - btnHeight / 2,
            btnWidth, btnHeight
            );
        stopButton->setVisible(true);
    }
}

void CircularProgressBar::updateChunkPosition() {
    startAngle -= 6;
    if (startAngle < 0) {
        startAngle += 366;
    }

    repaint();
}

void CircularProgressBar::start() {
    this->paint = new QPainter();
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    resize(width, height);
    setMinimumSize(50, 50);

    animation = new QPropertyAnimation(this, "value");
    animation->setEasingCurve(m_curve);
    animation->setDuration(120);

    if (stopButton) {
        stopButton->setStyleSheet("background-color: transparent; border-radius: 6px; border: none;");
    }

    setChunkColor(Qt::white);
    updateGeometry();
}

void CircularProgressBar::setChunkColor(const QColor &color) {
    this->chunk_color = color;
    repaint();
    emit this->SI_chunkColorChanged(color);
}

void CircularProgressBar::setEasingcurve(QEasingCurve::Type curve) {
    m_curve = curve;
}

void CircularProgressBar::setChunkLength(double length) {
    m_chunkLength = length;
}

void CircularProgressBar::setDuration(short duration) {
    m_duration = duration;
    if (infiniteloop) {
        setInfiniteLoop(false);
        setInfiniteLoop(true);
    }
}

void CircularProgressBar::setShadow(const bool &enable) {
    if (enable) {
        auto *shadow = new QGraphicsDropShadowEffect();
        shadow->setBlurRadius(15);
        shadow->setXOffset(0);
        shadow->setYOffset(0);
        shadow->setColor(QColor(0, 0, 0, 80));
        setGraphicsEffect(shadow);
    } else {
        setGraphicsEffect(nullptr);
    }
    this->shadow = enable;

    emit this->SI_shadowChanged(enable);
}

void CircularProgressBar::setWidth(const int &width) {
    this->width = width;
    QResizeEvent event(this->size(), this->size());
    this->resizeEvent(&event);
    repaint();
    emit this->SI_widthChanged(width);
}

void CircularProgressBar::setHeight(const int &height) {
    this->height = height;
    QResizeEvent event(this->size(), this->size());
    this->resizeEvent(&event);
    repaint();
    emit this->SI_heightChanged(height);
}

void CircularProgressBar::setTextAlignment(const Qt::Alignment &alignment) {
    this->textAlignment = alignment;
    repaint();
    emit this->SI_textAlignmentChanged(alignment);
}

void CircularProgressBar::setProgressAlignment(const Qt::Alignment &alignment) {
    this->progressAlignment = alignment;
    repaint();
    emit this->SI_progressAlignmentChanged(alignment);
}

void CircularProgressBar::setProgressWidth(const int &width) {
    this->progress_width = width;
    repaint();
    emit this->SI_progressWidthChanged(width);
}

void CircularProgressBar::setProgressRoundedCap(const bool &enable) {
    this->progress_rounded_cap = enable;
    repaint();
    emit this->SI_progressCapChanged(enable);
}

void CircularProgressBar::setEnableBg(const bool &enable) {
    this->enable_bg = enable;
    repaint();
    emit this->SI_backgroundChanged(enable);
}

void CircularProgressBar::setBgColor(const QColor &color) {
    this->bg_color = color;
    repaint();
    emit this->SI_backgroundColorChanged(color);
}

void CircularProgressBar::setEnableText(const bool &enable) {
    this->enable_text = enable;
    repaint();
    emit this->SI_textEnableChanged(enable);
}

void CircularProgressBar::setSuffix(const QString &suffix) {
    this->suffix = suffix;
    repaint();
    emit this->SI_suffixChanged(suffix);
}

void CircularProgressBar::setTextColor(const QColor &color) {
    this->text_color = color;
    repaint();
    emit this->SI_textColorChanged(color);
}

void CircularProgressBar::setSquare(const bool &enable) {
    this->square = enable;
    QResizeEvent event(this->size(), this->size());
    this->resizeEvent(&event);
    emit this->SI_squareChanged(enable);
}

void CircularProgressBar::setGradient(const bool &enable) {
    this->gradient = enable;
    repaint();
    emit this->SI_squareChanged(enable);
}

void CircularProgressBar::setGradientValues(const QMap<qreal, QColor> &colors) {
    this->gradient_colors = colors;
    repaint();
    emit this->SI_gradientValuesChanged(colors);
}

bool CircularProgressBar::isStop() const {
    return m_timer->isActive();
}

QPushButton *CircularProgressBar::button() const
{
    return stopButton;
}
