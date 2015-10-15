/*
**  UICore
**  Copyright (c) 1997-2015 The UICore Team
**
**  This software is provided 'as-is', without any express or implied
**  warranty.  In no event will the authors be held liable for any damages
**  arising from the use of this software.
**
**  Permission is granted to anyone to use this software for any purpose,
**  including commercial applications, and to alter it and redistribute it
**  freely, subject to the following restrictions:
**
**  1. The origin of this software must not be misrepresented; you must not
**     claim that you wrote the original software. If you use this software
**     in a product, an acknowledgment in the product documentation would be
**     appreciated but is not required.
**  2. Altered source versions must be plainly marked as such, and must not be
**     misrepresented as being the original software.
**  3. This notice may not be removed or altered from any source distribution.
**
**  Note: Some of the libraries UICore may link to may have additional
**  requirements or restrictions.
**
**  File Author(s):
**
**    Mark Page
*/

#pragma once

#include "UICore/Core/System/databuffer.h"
#include "UICore/Core/IOData/iodevice.h"
#include "UICore/Core/Crypto/secret.h"
#include "UICore/Core/Crypto/random.h"
#include "UICore/Core/Crypto/rsa.h"
#include "UICore/Core/Crypto/hash_functions.h"
#include "UICore/Core/Crypto/tls_client.h"
#include "x509.h"

namespace uicore
{
	enum TLS_ConnectionEnd
	{
		cl_tls_connection_server,
		cl_tls_connection_client
	};

	enum TLS_BulkCipherAlgorithm
	{
		cl_tls_cipher_algorithm_null,
		cl_tls_cipher_algorithm_rc4,
		cl_tls_cipher_algorithm_rc2,
		cl_tls_cipher_algorithm_des,
		cl_tls_cipher_algorithm_3des,
		cl_tls_cipher_algorithm_des40,
		cl_tls_cipher_algorithm_aes128,
		cl_tls_cipher_algorithm_aes256
	};

	enum TLS_CipherType
	{
		cl_tls_cipher_type_stream,
		cl_tls_cipher_type_block
	};

	enum TLS_MACAlgorithm
	{
		cl_tls_mac_algorithm_null,
		cl_tls_mac_algorithm_md5,
		cl_tls_mac_algorithm_sha,
		cl_tls_mac_algorithm_sha256
	};

	enum TLS_CompressionMethod
	{
		cl_tls_compression_null = 0
	};

	enum TLS_HandshakeType
	{
		cl_tls_handshake_hello_request = 0,
		cl_tls_handshake_client_hello = 1,
		cl_tls_handshake_server_hello = 2,
		cl_tls_handshake_certificate = 11,
		cl_tls_handshake_server_key_exchange = 12,
		cl_tls_handshake_certificate_request = 13,
		cl_tls_handshake_server_hello_done = 14,
		cl_tls_handshake_certificate_verify = 15,
		cl_tls_handshake_client_key_exchange = 16,
		cl_tls_handshake_finished = 20
	};

	enum TLS_ContentType
	{
		cl_tls_content_change_cipher_spec = 20,
		cl_tls_content_alert = 21,
		cl_tls_content_handshake = 22,
		cl_tls_content_application_data = 23
	};

	enum TLS_AlertLevel
	{
		cl_tls_warning = 1,
		cl_tls_fatal = 2
	};

	enum TLS_AlertDescription
	{
		cl_tls_close_notify = 0,
		cl_tls_unexpected_message = 10,
		cl_tls_bad_record_mac = 20,
		cl_tls_decryption_failed = 21,
		cl_tls_record_overflow = 22,
		cl_tls_decompression_failure = 30,
		cl_tls_handshake_failure = 40,
		cl_tls_bad_certificate = 42,
		cl_tls_unsupported_certificate = 43,
		cl_tls_certificate_revoked = 44,
		cl_tls_certificate_expired = 45,
		cl_tls_certificate_unknown = 46,
		cl_tls_illegal_parameter = 47,
		cl_tls_unknown_ca = 48,
		cl_tls_access_denied = 49,
		cl_tls_decode_error = 50,
		cl_tls_decrypt_error = 51,
		cl_tls_export_restriction = 60,
		cl_tls_protocol_version = 70,
		cl_tls_insufficient_security = 71,
		cl_tls_internal_error = 80,
		cl_tls_user_canceled = 90,
		cl_tls_no_renegotiation = 100
	};

