/*
*	 Written by Eli Crystal
*/

#ifndef _CRYSTAL_DRM_
#define _CRYSTAL_DRM_
#include <vita2d.h>
#include <vitasdk.h>

using namespace std;

namespace EliCrystal {
	
	typedef uint64_t CrystalAccountId;
	
	class CrystalDrm{
		private:
			char RifName[0x30];
			void Init();
		public:
			CrystalDrm();
			static CrystalAccountId GetCurrentAccountId();
			ScePsmDrmLicense LoadLicense(string titleid);
			ScePsmDrmLicense DecryptLicense(ScePsmDrmLicense license);
	};
}


#endif
