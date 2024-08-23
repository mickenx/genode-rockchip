/*
 * \brief  Frame-buffer driver for the RK3399 ( when started from u-boot)
 * \author Michael Grunditz
 * \author Stefan Kalkowski
 * \author Norman Feske
 * \date   2024-08-24
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
#include <framebuffer_session/framebuffer_session.h>
#include <cpu/cache.h>
#include <os/pixel_rgb565.h>

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
		return node.attribute_value<unsigned>("width", 1920UL); }

	static size_t _height(Xml_node node) {
		return node.attribute_value<unsigned>("height", 1080UL); }

	Env &                       _env;
	Platform::Connection        _platform { _env      };
	Platform::Device            _device   { _platform };
	Vop                         _vop      { _device  };
		
	Capture::Area const         _size     {(1920),
	                                        (1080) };
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

	enum Resolutions { BYTES_PER_PIXEL  = 2 };

	Main(Env &env) : _env(env)
	{
	log("--- rk3399 fb driver ---");
        _vop.init(_fb_buf.dma_addr(),env);
	_timer.sigh(_timer_handler);
	_timer.trigger_periodic(10*1000);
	}
};


void Component::construct(Genode::Env &env) { 
	static Framebuffer::Main main(env); }
