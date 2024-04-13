/*
 * Copyright (c) 2017-2020 The Linux Foundation. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *  * Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *  * Redistributions in binary form must reproduce the above
 *    copyright notice, this list of conditions and the following
 *    disclaimer in the documentation and/or other materials provided
 *    with the distribution.
 *  * Neither the name of The Linux Foundation nor the names of its
 *    contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS
 * BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
 * BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
 * OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN
 * IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef __TESTS_UTILS__H__
#define __TESTS_UTILS__H__

#include <stdarg.h>
#include <vector>
#include <string>
#include <linux/if_ether.h>
#include <linux/msm_ipa.h>
#include <arpa/inet.h>
#include "TestBase.h"
#include "Constants.h"
#include "RoutingDriverWrapper.h"
#include "InterfaceAbstraction.h"
#include "ipa_test_module.h"
#include "TestManager.h"
#include "Logger.h"
#include "Constants.h"
extern "C" {
#include "ipa_nat_utils.h"
}

using namespace std;
#define TEST_APP_VERSION "2.00"

#define Free(x) do { if (x) {free(x); x = NULL; } } while (0)

#define MAX3(_X, _Y, _Z) max(max((_X), (_Y)), (_Z))

#define ETH2_DST_ADDR_OFFSET (0)
#define ETH2_SRC_ADDR_OFFSET (ETH_ALEN)
#define ETH2_ETH_TYPE_OFFSET (ETH2_SRC_ADDR_OFFSET + ETH_ALEN)
#define ETH2_ETH_TYPE_LEN (2)
#define ETH2_PAYLOAD_OFFSET (ETH2_ETH_TYPE_OFFSET + ETH2_ETH_TYPE_LEN)

#define ETH8021Q_HEADER_LEN (18)
#define ETH8021Q_METADATA_OFFSET (12)
#define ETH8021Q_8021Q_TAG_LEN (4)
#define ETH8021Q_ETH_TYPE_OFFSET (ETH8021Q_METADATA_OFFSET + ETH8021Q_8021Q_TAG_LEN)

#define WLAN_HDR_SIZE (4)
#define RNDIS_HDR_SIZE (44)

// 26 ROME WLAN Frame =
// 4	ROME WLAN header +
// 14	IEEE 802.3 +
// 8	802.2 LLC/SNAP
#define _802_3_HDR_SIZE (26)

// [WLAN][ETH2] header
#define WLAN_ETH2_HDR_SIZE (WLAN_HDR_SIZE + ETH_HLEN)

// [RNDIS][ETH2] header
#define RNDIS_ETH2_HDR_SIZE (RNDIS_HDR_SIZE + ETH_HLEN)
#define IP4_PACKET_SIZE (70) // Arbitrary number

// OFFSET = sizeof(struct rndis_pkt_hdr) - RNDIS_HDR_OFST(data_ofst)
#define RNDIS_DATA_OFFSET (36)

// [WLAN][802.3] header
#define WLAN_802_3_HDR_SIZE (WLAN_HDR_SIZE + _802_3_HDR_SIZE)

#define IPA_CLIENT_IS_PROD(x) \
	(x < IPA_CLIENT_MAX && (x & 0x1) == 0)
#define IPA_CLIENT_IS_CONS(x) \
	(x < IPA_CLIENT_MAX && (x & 0x1) == 1)

// IP V4 header length (20 bytes) plus GRE header length (4 bytes)
#define EoGRE_V4_HDR_LEN (20 + 4)

// IP V6 header length (40 bytes) plus options header length (8 bytes)
// plus GRE header length (4 bytes)
#define EoGRE_V6_HDR_LEN (40 + 8 + 4)

enum msgType {
	ERROR = 0,
	DEBUG,
	INFO,
	STACK
};

/**
	@brief
	Do not Use this function. Use MACROs instead.

	@details
	Do not Use this function.
	Instead use the MACROs: LOG_MSG_ERROR, LOG_MSG_INFO & LOG_MSG_DEBUG
	*/
void __log_msg(enum msgType,
		const char *filename,
		int line, const char *function,
		const char *format, ...);

#define LOG_MSG_ERROR(...) \
__log_msg(ERROR, __FILE__, __LINE__, __func__, __VA_ARGS__)
#define LOG_MSG_DEBUG(...) \
__log_msg(DEBUG, __FILE__, __LINE__, __func__, __VA_ARGS__)
#define LOG_MSG_INFO(...) \
__log_msg(INFO, __FILE__, __LINE__, __func__, __VA_ARGS__)
#define LOG_MSG_STACK(...) \
__log_msg(STACK, __FILE__, __LINE__, __func__, __VA_ARGS__)

