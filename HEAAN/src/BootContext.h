/*
* Copyright (c) by CryptoLab inc.
* This program is licensed under a
* Creative Commons Attribution-NonCommercial 3.0 Unported License.
* You should have received a copy of the license along with this
* work.  If not, see <http://creativecommons.org/licenses/by-nc/3.0/>.
*/
#ifndef HEAAN_BOOTCONTEXT_H_
#define HEAAN_BOOTCONTEXT_H_

#include <NTL/ZZX.h>

using namespace NTL;

class BootContext {

public:

	ZZX* pvec; ///< encodings of "diagonal" values of CoeffToSlot matrix
	ZZX* pvecInv; ///< encodings of "diagonal" values of SlotToCoeff matrix

	ZZX p1; ///< auxiliary encoding for EvalExp
	ZZX p2; ///< auxiliary encoding for EvalExp

	long logp; ///< number of quantized bits

	BootContext(ZZX* pvec = NULL, ZZX* pvecInv = NULL, ZZX p1 = ZZX::zero(), ZZX p2 = ZZX::zero(), long logp = 0);

};

#endif
