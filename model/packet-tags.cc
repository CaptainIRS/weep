#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/application.h"


#include "ns3/type-id.h"
#include "packet-tags.h"
#include <cstdint>


using namespace ns3;

TypeId 
TimestampTag::GetTypeId (void)
{
  static TypeId tid = TypeId ("TimestampTag")
    .SetParent<Tag> ()
    .AddConstructor<TimestampTag> ()
    .AddAttribute ("Timestamp",
                   "Timestamp",
                   EmptyAttributeValue (),
                   MakeTimeAccessor (&TimestampTag::GetTimestamp),
                   MakeTimeChecker ())
  ;
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
  i.Write ((const uint8_t *)&t, 8);
}
void 
TimestampTag::Deserialize (TagBuffer i)
{
  int64_t t;
  i.Read ((uint8_t *)&t, 8);
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
RadioRangeTag::GetTypeId()
{
  static TypeId tid = TypeId("RadioRangeTag")
    .SetParent<Tag>()
    .AddConstructor<RadioRangeTag>()
    .AddAttribute("Range",
                  "The range of the radio",
                  EmptyAttributeValue(),
                  MakeDoubleAccessor(&RadioRangeTag::GetRadioRange),
                  MakeDoubleChecker<double>())
  ;
  return tid;
}

TypeId
RadioRangeTag::GetInstanceTypeId() const
{
  return GetTypeId();
}

uint32_t
RadioRangeTag::GetSerializedSize() const
{
  return sizeof(double);
}

void
RadioRangeTag::Serialize(TagBuffer i) const
{
  i.WriteDouble(m_range);
}

void
RadioRangeTag::Deserialize(TagBuffer i)
{
  m_range = i.ReadDouble();
}

void
RadioRangeTag::SetRadioRange(double range)
{
  m_range = range;
}

double
RadioRangeTag::GetRadioRange() const
{
  return m_range;
}

void
RadioRangeTag::Print(std::ostream &os) const
{
  os << "range=" << m_range;
}

// Implement similarly for MaximumEnergyTag
TypeId
MaximumEnergyTag::GetTypeId()
{
  static TypeId tid = TypeId("MaximumEnergyTag")
    .SetParent<Tag>()
    .AddConstructor<MaximumEnergyTag>()
    .AddAttribute("Energy",
                  "The maximum energy of the node",
                  EmptyAttributeValue(),
                  MakeDoubleAccessor(&MaximumEnergyTag::GetMaximumEnergy),
                  MakeDoubleChecker<double>())
  ;
  return tid;
}

TypeId
MaximumEnergyTag::GetInstanceTypeId() const
{
  return GetTypeId();
}

uint32_t
MaximumEnergyTag::GetSerializedSize() const
{
  return sizeof(double);
}

void
MaximumEnergyTag::Serialize(TagBuffer i) const
{
  i.WriteDouble(m_maximumEnergy);
}

void
MaximumEnergyTag::Deserialize(TagBuffer i)
{
  m_maximumEnergy = i.ReadDouble();
}

void
MaximumEnergyTag::SetMaximumEnergy(double energy)
{
  m_maximumEnergy = energy;
}

double
MaximumEnergyTag::GetMaximumEnergy() const
{
  return m_maximumEnergy;
}

void
MaximumEnergyTag::Print(std::ostream &os) const
{
  os << "maxEnergy=" << m_maximumEnergy;
}


// Implement similarly for CurrentResidualEnergyTag
TypeId
CurrentResidualEnergyTag::GetTypeId()
{
  static TypeId tid = TypeId("CurrentResidualEnergyTag")
    .SetParent<Tag>()
    .AddConstructor<CurrentResidualEnergyTag>()
    .AddAttribute("Energy",
                  "The current residual energy of the node",
                  EmptyAttributeValue(),
                  MakeDoubleAccessor(&CurrentResidualEnergyTag::GetCurrentResidualEnergy),
                  MakeDoubleChecker<double>())
  ;
  return tid;
}

TypeId
CurrentResidualEnergyTag::GetInstanceTypeId() const
{
  return GetTypeId();
}

uint32_t
CurrentResidualEnergyTag::GetSerializedSize() const
{
  return sizeof(double);
}

void
CurrentResidualEnergyTag::Serialize(TagBuffer i) const
{
  i.WriteDouble(m_currentResidualEnergy);
}

void
CurrentResidualEnergyTag::Deserialize(TagBuffer i)
{
  m_currentResidualEnergy = i.ReadDouble();
}

void
CurrentResidualEnergyTag::SetCurrentResidualEnergy(double energy)
{
  m_currentResidualEnergy = energy;
}

double
CurrentResidualEnergyTag::GetCurrentResidualEnergy() const
{
  return m_currentResidualEnergy;
}

void
CurrentResidualEnergyTag::Print(std::ostream &os) const
{
  os << "currentResidualEnergy=" << m_currentResidualEnergy;
}


// Implement similarly for EnergyDepletionRateTag
TypeId
EnergyDepletionRateTag::GetTypeId()
{
  static TypeId tid = TypeId("EnergyDepletionRateTag")
    .SetParent<Tag>()
    .AddConstructor<EnergyDepletionRateTag>()
    .AddAttribute("Energy",
                  "The energy depletion rate of the node",
                  EmptyAttributeValue(),
                  MakeDoubleAccessor(&EnergyDepletionRateTag::GetEnergyDepletionRate),
                  MakeDoubleChecker<double>())
  ;
  return tid;
}

TypeId
EnergyDepletionRateTag::GetInstanceTypeId() const
{
  return GetTypeId();
}

uint32_t
EnergyDepletionRateTag::GetSerializedSize() const
{
  return sizeof(double);
}

void
EnergyDepletionRateTag::Serialize(TagBuffer i) const
{
  i.WriteDouble(m_energyDepletionRate);
}

void
EnergyDepletionRateTag::Deserialize(TagBuffer i)
{
  m_energyDepletionRate = i.ReadDouble();
}

void
EnergyDepletionRateTag::SetEnergyDepletionRate(double energy)
{
  m_energyDepletionRate = energy;
}

double
EnergyDepletionRateTag::GetEnergyDepletionRate() const
{
  return m_energyDepletionRate;
}

void
EnergyDepletionRateTag::Print(std::ostream &os) const
{
  os << "energyDepletionRate=" << m_energyDepletionRate;
}
