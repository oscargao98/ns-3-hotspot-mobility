/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 *
 * Author: Oscar and Balaji
 */

#include "ns3/core-module.h"
#include "ns3/mobility-module.h"
#include <iostream>
#include <fstream>
#include <iomanip>

using namespace ns3;

NS_LOG_COMPONENT_DEFINE("6110-Final-Project");

int main(int argc, char *argv[])
{
    // Initialize Parameters
    ns3::RngSeedManager::SetSeed(42069);
    uint32_t numNodes = 100;
    uint32_t duration = 400;
    float minSpeed = 4.0;
    float maxSpeed = 6.0;
    bool normal = 0;

    // Attach Command Line Inputs
    CommandLine cmd(__FILE__);
    cmd.AddValue("numNodes", "Number of nodes in the simulation", numNodes);
    cmd.AddValue("duration", "Duration of simulation", duration);
    cmd.AddValue("minSpeed", "Min Speed", minSpeed);
    cmd.AddValue("maxSpeed", "Max Speed", maxSpeed);
    cmd.AddValue("normalProb", "normalize probability", normal);
    cmd.Parse(argc, argv);

    // Create Nodes
    NodeContainer nodes;
    nodes.Create(numNodes);

    // Set Mobility Model
    ObjectFactory pos;
    pos.SetTypeId("ns3::RandomRectanglePositionAllocator");
    pos.Set("X", StringValue("ns3::UniformRandomVariable[Min=0.0|Max=45.0]"));
    pos.Set("Y", StringValue("ns3::UniformRandomVariable[Min=0.0|Max=45.0]"));
    Ptr<PositionAllocator> PositionAlloc = pos.Create()->GetObject<PositionAllocator>();

    std::ostringstream speedInputStream;
    speedInputStream << std::fixed << std::setprecision(1) << "ns3::UniformRandomVariable[Min="
                     << minSpeed << "|Max=" << maxSpeed << "]";

    // Set Mobility Model
    MobilityHelper mobilityModel;
    mobilityModel.SetMobilityModel("ns3::HotspotMobilityModel",
                                   "Speed", StringValue(speedInputStream.str()),
                                   "NormalizeProbability", BooleanValue(normal));
    mobilityModel.SetPositionAllocator(PositionAlloc);
    mobilityModel.Install(nodes);
    // Add the same set of hotspot to every node
    for (int i{0}; i < numNodes; i++)
    {
        nodes.Get(i)->GetObject<HotspotMobilityModel>()->AddHotspot({10, 40, 0}, 15.0, 0.4);
        nodes.Get(i)->GetObject<HotspotMobilityModel>()->AddHotspot({5, 7, 0}, 15.0, 0.2);
        nodes.Get(i)->GetObject<HotspotMobilityModel>()->AddHotspot({20, 40, 0}, 15.0, 0.1);
        nodes.Get(i)->GetObject<HotspotMobilityModel>()->AddHotspot({15, 25, 0}, 15.0, 0.25);
        nodes.Get(i)->GetObject<HotspotMobilityModel>()->AddHotspot({35, 20, 0}, 15.0, 1);
        // nodes.Get(i)->GetObject<HotspotMobilityModel>()->ClearHotspots(); // DO NOT UNCOMMENT!!!
    }

    // A special node for demo
    NodeContainer Oscar;
    Oscar.Create(1);
    mobilityModel.Install(Oscar);
    Oscar.Get(0)->GetObject<HotspotMobilityModel>()->AddHotspot({0, 0, 0}, 1000, 1);

    std::cout << "Before Simulation" << '\n';
    std::ofstream preSim("pre-node-locations.data");
    for (int i{0}; i < numNodes; i++)
    {
        Vector apV = nodes.Get(i)->GetObject<MobilityModel>()->GetPosition();
        preSim << apV.x << ", " << apV.y << '\n';
    }
    Vector apV = Oscar.Get(0)->GetObject<MobilityModel>()->GetPosition();
    preSim << apV.x << ", " << apV.y;
    preSim.close();

    Simulator::Stop(Seconds(duration));
    Simulator::Run();
    std::cout << "************************" << '\n';
    std::cout << "After Simulation" << '\n';

    // Get post-simulation nodes' locations
    std::ofstream postSim("post-node-locations.data");
    for (int i{0}; i < numNodes; i++)
    {
        Vector apV = nodes.Get(i)->GetObject<MobilityModel>()->GetPosition();
        std::cout << "Node #" << (i + 1) << " "
                  << "x = " << apV.x << ", y = " << apV.y << "\n";
        postSim << apV.x << ", " << apV.y << '\n';
    }
    apV = Oscar.Get(0)->GetObject<MobilityModel>()->GetPosition();
    postSim << apV.x << ", " << apV.y;
    postSim.close();

    Simulator::Destroy();
    return 0;
}
