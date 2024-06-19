
void pci_init();

#define PCI_VENDOR_None		0xFFFF

#define PCI_CLASS_Unclassified	0x0
#define PCI_CLASS_Mass_Storage_Controller	0x1
#define PCI_CLASS_Network_Controller	0x2
#define PCI_CLASS_Display_Controller	0x3
#define PCI_CLASS_Multimedia_Controller	0x4
#define PCI_CLASS_Memory_Controller	0x5
#define PCI_CLASS_Bridge	0x6
#define PCI_CLASS_Simple_Communication_Controller	0x7
#define PCI_CLASS_Base_System_Peripheral	0x8
#define PCI_CLASS_Input_Device_Controller	0x9
#define PCI_CLASS_Docking_Station	0xA
#define PCI_CLASS_Processor	0xB
#define PCI_CLASS_Serial_Bus_Controller	0xC
#define PCI_CLASS_Wireless_Controller	0xD
#define PCI_CLASS_Intelligent_Controller	0xE
#define PCI_CLASS_Satellite_Communication_Controller	0xF
#define PCI_CLASS_Encryption_Controller	0x10
#define PCI_CLASS_Signal_Processing_Controller	0x11
#define PCI_CLASS_Processing_Accelator	0x12
#define PCI_CLASS_Nonessential_Instrumention	0x13
#define PCI_CLASS_Coprocessor	0x40
#define PCI_CLASS_Unassigned_Class	0xFF

#define PCI_SUBCLASS_Bridge_Host	0x00
#define PCI_SUBCLASS_Bridge_ISA		0x01
#define PCI_SUBCLASS_Bridge_PCItoPCI	0x04
#define PCI_SUBCLASS_Bridge_PCItoPCI2	0x09