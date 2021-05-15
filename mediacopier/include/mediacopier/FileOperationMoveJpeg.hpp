/* Copyright (C) 2020 Patrick Ziegler
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#pragma once

#include <mediacopier/FileOperationCopyJpeg.hpp>

namespace MediaCopier {

class FileOperationMoveJpeg : public FileOperationCopyJpeg {
public:
    using FileOperationCopyJpeg::FileOperationCopyJpeg;
    void visit(const FileInfoImage& file) const override;
    void visit(const FileInfoImageJpeg& file) const override;
    void visit(const FileInfoVideo& file) const override;
};

} // namespace MediaCopier