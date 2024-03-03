#include "Button.hpp"

#include <string>

using namespace std;

Button::Button(string n, int px, int py, int pw, int ph, unsigned char r1, unsigned char g1, unsigned char b1, unsigned char r2, unsigned char g2, unsigned char b2, bool s) : name(n), show(s) {
	area = {px, py, pw, ph};
	buttoncolor = {r1, g1, b1, 255};
	textcolor = {r2, g2, b2, 255};
}