#ifndef __ozb_mac_mcps_h__
#define __ozb_mac_mcps_h__

#include <mac/ozb_mac_types.h>

COMPILER_INLINE 
uint8_t ozb_mac_set_tx_options(uint8_t ack, uint8_t use_gts, uint8_t indirect)
{
	return (ack & 0x1) | ((use_gts & 0x1) << 1) | ((indirect & 0x1) << 2);
}

#define OZB_MAC_TX_OPTION_ACK(opt)	((opt) & 0x01)
#define OZB_MAC_TX_OPTION_GTS(opt)	(((opt) >> 1) & 0x01)
#define OZB_MAC_TX_OPTION_INDIRECT(opt)	(((opt) >> 2) & 0x01)

int8_t ozb_MCPS_DATA_request(uint8_t SrcAddrMode, uint8_t DstAddrMode,
			     uint16_t DstPANId, void *DstAddr,
			     uint8_t msduLength, uint8_t *msdu,
			     uint8_t msduHandle, uint8_t TxOptions,
			     uint8_t SecurityLevel, uint8_t KeyIdMode,
			     uint8_t *KeySource, uint8_t KeyIndex);

int8_t ozb_MCPS_DATA_confirm(uint8_t msduHandle, enum ozb_mac_code_t status,
			     uint32_t Timestamp);

int8_t ozb_MCPS_DATA_indication(uint8_t SrcAddrMode, uint16_t SrcPANId,
				ozb_mac_dev_addr_t SrcAddr, uint8_t DstAddrMode,
				uint16_t DstPANId, ozb_mac_dev_addr_t DstAddr,
				uint8_t msduLength, uint8_t *msdu,
				uint8_t mpduLinkQuality, uint8_t DSN,
				uint32_t Timestamp, uint8_t SecurityLevel,
				uint8_t KeyIdMode, uint8_t *KeySource,
				uint8_t KeyIndex);

#ifdef OZB_RFD_DISABLE_OPTIONAL
int8_t ozb_MCPS_PURGE_request(uint8_t msduHandle);

int8_t ozb_MCPS_PURGE_confirm(uint8_t msduHandle, enum ozb_mac_code_t status);
#endif /* OZB_RFD_NO_OPTIONAL */

#endif /* Header Protection */
