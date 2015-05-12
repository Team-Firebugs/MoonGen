/*******************************************************************************

Copyright (c) 2001-2014, Intel Corporation
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

 1. Redistributions of source code must retain the above copyright notice,
    this list of conditions and the following disclaimer.

 2. Redistributions in binary form must reproduce the above copyright
    notice, this list of conditions and the following disclaimer in the
    documentation and/or other materials provided with the distribution.

 3. Neither the name of the Intel Corporation nor the names of its
    contributors may be used to endorse or promote products derived from
    this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
POSSIBILITY OF SUCH DAMAGE.

***************************************************************************/

#include "ixgbe_x550.h"
#include "ixgbe_x540.h"
#include "ixgbe_type.h"
#include "ixgbe_api.h"
#include "ixgbe_common.h"
#include "ixgbe_phy.h"

/**
 *  ixgbe_init_ops_X550 - Inits func ptrs and MAC type
 *  @hw: pointer to hardware structure
 *
 *  Initialize the function pointers and assign the MAC type for X550.
 *  Does not touch the hardware.
 **/
s32 ixgbe_init_ops_X550(struct ixgbe_hw *hw)
{
	struct ixgbe_mac_info *mac = &hw->mac;
	struct ixgbe_eeprom_info *eeprom = &hw->eeprom;
	s32 ret_val;

	DEBUGFUNC("ixgbe_init_ops_X550");

	ret_val = ixgbe_init_ops_X540(hw);
	mac->ops.dmac_config = &ixgbe_dmac_config_X550;
	mac->ops.dmac_config_tcs = &ixgbe_dmac_config_tcs_X550;
	mac->ops.dmac_update_tcs = &ixgbe_dmac_update_tcs_X550;
	mac->ops.setup_eee = &ixgbe_setup_eee_X550;
	mac->ops.set_source_address_pruning =
			&ixgbe_set_source_address_pruning_X550;
	mac->ops.set_ethertype_anti_spoofing =
			&ixgbe_set_ethertype_anti_spoofing_X550;

	mac->ops.get_rtrup2tc = &ixgbe_dcb_get_rtrup2tc_generic;
	eeprom->ops.init_params = &ixgbe_init_eeprom_params_X550;
	eeprom->ops.calc_checksum = &ixgbe_calc_eeprom_checksum_X550;
	eeprom->ops.read = &ixgbe_read_ee_hostif_X550;
	eeprom->ops.read_buffer = &ixgbe_read_ee_hostif_buffer_X550;
	eeprom->ops.write = &ixgbe_write_ee_hostif_X550;
	eeprom->ops.write_buffer = &ixgbe_write_ee_hostif_buffer_X550;
	eeprom->ops.update_checksum = &ixgbe_update_eeprom_checksum_X550;
	eeprom->ops.validate_checksum = &ixgbe_validate_eeprom_checksum_X550;

	mac->ops.disable_mdd = &ixgbe_disable_mdd_X550;
	mac->ops.enable_mdd = &ixgbe_enable_mdd_X550;
	mac->ops.mdd_event = &ixgbe_mdd_event_X550;
	mac->ops.restore_mdd_vf = &ixgbe_restore_mdd_vf_X550;
	mac->ops.disable_rx = &ixgbe_disable_rx_x550;
	return ret_val;
}

/**
 * ixgbe_identify_phy_x550em - Get PHY type based on device id
 * @hw: pointer to hardware structure
 *
 * Returns error code
 */
STATIC s32 ixgbe_identify_phy_x550em(struct ixgbe_hw *hw)
{
	u32 esdp = IXGBE_READ_REG(hw, IXGBE_ESDP);

	switch (hw->device_id) {
	case IXGBE_DEV_ID_X550EM_X_SFP:
		/* set up for CS4227 usage */
		hw->phy.lan_id = IXGBE_READ_REG(hw, IXGBE_STATUS) &
				 IXGBE_STATUS_LAN_ID_1;
		hw->phy.phy_semaphore_mask = IXGBE_GSSR_SHARED_I2C_SM;
		if (hw->phy.lan_id) {

			esdp &= ~(IXGBE_ESDP_SDP1_NATIVE | IXGBE_ESDP_SDP1);
			esdp |= IXGBE_ESDP_SDP1_DIR;
		}
		esdp &= ~(IXGBE_ESDP_SDP0_NATIVE | IXGBE_ESDP_SDP0_DIR);
		IXGBE_WRITE_REG(hw, IXGBE_ESDP, esdp);

		return ixgbe_identify_module_generic(hw);
		break;
	case IXGBE_DEV_ID_X550EM_X_KX4:
		hw->phy.type = ixgbe_phy_x550em_kx4;
		break;
	case IXGBE_DEV_ID_X550EM_X_KR:
	case IXGBE_DEV_ID_X550EM_X:
		hw->phy.type = ixgbe_phy_x550em_kr;
		break;
	default:
		break;
	}
	return IXGBE_SUCCESS;
}

STATIC s32 ixgbe_read_phy_reg_x550em(struct ixgbe_hw *hw, u32 reg_addr,
				     u32 device_type, u16 *phy_data)
{
	UNREFERENCED_4PARAMETER(*hw, reg_addr, device_type, *phy_data);
	return IXGBE_NOT_IMPLEMENTED;
}

STATIC s32 ixgbe_write_phy_reg_x550em(struct ixgbe_hw *hw, u32 reg_addr,
				      u32 device_type, u16 phy_data)
{
	UNREFERENCED_4PARAMETER(*hw, reg_addr, device_type, phy_data);
	return IXGBE_NOT_IMPLEMENTED;
}

/**
*  ixgbe_init_ops_X550EM - Inits func ptrs and MAC type
*  @hw: pointer to hardware structure
*
*  Initialize the function pointers and for MAC type X550EM.
*  Does not touch the hardware.
**/
s32 ixgbe_init_ops_X550EM(struct ixgbe_hw *hw)
{
	struct ixgbe_mac_info *mac = &hw->mac;
	struct ixgbe_eeprom_info *eeprom = &hw->eeprom;
	struct ixgbe_phy_info *phy = &hw->phy;
	s32 ret_val;

	DEBUGFUNC("ixgbe_init_ops_X550EM");

	/* Similar to X550 so start there. */
	ret_val = ixgbe_init_ops_X550(hw);

	/* Since this function eventually calls
	 * ixgbe_init_ops_540 by design, we are setting
	 * the pointers to NULL explicitly here to overwrite
	 * the values being set in the x540 function.
	 */
	/* Thermal sensor not supported in x550EM */
	mac->ops.get_thermal_sensor_data = NULL;
	mac->ops.init_thermal_sensor_thresh = NULL;
	mac->thermal_sensor_enabled = false;

	/* FCOE not supported in x550EM */
	mac->ops.get_san_mac_addr = NULL;
	mac->ops.set_san_mac_addr = NULL;
	mac->ops.get_wwn_prefix = NULL;
	mac->ops.get_fcoe_boot_status = NULL;

	/* IPsec not supported in x550EM */
	mac->ops.disable_sec_rx_path = NULL;
	mac->ops.enable_sec_rx_path = NULL;

	/* PCIe bus info not supported in X550EM */
	mac->ops.get_bus_info = NULL;

	mac->ops.read_iosf_sb_reg = ixgbe_read_iosf_sb_reg_x550;
	mac->ops.write_iosf_sb_reg = ixgbe_write_iosf_sb_reg_x550;
	mac->ops.get_media_type = &ixgbe_get_media_type_X550em;
	mac->ops.setup_sfp = &ixgbe_setup_sfp_modules_X550em;
	mac->ops.get_link_capabilities = &ixgbe_get_link_capabilities_X550em;
	mac->ops.reset_hw = &ixgbe_reset_hw_X550em;
	mac->ops.get_supported_physical_layer =
				    &ixgbe_get_supported_physical_layer_X550em;

	/* PHY */
	phy->ops.init = &ixgbe_init_phy_ops_X550em;
	phy->ops.identify = &ixgbe_identify_phy_x550em;
	phy->ops.read_reg = ixgbe_read_phy_reg_x550em;
	phy->ops.write_reg = ixgbe_write_phy_reg_x550em;
	phy->ops.setup_link = ixgbe_setup_kr_x550em;


	/* EEPROM */
	eeprom->ops.init_params = &ixgbe_init_eeprom_params_X540;
	eeprom->ops.read = &ixgbe_read_ee_hostif_X550;
	eeprom->ops.read_buffer = &ixgbe_read_ee_hostif_buffer_X550;
	eeprom->ops.write = &ixgbe_write_ee_hostif_X550;
	eeprom->ops.write_buffer = &ixgbe_write_ee_hostif_buffer_X550;
	eeprom->ops.update_checksum = &ixgbe_update_eeprom_checksum_X550;
	eeprom->ops.validate_checksum = &ixgbe_validate_eeprom_checksum_X550;
	eeprom->ops.calc_checksum = &ixgbe_calc_eeprom_checksum_X550;

	return ret_val;
}

/**
 *  ixgbe_dmac_config_X550
 *  @hw: pointer to hardware structure
 *
 *  Configure DMA coalescing. If enabling dmac, dmac is activated.
 *  When disabling dmac, dmac enable dmac bit is cleared.
 **/
