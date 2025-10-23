struct RSDP_t {
 char Signature[8];
 uint8_t Checksum;
 char OEMID[6];
 uint8_t Revision;
 uint32_t RsdtAddress;
} __attribute__ ((packed));

struct ACPISDTHeader {
  char Signature[4];
  uint32_t Length;
  uint8_t Revision;
  uint8_t Checksum;
  char OEMID[6];
  char OEMTableID[8];
  uint32_t OEMRevision;
  uint32_t CreatorID;
  uint32_t CreatorRevision;
};

struct RSDT {
  struct ACPISDTHeader h;
  uint32_t rest;
};


struct  GenericAddressStructure {
  uint8_t AddressSpace;
  uint8_t BitWidth;
  uint8_t BitOffset;
  uint8_t AccessSize;
  uint64_t Address;
};


struct FADT {
  struct   ACPISDTHeader h;
  uint32_t FirmwareCtrl;
  uint32_t Dsdt;

  // field used in ACPI 1.0; no longer in use, for compatibility only
  uint8_t  Reserved;

  uint8_t  PreferredPowerManagementProfile;
  uint16_t SCI_Interrupt;
  uint32_t SMI_CommandPort;
  uint8_t  AcpiEnable;
  uint8_t  AcpiDisable;
  uint8_t  S4BIOS_REQ;
  uint8_t  PSTATE_Control;
  uint32_t PM1aEventBlock;
  uint32_t PM1bEventBlock;
  uint32_t PM1aControlBlock;
  uint32_t PM1bControlBlock;
  uint32_t PM2ControlBlock;
  uint32_t PMTimerBlock;
  uint32_t GPE0Block;
  uint32_t GPE1Block;
  uint8_t  PM1EventLength;
  uint8_t  PM1ControlLength;
  uint8_t  PM2ControlLength;
  uint8_t  PMTimerLength;
  uint8_t  GPE0Length;
  uint8_t  GPE1Length;
  uint8_t  GPE1Base;
  uint8_t  CStateControl;
  uint16_t WorstC2Latency;
  uint16_t WorstC3Latency;
  uint16_t FlushSize;
  uint16_t FlushStride;
  uint8_t  DutyOffset;
  uint8_t  DutyWidth;
  uint8_t  DayAlarm;
  uint8_t  MonthAlarm;
  uint8_t  Century;

  // reserved in ACPI 1.0; used since ACPI 2.0+
  uint16_t BootArchitectureFlags;

  uint8_t  Reserved2;
  uint32_t Flags;

  // 12 byte structure; see below for details
  struct GenericAddressStructure ResetReg;

  uint8_t  ResetValue;
  uint8_t  Reserved3[3];

  // 64bit pointers - Available on ACPI 2.0+
  uint64_t                X_FirmwareControl;
  uint64_t                X_Dsdt;

  struct GenericAddressStructure X_PM1aEventBlock;
  struct GenericAddressStructure X_PM1bEventBlock;
  struct GenericAddressStructure X_PM1aControlBlock;
  struct GenericAddressStructure X_PM1bControlBlock;
  struct GenericAddressStructure X_PM2ControlBlock;
  struct GenericAddressStructure X_PMTimerBlock;
  struct GenericAddressStructure X_GPE0Block;
  struct GenericAddressStructure X_GPE1Block;
};

struct DSDT {
  struct ACPISDTHeader h;
  uint8_t aml;
};

struct RSDP_t* findRSDP();
int initAcpi();
