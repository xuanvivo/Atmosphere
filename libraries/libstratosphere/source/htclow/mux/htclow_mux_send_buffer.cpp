/*
 * Copyright (c) 2018-2020 Atmosphère-NX
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms and conditions of the GNU General Public License,
 * version 2, as published by the Free Software Foundation.
 *
 * This program is distributed in the hope it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include <stratosphere.hpp>
#include "htclow_mux_channel_impl.hpp"
#include "../htclow_packet_factory.hpp"

namespace ams::htclow::mux {

    void SendBuffer::SetVersion(s16 version) {
        /* Set version. */
        m_version = version;
    }

    void SendBuffer::Clear() {
        while (!m_packet_list.empty()) {
            auto *packet = std::addressof(m_packet_list.front());
            m_packet_list.pop_front();
            m_packet_factory->Delete(packet);
        }
    }

}