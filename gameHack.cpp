// gameHack.cpp : This file contains the 'main' function. Program execution begins and ends there.

#include <iostream>
#include "proc.h"
#include <vector>
#include <Windows.h>
#include <conio.h>

#define HEALTH_OFFSET 0xEC
#define AMMO_OFFSET 0x140
#define ARMOUR_OFFSET 0x00F0
#define SPEED_OFFSET_X 0x1C
#define SPEED_OFFSET_Y 0x20
#define SPEED_OFFSET_Z 0x24

int main()
{
	// Get ProcID of the target process
	DWORD procID = GetProcID(L"ac_client.exe");
	
	// Get ModuleBaseAddress

	uintptr_t moduleBase = GetModuleBaseAddress(procID, L"ac_client.exe");

	std::cout << "Module Base: " << "0x" << std::hex << moduleBase << std::endl;

	// Get Handle to Process

	HANDLE hProcess = 0;
	hProcess = OpenProcess(PROCESS_ALL_ACCESS, NULL, procID);

	// Resolve Base Address of the pointer chain

	uintptr_t dynamicPtrBaseAddr = moduleBase + 0x0017E0A8;


	std::cout << "Dynamic Pointer Addr: " << "0x" << std::hex << dynamicPtrBaseAddr << std::endl;

	// Resolve our ammo pointer chain
	std::vector<unsigned int> ammoOffsets = {AMMO_OFFSET};
	std::vector<unsigned int> healthOffsets = {HEALTH_OFFSET};
	std::vector<unsigned int> armourOffsets = {ARMOUR_OFFSET};

	std::vector<unsigned int> speedXoffsets = {SPEED_OFFSET_X};
	std::vector<unsigned int> speedYoffsets = {SPEED_OFFSET_Y};
	std::vector<unsigned int> speedZoffsets = {SPEED_OFFSET_Z};

	
	uintptr_t ammoAddr = FindDMAAddy(hProcess, dynamicPtrBaseAddr, ammoOffsets);
	uintptr_t healthAddr = FindDMAAddy(hProcess, dynamicPtrBaseAddr, healthOffsets);
	uintptr_t armourAddr= FindDMAAddy(hProcess, dynamicPtrBaseAddr, armourOffsets);
	
	uintptr_t speedXaddr = FindDMAAddy(hProcess, dynamicPtrBaseAddr, speedXoffsets);
	uintptr_t speedYaddr = FindDMAAddy(hProcess, dynamicPtrBaseAddr, speedYoffsets);
	uintptr_t speedZaddr = FindDMAAddy(hProcess, dynamicPtrBaseAddr, speedZoffsets);
	/*
	std::cout << "Ammo Address: " << "0x" << std::hex << ammoAddr << std::endl;
	std::cout << "Health Address: " << "0x" << std::hex << ammoAddr << std::endl;

	// Read ammo value

	int ammoValue;
	
	ReadProcessMemory(hProcess, (BYTE*)ammoAddr, &ammoValue, sizeof(ammoValue), nullptr);
	std::cout << "Ammo count: " << std::dec << ammoValue << std::endl;

	*/

	// Write to it
	int newAmmo = 1999;
	int newHealth = 1000;
	int newArmour = 1000;

	int speedX = 0;

	int speedMultiplier = 2;

	while (1) {
		if (_kbhit()) {
			std::cout << "\nKey Pressed! Exiting Loop...\n";
			break;
		}

		WriteProcessMemory(hProcess, (BYTE*)ammoAddr, &newAmmo, sizeof(newAmmo), nullptr);
		WriteProcessMemory(hProcess, (BYTE*)healthAddr, &newHealth, sizeof(newHealth), nullptr);
		WriteProcessMemory(hProcess, (BYTE*)armourAddr, &newArmour, sizeof(newArmour), nullptr);

		ReadProcessMemory(hProcess, (BYTE*)speedXaddr, &speedX, sizeof(speedX), nullptr);
		speedX *= speedMultiplier;
		WriteProcessMemory(hProcess, (BYTE*)speedXaddr, &speedX, sizeof(speedX), nullptr);
	}
	return 0;
}

