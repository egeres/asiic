#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "canvas.h"
#include "button.h"
#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>
#include <iterator>

#ifdef _WIN32
#include <windows.h>
#include <Commdlg.h>

//funtion used to select a file with the system interface. It should be replaced with GTK or at least make this work in mac/linux
std::string select_file(bool filehastoexist)
{
	char filename[MAX_PATH];

	OPENFILENAME ofn;
	ZeroMemory(&filename, sizeof(filename));
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = NULL;  // If you have a window to center over, put its HANDLE here
	ofn.lpstrFilter = "Text Files\0*.txt\0Any File\0*.*\0";
	ofn.lpstrFile = filename;
	ofn.nMaxFile = MAX_PATH;
	ofn.lpstrTitle = "Select a text file to open";
	//ofn.Flags = OFN_DONTADDTORECENT | OFN_FILEMUSTEXIST;

	if (filehastoexist)
	{
		ofn.Flags = OFN_FILEMUSTEXIST;
	}

	if (GetOpenFileNameA(&ofn))
	{
		//std::cout << "Selected file... \"" << filename << "\"\n";
		return filename;
		//input_canvas.load_text_file(filename);
	}
	else
	{
		return "";
	}
	// else
	// {
	// 	// All this stuff below is to tell you exactly how you messed up above.
	// 	// Once you've got that fixed, you can often (not always!) reduce it to a 'user cancelled' assumption.
	// 	switch (CommDlgExtendedError())
	// 	{
	// 	case CDERR_DIALOGFAILURE: std::cout << "CDERR_DIALOGFAILURE\n";   break;
	// 	case CDERR_FINDRESFAILURE: std::cout << "CDERR_FINDRESFAILURE\n";  break;
	// 	case CDERR_INITIALIZATION: std::cout << "CDERR_INITIALIZATION\n";  break;
	// 	case CDERR_LOADRESFAILURE: std::cout << "CDERR_LOADRESFAILURE\n";  break;
	// 	case CDERR_LOADSTRFAILURE: std::cout << "CDERR_LOADSTRFAILURE\n";  break;
	// 	case CDERR_LOCKRESFAILURE: std::cout << "CDERR_LOCKRESFAILURE\n";  break;
	// 	case CDERR_MEMALLOCFAILURE: std::cout << "CDERR_MEMALLOCFAILURE\n"; break;
	// 	case CDERR_MEMLOCKFAILURE: std::cout << "CDERR_MEMLOCKFAILURE\n";  break;
	// 	case CDERR_NOHINSTANCE: std::cout << "CDERR_NOHINSTANCE\n";     break;
	// 	case CDERR_NOHOOK: std::cout << "CDERR_NOHOOK\n";          break;
	// 	case CDERR_NOTEMPLATE: std::cout << "CDERR_NOTEMPLATE\n";      break;
	// 	case CDERR_STRUCTSIZE: std::cout << "CDERR_STRUCTSIZE\n";      break;
	// 	case FNERR_BUFFERTOOSMALL: std::cout << "FNERR_BUFFERTOOSMALL\n";  break;
	// 	case FNERR_INVALIDFILENAME: std::cout << "FNERR_INVALIDFILENAME\n"; break;
	// 	case FNERR_SUBCLASSFAILURE: std::cout << "FNERR_SUBCLASSFAILURE\n"; break;
	// 	default: std::cout << "You cancelled.\n";
	// 	}
	// }

	// return "e - e";
}

#elif __unix__
#elif __linux__
#elif __APPLE__
#endif

/*  TO DO

	optimize the cpu usage... maybe is an issue with the joystick ?

	add debug mode
	find cool use for the enter key

	make center_canvas_in_window() great again, lol

	options that should be available to the user :
		change font
		default new canvas size
		mute sound
		night/day mode
		remap shorcuts ?

	functionality / tools :
		ruler ?
		options box
		square selection moving when out of boundaries
		select lines ?
		add ctrl+z
		blinking cursor displace text sideways both left and right ?
		move selection with arrow keys
		when saving, automatically add ".txt" at the end of the namefile if name is left blank

	menu / interface :
		display square selection size ?
		help window ?
		blinking cursor
		invert colors option ?
		right click pie menu ?
		options menu
		startup dialog
		individual camera offset per project tab, right now, if you pan in a project window, that pan offset
		applies to a different tab

	bugs / improvements:
		remove the canvas input_value from the function cell_location()
		size of upper toolbar can sometimes be incorrect ? or.. the buttons ? dunno
		there's a quintillion of warnings about possible data loss...
		fix square selection lagging
		fix weird displacement with the canvas drag and drop button
		fix "out of matrix" tier issues with the "blinking cursor" when moving left
		fix square selection bugging when leaving things selected
		better text input, keys lag by some reason

		maybe it's more efficient to use instead of the  current events system ? :

			std::cout << "control:" << event.key.control << std::endl;
			std::cout << "alt:" << event.key.alt << std::endl;
			std::cout << "shift:" << event.key.shift << std::endl;

		shorcuts might be executed every frame intead of just in press ?
		bugs when changing to a different view with ctrl+tab / ctrl+shift+tab

	planing for future shorcuts :

		ctrl + tab         | change to the tab on the left
		ctrl + tab + shift | change to the tab on the right
		ctrl + w           | close tab
		ctrl + t           | new tab

		ctrl + s           | save
		ctrl + s + shift   | save as
		ctrl + o           | open

		ctrl + a           | select all
		ctrl + i           | invert selection ?

		ctrl + 1           | pencil mode
		ctrl + 2           | rectangle mode
		ctrl + 3           | wand mode
		ctrl + 4           | equal character mode

		shift + leftmouse  | add selecion, don't erase

		ctrl + middlemouse | move selection
*/

/* IMPLENTED / FIXED
	antialiasing + icon
	semi-fixed ctrl+tab / ctrl+shift+tab shorcuts
	display the active tab
	separate main loop in smaller functions
	if no file is selected when opening the search bar, asiic crashes
	everything crashes if input file is empty
	create a remove button to close tabs
	create an add button for creating more tabs
	shorcuts :P
	drag and drop system
	'color picker' implemented
	fix canvas resizing causing issues with te interface
	everything crashes when two file types are the same i sepparated tabs... -_-"
	fix camera zoom
	fix "out of matrix" tier issues with the "blinking cursor" when moving right
	fixed issue with the main active canvas
	upper tabs select different canvases
	canvases are located in a dynamic vector now
	upper tabs for multiple canvas selecion
	spacer class
	GUI object class
	open file menu / load file
	show the "pixel" position
	text_over_toolbar
	tool bar
	button system
	icon for the project
	resize canvas dynamically
	wand selection
	fix issue with pencil selection crashing w/ just 1 click
	backspace functionality implemented
	type-in as normal text implemented
	correct mouse in view
	better movement
	save to .txt
	pencil toggle selection
	square selection
	text size corrected
*/


