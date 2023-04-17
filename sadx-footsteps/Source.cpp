#include "pch.h"
// or #include "stdafx.h" for previous Visual Studio versions

FunctionHook<void, task*> Sonic_Display_h(0x4948C0);

// did we already play footsteps?
bool playedStep = false;

int FootstepSounds[] = {
	SE_LAND,  // concrete?
	SE_P_FT1, // dirt?
	SE_P_FT2, // wood?
	SE_P_FT3, // grass? (default)
	SE_P_FT4, // sand?
	SE_P_FT5, // duplicate
	SE_P_FT6, // duplicate
	SE_P_FT7, // duplicate
	SE_P_FT8, // duplicate
};

static void playFootsteps(playerwk* pwp, NJS_VECTOR pos)
{
	int vol = -64;
	switch (pwp->mj.action) {
	case 9:
		if ((pwp->mj.nframe >= 25 && pwp->mj.nframe <= 30) || (pwp->mj.nframe >= 10 && pwp->mj.nframe <= 15))
		{
			if (!playedStep)
			{
				dsPlay_oneshot_v(FootstepSounds[0], 0, 0, vol, pos.x, pos.y, pos.z);
				playedStep = true;
			}
		}
		else
		{
			playedStep = false;
		}
		break;
	case 10:
	case 11:
	case 12:
	case 13:
		if ((pwp->mj.nframe >= 30 || pwp->mj.nframe <= 2) || (pwp->mj.nframe >= 14 && pwp->mj.nframe <= 16))
		{
			if (!playedStep)
			{
				dsPlay_oneshot_v(FootstepSounds[0], 0, 0, vol, pos.x, pos.y, pos.z);
				playedStep = true;
			}
		}
		else
		{
			playedStep = false;
		}
		break;
	}
}


static void Sonic_Display_r(task* tp)
{
	// hook the original display function
	Sonic_Display_h.Original(tp);
	// various variable declarations, to make referencing the original memory locations simpler
	auto twp = tp->twp;
	auto pwp = (playerwk*)tp->mwp->work.ptr;
	// get player position
	NJS_VECTOR pos = twp->pos;
	// if on the ground, do fire check
	if (EntityData1Ptrs[0]->Status & (Status_Ground))
	{
		playFootsteps(pwp, pos);
	}
}


extern "C"
{
	__declspec(dllexport) void __cdecl Init(const char* path, const HelperFunctions& helperFunctions)
	{
		// Executed at startup, contains helperFunctions and the path to your mod (useful for getting the config file.)
		// This is where we override functions, replace static data, etc.

		Sonic_Display_h.Hook(Sonic_Display_r);
	}

	__declspec(dllexport) ModInfo SADXModInfo = { ModLoaderVer }; // This is needed for the Mod Loader to recognize the DLL.
}
