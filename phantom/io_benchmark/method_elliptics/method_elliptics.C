// This file is part of the phantom::io_benchmark module.
// Copyright (C) 2013, Ruslan Nigmatullin <euroelessar@yandex.ru>.
// Copyright (C) 2013, YANDEX LLC.
// This module may be distributed under the terms of the GNU LGPL 2.1.
// See the file ‘COPYING’ or ‘http://www.gnu.org/licenses/lgpl-2.1.html’.

#include "method_elliptics.H"
#include "source.H"
#include "../method_stream/logger.H"

#include "../../module.H"

#include <pd/bq/bq_util.H>
#include <pd/base/exception.H>

namespace phantom {
namespace io_benchmark {

MODULE(io_benchmark_method_elliptics);

method_elliptics_t::config_t::config_t() throw() :
	logger_filename(STRING("/dev/null")) {
}

void method_elliptics_t::config_t::check(const in_t::ptr_t &ptr) const {
	if (!remotes)
		config::error(ptr, "remotes is empty list");

	if (!source)
		config::error(ptr, "source is required");
}

void method_elliptics_t::loggers_t::commit(
	in_segment_t const &request, in_segment_t &tag, result_t const &res
) const {
	for(size_t i = 0; i < size; ++i) {
		logger_t *logger = items[i];
		if(res.log_level >= logger->level)
			logger->commit(request, tag, res);
	}
}

namespace method_elliptics {
config_binding_sname(method_elliptics_t);
config_binding_value(method_elliptics_t, remotes);
config_binding_type(method_elliptics_t, source_t);
config_binding_value(method_elliptics_t, source);
config_binding_value(method_elliptics_t, logger_filename);
config_binding_type(method_elliptics_t, logger_t);
config_binding_value(method_elliptics_t, loggers);
config_binding_ctor(method_t, method_elliptics_t);
}

static ioremap::elliptics::logger create_logger(const method_elliptics_t::config_t &config) {
	MKCSTR(logger_filename, config.logger_filename);
	if (strcmp(logger_filename, "/dev/null") == 0)
		return method_elliptics_t::elliptics_logger_t(0, DNET_LOG_ERROR);
	else
		return method_elliptics_t::elliptics_file_logger_t(logger_filename, DNET_LOG_ERROR);
}

method_elliptics_t::method_elliptics_t(const string_t &, const config_t &config) :
	method_t(), logger(create_logger(config)), node(logger),
	source(*config.source) {

	dnet_config cfg;
	memset(&cfg, 0, sizeof(cfg));

	for(typeof(config.remotes.ptr()) rptr = config.remotes; rptr; ++rptr) {
		MKCSTR(remote, rptr.val());
		node.add_remote(remote);
	}
}

method_elliptics_t::~method_elliptics_t() throw() {
}

static inline std::string make_string(in_segment_t &in)
{
	std::string string;
	string.resize(in.size());
	out_t(&string[0], string.size())(in);
	return string;
}

bool method_elliptics_t::test(stat_t &stat) const
{
	request_t request;
	if (!source.get_request(request))
		return false;

	try {
		stat_t::tcount_guard_t tcount_guard(stat);

		elliptics_session_t session(node);

		session.set_cflags(request.cflags);
		session.set_ioflags(request.ioflags);
		session.set_groups(request.groups);

		ioremap::elliptics::key id(make_string(request.filename));

		switch (request.command) {
		case method_elliptics::write_data:
			session.write_data(id, make_string(request.data), request.offset);
			break;
		case method_elliptics::read_data:
			session.read_data(id, request.offset, request.size);
			break;
		case method_elliptics::remove_data:
			session.remove(id);
			break;
		}
	} catch (const ioremap::elliptics::error &e) {
		throw exception_sys_t(log::error, e.error_code(), "%s", e.what());
	}

	return true;
}

void method_elliptics_t::init()
{
	source.init();
}

void method_elliptics_t::stat(out_t &out, bool clear, bool hrr_flag) const
{
	source.stat(out, clear, hrr_flag);
}

void method_elliptics_t::fini()
{
	source.fini();
}

size_t method_elliptics_t::maxi() const throw()
{
	return 0;
}

class network_descr_t : public descr_t {
	static size_t const max_errno = 140;

	virtual size_t value_max() const { return max_errno; }

	virtual void print_header(out_t &out) const {
		out(CSTR("network"));
	}

	virtual void print_value(out_t &out, size_t value) const {
		if(value < max_errno) {
			char buf[128];
			char *res = strerror_r(value, buf, sizeof(buf));
			buf[sizeof(buf) - 1] = '\0';
			out(str_t(res, strlen(res)));
		}
		else
			out(CSTR("Unknown error"));
	}
public:
	inline network_descr_t() throw() : descr_t() { }
	inline ~network_descr_t() throw() { }
};

static network_descr_t const network_descr;

const descr_t *method_elliptics_t::descr(size_t ind) const throw()
{
	(void) ind;
	return &network_descr;
}

} // namespace io_benchmark
} // namespace phantom