s32 ixgbe_dmac_config_X550(struct ixgbe_hw *hw)
{
	u32 reg, high_pri_tc;

	DEBUGFUNC("ixgbe_dmac_config_X550");

	/* Disable DMA coalescing before configuring */
	reg = IXGBE_READ_REG(hw, IXGBE_DMACR);
	reg &= ~IXGBE_DMACR_DMAC_EN;
	IXGBE_WRITE_REG(hw, IXGBE_DMACR, reg);

	/* Disable DMA Coalescing if the watchdog timer is 0 */
	if (!hw->mac.dmac_config.watchdog_timer)
		goto out;

	ixgbe_dmac_config_tcs_X550(hw);

	/* Configure DMA Coalescing Control Register */
	reg = IXGBE_READ_REG(hw, IXGBE_DMACR);

	/* Set the watchdog timer in units of 40.96 usec */
	reg &= ~IXGBE_DMACR_DMACWT_MASK;
	reg |= (hw->mac.dmac_config.watchdog_timer * 100) / 4096;

	reg &= ~IXGBE_DMACR_HIGH_PRI_TC_MASK;
	/* If fcoe is enabled, set high priority traffic class */
	if (hw->mac.dmac_config.fcoe_en) {
		high_pri_tc = 1 << hw->mac.dmac_config.fcoe_tc;
		reg |= ((high_pri_tc << IXGBE_DMACR_HIGH_PRI_TC_SHIFT) &
			IXGBE_DMACR_HIGH_PRI_TC_MASK);
	}
	reg |= IXGBE_DMACR_EN_MNG_IND;

	/* Enable DMA coalescing after configuration */
	reg |= IXGBE_DMACR_DMAC_EN;
	IXGBE_WRITE_REG(hw, IXGBE_DMACR, reg);

out:
	return IXGBE_SUCCESS;
}

/**
 *  ixgbe_dmac_config_tcs_X550
 *  @hw: pointer to hardware structure
 *
 *  Configure DMA coalescing threshold per TC. The dmac enable bit must
 *  be cleared before configuring.
 **/
s32 ixgbe_dmac_config_tcs_X550(struct ixgbe_hw *hw)
{
	u32 tc, reg, pb_headroom, rx_pb_size, maxframe_size_kb;

	DEBUGFUNC("ixgbe_dmac_config_tcs_X550");

	/* Configure DMA coalescing enabled */
	switch (hw->mac.dmac_config.link_speed) {
	case IXGBE_LINK_SPEED_100_FULL:
		pb_headroom = IXGBE_DMACRXT_100M;
		break;
	case IXGBE_LINK_SPEED_1GB_FULL:
		pb_headroom = IXGBE_DMACRXT_1G;
		break;
	default:
		pb_headroom = IXGBE_DMACRXT_10G;
		break;
	}

	maxframe_size_kb = ((IXGBE_READ_REG(hw, IXGBE_MAXFRS) >>
			     IXGBE_MHADD_MFS_SHIFT) / 1024);

	/* Set the per Rx packet buffer receive threshold */
	for (tc = 0; tc < IXGBE_DCB_MAX_TRAFFIC_CLASS; tc++) {
		reg = IXGBE_READ_REG(hw, IXGBE_DMCTH(tc));
		reg &= ~IXGBE_DMCTH_DMACRXT_MASK;

		if (tc < hw->mac.dmac_config.num_tcs) {
			/* Get Rx PB size */
			rx_pb_size = IXGBE_READ_REG(hw, IXGBE_RXPBSIZE(tc));
			rx_pb_size = (rx_pb_size & IXGBE_RXPBSIZE_MASK) >>
				IXGBE_RXPBSIZE_SHIFT;

			/* Calculate receive buffer threshold in kilobytes */
			if (rx_pb_size > pb_headroom)
				rx_pb_size = rx_pb_size - pb_headroom;
			else
				rx_pb_size = 0;

			/* Minimum of MFS shall be set for DMCTH */
			reg |= (rx_pb_size > maxframe_size_kb) ?
				rx_pb_size : maxframe_size_kb;
		}
		IXGBE_WRITE_REG(hw, IXGBE_DMCTH(tc), reg);
	}
	return IXGBE_SUCCESS;
}

/**
 *  ixgbe_dmac_update_tcs_X550
 *  @hw: pointer to hardware structure
 *
 *  Disables dmac, updates per TC settings, and then enables dmac.
 **/
s32 ixgbe_dmac_update_tcs_X550(struct ixgbe_hw *hw)
{
	u32 reg;

	DEBUGFUNC("ixgbe_dmac_update_tcs_X550");

	/* Disable DMA coalescing before configuring */
	reg = IXGBE_READ_REG(hw, IXGBE_DMACR);
	reg &= ~IXGBE_DMACR_DMAC_EN;
	IXGBE_WRITE_REG(hw, IXGBE_DMACR, reg);

	ixgbe_dmac_config_tcs_X550(hw);

	/* Enable DMA coalescing after configuration */
	reg = IXGBE_READ_REG(hw, IXGBE_DMACR);
	reg |= IXGBE_DMACR_DMAC_EN;
	IXGBE_WRITE_REG(hw, IXGBE_DMACR, reg);

	return IXGBE_SUCCESS;
}

/**
 *  ixgbe_init_eeprom_params_X550 - Initialize EEPROM params
 *  @hw: pointer to hardware structure
 *
 *  Initializes the EEPROM parameters ixgbe_eeprom_info within the
 *  ixgbe_hw struct in order to set up EEPROM access.
 **/
s32 ixgbe_init_eeprom_params_X550(struct ixgbe_hw *hw)
{
	struct ixgbe_eeprom_info *eeprom = &hw->eeprom;
	u32 eec;
	u16 eeprom_size;

	DEBUGFUNC("ixgbe_init_eeprom_params_X550");

	if (eeprom->type == ixgbe_eeprom_uninitialized) {
		eeprom->semaphore_delay = 10;
		eeprom->type = ixgbe_flash;

		eec = IXGBE_READ_REG(hw, IXGBE_EEC);
		eeprom_size = (u16)((eec & IXGBE_EEC_SIZE) >>
				    IXGBE_EEC_SIZE_SHIFT);
		eeprom->word_size = 1 << (eeprom_size +
					  IXGBE_EEPROM_WORD_SIZE_SHIFT);

		DEBUGOUT2("Eeprom params: type = %d, size = %d\n",
			  eeprom->type, eeprom->word_size);
	}

	return IXGBE_SUCCESS;
}

/**
 *  ixgbe_setup_eee_X550 - Enable/disable EEE support
 *  @hw: pointer to the HW structure
 *  @enable_eee: boolean flag to enable EEE
 *
 *  Enable/disable EEE based on enable_eee flag.
 *  Auto-negotiation must be started after BASE-T EEE bits in PHY register 7.3C
 *  are modified.
 *
 **/
s32 ixgbe_setup_eee_X550(struct ixgbe_hw *hw, bool enable_eee)
{
	u32 eeer;
	u16 autoneg_eee_reg;
	u32 link_reg;
	s32 status;

	DEBUGFUNC("ixgbe_setup_eee_X550");

	eeer = IXGBE_READ_REG(hw, IXGBE_EEER);
	/* Enable or disable EEE per flag */
	if (enable_eee) {
		eeer |= (IXGBE_EEER_TX_LPI_EN | IXGBE_EEER_RX_LPI_EN);

		if (hw->device_id == IXGBE_DEV_ID_X550T) {
			/* Advertise EEE capability */
			hw->phy.ops.read_reg(hw, IXGBE_MDIO_AUTO_NEG_EEE_ADVT,
				IXGBE_MDIO_AUTO_NEG_DEV_TYPE, &autoneg_eee_reg);

			autoneg_eee_reg |= (IXGBE_AUTO_NEG_10GBASE_EEE_ADVT |
				IXGBE_AUTO_NEG_1000BASE_EEE_ADVT |
				IXGBE_AUTO_NEG_100BASE_EEE_ADVT);

			hw->phy.ops.write_reg(hw, IXGBE_MDIO_AUTO_NEG_EEE_ADVT,
				IXGBE_MDIO_AUTO_NEG_DEV_TYPE, autoneg_eee_reg);
		} else if (hw->device_id == IXGBE_DEV_ID_X550EM_X_KR ||
			   hw->device_id == IXGBE_DEV_ID_X550EM_X) {
			status = ixgbe_read_iosf_sb_reg_x550(hw,
				IXGBE_KRM_LINK_CTRL_1(hw->phy.lan_id),
				IXGBE_SB_IOSF_TARGET_KR_PHY, &link_reg);
			if (status != IXGBE_SUCCESS)
				return status;

			link_reg |= IXGBE_KRM_LINK_CTRL_1_TETH_EEE_CAP_KR |
				    IXGBE_KRM_LINK_CTRL_1_TETH_EEE_CAP_KX;

			status = ixgbe_write_iosf_sb_reg_x550(hw,
				IXGBE_KRM_LINK_CTRL_1(hw->phy.lan_id),
				IXGBE_SB_IOSF_TARGET_KR_PHY, link_reg);
			if (status != IXGBE_SUCCESS)
				return status;
		}
	} else {
		eeer &= ~(IXGBE_EEER_TX_LPI_EN | IXGBE_EEER_RX_LPI_EN);

		if (hw->device_id == IXGBE_DEV_ID_X550T) {
			/* Disable advertised EEE capability */
			hw->phy.ops.read_reg(hw, IXGBE_MDIO_AUTO_NEG_EEE_ADVT,
				IXGBE_MDIO_AUTO_NEG_DEV_TYPE, &autoneg_eee_reg);

			autoneg_eee_reg &= ~(IXGBE_AUTO_NEG_10GBASE_EEE_ADVT |
				IXGBE_AUTO_NEG_1000BASE_EEE_ADVT |
				IXGBE_AUTO_NEG_100BASE_EEE_ADVT);

			hw->phy.ops.write_reg(hw, IXGBE_MDIO_AUTO_NEG_EEE_ADVT,
				IXGBE_MDIO_AUTO_NEG_DEV_TYPE, autoneg_eee_reg);
		} else if (hw->device_id == IXGBE_DEV_ID_X550EM_X_KR ||
			   hw->device_id == IXGBE_DEV_ID_X550EM_X) {
			status = ixgbe_read_iosf_sb_reg_x550(hw,
				IXGBE_KRM_LINK_CTRL_1(hw->phy.lan_id),
				IXGBE_SB_IOSF_TARGET_KR_PHY, &link_reg);
			if (status != IXGBE_SUCCESS)
				return status;

			link_reg &= ~(IXGBE_KRM_LINK_CTRL_1_TETH_EEE_CAP_KR |
				IXGBE_KRM_LINK_CTRL_1_TETH_EEE_CAP_KX);

			status = ixgbe_write_iosf_sb_reg_x550(hw,
				IXGBE_KRM_LINK_CTRL_1(hw->phy.lan_id),
				IXGBE_SB_IOSF_TARGET_KR_PHY, link_reg);
			if (status != IXGBE_SUCCESS)
				return status;
		}
	}
	IXGBE_WRITE_REG(hw, IXGBE_EEER, eeer);

	return IXGBE_SUCCESS;
}

