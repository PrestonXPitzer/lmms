/*
 * IoHelper.h - helper functions for file I/O
 *
 * Copyright (c) 2018 Hyunjin Song <tteu.ingog/at/gmail.com>
 *
 * This file is part of LMMS - https://lmms.io
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public
 * License along with this program (see COPYING); if not, write to the
 * Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301 USA.
 *
 */

#ifndef IO_HELPER_H
#define IO_HELPER_H

#include "lmmsconfig.h"

#include <cstdio>

#ifdef _WIN32
#include <windows.h>
#endif

#ifdef LMMS_BUILD_WIN32
#include <io.h>
#else
#ifdef LMMS_HAVE_UNISTD_H
#include <unistd.h>
#endif
#endif // LMMS_BUILD_WIN32

namespace lmms
{


#ifdef _WIN32

std::wstring toWString(const std::string& s)
{
	std::wstring ret;
	int len = MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, s.data(),
			s.length(), nullptr, 0);
	if (len == 0)
	{
		return ret;
	}
	ret.resize(len);
	MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, s.data(), s.length(), &ret[0], len);
	return ret;
}

#endif


FILE* F_OPEN_UTF8(std::string const& fname, const char* mode){
#ifdef LMMS_BUILD_WIN32
	return _wfopen(toWString(fname).data(), toWString(mode).data());
#else
	return fopen(fname.data(), mode);
#endif
}


int fileToDescriptor(FILE* f, bool closeFile = true)
{
	int fh;
	if (f == nullptr) {return -1;}

#ifdef LMMS_BUILD_WIN32
	fh = _dup(_fileno(f));
#else
	fh = dup(fileno(f));
#endif

	if (closeFile) {fclose(f);}
	return fh;
}


} // namespace lmms

#endif
