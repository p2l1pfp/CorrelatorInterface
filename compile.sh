#!/bin/bash
scram project CMSSW CMSSW_9_2_0
cd CMSSW_9_2_0/src
eval `scram runtime -sh`
git cms-init
git remote add cms-l1t-offline git://github.com/cms-l1t-offline/cmssw.git
git fetch cms-l1t-offline
git cms-merge-topic -u cms-l1t-offline:l1t-phase2-v1.14.1
git clone git://github.com/p2l1pfp/FastPUPPI.git -b 92X
scram b -j8
cd -