/**
 * ixgbe_set_source_address_pruning_X550 - Enable/Disbale source address pruning
 * @hw: pointer to hardware structure
 * @enable: enable or disable source address pruning
 * @pool: Rx pool to set source address pruning for
 **/
void ixgbe_set_source_address_pruning_X550(struct ixgbe_hw *hw, bool enable,
					   unsigned int pool)
{
	u64 pfflp;

	/* max rx pool is 63 */
	if (pool > 63)
		return;

	pfflp = (u64)IXGBE_READ_REG(hw, IXGBE_PFFLPL);
	pfflp |= (u64)IXGBE_READ_REG(hw, IXGBE_PFFLPH) << 32;

	if (enable)
		pfflp |= (1ULL << pool);
	else
		pfflp &= ~(1ULL << pool);

	IXGBE_WRITE_REG(hw, IXGBE_PFFLPL, (u32)pfflp);
	IXGBE_WRITE_REG(hw, IXGBE_PFFLPH, (u32)(pfflp >> 32));
}

/**
 *  ixgbe_set_ethertype_anti_spoofing_X550 - Enable/Disable Ethertype anti-spoofing
 *  @hw: pointer to hardware structure
 *  @enable: enable or disable switch for Ethertype anti-spoofing
 *  @vf: Virtual Function pool - VF Pool to set for Ethertype anti-spoofing
 *
 **/
void ixgbe_set_ethertype_anti_spoofing_X550(struct ixgbe_hw *hw,
		bool enable, int vf)
{
	int vf_target_reg = vf >> 3;
	int vf_target_shift = vf % 8 + IXGBE_SPOOF_ETHERTYPEAS_SHIFT;
	u32 pfvfspoof;

	DEBUGFUNC("ixgbe_set_ethertype_anti_spoofing_X550");

	pfvfspoof = IXGBE_READ_REG(hw, IXGBE_PFVFSPOOF(vf_target_reg));
	if (enable)
		pfvfspoof |= (1 << vf_target_shift);
	else
		pfvfspoof &= ~(1 << vf_target_shift);

	IXGBE_WRITE_REG(hw, IXGBE_PFVFSPOOF(vf_target_reg), pfvfspoof);
}

/**
 *  ixgbe_write_iosf_sb_reg_x550 - Writes a value to specified register of the IOSF
 *  device
 *  @hw: pointer to hardware structure
 *  @reg_addr: 32 bit PHY register to write
 *  @device_type: 3 bit device type
 *  @data: Data to write to the register
 **/
s32 ixgbe_write_iosf_sb_reg_x550(struct ixgbe_hw *hw, u32 reg_addr,
			    u32 device_type, u32 data)
{
	u32 i, command, error;

	command = ((reg_addr << IXGBE_SB_IOSF_CTRL_ADDR_SHIFT) |
		   (device_type << IXGBE_SB_IOSF_CTRL_TARGET_SELECT_SHIFT));

	/* Write IOSF control register */
	IXGBE_WRITE_REG(hw, IXGBE_SB_IOSF_INDIRECT_CTRL, command);

	/* Write IOSF data register */
	IXGBE_WRITE_REG(hw, IXGBE_SB_IOSF_INDIRECT_DATA, data);
	/*
	 * Check every 10 usec to see if the address cycle completed.
	 * The SB IOSF BUSY bit will clear when the operation is
	 * complete
	 */
	for (i = 0; i < IXGBE_MDIO_COMMAND_TIMEOUT; i++) {
		usec_delay(10);

		command = IXGBE_READ_REG(hw, IXGBE_SB_IOSF_INDIRECT_CTRL);
		if ((command & IXGBE_SB_IOSF_CTRL_BUSY) == 0)
			break;
	}

	if ((command & IXGBE_SB_IOSF_CTRL_CMPL_ERR_MASK) != 0) {
		error = (command & IXGBE_SB_IOSF_CTRL_CMPL_ERR_MASK) >>
			 IXGBE_SB_IOSF_CTRL_CMPL_ERR_SHIFT;
		ERROR_REPORT2(IXGBE_ERROR_POLLING,
			      "Failed to write, error %x\n", error);
		return IXGBE_ERR_PHY;
	}

	if (i == IXGBE_MDIO_COMMAND_TIMEOUT) {
		ERROR_REPORT1(IXGBE_ERROR_POLLING, "Write timed out\n");
		return IXGBE_ERR_PHY;
	}

	return IXGBE_SUCCESS;
}

/**
 *  ixgbe_read_iosf_sb_reg_x550 - Writes a value to specified register of the IOSF
 *  device
 *  @hw: pointer to hardware structure
 *  @reg_addr: 32 bit PHY register to write
 *  @device_type: 3 bit device type
 *  @phy_data: Pointer to read data from the register
 **/
s32 ixgbe_read_iosf_sb_reg_x550(struct ixgbe_hw *hw, u32 reg_addr,
			   u32 device_type, u32 *data)
{
	u32 i, command, error;

	command = ((reg_addr << IXGBE_SB_IOSF_CTRL_ADDR_SHIFT) |
		   (device_type << IXGBE_SB_IOSF_CTRL_TARGET_SELECT_SHIFT));

	/* Write IOSF control register */
	IXGBE_WRITE_REG(hw, IXGBE_SB_IOSF_INDIRECT_CTRL, command);

	/*
	 * Check every 10 usec to see if the address cycle completed.
	 * The SB IOSF BUSY bit will clear when the operation is
	 * complete
	 */
	for (i = 0; i < IXGBE_MDIO_COMMAND_TIMEOUT; i++) {
		usec_delay(10);

		command = IXGBE_READ_REG(hw, IXGBE_SB_IOSF_INDIRECT_CTRL);
		if ((command & IXGBE_SB_IOSF_CTRL_BUSY) == 0)
			break;
	}

	if ((command & IXGBE_SB_IOSF_CTRL_CMPL_ERR_MASK) != 0) {
		error = (command & IXGBE_SB_IOSF_CTRL_CMPL_ERR_MASK) >>
			 IXGBE_SB_IOSF_CTRL_CMPL_ERR_SHIFT;
		ERROR_REPORT2(IXGBE_ERROR_POLLING,
				"Failed to read, error %x\n", error);
		return IXGBE_ERR_PHY;
	}

	if (i == IXGBE_MDIO_COMMAND_TIMEOUT) {
		ERROR_REPORT1(IXGBE_ERROR_POLLING, "Read timed out\n");
		return IXGBE_ERR_PHY;
	}

	*data = IXGBE_READ_REG(hw, IXGBE_SB_IOSF_INDIRECT_DATA);

	return IXGBE_SUCCESS;
}

/**
 *  ixgbe_disable_mdd_X550
 *  @hw: pointer to hardware structure
 *
 *  Disable malicious driver detection
 **/
void ixgbe_disable_mdd_X550(struct ixgbe_hw *hw)
{
	u32 reg;

	DEBUGFUNC("ixgbe_disable_mdd_X550");

	/* Disable MDD for TX DMA and interrupt */
	reg = IXGBE_READ_REG(hw, IXGBE_DMATXCTL);
	reg &= ~(IXGBE_DMATXCTL_MDP_EN | IXGBE_DMATXCTL_MBINTEN);
	IXGBE_WRITE_REG(hw, IXGBE_DMATXCTL, reg);

	/* Disable MDD for RX and interrupt */
	reg = IXGBE_READ_REG(hw, IXGBE_RDRXCTL);
	reg &= ~(IXGBE_RDRXCTL_MDP_EN | IXGBE_RDRXCTL_MBINTEN);
	IXGBE_WRITE_REG(hw, IXGBE_RDRXCTL, reg);
}

/**
 *  ixgbe_enable_mdd_X550
 *  @hw: pointer to hardware structure
 *
 *  Enable malicious driver detection
 **/
