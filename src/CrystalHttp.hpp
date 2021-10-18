/*
*	 Written by Eli Crystal
*/

#ifndef _CRYSTAL_HTTP_
#define _CRYSTAL_HTTP_
#include <vita2d.h>
#include <vitasdk.h>
#include "CrystalFileData.hpp"

using namespace std;

namespace EliCrystal {
	typedef int SceHttpTemplate;
	typedef int SceHttpConnection;
	typedef int SceHttpRequest;
	
	class CrystalHttp{
		public:
			static void Init();
			static bool UploadData(string Url, CrystalFileData* Data);
	};
}


#endif


