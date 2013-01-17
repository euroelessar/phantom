// This file is part of the phantom::io_benchmark module.
// Copyright (C) 2013, Ruslan Nigmatullin <euroelessar@yandex.ru>.
// Copyright (C) 2013, YANDEX LLC.
// This module may be distributed under the terms of the GNU LGPL 2.1.
// See the file ‘COPYING’ or ‘http://www.gnu.org/licenses/lgpl-2.1.html’.

#include "method_elliptics.H"

#include "../../module.H"

namespace phantom {
namespace io_benchmark {

MODULE(io_benchmark_method_elliptics);

method_elliptics_t::config_t::config_t() throw() {
}

void method_elliptics_t::config_t::check(const in_t::ptr_t &ptr) const {
	if (!remotes)
		config::error(ptr, "remotes is empty list");

	if (!groups)
		config::error(ptr, "groups is empty list");
}

namespace method_elliptics {
config_binding_sname(method_elliptics_t);
config_binding_value(method_elliptics_t, remotes);
config_binding_value(method_elliptics_t, groups);
config_binding_ctor(method_t, method_elliptics_t);
}

method_elliptics_t::method_elliptics_t(const string_t &, const config_t &) :
	method_t() {
}

method_elliptics_t::~method_elliptics_t() throw() {
}

bool method_elliptics_t::test(stat_t &stat) const
{
	(void) stat;
	return false;
}

void method_elliptics_t::init()
{
}

void method_elliptics_t::stat(out_t &out, bool clear, bool hrr_flag) const
{
	(void) out;
	(void) clear;
	(void) hrr_flag;
}

void method_elliptics_t::fini()
{
}

size_t method_elliptics_t::maxi() const throw()
{
	return 0;
}

const descr_t *method_elliptics_t::descr(size_t ind) const throw()
{
	(void) ind;
	return 0;
}

} // namespace io_benchmark
} // namespace phantom