// icon for asiic
static const struct {
  unsigned int width;
  unsigned int height;
  unsigned int bytes_per_pixel; /* 2:RGB16, 3:RGB, 4:RGBA */
  unsigned char pixel_data[32 * 32 * 4 + 1];
} asiic_icon = {
  32, 32, 4,
  "\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"
  "\000\000\000\000\014\014\014\020\014\014\014[\014\014\014\232\014\014\014\317\014\014\014\352\014"
  "\014\014\373\014\014\014\373\014\014\014\351\014\014\014\316\014\014\014\231\014\014\014Z"
  "\014\014\014\017\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"
  "\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"
  "\000\000\000\000\000\000\014\014\014&\014\014\014\237\014\014\014\365\014\014\014\377\014\014\014\377"
  "\014\014\014\377\014\014\014\377\014\014\014\377\014\014\014\377\014\014\014\377\014\014\014"
  "\377\014\014\014\377\014\014\014\377\014\014\014\365\014\014\014\236\014\014\014%\000\000\000"
  "\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"
  "\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\014\014\014\011\014\014\014\215\014\014\014\372"
  "\014\014\014\377\014\014\014\377\014\014\014\377\014\014\014\377\014\014\014\377\014\014\014"
  "\377\014\014\014\377\014\014\014\377\014\014\014\377\014\014\014\377\014\014\014\377\014"
  "\014\014\377\014\014\014\377\014\014\014\377\014\014\014\372\014\014\014\213\014\014\014\011"
  "\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"
  "\000\000\000\000\000\000\000\000\014\014\014%\014\014\014\324\014\014\014\377\014\014\014\377\014\014\014"
  "\377\014\014\014\377\014\014\014\377\014\014\014\377\014\014\014\377\014\014\014\377\014"
  "\014\014\377\014\014\014\377\014\014\014\377\014\014\014\377\014\014\014\377\014\014\014\377"
  "\014\014\014\377\014\014\014\377\014\014\014\377\014\014\014\377\014\014\014\323\014\014\014"
  "$\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"
  "\014\014\014\061\014\014\014\353\014\014\014\377\014\014\014\377\014\014\014\377\014\014\014"
  "\377\014\014\014\377\014\014\014\377\014\014\014\377\014\014\014\377\014\014\014\377\014"
  "\014\014\377\014\014\014\377\014\014\014\377\014\014\014\377\014\014\014\377\014\014\014\377"
  "\014\014\014\377\014\014\014\377\014\014\014\377\014\014\014\377\014\014\014\377\014\014\014"
  "\351\014\014\014\060\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"
  "\014\014\014%\014\014\014\352\014\014\014\377\014\014\014\377\014\014\014\377\014\014\014\377"
  "\014\014\014\377\014\014\014\377\014\014\014\377\014\014\014\377\014\014\014\377\014\014\014"
  "\377\014\014\014\377\014\014\014\377\014\014\014\377\014\014\014\377\014\014\014\377\014"
  "\014\014\377\014\014\014\377\014\014\014\377\014\014\014\377\014\014\014\377\014\014\014\377"
  "\014\014\014\377\014\014\014\351\014\014\014#\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"
  "\000\000\014\014\014\012\014\014\014\325\014\014\014\377\014\014\014\377\014\014\014\377\014\014"
  "\014\377\014\014\014\377\014\014\014\377\014\014\014\377\014\014\014\377\014\014\014\377"
  "\014\014\014\377\014\014\014\377\014\014\014\377\014\014\014\377\014\014\014\377\014\014\014"
  "\377\014\014\014\377\014\014\014\377\014\014\014\377\014\014\014\377\014\014\014\377\014"
  "\014\014\377\014\014\014\377\014\014\014\377\014\014\014\377\014\014\014\322\014\014\014\010"
  "\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\014\014\014\215\014\014\014\377\014\014\014\377\014"
  "\014\014\377\014\014\014\377\014\014\014\377\014\014\014\377\014\014\014\377\014\014\014\377"
  "\014\014\014\377\014\014\014\377\014\014\014\377\014\014\014\377\014\014\014\377\014\014\014"
  "\377\014\014\014\377\014\014\014\377\014\014\014\377\014\014\014\377\014\014\014\377\014"
  "\014\014\377\014\014\014\377\014\014\014\377\014\014\014\377\014\014\014\377\014\014\014\377"
  "\014\014\014\377\014\014\014\212\000\000\000\000\000\000\000\000\000\000\000\000\014\014\014&\014\014\014\372"
  "\014\014\014\377\014\014\014\377\014\014\014\377\014\014\014\377\014\014\014\377\014\014\014"
  "\377\014\014\014\377\014\014\014\377\014\014\014\377\014\014\014\377\014\014\014\377\014"
  "\014\014\377\014\014\014\377\014\014\014\377\014\014\014\377\014\014\014\377\014\014\014\377"
  "\014\014\014\377\014\014\014\377\014\014\014\377\014\014\014\377\014\014\014\377\014\014\014"
  "\377\014\014\014\377\014\014\014\377\014\014\014\377\014\014\014\371\014\014\014#\000\000\000"
  "\000\000\000\000\000\014\014\014\237\014\014\014\377\014\014\014\377\014\014\014\377\014\014\014\377"
  "\014\014\014\377\014\014\014\377,,,\377\030\030\030\377\014\014\014\377\014\014\014\377"
  "\014\014\014\377\014\014\014\377\014\014\014\377\014\014\014\377\014\014\014\377\014\014\014"
  "\377\014\014\014\377\014\014\014\377\014\014\014\377\014\014\014\377\037\037\037\377$$$"
  "\377\014\014\014\377\014\014\014\377\014\014\014\377\014\014\014\377\014\014\014\377\014"
  "\014\014\377\014\014\014\233\000\000\000\000\014\014\014\020\014\014\014\365UUU\377\251\251\251"
  "\377\014\014\014\377\014\014\014\377\014\014\014\377\014\014\014\377\217\217\217\377"
  "\071\071\071\377\014\014\014\377\014\014\014\377\014\014\014\377\014\014\014\377\014\014\014"
  "\377\014\014\014\377\014\014\014\377\014\014\014\377\014\014\014\377\014\014\014\377\014"
  "\014\014\377\014\014\014\377VVV\377lll\377\014\014\014\377\014\014\014\377\014\014\014"
  "\377\014\014\014\377\252\252\252\377RRR\377\014\014\014\364\014\014\014\016\014\014\014"
  "[\014\014\014\377bbb\377\303\303\303\377\014\014\014\377\014\014\014\377\060\060\060"
  "\377\247\247\247\377\351\351\351\377\334\334\334\377\255\255\255\377\060\060"
  "\060\377\014\014\014\377\014\014\014\377\014\014\014\377\014\014\014\377\014\014\014\377"
  "\014\014\014\377\014\014\014\377\014\014\014\377\034\034\034\377\226\226\226\377\334"
  "\334\334\377\346\346\346\377\270\270\270\377RRR\377\014\014\014\377\014\014\014"
  "\377\306\306\306\377^^^\377\014\014\014\377\014\014\014W\014\014\014\232\014\014\014"
  "\377bbb\377\303\303\303\377\014\014\014\377\014\014\014\377\333\333\333\377\223"
  "\223\223\377\235\235\235\377TTT\377jjj\377\064\064\064\377\014\014\014\377\014\014"
  "\014\377\014\014\014\377\014\014\014\377\014\014\014\377\014\014\014\377\014\014\014\377"
  "\014\014\014\377\262\262\262\377\273\273\273\377ooo\377~~~\377XXX\377TTT\377"
  "\014\014\014\377\014\014\014\377\306\306\306\377^^^\377\014\014\014\377\014\014\014\227"
  "\014\014\014\317\014\014\014\377<<<\377ttt\377\014\014\014\377\022\022\022\377\377\377"
  "\377\377\060\060\060\377\217\217\217\377\071\071\071\377\014\014\014\377\014\014\014"
  "\377\014\014\014\377\014\014\014\377\014\014\014\377\014\014\014\377\014\014\014\377\014"
  "\014\014\377\014\014\014\377\014\014\014\377\336\336\336\377ddd\377VVV\377lll\377"
  "\014\014\014\377\014\014\014\377\014\014\014\377\014\014\014\377uuu\377:::\377\014\014"
  "\014\377\014\014\014\311\014\014\014\352\014\014\014\377\014\014\014\377\014\014\014\377"
  "\014\014\014\377\014\014\014\377\303\303\303\377\326\326\326\377\311\311\311\377"
  "hhh\377\024\024\024\377\014\014\014\377\014\014\014\377\014\014\014\377\014\014\014\377"
  "\014\014\014\377\014\014\014\377\014\014\014\377\014\014\014\377\014\014\014\377\227\227"
  "\227\377\347\347\347\377\263\263\263\377\223\223\223\377\037\037\037\377\014"
  "\014\014\377\014\014\014\377\014\014\014\377\014\014\014\377\014\014\014\377\014\014\014\377"
  "\014\014\014\346\014\014\014\373\014\014\014\377\014\014\014\377\014\014\014\377\014\014\014"
  "\377\014\014\014\377\025\025\025\377ppp\377\317\317\317\377\346\346\346\377\356"
  "\356\356\377SSS\377\014\014\014\377\014\014\014\377\014\014\014\377\014\014\014\377\014"
  "\014\014\377\014\014\014\377\014\014\014\377\014\014\014\377\014\014\014\377[[[\377\267"
  "\267\267\377\347\347\347\377\364\364\364\377zzz\377\014\014\014\377\014\014\014"
  "\377\014\014\014\377\014\014\014\377\014\014\014\377\014\014\014\372\014\014\014\373\014"
  "\014\014\377\014\014\014\377\014\014\014\377\014\014\014\377\014\014\014\377\014\014\014\377"
  "\014\014\014\377\217\217\217\377\071\071\071\377\224\224\224\377\317\317\317\377"
  "\014\014\014\377\014\014\014\377\014\014\014\377\014\014\014\377\014\014\014\377\014\014\014"
  "\377\014\014\014\377\014\014\014\377\014\014\014\377\014\014\014\377VVV\377lll\377bb"
  "b\377\366\366\366\377\017\017\017\377\014\014\014\377\014\014\014\377\014\014\014\377"
  "\014\014\014\377\014\014\014\372\014\014\014\351\014\014\014\377\014\014\014\377\014\014\014"
  "\377\014\014\014\377\020\020\020\377///\377\014\014\014\377\217\217\217\377\071\071"
  "\071\377\201\201\201\377\314\314\314\377\014\014\014\377\014\014\014\377\014\014\014"
  "\377\014\014\014\377\014\014\014\377\014\014\014\377\014\014\014\377\014\014\014\377\063"
  "\063\063\377\014\014\014\377VVV\377lll\377OOO\377\363\363\363\377\016\016\016\377"
  "\014\014\014\377\014\014\014\377\014\014\014\377\014\014\014\377\014\014\014\345\014\014\014"
  "\316\014\014\014\377\014\014\014\377\014\014\014\377\014\014\014\377\024\024\024\377\344"
  "\344\344\377\310\310\310\377\324\324\324\377\314\314\314\377\351\351\351"
  "\377NNN\377\014\014\014\377\014\014\014\377\014\014\014\377\014\014\014\377\014\014\014"
  "\377\014\014\014\377\014\014\014\377\014\014\014\377\306\306\306\377\323\323\323\377"
  "\301\301\301\377\323\323\323\377\353\353\353\377uuu\377\014\014\014\377\014\014"
  "\014\377\014\014\014\377\014\014\014\377\014\014\014\377\014\014\014\310\014\014\014\231"
  "\014\014\014\377\014\014\014\377\014\014\014\377\014\014\014\377\014\014\014\377\016\016\016"
  "\377:::\377\254\254\254\377aaa\377\025\025\025\377\014\014\014\377\014\014\014\377"
  "\014\014\014\377\014\014\014\377\014\014\014\377\014\014\014\377\014\014\014\377\014\014\014"
  "\377\014\014\014\377\014\014\014\377\060\060\060\377\200\200\200\377\214\214\214\377"
  "\037\037\037\377\014\014\014\377\014\014\014\377\014\014\014\377\014\014\014\377\014\014\014"
  "\377\014\014\014\377\014\014\014\226\014\014\014Z\014\014\014\377\014\014\014\377\014\014"
  "\014\377\014\014\014\377\014\014\014\377\014\014\014\377\014\014\014\377\220\220\220\377"
  "\071\071\071\377\014\014\014\377\014\014\014\377\014\014\014\377\014\014\014\377\014\014\014"
  "\377\014\014\014\377\014\014\014\377\014\014\014\377\014\014\014\377\014\014\014\377\014"
  "\014\014\377\014\014\014\377VVV\377lll\377\014\014\014\377\014\014\014\377\014\014\014"
  "\377\014\014\014\377\014\014\014\377\014\014\014\377\014\014\014\377\014\014\014V\014\014"
  "\014\017\014\014\014\365\014\014\014\377\014\014\014\377\014\014\014\377\014\014\014\377\014"
  "\014\014\377\014\014\014\377\036\036\036\377\023\023\023\377\014\014\014\377\014\014\014\377"
  ":::\377\262\262\262\377\262\262\262\377\262\262\262\377\262\262\262\377\262"
  "\262\262\377\262\262\262\377\066\066\066\377\014\014\014\377\014\014\014\377\026\026"
  "\026\377\031\031\031\377\014\014\014\377\014\014\014\377\014\014\014\377\014\014\014\377"
  "\014\014\014\377\014\014\014\377\014\014\014\363\014\014\014\015\000\000\000\000\014\014\014\236"
  "\014\014\014\377\014\014\014\377\014\014\014\377\014\014\014\377\014\014\014\377\014\014\014"
  "\377\014\014\014\377\014\014\014\377\014\014\014\377\014\014\014\377\035\035\035\377EEE"
  "\377EEE\377EEE\377EEE\377EEE\377EEE\377\034\034\034\377\014\014\014\377\014\014\014"
  "\377\014\014\014\377\014\014\014\377\014\014\014\377\014\014\014\377\014\014\014\377\014"
  "\014\014\377\014\014\014\377\014\014\014\377\014\014\014\232\000\000\000\000\000\000\000\000\014\014\014"
  "%\014\014\014\371\014\014\014\377\014\014\014\377\014\014\014\377\014\014\014\377\014\014"
  "\014\377\014\014\014\377\014\014\014\377\014\014\014\377\014\014\014\377\014\014\014\377"
  "\014\014\014\377\014\014\014\377\014\014\014\377\014\014\014\377\014\014\014\377\014\014\014"
  "\377\014\014\014\377\014\014\014\377\014\014\014\377\014\014\014\377\014\014\014\377\014"
  "\014\014\377\014\014\014\377\014\014\014\377\014\014\014\377\014\014\014\377\014\014\014\371"
  "\014\014\014\"\000\000\000\000\000\000\000\000\000\000\000\000\014\014\014\213\014\014\014\377\014\014\014\377"
  "\014\014\014\377\014\014\014\377\014\014\014\377\014\014\014\377\014\014\014\377\014\014\014"
  "\377\014\014\014\377\014\014\014\377\014\014\014\377\014\014\014\377\014\014\014\377\014"
  "\014\014\377\014\014\014\377\014\014\014\377\014\014\014\377\014\014\014\377\014\014\014\377"
  "\014\014\014\377\014\014\014\377\014\014\014\377\014\014\014\377\014\014\014\377\014\014\014"
  "\377\014\014\014\377\014\014\014\210\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\014\014\014\011"
  "\014\014\014\323\014\014\014\377\014\014\014\377\014\014\014\377\014\014\014\377\014\014\014"
  "\377\014\014\014\377\014\014\014\377\014\014\014\377\014\014\014\377\014\014\014\377\014"
  "\014\014\377\014\014\014\377\014\014\014\377\014\014\014\377\014\014\014\377\014\014\014\377"
  "\014\014\014\377\014\014\014\377\014\014\014\377\014\014\014\377\014\014\014\377\014\014\014"
  "\377\014\014\014\377\014\014\014\377\014\014\014\320\014\014\014\010\000\000\000\000\000\000\000\000\000"
  "\000\000\000\000\000\000\000\000\000\000\000\014\014\014$\014\014\014\351\014\014\014\377\014\014\014\377\014"
  "\014\014\377\014\014\014\377\014\014\014\377\014\014\014\377\014\014\014\377\014\014\014\377"
  "\014\014\014\377\014\014\014\377\014\014\014\377\014\014\014\377\014\014\014\377\014\014\014"
  "\377\014\014\014\377\014\014\014\377\014\014\014\377\014\014\014\377\014\014\014\377\014"
  "\014\014\377\014\014\014\377\014\014\014\377\014\014\014\350\014\014\014!\000\000\000\000\000\000\000"
  "\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\014\014\014\060\014\014\014\351\014\014"
  "\014\377\014\014\014\377\014\014\014\377\014\014\014\377\014\014\014\377\014\014\014\377"
  "\014\014\014\377\014\014\014\377\014\014\014\377\014\014\014\377\014\014\014\377\014\014\014"
  "\377\014\014\014\377\014\014\014\377\014\014\014\377\014\014\014\377\014\014\014\377\014"
  "\014\014\377\014\014\014\377\014\014\014\377\014\014\014\350\014\014\014.\000\000\000\000\000\000\000"
  "\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\014\014\014#\014\014"
  "\014\322\014\014\014\377\014\014\014\377\014\014\014\377\014\014\014\377\014\014\014\377"
  "\014\014\014\377\014\014\014\377\014\014\014\377\014\014\014\377\014\014\014\377\014\014\014"
  "\377\014\014\014\377\014\014\014\377\014\014\014\377\014\014\014\377\014\014\014\377\014"
  "\014\014\377\014\014\014\377\014\014\014\321\014\014\014\"\000\000\000\000\000\000\000\000\000\000\000\000\000"
  "\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\014\014\014\010"
  "\014\014\014\212\014\014\014\371\014\014\014\377\014\014\014\377\014\014\014\377\014\014\014"
  "\377\014\014\014\377\014\014\014\377\014\014\014\377\014\014\014\377\014\014\014\377\014"
  "\014\014\377\014\014\014\377\014\014\014\377\014\014\014\377\014\014\014\377\014\014\014\371"
  "\014\014\014\210\014\014\014\010\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"
  "\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\014\014\014"
  "#\014\014\014\234\014\014\014\364\014\014\014\377\014\014\014\377\014\014\014\377\014\014"
  "\014\377\014\014\014\377\014\014\014\377\014\014\014\377\014\014\014\377\014\014\014\377"
  "\014\014\014\377\014\014\014\364\014\014\014\233\014\014\014#\000\000\000\000\000\000\000\000\000\000\000\000"
  "\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"
  "\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\014\014\014\016\014\014\014X\014"
  "\014\014\227\014\014\014\312\014\014\014\347\014\014\014\373\014\014\014\373\014\014\014\347"
  "\014\014\014\312\014\014\014\227\014\014\014W\014\014\014\016\000\000\000\000\000\000\000\000\000\000\000\000"
  "\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000",
};

