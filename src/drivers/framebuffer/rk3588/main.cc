/*
 * \brief  Frame-buffer driver for the rk3588
 * \author Stefan Kalkowski
 * \author Norman Feske
 * \author Michael Grunditz
 * \date   2023-01-26
 */

/*
 * Copyright (C) 2012-2022 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU Affero General Public License version 3.
 */

/* Genode includes */
#include <base/attached_rom_dataspace.h>
#include <base/attached_io_mem_dataspace.h>
#include <base/component.h>
#include <base/log.h>
#include <capture_session/connection.h>
#include <dataspace/client.h>
#include <platform_session/connection.h>
#include <platform_session/dma_buffer.h>
#include <platform_session/device.h>
#include <timer_session/connection.h>
#include <cpu/cache.h>

/* local includes */
#include <vop.h>

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
		return node.attribute_value<unsigned>("width", 3840UL); }

	static size_t _height(Xml_node node) {
		return node.attribute_value<unsigned>("height", 2160UL); }

	Env &                       _env;
	Platform::Connection        _platform { _env      };
	Platform::Device            _device   { _platform };
	Vop                         _vop      { _device  };
	Capture::Area const         _size     {(_vop.getwidth()),
	                                        (_vop.getheight()) };
	
	
	Platform::Dma_buffer        _fb_buf   { _platform,
	                                        _size.count()*sizeof(Pixel),
	                                        CACHED };
	
	
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
			log("--- rk3588 fb driver ---");
		
		     _vop.init(_fb_buf.dma_addr());
		_timer.sigh(_timer_handler);
		_timer.trigger_periodic(10*1000);
	}
};


void Component::construct(Genode::Env &env) { 
	static Framebuffer::Main main(env); }