	typedef struct	// As stated in TLS specification
	{
		uint8_t major, minor;
	} TLS_ProtocolVersion;

	typedef struct	// As stated in TLS specification
	{
		uint8_t type;		// TLS_ContentType
		TLS_ProtocolVersion version;
		uint8_t length[2];
	} TLS_Record;

	typedef struct	// As stated in TLS specification
	{
		uint8_t msg_type;		// TLS_HandshakeType
		uint8_t length[3];			// bytes in message
	} TLS_Handshake;

	typedef struct	// As stated in TLS specification
	{
		uint8_t gmt_unix_time[4];
		uint8_t random_bytes[28];		// opaque
	} TLS_Random;

	class TLS_SecurityParameters
	{
	public:
		TLS_SecurityParameters()
		{
			reset();
		}

		void reset()
		{
			entity = cl_tls_connection_client;
			bulk_cipher_algorithm = cl_tls_cipher_algorithm_null;
			cipher_type = cl_tls_cipher_type_block;
			key_size = 0;
			key_material_length = 0;
			iv_size = 0;
			is_exportable = false;
			mac_algorithm = cl_tls_mac_algorithm_null;
			hash_size = 0;
			compression_algorithm = cl_tls_compression_null;
			master_secret = Secret::create(48);
			client_random = Secret::create(32);
			server_random = Secret::create(32);
			is_send_encrypted = false;
			is_receive_encrypted = false;

			client_write_mac_secret = SecretPtr();
			server_write_mac_secret = SecretPtr();
			client_write_key = SecretPtr();
			server_write_key = SecretPtr();
			client_write_iv = SecretPtr();
			server_write_iv = SecretPtr();
			read_sequence_number = 0;
			write_sequence_number = 0;
		}

		TLS_ConnectionEnd entity;
		TLS_BulkCipherAlgorithm bulk_cipher_algorithm;
		TLS_CipherType cipher_type;
		uint8_t key_size;
		uint8_t key_material_length;
		uint8_t iv_size;
		bool is_exportable;
		TLS_MACAlgorithm mac_algorithm;
		uint8_t hash_size;
		TLS_CompressionMethod compression_algorithm;
		SecretPtr master_secret;
		SecretPtr client_random;
		SecretPtr server_random;

		SecretPtr client_write_mac_secret;
		SecretPtr server_write_mac_secret;
		SecretPtr client_write_key;
		SecretPtr server_write_key;
		SecretPtr client_write_iv;
		SecretPtr server_write_iv;
		uint64_t read_sequence_number;
		uint64_t write_sequence_number;

		bool is_send_encrypted;
		bool is_receive_encrypted;

	};

	enum TLS_ConversationState
	{
		cl_tls_state_send_client_hello,
		cl_tls_state_receive_server_hello,
		cl_tls_state_receive_certificate,
		cl_tls_state_receive_server_hello_done,
		cl_tls_state_send_client_key_exchange,
		cl_tls_state_send_change_cipher_spec,
		cl_tls_state_send_finished,
		cl_tls_state_receive_change_cipher_spec,
		cl_tls_state_receive_finished,
		cl_tls_state_connected,
		cl_tls_state_error
	};

	class TLSClient_Impl : public TLSClient
	{
	public:
		TLSClient_Impl();
		~TLSClient_Impl();

		const void *get_decrypted_data() const override;
		int get_decrypted_data_available() const override;

		const void *get_encrypted_data() const override;
		int get_encrypted_data_available() const override;

		int encrypt(const void *data, int size) override;
		int decrypt(const void *data, int size) override;

		void decrypted_data_consumed(int size) override;
		void encrypted_data_consumed(int size) override;

	private:
		void progress_conversation();

		bool can_send_record() const;
		void send_record(void *data_ptr, unsigned int data_size);	// !< Note "data_ptr" may be written to

		bool receive_record();

		void change_cipher_spec_data(DataBufferPtr record_plaintext);
		void alert_data(DataBufferPtr record_plaintext);
		void handshake_data(DataBufferPtr record_plaintext);
		void application_data(DataBufferPtr record_plaintext);

