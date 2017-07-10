#ifndef SCHEME_SCHEME_H_
#define SCHEME_SCHEME_H_

#include "CZZ.h"
#include "SecKey.h"
#include "Cipher.h"
#include "Message.h"
#include "PubKey.h"
#include "SchemeAux.h"

using namespace std;
using namespace NTL;

class Scheme {
private:
public:
	Params& params;
	PubKey& publicKey;
	SchemeAux& aux;

	//-----------------------------------------

	Scheme(Params& params, PubKey& publicKey, SchemeAux& schemeaux) : params(params), publicKey(publicKey), aux(schemeaux) {};

	//-----------------------------------------

	/**
	 * @param[in] level
	 * @return modulus of given level
	 */
	ZZ getqi(long& level);

	/**
	 * @param[in] level
	 * @return big modulus of given level
	 */
	ZZ getPqi(long& level);

	/**
	 * @param level
	 * @return log of modulus of given level
	 */
	long getLogqi(long& level);

	//-----------------------------------------

	/**
	 * regroup vals indexes (i) -> (3^i mod slots) and add conjugates
	 * @param[in] vals
	 * @param[in] slots
	 * @return regrouped vals with conjugates
	 */
	CZZ* groupidx(CZZ*& vals, long slots);

	/**
	 * groupidx with only one val
	 * @param[in] val
	 * @param[in] slots
	 * @return regrouped vals
	 */
	CZZ* groupidx(CZZ& val);

	/**
	 * degroup vals indexes (3^i mod N) -> (i) and remove conjugates
	 * @param[in] vals
	 * @param[in] slots
	 * @return degrouped vals without conjugates
	 */
	CZZ* degroupidx(CZZ*& vals, long dslots);

	//-----------------------------------------

	/**
	 * encodes regrouped vals into ZZX using fft inverse
	 * @param[in] vals
	 * @param[in] slots
	 * @return Message ZZX slots and level
	 */
	Message encode(CZZ*& vals, long slots);

	/**
	 * decodes ZZX into regrouped vals using fft
	 * @param[in] message
	 * @return array of CZZ values
	 */
	CZZ* decode(Message& msg);

	//-----------------------------------------

	/**
	 * encrypts message in RLWE instance bx = ax * sx + ex + mx mod qi
	 * @param[in] message
	 * @param[in] level
	 * @return cipher
	 */
	Cipher encryptMsg(Message& msg, long level = 1);

	/**
	 * decrypts cipher
	 * @param[in] secret key
	 * @param[in] cipher
	 * @return message
	 */
	Message decryptMsg(SecKey& secretKey, Cipher& cipher);

	//-----------------------------------------

	/**
	 * All encryption process: regroup vals with adding conjugates, encode and encrypts
	 * @param[in] vals
	 * @param[in] slots
	 * @param[in] level
	 * @return cipher
	 */
	Cipher encrypt(CZZ*& vals, long slots, long level = 1);

	/**
	 * All decryption process: decrypt, decode, and degroup vals with removing conjugates
	 * @param[in] cipher
	 * @return vals
	 */
	CZZ* decrypt(SecKey& secretKey, Cipher& cipher);

	//-----------------------------------------

	/**
	 * encrypt with only one value
	 * @param[in] val
	 * @param[in] level
	 * @return cipher
	 */
	Cipher encryptSingle(CZZ& val, long level = 1);

	/**
	 * decrypt with advance knowledge that slots = 1;
	 * @param[in] cipher
	 * @return val
	 */
	CZZ decryptSingle(SecKey& secretKey, Cipher& cipher);

	//-----------------------------------------

	/**
	 * addition of ciphers
	 * @param[in] cipher(m1)
	 * @param[in] cipher(m2)
	 * @return cipher(m1 + m2)
	 */
	Cipher add(Cipher& cipher1, Cipher& cipher2);

	/**
	 * addition of ciphers
	 * @param[in, out] cipher(m1) -> cipher(m1 + m2)
	 * @param[in] cipher(m2)
	 */
	void addAndEqual(Cipher& cipher1, Cipher& cipher2);

	//-----------------------------------------

