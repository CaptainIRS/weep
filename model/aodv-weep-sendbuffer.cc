/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */

#include "aodv-weep-sendbuffer.h"
#include <algorithm>
#include <functional>
#include "ns3/ipv4-route.h"
#include "ns3/socket.h"
#include "ns3/log.h"

namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("AodvRequestQueue");

namespace weep {
uint32_t
AodvSendBuffer::GetSize ()
{
  Purge ();
  return m_queue.size ();
}

bool
AodvSendBuffer::Enqueue (AodvSendBufferEntry & entry)
{
  Purge ();
  for (std::vector<AodvSendBufferEntry>::const_iterator i = m_queue.begin (); i
       != m_queue.end (); ++i)
    {
      if ((i->GetPacket ()->GetUid () == entry.GetPacket ()->GetUid ())
          && (i->GetIpv4Header ().GetDestination ()
              == entry.GetIpv4Header ().GetDestination ()))
        {
          return false;
        }
    }
  entry.SetExpireTime (m_sendBufferTimeout);
  if (m_queue.size () == m_maxLen)
    {
      Drop (m_queue.front (), "Drop the most aged packet"); // Drop the most aged packet
      m_queue.erase (m_queue.begin ());
    }
  m_queue.push_back (entry);
  return true;
}

void
AodvSendBuffer::DropPacketWithDst (Ipv4Address dst)
{
  NS_LOG_FUNCTION (this << dst);
  Purge ();
  for (std::vector<AodvSendBufferEntry>::iterator i = m_queue.begin (); i
       != m_queue.end (); ++i)
    {
      if (i->GetIpv4Header ().GetDestination () == dst)
        {
          Drop (*i, "DropPacketWithDst ");
        }
    }
  auto new_end = std::remove_if (m_queue.begin (), m_queue.end (),
                                 [&](const AodvSendBufferEntry& en) { return en.GetIpv4Header ().GetDestination () == dst; });
  m_queue.erase (new_end, m_queue.end ());
}

bool
AodvSendBuffer::Dequeue (Ipv4Address dst, AodvSendBufferEntry & entry)
{
  Purge ();
  for (std::vector<AodvSendBufferEntry>::iterator i = m_queue.begin (); i != m_queue.end (); ++i)
    {
      if (i->GetIpv4Header ().GetDestination () == dst)
        {
          entry = *i;
          m_queue.erase (i);
          return true;
        }
    }
  return false;
}

bool
AodvSendBuffer::Find (Ipv4Address dst)
{
  for (std::vector<AodvSendBufferEntry>::const_iterator i = m_queue.begin (); i
       != m_queue.end (); ++i)
    {
      if (i->GetIpv4Header ().GetDestination () == dst)
        {
          return true;
        }
    }
  return false;
}

/**
 * \brief IsExpired structure
 */
struct IsExpired
{
  bool
  /**
   * Check if the entry is expired
   *
   * \param e AodvSendBufferEntry entry
   * \return true if expired, false otherwise
   */
  operator() (AodvSendBufferEntry const & e) const
  {
    return (e.GetExpireTime () < Seconds (0));
  }
};

void
AodvSendBuffer::Purge ()
{
  IsExpired pred;
  for (std::vector<AodvSendBufferEntry>::iterator i = m_queue.begin (); i
       != m_queue.end (); ++i)
    {
      if (pred (*i))
        {
          Drop (*i, "Drop outdated packet ");
        }
    }
  m_queue.erase (std::remove_if (m_queue.begin (), m_queue.end (), pred),
                 m_queue.end ());
}

void
AodvSendBuffer::Drop (AodvSendBufferEntry en, std::string reason)
{
  NS_LOG_LOGIC (reason << en.GetPacket ()->GetUid () << " " << en.GetIpv4Header ().GetDestination ());
  en.GetErrorCallback () (en.GetPacket (), en.GetIpv4Header (),
                          Socket::ERROR_NOROUTETOHOST);
  return;
}

}  // namespace weep
}  // namespace ns3