// used in order to convert ints to strings.
namespace patch
{
    template < typename T > std::string to_string( const T& n )
    {
        std::ostringstream stm ;
        stm << n ;
        return stm.str() ;
    }
}

// function which draws the base grid
void draw_grid(sf::RenderWindow& input_window, canvas input_canvas, int disp_x, int disp_y, int spacing_x, int spacing_y)
{
	sf::RectangleShape rectangle(sf::Vector2f(spacing_x, spacing_y));
	rectangle.setFillColor(sf::Color(5, 5, 5));
	rectangle.setOutlineThickness(1);
	rectangle.setOutlineColor(sf::Color(13, 13, 13));
	for (int i = 0; i < input_canvas.size_x; i++)
	{
		for (int j = 0; j < input_canvas.size_y; j++)
		{
			rectangle.setPosition(disp_x + i * spacing_x, disp_y + j * spacing_y);
			input_window.draw(rectangle);
		}
	}
}

// returns a vector which contains the location in which the
sf::Vector2i cell_location(sf::Vector2i inpt, canvas input_canvas, int spacing_x, int spacing_y)
{

	sf::Vector2i to_return;

	int offset_x;
	offset_x     = inpt.x % spacing_x;
	offset_x     = inpt.x - offset_x;
	offset_x     = offset_x / spacing_x;

	int offset_y;
	offset_y     = inpt.y % spacing_y;
	offset_y     = inpt.y - offset_y;
	offset_y     = offset_y / spacing_y;

	to_return.x = offset_x;
	to_return.y = offset_y;

	return to_return;
}

// function which draws the selected cells in the grid
void draw_selected(sf::RenderWindow& input_window, canvas input_canvas, int disp_x, int disp_y, int spacing_x, int spacing_y)
{
	//just to crearify, tmp_activ_cells is a temporal matrix created by the square selection
	input_canvas.overlay_short_matrix(input_canvas.tmp_activ_cells);
	sf::RectangleShape rectangle(sf::Vector2f(spacing_x, spacing_y));
	rectangle.setFillColor(sf::Color(20, 20, 20));

	for (int i = 0; i < input_canvas.size_y; i++)
	{
		for (int j = 0; j < input_canvas.size_x; j++)
		{
			if (input_canvas.activ_cells[i][j])
			{
				rectangle.setPosition(disp_x+j*spacing_x, disp_y+i*spacing_y);
				input_window.draw(rectangle);
			}

		}
	}
}