/*#define LOG_MSG_ERROR(x...)
__log_msg(ERROR, __FILE__, __LINE__, __func__, x)
#define LOG_MSG_DEBUG(x...)
__log_msg(DEBUG, __FILE__, __LINE__, __func__, x)
#define LOG_MSG_INFO(x...)
__log_msg(INFO, __FILE__, __LINE__, __func__, x)
#define LOG_MSG_STACK(x...)
__log_msg(STACK, __FILE__, __LINE__, __func__, x)*/

/**
	@brief
	Function loads a default IPv4 / IPv6 Packet

	@param [in] eIP - Type of Packet to load (IPA_IP_v4 / IPA_IP_v6)
	@param [in] pBuffer - pointer to the destination buffer
	@param [in,out] nMaxSize - The size of the buffer.
	Upon function return,
	the total number of bytes copied will be stored in this parameter.
	@return boolean indicating whether the
	operation completed successfully or not.

	@details
	Function loads a default IPv4 / IPv6 packet into pBuffer.
	*/
bool LoadDefaultPacket(
	enum ipa_ip_type eIP,
	uint8_t *pBuffer,
	size_t &nMaxSize);

bool LoadDefaultEth2Packet(
	enum ipa_ip_type eIP,
	uint8_t *pBuffer,
	size_t &nMaxSize);

bool LoadDefaultWLANEth2Packet(
	enum ipa_ip_type eIP,
	uint8_t *pBuffer,
	size_t &nMaxSize);

bool LoadDefaultWLAN802_32Packet(
	enum ipa_ip_type eIP,
	uint8_t *pBuffer,
	size_t &nMaxSize);

bool LoadNoPayloadPacket(
	enum ipa_ip_type eIP,
	uint8_t *pBuffer,
	size_t &nMaxSize);

bool LoadDefault802_1Q(
	enum ipa_ip_type eIP,
	uint8_t *pBuffer,
	size_t &nMaxSize);

/**
	@brief
	Function loads a default IPv4 / IPv6 Packet

	@param [in] eIP - Type of Packet to load (IPA_IP_v4 / IPA_IP_v6)
	@param [in] extHdrType - Type of IPV6 extension header(FRAGMENT / NONE)
	@param [in] pBuffer - pointer to the destination buffer
	@param [in,out] nMaxSize - The size of the buffer.
	Upon function return,
	the total number of bytes copied will be stored in this parameter.
	@return boolean indicating whether the
	operation completed successfully or not.

	@details
	Function loads a default IPv4 / IPv6 packet into pBuffer.
	*/
bool LoadDefaultPacket(
		enum ipa_ip_type eIP,
		enum ipv6_ext_hdr_type extHdrType,
		uint8_t *pBuffer,
		size_t &nMaxSize);
/**
	@brief
	Function Sends a Packet, Receive a packet
	and compares the received result with an expected buffer

	@param [in] pSink - Destination to which a packet will be sent.
	@param [in] pSendBuffer -
	Pointer to a buffer containing the packet that will be sent.
	@param [in] nSendBuffSize - The size of the data in the packet.
	@param [in] pSource - Source from which a packet will be received.
	@param [in] pExpectedBuffer - Pointer a
	buffer containing the expected packet (from the receiver)
	@param [in] nExpectedBuffSize - The size of
	valid data within pExpectedBuffer.
	@return Boolean indicating whether the operation
	completed successfully and the buffers matching or not.

	@details
	Function sends a packet to pSink, and receives a packet from pSource.
	The packet received from pSource
	is compared to the expected data from pExpectedBuffer.
	If ExpectData is identical to the
	received data, the function returns TRUE.
	*/
bool SendReceiveAndCompare(
		InterfaceAbstraction * pSink,
		uint8_t *pSendBuffer,
		size_t nSendBuffSize,
		InterfaceAbstraction * pSource,
		uint8_t *pExpectedBuffer,
		size_t nExpectedBuffSize);

/**
	@brief
	This function creates a bypass rule within a table in the Routing block

	@param [in] pRouting - pointer to the Routing Class
	@param [in] eIP - Type of Packet to load (IPA_IP_v4 / IPA_IP_v6)
	@param [in] pTableName - pointer to the Table's Name.
	@param [in] eRuleDestination - destination of the bypass rule.
	@param [in] uHeaderHandle -
	handle to the Header that should be Added (0 should be used as default).
	@param [out] pTableHdl -
	pointer to the table Handle (Can be Null)
	@return boolean indicating whether
	the operation completed successfully or not.

	@details
	This function creates bypass rule within a table in the Routing block.
	*/
