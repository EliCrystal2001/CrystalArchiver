/*
*	 Written by Eli Crystal
*/

#include <vitasdk.h>
#include <sstream>
#include <vector>
#include <cstdio>
#include "CrystalGui.hpp"
#include "CrystalHttp.hpp"

using namespace std;
namespace EliCrystal {
	/*
	*	Default Class
	*	Gets called at the start of the program.
	*/
	class Program{
		private:
			CrystalGui* Gui;
		public:
			Program(){
				CrystalHttp::Init();
				this->Gui = new CrystalGui();
				/*
				*	Main program loop, Handles UI, Actions, Etc
				*/
				while(true){
					this->Gui->Render();
					this->Gui->Update();
				}
			}
	};
}


/*
*	Required entry point method.
*/
int main(int argc, char *argv[]) {	
	// Run program
	sceClibPrintf("CrystalArchiver.\n");
	sceClibPrintf("(c) 2021 Eli Crystal\n");
	sceClibPrintf("Starting program...\n");
	EliCrystal::Program* program = new EliCrystal::Program();
    return 0;
}
