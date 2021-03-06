#!/usr/bin/env python
#############################################################################
# Copyright (c) 2015-2019 Balabit
#
# This program is free software; you can redistribute it and/or modify it
# under the terms of the GNU General Public License version 2 as published
# by the Free Software Foundation, or (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
#
# As an additional exemption you are allowed to compile & link against the
# OpenSSL libraries as published by the OpenSSL project. See the file
# COPYING for details.
#
#############################################################################


def write_dummy_message(tc, file_source):
    bsd_message = tc.new_log_message()
    bsd_log = tc.format_as_bsd(bsd_message)
    file_source.write_log(bsd_log)
    return bsd_message


def test_flags_catch_all(tc):
    # Check the correct output if the logpath is the following
    # log {
    #     source(s_file);
    #     log { destination(d_file1); };
    # };
    # log { destination(d_file2); flags(catch-all);};
    # input logs:
    # Oct 11 22:14:15 host-A testprogram: message from host-A

    config = tc.new_config()

    file_source = config.create_file_source(file_name="input.log")
    file_destination = config.create_file_destination(file_name="output.log")
    catch_all_destination = config.create_file_destination(file_name="catchall_output.log")

    inner_logpath = config.create_inner_logpath(statements=[file_destination])

    config.create_logpath(statements=[file_source, inner_logpath])
    config.create_logpath(statements=[catch_all_destination], flags="catch-all")

    bsd_message = write_dummy_message(tc, file_source)

    expected_bsd_message = bsd_message.remove_priority()

    syslog_ng = tc.new_syslog_ng()
    syslog_ng.start(config)

    destination_logs = file_destination.read_log()
    # message should arrived into destination1
    assert tc.format_as_bsd(expected_bsd_message) in destination_logs

    catch_all_destination_logs = catch_all_destination.read_log()
    # message should arrived into catch_all_destination
    # there is a flags(catch-all)
    assert tc.format_as_bsd(expected_bsd_message) in catch_all_destination_logs
