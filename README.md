# ns-3-hotspot-mobility

**Finished** -- Ready to Test

**Goal**: Implement a hotspot mobility model to add to ns-3.

## To Start
1. Put the ``hotspot-mobility-model.h`` and ``hotspot-mobility-model.cc`` files into ``$NS3/src/mobility/model`` directory
2. Add ``#include "hotspot-mobility-model.h"`` to ``mobility-module.h`` under ``$NS3/build/ns3``
3. Add ``'model/hotspot-mobility-model.cc'`` and ``        'model/hotspot-mobility-model.h'`` to ``wscript`` under ``$NS3/src/mobility/``

## Usage

Put ``proj.cc`` file in directory ``$NS3/sratch``, run script.

``plotter.m`` can be used to plot the before/after results.
