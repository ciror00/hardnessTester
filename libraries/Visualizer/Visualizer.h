#include <Arudino.h>
#include "SSD1306Wire.h"
#include "Images.h"

enum Images {LOGO,PUSH};

class Visualizer{
  SSD1306Wire display;
  public:
    Visualizer() = default;
    void begin(const uint8_t address = 0x3c, const uint8_t sda, const uint8_t scl);
    void showMessage();
    void showMeassure();
    void showImage()
    void disclaimer();
    ~Visualizer() = default;
}
