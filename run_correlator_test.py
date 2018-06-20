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
        pwd = os.environ['PWD']
        exec_me('ls', args.dryRun)
        exec_me('./compile.sh', args.dryRun)
        
        

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
