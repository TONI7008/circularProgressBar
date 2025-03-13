# Circular Progress Bar

A modern and customizable circular progress bar implemented in C++ with Qt framework.

## Features
- Customizable size and appearance
- Smooth progress updates
- Easy integration into C++ projects

## Installation
1. Clone this repository:
   ```sh
   git clone git@github.com:TONI7008/circularProgressBar.git
   cd circular-progress-bar
   ```
2. Include `circularprogressbar.h` and `circularprogressbar.cpp` in your project.

## Usage

Include the header file and use the progress bar in your C++ application:

```cpp
#include <QCoreApplication>
#include <QApplication>
#include "circularprogressbar.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    CircularProgressBar *bar=new CircularProgressBar(nullptr);

    //uncoment to see it moving
    //bar->setInfiniteLoop(true);
    bar->setProgressAlignment(Qt::AlignCenter);
    bar->setRange(0,1000);
    bar->setEnableText(true);
    bar->setBgColor(Qt::lightGray);
    bar->setChunkColor(QColor(255,255,197));
    bar->setValue(718);
    bar->show();

    short d=app.exec();

    delete bar;
    return d;
}

```

## Contributing
Contributions are welcome! Please follow these steps:
1. Fork the repository.
2. Create a new branch (`feature-branch`)
3. Commit your changes.
4. Push to the branch and create a Pull Request.

## License
This project is licensed under the GNU License.

