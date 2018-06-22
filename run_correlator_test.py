#!/usr/bin/env python
"""
Correlator Interface Tests
"""
# python3 print statement
from __future__ import print_function

__author__ = "Javier Duarte"
__version__ = "0.1.0"
__license__ = "MIT"


import argparse
import os


def exec_me(command,dryRun=True):
    print(command)
    if not dryRun: os.system(command)

def main(args):
    """ Main entry point of the app """

    if not os.path.exists('CMSSW_9_2_0'):
        print('Checking out and compiling CMSSW_9_2_0')
        exec_me('./compile.sh', args.dryRun)

    if not os.path.isfile('barrel_sectors_%sx%s_TTbar_PU140.dump'%(args.neta, args.nphi)):
        exec_me('cd CMSSW_9_2_0/src/; ' + \
                    'eval `scram runtime -sh`; ' + \
                    'cd -; ' + \
                    'cmsRun runRespNTupler.py neta=%s nphi=%s'%(args.neta, args.nphi), args.dryRun)
    
    if not os.path.exists('GlobalCorrelator_HLS'):
        print('Checking out GlobalCorrelator_HLS')
        exec_me('git clone git://github.com/p2l1pfp/GlobalCorrelator_HLS.git', args.dryRun)

    exec_me('cp barrel_sectors_%sx%s_TTbar_PU140.dump GlobalCorrelator_HLS/regionizer/data/'%(args.neta, args.nphi), args.dryRun)

    if not args.dryRun: os.chdir('GlobalCorrelator_HLS/regionizer')

    exec_me('. /home/jduarte1/setup_vivado.sh; ' + \
                'vivado_hls -f run_hls_regionizer.tcl', args.dryRun)    
        

if __name__ == "__main__":
    """ This is executed when run from the command line """
    parser = argparse.ArgumentParser()

    # Optional argument dry-run flag which defaults to False
    parser.add_argument("-d", "--dry-run", dest='dryRun', action="store_true", default=False)

    # Optional argument which requires a parameter (eg. -d test)
    parser.add_argument("--neta", action="store", dest="neta", default=1, type=int)
    parser.add_argument("--nphi", action="store", dest="nphi", default=6, type=int)

    args = parser.parse_args()
    main(args)
