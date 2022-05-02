/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 *
 * Author: Oscar and Balaji
 */

#include "hotspot-mobility-model.h"

namespace ns3
{
  NS_OBJECT_ENSURE_REGISTERED(HotspotMobilityModel);

  TypeId
  HotspotMobilityModel::GetTypeId(void)
  {
    static TypeId tid = TypeId("ns3::HotspotMobilityModel")
                            .SetParent<MobilityModel>()
                            .SetGroupName("Mobility")
                            .AddConstructor<HotspotMobilityModel>()
                            .AddAttribute("Speed",
                                          "A random variable used to pick the speed of a hotspot mobility model.",
                                          StringValue("ns3::UniformRandomVariable[Min=20|Max=50]"),
                                          MakePointerAccessor(&HotspotMobilityModel::m_speed),
                                          MakePointerChecker<RandomVariableStream>())
                            .AddAttribute("NormalizeProbability",
                                          "A boolean to decide to normalize probability or not.",
                                          BooleanValue(0),
                                          MakeBooleanAccessor(&HotspotMobilityModel::m_normal),
                                          MakeBooleanChecker())
                            .AddAttribute ("HotspotPositionAlloc",
                                           "The position model used to get the Hotspot Location",
                                           PointerValue (),
                                           MakePointerAccessor (&HotspotMobilityModel::m_position),
                                           MakePointerChecker<ListPositionAllocator> ())
                            .AddAttribute ("Probabilities",
                                           "A list of probabilities corresponding to each Hotspot",
                                           StringValue (),
                                           MakeStringAccessor (&HotspotMobilityModel::m_probabilities_str),
                                           MakeStringChecker ())
                            .AddAttribute ("PauseTimes",
                                           "A list of pause times corresponding to each Hotspot",
                                           StringValue (),
                                           MakeStringAccessor (&HotspotMobilityModel::m_pauses_str),
                                           MakeStringChecker());
    return tid;
  }

  void
  HotspotMobilityModel::BeginWalk(void)
  {
    m_helper.Update();
    Vector m_current = m_helper.GetCurrentPosition();

    // Randomly choose hotspot -- Cumulative Prob.
    Ptr<UniformRandomVariable> x = CreateObject<UniformRandomVariable>();
    double random_number = x->GetValue();
    double cum_probability = 0.0;
    int m_which = 0;
    for (; m_which < m_locations.size(); ++m_which)
    {
      if (m_normal)
        cum_probability += m_probabilities[m_which] / m_totalProb;
      else
        cum_probability += m_probabilities[m_which];

      if (random_number < cum_probability)
        break;
    }

    Vector destination = m_locations[m_which];
    double speed = m_speed->GetValue();
    double dx = (destination.x - m_current.x);
    double dy = (destination.y - m_current.y);
    double dz = (destination.z - m_current.z);
    double k = speed / std::sqrt(dx * dx + dy * dy + dz * dz);

    m_helper.SetVelocity(Vector(k * dx, k * dy, k * dz));
    m_helper.Unpause();
    Time travelDelay = Seconds(CalculateDistance(destination, m_current) / speed);
    m_event.Cancel();
    m_event = Simulator::Schedule(travelDelay,
                                  &HotspotMobilityModel::DoInitializePrivate, this);
    NotifyCourseChange();
  }

  void
  HotspotMobilityModel::DoInitialize(void)
  {
    NS_ASSERT_MSG (m_position, "No position allocator added before using this model");
    int m_size = m_position->GetSize();
    for(int i=0;i<m_size; i++){
      m_locations.push_back(m_position->GetNext());
    }
    NS_ASSERT_MSG((m_locations.size()), "Must add at least one hotspot!");
    NS_ASSERT_MSG(!(m_pauses_str.empty()), "No Pause times added before using this model");
    std::stringstream ss1(m_pauses_str);
    std::string s;
    int i=0;
    while(std::getline(ss1, s, ',')){
      std::cout<<i<<"th elemet is:"<<s<<"\n";
      m_pauses.push_back(std::stod(s));
      i++;
    }
    i = 0;
    NS_ASSERT_MSG (!(m_probabilities_str.empty()), "No Probabilities added before using this model");
    std::stringstream ss2(m_probabilities_str);
    while(std::getline(ss2, s, ',')){
      m_probabilities.push_back(std::stod(s));
      i++;
    }
    int m_probsize = m_probabilities.size();
    int m_pausesize = m_pauses.size();
    NS_ASSERT_MSG(((m_probsize == m_size)&(m_pausesize == m_size)), "Number of probabilities and pause times are not equal to number of hotspots");
    for(int i=0; i<m_probsize;i++){
      m_totalProb += m_probabilities[i];
    }
    DoInitializePrivate();
    MobilityModel::DoInitialize();
  }

  void
  HotspotMobilityModel::DoInitializePrivate(void)
  {
    m_helper.Update();
    m_helper.Pause();
    if (!m_normal)
      NS_ASSERT_MSG((m_totalProb == 1), "Probability must add to 1!");
    Time pause{0};
    if (!m_first)
      pause = Seconds(m_pauses[m_which]);
    else
      m_first = false;
    m_event = Simulator::Schedule(pause, &HotspotMobilityModel::BeginWalk, this);
    NotifyCourseChange();
  }

  /*void
  HotspotMobilityModel::AddHotspot(Vector location, double pause, double prob)
  {
    m_locations.push_back(location);
    m_pauses.push_back(pause);
    m_probabilities.push_back(prob);
    m_totalProb += prob;
  }

  void
  HotspotMobilityModel::ClearHotspots(void)
  {
    m_locations.clear();
    m_pauses.clear();
    m_probabilities.clear();
    m_totalProb = 0;
  }*/

  Vector
  HotspotMobilityModel::DoGetPosition(void) const
  {
    m_helper.Update();
    return m_helper.GetCurrentPosition();
  }

  void
  HotspotMobilityModel::DoSetPosition(const Vector &position)
  {
    m_helper.SetPosition(position);
    m_event.Cancel();
    m_event = Simulator::ScheduleNow(&HotspotMobilityModel::DoInitializePrivate, this);
  }

  Vector
  HotspotMobilityModel::DoGetVelocity(void) const
  {
    return m_helper.GetVelocity();
  }

} // namespace ns3
