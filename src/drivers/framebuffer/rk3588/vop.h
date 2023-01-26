/*
 * \brief  VOP registers
 * \author Nikolay Golikov  <nik@ksyslabs.org>
 * \author Stefan Kalkowski
 * \author Michael Grunditz
 * \date   2023-01-26
 */

/*
 * Copyright (C) 2009-2022 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU Affero General Public License version 3.
 */

#ifndef _DRIVERS__FRAMEBUFFER__RK3588__VOP_H_
#define _DRIVERS__FRAMEBUFFER__RK3588__VOP_H_

#include <platform_session/device.h>

namespace Framebuffer { struct Vop; }


struct Framebuffer::Vop : Platform::Device::Mmio
{
	


	/***********************************
	 **  Display interface registers  **
	 ***********************************/

	template <Genode::off_t OFF>
	struct emstart
	{
		struct cfgdone        : Register<OFF+0x0,   32> { };
		struct dsp_info        : Register<OFF+0x1824,   32> { };
		struct fb     : Register<OFF+0x1814,   32> { };
		
	};

	typedef emstart<0> Di0;


	unsigned int getwidth()
	{
		Genode::log("getwidth: ",((read<Di0::dsp_info>())&0x1fff)+1);
		return((read<Di0::dsp_info>()&0x1fff)+1);

	}

	unsigned int  getheight()
	{

		Genode::log("getheight: ",((read<Di0::dsp_info>()>>16)&0x1fff)+1);
		return(((read<Di0::dsp_info>()>>16)&0x1fff)+1);

	}

	
		

	/**
	 * IPU initialization
	 */
	void init( Genode::addr_t phys_base)
	          
	{
		/* stop pixel clocks */
		Genode::log("fb init: ",read<Di0::fb>());
		Genode::log("fb cfg: ",read<Di0::cfgdone>());
		write<Di0::fb>(phys_base);
		write<Di0::cfgdone>(0xFFFFC0FF);
		
	}


	
	using Platform::Device::Mmio::Mmio;
};

#endif /* _DRIVERS__FRAMEBUFFER__RK3588__VOP_H_ */
