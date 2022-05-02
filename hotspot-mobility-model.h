/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 *
 * Author: Oscar Gao and Balaji Madhu
 */

#ifndef HOTSPOT_MOBILITY_MODEL_H
#define HOTSPOT_MOBILITY_MODEL_H

#include "constant-velocity-helper.h"
#include "mobility-model.h"
#include "position-allocator.h"
#include "ns3/object-vector.h"
#include "ns3/random-variable-stream.h"
#include "ns3/string.h"
#include "ns3/boolean.h"
#include "ns3/simulator.h"
#include "ns3/pointer.h"
#include <tuple>
#include <vector>
#include <string>
#include <sstream>

namespace ns3
{
  /**
   * \ingroup mobility
   * \brief Hotspot Mobility Model.
   *
   */
  class HotspotMobilityModel : public MobilityModel
  {
  public:
    /**
     * Register this type with the TypeId system.
     * \return the object TypeId
     */
    static TypeId GetTypeId(void);
    void AddHotspot(Vector, double, double);
    void ClearHotspots(void);

  protected:
    virtual void DoInitialize(void);

  private:
    /**
     * Get next position, begin moving towards it, schedule future pause event
     */
    void BeginWalk(void);
    /**
     * Begin current pause event, schedule future walk event
     */
    void DoInitializePrivate(void);

    virtual Vector DoGetPosition(void) const;
    virtual void DoSetPosition(const Vector &position);
    virtual Vector DoGetVelocity(void) const;

    ConstantVelocityHelper m_helper;     //!< helper for velocity computations
    Ptr<RandomVariableStream> m_speed;   //!< random variable to generate speeds
    Ptr<ListPositionAllocator> m_position; //!< pointer to position allocator
    std::vector<Vector> m_locations;     //!< array of hotspot locations
    std::string m_pauses_str;
    std::string m_probabilities_str;
    std::vector<double> m_pauses;        //!< array of hotspot pauses
    std::vector<double> m_probabilities; //!< array of hotspot probabilities

    int m_which;           //!< index to pick hotspot
    bool m_first{true};    //!< whether is first walk or not
    bool m_normal;         //!< normalize prob. or not
    double m_totalProb{0}; //!< total prob.

    EventId m_event; //!< event ID of next scheduled event
  };

} // namespace ns3

#endif /* HOTSPOT_MOBILITY_MODEL_H */
