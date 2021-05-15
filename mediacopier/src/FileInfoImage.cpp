/* Copyright (C) 2020-2021 Patrick Ziegler
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

#include <date/date.h>
#include <mediacopier/AbstractFileOperation.hpp>
#include <mediacopier/Error.hpp>
#include <mediacopier/FileInfoImage.hpp>

static constexpr const std::array<char[29], 4> keysDateTime = {
    "Exif.Photo.DateTimeOriginal",
    "Exif.Image.DateTimeOriginal",
    "Exif.Photo.DateTimeDigitized",
    "Exif.Image.DateTime",
};

static constexpr const std::array<char[31], 3> keysSubSec = {
    "Exif.Photo.SubSecTimeOriginal",
    "Exif.Photo.SubSecTimeDigitized",
    "Exif.Photo.SubSecTime"
};

namespace MediaCopier {

FileInfoImage::FileInfoImage(std::filesystem::path path, Exiv2::ExifData& exif) : AbstractFileInfo{std::move(path)}
{
    for (const std::string& key : keysDateTime) {
        if (exif.findKey(Exiv2::ExifKey{key}) == exif.end()) {
            continue;
        }
        std::istringstream ss{exif[key].toString()};
        ss >> date::parse("%Y:%m:%d %H:%M:%S", m_timestamp);
        break;
    }

    for (const std::string& key : keysSubSec) {
        if (exif.findKey(Exiv2::ExifKey{key}) == exif.end()) {
            continue;
        }
        std::string subsec = exif[key].toString();
        if (subsec.length() < 1) {
            continue;
        } else if (subsec.length() < 4) {
            m_timestamp += std::chrono::milliseconds{exif[key].toLong()};
        } else {
            m_timestamp += std::chrono::microseconds{exif[key].toLong()};
        }
        break;
    }

    if (m_timestamp == std::chrono::system_clock::time_point{}) {
        throw FileInfoError{"No date information found"};
    }
}

void FileInfoImage::accept(const AbstractFileOperation& operation) const
{
    operation.visit(*this);
}

} // namespace MediaCopier