bool CreateBypassRoutingTable(
		RoutingDriverWrapper * pRouting,
		enum ipa_ip_type eIP,
		const char *pTableName,
		enum ipa_client_type eRuleDestination,
		uint32_t uHeaderHandle,
		uint32_t *pTableHdl);

/**
	@brief
	This function creates a bypass rule within a table in the Routing block

	@param [in] pRouting - pointer to the Routing Class
	@param [in] eIP - Type of Packet to load (IPA_IP_v4 / IPA_IP_v6)
	@param [in] pTableName - pointer to the Table's Name.
	@param [in] eRuleDestination - destination of the bypass rule.
	@param [in] uHeaderHandle -
	handle to the Header that should be Added (0 should be used as default).
	@param [out] pTableHdl -
	pointer to the table Handle (Can be Null)
	@return boolean indicating whether
	the operation completed successfully or not.

	@details
	This function creates bypass rule within a table in the Routing block.
	*/
bool CreateBypassRoutingTable_v2(
		RoutingDriverWrapper * pRouting,
		enum ipa_ip_type eIP,
		const char *pTableName,
		enum ipa_client_type eRuleDestination,
		uint32_t uHeaderHandle,
		uint32_t *pTableHdl,
		uint8_t uClsAggrIrqMod);

/**
	@brief
		Configures the sytem to one of the pre-determined
		configurations.

		@param [in] testConfiguration - Configuration number
		@param [in] params - additional parameters
	@return void

	@details
		Writes the configuration index to /dev/ipa_test. In case
		the system has already been configured, returns.
*/
void ConfigureScenario(int testConfiguration);
void ConfigureScenario(int testConfiguration, const char *params);
int GenericConfigureScenario(struct ipa_test_config_header *header,
		bool isUlso=false);
int GenericConfigureScenarioDestory(void);
int ConfigureSystem(int testConfiguration, int fd);
int ConfigureSystem(int testConfiguration, int fd, const char *params);

void prepare_channel_struct(struct ipa_channel_config *channel,
		int index,
		enum ipa_client_type client,
		void *cfg,
		size_t config_size,
		bool en_status = 0);

void prepare_header_struct(struct ipa_test_config_header *header,
		struct ipa_channel_config **from,
		struct ipa_channel_config **to);

/**
	@brief
		Compares two data buffers.

		@param [in] goldenBuffer - Pointer to the first data
			buffer
		@param [in] goldenSize - First data buffer size
		@param [in] receivedBuffer - Pointer to the second data
			buffer
		@param [in] receivedSize - Second data buffer size
		@return True - the buffers are identical. False
			otherwise.

		@details
	In case the sizes are differnt, false is returned.
*/
bool CompareResultVsGolden(
		unsigned char *goldenBuffer,
		unsigned int goldenSize,
		unsigned char *receivedBuffer,
		unsigned int receivedSize);

/**
	@brief
		Compares two data buffers considering the returned status.

		@param [in] goldenBuffer - Pointer to the first data
			buffer
		@param [in] goldenSize - First data buffer size
		@param [in] receivedBuffer - Pointer to the second data
			buffer
		@param [in] receivedSize - Second data buffer size
		@return True - the buffers are identical. False
			otherwise.

		@details
	In case the sizes are differnt, false is returned.
*/
bool CompareResultVsGolden_w_Status(
	Byte *goldenBuffer,
	unsigned int goldenSize,
	Byte *receivedBuffer,
	unsigned int receivedSize);


/**
	@brief
		Loads a file to memory

		@param [in] fileFullPath
		@param [inout] sizeLoaded - returns the number of bytes
			which were read from the file
	@return Address of the loaded data buffer

		@details
		Allocates memory by itself, user should free the memory

*/
unsigned char *LoadFileToMemory(
		const string & fileFullPath,
		unsigned int *sizeLoaded);

/**
		@brief
		Checks whether a file exists on disk

		@param [in] filename
	@return True if the file exists, false otherwise.

		@details
*/
bool file_exists(const char *filename);

/**
		@brief
		Prints a data buffer.
		@param [in] data - Pointer to the data
		@param [in] size - How many bytes to print
	@return void

		@details
*/
void print_buff(void *data, size_t size);

void add_buff(uint8_t *data, size_t size, uint8_t val);

