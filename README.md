# CapMan

## Getting Started

These instructions will get you a copy of the project up and running on your local machine for development and testing purposes.

### Prerequisites

CapMan is depended on two major frameworks, Qt & ROOT.

* [Qt](http://www.qt.io) - The overall framework used to manage everything
* [ROOT](https://root.cern.ch) - BDT Training 

### Installing

After installing the prerequisties, compile by:

```
qmake CapMan.pro
make
```

### Running

The current running modes are controlled by commenting in/out lines in the main file (should be as a cmd line arg in the future). The different options are:
* Regular Work - main windows up, GUI based operation for bulk application of the trained BDT or building image meta-data for training and classification.
* MVA Training - training the BDT on a list of given ROOT based image meta-data

## Authors

* **Hadar Cohen** 