	/**
	 * constant addition
	 * @param[in] cipher(m)
	 * @param[in] constant
	 * @return cipher(m + constant)
	 */
	Cipher addConst(Cipher& cipher, ZZ& cnst);

	/**
	 * constant addition
	 * @param[in, out] cipher(m) -> cipher(m + constant)
	 * @param[in] constant
	 */
	void addConstAndEqual(Cipher& cipher, ZZ& cnst);

	//-----------------------------------------

	/**
	 * substraction of ciphers
	 * @param[in] cipher(m1)
	 * @param[in] cipher(m2)
	 * @return cipher(m1 - m2)
	 */
	Cipher sub(Cipher& cipher1, Cipher& cipher2);

	/**
	 * substraction of ciphers
	 * @param[in, out] cipher(m1) -> cipher(m1 - m2)
	 * @param[in] cipher(m2)
	 */
	void subAndEqual(Cipher& cipher1, Cipher& cipher2);

	/**
	 * conjugation of cipher
	 * @param[in] cipher(m)
	 * @return cipher(conj(m))
	 */
	Cipher conjugate(Cipher& cipher);

	/**
	 * conjugation of cipher
	 * @param[in, out] cipher(m) -> cipher(conj(m))
	 */
	void conjugateAndEqual(Cipher& cipher);

	//-----------------------------------------

	/**
	 * multiplication of ciphers. This algorithm contain linearization.
	 * To manage the noise we usually need modular switching method after mult
	 * @param[in] cipher(m1)
	 * @param[in] cipher(m2)
	 * @return cipher(m1 * m2)
	 */
	Cipher mult(Cipher& cipher1, Cipher& cipher2);

	/**
	 * multiplication of ciphers. This algorithm contain linearization.
	 * To manage the noise we usually need modular switching method after mult
	 * @param[in, out] cipher(m1) -> cipher(m1 * m2)
	 * @param[in] cipher(m2)
	 */
	void multAndEqual(Cipher& cipher1, Cipher& cipher2);

	//-----------------------------------------

	/**
	 * square of cipher. This algorithm contain linearization.
	 * To manage the noise we usually need modular switching method after square
	 * @param[in] cipher(m)
	 * @return cipher(m^2)
	 */
	Cipher square(Cipher& cipher);

	/**
	 * square of cipher. This algorithm contain linearization.
	 * To manage the noise we usually need modular switching method after square
	 * @param[in, out] cipher(m) -> cipher(m^2)
	 */
	void squareAndEqual(Cipher& cipher);

	//-----------------------------------------

	/**
	 * constant multiplication
	 * @param[in] cipher(m)
	 * @param[in] constant
	 * @return cipher(m * constant)
	 */
	Cipher multByConst(Cipher& cipher, ZZ& cnst);

	/**
	 * constant multiplication
	 * @param[in, out] cipher(m) -> cipher(m * constant)
	 * @param[in] constant
	 */
	void multByConstAndEqual(Cipher& cipher, ZZ& cnst);

	//-----------------------------------------

	/**
	 * X^degree multiplication
	 * @param[in] cipher(m)
	 * @param[in] degree
	 * @return cipher(m * X^degree)
	 */
	Cipher multByMonomial(Cipher& cipher, const long& degree);

	/**
	 * X^degree multiplication
	 * @param[in, out] cipher(m) -> cipher(m * X^degree)
	 * @param[in] degree
	 */
	void multByMonomialAndEqual(Cipher& cipher, const long& degree);

	//-----------------------------------------

	/**
	 * 2^bits multiplication
	 * @param[in] cipher(m)
	 * @param[in] bits
	 * @return cipher(m * 2^bits)
	 */
	Cipher leftShift(Cipher& cipher, long& bits);

	/**
	 * 2^bits multiplication
	 * @param[in, out] cipher(m) -> cipher(m * 2^bits)
	 * @param[in] bits
	 */
	void leftShiftAndEqual(Cipher& cipher, long& bits);

	/**
	 * doubles
	 * @param[in, out] cipher(m) -> cipher(2m)
	 */
	void doubleAndEqual(Cipher& cipher);

