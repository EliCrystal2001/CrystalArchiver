/*
*	 Written by Eli Crystal
*/

#include <sstream>
#include <iostream>
#include <vector>
#include <cstdio>
#include "CrystalHttp.hpp"

/*
*	HTTP Class
*	Sends Network Requests
*/
namespace EliCrystal {

	/*
	*	Method to setup LibHttp and LibNet
	*/
	void CrystalHttp::Init(){
		sceClibPrintf("Starting SceNet.\n");
		sceSysmoduleLoadModule(SCE_SYSMODULE_NET);	
		SceNetInitParam netInitParam;
		int size = 1*1024*1024;
		netInitParam.memory = malloc(size);
		netInitParam.size = size;
		netInitParam.flags = 0;
		sceNetInit(&netInitParam);
		sceNetCtlInit();

		sceClibPrintf("Starting SceHttp.\n");
		sceSysmoduleLoadModule(SCE_SYSMODULE_HTTPS);
		sceHttpInit(1*1024*1024);
	}
	
	/*
	*	Upload data to a server.
	*/
	bool CrystalHttp::UploadData(string Url, CrystalFileData* Data){
		SceHttpTemplate Template = sceHttpCreateTemplate("CrystalHttp", 1, 1);
		SceHttpConnection Connection = sceHttpCreateConnectionWithURL(Template, Url.c_str(), 0);
		SceHttpRequest Request = sceHttpCreateRequestWithURL(Connection, SCE_HTTP_METHOD_POST, Url.c_str(), Data->FileSize);
		
		int Res = sceHttpSendRequest(Request, Data->FileData, Data->FileSize);
		
		sceHttpDeleteTemplate(Template);
		sceHttpDeleteConnection(Connection);
		sceHttpDeleteRequest(Request);
		
		if(Res >= 0)
			return true;
		return false;
	}

}