void ixgbe_enable_mdd_X550(struct ixgbe_hw *hw)
{
	u32 reg;

	DEBUGFUNC("ixgbe_enable_mdd_X550");

	/* Enable MDD for TX DMA and interrupt */
	reg = IXGBE_READ_REG(hw, IXGBE_DMATXCTL);
	reg |= (IXGBE_DMATXCTL_MDP_EN | IXGBE_DMATXCTL_MBINTEN);
	IXGBE_WRITE_REG(hw, IXGBE_DMATXCTL, reg);

	/* Enable MDD for RX and interrupt */
	reg = IXGBE_READ_REG(hw, IXGBE_RDRXCTL);
	reg |= (IXGBE_RDRXCTL_MDP_EN | IXGBE_RDRXCTL_MBINTEN);
	IXGBE_WRITE_REG(hw, IXGBE_RDRXCTL, reg);
}

/**
 *  ixgbe_restore_mdd_vf_X550
 *  @hw: pointer to hardware structure
 *  @vf: vf index
 *
 *  Restore VF that was disabled during malicious driver detection event
 **/
void ixgbe_restore_mdd_vf_X550(struct ixgbe_hw *hw, u32 vf)
{
	u32 idx, reg, num_qs, start_q, bitmask;

	DEBUGFUNC("ixgbe_restore_mdd_vf_X550");

	/* Map VF to queues */
	reg = IXGBE_READ_REG(hw, IXGBE_MRQC);
	switch (reg & IXGBE_MRQC_MRQE_MASK) {
	case IXGBE_MRQC_VMDQRT8TCEN:
		num_qs = 8;  /* 16 VFs / pools */
		bitmask = 0x000000FF;
		break;
	case IXGBE_MRQC_VMDQRSS32EN:
	case IXGBE_MRQC_VMDQRT4TCEN:
		num_qs = 4;  /* 32 VFs / pools */
		bitmask = 0x0000000F;
		break;
	default:            /* 64 VFs / pools */
		num_qs = 2;
		bitmask = 0x00000003;
		break;
	}
	start_q = vf * num_qs;

	/* Release vf's queues by clearing WQBR_TX and WQBR_RX (RW1C) */
	idx = start_q / 32;
	reg = 0;
	reg |= (bitmask << (start_q % 32));
	IXGBE_WRITE_REG(hw, IXGBE_WQBR_TX(idx), reg);
	IXGBE_WRITE_REG(hw, IXGBE_WQBR_RX(idx), reg);
}

/**
 *  ixgbe_mdd_event_X550
 *  @hw: pointer to hardware structure
 *  @vf_bitmap: vf bitmap of malicious vfs
 *
 *  Handle malicious driver detection event.
 **/
void ixgbe_mdd_event_X550(struct ixgbe_hw *hw, u32 *vf_bitmap)
{
	u32 wqbr;
	u32 i, j, reg, q, shift, vf, idx;

	DEBUGFUNC("ixgbe_mdd_event_X550");

	/* figure out pool size for mapping to vf's */
	reg = IXGBE_READ_REG(hw, IXGBE_MRQC);
	switch (reg & IXGBE_MRQC_MRQE_MASK) {
	case IXGBE_MRQC_VMDQRT8TCEN:
		shift = 3;  /* 16 VFs / pools */
		break;
	case IXGBE_MRQC_VMDQRSS32EN:
	case IXGBE_MRQC_VMDQRT4TCEN:
		shift = 2;  /* 32 VFs / pools */
		break;
	default:
		shift = 1;  /* 64 VFs / pools */
		break;
	}

	/* Read WQBR_TX and WQBR_RX and check for malicious queues */
	for (i = 0; i < 4; i++) {
		wqbr = IXGBE_READ_REG(hw, IXGBE_WQBR_TX(i));
		wqbr |= IXGBE_READ_REG(hw, IXGBE_WQBR_RX(i));

		if (!wqbr)
			continue;

		/* Get malicious queue */
		for (j = 0; j < 32 && wqbr; j++) {

			if (!(wqbr & (1 << j)))
				continue;

			/* Get queue from bitmask */
			q = j + (i * 32);

			/* Map queue to vf */
			vf = (q >> shift);

			/* Set vf bit in vf_bitmap */
			idx = vf / 32;
			vf_bitmap[idx] |= (1 << (vf % 32));
			wqbr &= ~(1 << j);
		}
	}
}

/**
 *  ixgbe_get_media_type_X550em - Get media type
 *  @hw: pointer to hardware structure
 *
 *  Returns the media type (fiber, copper, backplane)
 */
enum ixgbe_media_type ixgbe_get_media_type_X550em(struct ixgbe_hw *hw)
{
	enum ixgbe_media_type media_type;

	DEBUGFUNC("ixgbe_get_media_type_X550em");

	/* Detect if there is a copper PHY attached. */
	switch (hw->device_id) {
	case IXGBE_DEV_ID_X550EM_X:
	case IXGBE_DEV_ID_X550EM_X_KR:
	case IXGBE_DEV_ID_X550EM_X_KX4:
		media_type = ixgbe_media_type_backplane;
		break;
	case IXGBE_DEV_ID_X550EM_X_SFP:
		media_type = ixgbe_media_type_fiber;
		break;
	default:
		media_type = ixgbe_media_type_unknown;
		break;
	}
	return media_type;
}

/**
 *  ixgbe_setup_sfp_modules_X550em - Setup SFP module
 *  @hw: pointer to hardware structure
 */
s32 ixgbe_setup_sfp_modules_X550em(struct ixgbe_hw *hw)
{
	bool setup_linear;
	u16 reg_slice, edc_mode;

	DEBUGFUNC("ixgbe_setup_sfp_modules_X550em");

	switch (hw->phy.sfp_type) {
	case ixgbe_sfp_type_unknown:
		return IXGBE_SUCCESS;
	case ixgbe_sfp_type_not_present:
		return IXGBE_ERR_SFP_NOT_PRESENT;
	case ixgbe_sfp_type_da_cu_core0:
	case ixgbe_sfp_type_da_cu_core1:
		setup_linear = true;
		break;
	case ixgbe_sfp_type_srlr_core0:
	case ixgbe_sfp_type_srlr_core1:
	case ixgbe_sfp_type_da_act_lmt_core0:
	case ixgbe_sfp_type_da_act_lmt_core1:
	case ixgbe_sfp_type_1g_sx_core0:
	case ixgbe_sfp_type_1g_sx_core1:
	case ixgbe_sfp_type_1g_lx_core0:
	case ixgbe_sfp_type_1g_lx_core1:
		setup_linear = false;
		break;
	default:
		return IXGBE_ERR_SFP_NOT_SUPPORTED;
	}

	ixgbe_init_mac_link_ops_X550em(hw);
	hw->phy.ops.reset = NULL;

	/* The CS4227 slice address is the base address + the port-pair reg
	 * offset. I.e. Slice 0 = 0x0000 and slice 1 = 0x1000.
	 */
	reg_slice = IXGBE_CS4227_SPARE24_LSB + (hw->phy.lan_id << 12);

	if (setup_linear)
		edc_mode = (IXGBE_CS4227_EDC_MODE_CX1 << 1) | 0x1;
	else
		edc_mode = (IXGBE_CS4227_EDC_MODE_SR << 1) | 0x1;

	/* Configure CS4227 for connection type. */
	return hw->phy.ops.write_i2c_combined(hw, IXGBE_CS4227,
					      reg_slice, edc_mode);
}

/**
 *  ixgbe_init_mac_link_ops_X550em - init mac link function pointers
 *  @hw: pointer to hardware structure
 */
void ixgbe_init_mac_link_ops_X550em(struct ixgbe_hw *hw)
{
	struct ixgbe_mac_info *mac = &hw->mac;

	DEBUGFUNC("ixgbe_init_mac_link_ops_X550em");

	/* CS4227 does not support autoneg, so disable the laser control
	 * functions for SFP+ fiber
	 */
	 if (hw->device_id == IXGBE_DEV_ID_X550EM_X_SFP) {
		mac->ops.disable_tx_laser = NULL;
		mac->ops.enable_tx_laser = NULL;
		mac->ops.flap_tx_laser = NULL;
	 }
}

/**
 *  ixgbe_get_link_capabilities_x550em - Determines link capabilities
 *  @hw: pointer to hardware structure
 *  @speed: pointer to link speed
 *  @autoneg: true when autoneg or autotry is enabled
 */
s32 ixgbe_get_link_capabilities_X550em(struct ixgbe_hw *hw,
				       ixgbe_link_speed *speed,
				       bool *autoneg)
{
	DEBUGFUNC("ixgbe_get_link_capabilities_X550em");

	/* SFP */
	if (hw->phy.media_type == ixgbe_media_type_fiber) {

		/* CS4227 SFP must not enable auto-negotiation */
		*autoneg = false;

		/* Check if 1G SFP module. */
		if (hw->phy.sfp_type == ixgbe_sfp_type_1g_sx_core0 ||
		    hw->phy.sfp_type == ixgbe_sfp_type_1g_sx_core1
		    || hw->phy.sfp_type == ixgbe_sfp_type_1g_lx_core0 ||
		    hw->phy.sfp_type == ixgbe_sfp_type_1g_lx_core1) {
			*speed = IXGBE_LINK_SPEED_1GB_FULL;
			return IXGBE_SUCCESS;
		}

		/* Link capabilities are based on SFP */
		if (hw->phy.multispeed_fiber)
			*speed |= IXGBE_LINK_SPEED_10GB_FULL |
				  IXGBE_LINK_SPEED_1GB_FULL;
		else
			*speed = IXGBE_LINK_SPEED_10GB_FULL;
	} else {
		*speed |= IXGBE_LINK_SPEED_10GB_FULL |
			  IXGBE_LINK_SPEED_1GB_FULL;
		*autoneg = true;
	}

	return IXGBE_SUCCESS;
}

