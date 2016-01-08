#include <windef.h>
#include <vector>
#include <string>

class vDropbox
{
public:
	int sel;
	HWND elem, elemp;
	std::string saveloc;
	std::vector< std::string > list;
	
	vDropbox(HWND parent, HWND self, const char* loc)
	{ 
		elemp = parent;
		elem = self;
		sel = 0;
		saveloc = loc;
	}
	
	void push_back(std::string item) { list.push_back(item); }
	//std::string& operator[](size_type n) { return list[n]; }
	
	void select() { SendMessage(elem, CB_SETCURSEL, sel, 0); }
	
	void select(int derp) {
		if (derp < 0) derp = 0;
		if ((unsigned)derp >= brackets.size()) sel = brackets.size() - 1;
		
		select();
	}
	
	void initialize_display() {	
		std::vector< std::string >::iterator it = brackets.begin();
		while (it != brackets.end()) {
			SendMessage(elem, (UINT)CB_ADDSTRING, (WPARAM)0,(LPARAM)((*it).c_str()));
			++it;
		}
		select();
	}
};