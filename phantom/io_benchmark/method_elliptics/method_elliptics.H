// This file is part of the phantom::io_benchmark module.
// Copyright (C) 2013, Ruslan Nigmatullin <euroelessar@yandex.ru>.
// Copyright (C) 2013, YANDEX LLC.
// This module may be distributed under the terms of the GNU LGPL 2.1.
// See the file ‘COPYING’ or ‘http://www.gnu.org/licenses/lgpl-2.1.html’.

#pragma once

#include <phantom/io_benchmark/method.H>

#include <pd/base/fd.H>
#include <pd/base/netaddr.H>
#include <pd/base/config.H>
#include <pd/base/config_list.H>

#pragma GCC visibility push(default)

namespace phantom { namespace io_benchmark {

class method_elliptics_t : public method_t
{
public:

private:
	virtual bool test(stat_t &stat) const;
	virtual void init();
	virtual void stat(out_t &out, bool clear, bool hrr_flag) const;
	virtual void fini();
	virtual size_t maxi() const throw();
	virtual descr_t const *descr(size_t ind) const throw();

public:
	struct config_t {
		config::list_t<string_t> remotes;
		config::list_t<sizeval_t> groups;

		config_t() throw();
		void check(in_t::ptr_t const &ptr) const;
		inline ~config_t() throw() { }
	};

	method_elliptics_t(string_t const &, config_t const &);
	~method_elliptics_t() throw();
};

}} // namespace phantom::io_benchmark

#pragma GCC visibility pop
