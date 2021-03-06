/* -*- mode: C; c-file-style: "gnu" -*- */
/* dbus-sysdeps-util.c Tests for dbus-sysdeps.h API
 * 
 * Copyright (C) 2002, 2003, 2004, 2005  Red Hat, Inc.
 * Copyright (C) 2003 CodeFactory AB
 *
 * Licensed under the Academic Free License version 2.1
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */
#include "dbus-sysdeps.h"
#include "dbus-internals.h"
#include "dbus-string.h"
#include "dbus-test.h"

#ifdef DBUS_BUILD_TESTS
#include <stdlib.h>
static void
check_dirname (const char *filename,
               const char *dirname)
{
  DBusString f, d;
  
  _dbus_string_init_const (&f, filename);

  if (!_dbus_string_init (&d))
    _dbus_assert_not_reached ("no memory");

  if (!_dbus_string_get_dirname (&f, &d))
    _dbus_assert_not_reached ("no memory");

  if (!_dbus_string_equal_c_str (&d, dirname))
    {
      _dbus_warn ("For filename \"%s\" got dirname \"%s\" and expected \"%s\"\n",
                  filename,
                  _dbus_string_get_const_data (&d),
                  dirname);
      exit (1);
    }

  _dbus_string_free (&d);
}

static void
check_path_absolute (const char *path,
                     dbus_bool_t expected)
{
  DBusString p;

  _dbus_string_init_const (&p, path);

  if (_dbus_path_is_absolute (&p) != expected)
    {
      _dbus_warn ("For path \"%s\" expected absolute = %d got %d\n",
                  path, expected, _dbus_path_is_absolute (&p));
      exit (1);
    }
}

/**
 * Unit test for dbus-sysdeps.c.
 * 
 * @returns #TRUE on success.
 */
dbus_bool_t
_dbus_sysdeps_test (void)
{
  DBusString str;
  double val;
  int pos;
  
  check_dirname ("foo", ".");
  check_dirname ("foo/bar", "foo");
  check_dirname ("foo//bar", "foo");
  check_dirname ("foo///bar", "foo");
  check_dirname ("foo/bar/", "foo");
  check_dirname ("foo//bar/", "foo");
  check_dirname ("foo///bar/", "foo");
  check_dirname ("foo/bar//", "foo");
  check_dirname ("foo//bar////", "foo");
  check_dirname ("foo///bar///////", "foo");
  check_dirname ("/foo", "/");
  check_dirname ("////foo", "/");
  check_dirname ("/foo/bar", "/foo");
  check_dirname ("/foo//bar", "/foo");
  check_dirname ("/foo///bar", "/foo");
  check_dirname ("/", "/");
  check_dirname ("///", "/");
  check_dirname ("", ".");  


  _dbus_string_init_const (&str, "3.5");
  if (!_dbus_string_parse_double (&str,
				  0, &val, &pos))
    {
      _dbus_warn ("Failed to parse double");
      exit (1);
    }
  if (ABS(3.5 - val) > 1e-6)
    {
      _dbus_warn ("Failed to parse 3.5 correctly, got: %f", val);
      exit (1);
    }
  if (pos != 3)
    {
      _dbus_warn ("_dbus_string_parse_double of \"3.5\" returned wrong position %d", pos);
      exit (1);
    }

  _dbus_string_init_const (&str, "0xff");
  if (!_dbus_string_parse_double (&str,
				  0, &val, &pos))
    {
      _dbus_warn ("Failed to parse double");
      exit (1);
    }
  if (ABS (0xff - val) > 1e-6)
    {
      _dbus_warn ("Failed to parse 0xff correctly, got: %f\n", val);
      exit (1);
    }
  if (pos != 4)
    {
      _dbus_warn ("_dbus_string_parse_double of \"0xff\" returned wrong position %d", pos);
      exit (1);
    }
  
  check_path_absolute ("/", TRUE);
  check_path_absolute ("/foo", TRUE);
  check_path_absolute ("", FALSE);
  check_path_absolute ("foo", FALSE);
  check_path_absolute ("foo/bar", FALSE);
  
  return TRUE;
}
#endif /* DBUS_BUILD_TESTS */
