/*
 * \brief  Board driver for bootstrap
 * \author Michael Grunditz
 * \date   2023-01-26
 */

/*
 * Copyright (C) 2021 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU Affero General Public License version 3.
 */

#ifndef _BOOTSTRAP__SPEC__RK3588__BOARD_H_
#define _BOOTSTRAP__SPEC__RK3588__BOARD_H_

#include <hw/spec/arm_64/rk3588_board.h>
#include <hw/spec/arm_64/cpu.h>
#include <hw/spec/arm/gicv3.h>
#include <hw/spec/arm/lpae.h>

namespace Board {

	using namespace Hw::rk3588_board;

	struct Cpu : Hw::Arm_64_cpu
	{
		static void wake_up_all_cpus(void*);
	};

	using Pic = Hw::Pic;
	static constexpr bool NON_SECURE = true;
};

#endif /* _BOOTSTRAP__SPEC__RK3588__BOARD_H_ */