/**
 *  ixgbe_init_phy_ops_X550em - PHY/SFP specific init
 *  @hw: pointer to hardware structure
 *
 *  Initialize any function pointers that were not able to be
 *  set during init_shared_code because the PHY/SFP type was
 *  not known.  Perform the SFP init if necessary.
 */
s32 ixgbe_init_phy_ops_X550em(struct ixgbe_hw *hw)
{
	struct ixgbe_phy_info *phy = &hw->phy;
	s32 ret_val;
	u32 esdp;

	DEBUGFUNC("ixgbe_init_phy_ops_X550em");

	if (hw->device_id == IXGBE_DEV_ID_X550EM_X_SFP) {
		esdp = IXGBE_READ_REG(hw, IXGBE_ESDP);
		phy->lan_id = IXGBE_READ_REG(hw, IXGBE_STATUS) &
			      IXGBE_STATUS_LAN_ID_1;
		phy->phy_semaphore_mask = IXGBE_GSSR_SHARED_I2C_SM;
		if (phy->lan_id) {
			esdp &= ~(IXGBE_ESDP_SDP1_NATIVE | IXGBE_ESDP_SDP1);
			esdp |= IXGBE_ESDP_SDP1_DIR;
		}
		esdp &= ~(IXGBE_ESDP_SDP0_NATIVE | IXGBE_ESDP_SDP0_DIR);
		IXGBE_WRITE_REG(hw, IXGBE_ESDP, esdp);
	}

	/* Identify the PHY or SFP module */
	ret_val = phy->ops.identify(hw);
	if (ret_val == IXGBE_ERR_SFP_NOT_SUPPORTED)
		return ret_val;

	/* Setup function pointers based on detected SFP module and speeds */
	ixgbe_init_mac_link_ops_X550em(hw);
	if (phy->sfp_type != ixgbe_sfp_type_unknown)
		phy->ops.reset = NULL;

	/* Set functions pointers based on phy type */
	switch (hw->phy.type) {
	case ixgbe_phy_x550em_kr:
		phy->ops.setup_link = ixgbe_setup_kr_x550em;
		break;
	default:
		break;
	}
	return ret_val;
}

/**
 *  ixgbe_reset_hw_X550em - Perform hardware reset
 *  @hw: pointer to hardware structure
 *
 *  Resets the hardware by resetting the transmit and receive units, masks
 *  and clears all interrupts, perform a PHY reset, and perform a link (MAC)
 *  reset.
 */
s32 ixgbe_reset_hw_X550em(struct ixgbe_hw *hw)
{
	ixgbe_link_speed link_speed;
	s32 status;
	u32 ctrl = 0;
	u32 i;
	bool link_up = false;

	DEBUGFUNC("ixgbe_reset_hw_X550em");

	/* Call adapter stop to disable Tx/Rx and clear interrupts */
	status = hw->mac.ops.stop_adapter(hw);
	if (status != IXGBE_SUCCESS)
		return status;

	/* flush pending Tx transactions */
	ixgbe_clear_tx_pending(hw);

	/* PHY ops must be identified and initialized prior to reset */

	/* Identify PHY and related function pointers */
	status = hw->phy.ops.init(hw);

	if (status == IXGBE_ERR_SFP_NOT_SUPPORTED)
		return status;

	/* Setup SFP module if there is one present. */
	if (hw->phy.sfp_setup_needed) {
		status = hw->mac.ops.setup_sfp(hw);
		hw->phy.sfp_setup_needed = false;
	}

	if (status == IXGBE_ERR_SFP_NOT_SUPPORTED)
		return status;

	/* Reset PHY */
	if (!hw->phy.reset_disable && hw->phy.ops.reset)
		hw->phy.ops.reset(hw);

mac_reset_top:
	/* Issue global reset to the MAC.  Needs to be SW reset if link is up.
	 * If link reset is used when link is up, it might reset the PHY when
	 * mng is using it.  If link is down or the flag to force full link
	 * reset is set, then perform link reset.
	 */
	ctrl = IXGBE_CTRL_LNK_RST;
	if (!hw->force_full_reset) {
		hw->mac.ops.check_link(hw, &link_speed, &link_up, false);
		if (link_up)
			ctrl = IXGBE_CTRL_RST;
	}

	ctrl |= IXGBE_READ_REG(hw, IXGBE_CTRL);
	IXGBE_WRITE_REG(hw, IXGBE_CTRL, ctrl);
	IXGBE_WRITE_FLUSH(hw);

	/* Poll for reset bit to self-clear meaning reset is complete */
	for (i = 0; i < 10; i++) {
		usec_delay(1);
		ctrl = IXGBE_READ_REG(hw, IXGBE_CTRL);
		if (!(ctrl & IXGBE_CTRL_RST_MASK))
			break;
	}

	if (ctrl & IXGBE_CTRL_RST_MASK) {
		status = IXGBE_ERR_RESET_FAILED;
		DEBUGOUT("Reset polling failed to complete.\n");
	}

	msec_delay(50);

	/* Double resets are required for recovery from certain error
	 * conditions.  Between resets, it is necessary to stall to
	 * allow time for any pending HW events to complete.
	 */
	if (hw->mac.flags & IXGBE_FLAGS_DOUBLE_RESET_REQUIRED) {
		hw->mac.flags &= ~IXGBE_FLAGS_DOUBLE_RESET_REQUIRED;
		goto mac_reset_top;
	}

	/* Store the permanent mac address */
	hw->mac.ops.get_mac_addr(hw, hw->mac.perm_addr);

	/* Store MAC address from RAR0, clear receive address registers, and
	 * clear the multicast table.  Also reset num_rar_entries to 128,
	 * since we modify this value when programming the SAN MAC address.
	 */
	hw->mac.num_rar_entries = 128;
	hw->mac.ops.init_rx_addrs(hw);

	return status;
}

/**
 *  ixgbe_setup_kr_x550em - Configure the KR PHY.
 *  @hw: pointer to hardware structure
 *
 *  Configures the integrated KR PHY.
 **/
s32 ixgbe_setup_kr_x550em(struct ixgbe_hw *hw)
{
	s32 status;
	u32 reg_val;

	status = ixgbe_read_iosf_sb_reg_x550(hw,
		IXGBE_KRM_LINK_CTRL_1(hw->phy.lan_id),
		IXGBE_SB_IOSF_TARGET_KR_PHY, &reg_val);

	reg_val |= IXGBE_KRM_LINK_CTRL_1_TETH_AN_ENABLE;
	reg_val |= IXGBE_KRM_LINK_CTRL_1_TETH_AN_FEC_REQ;
	reg_val |= IXGBE_KRM_LINK_CTRL_1_TETH_AN_CAP_FEC;
	reg_val &= ~(IXGBE_KRM_LINK_CTRL_1_TETH_AN_CAP_KR |
		     IXGBE_KRM_LINK_CTRL_1_TETH_AN_CAP_KX);

	/* Advertise 10G support. */
	if (hw->phy.autoneg_advertised & IXGBE_LINK_SPEED_10GB_FULL)
		reg_val |= IXGBE_KRM_LINK_CTRL_1_TETH_AN_CAP_KR;

	/* Advertise 1G support. */
	if (hw->phy.autoneg_advertised & IXGBE_LINK_SPEED_1GB_FULL)
		reg_val |= IXGBE_KRM_LINK_CTRL_1_TETH_AN_CAP_KX;

	/* Restart auto-negotiation. */
	reg_val |= IXGBE_KRM_LINK_CTRL_1_TETH_AN_RESTART;
	status = ixgbe_write_iosf_sb_reg_x550(hw,
		IXGBE_KRM_LINK_CTRL_1(hw->phy.lan_id),
		IXGBE_SB_IOSF_TARGET_KR_PHY, reg_val);

	return status;
}

/**
 *  ixgbe_setup_ixfi_x550em - Configure the KR PHY for iXFI.
 *  @hw: pointer to hardware structure
 *
 *  Configures the integrated KR PHY to use iXFI mode.
 **/
