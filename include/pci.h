struct pciDevice {
  uint16_t vendorID;
  uint16_t deviceID;

  uint16_t command;
  uint16_t status;

  uint8_t revisionID;
  uint8_t progIF;
  uint8_t subclass;
  uint8_t class;

  uint8_t cacheLineSize;
  uint8_t latencyTime;
  uint8_t headerType;
  uint8_t BIST;

  uint32_t BAR0;
  uint32_t BAR1;
  uint32_t BAR2;
  uint32_t BAR3;
  uint32_t BAR4;
  uint32_t BAR5;

  uint32_t cardBusCisPointer;

  uint16_t subsystemVendorID;
  uint16_t  subsystemID;

  uint32_t expansionRomBaseAddress;

  uint8_t capabilitiesPonter;
  uint8_t RES1;
  uint16_t RES2;

  uint32_t RES3;
  
  uint8_t interruptLine;
  uint8_t interruptPIN;
  uint8_t minGrant;
  uint8_t maxLatency;
};

extern struct pciDevice bus0[32];

int enumPci();
int pciCheckFunc(uint8_t , uint8_t );
