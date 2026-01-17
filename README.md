# embedPython

Embedding the Python Interpreter in a Qt/C++ Application.

This repository demonstrates how to integrate Python directly into a Qt-based C++ application using the Python/C API. The code is developed step by step in my tutorial at [ubuverse.com/embedding-the-python-interpreter-in-a-qt-application](https://ubuverse.com/embedding-the-python-interpreter-in-a-qt-application/).

## Prerequisites

- Qt 6 (development libraries)
- Python 3 development headers
- C++ build tools

On Debian/Ubuntu-based systems, you can install everything with:

```shell
sudo apt install build-essential qtcreator qt6-base-dev python3-dev
```

## Build & Run

1. Build and Run
2. Open Qt Creator
3. Open the project file from this repository
4. Configure the project using a Qt 6 kit
5. Build and run the application

The tutorial linked above explains each step in detail.

## Packing Python's Standard Library

The Python script in `python/compile.py` is used to collect and compile the subset of Python's standard library required by your program and package it into a ZIP archive. This file must be distributed alongside the Qt application binary so the embedded Python interpreter can load the necessary modules at runtime. See the tutorial for more details.

## License

GPL-3.0
