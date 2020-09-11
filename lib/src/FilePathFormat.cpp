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

#include <mediacopier/AbstractFileInfo.hpp>
#include <mediacopier/FilePathFormat.hpp>

#include <random>
#include <sstream>

namespace fs = std::filesystem;
namespace mc = MediaCopier;

static fs::path create_unique_path(const std::string prefix = "MediaCopier-", const unsigned int len = 12) {
    std::random_device device;
    std::mt19937 generator{device()};
    std::uniform_int_distribution<> distribution{0, 255};

    std::ostringstream unique_identifier;
    unique_identifier << prefix << std::hex;

    for (unsigned int i = 0; i < len; ++i) {
        unique_identifier << distribution(generator);
    }

    fs::path unique_path = fs::temp_directory_path();
    unique_path /= unique_identifier.str();
    unique_path /= "";

    return unique_path;
}

fs::path createPath(const mc::AbstractFileInfo &file, fs::path destination)
{
    // TODO: make these fields configurable
    std::string pattern{"IMG_%Y%m%d_%H%M%S_"};
    bool subsec = true;

    std::stringstream ss;
    ss << destination.string();

    auto ts = std::chrono::system_clock::to_time_t(file.timestamp());
    ss << std::put_time(std::gmtime(&ts), pattern.c_str());

    if (subsec) {
        auto us = std::chrono::duration_cast<std::chrono::microseconds>(file.timestamp().time_since_epoch()) % 1000000;
        ss << std::setfill('0') << std::setw(6) << us.count();
    }

    ss << file.path().extension().string();

    return {ss.str()};
}

mc::FilePathFormat::FilePathFormat(fs::path destination) : m_destination{std::move(destination)}
{
    m_destination /= ""; // this will append a trailing directory separator when necessary
    m_tempdir = create_unique_path();
}

mc::FilePathFormat::~FilePathFormat()
{
    if (fs::exists(m_tempdir)) {
        fs::remove_all(m_tempdir);
    }
}

fs::path mc::FilePathFormat::createPathFrom(const mc::AbstractFileInfo &file) const
{
    return createPath(file, m_destination);
}

fs::path mc::FilePathFormat::createTemporaryPathFrom(const AbstractFileInfo &file) const
{
    return createPath(file, m_tempdir);
}
