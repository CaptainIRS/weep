#include "ns3/core-module.h"
#include "ns3/ipv4-address.h"
#include "ns3/network-module.h"
#include "ns3/application.h"

#include "ns3/type-id.h"
#include "ns3/uinteger.h"
#include "packet-tags.h"
#include <cstdint>

using namespace ns3;

TypeId
TimestampTag::GetTypeId (void)
{
  static TypeId tid =
      TypeId ("TimestampTag")
          .SetParent<Tag> ()
          .AddConstructor<TimestampTag> ()
          .AddAttribute ("Timestamp", "Timestamp", EmptyAttributeValue (),
                         MakeTimeAccessor (&TimestampTag::GetTimestamp), MakeTimeChecker ());
  return tid;
}

TypeId
TimestampTag::GetInstanceTypeId (void) const
{
  return GetTypeId ();
}

uint32_t
TimestampTag::GetSerializedSize (void) const
{
  return 8;
}
void
TimestampTag::Serialize (TagBuffer i) const
{
  int64_t t = m_timestamp.GetNanoSeconds ();
  i.Write ((const uint8_t *) &t, 8);
}
void
TimestampTag::Deserialize (TagBuffer i)
{
  int64_t t;
  i.Read ((uint8_t *) &t, 8);
  m_timestamp = NanoSeconds (t);
}

void
TimestampTag::SetTimestamp (Time time)
{
  m_timestamp = time;
}
Time
TimestampTag::GetTimestamp (void) const
{
  return m_timestamp;
}

void
TimestampTag::Print (std::ostream &os) const
{
  os << "t=" << m_timestamp;
}

TypeId
RadioRangeTag::GetTypeId ()
{
  static TypeId tid = TypeId ("RadioRangeTag")
                          .SetParent<Tag> ()
                          .AddConstructor<RadioRangeTag> ()
                          .AddAttribute ("Range", "The range of the radio", EmptyAttributeValue (),
                                         MakeDoubleAccessor (&RadioRangeTag::GetRadioRange),
                                         MakeDoubleChecker<double> ());
  return tid;
}

TypeId
RadioRangeTag::GetInstanceTypeId () const
{
  return GetTypeId ();
}

uint32_t
RadioRangeTag::GetSerializedSize () const
{
  return sizeof (double);
}

void
RadioRangeTag::Serialize (TagBuffer i) const
{
  i.WriteDouble (m_range);
}

void
RadioRangeTag::Deserialize (TagBuffer i)
{
  m_range = i.ReadDouble ();
}

void
RadioRangeTag::SetRadioRange (double range)
{
  m_range = range;
}

double
RadioRangeTag::GetRadioRange () const
{
  return m_range;
}

void
RadioRangeTag::Print (std::ostream &os) const
{
  os << "range=" << m_range;
}

TypeId
MaximumEnergyTag::GetTypeId ()
{
  static TypeId tid =
      TypeId ("MaximumEnergyTag")
          .SetParent<Tag> ()
          .AddConstructor<MaximumEnergyTag> ()
          .AddAttribute ("Energy", "The maximum energy of the node", EmptyAttributeValue (),
                         MakeDoubleAccessor (&MaximumEnergyTag::GetMaximumEnergy),
                         MakeDoubleChecker<double> ());
  return tid;
}

TypeId
MaximumEnergyTag::GetInstanceTypeId () const
{
  return GetTypeId ();
}

uint32_t
MaximumEnergyTag::GetSerializedSize () const
{
  return sizeof (double);
}

void
MaximumEnergyTag::Serialize (TagBuffer i) const
{
  i.WriteDouble (m_maximumEnergy);
}

void
MaximumEnergyTag::Deserialize (TagBuffer i)
{
  m_maximumEnergy = i.ReadDouble ();
}

void
MaximumEnergyTag::SetMaximumEnergy (double energy)
{
  m_maximumEnergy = energy;
}

double
MaximumEnergyTag::GetMaximumEnergy () const
{
  return m_maximumEnergy;
}

void
MaximumEnergyTag::Print (std::ostream &os) const
{
  os << "maxEnergy=" << m_maximumEnergy;
}

TypeId
CurrentResidualEnergyTag::GetTypeId ()
{
  static TypeId tid =
      TypeId ("CurrentResidualEnergyTag")
          .SetParent<Tag> ()
          .AddConstructor<CurrentResidualEnergyTag> ()
          .AddAttribute ("Energy", "The current residual energy of the node",
                         EmptyAttributeValue (),
                         MakeDoubleAccessor (&CurrentResidualEnergyTag::GetCurrentResidualEnergy),
                         MakeDoubleChecker<double> ());
  return tid;
}

TypeId
CurrentResidualEnergyTag::GetInstanceTypeId () const
{
  return GetTypeId ();
}

uint32_t
CurrentResidualEnergyTag::GetSerializedSize () const
{
  return sizeof (double);
}

