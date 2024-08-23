/*
 * \brief  Board definitions for rk3399
 * \author Michael Grunditz
 * \date   2024-08-24
 */

/*
 * Copyright (C) 2021 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU Affero General Public License version 3.
 */

#ifndef _SRC__INCLUDE__HW__SPEC__ARM_64__RK3399__BOARD_H_
#define _SRC__INCLUDE__HW__SPEC__ARM_64__RK3399__BOARD_H_

#include <drivers/uart/rockchip.h>
#include <hw/spec/arm/boot_info.h>

namespace Hw::rk3399_board {

	using Serial = Genode::rockchip_uart;

	enum {
		RAM_BASE   = 0x10000000,
		RAM_SIZE   = 0xC0000000,

		UART_BASE  = 0xff1a0000,
		UART_SIZE  = 0x10000,
		UART_CLOCK = 0,
	};

	namespace Cpu_mmio {
		enum {
			IRQ_CONTROLLER_DISTR_BASE   = 0xfee00000,
			IRQ_CONTROLLER_DISTR_SIZE   = 0x100000,
			IRQ_CONTROLLER_REDIST_BASE = 0xfef00000,
			IRQ_CONTROLLER_REDIST_SIZE = 0x100000,
		};
	};
	static constexpr Genode::size_t NR_OF_CPUS = 1;
};

#endif /* _SRC__INCLUDE__HW__SPEC__ARM_64__RK3399__BOARD_H_ */