/**
		@brief
		Performes ep control of a specific endpoint.
		@param [in] ep_ctrl - Pointer to ipa_test_ep_ctrl struct with
					the data of the requested operation
	@return bool

		@details
		Suspend\Unsuspends\Delays\resumes an endpoint.
*/
bool configure_ep_ctrl(struct ipa_test_ep_ctrl *ep_ctrl);

/**
		@brief
		Performes holb config of a specific pipe.
 *  	@param [in] test_holb_config - Pointer to
 *  				ipa_test_holb_config struct with the data of
 *  				the requested operation
	@return bool

		@details
		Configures HOLB parameters on a pipe.
*/
bool configure_holb(struct ipa_test_holb_config *test_holb_config);

/**
		@brief
		Register an alternative suspend handler
		@param [in] deferred_flag - should the handler execute in defer mode
		@param [in] reg - register or unregister the suspend handler
		@param [in] DevNum - the index of the ep that the handler is registered to
	@return bool

		@details
		Register the test framework suspend handler for a given endpoint
*/
bool RegSuspendHandler(bool deferred_flag, bool reg, int DevNum);

class Eth2Helper {
public:
	static const Byte m_ETH2_IP4_HDR[ETH_HLEN];

	static bool LoadEth2IP4Header(
		uint8_t *pBuffer,
		size_t bufferSize,
		size_t *pLen);

	static bool LoadEth2IP6Header(
		uint8_t *pBuffer,
		size_t bufferSize,
		size_t *pLen);

	static bool LoadEth2IP4Packet(
		uint8_t *pBuffer,
		size_t bufferSize,
		size_t *pLen);

	static bool LoadEth2IP6Packet(
		uint8_t *pBuffer,
		size_t bufferSize,
		size_t *pLen);
};

class WlanHelper {
public:
	static const Byte m_WLAN_HDR[WLAN_HDR_SIZE];

	static bool LoadWlanHeader(
		uint8_t *pBuffer,
		size_t bufferSize,
		size_t *pLen);

	static bool LoadWlanEth2IP4Header(
		uint8_t *pBuffer,
		size_t bufferSize,
		size_t *pLen);

	static bool LoadWlanEth2IP6Header(
		uint8_t *pBuffer,
		size_t bufferSize,
		size_t *pLen);

	static bool LoadWlanEth2IP4Packet(
		uint8_t *pBuffer,
		size_t bufferSize,
		size_t *pLen);

	static bool LoadWlanEth2IP4PacketByLength(
		uint8_t *pBuffer,
		size_t bufferSize,
		size_t len,
		uint8_t padValue);

	static bool LoadWlanEth2IP6Packet(
		uint8_t *pBuffer,
		size_t bufferSize,
		size_t *pLen);
};

#pragma pack(push)  /* push current alignment to stack */
#pragma pack(1)     /* set alignment to 1 byte boundary */
struct RndisHeader {
	uint32_t MessageType;
	uint32_t MessageLength;
	uint32_t DataOffset;
	uint32_t DataLength;
	uint32_t OOBDataOffset;
	uint32_t OOBDataLength;
	uint32_t OOBNumber;
	uint32_t PacketInfoOffset;
	uint32_t PacketInfoLength;
	uint64_t Reserved;
};

struct RndisEtherHeader {
	struct RndisHeader rndisHeader;
	struct ethhdr etherHeader;
};
#pragma pack(pop)   /* restore original alignment from stack */

class RNDISAggregationHelper {
public:
	static const size_t RNDIS_AGGREGATION_BYTE_LIMIT = 1024;

	static bool LoadRNDISHeader(
		uint8_t *pBuffer,
		size_t bufferSize,
		uint32_t messageLength,
		size_t *pLen);

	static bool LoadRNDISEth2IP4Header(
		uint8_t *pBuffer,
		size_t bufferSize,
		uint32_t messageLength,
		size_t *pLen);

	static bool LoadRNDISPacket(
		enum ipa_ip_type eIP,
		uint8_t *pBuffer,
		size_t &nMaxSize);

	static bool LoadEtherPacket(
		enum ipa_ip_type eIP,
		uint8_t *pBuffer,
		size_t &nMaxSize);

	static bool ComparePackets(
		Byte *pPacket1,
		int pPacket1Size,
		Byte *pPacket2,
		int pPacket2Size);

	static bool CompareEthervsRNDISPacket(
		Byte *pIPPacket,
		size_t ipPacketSize,
		Byte *pRNDISPacket,
		size_t rndisPacketSize);

	static bool CompareIPvsRNDISPacket(
		Byte *pIPPacket,
		int ipPacketSize,
		Byte *pRNDISPacket,
		size_t rndisPacketSize);
};