// function which draws... things ? I need to sleep more, lol
void draw_drag_and_drop(sf::RenderWindow& input_window, canvas input_canvas, int disp_x, int disp_y, int spacing_x, int spacing_y, sf::Vector2i displacement_from_mouses, canvas working_canvas)
{
	//just to crearify, tmp_activ_cells is a temporal matrix created by the square selection
	//input_canvas.overlay_short_matrix(input_canvas.tmp_activ_cells);
	sf::RectangleShape rectangle(sf::Vector2f(spacing_x, spacing_y));
	rectangle.setFillColor(sf::Color(29, 29, 29));

	sf::Vector2i temporal_disp = cell_location(displacement_from_mouses , working_canvas, spacing_x, spacing_y);
	//std::cout << temporal_disp.x << " _ " << temporal_disp.y << "    "
	//<< displacement_from_mouses.x << " _ " << displacement_from_mouses.y << "\n";

	for (int i = 0; i < input_canvas.size_y; i++)
	{
		for (int j = 0; j < input_canvas.size_x; j++)
		{
			if (input_canvas.activ_cells[i][j])
			{
				rectangle.setPosition((temporal_disp.x*spacing_x) +  disp_x  +  j*spacing_x, (temporal_disp.y*spacing_y) + disp_y + i*spacing_y);
				input_window.draw(rectangle);
			}
		}
	}
}

// function to draw the characters from the matrix
void draw_characters(sf::RenderWindow& input_window, canvas input_canvas, int disp_x, int disp_y, int spacing_x, int spacing_y, sf::Text& text_obj)
{
	sf::RectangleShape rectangle(sf::Vector2f(spacing_x, spacing_y));
	rectangle.setFillColor(sf::Color(20, 20, 20));

	for (int i = 0; i < input_canvas.size_y; i++)
	{
		for (int j = 0; j < input_canvas.size_x; j++)
		{
			text_obj.setPosition(disp_x + j * spacing_x - 10, disp_y + i * spacing_y);
			text_obj.setString(input_canvas.cell_letters[i][j]);
			input_window.draw(text_obj);
		}
	}
}

// draw the new canvas when resizing
void draw_working_canvas_size(sf::RenderWindow& input_window, canvas input_canvas, sf::Vector2i increment_decrement_vector, int disp_x, int disp_y, int spacing_x, int spacing_y)
{
	sf::RectangleShape rectangle(sf::Vector2f(spacing_x, spacing_y));
	rectangle.setFillColor(sf::Color(25, 25, 25, 50));
	rectangle.setOutlineThickness(1);
	rectangle.setOutlineColor(sf::Color(30, 30, 30));

	for (int i = 0; i < input_canvas.size_x + increment_decrement_vector.x; i++)
	{
		for (int j = 0; j < input_canvas.size_y + increment_decrement_vector.y; j++)
		{
			rectangle.setPosition(disp_x + i * spacing_x, disp_y + j * spacing_y);
			input_window.draw(rectangle);
		}
	}
}

// returns true if coordinate is inside of the rect described
bool inside_rect(sf::Vector2i inpt_vector, int pos_x, int pos_y, int width, int height)
{
	if (inpt_vector.x > pos_x && inpt_vector.x < pos_x + width)
	{
		if (inpt_vector.y > pos_y && inpt_vector.y < pos_y + height)
		{
			return true;
		}
	}
	return false;
}

// draws a text string (with a sexy coloured background) above the toolbox from below
void draw_text_over_toobox_up(sf::RenderWindow& input_window, std::string input_string,  sf::Text& text_obj, sf::Font fnt)
{

	text_obj.setString(input_string);
	sf::FloatRect bounds = text_obj.getLocalBounds();

	text_obj.setPosition(
		((int)input_window.getSize().x / 2) - bounds.width/2,
		((int)input_window.getSize().y - (int)input_window.getSize().y * 0.15) - 65 + 3
	);

	sf::RectangleShape rectangle(sf::Vector2f(bounds.width + 16, fnt.getLineSpacing(40)));
	rectangle.setFillColor(sf::Color(14, 14, 14));
	rectangle.setPosition(
		((int)input_window.getSize().x / 2) - bounds.width/2 - 8,
		((int)input_window.getSize().y - (int)input_window.getSize().y * 0.15) - 50 + 3
	);

	input_window.draw(rectangle);
	input_window.draw(text_obj);
}

// draws a text string (with a sexy coloured background) below the toolbox from below
void draw_text_over_toobox_bottom(sf::RenderWindow& input_window, std::string input_string,  sf::Text& text_obj, sf::Font fnt)
{

	text_obj.setString(input_string);
	sf::FloatRect bounds = text_obj.getLocalBounds();

	text_obj.setPosition(
		((int)input_window.getSize().x / 2) - bounds.width/2,
		((int)input_window.getSize().y - (int)input_window.getSize().y * 0.15) - 65 + 126
	);

	sf::RectangleShape rectangle(sf::Vector2f(bounds.width + 16, fnt.getLineSpacing(40)));
	rectangle.setFillColor(sf::Color(14, 14, 14));
	rectangle.setPosition(
		((int)input_window.getSize().x / 2) - bounds.width/2 - 8,
		((int)input_window.getSize().y - (int)input_window.getSize().y * 0.15) - 50 + 126
	);

	input_window.draw(rectangle);
	input_window.draw(text_obj);
}

using namespace std;

// View variables
int initial_size_x = 1500;
int initial_size_y = 900;
int displacement_x = 350;
int displacement_y = 50;
int cell_size_x    = 17;
int cell_size_y    = 40;
float zoom         = 1.0;
sf::View view1(      sf::FloatRect(0, 0, initial_size_x, initial_size_y));
sf::View hud_view(   sf::FloatRect(0, 0, initial_size_x, initial_size_y));
sf::View canvas_view(sf::FloatRect(0, 0, initial_size_x, initial_size_y));

sf::RenderWindow window(sf::VideoMode(1500, 900), "ASIIC", sf::Style::Default, sf::ContextSettings(0,0,8,2,0) );
string string_cell_upper_toolbox = "";
bool   showing_options_menu      = false;

// Tools variables
int  selection_mode   = 0;
bool selection_value  = true;
sf::Vector2i square_selection_initial_point;
sf::Vector2i square_selection_end_point;
bool moving_selection_around      = false;
bool prev_moving_selection_around = false;
sf::Vector2i drag_and_drop_starting_point;
sf::Vector2i drag_and_drop_cell_displacement;

// The canvas itself
vector<canvas*> canvases;
int active_canvas_index = 0;
canvas working_canvas       = canvas(1, 1, "1");

// Variables for the loop
bool out_of_canvas = true;
bool left_mouse_button_is_down;
bool prev_left_mouse_button_is_down = false;
bool prev_any_key_pressed;
bool left_mouse_button_just_down;
bool left_mouse_button_just_up;
bool prev_any_key_is_pressed;
sf::Vector2f hud_mouse_position;
sf::Vector2f mouse_position;
sf::Vector2f prev_mouse_position;
sf::Vector2f initial_mouse_position;
sf::Vector2i cell_location_vector;
sf::Vector2i displacement_v;
sf::Vector2i selection_position;
sf::Vector2i increment_decrement_vector;

bool prev_pressing_Lctrl  = false;
bool prev_pressing_Rctrl  = false;
bool prev_pressing_Tab    = false;
bool prev_pressing_Lshift = false;
bool prev_pressing_Rshift = false;
bool prev_pressing_N      = false;
bool prev_pressing_W      = false;
bool prev_index           = false;

// Text item for the GUI system
sf::Font font_consolas;
sf::Text text_consolas;
sf::Font font_pixel;
sf::Text text_pixel;

// Textures
sf::Texture tex_icon_pencil_selection;
sf::Texture tex_icon_equal_character_selection;
sf::Texture tex_icon_resize;
sf::Texture tex_icon_save;
sf::Texture tex_icon_square_selection;
sf::Texture tex_icon_wand_selection;
sf::Texture tex_icon_folder;
sf::Texture tex_icon_options;
sf::Texture tex_icon_resize_b;
sf::Texture tex_icon_spacing;
sf::Texture tex_icon_new_tab;
sf::Texture tex_icon_close_tab;
sf::Texture tex_icon_selected;
sf::Texture tex_icon_options_menu;

// Images & sprites
sf::Sprite spr_icon_pencil_selection;
sf::Sprite spr_icon_equal_character_selection;
sf::Sprite spr_icon_resize;
sf::Sprite spr_icon_save;
sf::Sprite spr_icon_square_selection;
sf::Sprite spr_icon_wand_selection;
sf::Sprite spr_icon_folder;
sf::Sprite spr_icon_options;
sf::Sprite spr_icon_resize_b;
sf::Sprite spr_icon_spacing;
sf::Sprite spr_icon_new_tab;
sf::Sprite spr_icon_close_tab;
sf::Sprite spr_icon_selected;
sf::Sprite spr_icon_options_menu;

// Sound (yeah, there's sound in this software...)
sf::SoundBuffer buffer_minimal_click;
sf::Sound       sound_minimal_click;
// sound_minimal_click.setBuffer(buffer_minimal_click);
// sound_minimal_click.setVolume(15);

// Navigation bars
navigation_bar_txt upper_toolbar(sf::Vector2i((int)window.getSize().x / 2, (int)window.getSize().y * 0.02), sf::Color(13, 13, 13), 8, 8, "centered", "horizontal", sound_minimal_click);
navigation_bar_img main_toolbar(sf::Vector2i((int)window.getSize().x / 2, (int)window.getSize().y - (int)window.getSize().y * 0.15), sf::Color(13, 13, 13), 5, 5, "centered", "horizontal", sound_minimal_click);

