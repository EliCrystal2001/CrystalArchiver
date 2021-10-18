/*
*	 Written by Eli Crystal
*/

#ifndef _CRYSTAL_GUI_
#define _CRYSTAL_GUI_
#include <list>
#include <sstream>
#include <iostream>
#include <string>
#include <cstdio>

#include "CrystalRender.hpp"
#include "CrystalDrm.hpp"

using namespace std;

namespace EliCrystal {

	enum CrystalGuiState{
		CrystalStateDecrypting,
		CrystalStateUploadDialog,
		CrystalStateDone
	};

	class CrystalGui{
		private:
			CrystalGuiState GuiState;
			CrystalDrm* Drm;
			CrystalRender* Renderer;
			list<string> PsmGameList;
			list<string>::iterator PsmGameSelection;
			
			CrystalTexture CurrentIcon;
			CrystalTexture DefaultIcon;
			CrystalTexture IconOverlay;
			CrystalTexture DialogOverlay;
			string CurrentTitleid;
			string CurrentContentId;
			
			bool UploadLicenses;
			
			uint64_t TotalPsmGames;
			uint64_t TotalDecrypted;
			uint64_t TotalErrored;
			uint64_t TotalProcessed;
			
			void InitCrystalDrm();
			void InitCrystalRender();
			void InitCrystalGui();
		public:
			CrystalGui();
			void Update();
			void Render();
	};
}


#endif