enum ipa_nat_en_type {
	IPA_BYPASS_NAT,
	IPA_SRC_NAT,
	IPA_DST_NAT,
};

enum ipa_ipv6ct_en_type {
	IPA_BYPASS_IPV6CT,
	IPA_ENABLE_IPV6CT,
};

enum ipa_mode_type {
	IPA_BASIC,
	IPA_ENABLE_FRAMING_HDLC,
	IPA_ENABLE_DEFRAMING_HDLC,
	IPA_DMA,
};

enum ipa_aggr_en_type {
	IPA_BYPASS_AGGR,
	IPA_ENABLE_AGGR,
	IPA_ENABLE_DEAGGR,
};

enum ipa_aggr_type {
	IPA_MBIM_16 = 0,
	IPA_HDLC    = 1,
	IPA_TLP     = 2,
	IPA_RNDIS   = 3,
	IPA_GENERIC = 4,
	IPA_QCMAP   = 6,
};

enum ipa_aggr_mode {
	IPA_MBIM_AGGR,
	IPA_QCNCM_AGGR,
};

enum hdr_total_len_or_pad_type {
	IPA_HDR_PAD = 0,
	IPA_HDR_TOTAL_LEN = 1,
};

struct ipa_ep_cfg_nat {
	enum ipa_nat_en_type nat_en;
};

struct ipa_ep_cfg_conn_track {
	enum ipa_ipv6ct_en_type conn_track_en;
};

struct ipa_ep_cfg_hdr {
	uint32_t hdr_len;
	uint32_t hdr_ofst_metadata_valid;
	uint32_t hdr_ofst_metadata;
	uint32_t hdr_additional_const_len;
	uint32_t hdr_ofst_pkt_size_valid;
	uint32_t hdr_ofst_pkt_size;
	uint32_t hdr_a5_mux;
	uint32_t hdr_remove_additional;
	uint32_t hdr_metadata_reg_valid;
};

struct ipa_ep_cfg_hdr_ext {
	uint32_t hdr_pad_to_alignment;
	uint32_t hdr_total_len_or_pad_offset;
	bool hdr_payload_len_inc_padding;
	enum hdr_total_len_or_pad_type hdr_total_len_or_pad;
	bool hdr_total_len_or_pad_valid;
	bool hdr_little_endian;
	struct ipa_ep_cfg_hdr *hdr;
	bool hdr_bytes_to_remove_valid;
	uint32_t hdr_bytes_to_remove;
};

struct ipa_ep_cfg_mode {
	enum ipa_mode_type mode;
	enum ipa_client_type dst;
};

struct ipa_ep_cfg_aggr {
	enum ipa_aggr_en_type aggr_en;
	enum ipa_aggr_type aggr;
	uint32_t aggr_byte_limit;
	uint32_t aggr_time_limit;
	uint32_t aggr_pkt_limit;
	uint32_t aggr_hard_byte_limit_en;
	bool aggr_sw_eof_active;
	uint8_t pulse_generator;
	uint8_t scaled_time;
};

struct ipa_ep_cfg_route {
	uint32_t rt_tbl_hdl;
};

struct ipa_ep_cfg_deaggr {
	uint32_t deaggr_hdr_len;
	bool syspipe_err_detection;
	bool packet_offset_valid;
	uint32_t packet_offset_location;
	bool ignore_min_pkt_err;
	uint32_t max_packet_len;
};

enum ipa_cs_offload {
	IPA_DISABLE_CS_OFFLOAD,
	IPA_ENABLE_CS_OFFLOAD_UL,
	IPA_ENABLE_CS_OFFLOAD_DL,
	IPA_CS_RSVD
};

struct ipa_ep_cfg_cfg {
	bool frag_offload_en;
	enum ipa_cs_offload cs_offload_en;
	uint8_t cs_metadata_hdr_offset;
	uint8_t gen_qmb_master_sel;
	uint8_t tx_instance;
};

struct ipa_ep_cfg_metadata_mask {
	uint32_t metadata_mask;
};

struct ipa_ep_cfg_metadata {
	uint32_t qmap_id;
};

struct ipa_ep_cfg_seq {
	bool set_dynamic;
	int seq_type;
};

struct ipa_ep_cfg_ulso {
	int ipid_min_max_idx;
	bool is_ulso_pipe;
};

struct ipa_ep_cfg_holb {
	uint32_t tmr_val;
	uint32_t base_val;
	uint32_t scale;
	uint16_t en;
	uint8_t pulse_generator;
	uint8_t scaled_time;
};