// Viewport individual buttons below
int  canvas_button_pos_x         = 0;
int  canvas_button_pos_y         = 0;
bool moving_canvas_button        = false;
button_image canvas_button_image = button_image(sf::Vector2i(canvas_button_pos_x, canvas_button_pos_y), spr_icon_resize_b, "resize");

int button_new_tab_pos_x         = 0;
int button_new_tab_pos_y         = 18;
button_image button_new_tab      = button_image(sf::Vector2i(button_new_tab_pos_x, button_new_tab_pos_y), spr_icon_new_tab, "new_tab" );

int button_close_tab_pos_x       = 0;
int button_close_tab_pos_y       = 18;
button_image button_close_tab    = button_image(sf::Vector2i(button_close_tab_pos_x, button_close_tab_pos_y), spr_icon_close_tab, "close_tab" );

// Others
sf::Event event;
sf::Time elapsed;


void loop_logic_shorcuts() {
	if (true)
	{
		//ctrl + tab         | move through tabs to the right
		if (
			(sf::Keyboard::isKeyPressed(sf::Keyboard::LControl) || sf::Keyboard::isKeyPressed(sf::Keyboard::RControl)) &&
			(sf::Keyboard::isKeyPressed(sf::Keyboard::Tab)) &&
			(!prev_pressing_Lctrl || !prev_pressing_Rctrl) &&
			(!prev_pressing_Tab)
			)
		{
			if ((active_canvas_index + 1) < canvases.size())
			{
				//active_canvas_index+=1;

				canvases[active_canvas_index]->activ_cells.clear();
				canvases[active_canvas_index]->tmp_activ_cells.clear();
				canvases[active_canvas_index]->cell_letters.clear();

				canvases[active_canvas_index]->activ_cells = working_canvas.activ_cells;
				canvases[active_canvas_index]->tmp_activ_cells = working_canvas.tmp_activ_cells;
				canvases[active_canvas_index]->cell_letters = working_canvas.cell_letters;
				canvases[active_canvas_index]->size_x = working_canvas.size_x;
				canvases[active_canvas_index]->size_y = working_canvas.size_y;

				active_canvas_index = active_canvas_index + 1;
				working_canvas = *canvases[active_canvas_index];
			}
		}

		//ctrl + shift + tab | move through tabs to the left
		if (
			(sf::Keyboard::isKeyPressed(sf::Keyboard::LControl) || sf::Keyboard::isKeyPressed(sf::Keyboard::RControl)) &&
			(sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)   || sf::Keyboard::isKeyPressed(sf::Keyboard::RShift))   &&
			(sf::Keyboard::isKeyPressed(sf::Keyboard::Tab)) &&
			(!prev_pressing_Lctrl  || !prev_pressing_Rctrl ) &&
			(!prev_pressing_Lshift || !prev_pressing_Rshift) &&
			(!prev_pressing_Tab)
			)
		{
			if (active_canvas_index > 0)
			{

				//active_canvas_index-=1;

				canvases[active_canvas_index]->activ_cells.clear();
				canvases[active_canvas_index]->tmp_activ_cells.clear();
				canvases[active_canvas_index]->cell_letters.clear();

				canvases[active_canvas_index]->activ_cells = working_canvas.activ_cells;
				canvases[active_canvas_index]->tmp_activ_cells = working_canvas.tmp_activ_cells;
				canvases[active_canvas_index]->cell_letters = working_canvas.cell_letters;
				canvases[active_canvas_index]->size_x = working_canvas.size_x;
				canvases[active_canvas_index]->size_y = working_canvas.size_y;

				active_canvas_index = active_canvas_index - 1;
				working_canvas = *canvases[active_canvas_index];

			}
		}

		//ctrl + W | close current tab
		if (
			(sf::Keyboard::isKeyPressed(sf::Keyboard::LControl) || sf::Keyboard::isKeyPressed(sf::Keyboard::RControl)) &&
			(sf::Keyboard::isKeyPressed(sf::Keyboard::W)) &&
			(!prev_pressing_Lctrl || !prev_pressing_Rctrl) &&
			(!prev_pressing_W)
			)
		{
			if (canvases.size() > 1)
			{
				canvases.erase(canvases.begin() + active_canvas_index );
				upper_toolbar.list_of_buttons.erase( upper_toolbar.list_of_buttons.begin() + active_canvas_index );
				upper_toolbar.update();

				if ((active_canvas_index + 1) > canvases.size())
				{
					active_canvas_index -= 1;
				}

				working_canvas = *canvases[active_canvas_index];
			}
		}

		//ctrl + N | create new tab
		if (
			(sf::Keyboard::isKeyPressed(sf::Keyboard::LControl) || sf::Keyboard::isKeyPressed(sf::Keyboard::RControl)) &&
			(sf::Keyboard::isKeyPressed(sf::Keyboard::N)) &&
			(!prev_pressing_Lctrl || !prev_pressing_Rctrl) &&
			(!prev_pressing_N)
			)
		{
			canvases.push_back(new canvas(45, 17, "unknow"));
			upper_toolbar.list_of_buttons.push_back(
				new button_text(sf::Vector2i(1, 1), "file_text_0.txt", "pr6essed_button_1", font_pixel, sf::Color(25, 25, 25), 5)
			);
			upper_toolbar.update();

			active_canvas_index = canvases.size() - 1;
			working_canvas = *canvases[active_canvas_index];
		}

		//ctrl + O | open file
		if (
			(sf::Keyboard::isKeyPressed(sf::Keyboard::LControl) || sf::Keyboard::isKeyPressed(sf::Keyboard::RControl)) &&
			(sf::Keyboard::isKeyPressed(sf::Keyboard::O))
			)
		{
			std::string to_loaaaad = select_file(true);
			if (!to_loaaaad.empty())
			{
				working_canvas.load_text_file(to_loaaaad);
			}
			canvases[active_canvas_index]->file_route  = working_canvas.file_route;
			canvases[active_canvas_index]->canvas_name = working_canvas.canvas_name;
		}

		//ctrl + S | save file
		if (
			(sf::Keyboard::isKeyPressed(sf::Keyboard::LControl) || sf::Keyboard::isKeyPressed(sf::Keyboard::RControl)) &&
			(sf::Keyboard::isKeyPressed(sf::Keyboard::S))
			)
		{
			if (working_canvas.file_route.empty())
			{
				working_canvas.file_route = select_file(false);

				if (!working_canvas.file_route.empty() && working_canvas.file_route != "")
				{
					working_canvas.set_name_from_path(working_canvas.file_route);
					working_canvas.save_to(working_canvas.file_route);
					working_canvas.set_name_from_path(working_canvas.file_route);

					canvases[active_canvas_index]->file_route  = working_canvas.file_route;
					canvases[active_canvas_index]->canvas_name = working_canvas.canvas_name;
				}
			}
			else if (false)
			{
				working_canvas.save_to(working_canvas.file_route);
				working_canvas.set_name_from_path(working_canvas.file_route);
			}

		}

		//ctrl + 1 | enable pencil selection
		if (
			(sf::Keyboard::isKeyPressed(sf::Keyboard::LControl) || sf::Keyboard::isKeyPressed(sf::Keyboard::RControl)) &&
			(sf::Keyboard::isKeyPressed(sf::Keyboard::Num1))
			)
		{
			selection_mode = 0;
		}

		//ctrl + 2 | enable square selection
		if (
			(sf::Keyboard::isKeyPressed(sf::Keyboard::LControl) || sf::Keyboard::isKeyPressed(sf::Keyboard::RControl)) &&
			(sf::Keyboard::isKeyPressed(sf::Keyboard::Num2))
			)
		{
			selection_mode = 1;
		}

		//ctrl + 3 | enable wand selection
		if (
			(sf::Keyboard::isKeyPressed(sf::Keyboard::LControl) || sf::Keyboard::isKeyPressed(sf::Keyboard::RControl)) &&
			(sf::Keyboard::isKeyPressed(sf::Keyboard::Num3))
			)
		{
			selection_mode = 2;
		}

		//ctrl + 4 | enable selection by similarity
		if (
			(sf::Keyboard::isKeyPressed(sf::Keyboard::LControl) || sf::Keyboard::isKeyPressed(sf::Keyboard::RControl)) &&
			(sf::Keyboard::isKeyPressed(sf::Keyboard::Num4))
			)
		{
			selection_mode = 3;
		}
	}

}

