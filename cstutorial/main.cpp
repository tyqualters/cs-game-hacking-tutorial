// Windows Standard Header File
#include <Windows.h>
// Memory Management Utilities
#include "utilities.h"
// Reversed SDK Utilities
#include "SDK.h"
// Hazedumper
#include "csgo.hpp"

#include <cstdio>

using namespace hazedumper::signatures;
using namespace hazedumper::netvars;

DWORD CheatTutorial(HMODULE hModule)
{
	AllocConsole();
	FILE* f;
	freopen_s(&f, "CONOUT$", "w", stdout);

	printf("Successfully started!\n");

	// Initialize values
	printf("Initializing variables...\n");
	uintptr_t client_module = (uintptr_t)GetModuleHandle(TEXT("client_panorama.dll"));
	uintptr_t engine_module = (uintptr_t)GetModuleHandle(TEXT("engine.dll"));
	C_BasePlayer* pLocalPlayer = nullptr;
	uintptr_t pEntity = NULL;
	CBaseEntity* selectedEntity = nullptr;

	while (true)
	{
		// Check if we want to break
		if (GetAsyncKeyState(VK_END)) break;

		// Check if we are in game
		if (dereference_pointer<int>(dereference_pointer<uintptr_t>(engine_module + dwClientState) + dwClientState_State) != 6)
		{
			// Sleep for a good while, not in game yet!
			Sleep(200);
			continue;
		}

		// Check if LocalPlayer exists
		if ((pLocalPlayer = dereference_pointer<C_BasePlayer*>(client_module + dwLocalPlayer)) == nullptr)
			// -- (pLocalPlayer = *(C_BasePlayer**)(client_module + dwLocalPlayer) == nullptr)
			// -- (pLocalPlayer = *reinterpret_cast<C_BasePlayer**>(client_module + dwLocalPlayer) == nullptr)
		{
			// Sleep so CPU can relax a little, but in game
			Sleep(50);
			continue;
		}

		printf("Resetting values\n");
		// Reset values
		pEntity = NULL;
		selectedEntity = nullptr;

		// Check if can hop
		if (pLocalPlayer->Flags & (1 << 0))
		{
			printf("Hopping\n");
			// Holding space
			if (GetAsyncKeyState(VK_SPACE) && dereference_pointer<int>(client_module + dwForceJump) == 4)
				set_pointer<int>(client_module + dwForceJump, 5);
		}
		else
		{
			// Stop hops
			if (dereference_pointer<int>(client_module + dwForceJump) >= 5)
				set_pointer<int>(client_module + dwForceJump, 4);
		}

		// Anti-Flash
		if (pLocalPlayer->FlashDuration > 0)
		{
			printf("Flash averted!\n");
			pLocalPlayer->FlashDuration = 0;
		}

		// Entity List 
		for (unsigned short i = 0; i < 32; i++)
		{
			pEntity = dereference_pointer<uintptr_t>(client_module + dwEntityList + i * 0x10);
			if (pEntity == NULL) continue;
			selectedEntity = reinterpret_cast<CBaseEntity*>(pEntity);
			if (selectedEntity->IsDormant) continue;
			else if (selectedEntity->TeamNum != 2 && selectedEntity->TeamNum != 3) continue;
			else if (selectedEntity->Health <= 0) continue;

			printf("Valid entity, doing stuff\n");

			// Set radar
			selectedEntity->Spotted = true;
			selectedEntity->SpottedByMask = true;

			// Glow hack
			if (selectedEntity->TeamNum == pLocalPlayer->TeamNum)
			{
				// Teammate
				glow_t* selectedEntityGlow = reinterpret_cast<glow_t*>(dereference_pointer<uintptr_t>(client_module + dwGlowObjectManager) + (selectedEntity->GlowIndex * 0x38) + 0x4);
				selectedEntityGlow->r = 0.f;
				selectedEntityGlow->g = 0.f;
				selectedEntityGlow->b = 1.f;
				selectedEntityGlow->a = 1.f;
				selectedEntityGlow->RenderWhenOccluded = true;
				selectedEntityGlow->RenderWhenUnoccluded = false;
			}
			else
			{
				// Enemy
				glow_t* selectedEntityGlow = reinterpret_cast<glow_t*>(dereference_pointer<uintptr_t>(client_module + dwGlowObjectManager) + (selectedEntity->GlowIndex * 0x38) + 0x4);
				selectedEntityGlow->r = 1.f;
				selectedEntityGlow->g = 0.f;
				selectedEntityGlow->b = 0.f;
				selectedEntityGlow->a = 1.f;
				selectedEntityGlow->RenderWhenOccluded = true;
				selectedEntityGlow->RenderWhenUnoccluded = false;
			}

		}

	}


	FreeConsole();
	FreeLibraryAndExitThread(hModule, 0);
	return 0;
}


BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
	if (ul_reason_for_call == DLL_PROCESS_ATTACH)
	{
		HANDLE thread = CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)CheatTutorial, hModule, 0, nullptr);
		if (thread != NULL)
			CloseHandle(thread);
		DisableThreadLibraryCalls(hModule);
	}
	return TRUE;
}