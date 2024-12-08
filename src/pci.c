// Copyright 2022-2024; for authors see bottom
// Licence: MIT

#include "pci.h"
#include "cpu.h"
#include "serial.h"
#include "string.h"


#define PCI_CONFIG_ADDRESS	0xCF8
#define PCI_CONFIG_DATA		0xCFC


void checkBus(uint8_t bus);

uint16_t pciConfigReadWord(uint8_t bus, uint8_t slot, uint8_t func, uint8_t offset) {
	uint32_t address;
	uint32_t lbus  = (uint32_t)bus & 127;
	uint32_t lslot = (uint32_t)slot & 31;
	uint32_t lfunc = (uint32_t)func & 7;
	uint16_t tmp = 0;

	// Create configuration address as per Figure 1
	address = (uint32_t)((lbus << 16) | (lslot << 11) |
				(lfunc << 8) | (offset & 0xFC) | ((uint32_t)0x80000000));

	// Write out the address
	cpu_out32(PCI_CONFIG_ADDRESS, address);

	// Read in the data
	// (offset & 2) * 8) = 0 will choose the first word of the 32-bit register
	tmp = (uint16_t)((cpu_in32(PCI_CONFIG_DATA) >> ((offset & 2) * 8)) & 0xFFFF);
	return tmp;
}
uint32_t pciConfigReadDWord(uint8_t bus, uint8_t slot, uint8_t func, uint8_t offset) {
	uint32_t address;
	uint32_t lbus  = (uint32_t)bus & 127;
	uint32_t lslot = (uint32_t)slot & 31;
	uint32_t lfunc = (uint32_t)func & 7;
	uint32_t tmp = 0;

	// Create configuration address as per Figure 1
	address = (uint32_t)((lbus << 16) | (lslot << 11) |
				(lfunc << 8) | (offset & 0xFC) | ((uint32_t)0x80000000));

	// Write out the address
	cpu_out32(PCI_CONFIG_ADDRESS, address);

	// Read in the data
	// (offset & 2) * 8) = 0 will choose the first word of the 32-bit register
	tmp = (uint32_t)((cpu_in32(PCI_CONFIG_DATA) >> ((offset & 2) * 8)));
	return tmp;
}



/*
uint16_t pciCheckVendor(uint8_t bus, uint8_t slot) {
	uint16_t vendor, device;
	// Try and read the first configuration register. Since there are no
	// vendors that == 0xFFFF, it must be a non-existent device.
	if ((vendor = pciConfigReadWord(bus, slot, 0, 0)) != PCI_VENDOR_None) {
		device = pciConfigReadWord(bus, slot, 0, 2);
	}
	return (vendor);
}
*/

uint16_t getVendorID(uint8_t bus, uint8_t device, uint8_t function) {
	return pciConfigReadWord(bus, device, function, PCI_Header_VendorId);
}
uint8_t getBaseClass(uint8_t bus, uint8_t device, uint8_t function) {
	return pciConfigReadWord(bus, device, function, 0x0A) >> 8;
}
uint8_t getSubClass(uint8_t bus, uint8_t device, uint8_t function) {
	return pciConfigReadWord(bus, device, function, 0x0A) & 255;
}
uint8_t getSecondaryBus(uint8_t bus, uint8_t device, uint8_t function) {
	return 0;  // das sieht komisch aus
}
uint8_t getHeaderType(uint8_t bus, uint8_t device, uint8_t function) {
	return pciConfigReadWord(bus, device, function, 0x0E);
}

void checkFunction(uint8_t bus, uint8_t device, uint8_t function) {
	uint8_t baseClass = getBaseClass(bus, device, function);
	uint8_t subClass = getSubClass(bus, device, function);
	
	kprintf("pci: found! bus %d device %d function %d is %d:%d\n", bus, device, function, baseClass, subClass);
	
	if ((baseClass == PCI_CLASS_Bridge) && (subClass == PCI_SUBCLASS_Bridge_PCItoPCI)) {
		uint8_t secondaryBus = getSecondaryBus(bus, device, function);
		kprintf("pci: checking secondaryBus %d\n", secondaryBus);
		checkBus(secondaryBus);
	}
 }
 
void checkDevice(uint8_t bus, uint8_t device) {
	
	uint16_t vendorID = getVendorID(bus, device, 0);
	if (vendorID == PCI_VENDOR_None) return; // Device doesn't exist

	uint16_t deviceType = pciConfigReadWord(bus, device, 0, 2);
	kprintf("pci: found! bus %d device %d with vendorID %x, dt %x\n", bus, device, vendorID, deviceType);
	
	if (vendorID == PCI_VENDOR_qemu && deviceType == 0x1111) {
		uint32_t bar0 = pciConfigReadDWord(bus, device, 0, PCI_Headertype00_BAR0);
		kprintf( "BAR0 ist %x\n", bar0);
	}
	
	uint8_t function = 0;
	checkFunction(bus, device, function);
	uint8_t headerType = getHeaderType(bus, device, function);
	if( (headerType & 0x80) != 0) {
		// It's a multi-function device, so check remaining functions
		for (function = 1; function < 8; function++) {
			if (getVendorID(bus, device, function) != PCI_VENDOR_None) {
				checkFunction(bus, device, function);
			}
		}
	}
}

void checkBus(uint8_t bus) {
	uint8_t device;
	
	for (device = 0; device < 32; device++) {
		checkDevice(bus, device);
	}
}

void checkAllBuses(void) {
	uint8_t function;
	uint8_t bus;
	uint8_t headerType;

	headerType = getHeaderType(0, 0, 0);
	if ((headerType & 0x80) == 0) {
		// Single PCI host controller
		kputs("pci: single PCI host controller\n");
		checkBus(0);
	} else {
		// Multiple PCI host controllers
		kputs("pci: multiple PCI host controller\n");
		for (function = 0; function < 8; function++) {
			if (getVendorID(0, 0, function) != PCI_VENDOR_None) break;
			bus = function;
			checkBus(bus);
		}
	}
}


void pci_init() {
	checkAllBuses();
}



/* ---- Authors (in alphabetical order) ----
 * Andreas Herzig
 */