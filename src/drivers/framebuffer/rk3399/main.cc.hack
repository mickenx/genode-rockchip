/*
 * \brief  Frame-buffer driver for the i.MX53
 * \author Stefan Kalkowski
 * \author Norman Feske
 * \date   2012-06-21
 */

/*
 * Copyright (C) 2012-2022 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU Affero General Public License version 3.
 */

/* Genode includes */
#include <base/attached_rom_dataspace.h>
#include <base/component.h>
#include <base/log.h>
#include <capture_session/connection.h>
#include <dataspace/client.h>
#include <platform_session/connection.h>
#include <platform_session/dma_buffer.h>
#include <platform_session/device.h>
#include <timer_session/connection.h>
#include <base/attached_io_mem_dataspace.h>

/* local includes */
//#include <ipu.h>

namespace Framebuffer {
	using namespace Genode;
	struct Main;
};


struct Framebuffer::Main
{
	using Pixel = Capture::Pixel;

	static unsigned _display(Xml_node node) {
		return node.attribute_value<unsigned>("display", 0U); }

	static size_t _width(Xml_node node) {
		return node.attribute_value<size_t>("width", 1920UL); }

	static size_t _height(Xml_node node) {
		return node.attribute_value<size_t>("height", 1080UL); }

	Env &                       _env;
	Attached_rom_dataspace      _config   { _env, "config" };
	unsigned                    _disp     { _display(_config.xml()) };
	Capture::Area const         _size     { 1920,
	                                        1080 };
	Platform::Connection        _platform { _env      };
	Platform::Device            _device   { _platform };
	Platform::Dma_buffer        _fb_buf   { _platform,
	                                        _size.count()*sizeof(Pixel),
	                                        CACHED };
	//Ipu                         _ipu      { _device   };
	Capture::Connection         _capture  { _env };
	Capture::Connection::Screen _captured_screen { _capture, _env.rm(), _size };
	Timer::Connection           _timer { _env };
	Signal_handler<Main>        _timer_handler { _env.ep(), *this,
	                                             &Main::_handle_timer };

	void _handle_timer()
	{
		Surface<Pixel> surface(_fb_buf.local_addr<Pixel>(), _size);
		_captured_screen.apply_to_surface(surface);
	}

	enum Resolutions { BYTES_PER_PIXEL  = 4 };

	Main(Env &env) : _env(env)
	{
		addr_t vop=0xff8f0000; //0xff900000;
		void * vopptr;
		Attached_io_mem_dataspace _vop_ds { env, vop,
	                                   64*1024, true };

		log("--- rk3399 fb driver ---");
		vopptr=(void*)_vop_ds.local_addr<void>();
	     *((uint32_t*)vopptr+0x0040)=_fb_buf.dma_addr();
	     *((uint32_t*)vopptr)=1;
		//_ipu.init(_size.w(), _size.h(), _size.w() * BYTES_PER_PIXEL,
		 //         _fb_buf.dma_addr(), _disp == 0);

		_timer.sigh(_timer_handler);
		_timer.trigger_periodic(10*1000);
	}
};


void Component::construct(Genode::Env &env) { static Framebuffer::Main main(env); }
