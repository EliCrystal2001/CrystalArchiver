/*
*	 Written by Eli Crystal
*/

#include <sstream>
#include <iostream>
#include <vector>
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
	CrystalPsmLicense CrystalDrm::LoadLicense(string titleid){
		CrystalPsmLicense PsmLicense;
		memset(&PsmLicense, 0x00, sizeof(CrystalPsmLicense));
		
		string RifPath = "ux0:/psm/"+titleid+"/RO/License/"+this->RifName;
		sceClibPrintf("Opening RIF: %s\n", RifPath.c_str());
		
		CrystalFileData* Data = CrystalIo::ReadFile(RifPath);
		
		if(Data->FileSize >= sizeof(CrystalPsmLicense)){
			memcpy(&PsmLicense, Data->FileData, sizeof(CrystalPsmLicense));
		}
		else{
			sceClibPrintf("Failed to load rif: %s\n", RifPath.c_str());
		}
		
		return PsmLicense;
	}
	
	/*
	*	Method to decrypt PSM License Files
	*/
	CrystalPsmLicense CrystalDrm::DecryptLicense(CrystalPsmLicense license){
		CrystalPsmLicense PsmLicense;
		memset(&PsmLicense, 0x00, sizeof(CrystalPsmLicense));
		
		char KeyData[0x200];
		memset(KeyData, 0x00, sizeof(KeyData));
		
		int Error = scePsmDrmGetRifKey((char*)&license, KeyData, 0);
		
		if(Error >= 0){
			PsmLicense.aid = FakeAid;
			PsmLicense.unk1 = __builtin_bswap32(1);
			
			memcpy(PsmLicense.content_id, license.content_id, 0x30);
			memcpy(PsmLicense.key, KeyData, sizeof(KeyData));
			
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