s32 ixgbe_setup_ixfi_x550em(struct ixgbe_hw *hw)
{
	s32 status;
	u32 reg_val;

	/* Disable AN and force speed to 10G Serial. */
	status = ixgbe_read_iosf_sb_reg_x550(hw,
					IXGBE_KRM_LINK_CTRL_1(hw->phy.lan_id),
					IXGBE_SB_IOSF_TARGET_KR_PHY, &reg_val);
	if (status != IXGBE_SUCCESS)
		return status;
	reg_val &= ~IXGBE_KRM_LINK_CTRL_1_TETH_AN_ENABLE;
	reg_val &= ~IXGBE_KRM_LINK_CTRL_1_TETH_FORCE_SPEED_MASK;
	reg_val |= IXGBE_KRM_LINK_CTRL_1_TETH_FORCE_SPEED_10G;
	status = ixgbe_write_iosf_sb_reg_x550(hw,
					IXGBE_KRM_LINK_CTRL_1(hw->phy.lan_id),
					IXGBE_SB_IOSF_TARGET_KR_PHY, reg_val);
	if (status != IXGBE_SUCCESS)
		return status;

	/* Disable training protocol FSM. */
	status = ixgbe_read_iosf_sb_reg_x550(hw,
				IXGBE_KRM_RX_TRN_LINKUP_CTRL(hw->phy.lan_id),
				IXGBE_SB_IOSF_TARGET_KR_PHY, &reg_val);
	if (status != IXGBE_SUCCESS)
		return status;
	reg_val |= IXGBE_KRM_RX_TRN_LINKUP_CTRL_CONV_WO_PROTOCOL;
	status = ixgbe_write_iosf_sb_reg_x550(hw,
				IXGBE_KRM_RX_TRN_LINKUP_CTRL(hw->phy.lan_id),
				IXGBE_SB_IOSF_TARGET_KR_PHY, reg_val);
	if (status != IXGBE_SUCCESS)
		return status;

	/* Disable Flex from training TXFFE. */
	status = ixgbe_read_iosf_sb_reg_x550(hw,
				IXGBE_KRM_DSP_TXFFE_STATE_4(hw->phy.lan_id),
				IXGBE_SB_IOSF_TARGET_KR_PHY, &reg_val);
	if (status != IXGBE_SUCCESS)
		return status;
	reg_val &= ~IXGBE_KRM_DSP_TXFFE_STATE_C0_EN;
	reg_val &= ~IXGBE_KRM_DSP_TXFFE_STATE_CP1_CN1_EN;
	reg_val &= ~IXGBE_KRM_DSP_TXFFE_STATE_CO_ADAPT_EN;
	status = ixgbe_write_iosf_sb_reg_x550(hw,
				IXGBE_KRM_DSP_TXFFE_STATE_4(hw->phy.lan_id),
				IXGBE_SB_IOSF_TARGET_KR_PHY, reg_val);
	if (status != IXGBE_SUCCESS)
		return status;
	status = ixgbe_read_iosf_sb_reg_x550(hw,
				IXGBE_KRM_DSP_TXFFE_STATE_5(hw->phy.lan_id),
				IXGBE_SB_IOSF_TARGET_KR_PHY, &reg_val);
	if (status != IXGBE_SUCCESS)
		return status;
	reg_val &= ~IXGBE_KRM_DSP_TXFFE_STATE_C0_EN;
	reg_val &= ~IXGBE_KRM_DSP_TXFFE_STATE_CP1_CN1_EN;
	reg_val &= ~IXGBE_KRM_DSP_TXFFE_STATE_CO_ADAPT_EN;
	status = ixgbe_write_iosf_sb_reg_x550(hw,
				IXGBE_KRM_DSP_TXFFE_STATE_5(hw->phy.lan_id),
				IXGBE_SB_IOSF_TARGET_KR_PHY, reg_val);
	if (status != IXGBE_SUCCESS)
		return status;

	/* Enable override for coefficients. */
	status = ixgbe_read_iosf_sb_reg_x550(hw,
				IXGBE_KRM_TX_COEFF_CTRL_1(hw->phy.lan_id),
				IXGBE_SB_IOSF_TARGET_KR_PHY, &reg_val);
	if (status != IXGBE_SUCCESS)
		return status;
	reg_val |= IXGBE_KRM_TX_COEFF_CTRL_1_OVRRD_EN;
	reg_val |= IXGBE_KRM_TX_COEFF_CTRL_1_CZERO_EN;
	reg_val |= IXGBE_KRM_TX_COEFF_CTRL_1_CPLUS1_OVRRD_EN;
	reg_val |= IXGBE_KRM_TX_COEFF_CTRL_1_CMINUS1_OVRRD_EN;
	status = ixgbe_write_iosf_sb_reg_x550(hw,
				IXGBE_KRM_TX_COEFF_CTRL_1(hw->phy.lan_id),
				IXGBE_SB_IOSF_TARGET_KR_PHY, reg_val);
	if (status != IXGBE_SUCCESS)
		return status;

	/* Toggle port SW reset by AN reset. */
	status = ixgbe_read_iosf_sb_reg_x550(hw,
					IXGBE_KRM_LINK_CTRL_1(hw->phy.lan_id),
					IXGBE_SB_IOSF_TARGET_KR_PHY, &reg_val);
	if (status != IXGBE_SUCCESS)
		return status;
	reg_val |= IXGBE_KRM_LINK_CTRL_1_TETH_AN_RESTART;
	status = ixgbe_write_iosf_sb_reg_x550(hw,
					IXGBE_KRM_LINK_CTRL_1(hw->phy.lan_id),
					IXGBE_SB_IOSF_TARGET_KR_PHY, reg_val);

	return status;
}

/**
 *  ixgbe_setup_phy_loopback_x550em - Configure the KR PHY for loopback.
 *  @hw: pointer to hardware structure
 *
 *  Configures the integrated KR PHY to use internal loopback mode.
 **/
s32 ixgbe_setup_phy_loopback_x550em(struct ixgbe_hw *hw)
{
	s32 status;
	u32 reg_val;

	/* Disable AN and force speed to 10G Serial. */
	status = ixgbe_read_iosf_sb_reg_x550(hw,
		IXGBE_KRM_LINK_CTRL_1(hw->phy.lan_id),
		IXGBE_SB_IOSF_TARGET_KR_PHY, &reg_val);
	if (status != IXGBE_SUCCESS)
		return status;
	reg_val &= ~IXGBE_KRM_LINK_CTRL_1_TETH_AN_ENABLE;
	reg_val &= ~IXGBE_KRM_LINK_CTRL_1_TETH_FORCE_SPEED_MASK;
	reg_val |= IXGBE_KRM_LINK_CTRL_1_TETH_FORCE_SPEED_10G;
	status = ixgbe_write_iosf_sb_reg_x550(hw,
		IXGBE_KRM_LINK_CTRL_1(hw->phy.lan_id),
		IXGBE_SB_IOSF_TARGET_KR_PHY, reg_val);
	if (status != IXGBE_SUCCESS)
		return status;

	/* Set near-end loopback clocks. */
	status = ixgbe_read_iosf_sb_reg_x550(hw,
		IXGBE_KRM_PORT_CAR_GEN_CTRL(hw->phy.lan_id),
		IXGBE_SB_IOSF_TARGET_KR_PHY, &reg_val);
	if (status != IXGBE_SUCCESS)
		return status;
	reg_val |= IXGBE_KRM_PORT_CAR_GEN_CTRL_NELB_32B;
	reg_val |= IXGBE_KRM_PORT_CAR_GEN_CTRL_NELB_KRPCS;
	status = ixgbe_write_iosf_sb_reg_x550(hw,
		IXGBE_KRM_PORT_CAR_GEN_CTRL(hw->phy.lan_id),
		IXGBE_SB_IOSF_TARGET_KR_PHY, reg_val);
	if (status != IXGBE_SUCCESS)
		return status;

	/* Set loopback enable. */
	status = ixgbe_read_iosf_sb_reg_x550(hw,
		IXGBE_KRM_PMD_DFX_BURNIN(hw->phy.lan_id),
		IXGBE_SB_IOSF_TARGET_KR_PHY, &reg_val);
	if (status != IXGBE_SUCCESS)
		return status;
	reg_val |= IXGBE_KRM_PMD_DFX_BURNIN_TX_RX_KR_LB_MASK;
	status = ixgbe_write_iosf_sb_reg_x550(hw,
		IXGBE_KRM_PMD_DFX_BURNIN(hw->phy.lan_id),
		IXGBE_SB_IOSF_TARGET_KR_PHY, reg_val);
	if (status != IXGBE_SUCCESS)
		return status;

	/* Training bypass. */
	status = ixgbe_read_iosf_sb_reg_x550(hw,
		IXGBE_KRM_RX_TRN_LINKUP_CTRL(hw->phy.lan_id),
		IXGBE_SB_IOSF_TARGET_KR_PHY, &reg_val);
	if (status != IXGBE_SUCCESS)
		return status;
	reg_val |= IXGBE_KRM_RX_TRN_LINKUP_CTRL_PROTOCOL_BYPASS;
	status = ixgbe_write_iosf_sb_reg_x550(hw,
		IXGBE_KRM_RX_TRN_LINKUP_CTRL(hw->phy.lan_id),
		IXGBE_SB_IOSF_TARGET_KR_PHY, reg_val);

	return status;
}

/**
 *  ixgbe_read_ee_hostif_X550 - Read EEPROM word using a host interface command
 *  assuming that the semaphore is already obtained.
 *  @hw: pointer to hardware structure
 *  @offset: offset of  word in the EEPROM to read
 *  @data: word read from the EEPROM
 *
 *  Reads a 16 bit word from the EEPROM using the hostif.
 **/
s32 ixgbe_read_ee_hostif_data_X550(struct ixgbe_hw *hw, u16 offset,
				   u16 *data)
{
	s32 status;
	struct ixgbe_hic_read_shadow_ram buffer;

	DEBUGFUNC("ixgbe_read_ee_hostif_data_X550");
	buffer.hdr.cmd = FW_READ_SHADOW_RAM_CMD;
	buffer.hdr.buf_len1 = 0;
	buffer.hdr.buf_len2 = FW_READ_SHADOW_RAM_LEN;
	buffer.hdr.checksum = FW_DEFAULT_CHECKSUM;

	/* convert offset from words to bytes */
	buffer.address = IXGBE_CPU_TO_BE32(offset * 2);
	/* one word */
	buffer.length = IXGBE_CPU_TO_BE16(sizeof(u16));

	status = ixgbe_host_interface_command(hw, (u32 *)&buffer,
					      sizeof(buffer), false);

	if (status)
		return status;

	*data = (u16)IXGBE_READ_REG_ARRAY(hw, IXGBE_FLEX_MNG,
					  FW_NVM_DATA_OFFSET);

	return 0;
}

