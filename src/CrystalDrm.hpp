/*
*	 Written by Eli Crystal
*/

#ifndef _CRYSTAL_DRM_
#define _CRYSTAL_DRM_
#include <vita2d.h>
#include <vitasdk.h>

using namespace std;

namespace EliCrystal {
	/*
	*	Lifted from NoPsmDrm
	*/
	typedef struct CrystalPsmLicense
	{
		char magic[0x8];             // 0x00
		uint32_t unk1;               // 0x08
		uint32_t unk2;               // 0x0C
		uint64_t aid;                // 0x10
		uint32_t unk3;               // 0x18
		uint32_t unk4;               // 0x1C
		uint64_t start_time;         // 0x20
		uint64_t expiration_time;    // 0x28
		uint8_t act_digest[0x20];    // 0x30
		char content_id[0x30];       // 0x50
		uint8_t unk5[0x80];          // 0x80
		uint8_t key[0x200];          // 0x100
		uint8_t sha256digest[0x100]; // 0x300
	} CrystalPsmLicense;
	
	typedef uint64_t CrystalAccountId;
	
	class CrystalDrm{
		private:
			char RifName[0x30];
			void Init();
		public:
			CrystalDrm();
			static CrystalAccountId GetCurrentAccountId();
			CrystalPsmLicense LoadLicense(string titleid);
			CrystalPsmLicense DecryptLicense(CrystalPsmLicense license);
	};
}


#endif