struct ipa_pkt_init_ex_hdr_ofst_set {
	char name[IPA_RESOURCE_NAME_MAX];
	enum ipa_client_type ep;
};

/*
 * This struct is a mirroring of the ipa struct
 * the test module expect to get from user-space the
 * exact same struct as IPA driver defined.
 * In case of any change to IPA driver struct
 * this struct should be updated as well!
 */
struct test_ipa_ep_cfg {
	struct ipa_ep_cfg_nat nat;
	struct ipa_ep_cfg_conn_track conn_track;
	struct ipa_ep_cfg_hdr hdr;
	struct ipa_ep_cfg_hdr_ext hdr_ext;
	struct ipa_ep_cfg_mode mode;
	struct ipa_ep_cfg_aggr aggr;
	struct ipa_ep_cfg_deaggr deaggr;
	struct ipa_ep_cfg_route route;
	struct ipa_ep_cfg_cfg cfg;
	struct ipa_ep_cfg_metadata_mask metadata_mask;
	struct ipa_ep_cfg_metadata meta;
	struct ipa_ep_cfg_seq seq;
	struct ipa_ep_cfg_ulso ulso;
};

/*! @brief Struct for the IPAv3.0 UL packet status header */
struct ipa3_hw_pkt_status {
	uint64_t status_opcode:8;
	uint64_t exception:8;
	uint64_t status_mask:16;
	uint64_t pkt_len:16;
	uint64_t endp_src_idx:5;
	uint64_t reserved_1:3;
	uint64_t endp_dest_idx:5;
	uint64_t reserved_2:3;
	uint64_t metadata:32;
	uint64_t filt_local:1;
	uint64_t filt_hash:1;
	uint64_t filt_global:1;
	uint64_t ret_hdr:1;
	uint64_t filt_rule_id:10;
	uint64_t route_local:1;
	uint64_t route_hash:1;
	uint64_t ucp:1;
	uint64_t route_tbl_idx:5;
	uint64_t route_rule_id:10;
	uint64_t nat_hit:1;
	uint64_t nat_tbl_idx:13;
	uint64_t nat_type:2;
	uint64_t tag:48;
	uint64_t seq_num:8;
	uint64_t time_day_ctr:24;
	uint64_t hdr_local:1;
	uint64_t hdr_offset:10;
	uint64_t frag_hit:1;
	uint64_t frag_rule:4;
	uint64_t reserved_4:16;
};

struct ipa3_hw_pkt_status_hw_v5_0 {
	uint64_t status_opcode : 8;
	uint64_t exception : 8;
	uint64_t status_mask : 16;
	uint64_t pkt_len : 16;
	uint64_t endp_src_idx : 8;
	uint64_t reserved_1 : 3;
	uint64_t route_local : 1;
	uint64_t route_hash : 1;
	uint64_t reserved_2 : 3;
	uint64_t metadata : 32;
	uint64_t filt_local : 1;
	uint64_t filt_hash : 1;
	uint64_t filt_global : 1;
	uint64_t ret_hdr : 1;
	uint64_t filt_rule_id : 10;
	uint64_t route_tbl_idx : 8;
	uint64_t route_rule_id : 10;
	uint64_t nat_hit : 1;
	uint64_t nat_tbl_idx : 13;
	uint64_t nat_type : 2;
	uint64_t tag : 48;
	uint64_t seq_num : 8;
	uint64_t time_day_ctr : 24;
	uint64_t hdr_local : 1;
	uint64_t hdr_offset : 10;
	uint64_t frag_hit : 1;
	uint64_t frag_rule : 4;
	uint64_t endp_dest_idx : 8;
	uint64_t reserved_4 : 7;
	uint64_t ucp : 1;
};

static inline uint8_t* Addr_AsStr(
	void*       addr,
	uint32_t    addr_len,
	void*       buf_ptr,
	uint32_t    buf_size )
{
	uint8_t* addr_ptr = (uint8_t*) addr;

	if ( addr_ptr && addr_len && buf_ptr && (addr_len*3) <= buf_size )
	{
		uint32_t i, len = 0;

		for ( i = 0; i < addr_len; i++ )
		{
			len += snprintf(
				(char*) buf_ptr + len,
				buf_size - len,
				(i == 0) ? "%02X" : ":%02X",
				addr_ptr[i]);
		}
	}

	return (uint8_t*) buf_ptr;
}

/*
 * Ethernet frame printing support...
 *
 * Ethernet constituent parts and header definition...
 */
struct Vlan_Tag_s
{
	uint16_t vid : 12;
	uint16_t dei :  1;
	uint16_t pcp :  3;
} __attribute__((packed));