void
CurrentResidualEnergyTag::Serialize (TagBuffer i) const
{
  i.WriteDouble (m_currentResidualEnergy);
}

void
CurrentResidualEnergyTag::Deserialize (TagBuffer i)
{
  m_currentResidualEnergy = i.ReadDouble ();
}

void
CurrentResidualEnergyTag::SetCurrentResidualEnergy (double energy)
{
  m_currentResidualEnergy = energy;
}

double
CurrentResidualEnergyTag::GetCurrentResidualEnergy () const
{
  return m_currentResidualEnergy;
}

void
CurrentResidualEnergyTag::Print (std::ostream &os) const
{
  os << "currentResidualEnergy=" << m_currentResidualEnergy;
}

TypeId
SourceIPTag::GetTypeId ()
{
  static TypeId tid =
      TypeId ("SourceIPTag")
          .SetParent<Tag> ()
          .AddConstructor<SourceIPTag> ()
          .AddAttribute ("SourceIP", "The IP Address of the source node", EmptyAttributeValue (),
                         MakeIpv4AddressAccessor (&SourceIPTag::GetSourceIP),
                         MakeIpv4AddressChecker ());
  return tid;
}

TypeId
SourceIPTag::GetInstanceTypeId () const
{
  return GetTypeId ();
}

uint32_t
SourceIPTag::GetSerializedSize () const
{
  return sizeof (uint8_t) * 4;
}

void
SourceIPTag::Serialize (TagBuffer i) const
{
  uint8_t ip[4];
  m_sourceIP.Serialize (ip);
  i.Write (ip, 4);
}

void
SourceIPTag::Deserialize (TagBuffer i)
{
  uint8_t ip[4];
  i.Read (ip, 4);
  m_sourceIP = m_sourceIP.Deserialize (ip);
}

void
SourceIPTag::SetSourceIP (Ipv4Address sourceIP)
{
  m_sourceIP = sourceIP;
}

Ipv4Address
SourceIPTag::GetSourceIP () const
{
  return m_sourceIP;
}

void
SourceIPTag::Print (std::ostream &os) const
{
  os << "sourceIP=" << m_sourceIP;
}

TypeId
DestinationIPTag::GetTypeId ()
{
  static TypeId tid =
      TypeId ("DestinationIPTag")
          .SetParent<Tag> ()
          .AddConstructor<DestinationIPTag> ()
          .AddAttribute ("DestinationIP", "The IP Address of the destination node",
                         EmptyAttributeValue (),
                         MakeIpv4AddressAccessor (&DestinationIPTag::GetDestinationIP),
                         MakeIpv4AddressChecker ());
  return tid;
}

TypeId
DestinationIPTag::GetInstanceTypeId () const
{
  return GetTypeId ();
}

uint32_t
DestinationIPTag::GetSerializedSize () const
{
  return sizeof (uint8_t) * 4;
}

void
DestinationIPTag::Serialize (TagBuffer i) const
{
  uint8_t ip[4];
  m_destinationIP.Serialize (ip);
  i.Write (ip, 4);
}

void
DestinationIPTag::Deserialize (TagBuffer i)
{
  uint8_t ip[4];
  i.Read (ip, 4);
  m_destinationIP = m_destinationIP.Deserialize (ip);
}

void
DestinationIPTag::SetDestinationIP (Ipv4Address destinationIP)
{
  m_destinationIP = destinationIP;
}

Ipv4Address
DestinationIPTag::GetDestinationIP () const
{
  return m_destinationIP;
}

void
DestinationIPTag::Print (std::ostream &os) const
{
  os << "destinationIP=" << m_destinationIP;
}

TypeId
HopCountTag::GetTypeId ()
{
  static TypeId tid =
      TypeId ("HopCountTag")
          .SetParent<Tag> ()
          .AddConstructor<HopCountTag> ()
          .AddAttribute ("HopCount", "The number of hops between source and destination",
                         EmptyAttributeValue (), MakeUintegerAccessor (&HopCountTag::GetHopCount),
                         MakeUintegerChecker<uint32_t> ());
  return tid;
}

TypeId
HopCountTag::GetInstanceTypeId () const
{
  return GetTypeId ();
}

uint32_t
HopCountTag::GetSerializedSize () const
{
  return sizeof (uint32_t);
}

void
HopCountTag::Serialize (TagBuffer i) const
{
  i.WriteU32 (m_hopCount);
}

void
HopCountTag::Deserialize (TagBuffer i)
{
  m_hopCount = i.ReadU32 ();
}

void
HopCountTag::SetHopCount (uint32_t hopCount)
{
  m_hopCount = hopCount;
}

uint32_t
HopCountTag::GetHopCount () const
{
  return m_hopCount;
}

void
HopCountTag::Print (std::ostream &os) const
{
  os << "hopCount=" << m_hopCount;
}
