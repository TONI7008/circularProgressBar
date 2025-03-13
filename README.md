# Circular Progress Bar

A modern and customizable circular progress bar implemented in C++.

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
#include "circularprogressbar.h"

int main() {
    CircularProgressBar progressBar(100); // Initialize with max value of 100
    progressBar.setValue(50); // Set progress to 50%
    progressBar.render(); // Render the progress bar
    return 0;
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

