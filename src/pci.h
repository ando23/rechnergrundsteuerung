
void pci_init();

#define PCI_VENDOR_None		0xFFFF
#define PCI_VENDOR_Intel	0x8086
// Intel E1000		8086:100E
// Intel AC97		8086:2415
#define PCI_VENDOR_qemu		0x1234
// Bochs Display	1234:1111
#define PCI_VENDOR_Realtec	0x10EC
// RTL8139 			10EC:8139


#define PCI_Header_VendorId			0x00
#define PCI_Header_DeviceId			0x02
#define PCI_Header_Command			0x04
#define PCI_Header_Status			0x06
#define PCI_Headertype00_BAR0		0x10
#define PCI_Headertype00_BAR1		0x14
#define PCI_Headertype00_BAR2		0x18
#define PCI_Headertype00_BAR3		0x1C
#define PCI_Headertype00_BAR4		0x29
#define PCI_Headertype00_BAR5		0x24
#define PCI_Headertype00_IRQ		0x3C


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

