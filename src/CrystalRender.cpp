/*
*	 Written by Eli Crystal
*/

#include <sstream>
#include <iostream>
#include <vector>
#include <cstdio>
#include "CrystalRender.hpp"
#include "CrystalIo.hpp"
#include "libdds/dds.h"

/*
*	Render Class
*	Handles all rendering...
*/
namespace EliCrystal {
	/*
	*	Method to initalize Vita2D
	*/
	void CrystalRender::InitVita2d(){
		sceClibPrintf("Setting up Vita2D..\n");
		vita2d_init();
		vita2d_set_clear_color(RGBA8(0x00, 0x00, 0x00, 0xFF));
		this->Pgf = vita2d_load_default_pgf();
		this->Pvf = vita2d_load_default_pvf();
	}
	
	/*
	*	Method to initalize CrystalRender
	*/
	void CrystalRender::InitCrystalRender(){
		sceClibPrintf("Setting up CrystalRender..\n");
		this->DrawColor = RGBA8(0xFF, 0xFF, 0xFF, 0xFF);
		this->DrawSize = 1.0F;
		this->DrawFont = CrystalPgfFont;
		
		// Load background images.
		sceClibPrintf("Loading background images...\n");
		this->Background = this->LoadImage("app0:/fw/bg.png");
		this->ProgressBarOverlay = this->LoadImage("app0:/fw/progbar_overlay.png");
		this->DialogWindow = this->LoadImage("app0:/fw/dialog_overlay.png");
		sceClibPrintf("Done!\n");
	}
	

	/*
	*	Class Constructor
	*	This gets called when the class is initalized.
	*/
	CrystalRender::CrystalRender(){
		this->InitVita2d();
		this->InitCrystalRender();
	}
	
	/*
	*	Method to load a texture
	*/
	CrystalTexture CrystalRender::LoadImage(string filename){
		return vita2d_load_PNG_file(filename.c_str());
	}
	
	/*
	*	Read DirectDraw Surface as CrystalTexture.
	*	Only supports RGBA8888 DDS.
	*/
	CrystalTexture CrystalRender::LoadDDS(string filename){
		CrystalFileData* FileData = CrystalIo::ReadFile(filename);
		
		if(FileData->FileData == NULL){
			sceClibPrintf("DDS Load Fail: File not found.\n");
			return NULL;
		}
		
		dds_image_t Dds = dds_load_from_memory(FileData->FileData, FileData->FileSize);
		
		if(Dds == NULL){
			sceClibPrintf("DDS Load Fail: dds == NULL\n");
			return NULL;
		}
		
		vita2d_texture* Texture = vita2d_create_empty_texture(Dds->header.width, Dds->header.height);
				
		char* TextureData = (char*)vita2d_texture_get_datap(Texture);
		uint32_t Stride = vita2d_texture_get_stride(Texture);
		
		for (int i = 0; i < Dds->header.height; i++) {
            uint32_t Y = Dds->header.height - 1 - i;
            uint32_t *TexPtr = (uint32_t *)(TextureData + Y*Stride);
            for (int X = 0; X < Dds->header.width; X++) {
                    uint32_t Color = *((uint32_t *)(Dds->pixels) + i * Dds->header.width + X);
                    *TexPtr = Color;
                    TexPtr++;
            }
        }
		dds_image_free(Dds);
		return Texture;
	}
	/*
	*	Method to unload a texture
	*/	
	void CrystalRender::UnloadImage(CrystalTexture image){
		if(image == NULL)
			return;
		vita2d_free_texture(image);
		image = NULL;
		return;
	}
	
	/*
	*	Set Draw Color
	*	Used for drawing text and other stuff
	*/
	void CrystalRender::SetDrawColor(int r, int g, int b, int a){
		this->DrawColor = RGBA8(r, g, b, a);
	}
	
