/*
 * \brief  RK3399 Board driver for bootstrap
 * \author Michael Grunditz
 * \date   2024-08-24
 */

/*
 * Copyright (C) 2021 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU Affero General Public License version 3.
 */

#ifndef _BOOTSTRAP__SPEC__RK3399__BOARD_H_
#define _BOOTSTRAP__SPEC__RK3399__BOARD_H_

#include <hw/spec/arm_64/rk3399_board.h>
#include <hw/spec/arm_64/cpu.h>
#include <hw/spec/arm/gicv3.h>
#include <hw/spec/arm/lpae.h>

namespace Board {

	using namespace Hw::rk3399_board;

	struct Cpu : Hw::Arm_64_cpu
	{
		static void wake_up_all_cpus(void*);
	};

	using Pic = Hw::Pic;
	static constexpr bool NON_SECURE = true;
};

#endif /* _BOOTSTRAP__SPEC__RK3399_BOARD_H_ */
