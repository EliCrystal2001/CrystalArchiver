/*
*	 Written by Eli Crystal
*/

#include <sstream>
#include <iostream>
#include <vector>
#include <cstdio>
#include <taihen.h>
#include "CrystalGui.hpp"
#include "CrystalIo.hpp"
#include "CrystalHttp.hpp"

/*
*	Gui Class
*	Handles Graphical User Interface
*/
namespace EliCrystal {
	/*
	*	Method to initalize CrystalRender;
	*/
	void CrystalGui::InitCrystalRender(){
		sceClibPrintf("Setting up CrystalRender..\n");
		this->Renderer = new CrystalRender();
	}
	
	void CrystalGui::InitCrystalGui(){
		sceClibPrintf("Setting up CrystalGui..\n");
		// Set some defaults.
		this->Renderer->SetDrawColor(0xFF, 0xFF, 0xFF, 0xFF);
		this->Renderer->SetDrawSize(1.0F);
		this->Renderer->SetDrawFont(CrystalPgfFont);
		this->GuiState = CrystalStateUploadDialog;
		
		// Load overlays
		this->IconOverlay = this->Renderer->LoadImage("app0:/fw/icon0_overlay.png");
		this->DialogOverlay = this->Renderer->LoadImage("app0:/fw/dialog_overlay.png");
		this->DefaultIcon = this->Renderer->LoadImage("app0:/fw/default_icon0.png");
		this->CurrentIcon = NULL;
		this->CurrentTitleid = *this->PsmGameSelection;
		
	}
	
	void CrystalGui::InitCrystalDrm(){
		this->Drm = new CrystalDrm();
		this->PsmGameList = CrystalIo::ListDir("ux0:/psm");
		this->PsmGameSelection = this->PsmGameList.begin();
		
		this->TotalPsmGames = this->PsmGameList.size()-1;
		this->TotalDecrypted = 0;
		this->TotalErrored = 0;
		this->TotalProcessed = 0;
	}
	
	/*
	*	Class Constructor
	*	This gets called when the class is initalized.
	*/
	CrystalGui::CrystalGui(){
		this->InitCrystalRender();
		this->InitCrystalDrm();
		this->InitCrystalGui();
	}
	
	/*
	*	Update method
	*	Handles Gui Updates (controls, etc) 
	*/
	void CrystalGui::Update(){
		CrystalPsmLicense PsmRif;
		CrystalPsmLicense PsmRifDec;
		CrystalFileData* Data;		
		SceCtrlData Pad;
		switch(this->GuiState){
			case CrystalStateDecrypting:
								
				PsmRif = Drm->LoadLicense(this->CurrentTitleid);
				PsmRifDec = Drm->DecryptLicense(PsmRif);
				
				if(PsmRifDec.aid != 0){
					Data = new CrystalFileData(sizeof(CrystalPsmLicense), (char*)&PsmRifDec);
				
					CrystalHttp::UploadData("http://psm.elicrystal.ca/submit_rif.php?keep="+to_string(this->UploadLicenses), Data);
					this->CurrentContentId.assign(PsmRifDec.content_id);
					sceIoMkdir("ux0:/data/psm", 0777);
					CrystalIo::WriteFile("ux0:/data/psm/"+CurrentContentId+".rif", Data);
					
					this->TotalDecrypted++;
				}
				else{
					sceKernelDelayThread(0x10000 * 10);
					this->TotalErrored++;
				}
				

				this->PsmGameSelection++;
				this->TotalProcessed++;
				
				if(this->TotalProcessed > this->TotalPsmGames){
					sceKernelDelayThread(0x10000 * 10);
					this->GuiState = CrystalStateDone;
					break;
				}
				
				this->CurrentTitleid = *this->PsmGameSelection;
				break;
			case CrystalStateUploadDialog:
				sceCtrlPeekBufferPositive(0, &Pad, 1);
				switch(Pad.buttons){
					case SCE_CTRL_CROSS:
						this->UploadLicenses = true;
						this->GuiState = CrystalStateDecrypting;
						break;
					case SCE_CTRL_CIRCLE:
						this->UploadLicenses = false;
						this->GuiState = CrystalStateDecrypting;
						break;
				}
				break;
			case CrystalStateDone:
				sceCtrlPeekBufferPositive(0, &Pad, 1);
				switch(Pad.buttons){
					case SCE_CTRL_CROSS:
						sceKernelExitProcess(0);
						break;
				}
				break;
		}
	}
	
	/*
	*	Render method
	*	Renders the current GUI to the screen.
	*/
	void CrystalGui::Render(){
		this->Renderer->DrawStart(); 

		/*
		*	Render always-visible elements
		*	In this case its just the progress bar and
		*	Done/Error count.
		*/
		
		this->Renderer->SetDrawColor(0, 0, 0, 255);
		this->Renderer->SetDrawSize(1.0F);
		
		this->Renderer->DrawText("Done:", 420,379);
		this->Renderer->DrawText(to_string(this->TotalDecrypted), 499,379);
		
		this->Renderer->SetDrawColor(255, 0, 0, 255);
		this->Renderer->DrawText("Errors:", 615, 379);
		this->Renderer->DrawText(to_string(this->TotalErrored), 694, 379);
		
		this->Renderer->SetDrawColor(0, 0, 0, 255);
		this->Renderer->DrawProgressBar(this->TotalPsmGames, this->TotalProcessed, 419, 317, 400, 30);

		switch(this->GuiState){
			case CrystalStateDecrypting:
				/*
				*	Draw PSM Archiver text.
				*/
				this->Renderer->SetDrawColor(255, 0, 255, 255);
				
				this->Renderer->SetDrawSize(1.3F);
				this->Renderer->DrawText("**** PSM Archiver ****", 381, 138); 
				this->Renderer->SetDrawSize(1.0F);

				this->Renderer->SetDrawColor(0, 0, 0, 255);
				
				// Free existing icon.
				if(this->CurrentIcon != this->CurrentIcon){
					this->Renderer->UnloadImage(this->CurrentIcon);
					this->CurrentIcon = NULL;
				}
				
				// Load new icon
				this->CurrentIcon = this->Renderer->LoadDDS("ur0:/appmeta/"+this->CurrentTitleid+"/icon0.dds");
				
				// Default icon.
				if(this->CurrentIcon == NULL){
					this->CurrentIcon = this->DefaultIcon;
				}

				/*
				*	Draw Stats
				*/
				this->Renderer->SetDrawColor(76, 128, 119, 255);
				this->Renderer->DrawText("Total:", 719, 140);
				this->Renderer->DrawText(to_string(this->TotalPsmGames), 790, 140);
				this->Renderer->SetDrawColor(0, 0, 0, 255);
				
				this->Renderer->DrawText("Backing Up Game #"+to_string(this->TotalProcessed), 560, 189);

				/*
				*	Draw icon0 and overaly.
				*/
				this->Renderer->DrawImage(this->CurrentIcon, 397,159);
				this->Renderer->DrawImage(this->IconOverlay, 386,149);
				
				this->Renderer->DrawText(this->CurrentTitleid, 396,278);
								
				break;
			case CrystalStateUploadDialog:
				this->Renderer->DrawMessageDialog("Contribute?", "Would you like to contribute to\nPSM Archive Project?");
				break;
			case CrystalStateDone:
				this->Renderer->DrawMessageDialog("Backup complete!", "Files stored at ux0:/data/psm\nWould you like to close this app?");
				break;
		}		
		

		this->Renderer->DrawEnd();
	}

}

