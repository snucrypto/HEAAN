/*
* Copyright (c) by CryptoLab inc.
* This program is licensed under a
* Creative Commons Attribution-NonCommercial 3.0 Unported License.
* You should have received a copy of the license along with this
* work.  If not, see <http://creativecommons.org/licenses/by-nc/3.0/>.
*/
#include "Key.h"

Key::Key(uint64_t* rax, uint64_t* rbx, long N, long np) : rax(rax), rbx(rbx), N(N), np(np) {

}

Key::~Key() {
	if(rax != NULL) delete[] rax;
	if(rbx != NULL) delete[] rbx;
}
