/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */

#ifndef AODV_WEEP_ID_CACHE_H
#define AODV_WEEP_ID_CACHE_H

#include "ns3/ipv4-address.h"
#include "ns3/simulator.h"
#include <vector>

namespace ns3 {
namespace weep {
/**
 * \ingroup weep
 *
 * \brief Unique packets identification cache used for simple duplicate detection.
 */
class IdCache
{
public:
  /**
   * constructor
   * \param lifetime the lifetime for added entries
   */
  IdCache (Time lifetime) : m_lifetime (lifetime)
  {
  }
  /**
   * Check that entry (addr, id) exists in cache. Add entry, if it doesn't exist.
   * \param addr the IP address
   * \param id the cache entry ID
   * \returns true if the pair exists
   */ 
  bool IsDuplicate (Ipv4Address addr, uint32_t id);
  /// Remove all expired entries
  void Purge ();
  /**
   * \returns number of entries in cache
   */
  uint32_t GetSize ();
  /**
   * Set lifetime for future added entries.
   * \param lifetime the lifetime for entries
   */
  void SetLifetime (Time lifetime)
  {
    m_lifetime = lifetime;
  }
  /**
   * Return lifetime for existing entries in cache
   * \returns thhe lifetime
   */
  Time GetLifeTime () const
  {
    return m_lifetime;
  }
private:
  /// Unique packet ID
  struct UniqueId
  {
    /// ID is supposed to be unique in single address context (e.g. sender address)
    Ipv4Address m_context;
    /// The id
    uint32_t m_id;
    /// When record will expire
    Time m_expire;
  };
  /**
   * \brief IsExpired structure
   */
  struct IsExpired
  {
    /**
     * \brief Check if the entry is expired
     *
     * \param u UniqueId entry
     * \return true if expired, false otherwise
     */
    bool operator() (const struct UniqueId & u) const
    {
      return (u.m_expire < Simulator::Now ());
    }
  };
  /// Already seen IDs
  std::vector<UniqueId> m_idCache;
  /// Default lifetime for ID records
  Time m_lifetime;
};

}  // namespace weep
}  // namespace ns3

#endif /* AODV_WEEP_ID_CACHE_H */