/**
 *  ixgbe_read_ee_hostif_X550 - Read EEPROM word using a host interface command
 *  @hw: pointer to hardware structure
 *  @offset: offset of  word in the EEPROM to read
 *  @data: word read from the EEPROM
 *
 *  Reads a 16 bit word from the EEPROM using the hostif.
 **/
s32 ixgbe_read_ee_hostif_X550(struct ixgbe_hw *hw, u16 offset,
			      u16 *data)
{
	s32 status = IXGBE_SUCCESS;

	DEBUGFUNC("ixgbe_read_ee_hostif_X550");

	if (hw->mac.ops.acquire_swfw_sync(hw, IXGBE_GSSR_EEP_SM) ==
	    IXGBE_SUCCESS) {
		status = ixgbe_read_ee_hostif_data_X550(hw, offset, data);
		hw->mac.ops.release_swfw_sync(hw, IXGBE_GSSR_EEP_SM);
	} else {
		status = IXGBE_ERR_SWFW_SYNC;
	}

	return status;
}

/**
 *  ixgbe_read_ee_hostif_buffer_X550- Read EEPROM word(s) using hostif
 *  @hw: pointer to hardware structure
 *  @offset: offset of  word in the EEPROM to read
 *  @words: number of words
 *  @data: word(s) read from the EEPROM
 *
 *  Reads a 16 bit word(s) from the EEPROM using the hostif.
 **/
s32 ixgbe_read_ee_hostif_buffer_X550(struct ixgbe_hw *hw,
				     u16 offset, u16 words, u16 *data)
{
	struct ixgbe_hic_read_shadow_ram buffer;
	u32 current_word = 0;
	u16 words_to_read;
	s32 status;
	u32 i;

	DEBUGFUNC("ixgbe_read_ee_hostif_buffer_X550");

	/* Take semaphore for the entire operation. */
	status = hw->mac.ops.acquire_swfw_sync(hw, IXGBE_GSSR_EEP_SM);
	if (status) {
		DEBUGOUT("EEPROM read buffer - semaphore failed\n");
		return status;
	}
	while (words) {
		if (words > FW_MAX_READ_BUFFER_SIZE / 2)
			words_to_read = FW_MAX_READ_BUFFER_SIZE / 2;
		else
			words_to_read = words;

		buffer.hdr.cmd = FW_READ_SHADOW_RAM_CMD;
		buffer.hdr.buf_len1 = 0;
		buffer.hdr.buf_len2 = FW_READ_SHADOW_RAM_LEN;
		buffer.hdr.checksum = FW_DEFAULT_CHECKSUM;

		/* convert offset from words to bytes */
		buffer.address = IXGBE_CPU_TO_BE32((offset + current_word) * 2);
		buffer.length = IXGBE_CPU_TO_BE16(words_to_read * 2);

		status = ixgbe_host_interface_command(hw, (u32 *)&buffer,
						      sizeof(buffer), false);

		if (status) {
			DEBUGOUT("Host interface command failed\n");
			goto out;
		}

		for (i = 0; i < words_to_read; i++) {
			u32 reg = IXGBE_FLEX_MNG + (FW_NVM_DATA_OFFSET << 2) +
				  2 * i;
			u32 value = IXGBE_READ_REG(hw, reg);

			data[current_word] = (u16)(value & 0xffff);
			current_word++;
			i++;
			if (i < words_to_read) {
				value >>= 16;
				data[current_word] = (u16)(value & 0xffff);
				current_word++;
			}
		}
		words -= words_to_read;
	}

out:
	hw->mac.ops.release_swfw_sync(hw, IXGBE_GSSR_EEP_SM);
	return status;
}

/**
 *  ixgbe_write_ee_hostif_X550 - Write EEPROM word using hostif
 *  @hw: pointer to hardware structure
 *  @offset: offset of  word in the EEPROM to write
 *  @data: word write to the EEPROM
 *
 *  Write a 16 bit word to the EEPROM using the hostif.
 **/
s32 ixgbe_write_ee_hostif_data_X550(struct ixgbe_hw *hw, u16 offset,
				    u16 data)
{
	s32 status;
	struct ixgbe_hic_write_shadow_ram buffer;

	DEBUGFUNC("ixgbe_write_ee_hostif_data_X550");

	buffer.hdr.cmd = FW_WRITE_SHADOW_RAM_CMD;
	buffer.hdr.buf_len1 = 0;
	buffer.hdr.buf_len2 = FW_WRITE_SHADOW_RAM_LEN;
	buffer.hdr.checksum = FW_DEFAULT_CHECKSUM;

	 /* one word */
	buffer.length = IXGBE_CPU_TO_BE16(sizeof(u16));
	buffer.data = data;
	buffer.address = IXGBE_CPU_TO_BE32(offset * 2);

	status = ixgbe_host_interface_command(hw, (u32 *)&buffer,
					      sizeof(buffer), false);

	return status;
}

/**
 *  ixgbe_write_ee_hostif_X550 - Write EEPROM word using hostif
 *  @hw: pointer to hardware structure
 *  @offset: offset of  word in the EEPROM to write
 *  @data: word write to the EEPROM
 *
 *  Write a 16 bit word to the EEPROM using the hostif.
 **/
s32 ixgbe_write_ee_hostif_X550(struct ixgbe_hw *hw, u16 offset,
			       u16 data)
{
	s32 status = IXGBE_SUCCESS;

	DEBUGFUNC("ixgbe_write_ee_hostif_X550");

	if (hw->mac.ops.acquire_swfw_sync(hw, IXGBE_GSSR_EEP_SM) ==
	    IXGBE_SUCCESS) {
		status = ixgbe_write_ee_hostif_data_X550(hw, offset, data);
		hw->mac.ops.release_swfw_sync(hw, IXGBE_GSSR_EEP_SM);
	} else {
		DEBUGOUT("write ee hostif failed to get semaphore");
		status = IXGBE_ERR_SWFW_SYNC;
	}

	return status;
}

/**
 *  ixgbe_write_ee_hostif_buffer_X550 - Write EEPROM word(s) using hostif
 *  @hw: pointer to hardware structure
 *  @offset: offset of  word in the EEPROM to write
 *  @words: number of words
 *  @data: word(s) write to the EEPROM
 *
 *  Write a 16 bit word(s) to the EEPROM using the hostif.
 **/
s32 ixgbe_write_ee_hostif_buffer_X550(struct ixgbe_hw *hw,
				      u16 offset, u16 words, u16 *data)
{
	s32 status = IXGBE_SUCCESS;
	u32 i = 0;

	DEBUGFUNC("ixgbe_write_ee_hostif_buffer_X550");

	/* Take semaphore for the entire operation. */
	status = hw->mac.ops.acquire_swfw_sync(hw, IXGBE_GSSR_EEP_SM);
	if (status != IXGBE_SUCCESS) {
		DEBUGOUT("EEPROM write buffer - semaphore failed\n");
		goto out;
	}

	for (i = 0; i < words; i++) {
		status = ixgbe_write_ee_hostif_data_X550(hw, offset + i,
							 data[i]);

		if (status != IXGBE_SUCCESS) {
			DEBUGOUT("Eeprom buffered write failed\n");
			break;
		}
	}

	hw->mac.ops.release_swfw_sync(hw, IXGBE_GSSR_EEP_SM);
out:

	return status;
}

/**
 * ixgbe_checksum_ptr_x550 - Checksum one pointer region
 * @hw: pointer to hardware structure
 * @ptr: pointer offset in eeprom
 * @size: size of section pointed by ptr, if 0 first word will be used as size
 * @csum: address of checksum to update
 *
 * Returns error status for any failure
 */
STATIC s32 ixgbe_checksum_ptr_x550(struct ixgbe_hw *hw, u16 ptr,
				   u16 size, u16 *csum)
{
	u16 buf[256];
	s32 status;
	u16 length, bufsz, i, start;

	bufsz = sizeof(buf) / sizeof(buf[0]);

	/* Read a chunk at the pointer location */
	status = ixgbe_read_ee_hostif_buffer_X550(hw, ptr, bufsz, buf);
	if (status) {
		DEBUGOUT("Failed to read EEPROM image\n");
		return status;
	}

	if (size) {
		start = 0;
		length = size;
	} else {
		start = 1;
		length = buf[0];

		/* Skip pointer section if length is invalid. */
		if (length == 0xFFFF || length == 0 ||
		    (ptr + length) >= hw->eeprom.word_size)
			return IXGBE_SUCCESS;
	}

	for (i = start; length; i++, length--) {
		if (i == bufsz) {
			ptr += bufsz;
			i = 0;
			if (length < bufsz)
				bufsz = length;

			/* Read a chunk at the pointer location */
			status = ixgbe_read_ee_hostif_buffer_X550(hw, ptr,
								  bufsz, buf);
			if (status) {
				DEBUGOUT("Failed to read EEPROM image\n");
				return status;
			}
		}
		*csum += buf[i];
	}
	return IXGBE_SUCCESS;
}

/**
 *  ixgbe_calc_eeprom_checksum_X550 - Calculates and returns the checksum
 *  @hw: pointer to hardware structure
 *
 *  Returns a negative error code on error, or the 16-bit checksum
 **/
