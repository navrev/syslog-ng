/*
 * Copyright (c) 2002-2013 Balabit
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 * As an additional exemption you are allowed to compile & link against the
 * OpenSSL libraries as published by the OpenSSL project. See the file
 * COPYING for details.
 *
 */
#ifndef HOST_RESOLVE_H_INCLUDED
#define HOST_RESOLVE_H_INCLUDED 1

#include "syslog-ng.h"
#include "gsockaddr.h"
#include "mainloop-io-worker.h"
#include "mainloop-worker.h"

typedef struct _HostResolveOptions
{
  gboolean use_dns;
  gboolean use_fqdn;
  gboolean use_dns_cache;
  gboolean normalize_hostnames;
} HostResolveOptions;


typedef enum {
  ADDR_SYNC = 0,
  ADDR_GO_ASYNC,
  ADDR_INPROGRESS,
  ADDR_SUCCESS,
  ADDR_FAILURE
} AddrResolverState;

typedef struct _AddrResolver
{
  GAtomicCounter      ret;
  gchar               *hostname;
  GSockAddr           *addr;
  gint                family;
  AddrResolverState   state;
  MainLoopIOWorkerJob dns_job;
} AddrResolver;

#ifdef SYSLOG_NG_ENABLE_DNS_ASYNC
void dns_resolver_init(AddrResolver **self);
void dns_resolver_deinit(AddrResolver **self);
#endif

/* name resolution */
const gchar *resolve_sockaddr_to_hostname(gsize *result_len, GSockAddr *saddr,
                                          const HostResolveOptions *host_resolve_options);
gboolean resolve_hostname_to_sockaddr_v1(GSockAddr **addr, gint family, const gchar *name, AddrResolver *resolv);
gboolean resolve_hostname_to_sockaddr(GSockAddr **addr, gint family, const gchar *name);
const gchar *resolve_hostname_to_hostname(gsize *result_len, const gchar *hostname, HostResolveOptions *options);

void host_resolve_options_defaults(HostResolveOptions *options);
void host_resolve_options_global_defaults(HostResolveOptions *options);
void host_resolve_options_init_globals(HostResolveOptions *options);
void host_resolve_options_init(HostResolveOptions *options, HostResolveOptions *global_options);
void host_resolve_options_destroy(HostResolveOptions *options);

#endif
