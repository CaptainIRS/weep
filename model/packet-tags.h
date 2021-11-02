#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/application.h"

#include "ns3/stats-module.h"

using namespace ns3;

/**
 * @brief Current timestamp (t)
 * 
 */
class TimestampTag : public Tag {
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
class RadioRangeTag : public Tag {
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
class MaximumEnergyTag : public Tag {
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
class CurrentResidualEnergyTag : public Tag {
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
 * @brief Rate of energy depletion (depl-eng(i))
 * 
 */
 class EnergyDepletionRateTag : public Tag {
public:
  static TypeId GetTypeId (void);
  virtual TypeId GetInstanceTypeId (void) const;
  
  virtual uint32_t GetSerializedSize (void) const;
  virtual void Serialize (TagBuffer i) const;
  virtual void Deserialize (TagBuffer i);
  
  void SetEnergyDepletionRate (double energyDepletionRate);
  double GetEnergyDepletionRate (void) const;
  
  void Print (std::ostream &os) const;

private:
    double m_energyDepletionRate;
};