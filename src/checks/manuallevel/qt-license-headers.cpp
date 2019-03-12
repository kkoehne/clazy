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

#include "qt-license-headers.h"

#include <algorithm>
#include <vector>
#include <regex>
#include <queue>

using namespace clang;
using namespace llvm;
using namespace std;

QtLicenseHeaders::QtLicenseHeaders(const std::string &name, ClazyContext *context)
    : CheckBase(name, context)
{
    enablePreProcessorCallbacks();
}

static std::vector<std::string> licensesInContext;
static std::map<clang::FileID, std::string> licensePerFile;

void QtLicenseHeaders::VisitFileChanged(SourceLocation loc, PPCallbacks::FileChangeReason reason,
                                        SrcMgr::CharacteristicKind /* FileType */, FileID /* PrevFID */)
{
    const SourceManager &sm = CheckBase::sm();

    loc = sm.getExpansionLoc(loc);
    if (reason != PPCallbacks::ExitFile)
        return;

    // returning from top-level include?
    if (sm.isInMainFile(loc)) {
        if (!licensesInContext.empty()) {
            std::string license_str;
            std::sort(licensesInContext.begin(), licensesInContext.end());
            for (auto license: licensesInContext)
                license_str += " " + license;
            // new loc is actually in the line afterwards, so move one character back
            emitWarning(loc.getLocWithOffset(-1), "Includes Qt code released under open-source licenses:" + license_str);
            licensesInContext.clear();
        }
        return;
    }

    // has file been already searched?
    clang::FileID fid = sm.getFileID(loc);
    auto iter = licensePerFile.find(fid);
    if (iter == licensePerFile.end()) {
        const FileEntry *fe = sm.getFileEntryForID(fid);
        if (!fe)
          return;

        bool invalidMemoryBuffer = true;
        MemoryBuffer *buf = m_sm.getMemoryBufferForFile(fe, &invalidMemoryBuffer);
        if (invalidMemoryBuffer)
            return;

        std::string licenseID;
        static regex rx(R"(\$QT_BEGIN_LICENSE:(.+)\$)");
        cmatch match;
        // todo: Only search in first 20 lines of buffer
        if (regex_search(buf->getBuffer().data(), match, rx))
            licenseID = match.str(1);
        iter = licensePerFile.insert(std::pair<clang::FileID, std::string>(fid, licenseID)).first;
    }

    std::string licenseID = iter->second;
    if (!licenseID.empty()
            && std::find(licensesInContext.cbegin(), licensesInContext.cend(), licenseID) == licensesInContext.cend())
        licensesInContext.push_back(licenseID);
}
