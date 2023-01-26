/*
 * \brief  Startup USB driver library
 * \author Sebastian Sumpf
 * \date   2013-02-20
 */

/*
 * Copyright (C) 2013-2017 Genode Labs GmbH
 *
 * This file is distributed under the terms of the GNU General Public License
 * version 2.
 */

/* Genode */
#include <base/component.h>
#include <base/attached_rom_dataspace.h>
#include <base/sleep.h>

/* Local */
#include <signal.h>
#include <lx_emul.h>

#include <legacy/lx_kit/env.h>
#include <legacy/lx_kit/pci.h>
#include <legacy/lx_kit/irq.h>
#include <legacy/lx_kit/malloc.h>
#include <legacy/lx_kit/scheduler.h>
#include <legacy/lx_kit/timer.h>
#include <legacy/lx_kit/work.h>


using namespace Genode;

extern "C" int  subsys_usb_init();
extern "C" void module_raw_driver_init();
extern "C" void start_input_service(void *ep, void *services);

struct workqueue_struct *system_power_efficient_wq;
struct workqueue_struct *system_wq;
struct workqueue_struct *tasklet_wq;

void breakpoint() { Genode::log("BREAK"); }


static void run_linux(void *s)
{
	Services *services = (Services *)s;

	system_power_efficient_wq = alloc_workqueue("system_power_efficient_wq", 0, 0);
	system_wq                 = alloc_workqueue("system_wq", 0, 0);
	tasklet_wq                = alloc_workqueue("tasklet_wq", 0, 0);

	/* low level interface */
	module_raw_driver_init();

	/* USB */
	subsys_usb_init();

	/* host controller */
	platform_hcd_init(Lx_kit::env().env(), services);

	while (true)
		Lx::scheduler().current()->block_and_schedule();
}


static void start_usb_driver(Genode::Env &env)
{
	/* initialize USB env */
	Lx_kit::construct_env(env);

	LX_MUTEX_INIT(init_usb_class_mutex);
	LX_MUTEX_INIT(usb_bus_idr_lock);
	LX_MUTEX_INIT(usb_port_peer_mutex);
	LX_MUTEX_INIT(usbfs_mutex);

	/* sets up backend alloc needed by malloc */
	backend_alloc_init(env, env.ram(), Lx_kit::env().heap());

	Lx::malloc_init(env, Lx_kit::env().heap());

	static Services services(env);

	Raw::init(env, services.raw_report_device_list);

	Lx::scheduler(&env);
	Lx::timer(&env, &env.ep(), &Lx_kit::env().heap(), &jiffies);

	Lx::Irq::irq(&env.ep(), &Lx_kit::env().heap());
	Lx::Work::work_queue(&Lx_kit::env().heap());

	static Lx::Task linux(run_linux, &services, "linux", Lx::Task::PRIORITY_0,
	                      Lx::scheduler());

	Lx::scheduler().schedule();
}

namespace Usb_driver {

	using namespace Genode;

	struct Driver_starter { virtual void start_driver() = 0; };
	struct Main;
}


struct Usb_driver::Main : Driver_starter
{
	Env &_env;

	/*
	 * Defer the startup of the USB driver until the first configuration
	 * becomes available. This is needed in scenarios where the configuration
	 * is dynamically generated and supplied to the USB driver via the
	 * report-ROM service.
	 */
	struct Initial_config_handler
	{
		Driver_starter &_driver_starter;

		Attached_rom_dataspace _config;

		Signal_handler<Initial_config_handler> _config_handler;

		void _handle_config()
		{
			_config.update();

			if (_config.xml().type() == "config")
				_driver_starter.start_driver();
		}

		Initial_config_handler(Env &env, Driver_starter &driver_starter)
		:
			_driver_starter(driver_starter),
			_config(env, "config"),
			_config_handler(env.ep(), *this, &Initial_config_handler::_handle_config)
		{
			_config.sigh(_config_handler);
			_handle_config();
		}
	};

	void _handle_start()
	{
		if (_initial_config_handler.constructed()) {
			_initial_config_handler.destruct();
			start_usb_driver(_env);
		}
	}

	Signal_handler<Main> _start_handler {
		_env.ep(), *this, &Main::_handle_start };

	Reconstructible<Initial_config_handler> _initial_config_handler { _env, *this };

	/*
	 * Called from 'Initial_config_handler'
	 */
	void start_driver() override
	{
		Signal_transmitter(_start_handler).submit();
	}

	Main(Env &env) : _env(env) { }
};


void Component::construct(Genode::Env &env)
{
	/* XXX execute constructors of global statics */
	env.exec_static_constructors();

	static Usb_driver::Main main(env);
}
