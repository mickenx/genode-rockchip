/*
 * \brief  Board definitions for rk3588
 * \author Michael Grunditz
 * \date   2023-01-26
 */

/*
 * Copyright (C) 2021 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU Affero General Public License version 3.
 */

#ifndef _SRC__INCLUDE__HW__SPEC__ARM_64__RK3588__BOARD_H_
#define _SRC__INCLUDE__HW__SPEC__ARM_64__RK3588__BOARD_H_

#include <drivers/uart/rockchip.h>
#include <hw/spec/arm/boot_info.h>

namespace Hw::rk3588_board {

	using Serial = Genode::rockchip_uart;

	enum {
		RAM_BASE   = 0x00200000,
		RAM_SIZE   = 0x80000000,

		UART_BASE  = 0xfeb50000,
		UART_SIZE  = 0x1000,
		UART_CLOCK = 0,
		NR_OF_CPUS = 4,
	};

	namespace Cpu_mmio {
		enum {
			IRQ_CONTROLLER_DISTR_BASE   = 0xfe600000,
			IRQ_CONTROLLER_DISTR_SIZE   = 0x1000,
			IRQ_CONTROLLER_REDIST_BASE = 0xfe680000,
			IRQ_CONTROLLER_REDIST_SIZE = 0x100000,
/*			IRQ_CONTROLLER_CPU_BASE     = 0x01c82000,
			IRQ_CONTROLLER_CPU_SIZE     = 0x2000,
			IRQ_CONTROLLER_VT_CTRL_BASE = 0xbbbbb000,
			IRQ_CONTROLLER_VT_CPU_BASE  = 0x31020000,
			IRQ_CONTROLLER_VT_CPU_SIZE  = 0x2000,*/
		};
	};
};

#endif /* _SRC__INCLUDE__HW__SPEC__ARM_64__RK3588__BOARD_H_ */