typedef Vlan_Tag_s Vlan_Tag_t;

struct Eth_Hdr_s
{
	uint8_t  src_addr[6];
	uint8_t  dst_addr[6];
	uint16_t ethtype1;
	uint16_t vlan;
	uint16_t ethtype2;
} __attribute__((packed));

typedef Eth_Hdr_s Eth_Hdr_t;

static inline uint8_t* Eth_Hdr_AsStr(
	void*       data_ptr,
	void*       buf_ptr,
	uint32_t    buf_size )
{
	Eth_Hdr_t* hdr_ptr = (Eth_Hdr_t*) data_ptr;

	if ( hdr_ptr && buf_ptr && buf_size )
	{
		uint16_t e1;

		uint8_t addr1[64];
		uint8_t addr2[64];

		Addr_AsStr(hdr_ptr->src_addr, sizeof(hdr_ptr->src_addr), addr1, sizeof(addr1));
		Addr_AsStr(hdr_ptr->dst_addr, sizeof(hdr_ptr->dst_addr), addr2, sizeof(addr2));

		e1 = ntohs(hdr_ptr->ethtype1);

		if ( e1 == 0x8100 ) /* 802 1Q ie. vlan */
		{
			uint16_t v = ntohs(hdr_ptr->vlan);

			uint16_t e2 = ntohs(hdr_ptr->ethtype2);

			Vlan_Tag_t* vt = (Vlan_Tag_t*) &v;

			snprintf(
				(char*) buf_ptr,
				buf_size,
				"src_addr: %s "
				"dst_addr: %s "
				"vlan ethtype: 0x%04X "
				"pcp: 0x%02X "
				"dei: 0x%02X "
				"vlan: 0x%04X "
				"ethtype: 0x%04X",
				addr1,
				addr2,
				e1,
				vt->pcp,
				vt->dei,
				vt->vid,
				e2);
		}
		else
		{
			snprintf(
				(char*) buf_ptr,
				buf_size,
				"src_addr: %s "
				"dst_addr: %s "
				"ethtype: 0x%04X",
				addr1,
				addr2,
				e1);
		}
	}

	return (uint8_t*) buf_ptr;
}

#define mask(bits) \
	( ((bits) == 32) ? 0xffffffff : ((1 << (bits)) - 1) )

#define pull(word, bits, shift) \
	( (ntohl((word)) >> (shift)) & mask(bits) )

#define push(val, word, bits, shift) \
	( (word) |= (htonl(((val) & mask(bits)) << (shift))) )

/*
 * IPv4 frame printing support...
 *
 * IPv4 constituent parts and header definition...
 *
 * For field value getting
 */
#define getv4_vers(hdr) \
	pull(hdr->w[0], 4, 28)

#define getv4_ihl(hdr) \
	pull(hdr->w[0], 4, 24)

#define getv4_dscp(hdr) \
	pull(hdr->w[0], 6, 18)

#define getv4_ecn(hdr) \
	pull(hdr->w[0], 2, 16)

#define getv4_tot_len(hdr) \
	pull(hdr->w[0], 16, 0)

#define getv4_id(hdr) \
	pull(hdr->w[1], 16, 16)

#define getv4_flags(hdr) \
	pull(hdr->w[1], 3, 13)

#define getv4_frag_off(hdr) \
	pull(hdr->w[1], 13, 0)

#define getv4_ttl(hdr) \
	pull(hdr->w[2], 8, 24)

#define getv4_protocol(hdr) \
	pull(hdr->w[2], 8, 16)

#define getv4_cksum(hdr) \
	pull(hdr->w[2], 16, 0)

/*
 * For field value setting
 */
#define setv4_dscp(hdr, val) \
	push(val, hdr->w[0], 6, 18)

#define setv4_cksum(hdr, val) \
	push(val, hdr->w[2], 16, 0)

typedef struct
{
	uint32_t w[3];
	uint32_t src_ip;
	uint32_t dst_ip;
	uint8_t  payload[0];
} IPv4_Hdr_t;

