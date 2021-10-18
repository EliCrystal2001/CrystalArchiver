/*
*	 Written by Eli Crystal
*/

#ifndef _CRYSTAL_RENDERER_
#define _CRYSTAL_RENDERER_
#include <vita2d.h>
#include <vitasdk.h>


using namespace std;

namespace EliCrystal {
	typedef vita2d_texture* CrystalTexture;
	typedef uint32_t CrystalDrawColor;
	typedef float CrystalDrawSize;

	enum CrystalDrawFont{
		CrystalPgfFont,
		CrystalPvfFont
	};

	class CrystalRender{
		private:
			vita2d_pgf *Pgf;
			vita2d_pvf *Pvf;
			CrystalDrawColor DrawColor;
			CrystalDrawFont DrawFont;
			CrystalDrawSize DrawSize;
			
			CrystalTexture Background;
			CrystalTexture DialogWindow;
			CrystalTexture ProgressBarOverlay;
			
			void InitVita2d();
			void InitCrystalRender();
		public:
			CrystalRender();
			CrystalTexture LoadImage(string filename);
			CrystalTexture LoadDDS(string filename);
			void UnloadImage(CrystalTexture image);
			
			void SetDrawColor(int r, int g, int b, int a);
			void SetDrawFont(CrystalDrawFont font);
			void SetDrawSize(CrystalDrawSize sz);
			
			bool DrawStart();
			bool DrawText(string text, int x, int y);
			bool DrawRectangle(int x, int y, int w, int h);
			bool DrawImage(CrystalTexture image, int x, int y);
			bool DrawImageScaled(CrystalTexture image, int x, int y, int w, int h);
			bool DrawMessageDialog(string titletext, string bodytext);
			bool DrawProgressBar(int total, int done, int x, int y, int w, int h);
			bool DrawEnd();
	};
}


#endif
