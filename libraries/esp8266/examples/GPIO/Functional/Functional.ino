//#include "FunctionalInterrupt.h"

#if defined(ESP32)
#define BUTTON1 16
#define BUTTON2 17
#elif defined(ARDUINO_ESP8266_WEMOS_D1MINI)
#define BUTTON1 D4
#define BUTTON2 D3
#else
#define BUTTON1 2
#define BUTTON2 0
#endif

class Button {
  public:
    Button(uint8_t reqPin) : PIN(reqPin) {
      pinMode(PIN, INPUT_PULLUP);
      // Arduino C API:
      //attachInterruptArg(PIN, [](void* self) {
      //  static_cast<Button*>(self)->buttonIsr();
      //}, this, FALLING); // fails on ESP8266: "buttonIsr not in IRAM"
      //attachInterruptArg(PIN, reinterpret_cast<void(*)(void*)>(&buttonIsr_static), this, FALLING); // works on ESP8266
      attachInterrupt(PIN, [this]() {
        buttonIsr();
      }, FALLING); // works on ESP8266
      // FunctionalInterrupt.h API:
      //attachScheduledInterrupt(PIN, [this](const InterruptInfo & ii) {
      //  Serial.print("Pin ");
      //  Serial.println(ii.pin);
      //  buttonIsr();
      //}, FALLING); // works on ESP8266
    };
    ~Button() {
      detachInterrupt(PIN);
    }

#if defined(ESP8266)
    void ICACHE_RAM_ATTR buttonIsr()
#elif defined(ESP32)
    void IRAM_ATTR buttonIsr()
#endif
    {
      numberKeyPresses += 1;
      pressed = true;
    }

#if defined(ESP8266)
    static void ICACHE_RAM_ATTR buttonIsr_static(Button* const self)
#elif defined(ESP32)
    static void IRAM_ATTR buttonIsr_static(Button* const self)
#endif
    {
      self->buttonIsr();
    }

    uint32_t checkPressed() {
      if (pressed) {
        Serial.printf("Button on pin %u has been pressed %u times\n", PIN, numberKeyPresses);
        pressed = false;
      }
      return numberKeyPresses;
    }

  private:
    const uint8_t PIN;
    volatile uint32_t numberKeyPresses = 0;
    volatile bool pressed = false;
};

Button* button1;
Button* button2;


void setup() {
  Serial.begin(115200);
  Serial.println("FunctionalInterrupt test/example");

  button1 = new Button(BUTTON1);
  button2 = new Button(BUTTON2);

  Serial.println("setup() complete");
}

void loop() {
  button1->checkPressed();
  if (nullptr != button2 && 10 < button2->checkPressed()) {
    delete button2;
    button2 = nullptr;
  }
}
