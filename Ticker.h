typedef struct {
  String symbol;
  String label;
  String currency;
  int value;
  int change;
  unsigned long lastUpdatedAt; // millis of the last update
} Ticker;

// TODO Load from file?
Ticker Tickers[] {
    {"APC.F",  "Apple",      "EUR", 0, 0, 0},
    {"ABNB",   "Airbnb",     "USD", 0, 0, 0},
    {"AMZ.F",  "Amazon",     "EUR", 0, 0, 0},
    {"NVD.F",  "NVIDIA",     "EUR", 0, 0, 0},
    {"IQQW.F", "MSCI World", "EUR", 0, 0, 0}
};

const uint8_t TickersCount = sizeof(Tickers)/sizeof(Ticker);
