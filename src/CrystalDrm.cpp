/*
*	 Written by Eli Crystal
*/

#include <sstream>
#include <iostream>
#include <vector>
#include <cstring>
#include <cstdio>
#include "CrystalDrm.hpp"
#include "CrystalIo.hpp"
#define FakeAid 0x0123456789ABCDEFLL

/*
*	DRM Class
*	Handles PsmDrm calls and stuff
*/
namespace EliCrystal {
	/*
	*	Method to initalize CrystalDRM
	*/
	void CrystalDrm::Init(){
		sceClibPrintf("Setting up CrystalDrm..\n");
		CrystalAccountId Aid = CrystalDrm::GetCurrentAccountId();
		_sceNpDrmGetRifName(this->RifName, Aid); 
		sceClibPrintf("Rif name: %s\n", this->RifName);
	}
	
	/*
	*	Class Constructor
	*	This gets called when the class is initalized.
	*/
	CrystalDrm::CrystalDrm(){
		this->Init();
	}
	
	/*
	*	Method to read a License File
	*/
	ScePsmDrmLicense CrystalDrm::LoadLicense(string titleid){
		ScePsmDrmLicense PsmLicense;
		memset(&PsmLicense, 0x00, sizeof(ScePsmDrmLicense));
		
		string RifPath = "ux0:/psm/"+titleid+"/RO/License/"+this->RifName;
		sceClibPrintf("Opening RIF: %s\n", RifPath.c_str());
		
		CrystalFileData* Data = CrystalIo::ReadFile(RifPath);
		
		if(Data->FileSize >= sizeof(ScePsmDrmLicense)){
			memcpy(&PsmLicense, Data->FileData, sizeof(ScePsmDrmLicense));
		}
		else{
			sceClibPrintf("Failed to load rif: %s\n", RifPath.c_str());
		}
		
		return PsmLicense;
	}
	
	/*
	*	Method to decrypt PSM License Files
	*/
	ScePsmDrmLicense CrystalDrm::DecryptLicense(ScePsmDrmLicense license){
		ScePsmDrmLicense PsmLicense;
		memset(&PsmLicense, 0x00, sizeof(ScePsmDrmLicense));
		
		ScePsmDrmKeySet KeySet;
		ScePsmDrmExpireTime ExpireTime;
		int Flags = 0;
		
		memset(&ExpireTime, 0x00, sizeof(ScePsmDrmExpireTime));
		memset(&KeySet, 0x00, sizeof(ScePsmDrmKeySet));
		
		int Error = scePsmDrmGetRifKey(&license, &KeySet, &Flags, &ExpireTime);
		
		if(Error >= 0){
			PsmLicense.account_id = FakeAid;
			PsmLicense.unk1 = __builtin_bswap32(1);
			
			memcpy(PsmLicense.content_id, license.content_id, 0x30);
			memcpy(&PsmLicense.keyset, &KeySet, sizeof(KeySet));
			
			sceClibPrintf("Decrypted: %s\n", PsmLicense.content_id);
		}
		else{
			sceClibPrintf("Failed to decrypt rif: %x\n", Error);
		}
		
		return PsmLicense;
	}
	
	/*
	*	Method to get the current account id
	*/
	CrystalAccountId CrystalDrm::GetCurrentAccountId(){
		CrystalAccountId Aid = 0;
		int Res = sceRegMgrGetKeyBin("/CONFIG/NP", "account_id", &Aid, sizeof(CrystalAccountId));
		if(Res >= 0){
			return Aid;
		}
		sceClibPrintf("Failed to get account id ... %x\n", Res);
		
		return 0;
	}
	
}