	//-----------------------------------------

	/**
	 * modulus switching procedure
	 * @param[in] cipher(m)
	 * @param[in] new level
	 * @return cipher(m/p^(newlevel-oldlevel)) with new level
	 */
	Cipher modSwitch(Cipher& cipher, long newLevel);

	/**
	 * modulus switching procedure
	 * @param[in, out] cipher(m) -> cipher(m/p^(newlevel-oldlevel)) with new level
	 * @param[in] new level
	 */
	void modSwitchAndEqual(Cipher& cipher, long newLevel);

	/**
	 * modulus switching procedure
	 * @param[in] cipher(m)
	 * @return cipher(m/p) with one level up
	 */
	Cipher modSwitchOne(Cipher& cipher);

	/**
	 * modulus switching procedure
	 * @param[in, out] cipher(m) -> cipher(m/p) with one level up
	 */
	void modSwitchOneAndEqual(Cipher& cipher);

	//-----------------------------------------

	/**
	 * modulus embedding procedure
	 * @param[in] cipher(m)
	 * @param[in] new level
	 * @return cipher(m) with new level
	 */
	Cipher modEmbed(Cipher& cipher, long newLevel);

	/**
	 * modulus embedding procedure
	 * @param[in, out] cipher(m) -> cipher(m) with new level
	 * @param[in] new level
	 */
	void modEmbedAndEqual(Cipher& cipher, long newLevel);

	/**
	 * modulus embedding procedure
	 * @param[in] cipher(m)
	 * @return cipher(m) with one level up
	 */
	Cipher modEmbedOne(Cipher& cipher);

	/**
	 * modulus embedding procedure
	 * @param[in, out] cipher(m) -> cipher(m) with one level up
	 */
	void modEmbedOneAndEqual(Cipher& cipher);

	//-----------------------------------------

	/**
	 * multiplication of ciphers and modulus switching procedure
	 * @param[in] cipher(m1)
	 * @param[in] cipher(m2)
	 * @return cipher(m1 * m2 / p) with one level up
	 */
	Cipher multAndModSwitchOne(Cipher& cipher1, Cipher& cipher2);

	/**
	 * multiplication of ciphers and modulus switching procedure
	 * @param[in, out] cipher(m1) -> cipher(m1 * m2 / p) with one level up
	 * @param[in] cipher(m2)
	 */
	void multModSwitchOneAndEqual(Cipher& cipher1, Cipher& cipher2);

	//-----------------------------------------

	/**
	 * calculates cipher of array with rotated indexes
	 * @param[in] cipher(m(v_1, v_2, ..., v_slots))
	 * @param[in] log of rotation slots
	 * @return cipher(m(v_{1+2^logsteps}, v_{2+2^logsteps}, ..., v_{slots+2^logsteps})
	 */
	Cipher leftRotateByPo2(Cipher& cipher, long& logrotSlots);
	/**
	 * calculates cipher of array with rotated indexes
	 * @param[in, out] cipher(m(v_1, v_2, ..., v_slots)) -> cipher(m(v_{1+2^logsteps}, v_{2+2^logsteps}, ..., v_{slots+2^logsteps})
	 * @param[in] log of rotation slots
	 * @return
	 */
	void leftRotateByPo2AndEqual(Cipher& cipher, long& logrotSlots);

	/**
	 * calculates cipher of array with rotated indexes
	 * @param[in] cipher(m(v_1, v_2, ..., v_slots))
	 * @param[in] rotation slots
	 * @return cipher(m(v_{1+steps}, v_{2+steps}, ..., v_{slots+steps})
	 */
	Cipher leftRotate(Cipher& cipher, long& rotSlots);

	/**
	 * calculates cipher of array with rotated indexes
	 * @param[in] cipher(m(v_1, v_2, ..., v_slots)) -> cipher(m(v_{1+steps}, v_{2+steps}, ..., v_{slots+steps})
	 * @param[in] rotation slots
	 */
	void leftRotateAndEqual(Cipher& cipher, long& rotSlots);

	//-----------------------------------------

};

#endif /* SCHEME_SCHEME_H_ */
