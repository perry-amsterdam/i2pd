#ifndef AES_H__
#define AES_H__

#include <inttypes.h>
#include <cryptopp/modes.h>
#include <cryptopp/aes.h>

namespace i2p
{
namespace crypto
{	
	union ChipherBlock	
	{
		uint8_t buf[16];
		uint64_t ll[2];
	};

#ifdef __x86_64__
	// AES-NI assumed
	class ECNCryptoAESNI
	{
		public:
		
			void SetKey (const uint8_t * key);
			void Encrypt (const ChipherBlock * in, ChipherBlock * out);	
			void Decrypt (const ChipherBlock * in, ChipherBlock * out);		

		private:

			uint32_t m_KeySchedule[4*(14+1)]; // 14 rounds for AES-256
	};	

#endif			

	class CBCEncryption
	{
		public:
	
			CBCEncryption () { memset (m_LastBlock.buf, 0, 16); };

			void SetKey (const uint8_t * key) { m_ECBEncryption.SetKey (key, 32); }; // 32 bytes
			void SetIV (const uint8_t * iv) { memcpy (m_LastBlock.buf, iv, 16); }; // 16 bytes

			void Encrypt (int numBlocks, const ChipherBlock * in, ChipherBlock * out);
			bool Encrypt (const uint8_t * in, std::size_t len, uint8_t * out);

		private:

			ChipherBlock m_LastBlock;
			CryptoPP::ECB_Mode<CryptoPP::AES>::Encryption m_ECBEncryption;
	};

	class CBCDecryption
	{
		public:
	
			CBCDecryption () { memset (m_IV.buf, 0, 16); };

			void SetKey (const uint8_t * key) { m_ECBDecryption.SetKey (key, 32); }; // 32 bytes
			void SetIV (const uint8_t * iv) { memcpy (m_IV.buf, iv, 16); }; // 16 bytes

			void Decrypt (int numBlocks, const ChipherBlock * in, ChipherBlock * out);
			bool Decrypt (const uint8_t * in, std::size_t len, uint8_t * out);

		private:

			ChipherBlock m_IV;
			CryptoPP::ECB_Mode<CryptoPP::AES>::Decryption m_ECBDecryption;
	};	
}
}

#endif

