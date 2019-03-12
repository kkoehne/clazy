/*
  This file is part of the clazy static checker.

    Copyright (C) 2018 Sergio Martins <smartins@kde.org>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public License
    along with this library; see the file COPYING.LIB.  If not, write to
    the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
    Boston, MA 02110-1301, USA.
*/

#ifndef CLAZY_QT_HEADER_LICENSES_H
#define CLAZY_QT_HEADER_LICENSES_H

#include "checkbase.h"

#include <clang/Basic/SourceLocation.h>

#include <string>

class ClazyContext;
namespace clang {
class MacroInfo;
class Token;
}  // namespace clang


/**
 * See README-qt-license-headers.md for more info.
 */
class QtLicenseHeaders
    : public CheckBase
{
public:
    explicit QtLicenseHeaders(const std::string &name, ClazyContext *context);
protected:
    void VisitFileChanged(clang::SourceLocation Loc, clang::PPCallbacks::FileChangeReason Reason, clang::SrcMgr::CharacteristicKind FileType, clang::FileID PrevFID) override;
};

#endif