void loop_logic_buttons_clicks() {
	if (left_mouse_button_is_down)
	{

		string index;
		int    index_esto = -1;

		if (!moving_canvas_button && !moving_selection_around)
		{
			index      = main_toolbar.check_click(    (sf::Vector2i)hud_mouse_position);
			index_esto = upper_toolbar.index_by_click((sf::Vector2i)hud_mouse_position);
		}
		else
		{

		}

		// this should activate in case we are just pressing a single button in the viewport
		if (index.empty())
		{
			if (button_new_tab.is_inside(   (sf::Vector2i)hud_mouse_position) ) { index =   "new_tab"; }
			if (button_close_tab.is_inside( (sf::Vector2i)hud_mouse_position) ) { index = "close_tab"; }
		}

		// if (index.empty()) {prev_index = false;} else {prev_index = true;}

		//means we've selected something from the toolbar
		if (!index.empty() && left_mouse_button_just_down)
		{
			if (index == "pencil_mode"    ) { selection_mode = 0; }
			if (index == "square_mode"    ) { selection_mode = 1; }
			if (index == "wand_mode"      ) { selection_mode = 2; }
			if (index == "similarity_mode") { selection_mode = 3; }

			if (index == "options") {
				showing_options_menu = !showing_options_menu;
			}

			if (index == "save") {
				if (working_canvas.file_route.empty())
				{
					working_canvas.file_route = select_file(false);
					// working_canvas.set_name_from_path(working_canvas.file_route);

					if (!working_canvas.file_route.empty() && working_canvas.file_route != "")
					{
						working_canvas.set_name_from_path(working_canvas.file_route);
						working_canvas.save_to(working_canvas.file_route);
						working_canvas.set_name_from_path(working_canvas.file_route);

						canvases[active_canvas_index]->file_route  = working_canvas.file_route;
						canvases[active_canvas_index]->canvas_name = working_canvas.canvas_name;
					}
				}
				else
				{
					working_canvas.save_to(working_canvas.file_route);
					working_canvas.set_name_from_path(working_canvas.file_route);
				}
				// canvases[active_canvas_index]->file_route  = working_canvas.file_route;
				// canvases[active_canvas_index]->canvas_name = working_canvas.canvas_name;
			}

			if (index == "open_file") {
				std::string to_loaaaad = select_file(true);

				if (!to_loaaaad.empty())
				{
					working_canvas.load_text_file(to_loaaaad);
				}

				canvases[active_canvas_index]->file_route  = working_canvas.file_route;
				canvases[active_canvas_index]->canvas_name = working_canvas.canvas_name;
			}

			if (index == "new_tab") {
				// button_new_tab_pos_y += 10;
				canvases.push_back(new canvas(45, 17, "unknow"));
				upper_toolbar.list_of_buttons.push_back( new button_text(sf::Vector2i(1, 1), "file_text_0.txt", "pr6essed_button_1", font_pixel, sf::Color(25, 25, 25), 5));
				upper_toolbar.update();



				// canvases[active_canvas_index]->activ_cells.clear();
				// canvases[active_canvas_index]->tmp_activ_cells.clear();
				// canvases[active_canvas_index]->cell_letters.clear();
				//
				// canvases[active_canvas_index]->activ_cells = working_canvas.activ_cells;
				// canvases[active_canvas_index]->tmp_activ_cells = working_canvas.tmp_activ_cells;
				// canvases[active_canvas_index]->cell_letters = working_canvas.cell_letters;
				// canvases[active_canvas_index]->size_x = working_canvas.size_x;
				// canvases[active_canvas_index]->size_y = working_canvas.size_y;

				// active_canvas_index = index_esto;
				// working_canvas = *canvases[active_canvas_index];

				active_canvas_index = canvases.size() - 1;
				working_canvas = *canvases[active_canvas_index];
			}

			if (index == "close_tab") {

				if (canvases.size() > 1)
				{

					canvases.erase(canvases.begin() + active_canvas_index );
					upper_toolbar.list_of_buttons.erase( upper_toolbar.list_of_buttons.begin() + active_canvas_index );
					upper_toolbar.update();

					if ((active_canvas_index + 1) > canvases.size())
					{
						active_canvas_index -= 1;
					}

					// canvases[active_canvas_index]->activ_cells.clear();
					// canvases[active_canvas_index]->tmp_activ_cells.clear();
					// canvases[active_canvas_index]->cell_letters.clear();
					//
					// canvases[active_canvas_index]->activ_cells = working_canvas.activ_cells;
					// canvases[active_canvas_index]->tmp_activ_cells = working_canvas.tmp_activ_cells;
					// canvases[active_canvas_index]->cell_letters = working_canvas.cell_letters;
					// canvases[active_canvas_index]->size_x = working_canvas.size_x;
					// canvases[active_canvas_index]->size_y = working_canvas.size_y;

					// active_canvas_index = index_esto;
					working_canvas = *canvases[active_canvas_index];
				}

			}
		}
		//means we've clicked one of the tabs
		else if (index_esto != -1 && left_mouse_button_just_down)
		{
			canvases[active_canvas_index]->activ_cells.clear();
			canvases[active_canvas_index]->tmp_activ_cells.clear();
			canvases[active_canvas_index]->cell_letters.clear();

			canvases[active_canvas_index]->activ_cells = working_canvas.activ_cells;
			canvases[active_canvas_index]->tmp_activ_cells = working_canvas.tmp_activ_cells;
			canvases[active_canvas_index]->cell_letters = working_canvas.cell_letters;
			canvases[active_canvas_index]->size_x = working_canvas.size_x;
			canvases[active_canvas_index]->size_y = working_canvas.size_y;
			canvases[active_canvas_index]->displacement = sf::Vector2i(displacement_x, displacement_y);
			canvases[active_canvas_index]->canvas_zoom  = zoom;

			active_canvas_index = index_esto;
			working_canvas      = *canvases[active_canvas_index];
			displacement_x      = working_canvas.displacement.x;
			displacement_y      = working_canvas.displacement.y;
			zoom                = working_canvas.canvas_zoom;
			canvas_view.setSize(sf::Vector2f(window.getSize().x * zoom, window.getSize().y * zoom));
		}
		//in case we just clicked the movement button a sound is played
		else if (canvas_button_image.is_inside((sf::Vector2i)mouse_position) && left_mouse_button_just_down)
		{
			initial_mouse_position = mouse_position;
			moving_canvas_button = true;
			sound_minimal_click.play();
		}
		//click happened inside of the canvas
		else if (!out_of_canvas)
		{
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl))
			{
				working_canvas.set_char_selected(working_canvas.cell_letters[cell_location_vector.y][cell_location_vector.x]);
			}
			else
			{
				//selection mode, if adding true values or false values...
				if (left_mouse_button_just_down)
				{
					//unnecesary violence
					selection_value = !working_canvas.activ_cells[cell_location_vector.y][cell_location_vector.x];
				}
				//pencil toggle selection
				if (selection_mode == 0 && !moving_canvas_button)
				{
				//stuff
				working_canvas.activ_cells[cell_location_vector.y][cell_location_vector.x] = selection_value;
				}
				//square toggle selection mode
				if (selection_mode == 1 && !moving_canvas_button)
				{
				if (left_mouse_button_just_down)
				{
					square_selection_initial_point = cell_location_vector;
					square_selection_end_point     = cell_location_vector;
				}
				square_selection_end_point = cell_location_vector;
				working_canvas.set_square_selection_temporal(square_selection_initial_point, square_selection_end_point, selection_value);
				}
				//wand selection mode
				if (selection_mode == 2 && left_mouse_button_just_down && !moving_canvas_button)
				{
				if (!(sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) || sf::Keyboard::isKeyPressed(sf::Keyboard::RShift)))
				{
					working_canvas.deselect_all();
				}
				working_canvas.select_bucket(cell_location_vector);
				}
				//wand selection mode
				if (selection_mode == 3 && left_mouse_button_just_down && !moving_canvas_button)
				{
				char selecion_character = working_canvas.cell_letters[cell_location_vector.y][cell_location_vector.x];
				if (!(sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) || sf::Keyboard::isKeyPressed(sf::Keyboard::RShift)))
				{
					working_canvas.deselect_all();
				}
				working_canvas.equal_character_selection(selecion_character);
				}
			}

		}
		if (!index.empty()) {}
	}
	if (left_mouse_button_just_up)
	{
		if (moving_canvas_button)
		{
			moving_canvas_button = false;
			working_canvas.resize(increment_decrement_vector.x, increment_decrement_vector.y);
			working_canvas.update_back_lineas(sf::Vector2i(cell_size_x,cell_size_y));
		}
		if (selection_mode == 0)
		{
		}
		if (selection_mode == 1)
		{
			working_canvas.overlay_short_matrix(working_canvas.tmp_activ_cells);
			working_canvas.clear_short_matrix();
		}
		if (selection_mode == 2)
		{
		}
	}
}

void loop_logic() {

}

