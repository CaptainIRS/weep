#include "ns3/application.h"
#include "ns3/ipv4-address.h"
#include <cstdint>

#ifndef PACKET_TAGS_H
#define PACKET_TAGS_H

using namespace ns3;

/**
 * @brief Current timestamp (t)
 * 
 */
class TimestampTag : public Tag
{
public:
  static TypeId GetTypeId (void);
  virtual TypeId GetInstanceTypeId (void) const;

  virtual uint32_t GetSerializedSize (void) const;
  virtual void Serialize (TagBuffer i) const;
  virtual void Deserialize (TagBuffer i);

  void SetTimestamp (Time time);
  Time GetTimestamp (void) const;

  void Print (std::ostream &os) const;

private:
  Time m_timestamp;
};

/**
 * @brief Radio-range rad(l)
 * 
 */
class RadioRangeTag : public Tag
{
public:
  static TypeId GetTypeId (void);
  virtual TypeId GetInstanceTypeId (void) const;

  virtual uint32_t GetSerializedSize (void) const;
  virtual void Serialize (TagBuffer i) const;
  virtual void Deserialize (TagBuffer i);

  void SetRadioRange (double range);
  double GetRadioRange (void) const;

  void Print (std::ostream &os) const;

private:
  double m_range;
};

/**
 * @brief Maximum energy (Total energy) (El)
 * 
 */
class MaximumEnergyTag : public Tag
{
public:
  static TypeId GetTypeId (void);
  virtual TypeId GetInstanceTypeId (void) const;

  virtual uint32_t GetSerializedSize (void) const;
  virtual void Serialize (TagBuffer i) const;
  virtual void Deserialize (TagBuffer i);

  void SetMaximumEnergy (double maximumEnergy);
  double GetMaximumEnergy (void) const;

  void Print (std::ostream &os) const;

private:
  double m_maximumEnergy;
};

/**
 * @brief  Current residual energy (remaining energy) (el(t))
 * 
 */
class CurrentResidualEnergyTag : public Tag
{
public:
  static TypeId GetTypeId (void);
  virtual TypeId GetInstanceTypeId (void) const;

  virtual uint32_t GetSerializedSize (void) const;
  virtual void Serialize (TagBuffer i) const;
  virtual void Deserialize (TagBuffer i);

  void SetCurrentResidualEnergy (double currentResidualEnergy);
  double GetCurrentResidualEnergy (void) const;

  void Print (std::ostream &os) const;

private:
  double m_currentResidualEnergy;
};

/**
 * @brief Source IP Address
 * 
 */
class SourceIPTag : public Tag
{
public:
  static TypeId GetTypeId (void);
  virtual TypeId GetInstanceTypeId (void) const;

  virtual uint32_t GetSerializedSize (void) const;
  virtual void Serialize (TagBuffer i) const;
  virtual void Deserialize (TagBuffer i);

  void SetSourceIP (Ipv4Address sourceIP);
  Ipv4Address GetSourceIP (void) const;

  void Print (std::ostream &os) const;

private:
  Ipv4Address m_sourceIP;
};

/**
 * @brief Destination IP Address
 * 
 */
class DestinationIPTag : public Tag
{
public:
  static TypeId GetTypeId (void);
  virtual TypeId GetInstanceTypeId (void) const;

  virtual uint32_t GetSerializedSize (void) const;
  virtual void Serialize (TagBuffer i) const;
  virtual void Deserialize (TagBuffer i);

  void SetDestinationIP (Ipv4Address destinationIP);
  Ipv4Address GetDestinationIP (void) const;

  void Print (std::ostream &os) const;

private:
  Ipv4Address m_destinationIP;
};

/**
 * @brief Hop Count
 * 
 */
class HopCountTag : public Tag
{
public:
  static TypeId GetTypeId (void);
  virtual TypeId GetInstanceTypeId (void) const;

  virtual uint32_t GetSerializedSize (void) const;
  virtual void Serialize (TagBuffer i) const;
  virtual void Deserialize (TagBuffer i);

  void SetHopCount (uint32_t hopCount);
  uint32_t GetHopCount (void) const;

  void Print (std::ostream &os) const;

private:
  uint32_t m_hopCount;
};

#endif /* PACKET_TAGS_H */
