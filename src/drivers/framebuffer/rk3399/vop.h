/*
 * \brief  RK3399 VOP ( visual output processor)
 * \author Michael Grunditz
 * \author Stefan Kalkowski
 * \date   2024-08-24
 */

/*
 * Copyright (C) 2009-2022 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU Affero General Public License version 3.
 */

#ifndef _DRIVERS__FRAMEBUFFER__RK3399__VOP_H_
#define _DRIVERS__FRAMEBUFFER__RK3399__VOP_H_

#include <platform_session/device.h>
#include <timer_session/connection.h>

namespace Framebuffer { struct Vop; }


struct Framebuffer::Vop : Platform::Device::Mmio<1920*1080*4>
{
	


	/***********************************
	 **  Display interface registers  **
	 ***********************************/

	template <Genode::off_t OFF>
	struct emstart
	{
		struct cfgdone        : Register<OFF+0x0,   32> { };
		struct fb     : Register<OFF+0x40,   32> { };
		struct vop_sys_ctrl        : Register<OFF+0x8,   32> { };
		struct vop_sys_ctrl1        : Register<OFF+0xc,   32> { };
		struct vop_dsp_ctrl0        : Register<OFF+0x10,   32> { };
		struct vop_dsp_ctrl1        : Register<OFF+0x14,   32> { };
		struct vop_dsp_bg        : Register<OFF+0x18,   32> { };
		struct vop_win0_ctr0        : Register<OFF+0x30,   32> { };
		struct vop_win0_ctr1        : Register<OFF+0x34,   32> { };
		struct vop_win0_vir        : Register<OFF+0x3c,   32> { };
		struct vop_win0_act_info        : Register<OFF+0x48,   32> { };
		struct vop_win0_dsp_info        : Register<OFF+0x4c,   32> { };
		struct vop_win0_dsp_st       : Register<OFF+0x50,   32> { };
		struct vop_dsp_htotal       : Register<OFF+0x188,   32> { };
		struct vop_dsp_hact_end       : Register<OFF+0x18c,   32> { };
		struct vop_dsp_vact_end       : Register<OFF+0x194,   32> { };
		struct vop_dsp_vtotal_end       : Register<OFF+0x190,   32> { };
		struct vop_dsp_htotal_end       : Register<OFF+0x188,   32> { };
	//	struct vop_win0_dsp_st       : Register<OFF+0x50,   32> { };
		struct vop_post_dsp_vat_info       : Register<OFF+0x174,   32> { };
		struct vop_post_dsp_hat_info       : Register<OFF+0x170,   32> { };
		//struct VOP_POST_DSP_VACT_INFO_F1       : Register<OFF+0x184,   32> { };

			struct dsp_info        : Register<OFF+0x1824,   32> { };
		
	};

	typedef emstart<0> Di0;
	//typedef Di<0x48000> Di1;


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
	void init( Genode::addr_t phys_base,Genode::Env &env)
	          
	{

	static Timer::Connection timer(env);
		Genode::log("fb init: ",read<Di0::fb>());
		Genode::log("fb cfg: ",read<Di0::cfgdone>());
		write<Di0::vop_dsp_htotal>(0x085e0020);

	
	write<Di0::vop_dsp_hact_end>(0x00ae082e);

     		write<Di0::vop_dsp_vtotal_end>(0x044c0006);
        
		write<Di0::vop_dsp_vact_end>(0x00110449);
        
		write<Di0::vop_post_dsp_hat_info>(0x00ae082e);

		write<Di0::vop_post_dsp_vat_info>(0x00110449);//5+36+1080);



		write<Di0::vop_dsp_ctrl0>(15);
		write<Di0::vop_dsp_vact_end>(0x00110449);//5+4+1080);
		write<Di0::vop_dsp_hact_end>(0x00ae082e);//(//5+4+1080);
		write<Di0::fb>(phys_base);
		write<Di0::cfgdone>(1);
		Genode::uint32_t sys_ctrl = read<Di0::vop_sys_ctrl>();
		sys_ctrl|=(1<<14);
		write<Di0::vop_sys_ctrl>(sys_ctrl);
		write<Di0::cfgdone>(1);
		write<Di0::vop_win0_vir>(1920); 
		write<Di0::vop_win0_act_info>(0x0437077f); 
		write<Di0::vop_win0_dsp_info>(0x0437077f);
		write<Di0::vop_win0_dsp_st>(0x001100ae);
		write<Di0::cfgdone>(1);
		write<Di0::fb>(0);
		write<Di0::fb>(phys_base);
		write<Di0::cfgdone>(1);
		Genode::uint32_t win0en = read<Di0::vop_win0_ctr0>();
		win0en=0x3A000081;
		Genode::log("win0en: ",Genode::Hex(win0en));
		write<Di0::vop_win0_ctr0>(win0en);
		write<Di0::cfgdone>(1);
		Genode::log("act info: ",Genode::Hex(read<Di0::vop_win0_act_info>()));
		Genode::log("fb init2: ",Genode::Hex(read<Di0::fb>()));
		Genode::log("fbset: ",Genode::Hex(phys_base));
		Genode::log("vop sys: ",Genode::Hex(read<Di0::vop_sys_ctrl>()));
		Genode::addr_t grf =0xff770000;
		Genode::Attached_io_mem_dataspace _grf_ds { env, grf,
	                                  64*1024, true };
		char* grfptr = (char*)_grf_ds.local_addr<char>();
		Genode::uint32_t grfset = *(grfptr+0x06520);
		grfset|=(0<<5);
		Genode::addr_t cru =0xff760000;
				Genode::Attached_io_mem_dataspace _cru_ds { env, cru,
	                                  64*1024, true };
		char* cruptr = (char*)_cru_ds.local_addr<char>();

		

		*(Genode::uint32_t*)((char*)cruptr+0x0444)=(1<<(16+8))|(1<<8);
		*(Genode::uint32_t*)((char*)cruptr+0x0444)=(1<<(16+8))|(0<<8);
		*(Genode::uint32_t*)((char*)grfptr+0x0e028)=(2<<(16+14)|(2<14));
		*(Genode::uint32_t*)((char*)grfptr+0x06250)=0xffff0249; //(1<<(16+5))|(0<<5);
		
		
	}


	
	using Platform::Device::Mmio<SIZE>::Mmio;
};

#endif /* _DRIVERS__FRAMEBUFFER__RK3399__VOP_H_ */