void loop_awake() {

	window.setVerticalSyncEnabled(true);
	window.setIcon( asiic_icon.width,  asiic_icon.height,  asiic_icon.pixel_data );
	canvases.push_back(new canvas(40, 15, "unknow"));
	// canvases.push_back(new canvas(10, 15, "unknow"));
	working_canvas = *canvases[active_canvas_index];

	if (!font_consolas.loadFromFile("assets/consolas.ttf")) { /* error... */ }
	text_consolas.setFont(font_consolas);
	text_consolas.setCharacterSize(30);
	text_consolas.setFillColor(sf::Color::White);
	if (!font_pixel.loadFromFile("assets/pixel.ttf")) { /* error... */ }
	text_pixel.setFont(font_pixel);
	text_pixel.setCharacterSize(40);
	text_pixel.setFillColor(sf::Color::White);

	//we load textures
	if (!tex_icon_pencil_selection.loadFromFile("assets/icon_pencil_selection.png")) {}
	if (!tex_icon_equal_character_selection.loadFromFile("assets/icon_equal_character_selection.png")) {}
	if (!tex_icon_resize.loadFromFile("assets/icon_resize.png")) {}
	if (!tex_icon_save.loadFromFile("assets/icon_save.png")) {}
	if (!tex_icon_square_selection.loadFromFile("assets/icon_square_selection.png")) {}
	if (!tex_icon_wand_selection.loadFromFile("assets/icon_wand_selection.png")) {}
	if (!tex_icon_folder.loadFromFile("assets/icon_folder.png")) {}
	if (!tex_icon_options.loadFromFile("assets/icon_options.png")) {}
	if (!tex_icon_resize_b.loadFromFile("assets/icon_resize_b.png")) {}
	if (!tex_icon_spacing.loadFromFile("assets/icon_alpha_20_60.png")) {}
	tex_icon_spacing.setSmooth(true);
	if (!tex_icon_new_tab.loadFromFile("assets/icon_new_tab.png")) {}
	if (!tex_icon_close_tab.loadFromFile("assets/icon_close_tab.png")) {}
	if (!tex_icon_selected.loadFromFile("assets/icon_selected.png")) {}
	if (!tex_icon_options_menu.loadFromFile("assets/icon_options_menu.png")) {}

	//we set textures to the sprites
	spr_icon_pencil_selection.setTexture(          tex_icon_pencil_selection);
	spr_icon_equal_character_selection.setTexture( tex_icon_equal_character_selection);
	spr_icon_resize.setTexture(                    tex_icon_resize);
	spr_icon_save.setTexture(                      tex_icon_save);
	spr_icon_square_selection.setTexture(          tex_icon_square_selection);
	spr_icon_wand_selection.setTexture(            tex_icon_wand_selection);
	spr_icon_folder.setTexture(                    tex_icon_folder);
	spr_icon_options.setTexture(                   tex_icon_options);
	spr_icon_resize_b.setTexture(                  tex_icon_resize_b);
	spr_icon_spacing.setTexture(                   tex_icon_spacing);
	spr_icon_new_tab.setTexture(                   tex_icon_new_tab);
	spr_icon_close_tab.setTexture(                 tex_icon_close_tab);
	spr_icon_selected.setTexture(                  tex_icon_selected);
	spr_icon_options_menu.setTexture(              tex_icon_options_menu);
	spr_icon_options_menu.setPosition(0, 98);

	//sound (yeah, there's sound in this software...)
	if(!buffer_minimal_click.loadFromFile("assets/minimal_clickb.wav")) {}
	sound_minimal_click.setBuffer(buffer_minimal_click);
	sound_minimal_click.setVolume(15);

	//upper navigation bar
	upper_toolbar = navigation_bar_txt(sf::Vector2i((int)window.getSize().x / 2, (int)window.getSize().y * 0.02), sf::Color(13, 13, 13), 8, 8, "centered", "horizontal", sound_minimal_click);
	upper_toolbar.list_of_buttons.push_back( new button_text(sf::Vector2i(1, 1), "unknow", "pressed_6on_1", font_pixel, sf::Color(25, 25, 25), 5) );
	upper_toolbar.update();

	//navigation bar
	main_toolbar = navigation_bar_img(sf::Vector2i((int)window.getSize().x / 2, (int)window.getSize().y - (int)window.getSize().y * 0.15), sf::Color(13, 13, 13), 5, 5, "centered", "horizontal", sound_minimal_click);
	main_toolbar.list_of_buttons.push_back( new button_image(sf::Vector2i(1, 1),spr_icon_pencil_selection,          "pencil_mode")    );
	main_toolbar.list_of_buttons.push_back( new button_image(sf::Vector2i(1, 1),spr_icon_square_selection,          "square_mode")    );
	main_toolbar.list_of_buttons.push_back( new button_image(sf::Vector2i(1, 1),spr_icon_wand_selection,            "wand_mode")      );
	main_toolbar.list_of_buttons.push_back( new button_image(sf::Vector2i(1, 1),spr_icon_equal_character_selection, "similarity_mode"));
	main_toolbar.list_of_buttons.push_back( new button_image(sf::Vector2i(1, 1),spr_icon_spacing,""));
	main_toolbar.list_of_buttons.push_back( new button_image(sf::Vector2i(1, 1),spr_icon_save,                      "save")           );
	main_toolbar.list_of_buttons.push_back( new button_image(sf::Vector2i(1, 1),spr_icon_folder,                    "open_file")      );
	main_toolbar.list_of_buttons.push_back( new button_image(sf::Vector2i(1, 1),spr_icon_options,                   "options")        );
	main_toolbar.update();

	canvas_button_image = button_image(sf::Vector2i(canvas_button_pos_x, canvas_button_pos_y), spr_icon_resize_b, "resize");
	button_new_tab      = button_image(sf::Vector2i(button_new_tab_pos_x, button_new_tab_pos_y), spr_icon_new_tab, "new_tab" );
	button_close_tab    = button_image(sf::Vector2i(button_close_tab_pos_x, button_close_tab_pos_y), spr_icon_close_tab, "close_tab" );
	working_canvas.update_back_lineas(sf::Vector2i(cell_size_x,cell_size_y));
}

void loop_render() {

	window.clear();
	window.setView(canvas_view);
	window.draw(text_consolas); //eeh ?

	//std::cout << "7.0\n";
	/*
	working_canvas.update_back_lineas(sf::Vector2i(cell_size_x,cell_size_y));
	working_canvas.background_lineas.setPosition(displacement_x, displacement_y);
	working_canvas.translation.translate(displacement_x, displacement_y);
	working_canvas.translation.transformPoint(displacement_x, displacement_y);
	window.draw(working_canvas.background_lineas, working_canvas.translation);
	working_canvas.background_lineas.setPosition(40,40);
	window.draw(working_canvas.background_lineas, working_canvas.translation);
	window.draw(working_canvas.background_lineas);
	*/

	draw_grid(      window, working_canvas, displacement_x, displacement_y, cell_size_x, cell_size_y);

	draw_selected(  window, working_canvas, displacement_x, displacement_y, cell_size_x, cell_size_y);

	if (moving_selection_around && prev_moving_selection_around)
	{
		sf::Vector2i to_insert;
		to_insert = ((sf::Vector2i)drag_and_drop_starting_point + (sf::Vector2i)mouse_position);
		to_insert = ((sf::Vector2i)mouse_position - (sf::Vector2i)drag_and_drop_starting_point);

		draw_drag_and_drop(window, working_canvas, displacement_x, displacement_y, cell_size_x, cell_size_y, to_insert, working_canvas);
	}

	draw_characters(window, working_canvas, displacement_x + 10, displacement_y, cell_size_x, cell_size_y, text_consolas);

	window.draw(canvas_button_image.spr);

	if (moving_canvas_button) draw_working_canvas_size(window, working_canvas, increment_decrement_vector, displacement_x, displacement_y, cell_size_x, cell_size_y);

	window.setView(hud_view);

	main_toolbar.render( window, (sf::Vector2i)hud_mouse_position);

	for (int i = 0; i< upper_toolbar.list_of_buttons.size();i++)
	{upper_toolbar.list_of_buttons[i]->str = canvases[i]->canvas_name;}
	upper_toolbar.update();
	upper_toolbar.render(window, (sf::Vector2i)hud_mouse_position);

	int tmp_x;
	tmp_x  = upper_toolbar.list_of_buttons[active_canvas_index]->pos.x;
	tmp_x += (upper_toolbar.list_of_buttons[active_canvas_index]->wh.x) / 2;
	tmp_x -= 8;
	spr_icon_selected.setPosition(tmp_x, 68);
	window.draw(spr_icon_selected);

	button_new_tab_pos_x  = upper_toolbar.pos.x + (upper_toolbar.wh.x / 2) + upper_toolbar.padding + 8;
	button_new_tab.pos = sf::Vector2i(button_new_tab_pos_x, button_new_tab_pos_y);
	button_new_tab.update();
	window.draw(button_new_tab.spr);

	button_close_tab_pos_x = upper_toolbar.pos.x - (upper_toolbar.wh.x / 2) - upper_toolbar.padding - 8 - button_close_tab.wh.x;
	button_close_tab.pos = sf::Vector2i(button_close_tab_pos_x, button_close_tab_pos_y);
	button_close_tab.update();
	window.draw(button_close_tab.spr);
	// selection_value = false;

	if (!out_of_canvas)
	{
		string_cell_upper_toolbox = "";
		string_cell_upper_toolbox += "<";
		string_cell_upper_toolbox += patch::to_string(cell_location_vector.x);
		string_cell_upper_toolbox += " ";
		string_cell_upper_toolbox += patch::to_string(cell_location_vector.y);
		string_cell_upper_toolbox += ">";
	}
	else
	{
		string_cell_upper_toolbox = "< >";
	}

	draw_text_over_toobox_up(window, string_cell_upper_toolbox, text_pixel, font_pixel);

	if (main_toolbar.check_click((sf::Vector2i)hud_mouse_position) != "" && main_toolbar.check_click((sf::Vector2i)hud_mouse_position) != "clicked the toolbar...")
	{
		std::string texti_pixel;
		texti_pixel = main_toolbar.check_click((sf::Vector2i)hud_mouse_position);
		draw_text_over_toobox_bottom(window, texti_pixel, text_pixel, font_pixel);
	}

	if (showing_options_menu)
	{
		tmp_x = window.getSize().x/2 - (spr_icon_options_menu.getGlobalBounds().width / 2);
		// tmp_x = window.getSize().x/2;
		spr_icon_options_menu.setPosition(tmp_x, 98);
		window.draw(spr_icon_options_menu);
	}

	window.display();
}

