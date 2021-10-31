/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */

#include "aodv-weep-dpd.h"

namespace ns3 {
namespace weep {

bool
DuplicatePacketDetection::IsDuplicate  (Ptr<const Packet> p, const Ipv4Header & header)
{
  return m_idCache.IsDuplicate (header.GetSource (), p->GetUid () );
}
void
DuplicatePacketDetection::SetLifetime (Time lifetime)
{
  m_idCache.SetLifetime (lifetime);
}

Time
DuplicatePacketDetection::GetLifetime () const
{
  return m_idCache.GetLifeTime ();
}


}
}