		void handshake_hello_request_received(const void *data, int size);
		void handshake_client_hello_received(const void *data, int size);
		void handshake_server_hello_received(const void *data, int size);
		void handshake_certificate_received(const void *data, int size);
		void handshake_server_key_exchange_received(const void *data, int size);
		void handshake_certificate_request_received(const void *data, int size);
		void handshake_server_hello_done_received(const void *data, int size);
		void handshake_certificate_verify_received(const void *data, int size);
		void handshake_client_key_exchange_received(const void *data, int size);
		void handshake_finished_received(const void *data, int size);

		bool send_client_hello();
		bool send_client_key_exchange();
		bool send_change_cipher_spec();
		bool send_finished();
		bool send_application_data();

		void reset();

		void copy_data(void *out_data, int size, const void *&data, int &data_left);

		void set_tls_record(unsigned char *dest_ptr, TLS_ContentType content_type, unsigned int length);
		void set_tls_handshake(unsigned char *dest_ptr, TLS_HandshakeType handshake_type, unsigned int length);
		void set_tls_protocol_version(unsigned char *dest_ptr);
		void create_security_parameters_client_random();
		void set_tls_random(unsigned char *dest_ptr, SecretPtr &time_and_random_struct) const;
		int get_session_id_length() const;
		void set_session_id(unsigned char *dest_ptr) const;
		int get_compression_methods_length() const;
		void set_compression_methods(unsigned char *dest_ptr) const;
		int get_cipher_suites_length() const;
		void set_cipher_suites(unsigned char *dest_ptr) const;
		void select_cipher_suite(uint8_t value1, uint8_t value2);
		void select_compression_method(uint8_t value);
		void inspect_certificate(std::vector<unsigned char> &cert);
		void set_server_public_key();
		void PRF(void *output_ptr, unsigned int output_size, const SecretPtr &secret, const char *label_ptr, const SecretPtr &seed_part1, const SecretPtr &seed_part2);
		void hash_handshake(const void *data_ptr, unsigned int data_size);

		DataBufferPtr decrypt_record(TLS_Record &record, const DataBufferPtr &record_data);
		DataBufferPtr decrypt_data(const void *data_ptr, unsigned int data_size);

		SecretPtr calculate_mac(const void *data_ptr, unsigned int data_size, const void *data2_ptr, unsigned int data2_size, uint64_t sequence_number, const SecretPtr &mac_secret);
		DataBufferPtr encrypt_data(const void *data_ptr, unsigned int data_size, const void *mac_ptr, unsigned int mac_size);

		static const unsigned int max_record_length = 2 << 14;	// RFC 2246 (6.2.1)
		static const unsigned int max_handshake_length = 2 << 24;	// RFC 2246 (implied by length in7.4)

		static const int desired_buffer_size = 64 * 1024;

		DataBufferPtr recv_in_data = DataBuffer::create(0);
		int recv_in_data_read_pos = 0;

		DataBufferPtr recv_out_data = DataBuffer::create(0);
		int recv_out_data_read_pos = 0;

		DataBufferPtr send_in_data = DataBuffer::create(0);
		int send_in_data_read_pos = 0;

		DataBufferPtr send_out_data = DataBuffer::create(0);
		int send_out_data_read_pos = 0;

		DataBufferPtr handshake_in_data = DataBuffer::create(0);
		int handshake_in_read_pos = 0;

		TLS_ConversationState conversation_state = cl_tls_state_send_client_hello;

		DataBufferPtr record_data_buffer = DataBuffer::create(0); // local variable of receive_record(). Placed here to avoid allocating memory each time a record is processed

		TLS_SecurityParameters security_parameters;
		TLS_ProtocolVersion protocol;

		DataBufferPtr server_public_exponent = DataBuffer::create(0);
		DataBufferPtr server_public_modulus = DataBuffer::create(0);

		bool is_protocol_chosen = false;	// Set by the server hello response

		RandomPtr m_Random = Random::create();

		MD5Ptr client_handshake_md5_hash = MD5::create();
		SHA1Ptr client_handshake_sha1_hash = SHA1::create();
		MD5Ptr server_handshake_md5_hash = MD5::create();
		SHA1Ptr server_handshake_sha1_hash = SHA1::create();

		std::vector<X509> certificate_chain;
	};
}