void loop_text_input_management() {
	//std::cout << "4\n";
	//text input management, pretty much it only works when something in the canvas is selected...
	if ((event.type == sf::Event::TextEntered) && !prev_any_key_pressed)
	{
		if (event.text.unicode < 256)
		{
			//in case we have more than just one cell selected
			if (working_canvas.return_ammount_selected() > 1)
			{
				char value_to_add = ' ';
				if(event.text.unicode != '\b') { value_to_add = event.text.unicode; }
				working_canvas.set_char_selected(value_to_add);
				working_canvas.deselect_all();
			}

			//in case we have just one cell selected
			else if (working_canvas.return_ammount_selected() == 1)
			{
				char value_to_add = ' ';

				if(event.text.unicode == '\b')
				{
					working_canvas.set_char_selected(value_to_add);
					selection_position = working_canvas.first_position_selection();
					working_canvas.deselect_all();
					working_canvas.activ_cells[selection_position.y][selection_position.x - 1] = true;
				}
				else
				{
					selection_position = working_canvas.first_position_selection();

					if (selection_position.y+1 < working_canvas.size_y)
					{
						if (selection_position.x+1 < working_canvas.size_x)
						{
							value_to_add = event.text.unicode;
							working_canvas.set_char_selected(value_to_add);
							working_canvas.deselect_all();
							working_canvas.activ_cells[selection_position.y][selection_position.x + 1] = true;
						}
						else
						{
							value_to_add = event.text.unicode;
							working_canvas.set_char_selected(value_to_add);
							working_canvas.deselect_all();
							working_canvas.activ_cells[selection_position.y + 1][0] = true;
						}
					}
					else
					{
						if (selection_position.x+1 < working_canvas.size_x)
						{
							value_to_add = event.text.unicode;
							working_canvas.set_char_selected(value_to_add);
							working_canvas.deselect_all();
							working_canvas.activ_cells[selection_position.y][selection_position.x + 1] = true;
						}
						else
						{
							value_to_add = event.text.unicode;
							working_canvas.set_char_selected(value_to_add);
						}
					}
				}
			}
		}
	}
}

void loop_arrow_movement_management() {
	//this parts adds usability to the keys in order to move the cursor if it's just one selected
	if (event.type == sf::Event::KeyPressed && !prev_any_key_is_pressed)
	{
		if (working_canvas.return_ammount_selected() == 1)
		{
			selection_position = working_canvas.first_position_selection();

			if (sf::Keyboard::isKeyPressed( sf::Keyboard::Left ))
			{
				if (selection_position.x > 0)
				{
					working_canvas.deselect_all();
					working_canvas.activ_cells[selection_position.y][selection_position.x - 1] = true;
				}
			}
			if (sf::Keyboard::isKeyPressed( sf::Keyboard::Right ))
			{
				if (selection_position.x < working_canvas.size_x - 1)
				{
					working_canvas.deselect_all();
					working_canvas.activ_cells[selection_position.y][selection_position.x + 1] = true;
				}
			}
			if (sf::Keyboard::isKeyPressed( sf::Keyboard::Up ))
			{
				if (selection_position.y > 0)
				{
					working_canvas.deselect_all();
					working_canvas.activ_cells[selection_position.y - 1][selection_position.x] = true;
				}
			}
			if (sf::Keyboard::isKeyPressed( sf::Keyboard::Down ))
			{
				if (selection_position.y < working_canvas.size_y - 1)
				{
					working_canvas.deselect_all();
					working_canvas.activ_cells[selection_position.y + 1][selection_position.x] = true;
				}
			}
		}
	}
}

void loop_start() {
	//currently zoom is disabled with this option from here
	//view1.setSize(sf::Vector2f(window.getSize().x, window.getSize().y));
	hud_view.setSize(  sf::Vector2f(window.getSize().x, window.getSize().y));
	hud_view.setCenter(window.getSize().x/2, window.getSize().y/2);

	upper_toolbar.pos = sf::Vector2i((int)window.getSize().x / 2, (int)window.getSize().y * 0.02);
	main_toolbar.pos  = sf::Vector2i((int)window.getSize().x / 2, (int)window.getSize().y - (int)window.getSize().y * 0.15);
	upper_toolbar.update();
	main_toolbar.update();

	//input management (part 1)
	window.setView(canvas_view);
	mouse_position = window.mapPixelToCoords(sf::Mouse::getPosition(window));
	window.setView(hud_view);
	hud_mouse_position = window.mapPixelToCoords(sf::Mouse::getPosition(window));

	out_of_canvas  = !inside_rect((sf::Vector2i)mouse_position, displacement_x, displacement_y, working_canvas.size_x*cell_size_x, working_canvas.size_y*cell_size_y);
	//displacement_v should be a 2d vector in first place
	displacement_v = sf::Vector2i(displacement_x, displacement_y);
	cell_location_vector = cell_location((sf::Vector2i)mouse_position - displacement_v, working_canvas, cell_size_x, cell_size_y);


	left_mouse_button_is_down = sf::Mouse::isButtonPressed(sf::Mouse::Left);
	left_mouse_button_just_down = false;
	if (left_mouse_button_is_down && !prev_left_mouse_button_is_down) {
		left_mouse_button_just_down = true; //std::cout << "left m";
	}
	left_mouse_button_just_up = false;
	if (!left_mouse_button_is_down && prev_left_mouse_button_is_down) {
		left_mouse_button_just_up = true; //std::cout << "up";
	}
}

void loop_canvas_button() {
	//std::cout << "1\n";
	canvas_button_pos_x = displacement_x + working_canvas.size_x * cell_size_x + 6;
	canvas_button_pos_y = displacement_y + working_canvas.size_y * cell_size_y + 6;

	//we move the corner button
	if (moving_canvas_button)
	{
		canvas_button_pos_x       += ( - prev_mouse_position + mouse_position).x;
		canvas_button_image.pos.x += ( - prev_mouse_position + mouse_position).x;
		canvas_button_pos_y       += ( - prev_mouse_position + mouse_position).y;
		canvas_button_image.pos.y += ( - prev_mouse_position + mouse_position).y;

		canvas_button_image.spr.setPosition(canvas_button_image.pos.x, canvas_button_image.pos.y);

		increment_decrement_vector =                               cell_location((sf::Vector2i)mouse_position         - displacement_v , working_canvas, cell_size_x, cell_size_y);
		increment_decrement_vector = increment_decrement_vector - (cell_location((sf::Vector2i)initial_mouse_position - displacement_v , working_canvas, cell_size_x, cell_size_y));
	}
	else
	{
		canvas_button_image.pos.x = canvas_button_pos_x;
		canvas_button_image.pos.y = canvas_button_pos_y;
		canvas_button_image.spr.setPosition(canvas_button_image.pos.x, canvas_button_image.pos.y);
	}
}

void loop_end() {
	//prev variables
	prev_moving_selection_around   = moving_selection_around;
	prev_mouse_position            = mouse_position;
	prev_left_mouse_button_is_down = left_mouse_button_is_down;
	prev_any_key_pressed           = (event.type == sf::Event::TextEntered);
	prev_any_key_is_pressed        = (event.type == sf::Event::KeyPressed);

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl)) { prev_pressing_Lctrl  = true;} else { prev_pressing_Lctrl  = false;}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::RControl)) { prev_pressing_Rctrl  = true;} else { prev_pressing_Rctrl  = false;}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Tab     )) { prev_pressing_Tab    = true;} else { prev_pressing_Tab    = false;}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift  )) { prev_pressing_Lshift = true;} else { prev_pressing_Lshift = false;}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::RShift  )) { prev_pressing_Rshift = true;} else { prev_pressing_Rshift = false;}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::N       )) { prev_pressing_N      = true;} else { prev_pressing_N      = false;}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W       )) { prev_pressing_W      = true;} else { prev_pressing_W      = false;}

}

void loop_move_selection() {
	//system to pan the canvas around the view OR move a selection around
	moving_selection_around = false;
	if (sf::Mouse::isButtonPressed(sf::Mouse::Middle))
	{
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl))
		{
			moving_selection_around = true;
		}
		else
		{
			displacement_x += mouse_position.x - prev_mouse_position.x;
			displacement_y += mouse_position.y - prev_mouse_position.y;
		}
	}

	// in case we just started dragging out selection
	if (moving_selection_around && !prev_moving_selection_around)
	{
		drag_and_drop_starting_point          = (sf::Vector2i)mouse_position;
		working_canvas.drag_drop_activ_cells  = working_canvas.activ_cells;
		working_canvas.drag_drop_cell_letters = working_canvas.cell_letters;
	}
	// if we are finished
	else if (!moving_selection_around && prev_moving_selection_around)
	{
		working_canvas.set_char_selected(' ');
		working_canvas.deselect_all();

		working_canvas.overlay_drag_and_drop(

		working_canvas.drag_drop_activ_cells,
		working_canvas.drag_drop_cell_letters,
		(
			cell_location(
			((sf::Vector2i)mouse_position - (sf::Vector2i)drag_and_drop_starting_point),
			working_canvas, cell_size_x, cell_size_y)
		)

		);
	}
}

void loop_right_click_deselect() {
	//used to clear the selection
	if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
	{
		//this deletes stuff
		working_canvas.deselect_all();
	}
}

int main()
{
	loop_awake();

	sf::Clock clock;while (window.isOpen())
	{
		clock.restart();

		loop_start();

		loop_canvas_button();

		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				window.close();
			}
			else if (event.type == sf::Event::MouseWheelMoved)
			{
				if ((zoom + event.mouseWheel.delta * 0.1) > 0.2)
				{
					zoom += (event.mouseWheel.delta * 0.1);
					//view1.setSize(initial_size_x * zoom, initial_size_y * zoom);
					canvas_view.setSize(sf::Vector2f(window.getSize().x * zoom, window.getSize().y * zoom));
				}
			}
		}

		loop_logic_shorcuts();

		loop_logic_buttons_clicks();

		loop_text_input_management();

		loop_arrow_movement_management();

		loop_move_selection();

		loop_right_click_deselect();

		loop_render();

		loop_end();

		//adjusting the timing in the loop
		elapsed = clock.getElapsedTime();
		float sleepTime = 1.f / 60.f - elapsed.asSeconds();
		if(sleepTime > 0.f)
		{
			sf::sleep(sf::seconds(sleepTime));
		}

	}
	return 0;
}