	/*
	*	Set Draw Font
	* 	Used when drawing text
	*/
	void CrystalRender::SetDrawFont(CrystalDrawFont font){
		this->DrawFont = font;
	}
	
	/*
	*	Set Draw Size
	*	Used when drawing text
	*/
	void CrystalRender::SetDrawSize(CrystalDrawSize sz){
		this->DrawSize = sz;
	}
	
	
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//																																				   //
	//																																				   //
	//																																				   //
	//	Drawing methods																																   //
	//																																				   //
	//																																				   //
	//																																				   //
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	/*
	*	Clear screen &
	*	Begin Drawing
	*/
	bool CrystalRender::DrawStart(){
		vita2d_start_drawing();
		vita2d_clear_screen();
		this->DrawImage(this->Background, 0, 0);
		return true;
	}
	
	/*
	*	Draw text to the screen
	*/
	bool CrystalRender::DrawText(string text, int x, int y){
		if(this->DrawFont == CrystalPgfFont)
			vita2d_pgf_draw_text(this->Pgf, x, y, this->DrawColor, this->DrawSize, text.c_str());
		else if(this->DrawFont == CrystalPvfFont)
			vita2d_pvf_draw_text(this->Pvf, x, y, this->DrawColor, this->DrawSize, text.c_str());
		else
			return false;
		return true;
	}
	
	/*
	*	Draw image to the screen
	*/
	bool CrystalRender::DrawImage(CrystalTexture image, int x, int y){
		if(image == NULL)
			return false;
		
		vita2d_draw_texture(image, (float)x, (float)y);
		return true;
	}

	/*
	*	Draw an image scaled
	*/
	bool CrystalRender::DrawImageScaled(CrystalTexture image, int x, int y, int w, int h){
		if(image == NULL)
			return false;
		
		vita2d_draw_texture_scale(image, (float)x, (float)y, (float)w, (float)h);
		return true;
	}
	
	/*
	*	Draw a rectangle to the screen
	*/
	bool CrystalRender::DrawRectangle(int x, int y, int w, int h)
	{
		vita2d_draw_rectangle((float)x, (float)y, (float)w, (float)h, this->DrawColor);
		return true;
	}
		
	/*
	*	Draw a dialog box with title and text.
	*/
	bool CrystalRender::DrawMessageDialog(string titletext, string bodytext){
		CrystalDrawColor col = this->DrawColor;
		CrystalDrawSize sz = this->DrawSize;
		
		this->DrawImage(this->DialogWindow, 346, 88);
		this->SetDrawSize(1.0F);
		
		this->SetDrawColor(76, 128, 119, 255);
		this->DrawText(titletext, 496, 154);
		this->SetDrawColor(0, 0, 0, 255);
		this->DrawText(bodytext, 508, 181);
		
		this->DrawText("Cancel", 633, 252);
		this->SetDrawColor(214, 255, 228, 255);
		this->DrawText("Ok",783, 252);
		
		this->DrawColor = col;
		this->DrawSize = sz;
		return true;
	}
	
	/*
	*	Draw a progress bar of total/done 
	*/
	bool CrystalRender::DrawProgressBar(int total, int done, int x, int y, int w, int h){
		int TotalWidth = (int)((double)w*((double)done/(double)total));
		CrystalDrawColor col = this->DrawColor;
		
		// Draw Back 
		this->SetDrawColor(0, 0, 0, 128);
		this->DrawRectangle(x, y, w, h);
		
		// Draw Front
		this->SetDrawColor(181, 230, 210, 128);
		this->DrawRectangle(x, y, TotalWidth, h);
		
		// Draw Front IMG
		this->DrawImage(this->ProgressBarOverlay, x, y);
		
		this->DrawColor = col;
		
		return true;
	}
	
	/*
	*	End Drawing &
	*	Swap buffers
	*/
	bool CrystalRender::DrawEnd(){
		vita2d_end_drawing();
		vita2d_swap_buffers();
		return true;
	}
}