static inline uint8_t* IPv4_Hdr_AsStr(
	void*       data_ptr,
	void*       buf_ptr,
	uint32_t    buf_size )
{
	IPv4_Hdr_t* hdr_ptr = (IPv4_Hdr_t*) data_ptr;

	if ( hdr_ptr && buf_ptr && buf_size )
	{
		snprintf(
			(char*) buf_ptr,
			buf_size,
			"vers: 0x%02X "
			"ihl: 0x%02X "
			"dscp: 0x%02X "
			"ecn: 0x%02X "
			"tot_len: 0x%04X "
			"id: 0x%04X "
			"flags: 0x%02X "
			"frag_off: 0x%04X "
			"ttl: 0x%02X "
			"protocol: 0x%02X "
			"cksum: 0x%04X "
			"src_ip: 0x%08X "
			"dst_ip: 0x%08X",
			getv4_vers(hdr_ptr),
			getv4_ihl(hdr_ptr),
			getv4_dscp(hdr_ptr),
			getv4_ecn(hdr_ptr),
			getv4_tot_len(hdr_ptr),
			getv4_id(hdr_ptr),
			getv4_flags(hdr_ptr),
			getv4_frag_off(hdr_ptr),
			getv4_ttl(hdr_ptr),
			getv4_protocol(hdr_ptr),
			getv4_cksum(hdr_ptr),
			hdr_ptr->src_ip,
			hdr_ptr->dst_ip);
	}

	return (uint8_t*) buf_ptr;
}

/*
 * IPv6 frame printing support...
 *
 * IPv6 constituent parts and header definition...
 *
 *
 * For field value getting
 */
#define getv6_vers(hdr) \
	pull(hdr->w[0], 4, 28)

#define getv6_trafClass(hdr) \
	pull(hdr->w[0], 8, 20)

#define getv6_flowLab(hdr) \
	pull(hdr->w[0], 20, 0)

#define getv6_payLen(hdr) \
	pull(hdr->w[1], 16, 16)

#define getv6_nxtHdr(hdr) \
	pull(hdr->w[1], 8, 8)

#define getv6_hopLim(hdr) \
	pull(hdr->w[1], 8, 0)

/*
 * For field value setting
 */
#define setv6_vers(hdr, val) \
	push(val, hdr->w[0], 4, 28)

#define setv6_trafClass(hdr, val) \
	push(val, hdr->w[0], 8, 20)

#define setv6_dscp(hdr, val) \
	setv6_trafClass(hdr, val<<2)

#define setv6_flowLab(hdr, val) \
	push(val, hdr->w[0], 20, 0)

#define setv6_payLen(hdr, val) \
	push(val, hdr->w[1], 16, 16)

#define setv6_nxtHdr(hdr, val) \
	push(val, hdr->w[1], 8, 8)

#define setv6_hopLim(hdr, val) \
	push(val, hdr->w[1], 8, 0)

struct IPv6_Hdr_s
{
	uint32_t w[2];
	uint8_t  saddr[16];
	uint8_t  daddr[16];
	uint8_t  payload[0];
} __attribute__((packed));

typedef struct IPv6_Hdr_s IPv6_Hdr_t;

static inline uint8_t* IPv6_Hdr_AsStr(
	void*       data_ptr,
	void*       buf_ptr,
	uint32_t    buf_size )
{
	IPv6_Hdr_t* hdr_ptr = (IPv6_Hdr_t*) data_ptr;

	if ( hdr_ptr && buf_ptr && buf_size )
	{
		uint8_t addr1[64];
		uint8_t addr2[64];

		Addr_AsStr(hdr_ptr->saddr, sizeof(hdr_ptr->saddr), addr1, sizeof(addr1));
		Addr_AsStr(hdr_ptr->daddr, sizeof(hdr_ptr->daddr), addr2, sizeof(addr2));

		snprintf(
			(char*) buf_ptr,
			buf_size,
			"vers: 0x%02X "
			"traffic_class: 0x%02X "
			"flow_label: 0x%06X "
			"payload_len: 0x%04X "
			"next_hdr: 0x%02X "
			"hop_limit: 0x%02X "
			"src_ip: %s "
			"dst_ip: %s",
			getv6_vers(hdr_ptr),
			getv6_trafClass(hdr_ptr),
			getv6_flowLab(hdr_ptr),
			getv6_payLen(hdr_ptr),
			getv6_nxtHdr(hdr_ptr),
			getv6_hopLim(hdr_ptr),
			addr1,
			addr2);
	}

	return (uint8_t*) buf_ptr;
}

union ipa_ip_params
{
	struct {
		uint8_t tos;
		uint8_t protocol;
		uint32_t src_addr;
		uint32_t src_addr_mask;
		uint32_t dst_addr;
		uint32_t dst_addr_mask;
	} v4;
	struct {
		uint8_t tc;
		uint32_t flow_label;
		uint8_t next_hdr;
		uint32_t src_addr[4];
		uint32_t src_addr_mask[4];
		uint32_t dst_addr[4];
		uint32_t dst_addr_mask[4];
	} v6;
};

#endif