s32 ixgbe_calc_eeprom_checksum_X550(struct ixgbe_hw *hw)
{
	u16 eeprom_ptrs[IXGBE_EEPROM_LAST_WORD + 1];
	s32 status;
	u16 checksum = 0;
	u16 pointer, i, size;

	DEBUGFUNC("ixgbe_calc_eeprom_checksum_X550");

	hw->eeprom.ops.init_params(hw);

	/* Read pointer area */
	status = ixgbe_read_ee_hostif_buffer_X550(hw, 0,
						  IXGBE_EEPROM_LAST_WORD + 1,
						  eeprom_ptrs);
	if (status) {
		DEBUGOUT("Failed to read EEPROM image\n");
		return status;
	}

	/*
	 * For X550 hardware include 0x0-0x41 in the checksum, skip the
	 * checksum word itself
	 */
	for (i = 0; i <= IXGBE_EEPROM_LAST_WORD; i++)
		if (i != IXGBE_EEPROM_CHECKSUM)
			checksum += eeprom_ptrs[i];

	/*
	 * Include all data from pointers 0x3, 0x6-0xE.  This excludes the
	 * FW, PHY module, and PCIe Expansion/Option ROM pointers.
	 */
	for (i = IXGBE_PCIE_ANALOG_PTR_X550; i < IXGBE_FW_PTR; i++) {
		if (i == IXGBE_PHY_PTR || i == IXGBE_OPTION_ROM_PTR)
			continue;

		pointer = eeprom_ptrs[i];

		/* Skip pointer section if the pointer is invalid. */
		if (pointer == 0xFFFF || pointer == 0 ||
		    pointer >= hw->eeprom.word_size)
			continue;

		switch (i) {
		case IXGBE_PCIE_GENERAL_PTR:
			size = IXGBE_IXGBE_PCIE_GENERAL_SIZE;
			break;
		case IXGBE_PCIE_CONFIG0_PTR:
		case IXGBE_PCIE_CONFIG1_PTR:
			size = IXGBE_PCIE_CONFIG_SIZE;
			break;
		default:
			size = 0;
			break;
		}

		status = ixgbe_checksum_ptr_x550(hw, pointer, size, &checksum);
		if (status)
			return status;
	}

	checksum = (u16)IXGBE_EEPROM_SUM - checksum;

	return (s32)checksum;
}

/**
 *  ixgbe_validate_eeprom_checksum_X550 - Validate EEPROM checksum
 *  @hw: pointer to hardware structure
 *  @checksum_val: calculated checksum
 *
 *  Performs checksum calculation and validates the EEPROM checksum.  If the
 *  caller does not need checksum_val, the value can be NULL.
 **/
s32 ixgbe_validate_eeprom_checksum_X550(struct ixgbe_hw *hw,
					u16 *checksum_val)
{
	s32 status;
	u16 checksum;
	u16 read_checksum = 0;

	DEBUGFUNC("ixgbe_validate_eeprom_checksum_X550");

	/* Read the first word from the EEPROM. If this times out or fails, do
	 * not continue or we could be in for a very long wait while every
	 * EEPROM read fails
	 */
	status = hw->eeprom.ops.read(hw, 0, &checksum);
	if (status) {
		DEBUGOUT("EEPROM read failed\n");
		return status;
	}

	status = hw->eeprom.ops.calc_checksum(hw);
	if (status < 0)
		return status;

	checksum = (u16)(status & 0xffff);

	status = ixgbe_read_ee_hostif_X550(hw, IXGBE_EEPROM_CHECKSUM,
					   &read_checksum);
	if (status)
		return status;

	/* Verify read checksum from EEPROM is the same as
	 * calculated checksum
	 */
	if (read_checksum != checksum) {
		status = IXGBE_ERR_EEPROM_CHECKSUM;
		ERROR_REPORT1(IXGBE_ERROR_INVALID_STATE,
			     "Invalid EEPROM checksum");
	}

	/* If the user cares, return the calculated checksum */
	if (checksum_val)
		*checksum_val = checksum;

	return status;
}

/**
 * ixgbe_update_eeprom_checksum_X550 - Updates the EEPROM checksum and flash
 * @hw: pointer to hardware structure
 *
 * After writing EEPROM to shadow RAM using EEWR register, software calculates
 * checksum and updates the EEPROM and instructs the hardware to update
 * the flash.
 **/
s32 ixgbe_update_eeprom_checksum_X550(struct ixgbe_hw *hw)
{
	s32 status;
	u16 checksum = 0;

	DEBUGFUNC("ixgbe_update_eeprom_checksum_X550");

	/* Read the first word from the EEPROM. If this times out or fails, do
	 * not continue or we could be in for a very long wait while every
	 * EEPROM read fails
	 */
	status = ixgbe_read_ee_hostif_X550(hw, 0, &checksum);
	if (status) {
		DEBUGOUT("EEPROM read failed\n");
		return status;
	}

	status = ixgbe_calc_eeprom_checksum_X550(hw);
	if (status < 0)
		return status;

	checksum = (u16)(status & 0xffff);

	status = ixgbe_write_ee_hostif_X550(hw, IXGBE_EEPROM_CHECKSUM,
					    checksum);
	if (status)
		return status;

	status = ixgbe_update_flash_X550(hw);

	return status;
}

/**
 *  ixgbe_update_flash_X550 - Instruct HW to copy EEPROM to Flash device
 *  @hw: pointer to hardware structure
 *
 *  Issue a shadow RAM dump to FW to copy EEPROM from shadow RAM to the flash.
 **/
s32 ixgbe_update_flash_X550(struct ixgbe_hw *hw)
{
	s32 status = IXGBE_SUCCESS;
	struct ixgbe_hic_hdr2 buffer;

	DEBUGFUNC("ixgbe_update_flash_X550");

	buffer.cmd = FW_SHADOW_RAM_DUMP_CMD;
	buffer.buf_len1 = 0;
	buffer.buf_len2 = FW_SHADOW_RAM_DUMP_LEN;
	buffer.checksum = FW_DEFAULT_CHECKSUM;

	status = ixgbe_host_interface_command(hw, (u32 *)&buffer,
					      sizeof(buffer), false);

	return status;
}

/**
 *  ixgbe_get_supported_physical_layer_X550em - Returns physical layer type
 *  @hw: pointer to hardware structure
 *
 *  Determines physical layer capabilities of the current configuration.
 **/
u32 ixgbe_get_supported_physical_layer_X550em(struct ixgbe_hw *hw)
{
	u32 physical_layer = IXGBE_PHYSICAL_LAYER_UNKNOWN;

	DEBUGFUNC("ixgbe_get_supported_physical_layer_X550em");

	hw->phy.ops.identify(hw);

	switch (hw->phy.type) {
	case ixgbe_phy_x550em_kr:
		physical_layer = IXGBE_PHYSICAL_LAYER_10GBASE_KR |
				 IXGBE_PHYSICAL_LAYER_1000BASE_KX;
		break;
	case ixgbe_phy_x550em_kx4:
		physical_layer = IXGBE_PHYSICAL_LAYER_10GBASE_KX4 |
				 IXGBE_PHYSICAL_LAYER_1000BASE_KX;
		break;
	default:
		break;
	}

	if (hw->mac.ops.get_media_type(hw) == ixgbe_media_type_fiber)
		physical_layer = ixgbe_get_supported_phy_sfp_layer_generic(hw);

	return physical_layer;
}

/**
 * ixgbe_disable_rx_x550 - Disable RX unit
 *
 * Enables the Rx DMA unit for x550
 **/
void ixgbe_disable_rx_x550(struct ixgbe_hw *hw)
{
	u32 rxctrl, pfdtxgswc;
	s32 status;
	struct ixgbe_hic_disable_rxen fw_cmd;

	DEBUGFUNC("ixgbe_enable_rx_dma_x550");

	rxctrl = IXGBE_READ_REG(hw, IXGBE_RXCTRL);
	if (rxctrl & IXGBE_RXCTRL_RXEN) {
		pfdtxgswc = IXGBE_READ_REG(hw, IXGBE_PFDTXGSWC);
		if (pfdtxgswc & IXGBE_PFDTXGSWC_VT_LBEN) {
			pfdtxgswc &= ~IXGBE_PFDTXGSWC_VT_LBEN;
			IXGBE_WRITE_REG(hw, IXGBE_PFDTXGSWC, pfdtxgswc);
			hw->mac.set_lben = true;
		} else {
			hw->mac.set_lben = false;
		}

		fw_cmd.hdr.cmd = FW_DISABLE_RXEN_CMD;
		fw_cmd.hdr.buf_len = FW_DISABLE_RXEN_LEN;
		fw_cmd.hdr.checksum = FW_DEFAULT_CHECKSUM;
		fw_cmd.port_number = hw->phy.lan_id;

		status = ixgbe_host_interface_command(hw, (u32 *)&fw_cmd,
					sizeof(struct ixgbe_hic_disable_rxen),
					true);

		/* If we fail - disable RX using register write */
		if (status) {
			rxctrl = IXGBE_READ_REG(hw, IXGBE_RXCTRL);
			if (rxctrl & IXGBE_RXCTRL_RXEN) {
				rxctrl &= ~IXGBE_RXCTRL_RXEN;
				IXGBE_WRITE_REG(hw, IXGBE_RXCTRL, rxctrl);
			}
		}
	}
}