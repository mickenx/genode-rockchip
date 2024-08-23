/*
 * \brief  RK3399 board driver for core
 * \author Michael Grunditz
 * \date   2024-08-24
 */

/*
 * Copyright (C) 2021 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU Affero General Public License version 3.
 */

#ifndef _CORE__SPEC__RK3399__BOARD_H_
#define _CORE__SPEC__RK3399__BOARD_H_

#include <hw/spec/arm_64/rk3399_board.h>
#include <spec/arm/generic_timer.h>
#include <spec/arm/virtualization/gicv3.h>
#include <spec/arm_v8/cpu.h>
#include <spec/arm/virtualization/board.h>

namespace Board {

	using namespace Hw::rk3399_board;

	enum {
		TIMER_IRQ           = 14 + 16,
		VT_TIMER_IRQ        = 11 + 16,
		VT_MAINTAINANCE_IRQ = 9  + 16,
		VCPU_MAX            = 16
	};
};

#endif /* _CORE__SPEC__RK3399__BOARD_H_ */
