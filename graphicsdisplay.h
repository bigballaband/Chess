#include <iostream>
#include <vector>
#include <sstream>
#include "observer.h"
#include "window.h"
#include "piece.h"

class GraphicsDisplay: public Observer {
        Xwindow &xw;
    public:
        GraphicsDisplay(Xwindow &xw);
        ~GraphicsDisplay() = default;
        void notify(Cell &c) override;
};
